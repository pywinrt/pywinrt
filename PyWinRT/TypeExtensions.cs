using System.Collections.Concurrent;
using System.Text;
using Mono.Cecil;

static class TypeExtensions
{
    extension(TypeDefinition type)
    {
        public bool IsExclusiveTo =>
            type.CustomAttributes.Any(a =>
                a.AttributeType.FullName == "Windows.Foundation.Metadata.ExclusiveToAttribute"
            );

        public bool IsWindowsRuntime => type.Attributes.HasFlag(TypeAttributes.WindowsRuntime);

        public bool IsDelegate => type.BaseType?.FullName == "System.MulticastDelegate";

        /// <summary>
        /// Indicates that C++/WinRT treats <paramref name="type"/> as a different
        /// struct than direct bindings.
        /// </summary>
        // FIXME: add EventRegistrationToken and HResult
        public bool IsCustomizedStruct =>
            type.FullName == "Windows.Foundation.DateTime"
            || type.FullName == "Windows.Foundation.TimeSpan";
    }

    extension(TypeDefinition type)
    {
        public bool IsStaticClass =>
            type.GetCategory() == Category.Class
            && type.Attributes.HasFlag(TypeAttributes.Abstract);

        /// <summary>
        /// The PEP 695 type parameter list a stub declares a generic type
        /// with, such as <c>[K, V]</c>, or nothing for a type that is not
        /// generic. A stub is read and not executed, so it can use the syntax
        /// whatever the runtime floor is, and the parameters are then scoped
        /// to the type rather than module-level TypeVars.
        /// </summary>
        public string PyTypeParameters =>
            type.HasGenericParameters
                ? $"[{string.Join(", ", type.GenericParameters.Select(p => p.Name))}]"
                : "";
    }

    /// <summary>
    /// Formats <paramref name="type"/> the way WinRT metadata names it, for use
    /// in error messages.
    /// </summary>
    public static string ToWinRtName(this TypeReference type) =>
        type switch
        {
            GenericParameter param => param.Name,
            GenericInstanceType generic => $"{generic.Namespace}.{generic.Name.ToNonGeneric()}"
                + $"<{string.Join(", ", generic.GenericArguments.Select(ToWinRtName))}>",
            { FullName: "System.String" } => "String",
            { FullName: "System.Object" } => "Object",
            { FullName: "System.Guid" } => "Guid",
            _ => type.FullName,
        };

    public static string GetDefaultPyValue(
        this TypeReference type,
        string ns,
        IReadOnlyDictionary<string, string> packageMap
    ) =>
        type switch
        {
            { FullName: "System.Boolean" } => "False",
            { FullName: "System.SByte" } => "0",
            { FullName: "System.Byte" } => "0",
            { FullName: "System.Char" } => "\"\"",
            { FullName: "System.Double" } => "0",
            { FullName: "System.Int16" } => "0",
            { FullName: "System.Int32" } => "0",
            { FullName: "System.Int64" } => "0",
            { FullName: "System.Single" } => "0",
            { FullName: "System.UInt16" } => "0",
            { FullName: "System.UInt32" } => "0",
            { FullName: "System.UInt64" } => "0",
            { FullName: "System.String" } => "\"\"",
            { FullName: "System.Guid" } => "_uuid.UUID(int=0)",
            { FullName: "Windows.Foundation.DateTime" } =>
                "datetime.datetime(1601, 1, 1, tzinfo=datetime.UTC)",
            { FullName: "Windows.Foundation.TimeSpan" } => "datetime.timedelta(0)",
            GenericInstanceType gen
                when gen.ElementType.FullName == "Windows.Foundation.IReference`1" => "None",
            { IsValueType: true } => type.Resolve() switch
            {
                TypeDefinition t => t switch
                {
                    { IsEnum: true } =>
                        $"{type.ToPyTypeName(ns, new TypeRefNullabilityInfo(type), packageMap)}(0)",
                    _ => $"{type.ToPyTypeName(ns, new TypeRefNullabilityInfo(type), packageMap)}()",
                },
            },
            _ => throw new NotImplementedException(),
        };

    /// <summary>
    /// Gets the default value of <paramref name="type"/> as a type stub
    /// spells it.
    /// </summary>
    /// <remarks>
    /// A stub is read and not executed, so a default that is not a literal is
    /// written as <c>...</c>, which is how a stub says that a parameter has a
    /// default without naming the value.
    /// </remarks>
    public static string GetDefaultPyValueForStub(
        this TypeReference type,
        string ns,
        IReadOnlyDictionary<string, string> packageMap
    ) =>
        type.GetDefaultPyValue(ns, packageMap) switch
        {
            var value and ("False" or "0" or "\"\"" or "None") => value,
            _ => "...",
        };

    public static string ToPyTypeName(
        this TypeReference type,
        string ns,
        TypeRefNullabilityInfo nullabilityInfo,
        IReadOnlyDictionary<string, string> packageMap,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default,
        bool quoteImportedTypes = false,
        bool usePythonCollectionTypes = true,
        bool useStructTupleUnion = false,
        bool useKeyValuePairIterMappingUnion = false,
        bool isUnpack = false,
        bool useBufferProtocol = true
    ) =>
        string.Format(
            (nullabilityInfo.AllowNull || nullabilityInfo.MaybeNull)
                // PEP 604 unions are only safe where the annotation is not
                // evaluated. quoteImportedTypes marks the one place that is
                // evaluated - the delegate type aliases in __init__.py - where
                // `"module.Type" | None` would raise TypeError at import time.
                ? quoteImportedTypes
                    ? "typing.Optional[{0}]"
                    : "{0} | None"
                : "{0}",
            type switch
            {
                GenericParameter p => map is null
                    ? type.Name
                    : map[p]
                        .ToPyTypeName(
                            ns,
                            new TypeRefNullabilityInfo(map[p]),
                            packageMap,
                            default,
                            quoteImportedTypes,
                            useBufferProtocol: useBufferProtocol
                        ),
                // a nullability rule by type; the rules by member are in
                // NullabilityRules
                GenericInstanceType gen
                    when gen.ElementType.FullName == "Windows.Foundation.IReference`1" =>
                    $"{gen.GenericArguments[0].ToPyTypeName(ns, nullabilityInfo.Args![0], packageMap, map, quoteImportedTypes, useBufferProtocol: useBufferProtocol)} | None",
                GenericInstanceType gen
                    when useKeyValuePairIterMappingUnion
                        && usePythonCollectionTypes
                        && gen.ElementType.FullName == "Windows.Foundation.Collections.IIterable`1"
                        && gen.GenericArguments[0] is GenericInstanceType gen2
                        && gen2.ElementType.FullName
                            == "Windows.Foundation.Collections.IKeyValuePair`2" =>
                    $"_cabc.Mapping[{string.Join(", ", gen2.GenericArguments.Select((p, i) => p.ToPyTypeName(ns, nullabilityInfo.Args![0].Args![i], packageMap, map, quoteImportedTypes, useBufferProtocol: useBufferProtocol)))}] | {type.ToPyTypeName(ns, nullabilityInfo, packageMap, map, quoteImportedTypes, usePythonCollectionTypes, useStructTupleUnion)}",
                GenericInstanceType gen
                    when usePythonCollectionTypes
                        && gen.ElementType.FullName
                            == "Windows.Foundation.Collections.IIterable`1" =>
                    $"_cabc.Iterable[{gen.GenericArguments[0].ToPyTypeName(ns, nullabilityInfo.Args![0], packageMap, map, quoteImportedTypes, useBufferProtocol: useBufferProtocol)}]",
                GenericInstanceType gen
                    when usePythonCollectionTypes
                        && gen.ElementType.FullName == "Windows.Foundation.Collections.IVector`1" =>
                    $"_cabc.MutableSequence[{gen.GenericArguments[0].ToPyTypeName(ns, nullabilityInfo.Args![0], packageMap, map, quoteImportedTypes, useBufferProtocol: useBufferProtocol)}]",
                GenericInstanceType gen
                    when usePythonCollectionTypes
                        && gen.ElementType.FullName
                            == "Windows.Foundation.Collections.IVectorView`1" =>
                    $"_cabc.Sequence[{gen.GenericArguments[0].ToPyTypeName(ns, nullabilityInfo.Args![0], packageMap, map, quoteImportedTypes, useBufferProtocol: useBufferProtocol)}]",
                GenericInstanceType gen
                    when usePythonCollectionTypes
                        && gen.ElementType.FullName == "Windows.Foundation.Collections.IMap`2" =>
                    $"_cabc.MutableMapping[{string.Join(", ", gen.GenericArguments.Select((p, i) => p.ToPyTypeName(ns, nullabilityInfo.Args![i], packageMap, map, quoteImportedTypes, useBufferProtocol: useBufferProtocol)))}]",
                GenericInstanceType gen
                    when usePythonCollectionTypes
                        && gen.ElementType.FullName
                            == "Windows.Foundation.Collections.IMapView`2" =>
                    $"_cabc.Mapping[{string.Join(", ", gen.GenericArguments.Select((p, i) => p.ToPyTypeName(ns, nullabilityInfo.Args![i], packageMap, map, quoteImportedTypes, useBufferProtocol: useBufferProtocol)))}]",
                GenericInstanceType gen =>
                    $"{(gen.Namespace == ns ? "" : $"{(quoteImportedTypes ? "\"" : "")}{gen.GetQualifiedNamespace(packageMap).PyModuleAlias}.")}{gen.Name.ToNonGeneric()}[{string.Join(", ", gen.GenericArguments.Select((p, i) => p.ToPyTypeName(ns, nullabilityInfo.Args![i], packageMap, map, useBufferProtocol: useBufferProtocol)))}]{(gen.Namespace != ns && quoteImportedTypes ? "\"" : "")}",
                // An [in] struct that the metadata spells `T& modopt(IsConst)`,
                // passed by reference, takes what one passed by value does.
                ByReferenceType t => t.ElementType.ToPyTypeName(
                    ns,
                    nullabilityInfo,
                    packageMap,
                    map,
                    quoteImportedTypes,
                    usePythonCollectionTypes,
                    useStructTupleUnion,
                    useKeyValuePairIterMappingUnion
                ),
                OptionalModifierType t => t.ElementType.ToPyTypeName(
                    ns,
                    nullabilityInfo,
                    packageMap,
                    map,
                    quoteImportedTypes,
                    usePythonCollectionTypes,
                    useStructTupleUnion,
                    useKeyValuePairIterMappingUnion
                ),
                ArrayType t => t.ElementType.ToPyTypeName(
                    ns,
                    nullabilityInfo,
                    packageMap,
                    map,
                    quoteImportedTypes
                ),
                { FullName: "System.Void" } => "None",
                { FullName: "System.Boolean" } => "bool",
                { FullName: "System.SByte" } => "winrt.system.Int8",
                { FullName: "System.Byte" } => "winrt.system.UInt8",
                { FullName: "System.Char" } => "winrt.system.Char16",
                { FullName: "System.Double" } => "winrt.system.Double",
                { FullName: "System.Int16" } => "winrt.system.Int16",
                { FullName: "System.Int32" } => "winrt.system.Int32",
                { FullName: "System.Int64" } => "winrt.system.Int64",
                { FullName: "System.Single" } => "winrt.system.Single",
                { FullName: "System.UInt16" } => "winrt.system.UInt16",
                { FullName: "System.UInt32" } => "winrt.system.UInt32",
                { FullName: "System.UInt64" } => "winrt.system.UInt64",
                { FullName: "System.String" } => "str",
                { FullName: "System.Guid" } => "_uuid.UUID",
                { FullName: "System.Object" } => "winrt.system.Object",
                { FullName: "Windows.Foundation.DateTime" } => "datetime.datetime",
                { FullName: "Windows.Foundation.TimeSpan" } => "datetime.timedelta",
                // Any Python buffer is taken where an IBuffer goes in, and
                // what comes out is the projected IBuffer, which is one too.
                { FullName: "Windows.Storage.Streams.IBuffer" }
                    when usePythonCollectionTypes && useBufferProtocol => "winrt.system.Buffer",
                // The integer structs are int subclasses, so neither is unpacked
                // into a tuple or accepted as one. An HRESULT is also accepted as
                // a plain int, which is what the winrt.system.hresult constants
                // are; an event token is only ever one that add_<event>() gave.
                { FullName: "Windows.Foundation.HResult" } when useStructTupleUnion =>
                    $"{type.ToPyTypeName(ns, nullabilityInfo, packageMap, map, quoteImportedTypes, usePythonCollectionTypes, useStructTupleUnion: false)} | int",
                { IsValueType: true }
                    when isUnpack
                        && !type.Resolve().IsEnum
                        && !ProjectedType.IsPyIntegerType(type.FullName) => type.ToPyTupleTyping(
                    ns,
                    packageMap,
                    quoteImportedTypes,
                    isUnpack: true
                ),
                { IsValueType: true }
                    when useStructTupleUnion
                        && !type.Resolve().IsEnum
                        && !ProjectedType.IsPyIntegerType(type.FullName) =>
                    $"{type.ToPyTypeName(ns, nullabilityInfo, packageMap, map, quoteImportedTypes, usePythonCollectionTypes, useStructTupleUnion: false)} | {type.ToPyTupleTyping(ns, packageMap, quoteImportedTypes)}",
                _ =>
                    $"{(type.Namespace == ns ? "" : $"{(quoteImportedTypes ? "\"" : "")}{type.GetQualifiedNamespace(packageMap).PyModuleAlias}.")}{type.Name.ToNonGeneric()}{(type.Namespace != ns && quoteImportedTypes ? "\"" : "")}",
            }
        );

    public static string ToPyTupleTyping(
        this TypeReference type,
        string ns,
        IReadOnlyDictionary<string, string> packageMap,
        bool quoteImportedTypes = false,
        bool isUnpack = false
    ) =>
        $"tuple[{string.Join(", ", type.Resolve().Fields.Select(f => f.FieldType.ToPyTypeName(ns, new TypeRefNullabilityInfo(f.FieldType), packageMap, default, quoteImportedTypes, useStructTupleUnion: !isUnpack, isUnpack: isUnpack)))}]";

    public static string ToPyInParamTyping(
        this ParameterDefinition param,
        string ns,
        TypeRefNullabilityInfo nullabilityInfo,
        IReadOnlyDictionary<string, string> packageMap,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default,
        bool quoteImportedTypes = false
    ) =>
        param.GetCategory() switch
        {
            ParamCategory.In => param.ParameterType.ToPyTypeName(
                ns,
                nullabilityInfo,
                packageMap,
                map,
                quoteImportedTypes,
                useStructTupleUnion: true,
                useKeyValuePairIterMappingUnion: true
            ),
            ParamCategory.PassArray =>
                $"winrt.system.Array[{param.ParameterType.ToPyTypeName(ns, nullabilityInfo, packageMap, map, quoteImportedTypes)}] | winrt.system.ReadableBuffer",
            ParamCategory.FillArray =>
                $"winrt.system.Array[{param.ParameterType.ToPyTypeName(ns, nullabilityInfo, packageMap, map, quoteImportedTypes)}] | winrt.system.WriteableBuffer",
            ParamCategory.ReceiveArray => param.ParameterType.ToPyTypeName(
                ns,
                nullabilityInfo,
                packageMap,
                map,
                quoteImportedTypes
            ),
            _ => throw new NotImplementedException(),
        };

    /// <summary>
    /// Gets Python type hint for the input parameter of a WinRT delegate.
    /// </summary>
    /// <param name="param">The parameter.</param>
    /// <param name="ns">The current namespace.</param>
    /// <param name="map">Map of generic parameters.</param>
    /// <param name="quoteImportedTypes">
    /// If types outside of the current namespace should be quoted.
    /// </param>
    /// <returns>The Python typing</returns>
    /// <remarks>
    /// In this case, input parameters are actually like output parameters in
    /// regular methods since we are passing data from C++ to Python.
    /// </remarks>
    public static string ToPyCallbackInParamTyping(
        this ParameterDefinition param,
        string ns,
        TypeRefNullabilityInfo nullabilityInfo,
        IReadOnlyDictionary<string, string> packageMap,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default,
        bool quoteImportedTypes = false
    ) =>
        param.GetCategory() switch
        {
            ParamCategory.In => param.ParameterType.ToPyTypeName(
                ns,
                nullabilityInfo,
                packageMap,
                map,
                quoteImportedTypes,
                useBufferProtocol: false
            ),
            // REVISIT: Do we need a different type hint for the winrt::array_view wrapper?
            // REVISIT: Do we want separate types for read-only arrays (PassArray)?
            ParamCategory.PassArray or ParamCategory.FillArray =>
                $"winrt.system.Array[{param.ParameterType.ToPyTypeName(ns, nullabilityInfo, packageMap, map, quoteImportedTypes)}]",
            ParamCategory.ReceiveArray => param.ParameterType.ToPyTypeName(
                ns,
                nullabilityInfo,
                packageMap,
                map,
                quoteImportedTypes
            ),
            _ => throw new NotImplementedException(),
        };

    public static string ToPyOutParamTyping(
        this ParameterDefinition param,
        string ns,
        TypeRefNullabilityInfo nullabilityInfo,
        IReadOnlyDictionary<string, string> packageMap,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default,
        bool quoteImportedTypes = false
    ) =>
        param.GetCategory() switch
        {
            ParamCategory.Out => param.ParameterType.ToPyTypeName(
                ns,
                nullabilityInfo,
                packageMap,
                map,
                quoteImportedTypes,
                useBufferProtocol: false
            ),
            ParamCategory.ReceiveArray =>
                $"winrt.system.Array[{param.ParameterType.ToPyTypeName(ns, nullabilityInfo, packageMap, map, quoteImportedTypes)}]",
            _ => throw new NotImplementedException(),
        };

    /// <summary>
    /// Gets the Python type hint for what a handler of a WinRT delegate returns.
    /// </summary>
    /// <remarks>
    /// What a handler returns is an input to the runtime, as the arguments of a
    /// call are, so each output takes what a parameter of its type takes: a
    /// tuple for a struct, a Python collection, any buffer for an array.
    /// </remarks>
    public static string ToPyCallbackReturnTyping(
        this MethodDefinition method,
        string ns,
        MethodNullabilityInfo nullabilityInfo,
        IReadOnlyDictionary<string, string> packageMap,
        bool quoteImportedTypes = false
    )
    {
        string Output(TypeReference type, TypeRefNullabilityInfo nullability, bool isArray) =>
            isArray
                ? $"winrt.system.Array[{type.ToPyTypeName(ns, nullability, packageMap, default, quoteImportedTypes)}] | winrt.system.ReadableBuffer"
                : type.ToPyTypeName(
                    ns,
                    nullability,
                    packageMap,
                    default,
                    quoteImportedTypes,
                    useStructTupleUnion: true,
                    useKeyValuePairIterMappingUnion: true
                );

        var outputs = method
            .Parameters.Where(p => p.IsPythonOutParam)
            .Select(p =>
                Output(
                    p.ParameterType,
                    nullabilityInfo.Parameters[p.Index].Type,
                    p.GetCategory() == ParamCategory.ReceiveArray
                )
            )
            .ToList();

        if (method.ReturnType.FullName != "System.Void")
        {
            outputs.Insert(
                0,
                Output(method.ReturnType, nullabilityInfo.Return.Type, method.ReturnType.IsArray)
            );
        }

        return outputs.Count switch
        {
            0 => "None",
            1 => outputs[0],
            _ => $"tuple[{string.Join(", ", outputs)}]",
        };
    }

    public static string ToPyReturnTyping(
        this MethodDefinition method,
        string ns,
        MethodNullabilityInfo nullabilityInfo,
        IReadOnlyDictionary<string, string> packageMap,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default,
        bool quoteImportedTypes = false
    )
    {
        var returnType = "None";
        var outParams = method
            .Parameters.Where(p => p.IsPythonOutParam)
            .Select(p =>
                p.ToPyOutParamTyping(
                    ns,
                    nullabilityInfo.Parameters[p.Index].Type,
                    packageMap,
                    map,
                    quoteImportedTypes
                )
            )
            .ToList();

        if (method.ReturnType.FullName != "System.Void")
        {
            if (method.ReturnType.IsArray)
            {
                outParams.Insert(
                    0,
                    $"winrt.system.Array[{method.ReturnType.ToPyTypeName(ns, nullabilityInfo.Return.Type, packageMap, map, quoteImportedTypes)}]"
                );
            }
            else
            {
                outParams.Insert(
                    0,
                    method.ReturnType.ToPyTypeName(
                        ns,
                        nullabilityInfo.Return.Type,
                        packageMap,
                        map,
                        quoteImportedTypes,
                        useBufferProtocol: false
                    )
                );
            }
        }

        if (outParams.Count == 1)
        {
            returnType = outParams[0];
        }
        else if (outParams.Count > 1)
        {
            returnType = $"tuple[{string.Join(", ", outParams)}]";
        }

        return returnType;
    }

    extension(ParameterDefinition param)
    {
        public bool IsPythonInParam =>
            param.GetCategory() switch
            {
                ParamCategory.In => true,
                ParamCategory.Out => false,
                ParamCategory.PassArray => true,
                ParamCategory.FillArray => true,
                ParamCategory.ReceiveArray => false,
                _ => throw new NotImplementedException(),
            };

        public bool IsPythonOutParam =>
            param.GetCategory() switch
            {
                ParamCategory.In => false,
                ParamCategory.Out => true,
                ParamCategory.PassArray => false,
                ParamCategory.FillArray => false,
                ParamCategory.ReceiveArray => true,
                _ => throw new NotImplementedException(),
            };
    }

    private static readonly ConcurrentDictionary<
        TypeDefinition,
        HashSet<string>
    > implementedInterfaceNames = new(
        Environment.ProcessorCount * 4,
        1 << 14,
        ReferenceEqualityComparer.Instance
    );

    /// <summary>
    /// Gets the full names of <paramref name="type"/> and all interfaces it
    /// implements, directly or indirectly.
    /// </summary>
    /// <remarks>
    /// The result is cached since this is queried many times per type.
    /// </remarks>
    private static HashSet<string> GetImplementedInterfaceNames(TypeDefinition type)
    {
        if (implementedInterfaceNames.TryGetValue(type, out var names))
        {
            return names;
        }

        names = [type.FullName];

        foreach (var iface in type.Interfaces)
        {
            names.UnionWith(GetImplementedInterfaceNames(iface.InterfaceType.Resolve()));
        }

        return implementedInterfaceNames.GetOrAdd(type, names);
    }

    /// <summary>
    /// Gets the WinRT <c>DeprecatedAttribute</c> of <paramref name="provider"/>,
    /// or <c>null</c> if it has none.
    /// </summary>
    public static CustomAttribute? GetDeprecatedAttribute(this ICustomAttributeProvider provider) =>
        provider.CustomAttributes.SingleOrDefault(a =>
            a.AttributeType.FullName == "Windows.Foundation.Metadata.DeprecatedAttribute"
        );

    public static bool ImplementsInterface(this TypeDefinition type, string interfaceName) =>
        GetImplementedInterfaceNames(type).Contains(interfaceName);

    extension(TypeDefinition type)
    {
        internal bool ImplementsIAsyncInfo =>
            type.ImplementsInterface("Windows.Foundation.IAsyncInfo");

        public bool HasFlagsAttribute =>
            type.CustomAttributes.Any(a => a.AttributeType.FullName == "System.FlagsAttribute");
    }

    static readonly IReadOnlyDictionary<string, string> CustomNumerics = new Dictionary<
        string,
        string
    >
    {
        { "Matrix3x2", "float3x2" },
        { "Matrix4x4", "float4x4" },
        { "Plane", "plane" },
        { "Quaternion", "quaternion" },
        { "Vector2", "float2" },
        { "Vector3", "float3" },
        { "Vector4", "float4" },
    };

    extension(TypeReference type)
    {
        public bool IsCustomNumeric =>
            type.Namespace == "Windows.Foundation.Numerics"
            && CustomNumerics.ContainsKey(type.Name);
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

    /// <summary>
    /// Resolves <paramref name="type"/> to a type definition or returns
    /// <c>null</c> if it cannot be resolved (e.g. fundamental types).
    /// </summary>
    public static TypeDefinition? TryResolve(this TypeReference? type)
    {
        if (type is null)
        {
            return null;
        }

        // Fast path: avoid the cost of throwing and catching exceptions for
        // unresolvable types, e.g. System.Object base types.
        if (type.Module?.MetadataResolver is MetadataResolver resolver)
        {
            return resolver.TryResolve(type);
        }

        try
        {
            return type.Resolve();
        }
        catch
        {
            return null;
        }
    }

    extension(ProjectedMethod method)
    {
        public bool IsProblematicOverride
        {
            get
            {
                if (method.Method.Name == "SetValue")
                {
                    for (
                        var baseType = method.Method.DeclaringType.BaseType;
                        baseType is not null;
                        baseType = TryResolve(baseType)?.BaseType
                    )
                    {
                        if (
                            (
                                baseType.Namespace == "Microsoft.UI.Xaml"
                                || baseType.Namespace == "Windows.UI.Xaml"
                            )
                            && baseType.Name == "DependencyObject"
                        )
                        {
                            return true;
                        }
                    }
                }

                if (method.Method.Name == "ShowAt")
                {
                    for (
                        var baseType = method.Method.DeclaringType.BaseType;
                        baseType is not null;
                        baseType = TryResolve(baseType)?.BaseType
                    )
                    {
                        if (
                            (
                                baseType.Namespace == "Microsoft.UI.Xaml.Controls.Primitives"
                                || baseType.Namespace == "Windows.UI.Xaml.Controls.Primitives"
                            )
                            && baseType.Name == "FlyoutBase"
                        )
                        {
                            return true;
                        }
                    }
                }

                return false;
            }
        }

        /// <summary>
        /// The mypy error codes that have to be silenced where this method is
        /// declared in a type stub, or <c>null</c> when there are none.
        /// </summary>
        /// <remarks>
        /// SetValue is both an incompatible override and an override of a
        /// member that the base class declares final, while ShowAt is only an
        /// incompatible override.
        /// </remarks>
        public string? OverrideIgnoreCodes =>
            method.IsProblematicOverride
                ? method.Method.Name == "SetValue"
                    ? "misc,override"
                    : "override"
                : null;
    }

    extension(TypeReference type)
    {
        /// <summary>
        /// Tests if type is one of the special interfaces that is projected as
        /// standard Python collection type.
        /// </summary>
        public bool IsPythonCollection =>
            type is GenericInstanceType gen
            && (
                gen.ElementType.FullName == "Windows.Foundation.Collections.IIterable`1"
                || gen.ElementType.FullName == "Windows.Foundation.Collections.IVector`1"
                || gen.ElementType.FullName == "Windows.Foundation.Collections.IVectorView`1"
                || gen.ElementType.FullName == "Windows.Foundation.Collections.IMap`2"
                || gen.ElementType.FullName == "Windows.Foundation.Collections.IMapView`2"
            );

        public bool IsStruct =>
            // if we can't resolve, then it is a fundamental type, so not a struct
            TryResolve(type)?.GetCategory() == Category.Struct;
    }

    private static readonly ConcurrentDictionary<
        TypeReference,
        QualifiedNamespace
    > qualifiedNamespaces = new(
        Environment.ProcessorCount * 4,
        1 << 14,
        ReferenceEqualityComparer.Instance
    );

    public static QualifiedNamespace GetQualifiedNamespace(
        this TypeReference type,
        IReadOnlyDictionary<string, string> packageMap
    )
    {
        // NB: Mono.Cecil shares TypeReference instances for the same metadata
        // row within a module, so the element type is a good cache key. Generic
        // instances are distinct objects, so we use the element type instead.
        var elementType = type.GetElementType();

        if (qualifiedNamespaces.TryGetValue(elementType, out var ns))
        {
            return ns;
        }

        ns = new(
            elementType.Namespace == "System"
                ? "winrt"
                : packageMap[elementType.Resolve().Module.Name],
            elementType.Namespace
        );

        return qualifiedNamespaces.GetOrAdd(elementType, ns);
    }
}
