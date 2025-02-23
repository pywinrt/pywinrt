using Mono.Cecil;

/// <summary>
/// Members of a WinRT namespace.
/// </summary>
sealed class Members
{
    /// <summary>
    /// Enumeration types.
    /// </summary>
    public IReadOnlyCollection<ProjectedType> Enums { get; }

    /// <summary>
    /// Structure types.
    /// </summary>
    public IReadOnlyCollection<ProjectedType> Structs { get; }

    /// <summary>
    /// Interface types.
    /// </summary>
    public IReadOnlyCollection<ProjectedType> Interfaces { get; }

    /// <summary>
    /// Runtime class types.
    /// </summary>
    public IReadOnlyCollection<ProjectedType> Classes { get; }

    /// <summary>
    /// Delegate types.
    /// </summary>
    public IReadOnlyCollection<ProjectedType> Delegates { get; }

    /// <summary>
    /// Attribute types.
    /// </summary>
    public IReadOnlyCollection<ProjectedType> Attributes { get; }

    /// <summary>
    /// Contract types.
    /// </summary>
    public IReadOnlyCollection<ProjectedType> Contracts { get; }

    /// <summary>
    /// Initializes a new instance of the <see cref="Members"/> class.
    /// </summary>
    /// <param name="typeDefinitions">Type definitions to categorize.</param>
    public Members(IEnumerable<TypeDefinition> typeDefinitions)
    {
        var enums = new List<TypeDefinition>();
        var structs = new List<TypeDefinition>();
        var interfaces = new List<TypeDefinition>();
        var classes = new List<TypeDefinition>();
        var delegates = new List<TypeDefinition>();
        var attributes = new List<TypeDefinition>();
        var contracts = new List<TypeDefinition>();

        foreach (var type in typeDefinitions.OrderBy(t => t.FullName, StringComparer.Ordinal))
        {
            switch (type.GetCategory())
            {
                case Category.Enum:
                    enums.Add(type);
                    break;
                case Category.Struct:
                    if (
                        type.CustomAttributes.Any(a =>
                            a.AttributeType.FullName
                            == "Windows.Foundation.Metadata.ApiContractAttribute"
                        )
                    )
                    {
                        contracts.Add(type);
                    }
                    else
                    {
                        structs.Add(type);
                    }
                    break;
                case Category.Interface:
                    interfaces.Add(type);
                    break;
                case Category.Class:
                    if (type.BaseType?.FullName == "System.Attribute")
                    {
                        attributes.Add(type);
                    }
                    else
                    {
                        classes.Add(type);
                    }
                    break;
                case Category.Delegate:
                    delegates.Add(type);
                    break;
            }
        }

        Enums = enums.Select(e => new ProjectedType(e)).ToArray();
        Structs = structs.Select(s => new ProjectedType(s)).ToArray();
        Interfaces = interfaces.Select(i => new ProjectedType(i)).ToArray();
        Classes = classes.Select(c => new ProjectedType(c)).ToArray();
        Delegates = delegates.Select(d => new ProjectedType(d)).ToArray();
        Attributes = attributes.Select(a => new ProjectedType(a)).ToArray();
        Contracts = contracts.Select(c => new ProjectedType(c)).ToArray();
    }

    private static IEnumerable<TypeReference> RecursiveGetTypes(TypeReference type)
    {
        yield return type;

        if (type is GenericInstanceType gen)
        {
            foreach (var t in gen.GenericArguments)
            {
                foreach (var tt in RecursiveGetTypes(t))
                {
                    yield return tt;
                }
            }
        }
    }

    public IEnumerable<QualifiedNamespace> GetRequiredNamespaces(
        IReadOnlyDictionary<string, string> packageMap
    )
    {
        var namespaces = new SortedSet<QualifiedNamespace>();

        foreach (var type in Classes)
        {
            if (
                type.Type.BaseType is TypeReference baseType
                && baseType.Namespace != type.Namespace
                && baseType.Namespace != "System"
            )
            {
                namespaces.Add(baseType.GetQualifiedNamespace(packageMap));
            }
        }

        return namespaces;
    }

    public IEnumerable<QualifiedNamespace> GetReferencedNamespaces(
        IReadOnlyDictionary<string, string> packageMap,
        bool includeDelegates = false,
        bool includeInheritedInterfaces = false
    )
    {
        var namespaces = new SortedSet<QualifiedNamespace>();

        foreach (
            var type in Structs.SelectMany(s =>
                s.Type.Fields.Select(f => f.FieldType)
                    .SelectMany(RecursiveGetTypes)
                    .Where(t =>
                        !string.IsNullOrEmpty(t.Namespace)
                        && t.Namespace != "System"
                        && t.Namespace != s.Namespace
                    )
            )
        )
        {
            namespaces.Add(type.GetQualifiedNamespace(packageMap));
        }

        foreach (var type in Classes.Concat(Interfaces))
        {
            if (
                type.Type.BaseType is TypeReference baseType
                && baseType.Namespace != type.Namespace
                && baseType.Namespace != "System"
            )
            {
                namespaces.Add(baseType.GetQualifiedNamespace(packageMap));
            }

            if (includeInheritedInterfaces)
            {
                foreach (var t in type.Interfaces.Where(t => t.Namespace != type.Namespace))
                {
                    namespaces.Add(t.GetQualifiedNamespace(packageMap));
                }
            }

            foreach (
                var method in type
                    .Constructors.Select(c => c.Method)
                    .Concat(type.Methods.Select(m => m.Method))
                    .Concat(
                        type.Properties.SelectMany(p =>
                            (IEnumerable<MethodDefinition>)(
                                p.Property.SetMethod is null
                                    ? [p.Property.GetMethod]
                                    : [p.Property.GetMethod, p.Property.SetMethod]
                            )
                        )
                    )
                    .Concat(
                        type.Events.SelectMany(e =>
                            (IEnumerable<MethodDefinition>)[e.Event.AddMethod, e.Event.RemoveMethod]
                        )
                    )
            )
            {
                foreach (
                    var t in method
                        .Parameters.Select(p => p.ParameterType)
                        .Append(method.ReturnType)
                        .SelectMany(RecursiveGetTypes)
                        .Where(t =>
                            !string.IsNullOrEmpty(t.Namespace)
                            && t.Namespace != "System"
                            && t.Namespace != type.Namespace
                        )
                )
                {
                    namespaces.Add(t.GetQualifiedNamespace(packageMap));
                }
            }
        }

        if (includeDelegates)
        {
            foreach (var m in Delegates.Select(d => d.Type.Methods.Single(m => m.Name == "Invoke")))
            {
                foreach (
                    var t in m
                        .Parameters.Select(p => p.ParameterType)
                        .Append(m.ReturnType)
                        .SelectMany(RecursiveGetTypes)
                        .Where(t =>
                            !string.IsNullOrEmpty(t.Namespace)
                            && t.Namespace != "System"
                            && t.Namespace != m.DeclaringType.Namespace
                        )
                )
                {
                    namespaces.Add(t.GetQualifiedNamespace(packageMap));
                }
            }
        }

        return namespaces;
    }
}
