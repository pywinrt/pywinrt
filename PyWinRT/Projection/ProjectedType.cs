using Mono.Cecil;

class ProjectedType
{
    // TODO: eventually, this should be private
    public readonly TypeDefinition Type;

    public ProjectedType(TypeDefinition type)
    {
        Type = type;

        Namespace = type.Namespace;
        Name = type.Name.ToNonGeneric();
        Category = type.GetCategory();
        IsStatic = type.IsStaticClass;
        IsGeneric = type.HasGenericParameters;
        IsComposable = type.CustomAttributes.Any(a =>
            a.AttributeType.FullName == "Windows.Foundation.Metadata.ComposableAttribute"
        );
        CircularDependencyDepth =
            Category == Category.Class ? type.GetCircularDependencyDepth() : 0;

        PyWrapperTypeName = Category == Category.Interface ? $"_{Name}" : Name;

        PyRequiresMetaclass =
            IsComposable || type.Methods.Any(m => m.IsStatic) || type.IsCustomNumeric;

        IsPyIterator = type.ImplementsInterface("Windows.Foundation.Collections.IIterator`1");
        IsPyIterable =
            IsPyIterator || type.ImplementsInterface("Windows.Foundation.Collections.IIterable`1");
        IsPyMutableSequence = type.ImplementsInterface("Windows.Foundation.Collections.IVector`1");
        IsPySequence =
            IsPyMutableSequence
            || type.ImplementsInterface("Windows.Foundation.Collections.IVectorView`1");
        IsPyMutableMapping = type.ImplementsInterface("Windows.Foundation.Collections.IMap`2");
        IsPyMapping =
            IsPyMutableMapping
            || type.ImplementsInterface("Windows.Foundation.Collections.IMapView`2");
        IsPyAwaitable =
            type.ImplementsInterface("Windows.Foundation.IAsyncInfo")
            && type.FullName != "Windows.Foundation.IAsyncInfo";
        IsPyCloseable = type.ImplementsInterface("Windows.Foundation.IClosable");
        IsPyStringable = type.ImplementsInterface("Windows.Foundation.IStringable");

        var iMemoryReference = type.ImplementsInterface(
            "Windows.Foundation.IMemoryBufferReference"
        );
        var iBuffer = type.ImplementsInterface("Windows.Storage.Streams.IBuffer");

        IsPyBuffer = iMemoryReference || iBuffer;
        PyBufferSize = iBuffer ? "Length" : "Capacity";

        var sortedInterfaces = DependencySort(
            type.Interfaces.Select(i => i.InterfaceType.Resolve())
        );
        Interfaces = type
            .Interfaces.Select(i => i.InterfaceType)
            .Where(i => !i.Resolve().IsExclusiveTo)
            .OrderBy(i => sortedInterfaces.FindIndex(s => s.FullName == i.Resolve().FullName))
            .ToArray();

        var factories = Factories = GetFactories(type);
        Constructors = EnumerateConstructors(type).ToArray();
        Properties = EnumerateProperties(type).ToArray();
        Events = EnumerateEvents(type).ToArray();
        MethodGroups = EnumerateMethodGroups(type, Properties, Events, GetReservedPyNames());
        Methods = MethodGroups.SelectMany(g => g.Overloads).ToList();

        HasComposableFactory = factories.Values.Any(f =>
            f.IsComposable && f.Type?.Methods.Count > 0
        );
    }

    private enum Mark
    {
        Unmarked,
        Temporary,
        Permanent,
    }

    private sealed class TypeComparer : IEqualityComparer<TypeDefinition>
    {
        public bool Equals(TypeDefinition? x, TypeDefinition? y) => x?.FullName == y?.FullName;

        public int GetHashCode(TypeDefinition obj) => obj.FullName.GetHashCode();
    }

    private static List<TypeDefinition> DependencySort(IEnumerable<TypeDefinition> interfaces)
    {
        // depth-first search
        var marked = interfaces.ToDictionary(i => i, _ => Mark.Unmarked, new TypeComparer());
        var sorted = new List<TypeDefinition>();

        void visit(TypeDefinition type)
        {
            marked.TryAdd(type, Mark.Unmarked);

            switch (marked[type])
            {
                case Mark.Permanent:
                    return;
                case Mark.Temporary:
                    throw new InvalidOperationException("Cyclic dependency detected");
            }

            marked[type] = Mark.Temporary;

            foreach (var iface in type.Interfaces)
            {
                visit(iface.InterfaceType.Resolve());
            }

            marked[type] = Mark.Permanent;
            sorted.Add(type);
        }

        while (marked.Values.Any(m => m != Mark.Permanent))
        {
            visit(marked.First(m => m.Value == Mark.Unmarked).Key);
        }

        sorted.Reverse();

        return sorted;
    }

    /// <summary>
    /// Gets the C#-style namespace of the type, e.g. "Windows.Foundation".
    /// </summary>
    public string Namespace { get; }

    /// <summary>
    /// Gets the non-generic name of the type, e.g. "IIterable".
    /// </summary>
    public string Name { get; }

    /// <summary>
    /// Gets the category of the type.
    /// </summary>
    public Category Category { get; }

    /// <summary>
    /// True if the type is a static class.
    /// </summary>
    public bool IsStatic { get; }

    /// <summary>
    /// True if the type is a generic type.
    /// </summary>
    public bool IsGeneric { get; }

    /// <summary>
    /// True if the type is a composable WinRT runtime class.
    /// </summary>
    public bool IsComposable { get; }

    /// <summary>
    /// Gets the depth of circular dependencies, if any.
    /// </summary>
    /// <remarks>
    /// A value of 0 indicates no circular dependencies. A value of 1 indicates
    /// that this type depends on a type in another namespace that depends on
    /// a type in this namespace.
    /// </remarks>
    public int CircularDependencyDepth { get; }

    /// <summary>
    /// Gets the Python type name for the wrapper class.
    /// </summary>
    /// <remarks>
    /// In the case of interfaces, this is not the same as <see cref="Name"/>.
    /// </remarks>
    public string PyWrapperTypeName { get; }

    /// <summary>
    /// True if the type requires a Python metaclass, e.g. for static members.
    /// </summary>
    public bool PyRequiresMetaclass { get; }

    /// <summary>
    /// True if the type implements Windows.Foundation.Collections.IIterable`1.
    /// </summary>
    public bool IsPyIterable { get; }

    /// <summary>
    /// True if the type implements Windows.Foundation.Collections.IIterator`1.
    /// </summary>
    public bool IsPyIterator { get; }

    /// <summary>
    /// True if the type implements Windows.Foundation.Collections.IVectorView`1.
    /// </summary>
    public bool IsPySequence { get; }

    /// <summary>
    /// True if the type implements Windows.Foundation.Collections.IVector`1.
    /// </summary>
    public bool IsPyMutableSequence { get; }

    /// <summary>
    /// True if the type implements Windows.Foundation.Collections.IMapView`2.
    /// </summary>
    public bool IsPyMapping { get; }

    /// <summary>
    /// True if the type implements Windows.Foundation.Collections.IMap`2.
    /// </summary>
    public bool IsPyMutableMapping { get; }

    /// <summary>
    /// True if the type implements Windows.Foundation.IAsyncInfo.
    /// </summary>
    public bool IsPyAwaitable { get; }

    /// <summary>
    /// True if the type implements Windows.Foundation.IClosable.
    /// </summary>
    public bool IsPyCloseable { get; }

    /// <summary>
    /// True if the type implements Windows.Foundation.IStringable.
    /// </summary>
    public bool IsPyStringable { get; }

    /// <summary>
    /// True if the type implements Windows.Storage.Streams.IBuffer or Windows.Storage.Streams.IMemoryBufferReference.
    /// </summary>
    public bool IsPyBuffer { get; }

    /// <summary>
    /// Gets the name of the property that returns the buffer size, e.g. "Length".
    /// </summary>
    /// <remarks>
    /// Only valid if <see cref="IsPyBuffer"/> is true.
    /// </remarks>
    public string PyBufferSize { get; }

    /// <summary>
    /// Gets the interfaces implemented by the type.
    /// </summary>
    public IReadOnlyCollection<TypeReference> Interfaces { get; }

    /// <summary>
    /// Gets the constructors of the type.
    /// </summary>
    public IReadOnlyCollection<ProjectedMethod> Constructors { get; }

    /// <summary>
    /// Gets the methods of the type.
    /// </summary>
    /// <remarks>
    /// This is the flattened list of all overloads of all method groups.
    /// </remarks>
    public IReadOnlyCollection<ProjectedMethod> Methods { get; }

    /// <summary>
    /// Gets the methods of the type, grouped by projected name.
    /// </summary>
    /// <remarks>
    /// Each group is projected as a single Python method that dispatches to
    /// one of the overloads based on the number of arguments.
    /// </remarks>
    public IReadOnlyList<ProjectedMethodGroup> MethodGroups { get; }

    /// <summary>
    /// Gets the properties of the type.
    /// </summary>
    public IReadOnlyCollection<ProjectedProperty> Properties { get; }

    /// <summary>
    /// Gets the events of the type.
    /// </summary>
    public IReadOnlyCollection<ProjectedEvent> Events { get; }

    /// <summary>
    /// True if the type has any factory methods for creating instances of a composable type.
    /// </summary>
    public bool HasComposableFactory { get; }

    /// <summary>
    /// Gets the activation, static and composition factories of the type, keyed
    /// by the full name of the factory interface. The key is the empty string
    /// for plain activation, which goes through
    /// <c>IActivationFactory::ActivateInstance</c> and has no interface of its
    /// own.
    /// </summary>
    public IReadOnlyDictionary<string, FactoryInfo> Factories { get; }

    /// <summary>
    /// Gets the interface that declares <paramref name="method"/>, which is the
    /// interface an instance has to be queried for before the call.
    /// </summary>
    /// <remarks>
    /// C++/WinRT gave a runtime class a base class of its default interface and
    /// reached every other interface with an implicit conversion that yields
    /// null when the object does not implement it, which the call then
    /// dereferences. The table says what declares a member and the runtime
    /// queries for it, which turns that crash into a Python exception and
    /// answers the question the call actually depends on.
    ///
    /// A static member and a constructor are declared by a factory interface
    /// instead, which <see cref="GetFactoryMethod"/> finds.
    /// </remarks>
    public TypeReference? GetDeclaringInterface(ProjectedMethod method)
    {
        if (method.IsStatic || method.IsConstructor)
        {
            return GetFactoryMethod(method)?.DeclaringType;
        }

        var declaring = method.Method.HasOverrides
            // a runtime class redeclares the members of its interfaces, so the
            // interface that declares one is the interface that it overrides
            ? method.Method.Overrides[0].DeclaringType
            : ResolveDeclaringInterface(method.Inheritance);

        if (declaring is not null)
        {
            return declaring;
        }

        // the member is declared by the type being projected, which for an
        // interface is that interface and for a class is its default interface
        return Category == Category.Interface ? Type : DefaultInterface;
    }

    /// <summary>
    /// Gets the method of the factory interface behind a static member or a
    /// constructor, or <c>null</c> when it is plain activation, which goes
    /// through <c>IActivationFactory::ActivateInstance</c>.
    /// </summary>
    /// <remarks>
    /// A runtime class redeclares the members of its factories the way it
    /// redeclares the members of its interfaces, but without the
    /// <c>.override</c> that says which one, so the factory is found by
    /// matching the signature.
    /// </remarks>
    public MethodDefinition? GetFactoryMethod(ProjectedMethod method)
    {
        foreach (var factory in Factories.Values)
        {
            if (factory.Type is null)
            {
                continue;
            }

            if (method.IsConstructor != (factory.IsActivatable || factory.IsComposable))
            {
                continue;
            }

            foreach (var candidate in factory.Type.Methods)
            {
                if (IsFactoryMethodFor(candidate, method))
                {
                    return candidate;
                }
            }
        }

        return null;
    }

    /// <summary>
    /// Tests whether <paramref name="candidate"/> is the factory method behind
    /// <paramref name="method"/>.
    /// </summary>
    /// <remarks>
    /// A composition factory method takes two more parameters than the
    /// constructor it implements - the outer object and the non-delegating inner
    /// - so only the parameters the constructor declares are compared.
    /// </remarks>
    private static bool IsFactoryMethodFor(MethodDefinition candidate, ProjectedMethod method)
    {
        if (!method.IsConstructor && candidate.Name != method.Method.Name)
        {
            return false;
        }

        var declared = method.Method.Parameters;

        if (candidate.Parameters.Count < declared.Count)
        {
            return false;
        }

        for (var i = 0; i < declared.Count; i++)
        {
            if (
                candidate.Parameters[i].ParameterType.FullName != declared[i].ParameterType.FullName
            )
            {
                return false;
            }
        }

        // a static member also has to agree on what it returns, since a factory
        // interface can declare several members with the same parameters
        return method.IsConstructor
            || candidate.ReturnType.FullName == method.Method.ReturnType.FullName;
    }

    /// <summary>
    /// Gets the default interface of the type or <c>null</c> if it does not have
    /// one.
    /// </summary>
    public TypeReference? DefaultInterface =>
        defaultInterface ??= Type
            .Interfaces.FirstOrDefault(i =>
                i.CustomAttributes.Any(a =>
                    a.AttributeType.FullName == "Windows.Foundation.Metadata.DefaultAttribute"
                )
            )
            ?.InterfaceType;

    private TypeReference? defaultInterface;

    /// <summary>
    /// Resolves the interface at the end of <paramref name="inheritance"/> to a
    /// type that can be named in C++, or <c>null</c> if the chain does not leave
    /// the type being projected.
    /// </summary>
    /// <remarks>
    /// Each step of the chain states its type arguments in terms of the generic
    /// parameters of the step before it, e.g. <c>IMap&lt;K, V&gt;</c> requires
    /// <c>IIterable&lt;IKeyValuePair&lt;K, V&gt;&gt;</c>, so the arguments have
    /// to be substituted one step at a time to arrive at a complete type.
    /// </remarks>
    private static TypeReference? ResolveDeclaringInterface(
        IReadOnlyList<TypeReference> inheritance
    )
    {
        var map = new Dictionary<GenericParameter, TypeReference>();
        var resolved = default(TypeReference);

        foreach (var type in inheritance.Skip(1))
        {
            resolved = SubstituteGenericArgs(type, map);

            map = resolved is GenericInstanceType generic
                ? generic
                    .ElementType.Resolve()
                    .GenericParameters.Zip(generic.GenericArguments)
                    .ToDictionary()
                : [];
        }

        return resolved;
    }

    /// <summary>
    /// Replaces every generic parameter in <paramref name="type"/> that
    /// <paramref name="map"/> names with the type argument it stands for.
    /// </summary>
    public static TypeReference SubstituteGenericArgs(
        TypeReference type,
        IReadOnlyDictionary<GenericParameter, TypeReference> map
    )
    {
        switch (type)
        {
            case GenericParameter param:
                return map.TryGetValue(param, out var arg) ? arg : param;
            case GenericInstanceType generic:
                var substituted = new GenericInstanceType(generic.ElementType);

                foreach (var argument in generic.GenericArguments)
                {
                    substituted.GenericArguments.Add(SubstituteGenericArgs(argument, map));
                }

                return substituted;
            default:
                return type;
        }
    }

    /// <summary>
    /// Gets the projected method with the given WinRT name and number of
    /// Python arguments.
    /// </summary>
    /// <remarks>
    /// This is for the well-known methods of well-known interfaces. A type can
    /// have other overloads of the same method, so the number of arguments is
    /// needed to select the right one.
    /// </remarks>
    public ProjectedMethod GetMethod(string baseName, int pyInParamCount)
    {
        var methods = Methods
            .Where(m => m.BaseName == baseName && m.PyInParamCount == pyInParamCount)
            .ToList();

        if (methods.Count != 1)
        {
            throw new InvalidOperationException(
                $"expected exactly one {baseName} method with {pyInParamCount} arguments "
                    + $"on {Type.FullName}, found {string.Join(", ", methods.Select(m => m.Signature))}"
            );
        }

        return methods[0];
    }

    private static IEnumerable<ProjectedMethod> EnumerateConstructors(TypeDefinition type)
    {
        return type
            .Methods.Where(m => m.IsConstructor && m.IsPublic)
            .Select(m => new ProjectedMethod(m, [type], default));
    }

    public readonly record struct FactoryInfo(
        TypeDefinition? Type,
        bool IsActivatable,
        bool IsStatic,
        bool IsComposable,
        bool IsVisible
    ) { }

    private static IReadOnlyDictionary<string, FactoryInfo> GetFactories(TypeDefinition type)
    {
        var factories = new Dictionary<string, FactoryInfo>();

        // NB: Only the factory attributes are of interest here. Reading the
        // constructor arguments of every metadata attribute (e.g. the
        // ContractVersionAttribute that is on everything) is relatively
        // expensive and the information is not used.
        foreach (
            var attr in type.CustomAttributes.Where(a =>
                a.AttributeType.Namespace == "Windows.Foundation.Metadata"
                && a.AttributeType.Name
                    is "ActivatableAttribute"
                        or "StaticAttribute"
                        or "ComposableAttribute"
            )
        )
        {
            var constructorType = attr.ConstructorArguments.FirstOrDefault(a =>
                a.Type.FullName == "System.Type"
            );
            var infoType = ((TypeReference)constructorType.Value)?.Resolve();

            var info = new FactoryInfo(
                infoType,
                attr.AttributeType.Name == "ActivatableAttribute",
                attr.AttributeType.Name == "StaticAttribute",
                attr.AttributeType.Name == "ComposableAttribute",
                false // not implemented yet
            );

            factories[info.Type?.FullName ?? ""] = info;
        }

        return factories;
    }

    /// <summary>
    /// Gets Python attribute names that are added to the type outside of the
    /// method groups and therefore must not be used by a deprecated alias.
    /// </summary>
    private IEnumerable<string> GetReservedPyNames()
    {
        if (IsPySequence)
        {
            yield return "index";
            yield return "count";
        }

        if (IsPyMutableSequence)
        {
            yield return "insert";
            yield return "append";
            yield return "clear";
            yield return "extend";
            yield return "reverse";
            yield return "pop";
            yield return "remove";
        }

        if (IsPyMapping)
        {
            yield return "keys";
            yield return "items";
            yield return "values";
            yield return "get";
        }

        if (IsPyMutableMapping)
        {
            yield return "clear";
            yield return "pop";
            yield return "popitem";
            yield return "setdefault";
            yield return "update";
        }

        if (IsPyAwaitable)
        {
            yield return "get";
            yield return "wait";
        }
    }

    private readonly record struct MethodGroupKey(bool IsStatic, bool IsPublic, string Name);

    private sealed class MethodGroupKeyComparer : IComparer<MethodGroupKey>
    {
        public static readonly MethodGroupKeyComparer Instance = new();

        public int Compare(MethodGroupKey x, MethodGroupKey y)
        {
            var result = string.CompareOrdinal(x.Name, y.Name);

            if (result != 0)
            {
                return result;
            }

            result = x.IsStatic.CompareTo(y.IsStatic);

            if (result != 0)
            {
                return result;
            }

            return x.IsPublic.CompareTo(y.IsPublic);
        }
    }

    /// <summary>
    /// Tests if two projected methods are the same WinRT method reached by way
    /// of different types.
    /// </summary>
    private static bool IsSameMethod(ProjectedMethod x, ProjectedMethod y) =>
        x.Method == y.Method
        // two methods that need separate Python names always have separate
        // Overload attribute names
        || (x.OverloadName is not null && x.OverloadName == y.OverloadName)
        || IsOverrideOf(x.Method, y.Method)
        || IsOverrideOf(y.Method, x.Method);

    /// <summary>
    /// Tests if the only name a method can be projected as is the one it
    /// currently has, i.e. it has no Overload attribute to fall back on.
    /// </summary>
    private static bool CanOnlyUseThisName(ProjectedMethod method) =>
        method.OverloadName is null || method.OverloadName == method.Name;

    /// <summary>
    /// Tests if two methods take the same parameters and have the same return
    /// type, i.e. calling one is the same as calling the other.
    /// </summary>
    private static bool HasSameSignature(ProjectedMethod x, ProjectedMethod y) =>
        x.Method.ReturnType.FullName == y.Method.ReturnType.FullName
        && x.Method.Parameters.Select(p => p.ParameterType.FullName)
            .SequenceEqual(y.Method.Parameters.Select(p => p.ParameterType.FullName));

    private static bool IsOverrideOf(MethodDefinition method, MethodDefinition other) =>
        method.Overrides.Any(o =>
            o.Name == other.Name
            && o.Parameters.Count == other.Parameters.Count
            && (
                o.DeclaringType is GenericInstanceType generic
                    ? generic.ElementType
                    : o.DeclaringType
            ).FullName == other.DeclaringType.FullName
        );

    private static IReadOnlyList<ProjectedMethodGroup> EnumerateMethodGroups(
        TypeDefinition type,
        IEnumerable<ProjectedProperty> properties,
        IEnumerable<ProjectedEvent> events,
        IEnumerable<string> reservedPyNames
    )
    {
        var collected = new SortedDictionary<
            MethodGroupKey,
            SortedDictionary<int, List<ProjectedMethod>>
        >(MethodGroupKeyComparer.Instance);

        void collect(ProjectedMethod projected)
        {
            var key = new MethodGroupKey(projected.IsStatic, projected.IsPublic, projected.Name);

            if (!collected.TryGetValue(key, out var buckets))
            {
                buckets = [];
                collected[key] = buckets;
            }

            if (!buckets.TryGetValue(projected.PyInParamCount, out var candidates))
            {
                candidates = [];
                buckets[projected.PyInParamCount] = candidates;
            }

            candidates.Add(projected);
        }

        void add(
            IEnumerable<MethodDefinition> methods,
            IEnumerable<TypeReference> inheritance,
            IReadOnlyDictionary<GenericParameter, TypeReference>? map
        )
        {
            foreach (var method in methods.Where(m => !m.IsSpecialName && !m.IsPrivate))
            {
                var projected = new ProjectedMethod(method, inheritance, map);

                // Overridable methods are called from WinRT into Python, where
                // methods can't be overloaded, so each overload has to keep a
                // name of its own instead of being called by argument count.
                if (projected.IsOverridable && projected.OverloadName is not null)
                {
                    projected = new ProjectedMethod(
                        method,
                        inheritance,
                        map,
                        projected.OverloadName
                    );
                }

                collect(projected);
            }
        }

        var inheritance = new Stack<TypeReference>([type]);

        add(type.Methods, inheritance, default);

        var visited = new HashSet<string>();

        void addInterfaces(TypeDefinition parent)
        {
            foreach (var iface in parent.Interfaces)
            {
                if (!visited.Add(iface.InterfaceType.FullName))
                {
                    continue;
                }

                var resolvedType = iface.InterfaceType.Resolve();

                if (resolvedType.IsExclusiveTo)
                {
                    continue;
                }

                var genericMap = default(Dictionary<GenericParameter, TypeReference>);

                if (iface.InterfaceType is GenericInstanceType gen)
                {
                    genericMap = resolvedType
                        .GenericParameters.Zip(gen.GenericArguments)
                        .ToDictionary();
                }

                inheritance.Push(iface.InterfaceType);
                add(resolvedType.Methods, inheritance.Reverse(), genericMap);
                addInterfaces(resolvedType);
                inheritance.Pop();
            }
        }

        addInterfaces(type);

        // WinRT overloads methods by the number of arguments, so there can be
        // only one method per name and argument count. An overload that loses
        // out is projected using the name from its Overload attribute instead,
        // since that is the only way it can be called.

        var resolved = new SortedDictionary<MethodGroupKey, SortedDictionary<int, ProjectedMethod>>(
            MethodGroupKeyComparer.Instance
        );
        var unreachable = new List<(ProjectedMethod Method, ProjectedMethod Winner)>();

        while (collected.Count > 0)
        {
            var pending = collected;
            collected = new(MethodGroupKeyComparer.Instance);

            foreach (var (key, buckets) in pending)
            {
                if (!resolved.TryGetValue(key, out var methods))
                {
                    methods = [];
                    resolved[key] = methods;
                }

                foreach (var (argCount, candidates) in buckets)
                {
                    var contenders = new List<ProjectedMethod>();

                    // a method that was resolved in an earlier pass can be
                    // displaced by a method that has no other name
                    if (methods.TryGetValue(argCount, out var previous))
                    {
                        contenders.Add(previous);
                    }

                    contenders.AddRange(candidates);

                    // a method that has no name of its own can only be called
                    // by this name, so it wins over methods that can also be
                    // projected using the name from their Overload attribute
                    var stuck = contenders.Where(CanOnlyUseThisName).ToList();
                    var pool = stuck.Count > 0 ? stuck : contenders;

                    // if there are multiple overloads with the same number
                    // of arguments, we need to use the default overload
                    // https://devblogs.microsoft.com/oldnewthing/20210528-00/?p=105259
                    var winner =
                        pool.FirstOrDefault(m => m.IsDefaultOverload)
                        // if there was no default, use the one that is
                        // closest to the type being projected
                        ?? pool.OrderBy(m => m.Inheritance.Count).First();

                    methods[argCount] = winner;

                    foreach (var candidate in contenders)
                    {
                        if (candidate == winner || IsSameMethod(candidate, winner))
                        {
                            continue;
                        }

                        if (!CanOnlyUseThisName(candidate))
                        {
                            collect(
                                new ProjectedMethod(
                                    candidate.Method,
                                    candidate.Inheritance,
                                    candidate.GenericArgMap,
                                    candidate.OverloadName
                                )
                            );

                            continue;
                        }

                        unreachable.Add((candidate, winner));
                    }
                }
            }
        }

        // A method that lost out to another method with the same signature is
        // not worth reporting since the same API is still available, e.g. when
        // a newer version of an interface repeats a method of an older one.

        foreach (var (method, winner) in unreachable)
        {
            var key = new MethodGroupKey(method.IsStatic, method.IsPublic, method.Name);

            if (
                resolved.TryGetValue(key, out var methods)
                && methods.TryGetValue(method.PyInParamCount, out var other)
                && HasSameSignature(method, other)
            )
            {
                continue;
            }

            Console.Error.WriteLine(
                $"warning: {type.FullName}: {method.Signature} is not callable "
                    + $"because it is shadowed by {winner.Signature}"
            );
        }

        // Methods that were renamed when the Overload attribute started being
        // used for method names in pywinrt v3.0 get a deprecated alias so that
        // code written for that version keeps working. A real attribute of the
        // type always wins over an alias.
        //
        // NB: This is transitional. When the aliases have been deprecated long
        // enough to be removed, delete everything that goes with them: the code
        // below and the Aliases of ProjectedMethodGroup, the alias_method() and
        // alias_static_method() calls in the generated __init__.py, the
        // @deprecated defs in the generated type stubs, the LegacyPyName of
        // ProjectedMethod and the lookup of the old name in
        // WriteGetPythonMethod(), and the helpers in winrt.runtime._internals.
        // The warning about an old name that can't be kept goes away with them.

        var reserved = new Dictionary<bool, HashSet<string>>
        {
            [false] = new(StringComparer.Ordinal),
            [true] = new(StringComparer.Ordinal),
        };

        foreach (var name in reservedPyNames)
        {
            reserved[false].Add(name);
        }

        foreach (var property in properties)
        {
            reserved[property.IsStatic]
                .Add(property.Name.ToPythonIdentifier(isTypeMethod: property.IsStatic));
        }

        foreach (var @event in events)
        {
            reserved[@event.IsStatic].Add(@event.AddMethod.PyName);
            reserved[@event.IsStatic].Add(@event.RemoveMethod.PyName);
        }

        var byPyName = new Dictionary<(bool IsStatic, string PyName), List<ProjectedMethod>>();

        foreach (var (key, methods) in resolved)
        {
            var pyName = methods.Values.First().PyName;

            reserved[key.IsStatic].Add(pyName);

            if (!byPyName.TryGetValue((key.IsStatic, pyName), out var list))
            {
                list = [];
                byPyName[(key.IsStatic, pyName)] = list;
            }

            list.AddRange(methods.Values);
        }

        var groups = new List<ProjectedMethodGroup>(resolved.Count);

        foreach (var (key, methods) in resolved)
        {
            var overloads = methods.Values.ToList();
            var pyName = overloads[0].PyName;
            var aliases = new List<MethodAlias>();

            // more than one overload can have had the same old name, in which
            // case the alias is overloaded as well
            foreach (
                var legacy in overloads
                    .Where(m => m.LegacyPyName != pyName)
                    .GroupBy(m => m.LegacyPyName)
            )
            {
                if (!reserved[key.IsStatic].Add(legacy.Key))
                {
                    // if the old name is a method that does the same thing,
                    // e.g. the same method of a newer version of an interface,
                    // then code that used the old name still works
                    var existing = byPyName.GetValueOrDefault((key.IsStatic, legacy.Key), []);

                    if (!legacy.All(m => existing.Any(e => HasSameSignature(m, e))))
                    {
                        Console.Error.WriteLine(
                            $"warning: {type.FullName}: {legacy.First().Signature} can no longer "
                                + $"be called as {legacy.Key}(), use {pyName}() instead"
                        );
                    }

                    continue;
                }

                aliases.Add(new MethodAlias(legacy.Key, [.. legacy]));
            }

            aliases.Sort((x, y) => string.CompareOrdinal(x.PyName, y.PyName));

            groups.Add(new ProjectedMethodGroup(overloads, aliases));
        }

        return groups;
    }

    private static IEnumerable<ProjectedProperty> EnumerateProperties(TypeDefinition type)
    {
        var collectedProperties = new List<ProjectedProperty>();

        void add(
            IEnumerable<PropertyDefinition> properties,
            IReadOnlyList<TypeReference> inheritance,
            IReadOnlyDictionary<GenericParameter, TypeReference>? map
        )
        {
            foreach (var property in properties.Where(p => p.GetMethod?.IsPublic ?? false))
            {
                // FIXME: replace with sorted set
                if (collectedProperties.Any(p => p.Name == property.Name))
                {
                    continue;
                }

                collectedProperties.Add(new ProjectedProperty(property, inheritance, map));
            }
        }

        var inheritance = new Stack<TypeReference>([type]);

        add(type.Properties, [.. inheritance], default);

        var visited = new HashSet<string>();

        void addInterfaces(TypeDefinition parent)
        {
            foreach (var iface in parent.Interfaces)
            {
                if (!visited.Add(iface.InterfaceType.FullName))
                {
                    continue;
                }

                var resolvedType = iface.InterfaceType.Resolve();

                if (resolvedType.IsExclusiveTo)
                {
                    continue;
                }

                var genericMap = default(Dictionary<GenericParameter, TypeReference>);

                if (iface.InterfaceType is GenericInstanceType gen)
                {
                    genericMap = resolvedType
                        .GenericParameters.Zip(gen.GenericArguments)
                        .ToDictionary();
                }

                inheritance.Push(iface.InterfaceType);
                add(resolvedType.Properties, inheritance.Reverse().ToList(), genericMap);
                addInterfaces(resolvedType);
                inheritance.Pop();
            }
        }

        addInterfaces(type);

        return collectedProperties;
    }

    private static IEnumerable<ProjectedEvent> EnumerateEvents(TypeDefinition type)
    {
        var collectedEvents = new List<ProjectedEvent>();

        void add(
            IEnumerable<EventDefinition> events,
            IReadOnlyList<TypeReference> inheritance,
            IReadOnlyDictionary<GenericParameter, TypeReference>? map
        )
        {
            foreach (var @event in events.Where(p => p.AddMethod.IsPublic))
            {
                // FIXME: replace with sorted set
                if (collectedEvents.Any(p => p.Name == @event.Name))
                {
                    continue;
                }

                collectedEvents.Add(new ProjectedEvent(@event, inheritance, map));
            }
        }

        var inheritance = new Stack<TypeReference>([type]);

        add(type.Events, [.. inheritance], default);

        var visited = new HashSet<string>();

        void addInterfaces(TypeDefinition parent)
        {
            foreach (var iface in parent.Interfaces)
            {
                if (!visited.Add(iface.InterfaceType.FullName))
                {
                    continue;
                }

                var resolvedType = iface.InterfaceType.Resolve();

                if (resolvedType.IsExclusiveTo)
                {
                    continue;
                }

                var genericMap = default(Dictionary<GenericParameter, TypeReference>);

                if (iface.InterfaceType is GenericInstanceType gen)
                {
                    genericMap = resolvedType
                        .GenericParameters.Zip(gen.GenericArguments)
                        .ToDictionary();
                }

                inheritance.Push(iface.InterfaceType);
                add(resolvedType.Events, inheritance.Reverse().ToList(), genericMap);
                addInterfaces(resolvedType);
                inheritance.Pop();
            }
        }

        addInterfaces(type);

        return collectedEvents;
    }
}
