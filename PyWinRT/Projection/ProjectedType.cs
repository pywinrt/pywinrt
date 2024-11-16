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
        IsStatic = type.IsStaticClass();
        IsGeneric = type.HasGenericParameters;
        IsComposable = type.CustomAttributes.Any(a =>
            a.AttributeType.FullName == "Windows.Foundation.Metadata.ComposableAttribute"
        );
        CircularDependencyDepth = Category == Category.Class ? GetCircularDependencyDepth(type) : 0;

        PyModuleName = Namespace.ToPyModuleName();
        PyExtModuleName = Namespace.ToNsModuleName();
        PyModuleAlias = Namespace.ToPyModuleAlias();

        CppNamespace = Namespace.ToCppNamespace();

        // FIXME: HResult and EventRegistrationToken should not be projected
        if (type.FullName == "Windows.Foundation.HResult")
        {
            CppWinrtType = "winrt::hresult";
        }
        else if (type.FullName == "Windows.Foundation.EventRegistrationToken")
        {
            CppWinrtType = "winrt::event_token";
        }
        else if (type.IsCustomNumeric(out var cppName))
        {
            CppWinrtType = $"winrt::{CppNamespace}::{cppName}";
        }
        else
        {
            CppWinrtType = $"winrt::{CppNamespace}::{Name}";

            if (IsGeneric)
            {
                CppWinrtType +=
                    $"<{string.Join(", ", type.GenericParameters.Select(p => p.ToCppTypeName()))}>";
            }
        }

        CppPyWrapperType = $"py::wrapper::{CppNamespace}::{Name}";
        CppPyWrapperTemplateType = IsGeneric ? $"py::proj::{CppNamespace}::{Name}" : CppWinrtType;

        PyRequiresMetaclass =
            IsComposable || type.Methods.Any(m => m.IsStatic) || type.IsCustomNumeric();

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
            // Skip types that are projected as Python protocols
            .Where(i =>
                !(
                    i is GenericInstanceType gen
                    && (
                        gen.ElementType.FullName == "Windows.Foundation.Collections.IIterable`1"
                        || gen.ElementType.FullName == "Windows.Foundation.Collections.IVector`1"
                        || gen.ElementType.FullName
                            == "Windows.Foundation.Collections.IVectorView`1"
                        || gen.ElementType.FullName == "Windows.Foundation.Collections.IMap`2"
                        || gen.ElementType.FullName == "Windows.Foundation.Collections.IMapView`2"
                    )
                )
            )
            .Where(i => !i.Resolve().IsExclusiveTo())
            .OrderBy(i => sortedInterfaces.FindIndex(s => s.FullName == i.Resolve().FullName))
            .ToArray();

        Constructors = EnumerateConstructors(type).ToArray();
        Methods = EnumerateMethods(type).ToList();
        Properties = EnumerateProperties(type).ToArray();
        Events = EnumerateEvents(type).ToArray();
    }

    private static TypeDefinition? TryResolve(TypeReference type)
    {
        try
        {
            return type?.Resolve();
        }
        catch
        {
            return null;
        }
    }

    private static int GetCircularDependencyDepth(TypeDefinition type)
    {
        var depth = 0;
        var isInOwnNamespace = true;

        for (var current = type; current != null; current = TryResolve(current.BaseType))
        {
            var wasInOwnNamespace = isInOwnNamespace;
            isInOwnNamespace = current.Namespace == type.Namespace;

            if (isInOwnNamespace && !wasInOwnNamespace)
            {
                depth++;
            }
        }

        return depth;
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
    /// Gets the dotted Python module name of the type, e.g. "winrt.windows.foundation".
    /// </summary>
    public string PyModuleName { get; }

    /// <summary>
    /// Gets the Python extension module name of the type, e.g. "_winrt_windows_foundation".
    /// </summary>
    public string PyExtModuleName { get; }

    /// <summary>
    /// Gets the Python module alias of the type, e.g. "windows_foundation".
    /// </summary>
    public string PyModuleAlias { get; }

    /// <summary>
    /// Gets the C++ namespace of the type, e.g. "Windows::Foundation".
    /// </summary>
    public string CppNamespace { get; }

    /// <summary>
    /// Gets the C++ type name, e.g. "winrt::Windows::Foundation::IIterable".
    /// </summary>
    public string CppWinrtType { get; }

    /// <summary>
    /// Gets the C++ wrapper type name, e.g. "py::wrapper::windows::foundation::IIterable".
    /// </summary>
    public string CppPyWrapperType { get; }

    /// <summary>
    /// Gets the C++ wrapper template type name, e.g. "py::proj::windows::foundation::IIterable"
    /// or <see cref="CppWinrtType"/> for non-generic types.
    /// </summary>
    public string CppPyWrapperTemplateType { get; }

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
    public IReadOnlyCollection<ProjectedMethod> Methods { get; }

    /// <summary>
    /// Gets the properties of the type.
    /// </summary>
    public IReadOnlyCollection<ProjectedProperty> Properties { get; }

    /// <summary>
    /// Gets the events of the type.
    /// </summary>
    public IReadOnlyCollection<ProjectedEvent> Events { get; }

    public string GetMethodInvokeContext(MethodDefinition method)
    {
        if (IsGeneric)
        {
            return "_obj.";
        }

        if (method.IsStatic || method.IsConstructor)
        {
            return $"{CppWinrtType}::";
        }

        if (IsComposable)
        {
            return $"self->obj.try_as<{CppWinrtType}>().";
        }

        // HACK: work around https://github.com/microsoft/cppwinrt/issues/1287
        // so far, this is the only case in the entire Windows SDK where
        // a property is entirely replaced with one of the same name
        if (
            Namespace == "Windows.UI.Xaml.Controls.Maps"
            && Name == "MapControl"
            && method.Name == "get_Style"
        )
        {
            return "static_cast<winrt::Windows::UI::Xaml::Controls::Maps::IMapControl>(self->obj).";
        }

        return "self->obj.";
    }

    public string GetMethodSelfParam(bool isStatic) =>
        isStatic ? "PyObject* /*unused*/" : $"{CppPyWrapperType}* self";

    private static IEnumerable<ProjectedMethod> EnumerateConstructors(TypeDefinition type)
    {
        return type
            .Methods.Where(m => m.IsConstructor && m.IsPublic)
            .Select(m => new ProjectedMethod(m, [type], default));
    }

    private static IEnumerable<ProjectedMethod> EnumerateMethods(TypeDefinition type)
    {
        var collectedMethods = new SortedDictionary<
            string,
            SortedDictionary<int, List<ProjectedMethod>>
        >(StringComparer.Ordinal);

        void add(
            IEnumerable<MethodDefinition> methods,
            IEnumerable<TypeReference> inheritance,
            IReadOnlyDictionary<GenericParameter, TypeReference>? map
        )
        {
            foreach (var method in methods.Where(m => !m.IsSpecialName && m.IsPublic))
            {
                var projected = new ProjectedMethod(method, inheritance, map);

                if (!collectedMethods.TryGetValue(projected.Name, out var dict))
                {
                    dict = [];
                    collectedMethods[projected.Name] = dict;
                }

                if (!dict.TryGetValue(method.Parameters.Count, out var list))
                {
                    list = [];
                    dict[method.Parameters.Count] = list;
                }

                list.Add(projected);
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

                if (resolvedType.IsExclusiveTo())
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

        foreach (var (name, methods) in collectedMethods)
        {
            foreach (var (argCount, overloads) in methods)
            {
                // if there are multiple overloads with the same number of
                // arguments, we need to use the default overload
                // https://devblogs.microsoft.com/oldnewthing/20210528-00/?p=105259
                var defaultOverload = overloads.FirstOrDefault(m => m.IsDefaultOverload);

                if (defaultOverload is not null)
                {
                    yield return defaultOverload;
                    continue;
                }

                // if there was no default, just use the first (and
                // hopefully only) overload

                yield return overloads.OrderBy(o => o.Inheritance.Count).First();
            }
        }
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

                if (resolvedType.IsExclusiveTo())
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

                if (resolvedType.IsExclusiveTo())
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
