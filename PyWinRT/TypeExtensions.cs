using System.Diagnostics.CodeAnalysis;
using System.Text;
using Mono.Cecil;
using Mono.Collections.Generic;

static class TypeExtensions
{
    public static bool IsExclusiveTo(this TypeDefinition type)
    {
        return type.CustomAttributes.Any(a =>
            a.AttributeType.FullName == "Windows.Foundation.Metadata.ExclusiveToAttribute"
        );
    }

    public static bool IsWindowsRuntime(this TypeDefinition type)
    {
        return type.Attributes.HasFlag(TypeAttributes.WindowsRuntime);
    }

    public static bool IsDelegate(this TypeDefinition type)
    {
        return type.BaseType?.FullName == "System.MulticastDelegate";
    }

    /// <summary>
    /// Indicates that C++/WinRT treats <paramref name="type"/> as a different
    /// struct than direct bindings.
    /// </summary>
    public static bool IsCustomizedStruct(this TypeDefinition type)
    {
        // FIXME: add EventRegistrationToken and HResult
        return type.FullName == "Windows.Foundation.DateTime"
            || type.FullName == "Windows.Foundation.TimeSpan";
    }

    public static string ToStructFieldType(this TypeReference type) =>
        type switch
        {
            GenericInstanceType gen
                => gen.ElementType.FullName switch
                {
                    "Windows.Foundation.IReference`1" => "PyObject*",
                    _ => throw new NotImplementedException(),
                },
            // NB: Boolean needs to be int for Python compatibility, C++ bool is wrong size
            { FullName: "System.Boolean" } => "int",
            { FullName: "System.SByte" } => "int8_t",
            { FullName: "System.Byte" } => "uint8_t",
            { FullName: "System.Int16" } => "int16_t",
            { FullName: "System.UInt16" } => "uint16_t",
            { FullName: "System.Int32" } => "int32_t",
            { FullName: "System.UInt32" } => "uint32_t",
            { FullName: "System.Int64" } => "int64_t",
            { FullName: "System.UInt64" } => "uint64_t",
            { FullName: "System.Single" } => "float",
            { FullName: "System.Double" } => "double",
            { FullName: "System.Char" }
            or { FullName: "System.String" }
            or { FullName: "System.Guid" }
                => "PyObject*",
            { IsValueType: true }
                => type.Resolve() switch
                {
                    TypeDefinition t
                        => t switch
                        {
                            { IsEnum: true } => t.HasFlagsAttribute() ? "uint32_t" : "int32_t",
                            _ => $"PyObject*"
                        }
                },
            _ => throw new NotImplementedException(),
        };

    public static string ToWinrtFieldName(this FieldDefinition field)
    {
        if (
            (
                field.DeclaringType.Namespace == "Windows.Foundation"
                && (
                    field.DeclaringType.Name == "HResult"
                    || field.DeclaringType.Name == "EventRegistrationToken"
                )
            ) || field.DeclaringType.IsCustomNumeric()
        )
        {
            return field.Name.ToLowerInvariant();
        }

        return field.Name;
    }

    public static string ToStructFieldKeywordList(this TypeDefinition type)
    {
        var sb = new StringBuilder();

        foreach (var field in type.Fields)
        {
            sb.Append($"\"{field.Name.ToPythonIdentifier()}\", ");
        }

        return sb.ToString();
    }

    public static string ToStructBufferFormat(this FieldDefinition field)
    {
        return field.FieldType switch
        {
            GenericInstanceType gen
                => gen.ElementType.FullName switch
                {
                    "Windows.Foundation.IReference`1" => "P",
                    _ => throw new NotImplementedException(),
                },
            { FullName: "System.Boolean" } => "?",
            { FullName: "System.Char" } => "u",
            { FullName: "System.SByte" } => "b",
            { FullName: "System.Byte" } => "B",
            { FullName: "System.Int16" } => "h",
            { FullName: "System.UInt16" } => "H",
            { FullName: "System.Int32" } => "i",
            { FullName: "System.UInt32" } => "I",
            { FullName: "System.Int64" } => "q",
            { FullName: "System.UInt64" } => "Q",
            { FullName: "System.Single" } => "f",
            { FullName: "System.Double" } => "d",
            { FullName: "System.String" } => "P",
            { FullName: "System.Guid" } => "T{I2H8B}",
            { IsValueType: true }
                => field.FieldType.Resolve() switch
                {
                    TypeDefinition t
                        => t switch
                        {
                            { IsEnum: true } => t.HasFlagsAttribute() ? "I" : "i",
                            _
                                => $"T{{{ string.Join(
            "",
            t.Fields.Select(f =>
                $"{f.ToStructBufferFormat()}:{f.Name.ToPythonIdentifier()}:"
            )
        )}}}",
                        }
                },
            _ => throw new NotImplementedException(),
        };
    }

    public static string ToStructFieldFormat(this TypeDefinition type)
    {
        var sb = new StringBuilder();

        foreach (var field in type.Fields)
        {
            sb.Append(
                field.FieldType switch
                {
                    GenericInstanceType gen
                        => gen.ElementType.FullName switch
                        {
                            "Windows.Foundation.IReference`1" => "O",
                            _ => throw new NotImplementedException(),
                        },
                    { FullName: "System.Boolean" } => "p",
                    { FullName: "System.SByte" } => "b",
                    { FullName: "System.Byte" } => "B",
                    { FullName: "System.Int16" } => "h",
                    { FullName: "System.UInt16" } => "H",
                    { FullName: "System.Int32" } => "i",
                    { FullName: "System.UInt32" } => "I",
                    { FullName: "System.Int64" } => "L",
                    { FullName: "System.UInt64" } => "K",
                    { FullName: "System.Single" } => "f",
                    { FullName: "System.Double" } => "d",
                    { FullName: "System.Char" }
                    or { FullName: "System.String" }
                    or { FullName: "System.Guid" }
                        => "O",
                    { IsValueType: true }
                        => field.FieldType.Resolve() switch
                        {
                            TypeDefinition t
                                => t switch
                                {
                                    { IsEnum: true } => t.HasFlagsAttribute() ? "I" : "i",
                                    _ => "O"
                                }
                        },
                    _ => throw new NotImplementedException(),
                }
            );
        }

        return sb.ToString();
    }

    public static string ToStructFieldParseParameterList(this TypeDefinition type)
    {
        var sb = new StringBuilder();

        foreach (var field in type.Fields)
        {
            sb.Append($", &_{field.Name}");
        }

        return sb.ToString();
    }

    public static string ToStructFieldInitializer(this FieldDefinition field)
    {
        return field.FieldType switch
        {
            GenericInstanceType gen
                => gen.ElementType.FullName switch
                {
                    "Windows.Foundation.IReference`1"
                        => $"py::converter<{field.FieldType.ToCppTypeName()}>::convert_to(_{field.Name})",
                    _ => throw new NotImplementedException(),
                },
            { FullName: "System.Boolean" }
            or { FullName: "System.SByte" }
            or { FullName: "System.Byte" }
            or { FullName: "System.Int16" }
            or { FullName: "System.UInt16" }
            or { FullName: "System.Int32" }
            or { FullName: "System.UInt32" }
            or { FullName: "System.Int64" }
            or { FullName: "System.UInt64" }
            or { FullName: "System.Single" }
            or { FullName: "System.Double" }
                => $"_{field.Name}",
            { FullName: "System.Char" }
            or { FullName: "System.String" }
            or { FullName: "System.Guid" }
                => $"py::converter<{field.FieldType.ToCppTypeName()}>::convert_to(_{field.Name})",
            { IsValueType: true }
                => field.FieldType.Resolve() switch
                {
                    { IsEnum: true }
                        => $"static_cast<{field.FieldType.ToCppTypeName()}>(_{field.Name})",
                    _
                        => $"py::converter<{field.FieldType.ToCppTypeName()}>::convert_to(_{field.Name})"
                },
            _ => throw new NotImplementedException(),
        };
    }

    public static bool IsStaticClass(this TypeDefinition type) =>
        type.GetCategory() == Category.Class && type.Attributes.HasFlag(TypeAttributes.Abstract);

    public static string ToParameterList(this Collection<ParameterDefinition> parameters) =>
        string.Join(", ", parameters.Select(ToParamName));

    public static string ToParamName(this ParameterDefinition param) => $"param{param.Index}";

    public static string ToCppTypeName(
        this TypeReference type,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default
    ) =>
        type switch
        {
            GenericParameter p => map is null ? type.Name : map[p].ToCppTypeName(),
            GenericInstanceType gen
                => $"winrt::{gen.Namespace.ToCppNamespace()}::{gen.Name.ToNonGeneric()}<{string.Join(", ", gen.GenericArguments.Select(p => p.ToCppTypeName(map)))}>",
            ByReferenceType byRef => byRef.ElementType.ToCppTypeName(map),
            OptionalModifierType opt => opt.ElementType.ToCppTypeName(map),
            ArrayType t => t.ElementType.ToCppTypeName(map),
            { FullName: "System.Boolean" } => "bool",
            { FullName: "System.SByte" } => "int8_t",
            { FullName: "System.Byte" } => "uint8_t",
            { FullName: "System.Char" } => "char16_t",
            { FullName: "System.Double" } => "double",
            { FullName: "System.Int16" } => "int16_t",
            { FullName: "System.Int32" } => "int32_t",
            { FullName: "System.Int64" } => "int64_t",
            { FullName: "System.Single" } => "float",
            { FullName: "System.UInt16" } => "uint16_t",
            { FullName: "System.UInt32" } => "uint32_t",
            { FullName: "System.UInt64" } => "uint64_t",
            { FullName: "System.String" } => "winrt::hstring",
            { FullName: "System.Guid" } => "winrt::guid",
            { FullName: "System.Object" } => "winrt::Windows::Foundation::IInspectable",
            { FullName: "Windows.Foundation.EventRegistrationToken" } => "winrt::event_token",
            { FullName: "Windows.Foundation.HResult" } => "winrt::hresult",
            { Namespace: "Windows.Foundation.Numerics" } when type.IsCustomNumeric(out var cppName)
                => $"winrt::{type.Namespace.ToCppNamespace()}::{cppName}",
            { HasGenericParameters: true }
                => $"winrt::{type.Namespace.ToCppNamespace()}::{type.Name.ToNonGeneric()}<{string.Join(", ", type.GenericParameters.Select(p => p.ToCppTypeName(map)))}>",
            _ => $"winrt::{type.FullName.ToCppNamespace()}"
        };

    public static string GetDefaultPyValue(this TypeReference type, string ns) =>
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
            { FullName: "Windows.Foundation.DateTime" }
                => "datetime.datetime(1601, 1, 1, tzinfo=datetime.timezone.utc)",
            { FullName: "Windows.Foundation.TimeSpan" } => "datetime.timedelta(0)",
            GenericInstanceType gen
                when gen.ElementType.FullName == "Windows.Foundation.IReference`1"
                => "None",
            { IsValueType: true }
                => type.Resolve() switch
                {
                    TypeDefinition t
                        => t switch
                        {
                            { IsEnum: true } => $"{type.ToPyTypeName(ns)}(0)",
                            _ => $"{type.ToPyTypeName(ns)}()",
                        }
                },
            _ => throw new NotImplementedException(),
        };

    public static string ToPyTypeName(
        this TypeReference type,
        string ns,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default,
        bool quoteImportedTypes = false
    ) =>
        type switch
        {
            GenericParameter p
                => map is null ? type.Name : map[p].ToPyTypeName(ns, default, quoteImportedTypes),
            GenericInstanceType gen
                when gen.ElementType.FullName == "Windows.Foundation.IReference`1"
                => $"typing.Optional[{string.Join(", ", gen.GenericArguments.Select(p => p.ToPyTypeName(ns, map, quoteImportedTypes)))}]",
            GenericInstanceType gen
                when gen.ElementType.FullName == "Windows.Foundation.Collections.IVector`1"
                => $"typing.MutableSequence[{gen.GenericArguments[0].ToPyTypeName(ns, map, quoteImportedTypes)}]",
            GenericInstanceType gen
                => $"{(gen.Namespace == ns ? "" : $"{(quoteImportedTypes ? "\"" : "")}{gen.Namespace.ToPyModuleAlias()}.")}{gen.Name.ToNonGeneric()}[{string.Join(", ", gen.GenericArguments.Select(p => p.ToPyTypeName(ns, map)))}]{(gen.Namespace != ns && quoteImportedTypes ? "\"" : "")}",
            ByReferenceType t => t.ElementType.ToPyTypeName(ns, map, quoteImportedTypes),
            OptionalModifierType t => t.ElementType.ToPyTypeName(ns, map, quoteImportedTypes),
            ArrayType t => t.ElementType.ToPyTypeName(ns, map, quoteImportedTypes),
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
            _
                => $"{(type.Namespace == ns ? "" : $"{(quoteImportedTypes ? "\"" : "")}{type.Namespace.ToPyModuleAlias()}.")}{type.Name.ToNonGeneric()}{(type.Namespace != ns && quoteImportedTypes ? "\"" : "")}"
        };

    public static string ToPyInParamTyping(
        this ParameterDefinition param,
        string ns,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default,
        bool quoteImportedTypes = false
    ) =>
        param.GetCategory() switch
        {
            ParamCategory.In
                => param.ParameterType switch
                {
                    GenericInstanceType gen
                        when gen.ElementType.FullName
                            == "Windows.Foundation.Collections.IIterable`1"
                        => $"typing.Iterable[{string.Join(", ", gen.GenericArguments.Select(p => p.ToPyTypeName(ns, map, quoteImportedTypes)))}]",
                    _ => param.ParameterType.ToPyTypeName(ns, map, quoteImportedTypes)
                },
            ParamCategory.PassArray
                => $"typing.Union[winrt.system.Array[{param.ParameterType.ToPyTypeName(ns, map, quoteImportedTypes)}], winrt.system.ReadableBuffer]",
            ParamCategory.FillArray
                => $"typing.Union[winrt.system.Array[{param.ParameterType.ToPyTypeName(ns, map, quoteImportedTypes)}], winrt.system.WriteableBuffer]",
            ParamCategory.ReceiveArray
                => param.ParameterType.ToPyTypeName(ns, map, quoteImportedTypes),
            _ => throw new NotImplementedException(),
        };

    public static string ToPyOutParamTyping(
        this ParameterDefinition param,
        string ns,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default,
        bool quoteImportedTypes = false
    ) =>
        param.GetCategory() switch
        {
            ParamCategory.Out => param.ParameterType.ToPyTypeName(ns, map, quoteImportedTypes),
            ParamCategory.ReceiveArray
                => $"winrt.system.Array[{param.ParameterType.ToPyTypeName(ns, map, quoteImportedTypes)}]",
            _ => throw new NotImplementedException(),
        };

    public static string ToPyReturnTyping(
        this MethodDefinition method,
        string ns,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default,
        bool quoteImportedTypes = false
    )
    {
        var returnType = "None";
        var outParams = method
            .Parameters.Where(p => p.IsPythonOutParam())
            .Select(p => p.ToPyOutParamTyping(ns, map, quoteImportedTypes))
            .ToList();

        if (method.ReturnType.FullName != "System.Void")
        {
            if (method.ReturnType.IsArray)
            {
                outParams.Insert(
                    0,
                    $"winrt.system.Array[{method.ReturnType.ToPyTypeName(ns, map, quoteImportedTypes)}]"
                );
            }
            else
            {
                outParams.Insert(0, method.ReturnType.ToPyTypeName(ns, map, quoteImportedTypes));
            }
        }

        if (outParams.Count == 1)
        {
            returnType = outParams[0];
        }
        else if (outParams.Count > 1)
        {
            returnType = $"typing.Tuple[{string.Join(", ", outParams)}]";
        }

        return returnType;
    }

    public static Func<int, string> ToConvertToArgs(this MethodDefinition method) =>
        method.GetArgumentConvention() switch
        {
            ArgumentConvention.Single => (_) => "arg",
            ArgumentConvention.Variable => (i) => $"args, {i}",
            _ => throw new NotImplementedException(),
        };

    public static bool IsInParam(this ParameterDefinition param) =>
        param.GetCategory() == ParamCategory.In || param.GetCategory() == ParamCategory.PassArray;

    public static bool IsPythonInParam(this ParameterDefinition param) =>
        param.GetCategory() switch
        {
            ParamCategory.In => true,
            ParamCategory.Out => false,
            ParamCategory.PassArray => true,
            ParamCategory.FillArray => true,
            ParamCategory.ReceiveArray => false,
            _ => throw new NotImplementedException(),
        };

    public static bool IsPythonOutParam(this ParameterDefinition param) =>
        param.GetCategory() switch
        {
            ParamCategory.In => false,
            ParamCategory.Out => true,
            ParamCategory.PassArray => false,
            ParamCategory.FillArray => false,
            ParamCategory.ReceiveArray => true,
            _ => throw new NotImplementedException(),
        };

    public static string ToDelegateParam(this ParameterDefinition param) =>
        param.GetCategory() switch
        {
            // REVISIT: would be nice to have real type here instead of auto
            // but that means we would need to copy `consumes_type` writer
            // handling from cppwinrt
            // https://github.com/microsoft/cppwinrt/blob/9b453cfc518bdaa7e2ff590526c4883457fd6065/cppwinrt/code_writers.h#L896
            ParamCategory.In
                => $"auto {param.ToParamName()}",
            ParamCategory.Out => $"{param.ParameterType.ToCppTypeName()}& {param.ToParamName()}",
            ParamCategory.PassArray
                => $"winrt::array_view<{param.ParameterType.ToCppTypeName()} const> {param.ToParamName()}",
            ParamCategory.FillArray
                => $"winrt::array_view<{param.ParameterType.ToCppTypeName()}> {param.ToParamName()}",
            ParamCategory.ReceiveArray
                => $"winrt::com_array<{param.ParameterType.ToCppTypeName()}>& {param.ToParamName()}",
            _ => throw new NotImplementedException(),
        };

    public static bool ImplementsInterface(this TypeDefinition type, string interfaceName)
    {
        if (type.FullName == interfaceName)
        {
            return true;
        }

        return type.Interfaces.Any(i =>
            i.InterfaceType.Resolve().ImplementsInterface(interfaceName)
        );
    }

    static bool ImplementsIAsyncInfo(this TypeDefinition type) =>
        type.ImplementsInterface("Windows.Foundation.IAsyncInfo");

    public static bool HasFlagsAttribute(this TypeDefinition type) =>
        type.CustomAttributes.Any(a => a.AttributeType.FullName == "System.FlagsAttribute");

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

    public static bool IsCustomNumeric(this TypeReference type) =>
        type.Namespace == "Windows.Foundation.Numerics" && CustomNumerics.ContainsKey(type.Name);

    public static bool IsCustomNumeric(
        this TypeReference type,
        [NotNullWhen(true)] out string? cppName
    )
    {
        cppName = default;

        return type.Namespace == "Windows.Foundation.Numerics"
            && CustomNumerics.TryGetValue(type.Name, out cppName);
    }
}
