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
    /// <param name="legacyMethodAliases">
    /// Whether methods get a deprecated alias under their pywinrt v3.x name.
    /// </param>
    /// <param name="renames">
    /// The names the projection published and has since renamed.
    /// </param>
    public Members(
        IEnumerable<TypeDefinition> typeDefinitions,
        bool legacyMethodAliases,
        Renames renames
    )
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

        Enums = Project(enums, legacyMethodAliases, renames);
        Structs = Project(structs, legacyMethodAliases, renames);
        Interfaces = Project(interfaces, legacyMethodAliases, renames);
        Classes = Project(classes, legacyMethodAliases, renames);
        Delegates = Project(delegates, legacyMethodAliases, renames);
        Attributes = Project(attributes, legacyMethodAliases, renames);
        Contracts = Project(contracts, legacyMethodAliases, renames);
    }

    private static ProjectedType[] Project(
        List<TypeDefinition> types,
        bool legacyMethodAliases,
        Renames renames
    )
    {
        // Analyzing a type is relatively expensive and independent of other
        // types, so for large namespaces, do it in parallel. This helps the
        // largest namespaces, which would otherwise be the last ones still
        // running at the end.
        if (types.Count < 32)
        {
            return types.Select(t => new ProjectedType(t, legacyMethodAliases, renames)).ToArray();
        }

        return types
            .AsParallel()
            .AsOrdered()
            .Select(t => new ProjectedType(t, legacyMethodAliases, renames))
            .ToArray();
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

    public IReadOnlyCollection<QualifiedNamespace> GetRequiredNamespaces(
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

        // A struct field of a struct is resolved when the struct's type is
        // built, which imports the package that defines it. A struct only
        // referenced through a generic, such as IReference<T>, is resolved on
        // first use instead.
        foreach (var s in Structs)
        {
            foreach (var fieldType in s.Type.Fields.Select(f => f.FieldType))
            {
                if (
                    fieldType.Namespace != s.Namespace
                    && fieldType.Namespace != "System"
                    && IsNestedStruct(fieldType)
                )
                {
                    namespaces.Add(fieldType.GetQualifiedNamespace(packageMap));
                }
            }
        }

        return namespaces;
    }

    /// <summary>
    /// Indicates that a field of type <paramref name="type"/> holds a struct
    /// the runtime resolves to its own type, rather than one it stores as a
    /// value of its own, like DateTime or HResult.
    /// </summary>
    private static bool IsNestedStruct(TypeReference type) =>
        type.FullName
            is not (
                "Windows.Foundation.DateTime"
                or "Windows.Foundation.TimeSpan"
                or "Windows.Foundation.HResult"
                or "Windows.Foundation.EventRegistrationToken"
            )
        && type.TryResolve()?.GetCategory() == Category.Struct;

    private readonly IReadOnlyCollection<QualifiedNamespace>?[] referencedNamespacesCache =
        new IReadOnlyCollection<QualifiedNamespace>?[4];

    public IReadOnlyCollection<QualifiedNamespace> GetReferencedNamespaces(
        IReadOnlyDictionary<string, string> packageMap,
        bool includeDelegates = false,
        bool includeInheritedInterfaces = false
    )
    {
        // This is called several times per namespace with the same arguments
        // (once for each generated file), so the result is cached.
        var cacheIndex = (includeDelegates ? 1 : 0) | (includeInheritedInterfaces ? 2 : 0);

        return referencedNamespacesCache[cacheIndex] ??= ComputeReferencedNamespaces(
            packageMap,
            includeDelegates,
            includeInheritedInterfaces
        );
    }

    private IReadOnlyCollection<QualifiedNamespace> ComputeReferencedNamespaces(
        IReadOnlyDictionary<string, string> packageMap,
        bool includeDelegates,
        bool includeInheritedInterfaces
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

            foreach (var t in GetForeignSignatureTypes(type))
            {
                namespaces.Add(t.GetQualifiedNamespace(packageMap));
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

    /// <summary>
    /// Gets every type from another namespace that appears in a member
    /// signature (constructor, method, property or event) of <paramref name="type"/>,
    /// including the arguments of generic instances.
    /// </summary>
    private static IEnumerable<TypeReference> GetForeignSignatureTypes(ProjectedType type)
    {
        return GetMemberMethods(type)
            .SelectMany(m => m.Parameters.Select(p => p.ParameterType).Append(m.ReturnType))
            .SelectMany(RecursiveGetTypes)
            .Where(t =>
                !string.IsNullOrEmpty(t.Namespace)
                && t.Namespace != "System"
                && t.Namespace != type.Namespace
            );
    }

    /// <summary>
    /// Gets the methods behind every member (constructor, method, property or
    /// event) of <paramref name="type"/> that the generated code calls.
    /// </summary>
    private static IEnumerable<MethodDefinition> GetMemberMethods(ProjectedType type)
    {
        return type
            .Constructors.Select(c => c.Method)
            .Concat(type.Methods.Select(m => m.Method))
            .Concat(
                type.Properties.SelectMany(p =>
                    (IEnumerable<MethodDefinition>)(
                        p.SetMethod is null
                            ? [p.GetMethod.Method]
                            : [p.GetMethod.Method, p.SetMethod.Method]
                    )
                )
            )
            .Concat(
                type.Events.SelectMany(e =>
                    (IEnumerable<MethodDefinition>)[e.Event.AddMethod, e.Event.RemoveMethod]
                )
            );
    }
}
