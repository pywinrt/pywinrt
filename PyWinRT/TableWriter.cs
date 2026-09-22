using System.Text;
using Mono.Cecil;

/// <summary>
/// The kind of value a parameter or a struct field holds, which is what the
/// interpreter converts by.
/// </summary>
/// <remarks>
/// The numbers are part of the table format and never change; see
/// <c>runtime/src/table-format.md</c>.
/// </remarks>
enum TypeCode : uint
{
    Void = 0,
    Boolean = 1,
    Int8 = 2,
    UInt8 = 3,
    Int16 = 4,
    UInt16 = 5,
    Int32 = 6,
    UInt32 = 7,
    Int64 = 8,
    UInt64 = 9,
    Single = 10,
    Double = 11,
    Char16 = 12,
    String = 13,
    Guid = 14,
    HResult = 15,
    EventToken = 16,
    Object = 17,
    DateTime = 18,
    TimeSpan = 19,
    Enum32 = 20,
    EnumU32 = 21,
    Struct = 22,
    Interface = 23,
    Class = 24,
    Delegate = 25,
    Generic = 26,
    Reference = 27,
}

enum TableTypeFlags : uint
{
    CategoryMask = 0x7,
    External = 1 << 3,
    Static = 1 << 4,
    Composable = 1 << 5,
    Parameterized = 1 << 6,
    Concrete = 1 << 7,
    DefaultActivatable = 1 << 8,
    PythonType = 1 << 9,
    Iterable = 1 << 10,
    Iterator = 1 << 11,
    Sequence = 1 << 12,
    MutableSequence = 1 << 13,
    Mapping = 1 << 14,
    MutableMapping = 1 << 15,
    Awaitable = 1 << 16,
    Closeable = 1 << 17,
    Stringable = 1 << 18,
    Buffer = 1 << 19,
    BufferLength = 1 << 20,
    Integer = 1 << 21,
    FlagsEnum = 1 << 22,
}

enum TableGroupKind : uint
{
    Method = 0,
    Property = 1,
    Event = 2,
    Constructor = 3,
}

enum TableGroupFlags : uint
{
    KindMask = 0x7,
    Static = 1 << 3,
    Deprecated = 1 << 4,
}

enum TableMemberKind : uint
{
    Method = 0,
    PropertyGet = 1,
    PropertyPut = 2,
    EventAdd = 3,
    EventRemove = 4,
    Constructor = 5,
}

enum TableMemberFlags : uint
{
    KindMask = 0x7,
    Static = 1 << 3,
    Overridable = 1 << 4,
    Protected = 1 << 5,
    Deprecated = 1 << 6,
    DefaultOverload = 1 << 7,
    RoleMask = 0x1f << TableWriter.MemberRoleShift,
}

/// <summary>
/// The part a member plays in a Python protocol, which is what the slots of a
/// projected collection call.
/// </summary>
/// <remarks>
/// Which WinRT member stands for <c>__len__</c> or <c>__getitem__</c> is a
/// convention rather than anything the metadata states, and what settles it is
/// the parameterized interface that declares the member: the <c>Size</c> of an
/// <c>IVector&lt;T&gt;</c> is a length, the <c>Size</c> of something else is a
/// property named <c>size</c>. Only the generator has that interface to hand,
/// so it writes the answer down instead of leaving the runtime to guess it
/// from a name.
/// </remarks>
enum TableMemberRole : uint
{
    None = 0,
    Size = 1,
    GetAt = 2,
    SetAt = 3,
    RemoveAt = 4,
    InsertAt = 5,
    First = 6,
    Current = 7,
    HasCurrent = 8,
    MoveNext = 9,
    Lookup = 10,
    HasKey = 11,
    Insert = 12,
    Remove = 13,
    Status = 14,
    Completed = 15,
    GetResults = 16,
    ToString = 17,
    Value = 18,
    Close = 19,
    GetMany = 20,
    GetView = 21,
    IndexOf = 22,
    Append = 23,
    RemoveAtEnd = 24,
    Clear = 25,
    ReplaceAll = 26,
    Split = 27,
    PairKey = 28,
    PairValue = 29,
}

enum TableParamFlags : uint
{
    CategoryMask = 0x7,
    IsReturnValue = 1 << 3,

    /// <summary>
    /// The parameter is part of the ABI signature but not of the projected
    /// member: the outer object and the non-delegating inner that a composition
    /// factory takes on top of what the constructor declares.
    /// </summary>
    Implicit = 1 << 4,

    /// <summary>
    /// The ABI passes a pointer to the parameter's value rather than the value
    /// itself.
    /// </summary>
    ByReference = 1 << 5,
}

/// <summary>
/// A type as the table describes it.
/// </summary>
/// <remarks>
/// A record serves two purposes that only partly overlap: saying enough about a
/// WinRT type to query for it and convert values of it - which needs its IID and
/// its shape - and naming a Python type to build here or to find in another
/// module. A declaring interface only ever needs the first, which is why the
/// table carries its IID rather than expecting the type registry to know an
/// interface that is exclusive to a runtime class and so is never projected.
/// </remarks>
sealed class TableType(string key, string name, string ns)
{
    public string Key { get; } = key;
    public string Name { get; } = name;
    public string Namespace { get; } = ns;
    public string PyName { get; set; } = "";
    public string Signature { get; set; } = "";
    public Guid? Guid { get; set; }
    public TableTypeFlags Flags { get; set; }
    public TableType? BaseType { get; set; }
    public TableType? DefaultInterface { get; set; }
    public List<TableType> Interfaces { get; } = [];
    public List<TableType> GenericArgs { get; } = [];
    public List<TableGroup> Groups { get; } = [];
    public List<TableField> Fields { get; } = [];
    public List<TableConstant> Constants { get; } = [];
    public List<TableType> Factories { get; } = [];
    public List<TableType> ComposableFactories { get; } = [];
    public List<TableType> Overridables { get; } = [];

    public bool IsExternal => Flags.HasFlag(TableTypeFlags.External);
}

sealed class TableGroup(string pyName, TableGroupKind kind)
{
    public string PyName { get; } = pyName;
    public TableGroupKind Kind { get; } = kind;
    public TableGroupFlags Flags { get; set; }
    public List<TableMember> Members { get; } = [];
}

sealed class TableMember(string winrtName, TableMemberKind kind)
{
    public string WinRtName { get; } = winrtName;
    public TableMemberKind Kind { get; } = kind;
    public TableMemberFlags Flags { get; set; }
    public TableMemberRole Role { get; set; }
    public TableType? Declaring { get; set; }
    public uint Slot { get; set; }
    public uint ForwardShape { get; set; } = uint.MaxValue;
    public uint ReverseShape { get; set; } = uint.MaxValue;
    public List<TableParam> Params { get; } = [];
    public uint InCount { get; set; }
    public uint OutCount { get; set; }
}

sealed class TableParam(string name, TypeCode code, TableType? type, ParamCategory category)
{
    public string Name { get; } = name;
    public TypeCode Code { get; } = code;
    public TableType? Type { get; } = type;
    public ParamCategory Category { get; } = category;
    public bool IsReturnValue { get; set; }
    public bool IsImplicit { get; set; }
    public bool IsByReference { get; set; }
}

sealed class TableField(string pyName, string winrtName, TypeCode code, TableType? type)
{
    public string PyName { get; } = pyName;
    public string WinRtName { get; } = winrtName;
    public TypeCode Code { get; } = code;
    public TableType? Type { get; } = type;
}

/// <summary>
/// One constant of a WinRT enum, as the member of a Python enum class that the
/// runtime builds it into.
/// </summary>
/// <param name="pyName">
/// The name the constant is reached by. It is the only name written down: an
/// enum constant is never named on the ABI, so, unlike a struct field, the
/// spelling the metadata gives it is of no use to anyone reading the table.
/// </param>
/// <param name="value">
/// The constant, as the thirty-two bits it occupies. A WinRT enum is a signed
/// int32 unless it carries FlagsAttribute, in which case it is an unsigned
/// uint32, so what the two mean by the same bits differs and the type record
/// is what says which.
/// </param>
sealed class TableConstant(string pyName, uint value)
{
    public string PyName { get; } = pyName;
    public uint Value { get; } = value;
}

/// <summary>
/// Writes the <c>_table.pywinrt.txt</c> of one WinRT namespace.
/// </summary>
/// <remarks>
/// This is the whole of what a projection package used to compile. The text is
/// what the repository carries; <c>winrt/table.py</c> compiles it into the
/// <c>_table.pywinrt</c> that <c>runtime/src/table.cpp</c> reads, as part of
/// building the projection. The format is written up in
/// <c>runtime/src/table-format.md</c>; keep the four in step.
/// </remarks>
sealed class TableWriter
{
    /// <summary>
    /// Where the member role sits in a member record's flags.
    /// </summary>
    public const int MemberRoleShift = 8;

    private const uint NoRef = uint.MaxValue;

    /// <summary>
    /// The compatibility generation, which is the same number as the ABI major
    /// in pywinrt/abi.h and the epoch in a projection package's version. The
    /// runtime asserts the two against each other.
    /// </summary>
    public const ushort FormatMajor = 4;

    /// <summary>
    /// Counts the additions to the table format within a generation.
    /// </summary>
    public const ushort FormatMinor = 0;

    private readonly QualifiedNamespace ns;
    private readonly IReadOnlyDictionary<string, string> packageMap;
    private readonly Census census;
    private readonly AbiShapeReader shapes = new();

    private readonly Dictionary<string, TableType> types = new(StringComparer.Ordinal);
    private readonly List<TableType> order = [];

    /// <summary>
    /// The projected form of the definition behind a generic instance, by full
    /// name, since the same definition serves every instance of it.
    /// </summary>
    private readonly Dictionary<string, ProjectedType> definitions = new(StringComparer.Ordinal);

    private TableWriter(
        QualifiedNamespace ns,
        IReadOnlyDictionary<string, string> packageMap,
        Census census
    )
    {
        this.ns = ns;
        this.packageMap = packageMap;
        this.census = census;
    }

    public static void Write(
        DirectoryInfo nsDir,
        QualifiedNamespace ns,
        IReadOnlyDictionary<string, string> packageMap,
        Members members,
        Census census
    )
    {
        var writer = new TableWriter(ns, packageMap, census);

        writer.Build(members);
        writer.Serialize(nsDir);
    }

    // ----- building the model ---------------------------------------------

    private void Build(Members members)
    {
        foreach (var type in members.Enums)
        {
            AddDefinedType(type);
        }

        foreach (var type in members.Structs)
        {
            AddDefinedType(type);
        }

        foreach (var type in members.Interfaces)
        {
            AddDefinedType(type);
        }

        foreach (var type in members.Classes)
        {
            AddDefinedType(type);
        }

        foreach (var type in members.Delegates)
        {
            AddDefinedType(type);
        }
    }

    /// <summary>
    /// Gets the record for a type this namespace defines, filling it in on the
    /// first call.
    /// </summary>
    private TableType AddDefinedType(ProjectedType type)
    {
        var record = GetTypeRecord(type.Type);

        if (record.Flags.HasFlag(TableTypeFlags.PythonType))
        {
            return record;
        }

        record.Flags |= TableTypeFlags.PythonType;
        record.PyName = type.PyWrapperTypeName;
        record.Signature = type.IsGeneric ? "" : WinRtGuid.TryGetSignature(type.Type) ?? "";

        if (type.IsStatic)
        {
            record.Flags |= TableTypeFlags.Static;
        }

        if (type.IsComposable)
        {
            record.Flags |= TableTypeFlags.Composable;
        }

        if (type.IsGeneric)
        {
            record.Flags |= TableTypeFlags.Parameterized;
        }

        SetProtocolFlags(record, type);

        switch (type.Category)
        {
            case Category.Enum:
                AddConstants(record, type);
                return record;
            case Category.Struct:
                AddFields(record, type);
                return record;
            case Category.Delegate:
                AddDelegateInvoke(record, type);
                return record;
        }

        if (
            type.Type.BaseType is TypeReference baseType
            && baseType.FullName != "System.Object"
            && baseType.FullName != "System.MulticastDelegate"
        )
        {
            record.BaseType = GetTypeRecord(baseType);
        }

        if (type.DefaultInterface is TypeReference defaultInterface)
        {
            record.DefaultInterface = GetTypeRecord(defaultInterface);
        }

        foreach (var iface in GetRequiredInterfaces(type.Type))
        {
            record.Interfaces.Add(GetTypeRecord(iface));
        }

        foreach (var factory in type.Factories.Values)
        {
            if (factory.Type is null)
            {
                if (factory.IsActivatable)
                {
                    record.Flags |= TableTypeFlags.DefaultActivatable;
                }

                continue;
            }

            var factoryRecord = GetTypeRecord(factory.Type);

            if (factory.IsComposable)
            {
                record.ComposableFactories.Add(factoryRecord);
            }
            else
            {
                record.Factories.Add(factoryRecord);
            }
        }

        // A [protected] interface belongs here just as little as any other
        // interface the class implements itself: what it marks is that only a
        // derived class may call it, which the members carry. An [overridable]
        // one is implemented by the derived class, so it is the derived
        // object that has to answer it.
        foreach (var iface in type.Type.Interfaces)
        {
            if (
                iface.CustomAttributes.Any(a =>
                    a.AttributeType.FullName == "Windows.Foundation.Metadata.OverridableAttribute"
                )
            )
            {
                record.Overridables.Add(GetTypeRecord(iface.InterfaceType));
            }
        }

        AddMembers(record, type);

        return record;
    }

    private static void SetProtocolFlags(TableType record, ProjectedType type)
    {
        if (type.IsPyIterable)
        {
            record.Flags |= TableTypeFlags.Iterable;
        }

        if (type.IsPyIterator)
        {
            record.Flags |= TableTypeFlags.Iterator;
        }

        if (type.IsPySequence)
        {
            record.Flags |= TableTypeFlags.Sequence;
        }

        if (type.IsPyMutableSequence)
        {
            record.Flags |= TableTypeFlags.MutableSequence;
        }

        if (type.IsPyMapping)
        {
            record.Flags |= TableTypeFlags.Mapping;
        }

        if (type.IsPyMutableMapping)
        {
            record.Flags |= TableTypeFlags.MutableMapping;
        }

        if (type.IsPyAwaitable)
        {
            record.Flags |= TableTypeFlags.Awaitable;
        }

        if (type.IsPyCloseable)
        {
            record.Flags |= TableTypeFlags.Closeable;
        }

        if (type.IsPyStringable)
        {
            record.Flags |= TableTypeFlags.Stringable;
        }

        if (type.IsPyInteger)
        {
            record.Flags |= TableTypeFlags.Integer;
        }

        if (type.IsPyBuffer)
        {
            record.Flags |= TableTypeFlags.Buffer;

            if (type.PyBufferSize == "Length")
            {
                record.Flags |= TableTypeFlags.BufferLength;
            }
        }
    }

    /// <summary>
    /// The role of each member that a Python protocol is built on, by the
    /// interface that declares it and the member's WinRT name.
    /// </summary>
    private static readonly Dictionary<string, TableMemberRole> memberRoles = new(
        StringComparer.Ordinal
    )
    {
        ["Windows.Foundation.Collections.IVector`1.get_Size"] = TableMemberRole.Size,
        ["Windows.Foundation.Collections.IVector`1.GetAt"] = TableMemberRole.GetAt,
        ["Windows.Foundation.Collections.IVector`1.SetAt"] = TableMemberRole.SetAt,
        ["Windows.Foundation.Collections.IVector`1.InsertAt"] = TableMemberRole.InsertAt,
        ["Windows.Foundation.Collections.IVector`1.RemoveAt"] = TableMemberRole.RemoveAt,
        ["Windows.Foundation.Collections.IVector`1.GetMany"] = TableMemberRole.GetMany,
        ["Windows.Foundation.Collections.IVector`1.GetView"] = TableMemberRole.GetView,
        ["Windows.Foundation.Collections.IVector`1.IndexOf"] = TableMemberRole.IndexOf,
        ["Windows.Foundation.Collections.IVector`1.Append"] = TableMemberRole.Append,
        ["Windows.Foundation.Collections.IVector`1.RemoveAtEnd"] = TableMemberRole.RemoveAtEnd,
        ["Windows.Foundation.Collections.IVector`1.Clear"] = TableMemberRole.Clear,
        ["Windows.Foundation.Collections.IVector`1.ReplaceAll"] = TableMemberRole.ReplaceAll,
        ["Windows.Foundation.Collections.IVectorView`1.get_Size"] = TableMemberRole.Size,
        ["Windows.Foundation.Collections.IVectorView`1.GetAt"] = TableMemberRole.GetAt,
        ["Windows.Foundation.Collections.IVectorView`1.GetMany"] = TableMemberRole.GetMany,
        ["Windows.Foundation.Collections.IVectorView`1.IndexOf"] = TableMemberRole.IndexOf,
        ["Windows.Foundation.Collections.IMap`2.get_Size"] = TableMemberRole.Size,
        ["Windows.Foundation.Collections.IMap`2.Lookup"] = TableMemberRole.Lookup,
        ["Windows.Foundation.Collections.IMap`2.HasKey"] = TableMemberRole.HasKey,
        ["Windows.Foundation.Collections.IMap`2.Insert"] = TableMemberRole.Insert,
        ["Windows.Foundation.Collections.IMap`2.Remove"] = TableMemberRole.Remove,
        ["Windows.Foundation.Collections.IMap`2.GetView"] = TableMemberRole.GetView,
        ["Windows.Foundation.Collections.IMap`2.Clear"] = TableMemberRole.Clear,
        ["Windows.Foundation.Collections.IMapView`2.get_Size"] = TableMemberRole.Size,
        ["Windows.Foundation.Collections.IMapView`2.Lookup"] = TableMemberRole.Lookup,
        ["Windows.Foundation.Collections.IMapView`2.HasKey"] = TableMemberRole.HasKey,
        ["Windows.Foundation.Collections.IMapView`2.Split"] = TableMemberRole.Split,
        ["Windows.Foundation.Collections.IKeyValuePair`2.get_Key"] = TableMemberRole.PairKey,
        ["Windows.Foundation.Collections.IKeyValuePair`2.get_Value"] = TableMemberRole.PairValue,
        ["Windows.Foundation.Collections.IIterable`1.First"] = TableMemberRole.First,
        ["Windows.Foundation.Collections.IIterator`1.get_Current"] = TableMemberRole.Current,
        ["Windows.Foundation.Collections.IIterator`1.get_HasCurrent"] = TableMemberRole.HasCurrent,
        ["Windows.Foundation.Collections.IIterator`1.MoveNext"] = TableMemberRole.MoveNext,
        ["Windows.Foundation.Collections.IIterator`1.GetMany"] = TableMemberRole.GetMany,
        ["Windows.Foundation.IAsyncInfo.get_Status"] = TableMemberRole.Status,
        ["Windows.Foundation.IAsyncAction.get_Completed"] = TableMemberRole.Completed,
        ["Windows.Foundation.IAsyncAction.GetResults"] = TableMemberRole.GetResults,
        ["Windows.Foundation.IAsyncActionWithProgress`1.get_Completed"] = TableMemberRole.Completed,
        ["Windows.Foundation.IAsyncActionWithProgress`1.GetResults"] = TableMemberRole.GetResults,
        ["Windows.Foundation.IAsyncOperation`1.get_Completed"] = TableMemberRole.Completed,
        ["Windows.Foundation.IAsyncOperation`1.GetResults"] = TableMemberRole.GetResults,
        ["Windows.Foundation.IAsyncOperationWithProgress`2.get_Completed"] =
            TableMemberRole.Completed,
        ["Windows.Foundation.IAsyncOperationWithProgress`2.GetResults"] =
            TableMemberRole.GetResults,
        ["Windows.Foundation.IStringable.ToString"] = TableMemberRole.ToString,
        ["Windows.Foundation.IReference`1.get_Value"] = TableMemberRole.Value,
        ["Windows.Foundation.IClosable.Close"] = TableMemberRole.Close,
    };

    /// <summary>
    /// Gets the part a member plays in a Python protocol.
    /// </summary>
    /// <param name="declaring">
    /// The interface that declares the member, closed over its type arguments
    /// where it has any, or <c>null</c> for a member that is reached without
    /// one.
    /// </param>
    /// <param name="name">The WinRT name of the member.</param>
    /// <remarks>
    /// The name alone does not answer this. A runtime class of its own may
    /// have a <c>Size</c> property or a <c>Remove</c> method that means
    /// something else entirely, and it is only the collection interface behind
    /// the member that makes it the one <c>len()</c> or <c>del</c> calls.
    /// </remarks>
    private static TableMemberRole GetMemberRole(TypeReference? declaring, string name)
    {
        if (declaring is null)
        {
            return TableMemberRole.None;
        }

        var key = $"{declaring.GetElementType().FullName}.{name}";

        return memberRoles.GetValueOrDefault(key, TableMemberRole.None);
    }

    /// <summary>
    /// Gets every interface <paramref name="type"/> implements, directly or
    /// through another interface, which is the set an instance can be queried
    /// for.
    /// </summary>
    private static IEnumerable<TypeReference> GetRequiredInterfaces(TypeDefinition type)
    {
        var seen = new HashSet<string>(StringComparer.Ordinal);
        var result = new List<TypeReference>();

        void visit(TypeDefinition current)
        {
            foreach (var iface in current.Interfaces)
            {
                if (!seen.Add(iface.InterfaceType.FullName))
                {
                    continue;
                }

                result.Add(iface.InterfaceType);

                var definition = iface.InterfaceType.TryResolve();

                if (definition is not null)
                {
                    visit(definition);
                }
            }
        }

        visit(type);

        return result;
    }

    /// <summary>
    /// Records the constants of an enum, which is the whole of what one is.
    /// </summary>
    /// <remarks>
    /// The metadata spells an enum's constants as static fields of it, and
    /// prefixes them with the instance field that carries the value, which is
    /// the one without a constant behind it.
    /// </remarks>
    private static void AddConstants(TableType record, ProjectedType type)
    {
        var isFlags = type.Type.HasFlagsAttribute;

        if (isFlags)
        {
            record.Flags |= TableTypeFlags.FlagsEnum;
        }

        foreach (var field in type.Type.Fields)
        {
            if (field.Constant is null)
            {
                continue;
            }

            var value = isFlags
                ? Convert.ToUInt32(field.Constant)
                : unchecked((uint)Convert.ToInt32(field.Constant));

            record.Constants.Add(new TableConstant(field.Name.ToPythonConstant(), value));
        }
    }

    private void AddFields(TableType record, ProjectedType type)
    {
        foreach (var field in type.Type.Fields.Where(f => !f.IsStatic))
        {
            var (code, fieldType) = GetTypeCode(field.FieldType, null);

            record.Fields.Add(
                new TableField(field.Name.ToPythonIdentifier(), field.Name, code, fieldType)
            );
        }
    }

    private void AddDelegateInvoke(
        TableType record,
        ProjectedType type,
        GenericContext? context = null
    )
    {
        var invoke = type.Type.Methods.FirstOrDefault(m => m.Name == "Invoke");

        if (invoke is null)
        {
            return;
        }

        var group = new TableGroup("", TableGroupKind.Method);
        var member = new TableMember("Invoke", TableMemberKind.Method)
        {
            Slot = ShapeCensus.DelegateInvokeSlot,
        };

        var map = context?.Map;

        member.InCount = (uint)invoke.Parameters.Count(p => p.IsPythonInParam);

        AddParams(member, invoke, map);
        SetShapes(member, invoke, map);

        group.Members.Add(member);
        record.Groups.Add(group);
    }

    private void AddMembers(TableType record, ProjectedType type, GenericContext? context = null)
    {
        if (type.Constructors.Count > 0)
        {
            var group = new TableGroup("", TableGroupKind.Constructor);

            foreach (var constructor in type.Constructors.OrderBy(c => c.PyInParamCount))
            {
                group.Members.Add(
                    MakeMember(type, constructor, TableMemberKind.Constructor, context)
                );
            }

            record.Groups.Add(group);
        }

        foreach (var property in type.Properties)
        {
            var group = new TableGroup(
                property.Name.ToPythonIdentifier(isTypeMethod: property.IsStatic),
                TableGroupKind.Property
            );

            if (property.IsStatic)
            {
                group.Flags |= TableGroupFlags.Static;
            }

            group.Members.Add(
                MakeMember(type, property.GetMethod, TableMemberKind.PropertyGet, context)
            );

            if (property.SetMethod is ProjectedMethod setMethod)
            {
                group.Members.Add(
                    MakeMember(type, setMethod, TableMemberKind.PropertyPut, context)
                );
            }

            record.Groups.Add(group);
        }

        foreach (var @event in type.Events)
        {
            var group = new TableGroup(
                @event.Name.ToPythonIdentifier(isTypeMethod: @event.IsStatic),
                TableGroupKind.Event
            );

            if (@event.IsStatic)
            {
                group.Flags |= TableGroupFlags.Static;
            }

            group.Members.Add(
                MakeMember(type, @event.AddMethod, TableMemberKind.EventAdd, context)
            );
            group.Members.Add(
                MakeMember(type, @event.RemoveMethod, TableMemberKind.EventRemove, context)
            );

            record.Groups.Add(group);
        }

        foreach (var methodGroup in type.MethodGroups)
        {
            var group = new TableGroup(methodGroup.PyName, TableGroupKind.Method);

            if (methodGroup.IsStatic)
            {
                group.Flags |= TableGroupFlags.Static;
            }

            foreach (var overload in methodGroup.Overloads.OrderBy(o => o.PyInParamCount))
            {
                group.Members.Add(MakeMember(type, overload, TableMemberKind.Method, context));
            }

            if (group.Members.All(m => m.Flags.HasFlag(TableMemberFlags.Deprecated)))
            {
                group.Flags |= TableGroupFlags.Deprecated;
            }

            record.Groups.Add(group);
        }
    }

    private TableMember MakeMember(
        ProjectedType type,
        ProjectedMethod method,
        TableMemberKind kind,
        GenericContext? context = null
    )
    {
        var isFactoryCall = method.IsStatic || method.IsConstructor;
        var factoryMethod = isFactoryCall ? type.GetFactoryMethod(method) : null;
        // A static member and a constructor are declared by a factory interface;
        // plain activation has none, because it is
        // IActivationFactory::ActivateInstance.
        var declaring = isFactoryCall
            ? factoryMethod?.DeclaringType
            : type.GetDeclaringInterface(method);

        if (declaring is not null)
        {
            declaring = SubstituteDeclaring(declaring, context, context?.Map);
        }

        // What a member's parameters mean is settled by the interface that
        // declares it, which is the one reference that has been closed over
        // every type argument on the way to it. A class lists its interfaces
        // closed - JsonArray implements IIterable<IJsonValue> directly - but
        // an interface does not: IObservableMap<K, V> requires IMap<K, V>,
        // which requires IIterable<IKeyValuePair<K, V>>, and walking that
        // chain a level at a time loses what K and V stood for.
        var map = GenericArguments(declaring) ?? EffectiveMap(method.GenericArgMap, context);

        var member = new TableMember(method.Method.Name, kind)
        {
            Declaring = declaring is null ? null : GetTypeRecord(declaring),
            Role = GetMemberRole(declaring, method.Method.Name),
            Slot = GetSlot(type, method),
        };

        if (method.IsStatic)
        {
            member.Flags |= TableMemberFlags.Static;
        }

        if (method.IsOverridable)
        {
            member.Flags |= TableMemberFlags.Overridable;
        }

        if (method.IsProtected)
        {
            member.Flags |= TableMemberFlags.Protected;
        }

        if (method.IsDeprecated)
        {
            member.Flags |= TableMemberFlags.Deprecated;
        }

        if (method.IsDefaultOverload)
        {
            member.Flags |= TableMemberFlags.DefaultOverload;
        }

        member.InCount = (uint)method.PyInParamCount;

        if (method.IsConstructor && factoryMethod is null)
        {
            // Plain activation takes no arguments and hands back the instance
            // through the one out parameter of ActivateInstance.
            member.Params.Add(
                new TableParam("", TypeCode.Object, null, ParamCategory.Out)
                {
                    IsReturnValue = true,
                }
            );
            member.OutCount = 1;
            member.ForwardShape = GetShapeId("p");

            return member;
        }

        var abiMethod = factoryMethod ?? method.Method;

        // A composition factory takes the outer object and the non-delegating
        // inner on top of what the constructor declares, and those are the
        // runtime's business rather than the caller's.
        var implicitFrom = method.IsConstructor ? method.Method.Parameters.Count : int.MaxValue;

        AddParams(member, abiMethod, map, implicitFrom);
        SetShapes(member, abiMethod, map);

        if (method.IsConstructor)
        {
            member.OutCount = 1;
        }

        return member;
    }

    /// <summary>
    /// Gets the vtable slot of <paramref name="method"/>, which is six plus its
    /// position in the metadata of the interface that declares it.
    /// </summary>
    /// <remarks>
    /// A runtime class redeclares the members of its interfaces, so the position
    /// to take is the one in the interface, not in the class. Plain activation
    /// has no interface of its own: it is
    /// <c>IActivationFactory::ActivateInstance</c>, the first method after
    /// <c>IInspectable</c>.
    /// </remarks>
    private static uint GetSlot(ProjectedType type, ProjectedMethod method)
    {
        var definition = method.Method;

        if (method.IsStatic || method.IsConstructor)
        {
            definition = type.GetFactoryMethod(method) ?? definition;

            if (definition == method.Method)
            {
                // IActivationFactory::ActivateInstance
                return ShapeCensus.FirstInterfaceSlot;
            }
        }
        else if (definition.HasOverrides)
        {
            var overridden = definition.Overrides[0].Resolve();

            if (overridden is not null)
            {
                definition = overridden;
            }
        }

        var index = definition.DeclaringType.Methods.IndexOf(definition);

        if (index < 0)
        {
            throw new InvalidOperationException(
                $"cannot find {definition} in {definition.DeclaringType}"
            );
        }

        return (uint)(ShapeCensus.FirstInterfaceSlot + index);
    }

    private void AddParams(
        TableMember member,
        MethodDefinition method,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map,
        int implicitFrom = int.MaxValue
    )
    {
        for (var i = 0; i < method.Parameters.Count; i++)
        {
            var param = method.Parameters[i];
            var category = param.GetCategory();
            var (code, type) = GetTypeCode(param.ParameterType, map);

            member.Params.Add(
                new TableParam(param.Name ?? "", code, type, category)
                {
                    IsImplicit = i >= implicitFrom,
                    IsByReference = param.IsPassedByReference(),
                }
            );

            if (i >= implicitFrom)
            {
                continue;
            }

            if (param.IsPythonOutParam)
            {
                member.OutCount++;
            }
        }

        if (method.ReturnType.FullName != "System.Void")
        {
            var (code, type) = GetTypeCode(method.ReturnType, map);

            member.Params.Add(
                new TableParam(
                    "",
                    code,
                    type,
                    method.ReturnType.IsArray ? ParamCategory.ReceiveArray : ParamCategory.Out
                )
                {
                    IsReturnValue = true,
                }
            );
            member.OutCount++;
        }
    }

    private void SetShapes(
        TableMember member,
        MethodDefinition method,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map
    )
    {
        var shape = shapes.GetShape(method, map);

        if (shape is null)
        {
            // A type parameter has no shape of its own; the concrete
            // instance that a caller names does, and that is what is called.
            return;
        }

        member.ForwardShape = GetShapeId(shape.Key);

        if (
            census.ReverseIds.TryGetValue(
                Census.ToReverseKey((int)member.Slot, shape.Key),
                out var reverseId
            )
        )
        {
            member.ReverseShape = (uint)reverseId;
        }
    }

    /// <summary>
    /// The id of the forward shape <paramref name="key"/>, or
    /// <see cref="NoRef"/> when the census does not have it.
    /// </summary>
    /// <remarks>
    /// A member the census cannot name is written without a shape rather than
    /// refused, so that the rest of the namespace is still projected and only
    /// the calls that have no trampoline are lost. The run reports every
    /// missing shape once, by shape rather than by member, because a shape is
    /// what a report upstream has to name.
    /// </remarks>
    private uint GetShapeId(string key) =>
        census.ShapeIds.TryGetValue(key, out var id) ? (uint)id : NoRef;

    // ----- generic instances ----------------------------------------------

    /// <summary>
    /// The generic instance whose members are being written and the type
    /// arguments its definition's parameters stand for.
    /// </summary>
    /// <remarks>
    /// A parameterized interface is written once per instance a namespace
    /// names, because what its members pass is not the same for
    /// <c>IVector&lt;String&gt;</c> as for <c>IVector&lt;Point&gt;</c>: the
    /// first passes a pointer where the second passes a struct by value, so
    /// they are two ABI shapes and two sets of type codes. The names, the
    /// slots and the grouping all come from the one open definition.
    /// </remarks>
    private sealed record GenericContext(
        GenericInstanceType Instance,
        IReadOnlyDictionary<GenericParameter, TypeReference> Map
    );

    /// <summary>
    /// Composes the map of a member reached through a required interface with
    /// the one that closes the type it was reached from.
    /// </summary>
    /// <remarks>
    /// <c>IVector&lt;String&gt;</c> reaches <c>First()</c> through
    /// <c>IIterable&lt;T&gt;</c>, so the member's own map says
    /// <c>IIterable.T</c> is <c>IVector.T</c> and the context says
    /// <c>IVector.T</c> is <c>String</c>. The two sets of keys belong to
    /// different types, so composing them is one dictionary.
    /// </remarks>
    private static IReadOnlyDictionary<GenericParameter, TypeReference>? EffectiveMap(
        IReadOnlyDictionary<GenericParameter, TypeReference>? map,
        GenericContext? context
    )
    {
        if (context is null)
        {
            return map;
        }

        if (map is null)
        {
            return context.Map;
        }

        var composed = new Dictionary<GenericParameter, TypeReference>(context.Map);

        foreach (var (parameter, argument) in map)
        {
            composed[parameter] = ProjectedType.SubstituteGenericArgs(argument, context.Map);
        }

        return composed;
    }

    /// <summary>
    /// Fills in whatever type arguments <paramref name="map"/> names, so that a
    /// member of a generic instance mentions the instances it really passes
    /// and not the open ones its definition is written in terms of.
    /// </summary>
    private static TypeReference Substitute(
        TypeReference type,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map
    ) => map is null ? type : ProjectedType.SubstituteGenericArgs(type, map);

    /// <summary>
    /// Gets the interface that declares a member of a generic instance, which
    /// for the definition's own members is the instance being written rather
    /// than the definition the member was read from.
    /// </summary>
    private static TypeReference SubstituteDeclaring(
        TypeReference declaring,
        GenericContext? context,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map
    )
    {
        if (context is null)
        {
            return declaring;
        }

        if (declaring.FullName == context.Instance.ElementType.FullName)
        {
            return context.Instance;
        }

        return Substitute(declaring, map);
    }

    /// <summary>
    /// The type arguments of a generic instance, by the parameter each fills
    /// in, or <c>null</c> if the type is not one.
    /// </summary>
    private static IReadOnlyDictionary<GenericParameter, TypeReference>? GenericArguments(
        TypeReference? type
    ) =>
        type is GenericInstanceType generic
        && generic.ElementType.TryResolve() is TypeDefinition definition
            ? definition.GenericParameters.Zip(generic.GenericArguments).ToDictionary()
            : null;

    /// <summary>
    /// The projected form of a parameterized definition, which is where the
    /// Python names, the grouping and the vtable slots of every instance of it
    /// come from.
    /// </summary>
    private ProjectedType GetProjectedDefinition(TypeDefinition definition)
    {
        if (definitions.TryGetValue(definition.FullName, out var existing))
        {
            return existing;
        }

        var projected = new ProjectedType(definition);

        definitions.Add(definition.FullName, projected);

        return projected;
    }

    /// <summary>
    /// Writes the members of a generic instance, which are the members of its
    /// definition with every type argument filled in.
    /// </summary>
    private void AddGenericInstanceMembers(
        TableType record,
        GenericInstanceType generic,
        TypeDefinition definition
    )
    {
        var projected = GetProjectedDefinition(definition);
        var context = new GenericContext(
            generic,
            definition.GenericParameters.Zip(generic.GenericArguments).ToDictionary()
        );

        // The Python type of an instance derives from the one its definition is
        // bound to, because that is where the collection protocol mixins that
        // __init__.py adds are.
        record.BaseType = GetTypeRecord(definition);

        SetProtocolFlags(record, projected);

        if (definition.GetCategory() == Category.Delegate)
        {
            AddDelegateInvoke(record, projected, context);

            return;
        }

        foreach (var iface in GetRequiredInterfaces(definition))
        {
            record.Interfaces.Add(GetTypeRecord(SubstituteDeclaring(iface, context, context.Map)));
        }

        AddMembers(record, projected, context);
    }

    // ----- type records ---------------------------------------------------

    private TableType GetTypeRecord(TypeReference type)
    {
        if (type is GenericInstanceType generic)
        {
            return GetGenericInstanceRecord(generic);
        }

        var key = type.FullName;

        if (types.TryGetValue(key, out var existing))
        {
            return existing;
        }

        var record = new TableType(key, type.Name.ToNonGeneric(), type.Namespace);

        types.Add(key, record);
        order.Add(record);

        if (type is GenericParameter)
        {
            // A type argument that is still standing in for a type, which is
            // recorded so that the instance can name it and nothing more: the
            // concrete instance a caller names is what is ever called.
            record.Flags |= TableTypeFlags.Parameterized;

            return record;
        }

        var definition = type.TryResolve();
        var category = definition?.GetCategory() ?? Category.Class;

        record.Flags |= (TableTypeFlags)(uint)category;

        if (definition is not null)
        {
            record.Guid = WinRtGuid.TryGetGuid(definition);

            if (definition.HasGenericParameters)
            {
                record.Flags |= TableTypeFlags.Parameterized;
            }
        }

        if (type.Namespace != ns.Namespace)
        {
            record.Flags |= TableTypeFlags.External;
            record.PyName =
                $"{type.GetQualifiedNamespace(packageMap).PyModuleName}."
                + $"{(category == Category.Interface ? "_" : "")}{type.Name.ToNonGeneric()}";
        }

        return record;
    }

    private TableType GetGenericInstanceRecord(GenericInstanceType generic)
    {
        var key = generic.ToWinRtName();

        if (types.TryGetValue(key, out var existing))
        {
            return existing;
        }

        var definition = generic.ElementType.TryResolve();
        // A member of a parameterized interface can mention another instance of
        // it - IMap<K, V> requires IIterable<IKeyValuePair<K, V>> - which has no
        // IID until the types are named, so only a concrete instance gets one.
        var isConcrete = IsConcrete(generic);
        var record = new TableType(
            key,
            generic.ElementType.Name.ToNonGeneric(),
            generic.ElementType.Namespace
        )
        {
            Flags =
                (TableTypeFlags)(uint)(definition?.GetCategory() ?? Category.Interface)
                | (isConcrete ? TableTypeFlags.Concrete : TableTypeFlags.Parameterized),
            Signature = isConcrete ? WinRtGuid.GetSignature(generic) : "",
            Guid = isConcrete ? WinRtGuid.GetGuid(generic) : null,
        };

        types.Add(key, record);
        order.Add(record);

        foreach (var argument in generic.GenericArguments)
        {
            record.GenericArgs.Add(GetTypeRecord(argument));
        }

        if (isConcrete && definition is not null)
        {
            record.PyName = GetInstancePyName(generic, definition);

            AddGenericInstanceMembers(record, generic, definition);
        }

        return record;
    }

    /// <summary>
    /// The name the Python type of a generic instance goes by.
    /// </summary>
    /// <remarks>
    /// It is fully qualified, the way the name of an external reference is,
    /// because the type belongs to the module that defines the parameterized
    /// interface rather than to whichever module first names the instance.
    /// Nothing is bound to it: an instance is only ever reached through a
    /// member that returns one.
    /// </remarks>
    private string GetInstancePyName(GenericInstanceType generic, TypeDefinition definition)
    {
        var module = definition.GetQualifiedNamespace(packageMap).PyModuleName;
        var prefix = definition.GetCategory() == Category.Interface ? "_" : "";

        return $"{module}.{prefix}{GetInstanceDisplayName(generic)}";
    }

    private static string GetInstanceDisplayName(TypeReference type) =>
        type is GenericInstanceType generic
            ? $"{generic.ElementType.Name.ToNonGeneric()}"
                + $"[{string.Join(", ", generic.GenericArguments.Select(GetInstanceDisplayName))}]"
            : type.Name.ToNonGeneric();

    /// <summary>
    /// True if every type argument of <paramref name="type"/>, and of the
    /// instances nested in it, is a real type rather than a generic parameter.
    /// </summary>
    private static bool IsConcrete(TypeReference type) =>
        type switch
        {
            GenericParameter => false,
            { FullName: "System.Type" } => false,
            GenericInstanceType generic => generic.GenericArguments.All(IsConcrete),
            TypeSpecification spec => IsConcrete(spec.ElementType),
            _ => true,
        };

    /// <summary>
    /// Gets the type code of <paramref name="type"/> and, where the code does
    /// not say everything, the record of the type it names.
    /// </summary>
    private (TypeCode Code, TableType? Type) GetTypeCode(
        TypeReference type,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map
    )
    {
        while (type is ByReferenceType or ArrayType or OptionalModifierType or RequiredModifierType)
        {
            type = ((TypeSpecification)type).ElementType;
        }

        if (type is GenericParameter parameter)
        {
            if (map is null || !map.TryGetValue(parameter, out var argument))
            {
                return (TypeCode.Object, null);
            }

            return GetTypeCode(argument, null);
        }

        switch (type)
        {
            case GenericInstanceType generic
                when generic.ElementType.FullName == "Windows.Foundation.IReference`1":
                return (TypeCode.Reference, GetTypeRecord(Substitute(generic, map)));
            case GenericInstanceType generic:
                // An instance of a parameterized delegate - EventHandler<T>,
                // TypedEventHandler<S, R> - is a delegate like any other: what
                // its type arguments change is the signature of its Invoke and
                // not what a caller passes where it is expected, which is a
                // callable.
                return (
                    generic.ElementType.TryResolve()?.GetCategory() == Category.Delegate
                        ? TypeCode.Delegate
                        : TypeCode.Generic,
                    GetTypeRecord(Substitute(generic, map))
                );
            case { FullName: "System.Void" }:
                return (TypeCode.Void, null);
            case { FullName: "System.Boolean" }:
                return (TypeCode.Boolean, null);
            case { FullName: "System.SByte" }:
                return (TypeCode.Int8, null);
            case { FullName: "System.Byte" }:
                return (TypeCode.UInt8, null);
            case { FullName: "System.Int16" }:
                return (TypeCode.Int16, null);
            case { FullName: "System.UInt16" }:
                return (TypeCode.UInt16, null);
            case { FullName: "System.Int32" }:
                return (TypeCode.Int32, null);
            case { FullName: "System.UInt32" }:
                return (TypeCode.UInt32, null);
            case { FullName: "System.Int64" }:
                return (TypeCode.Int64, null);
            case { FullName: "System.UInt64" }:
                return (TypeCode.UInt64, null);
            case { FullName: "System.Single" }:
                return (TypeCode.Single, null);
            case { FullName: "System.Double" }:
                return (TypeCode.Double, null);
            case { FullName: "System.Char" }:
                return (TypeCode.Char16, null);
            case { FullName: "System.String" }:
                return (TypeCode.String, null);
            case { FullName: "System.Guid" }:
                return (TypeCode.Guid, null);
            case { FullName: "System.Object" }:
                return (TypeCode.Object, null);
            case { FullName: "Windows.Foundation.DateTime" }:
                return (TypeCode.DateTime, null);
            case { FullName: "Windows.Foundation.TimeSpan" }:
                return (TypeCode.TimeSpan, null);
            // These two are one integer each, so the ABI is the integer, but
            // they keep a Python type of their own and the record names it the
            // way an enum's does.
            case { FullName: "Windows.Foundation.HResult" }:
                return (TypeCode.HResult, GetTypeRecord(type));
            case { FullName: "Windows.Foundation.EventRegistrationToken" }:
                return (TypeCode.EventToken, GetTypeRecord(type));
            case { FullName: "System.Type" }:
                // Windows.UI.Xaml.Interop.TypeName, which is a struct that
                // C++/WinRT and the metadata spell differently
                return (TypeCode.Object, null);
        }

        var definition = type.TryResolve();

        if (definition is null)
        {
            return (TypeCode.Object, null);
        }

        var record = GetTypeRecord(type);

        return definition.GetCategory() switch
        {
            Category.Enum => (
                definition.HasFlagsAttribute ? TypeCode.EnumU32 : TypeCode.Enum32,
                record
            ),
            Category.Struct => (TypeCode.Struct, record),
            Category.Interface => (TypeCode.Interface, record),
            Category.Delegate => (TypeCode.Delegate, record),
            _ => (TypeCode.Class, record),
        };
    }

    // ----- serializing ----------------------------------------------------

    private static readonly string[] categoryNames =
    [
        "enum",
        "struct",
        "interface",
        "class",
        "delegate",
    ];

    private static readonly (uint Flag, string Name)[] typeFlagNames =
    [
        ((uint)TableTypeFlags.External, "external"),
        ((uint)TableTypeFlags.Static, "static"),
        ((uint)TableTypeFlags.Composable, "composable"),
        ((uint)TableTypeFlags.Parameterized, "parameterized"),
        ((uint)TableTypeFlags.Concrete, "concrete"),
        ((uint)TableTypeFlags.DefaultActivatable, "activatable"),
        ((uint)TableTypeFlags.PythonType, "python_type"),
        ((uint)TableTypeFlags.Iterable, "iterable"),
        ((uint)TableTypeFlags.Iterator, "iterator"),
        ((uint)TableTypeFlags.Sequence, "sequence"),
        ((uint)TableTypeFlags.MutableSequence, "mutable_sequence"),
        ((uint)TableTypeFlags.Mapping, "mapping"),
        ((uint)TableTypeFlags.MutableMapping, "mutable_mapping"),
        ((uint)TableTypeFlags.Awaitable, "awaitable"),
        ((uint)TableTypeFlags.Closeable, "closeable"),
        ((uint)TableTypeFlags.Stringable, "stringable"),
        ((uint)TableTypeFlags.Buffer, "buffer"),
        ((uint)TableTypeFlags.BufferLength, "buffer_length"),
        ((uint)TableTypeFlags.Integer, "integer"),
        ((uint)TableTypeFlags.FlagsEnum, "flags_enum"),
    ];

    private static readonly string[] groupKindNames =
    [
        "method",
        "property",
        "event",
        "constructor",
    ];

    private static readonly (uint Flag, string Name)[] groupFlagNames =
    [
        ((uint)TableGroupFlags.Static, "static"),
        ((uint)TableGroupFlags.Deprecated, "deprecated"),
    ];

    private static readonly string[] memberKindNames =
    [
        "method",
        "get",
        "put",
        "add",
        "remove",
        "ctor",
    ];

    private static readonly (uint Flag, string Name)[] memberFlagNames =
    [
        ((uint)TableMemberFlags.Static, "static"),
        ((uint)TableMemberFlags.Overridable, "overridable"),
        ((uint)TableMemberFlags.Protected, "protected"),
        ((uint)TableMemberFlags.Deprecated, "deprecated"),
        ((uint)TableMemberFlags.DefaultOverload, "default_overload"),
    ];

    private static readonly string[] roleNames =
    [
        "none",
        "size",
        "get_at",
        "set_at",
        "remove_at",
        "insert_at",
        "first",
        "current",
        "has_current",
        "move_next",
        "lookup",
        "has_key",
        "insert",
        "remove",
        "status",
        "completed",
        "get_results",
        "to_string",
        "value",
        "close",
        "get_many",
        "get_view",
        "index_of",
        "append",
        "remove_at_end",
        "clear",
        "replace_all",
        "split",
        "pair_key",
        "pair_value",
    ];

    private static readonly string[] paramCategoryNames =
    [
        "in",
        "out",
        "pass_array",
        "fill_array",
        "receive_array",
    ];

    private static readonly string[] codeNames =
    [
        "void",
        "boolean",
        "int8",
        "uint8",
        "int16",
        "uint16",
        "int32",
        "uint32",
        "int64",
        "uint64",
        "single",
        "double",
        "char16",
        "string",
        "guid",
        "hresult",
        "event_token",
        "object",
        "datetime",
        "timespan",
        "enum32",
        "enum_u32",
        "struct",
        "interface",
        "class",
        "delegate",
        "parameterized",
        "reference",
    ];

    /// <summary>
    /// Writes the table of this namespace in the text form the repository
    /// carries and a source distribution ships.
    /// </summary>
    /// <remarks>
    /// The binary the runtime reads is compiled from this text by
    /// <c>winrt/table.py</c> in <c>winrt-runtime</c>. So nothing
    /// written here is an offset or an index: a reference is spelled as the name
    /// of the record it points at, and the order records are written in is the
    /// order they are numbered in there. The grammar and the vocabulary are in
    /// <c>runtime/src/table-format.md</c>.
    /// </remarks>
    private void Serialize(DirectoryInfo nsDir)
    {
        // The types this namespace defines come first, sorted by name, and the
        // references to other namespaces follow, so that a runtime that wants to
        // find a type by name can binary search the first block.
        var sorted = order
            .Where(t => !t.IsExternal)
            .OrderBy(t => t.Name, StringComparer.Ordinal)
            .ThenBy(t => t.Key, StringComparer.Ordinal)
            .Concat(
                order
                    .Where(t => t.IsExternal)
                    .OrderBy(t => t.Namespace, StringComparer.Ordinal)
                    .ThenBy(t => t.Name, StringComparer.Ordinal)
            )
            .ToList();

        var names = new HashSet<string>(StringComparer.Ordinal);

        foreach (var type in sorted)
        {
            var name = TextName(type);

            if (!names.Add(name))
            {
                throw new InvalidOperationException(
                    $"{ns.Namespace} has two type records that are both written as '{name}'"
                );
            }
        }

        FileContent.WriteIfChanged(
            nsDir,
            "_table.pywinrt.txt",
            sink => WriteTable(new TextSink(sink), sorted)
        );
    }

    private void WriteTable(TextSink sink, IReadOnlyList<TableType> sorted)
    {
        sink.Line($"format {FormatMajor}.{FormatMinor}");
        sink.Line($"generator {PyWinRT.VersionString}");
        sink.Line($"census {census.Lineage} {census.Revision}");
        sink.Line($"namespace {ns.Namespace}");

        foreach (var type in sorted)
        {
            sink.Line("");

            WriteType(sink, type);
        }
    }

    private static void WriteType(TextSink sink, TableType type)
    {
        var category = categoryNames[(uint)type.Flags & (uint)TableTypeFlags.CategoryMask];

        sink.Line($"{category} {TextName(type)}{FlagNames((uint)type.Flags, typeFlagNames)}");

        if (type.PyName.Length > 0)
        {
            sink.Line($"    py {type.PyName}");
        }

        if (type.Guid is Guid guid)
        {
            sink.Line($"    guid {guid:d}");
        }

        if (type.Signature.Length > 0)
        {
            sink.Line($"    signature {type.Signature}");
        }

        if (type.BaseType is TableType baseType)
        {
            sink.Line($"    base {TextName(baseType)}");
        }

        if (type.DefaultInterface is TableType defaultInterface)
        {
            sink.Line($"    default {TextName(defaultInterface)}");
        }

        foreach (var iface in type.Interfaces)
        {
            sink.Line($"    requires {TextName(iface)}");
        }

        foreach (var argument in type.GenericArgs)
        {
            sink.Line($"    argument {TextName(argument)}");
        }

        foreach (var factory in type.Factories)
        {
            sink.Line($"    factory {TextName(factory)}");
        }

        foreach (var factory in type.ComposableFactories)
        {
            sink.Line($"    composable {TextName(factory)}");
        }

        foreach (var iface in type.Overridables)
        {
            sink.Line($"    overridable {TextName(iface)}");
        }

        foreach (var field in type.Fields)
        {
            sink.Line(
                $"    field {field.PyName} {field.WinRtName} {codeNames[(uint)field.Code]}"
                    + TypeName(field.Type)
            );
        }

        foreach (var constant in type.Constants)
        {
            // A flags enum is unsigned and is read as a set of bits, so it is
            // written in hexadecimal; every other enum is a signed number.
            var value = type.Flags.HasFlag(TableTypeFlags.FlagsEnum)
                ? $"0x{constant.Value:X}"
                : unchecked((int)constant.Value).ToString();

            sink.Line($"    constant {constant.PyName} {value}");
        }

        foreach (var group in type.Groups)
        {
            WriteGroup(sink, group);
        }
    }

    private static void WriteGroup(TextSink sink, TableGroup group)
    {
        // A constructor group, and the Invoke of a delegate, are bound to no
        // name of their own. The name is written all the same, as a dash, so
        // that the flags after it are never mistaken for it.
        var name = group.PyName.Length > 0 ? group.PyName : "-";

        sink.Line(
            $"    {groupKindNames[(uint)group.Kind]} {name}"
                + FlagNames((uint)group.Flags, groupFlagNames)
        );

        foreach (var member in group.Members)
        {
            WriteMember(sink, member);
        }
    }

    private static void WriteMember(TextSink sink, TableMember member)
    {
        var line = new StringBuilder("        ");

        line.Append(memberKindNames[(uint)member.Kind])
            .Append(' ')
            .Append(member.WinRtName)
            .Append(" slot=")
            .Append(member.Slot)
            .Append(" inputs=")
            .Append(member.InCount)
            .Append(" outputs=")
            .Append(member.OutCount);

        if (member.Declaring is TableType declaring)
        {
            line.Append(" declaring=").Append(TextName(declaring));
        }

        if (member.ForwardShape != NoRef)
        {
            line.Append(" shape=").Append(member.ForwardShape);
        }

        if (member.ReverseShape != NoRef)
        {
            line.Append(" reverse=").Append(member.ReverseShape);
        }

        if (member.Role != TableMemberRole.None)
        {
            line.Append(" role=").Append(roleNames[(uint)member.Role]);
        }

        line.Append(FlagNames((uint)member.Flags, memberFlagNames));

        sink.Line(line.ToString());

        foreach (var parameter in member.Params)
        {
            WriteParam(sink, parameter);
        }
    }

    private static void WriteParam(TextSink sink, TableParam parameter)
    {
        var line = new StringBuilder("            ");

        line.Append(paramCategoryNames[(uint)parameter.Category])
            .Append(' ')
            .Append(codeNames[(uint)parameter.Code])
            .Append(TypeName(parameter.Type));

        if (parameter.Name.Length > 0)
        {
            line.Append(" name=").Append(parameter.Name);
        }

        if (parameter.IsReturnValue)
        {
            line.Append(" return");
        }

        if (parameter.IsImplicit)
        {
            line.Append(" implicit");
        }

        if (parameter.IsByReference)
        {
            line.Append(" by_reference");
        }

        sink.Line(line.ToString());
    }

    /// <summary>
    /// The name a type record goes by in the text, which is what every
    /// reference to it is written as.
    /// </summary>
    /// <remarks>
    /// The binary numbers the records and points at them by index. The text
    /// names them instead, so a name has to be unique within a table and has to
    /// carry the record's own WinRT namespace and name, which is what the
    /// compiler splits back out of it. A type argument that is still standing in
    /// for a type belongs to no namespace, so it is written as the bare name the
    /// metadata gives it.
    /// </remarks>
    private static string TextName(TableType type)
    {
        var name = type.Namespace.Length > 0 ? $"{type.Namespace}.{type.Name}" : type.Name;

        if (type.GenericArgs.Count == 0)
        {
            return name;
        }

        return $"{name}<{string.Join(",", type.GenericArgs.Select(TextName))}>";
    }

    private static string TypeName(TableType? type) =>
        type is null ? "" : $" type={TextName(type)}";

    private static string FlagNames(uint flags, (uint Flag, string Name)[] names)
    {
        var text = new StringBuilder();

        foreach (var (flag, name) in names)
        {
            if ((flags & flag) != 0)
            {
                text.Append(' ').Append(name);
            }
        }

        return text.ToString();
    }
}
