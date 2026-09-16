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

        Enums = Project(enums);
        Structs = Project(structs);
        Interfaces = Project(interfaces);
        Classes = Project(classes);
        Delegates = Project(delegates);
        Attributes = Project(attributes);
        Contracts = Project(contracts);
    }

    private static ProjectedType[] Project(List<TypeDefinition> types)
    {
        // Analyzing a type is relatively expensive and independent of other
        // types, so for large namespaces, do it in parallel. This helps the
        // largest namespaces, which would otherwise be the last ones still
        // running at the end.
        if (types.Count < 32)
        {
            return types.Select(t => new ProjectedType(t)).ToArray();
        }

        return types.AsParallel().AsOrdered().Select(t => new ProjectedType(t)).ToArray();
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

        return namespaces;
    }

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

    private IReadOnlyList<GenericInstanceType>? genericInstancesCache;

    /// <summary>
    /// Gets every closed parameterized interface or delegate instance (e.g.
    /// <c>IAsyncOperation&lt;StorageFile&gt;</c>) that appears in the member
    /// signatures and struct fields of this namespace, including the nested
    /// arguments of other instances, sorted by C++ type name.
    /// </summary>
    /// <remarks>
    /// Interfaces that a type only implements (rather than mentions in a
    /// signature) are left out. Adding them was measured and made no
    /// difference to compile time, because the few instantiations it saves
    /// are paid back by the larger generated header.
    ///
    /// These are the types whose GUIDs C++/WinRT would otherwise compute with
    /// a constexpr SHA-1 in every module that uses them, so the generated
    /// GUID header spells them out. The related instances that the runtime
    /// wrappers use (iterators, completed handlers, key-value pairs, ...) are
    /// included. Instances that mention <c>System.Type</c> are left out
    /// because C++/WinRT maps that to a different struct.
    /// </remarks>
    public IReadOnlyList<GenericInstanceType> GetGenericInstances()
    {
        return genericInstancesCache ??= ComputeGenericInstances();
    }

    private IReadOnlyList<GenericInstanceType> ComputeGenericInstances()
    {
        var instances = new SortedDictionary<string, GenericInstanceType>(StringComparer.Ordinal);

        void Collect(TypeReference type)
        {
            while (
                type is ByReferenceType or ArrayType or OptionalModifierType or RequiredModifierType
            )
            {
                type = ((TypeSpecification)type).ElementType;
            }

            if (type is not GenericInstanceType gen || !IsClosed(gen))
            {
                return;
            }

            if (!instances.TryAdd(gen.ToCppTypeName(), gen))
            {
                return;
            }

            foreach (var arg in gen.GenericArguments)
            {
                Collect(arg);
            }

            // The runtime wrappers in pywinrt/base.h and the generated headers use
            // these related instances to implement the collection and async
            // interfaces, so they are needed too.
            foreach (var related in WinRtGuid.GetRelatedInstances(gen))
            {
                Collect(related);
            }
        }

        var methods = Classes
            .Concat(Interfaces)
            .SelectMany(GetMemberMethods)
            .Concat(Delegates.Select(d => d.Type.Methods.Single(m => m.Name == "Invoke")));

        foreach (var method in methods)
        {
            foreach (
                var t in method.Parameters.Select(p => p.ParameterType).Append(method.ReturnType)
            )
            {
                Collect(t);
            }
        }

        foreach (var field in Structs.SelectMany(s => s.Type.Fields.Where(f => !f.IsStatic)))
        {
            Collect(field.FieldType);
        }

        return instances.Values.ToList();
    }

    private static bool IsClosed(TypeReference type) =>
        type switch
        {
            GenericParameter => false,
            { FullName: "System.Type" } => false,
            GenericInstanceType gen => gen.GenericArguments.All(IsClosed),
            TypeSpecification spec => IsClosed(spec.ElementType),
            _ => true,
        };

    private IReadOnlyCollection<QualifiedNamespace>? fullHeaderNamespacesCache;

    /// <summary>
    /// Gets the other namespaces whose full generated header (and therefore
    /// full C++/WinRT header) the generated code of this namespace needs,
    /// as opposed to just the light <c>py.*.types.h</c> header.
    /// </summary>
    /// <remarks>
    /// Wrapping and unwrapping an object of a foreign type only needs its
    /// Python type name and the C++/WinRT declarations (<c>impl/*.2.h</c>)
    /// that the C++/WinRT header of this namespace already includes. The
    /// exceptions are delegates, whose converting constructor is defined in
    /// the full C++/WinRT header and whose Python wrapper is in the full
    /// generated header; generic interfaces and delegates, whose Python
    /// wrapper templates are in the full generated header (in practice only
    /// Windows.Foundation and Windows.Foundation.Collections, which are
    /// included by pywinrt/base.h anyway); and the interfaces that declare the
    /// members being called (e.g. a required interface from another
    /// namespace), whose C++/WinRT consume methods are defined in the full
    /// C++/WinRT header; and, for composable classes, the base classes whose
    /// overridable interfaces the C++/WinRT "FooT" template implements.
    /// </remarks>
    public IReadOnlyCollection<QualifiedNamespace> GetFullHeaderNamespaces(
        IReadOnlyDictionary<string, string> packageMap
    )
    {
        return fullHeaderNamespacesCache ??= ComputeFullHeaderNamespaces(packageMap);
    }

    private IReadOnlyCollection<QualifiedNamespace> ComputeFullHeaderNamespaces(
        IReadOnlyDictionary<string, string> packageMap
    )
    {
        var namespaces = new SortedSet<QualifiedNamespace>();

        foreach (var type in Classes.Concat(Interfaces))
        {
            foreach (
                var t in GetForeignSignatureTypes(type)
                    .Where(t =>
                        t is GenericInstanceType
                        || (t.Resolve() is TypeDefinition def && def.IsDelegate())
                    )
            )
            {
                namespaces.Add(t.GetQualifiedNamespace(packageMap));
            }

            // A runtime class redeclares the methods of its interfaces, so the
            // interface that actually declares (and whose C++/WinRT header
            // implements) a class method is the one it overrides.
            foreach (
                var t in GetMemberMethods(type)
                    .Select(m => m.HasOverrides ? m.Overrides[0].DeclaringType : m.DeclaringType)
                    .Where(t => t.Namespace != "System" && t.Namespace != type.Namespace)
            )
            {
                namespaces.Add(t.GetQualifiedNamespace(packageMap));
            }

            // The C++/WinRT "FooT<D>" template of a composable class implements
            // the overridable interfaces of every base class, and the produce<>
            // specializations for those live in the full header of the
            // namespace of each base class (and of its interfaces).
            if (type.IsComposable)
            {
                for (
                    var baseTypeRef = type.Type.BaseType;
                    baseTypeRef is not null && baseTypeRef.Namespace != "System";
                    baseTypeRef = baseTypeRef.Resolve().BaseType
                )
                {
                    var baseType = baseTypeRef.Resolve();

                    foreach (
                        var t in baseType
                            .Interfaces.Select(i => i.InterfaceType)
                            .Prepend(baseType)
                            .Where(t => t.Namespace != "System" && t.Namespace != type.Namespace)
                    )
                    {
                        namespaces.Add(t.GetQualifiedNamespace(packageMap));
                    }
                }
            }
        }

        return namespaces;
    }
}
