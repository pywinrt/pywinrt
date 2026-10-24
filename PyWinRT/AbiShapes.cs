using System.Text;
using Mono.Cecil;

/// <summary>
/// A primitive that can appear in a WinRT ABI signature, either as an argument
/// of a call or as a field of a struct that is passed by value.
/// </summary>
/// <remarks>
/// These are storage kinds, not WinRT types: what matters here is how wide the
/// value is and whether it travels in an integer or a floating point register.
/// </remarks>
enum AbiPrimitive : byte
{
    Bool,
    Int8,
    UInt8,
    Int16,
    UInt16,
    Char16,
    Int32,
    UInt32,
    Int64,
    UInt64,
    Single,
    Double,

    /// <summary>
    /// Anything that travels as an address: an interface pointer, an HSTRING,
    /// an out parameter, the two halves of an array parameter.
    /// </summary>
    Pointer,
}

static class AbiPrimitiveExtensions
{
    /// <summary>
    /// Gets the size of <paramref name="primitive"/> in bytes, where a pointer
    /// is <paramref name="pointerSize"/> bytes wide.
    /// </summary>
    public static int GetSize(this AbiPrimitive primitive, int pointerSize) =>
        primitive switch
        {
            AbiPrimitive.Bool or AbiPrimitive.Int8 or AbiPrimitive.UInt8 => 1,
            AbiPrimitive.Int16 or AbiPrimitive.UInt16 or AbiPrimitive.Char16 => 2,
            AbiPrimitive.Int32 or AbiPrimitive.UInt32 or AbiPrimitive.Single => 4,
            AbiPrimitive.Int64 or AbiPrimitive.UInt64 or AbiPrimitive.Double => 8,
            AbiPrimitive.Pointer => pointerSize,
            _ => throw new ArgumentOutOfRangeException(nameof(primitive)),
        };

    /// <summary>
    /// Gets the C++ spelling of <paramref name="primitive"/> used in the
    /// generated trampoline instantiations.
    /// </summary>
    public static string ToCppType(this AbiPrimitive primitive) =>
        primitive switch
        {
            // WinRT boolean is one byte, which is what uint8_t spells without
            // depending on how a toolchain lays bool out.
            AbiPrimitive.Bool or AbiPrimitive.UInt8 => "uint8_t",
            AbiPrimitive.Int8 => "int8_t",
            AbiPrimitive.Int16 => "int16_t",
            AbiPrimitive.UInt16 => "uint16_t",
            AbiPrimitive.Char16 => "char16_t",
            AbiPrimitive.Int32 => "int32_t",
            AbiPrimitive.UInt32 => "uint32_t",
            AbiPrimitive.Int64 => "int64_t",
            AbiPrimitive.UInt64 => "uint64_t",
            AbiPrimitive.Single => "float",
            AbiPrimitive.Double => "double",
            AbiPrimitive.Pointer => "void*",
            _ => throw new ArgumentOutOfRangeException(nameof(primitive)),
        };

    /// <summary>
    /// Gets the one or two character code that spells
    /// <paramref name="primitive"/> in a shape key.
    /// </summary>
    public static string ToCode(this AbiPrimitive primitive) =>
        primitive switch
        {
            AbiPrimitive.Bool => "b",
            AbiPrimitive.Int8 => "i1",
            AbiPrimitive.UInt8 => "u1",
            AbiPrimitive.Int16 => "i2",
            AbiPrimitive.UInt16 => "u2",
            AbiPrimitive.Char16 => "c2",
            AbiPrimitive.Int32 => "i4",
            AbiPrimitive.UInt32 => "u4",
            AbiPrimitive.Int64 => "i8",
            AbiPrimitive.UInt64 => "u8",
            AbiPrimitive.Single => "f4",
            AbiPrimitive.Double => "f8",
            AbiPrimitive.Pointer => "p",
            _ => throw new ArgumentOutOfRangeException(nameof(primitive)),
        };
}

/// <summary>
/// The layout of a WinRT struct that is passed by value, flattened to the
/// primitives it is made of.
/// </summary>
/// <remarks>
/// Flattening is faithful: a nested struct has the same size and alignment as
/// its fields spelled in place, because WinRT structs are never packed, and the
/// arm64 rule for a homogeneous floating point aggregate looks through nesting
/// too. So the flattened field list is all a calling convention needs, and it is
/// what the generated trampoline spells.
/// </remarks>
sealed class AbiStructLayout
{
    public AbiStructLayout(IReadOnlyList<AbiPrimitive> fields)
    {
        if (fields.Count == 0)
        {
            throw new ArgumentException("a WinRT struct has at least one field", nameof(fields));
        }

        Fields = fields;
        Key = string.Join("", fields.Select(f => f.ToCode()));
        CppName = $"s_{Key}";
    }

    /// <summary>
    /// The primitives the struct is made of, in declaration order, with nested
    /// structs flattened in place.
    /// </summary>
    public IReadOnlyList<AbiPrimitive> Fields { get; }

    /// <summary>
    /// A key that identifies the layout, e.g. <c>f4f4</c> for a Point.
    /// </summary>
    public string Key { get; }

    /// <summary>
    /// The name of the C++ struct that spells this layout in the generated
    /// trampoline instantiations.
    /// </summary>
    public string CppName { get; }

    /// <summary>
    /// The names of the WinRT structs that have this layout, for the census
    /// report and the comment in the generated file.
    /// </summary>
    public SortedSet<string> WinRtNames { get; } = new(StringComparer.Ordinal);

    public int GetAlign(int pointerSize) => Fields.Max(f => f.GetSize(pointerSize));

    public int GetSize(int pointerSize)
    {
        var offset = 0;

        foreach (var field in Fields)
        {
            var size = field.GetSize(pointerSize);
            offset = AlignUp(offset, size) + size;
        }

        return AlignUp(offset, GetAlign(pointerSize));
    }

    /// <summary>
    /// True if the struct is a homogeneous floating point aggregate, which
    /// arm64 passes in one to four floating point registers.
    /// </summary>
    public bool IsHomogeneousFloatAggregate =>
        Fields.Count <= 4
        && (Fields.All(f => f == AbiPrimitive.Single) || Fields.All(f => f == AbiPrimitive.Double));

    public static int AlignUp(int value, int align) => (value + align - 1) / align * align;

    /// <summary>
    /// Rebuilds a layout from its <see cref="Key"/>, which is how the census
    /// file stores it.
    /// </summary>
    public static AbiStructLayout Parse(string key)
    {
        var fields = new List<AbiPrimitive>();
        var position = 0;

        while (position < key.Length)
        {
            fields.Add(ParsePrimitive(key, ref position));
        }

        return new AbiStructLayout(fields);
    }

    /// <summary>
    /// Reads the one or two character code of a primitive at
    /// <paramref name="position"/> and advances past it.
    /// </summary>
    public static AbiPrimitive ParsePrimitive(string key, ref int position)
    {
        var code = key[position] switch
        {
            'p' or 'b' => key.Substring(position, 1),
            _ => key.Substring(position, 2),
        };

        position += code.Length;

        return code switch
        {
            "b" => AbiPrimitive.Bool,
            "i1" => AbiPrimitive.Int8,
            "u1" => AbiPrimitive.UInt8,
            "i2" => AbiPrimitive.Int16,
            "u2" => AbiPrimitive.UInt16,
            "c2" => AbiPrimitive.Char16,
            "i4" => AbiPrimitive.Int32,
            "u4" => AbiPrimitive.UInt32,
            "i8" => AbiPrimitive.Int64,
            "u8" => AbiPrimitive.UInt64,
            "f4" => AbiPrimitive.Single,
            "f8" => AbiPrimitive.Double,
            "p" => AbiPrimitive.Pointer,
            _ => throw new FormatException($"unknown ABI primitive code '{code}' in '{key}'"),
        };
    }
}

/// <summary>
/// One argument of a WinRT ABI call.
/// </summary>
/// <remarks>
/// Every integer that fits in a register, and every pointer, is the same
/// argument here, spelled as a pointer. All three supported calling conventions
/// put a narrow integer in the low bits of one register or of one four byte
/// stack slot and let the callee ignore the rest, so a trampoline that passes
/// the widened value calls a member that takes a boolean, an enum, an int32 or
/// an interface pointer equally well. That is what makes the census small, and
/// it is why the interpreter widens to register width when it fills the
/// argument buffer. A 64 bit integer is not folded in, because on x86 it is two
/// stack slots rather than one, and neither are floating point arguments, which
/// travel in registers of their own. Struct fields are not widened at all:
/// there the width is the layout.
/// </remarks>
readonly record struct AbiArg(AbiPrimitive Primitive, AbiStructLayout? Struct)
{
    public static AbiArg Of(AbiPrimitive primitive) =>
        new(
            primitive switch
            {
                AbiPrimitive.Bool
                or AbiPrimitive.Int8
                or AbiPrimitive.UInt8
                or AbiPrimitive.Int16
                or AbiPrimitive.UInt16
                or AbiPrimitive.Char16
                or AbiPrimitive.Int32
                or AbiPrimitive.UInt32 => AbiPrimitive.Pointer,
                AbiPrimitive.UInt64 => AbiPrimitive.Int64,
                _ => primitive,
            },
            null
        );

    public static AbiArg Of(AbiStructLayout layout) => new(AbiPrimitive.Pointer, layout);

    public string Key => Struct is null ? Primitive.ToCode() : $"{{{Struct.Key}}}";

    public string ToCppType() => Struct is null ? Primitive.ToCppType() : Struct.CppName;
}

/// <summary>
/// The ABI signature of a WinRT call, as the arguments that follow the
/// <c>this</c> pointer.
/// </summary>
/// <remarks>
/// Every WinRT method is <c>HRESULT __stdcall f(void* this, ...)</c> and every
/// output is a pointer, so the arguments are all there is to a calling
/// convention. Two members with the same shape are called by the same
/// trampoline, whatever they mean.
/// </remarks>
sealed class AbiShape
{
    public AbiShape(IReadOnlyList<AbiArg> args)
    {
        Args = args;
        Key = string.Join("", args.Select(a => a.Key));
    }

    public IReadOnlyList<AbiArg> Args { get; }

    /// <summary>
    /// A key that identifies the shape, e.g. <c>i4p</c> for a method that takes
    /// an integer and returns an object.
    /// </summary>
    public string Key { get; }

    public string ToCppArgumentList() => string.Join(", ", Args.Select(a => a.ToCppType()));

    /// <summary>
    /// Rebuilds a shape from its <see cref="Key"/>, which is how the census
    /// file stores it.
    /// </summary>
    public static AbiShape Parse(string key)
    {
        var args = new List<AbiArg>();
        var position = 0;

        while (position < key.Length)
        {
            if (key[position] == '{')
            {
                var end = key.IndexOf('}', position);

                if (end < 0)
                {
                    throw new FormatException($"unterminated struct in shape '{key}'");
                }

                args.Add(AbiArg.Of(AbiStructLayout.Parse(key[(position + 1)..end])));
                position = end + 1;
            }
            else
            {
                args.Add(new AbiArg(AbiStructLayout.ParsePrimitive(key, ref position), null));
            }
        }

        return new AbiShape(args);
    }

    /// <summary>
    /// Gets the machine level signature of this shape on x64, where every
    /// argument occupies one eight byte positional slot: an integer register, a
    /// floating point register, or - for a struct whose size is not 1, 2, 4 or
    /// 8 bytes - the address of a copy the caller makes.
    /// </summary>
    public string GetX64FoldKey()
    {
        var sb = new StringBuilder();

        foreach (var arg in Args)
        {
            if (arg.Struct is AbiStructLayout layout)
            {
                var size = layout.GetSize(8);
                sb.Append(size is 1 or 2 or 4 or 8 ? "X" : $"M{size}");
            }
            else
            {
                sb.Append(
                    arg.Primitive switch
                    {
                        AbiPrimitive.Single => "S",
                        AbiPrimitive.Double => "D",
                        _ => "X",
                    }
                );
            }
        }

        return sb.ToString();
    }

    /// <summary>
    /// Gets the machine level signature of this shape on arm64, which counts
    /// general purpose and floating point registers separately and looks inside
    /// a struct to decide which it uses.
    /// </summary>
    public string GetArm64FoldKey()
    {
        var sb = new StringBuilder();

        foreach (var arg in Args)
        {
            if (arg.Struct is AbiStructLayout layout)
            {
                if (layout.IsHomogeneousFloatAggregate)
                {
                    sb.Append(
                        new string(
                            layout.Fields[0] == AbiPrimitive.Single ? 'S' : 'D',
                            layout.Fields.Count
                        )
                    );
                }
                else
                {
                    var size = layout.GetSize(8);
                    sb.Append(size <= 16 ? new string('X', (size + 7) / 8) : "M");
                }
            }
            else
            {
                sb.Append(
                    arg.Primitive switch
                    {
                        AbiPrimitive.Single => "S",
                        AbiPrimitive.Double => "D",
                        _ => "X",
                    }
                );
            }
        }

        return sb.ToString();
    }

    /// <summary>
    /// Gets the machine level signature of this shape on x86, where everything
    /// is pushed onto the stack in four byte slots and the callee pops them, so
    /// the only thing a trampoline has to get right is how many there are.
    /// </summary>
    public string GetX86FoldKey()
    {
        // one slot for the this pointer
        var slots = 1;

        foreach (var arg in Args)
        {
            slots += arg.Struct is AbiStructLayout layout
                ? (layout.GetSize(4) + 3) / 4
                : (arg.Primitive.GetSize(4) + 3) / 4;
        }

        return slots.ToString();
    }
}

/// <summary>
/// Reads the ABI shape of a WinRT method out of the metadata.
/// </summary>
sealed class AbiShapeReader
{
    private readonly Dictionary<string, AbiStructLayout> structs = new(StringComparer.Ordinal);
    private readonly Dictionary<string, AbiShape> shapes = new(StringComparer.Ordinal);

    /// <summary>
    /// Every struct layout that appears as a by-value argument, by layout key.
    /// </summary>
    public IReadOnlyDictionary<string, AbiStructLayout> Structs => structs;

    /// <summary>
    /// Every shape that appears in the metadata, by shape key.
    /// </summary>
    public IReadOnlyDictionary<string, AbiShape> Shapes => shapes;

    /// <summary>
    /// Gets the ABI shape of <paramref name="method"/>, with the generic
    /// parameters of its declaring interface substituted by
    /// <paramref name="map"/>, or <c>null</c> if the shape cannot be determined
    /// because a generic parameter was left open.
    /// </summary>
    public AbiShape? GetShape(
        MethodDefinition method,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = null
    )
    {
        var args = new List<AbiArg>();

        foreach (var param in method.Parameters)
        {
            if (!TryAddParam(args, param, param.ParameterType, map))
            {
                return null;
            }
        }

        if (method.ReturnType.FullName != "System.Void")
        {
            // The return value is the last output parameter, so it is a pointer
            // to storage the caller owns - two of them for an array, which is
            // received as a count and a buffer.
            args.Add(AbiArg.Of(AbiPrimitive.Pointer));

            if (method.ReturnType.IsArray)
            {
                args.Add(AbiArg.Of(AbiPrimitive.Pointer));
            }
        }

        return Intern(args);
    }

    /// <summary>
    /// Gets the shape of a call whose arguments all travel in one integer
    /// register, which is every argument that is a pointer, a boolean, an enum
    /// or an integer of at most 32 bits.
    /// </summary>
    public AbiShape GetShape(int arity) =>
        Intern([.. Enumerable.Repeat(AbiArg.Of(AbiPrimitive.Pointer), arity)]);

    /// <summary>
    /// Adds <paramref name="shape"/> to the census if it is not already there
    /// and returns the interned instance.
    /// </summary>
    private AbiShape Intern(IReadOnlyList<AbiArg> args)
    {
        var shape = new AbiShape(args);

        if (shapes.TryGetValue(shape.Key, out var existing))
        {
            return existing;
        }

        shapes.Add(shape.Key, shape);

        return shape;
    }

    private bool TryAddParam(
        List<AbiArg> args,
        ParameterDefinition param,
        TypeReference type,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map
    )
    {
        switch (param.GetCategory())
        {
            case ParamCategory.Out:
                // every WinRT output is a pointer to storage the caller owns
                args.Add(AbiArg.Of(AbiPrimitive.Pointer));
                return true;
            case ParamCategory.PassArray:
            case ParamCategory.FillArray:
                // uint32 count, then the elements
                args.Add(AbiArg.Of(AbiPrimitive.UInt32));
                args.Add(AbiArg.Of(AbiPrimitive.Pointer));
                return true;
            case ParamCategory.ReceiveArray:
                // uint32* count, then the elements
                args.Add(AbiArg.Of(AbiPrimitive.Pointer));
                args.Add(AbiArg.Of(AbiPrimitive.Pointer));
                return true;
        }

        if (param.IsPassedByReference())
        {
            args.Add(AbiArg.Of(AbiPrimitive.Pointer));
            return true;
        }

        var resolved = Substitute(type, map);

        if (resolved is null)
        {
            return false;
        }

        var arg = GetInArg(resolved);

        if (arg is null)
        {
            return false;
        }

        args.Add(arg.Value);

        return true;
    }

    /// <summary>
    /// Resolves <paramref name="type"/> through <paramref name="map"/>, or
    /// returns <c>null</c> if it is still open afterwards.
    /// </summary>
    private static TypeReference? Substitute(
        TypeReference type,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map
    )
    {
        while (type is ByReferenceType or OptionalModifierType or RequiredModifierType)
        {
            type = ((TypeSpecification)type).ElementType;
        }

        if (type is GenericParameter param)
        {
            if (map is null || !map.TryGetValue(param, out var arg))
            {
                return null;
            }

            return Substitute(arg, null);
        }

        return type;
    }

    /// <summary>
    /// Gets how an input parameter of <paramref name="type"/> is passed, or
    /// <c>null</c> if it is not a type that can appear in an ABI signature.
    /// </summary>
    private AbiArg? GetInArg(TypeReference type)
    {
        switch (type)
        {
            case GenericInstanceType:
                // a parameterized interface or delegate is an interface pointer
                return AbiArg.Of(AbiPrimitive.Pointer);
            case { FullName: "System.Boolean" }:
                return AbiArg.Of(AbiPrimitive.Bool);
            case { FullName: "System.SByte" }:
                return AbiArg.Of(AbiPrimitive.Int8);
            case { FullName: "System.Byte" }:
                return AbiArg.Of(AbiPrimitive.UInt8);
            case { FullName: "System.Int16" }:
                return AbiArg.Of(AbiPrimitive.Int16);
            case { FullName: "System.UInt16" }:
                return AbiArg.Of(AbiPrimitive.UInt16);
            case { FullName: "System.Char" }:
                return AbiArg.Of(AbiPrimitive.Char16);
            case { FullName: "System.Int32" }:
                return AbiArg.Of(AbiPrimitive.Int32);
            case { FullName: "System.UInt32" }:
                return AbiArg.Of(AbiPrimitive.UInt32);
            case { FullName: "System.Int64" }:
                return AbiArg.Of(AbiPrimitive.Int64);
            case { FullName: "System.UInt64" }:
                return AbiArg.Of(AbiPrimitive.UInt64);
            case { FullName: "System.Single" }:
                return AbiArg.Of(AbiPrimitive.Single);
            case { FullName: "System.Double" }:
                return AbiArg.Of(AbiPrimitive.Double);
            case { FullName: "System.String" }:
            case { FullName: "System.Object" }:
                return AbiArg.Of(AbiPrimitive.Pointer);
            case { FullName: "System.Guid" }:
                return AbiArg.Of(GetGuidLayout());
            case { FullName: "System.Type" }:
                // Windows.UI.Xaml.Interop.TypeName: an HSTRING and an enum
                return AbiArg.Of(
                    InternStruct([AbiPrimitive.Pointer, AbiPrimitive.Int32], "System.Type")
                );
        }

        var definition = type.TryResolve();

        if (definition is null)
        {
            return null;
        }

        switch (definition.GetCategory())
        {
            case Category.Enum:
                return AbiArg.Of(
                    definition.HasFlagsAttribute ? AbiPrimitive.UInt32 : AbiPrimitive.Int32
                );
            case Category.Struct:
                var fields = new List<AbiPrimitive>();

                return TryFlatten(definition, fields)
                    ? AbiArg.Of(InternStruct(fields, definition.FullName))
                    : null;
            default:
                // a class, interface or delegate is an interface pointer
                return AbiArg.Of(AbiPrimitive.Pointer);
        }
    }

    private AbiStructLayout GetGuidLayout() =>
        InternStruct(
            [
                AbiPrimitive.UInt32,
                AbiPrimitive.UInt16,
                AbiPrimitive.UInt16,
                AbiPrimitive.UInt8,
                AbiPrimitive.UInt8,
                AbiPrimitive.UInt8,
                AbiPrimitive.UInt8,
                AbiPrimitive.UInt8,
                AbiPrimitive.UInt8,
                AbiPrimitive.UInt8,
                AbiPrimitive.UInt8,
            ],
            "System.Guid"
        );

    private AbiStructLayout InternStruct(IReadOnlyList<AbiPrimitive> fields, string winrtName)
    {
        var layout = new AbiStructLayout(fields);

        if (structs.TryGetValue(layout.Key, out var existing))
        {
            layout = existing;
        }
        else
        {
            structs.Add(layout.Key, layout);
        }

        layout.WinRtNames.Add(winrtName);

        return layout;
    }

    /// <summary>
    /// Appends the primitives that <paramref name="type"/> is made of to
    /// <paramref name="fields"/>, recursing into nested structs.
    /// </summary>
    private bool TryFlatten(TypeDefinition type, List<AbiPrimitive> fields)
    {
        foreach (var field in type.Fields.Where(f => !f.IsStatic))
        {
            switch (field.FieldType)
            {
                case GenericInstanceType:
                    // IReference<T> is an interface pointer
                    fields.Add(AbiPrimitive.Pointer);
                    continue;
                case { FullName: "System.Boolean" }:
                    fields.Add(AbiPrimitive.Bool);
                    continue;
                case { FullName: "System.SByte" }:
                    fields.Add(AbiPrimitive.Int8);
                    continue;
                case { FullName: "System.Byte" }:
                    fields.Add(AbiPrimitive.UInt8);
                    continue;
                case { FullName: "System.Int16" }:
                    fields.Add(AbiPrimitive.Int16);
                    continue;
                case { FullName: "System.UInt16" }:
                    fields.Add(AbiPrimitive.UInt16);
                    continue;
                case { FullName: "System.Char" }:
                    fields.Add(AbiPrimitive.Char16);
                    continue;
                case { FullName: "System.Int32" }:
                    fields.Add(AbiPrimitive.Int32);
                    continue;
                case { FullName: "System.UInt32" }:
                    fields.Add(AbiPrimitive.UInt32);
                    continue;
                case { FullName: "System.Int64" }:
                    fields.Add(AbiPrimitive.Int64);
                    continue;
                case { FullName: "System.UInt64" }:
                    fields.Add(AbiPrimitive.UInt64);
                    continue;
                case { FullName: "System.Single" }:
                    fields.Add(AbiPrimitive.Single);
                    continue;
                case { FullName: "System.Double" }:
                    fields.Add(AbiPrimitive.Double);
                    continue;
                case { FullName: "System.String" }:
                case { FullName: "System.Object" }:
                    fields.Add(AbiPrimitive.Pointer);
                    continue;
                case { FullName: "System.Guid" }:
                    fields.AddRange(GetGuidLayout().Fields);
                    continue;
            }

            var definition = field.FieldType.TryResolve();

            if (definition is null)
            {
                return false;
            }

            switch (definition.GetCategory())
            {
                case Category.Enum:
                    fields.Add(
                        definition.HasFlagsAttribute ? AbiPrimitive.UInt32 : AbiPrimitive.Int32
                    );
                    break;
                case Category.Struct:
                    if (!TryFlatten(definition, fields))
                    {
                        return false;
                    }
                    break;
                default:
                    fields.Add(AbiPrimitive.Pointer);
                    break;
            }
        }

        return fields.Count > 0;
    }
}
