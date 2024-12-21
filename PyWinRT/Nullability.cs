using System.Collections.Concurrent;
using System.Text.Json.Serialization;
using Mono.Cecil;

sealed class NamespaceNullabilityInfo
{
    private readonly ConcurrentDictionary<string, TypeNullabilityInfo> types = new();

    [JsonConstructor]
    public NamespaceNullabilityInfo(string @namespace, IList<TypeNullabilityInfo> types)
    {
        Namespace = @namespace;
        this.types = new(types.ToDictionary(t => t.Name), StringComparer.Ordinal);
    }

    public string Namespace { get; }

    public IList<TypeNullabilityInfo> Types =>
        [.. types.Values.OrderBy(t => t.Name, StringComparer.Ordinal)];

    public TypeNullabilityInfo AddOrUpdateType(
        TypeDefinition type,
        Func<TypeNullabilityInfo> addValueFactory,
        Func<TypeNullabilityInfo, TypeNullabilityInfo> updateValueFactory
    ) => types.AddOrUpdate(type.Name, _ => addValueFactory(), (_, v) => updateValueFactory(v));
}

class TypeNullabilityInfo
{
    private class MethodComparer : IComparer<MethodNullabilityInfo>
    {
        public int Compare(MethodNullabilityInfo? x, MethodNullabilityInfo? y)
        {
            if (x is null || y is null)
            {
                throw new InvalidOperationException();
            }

            var result = string.Compare(x.Name, y.Name, StringComparison.Ordinal);

            if (result == 0)
            {
                return Comparer<int>.Default.Compare(x.Parameters.Count, y.Parameters.Count);
            }

            return result;
        }
    }

    [JsonConstructor]
    public TypeNullabilityInfo(string name, IList<MethodNullabilityInfo> methods)
    {
        Name = name;
        var comparer = new MethodComparer();
        Methods = [.. methods.OrderBy(m => m, comparer)];
    }

    public TypeNullabilityInfo(TypeDefinition type)
    {
        Name = type.Name;
        var comparer = new MethodComparer();

        if (type.IsDelegate())
        {
            // have to exclude constructor for delegates since that isn't part of the projection
            Methods = [new MethodNullabilityInfo(type.Methods.Single(m => m.Name == "Invoke"))];
        }
        else
        {
            Methods =
            [
                .. type.Methods.Select(m => new MethodNullabilityInfo(m)).OrderBy(m => m, comparer)
            ];
        }
    }

    public TypeNullabilityInfo(TypeDefinition type, TypeNullabilityInfo old)
    {
        Name = type.Name;
        var comparer = new MethodComparer();
        var oldMethods = old.Methods.ToDictionary(m => m.Signature);

        if (type.IsDelegate())
        {
            // have to exclude constructor for delegates since that isn't part of the projection

            var invokeMethod = type.Methods.Single(m => m.Name == "Invoke");

            Methods =
            [
                new MethodNullabilityInfo(invokeMethod, oldMethods[invokeMethod.ToString()])
            ];
        }
        else
        {
            Methods =
            [
                .. type
                    .Methods.Select(m =>
                    {
                        if (oldMethods.TryGetValue(m.ToString(), out var om))
                        {
                            return new MethodNullabilityInfo(m, om);
                        }

                        return new MethodNullabilityInfo(m);
                    })
                    .OrderBy(m => m, comparer)
            ];
        }
    }

    public string Name { get; }

    public IList<MethodNullabilityInfo> Methods { get; }
}

class MethodNullabilityInfo
{
    [JsonConstructor]
    public MethodNullabilityInfo(
        string signature,
        string name,
        ReturnNullabilityInfo @return,
        IList<ParameterNullabilityInfo> parameters
    )
    {
        Signature = signature;
        Name = name;
        Return = @return;
        Parameters = parameters;
    }

    public MethodNullabilityInfo(MethodDefinition method)
    {
        Signature = method.ToString();
        Name = method.Name;
        Return = new ReturnNullabilityInfo(method.ReturnType);
        Parameters = method.Parameters.Select(p => new ParameterNullabilityInfo(p)).ToList();
    }

    public MethodNullabilityInfo(MethodDefinition method, MethodNullabilityInfo old)
    {
        Signature = method.ToString();
        Name = method.Name;
        Return = new ReturnNullabilityInfo(method.ReturnType, old.Return);
        Parameters =
        [
            .. method.Parameters.Select(
                (p, i) => new ParameterNullabilityInfo(p, old.Parameters[i])
            )
        ];
    }

    public string Signature { get; }
    public string Name { get; }
    public ReturnNullabilityInfo Return { get; }
    public IList<ParameterNullabilityInfo> Parameters { get; }
}

class ReturnNullabilityInfo
{
    [JsonConstructor]
    public ReturnNullabilityInfo(TypeRefNullabilityInfo type)
    {
        Type = type;
    }

    public ReturnNullabilityInfo(TypeReference type)
    {
        Type = new TypeRefNullabilityInfo(type);
    }

    public ReturnNullabilityInfo(TypeReference type, ReturnNullabilityInfo old)
    {
        Type = new TypeRefNullabilityInfo(type, old.Type);
    }

    public TypeRefNullabilityInfo Type { get; }
}

class ParameterNullabilityInfo
{
    public enum ParameterKind
    {
        In = ParamCategory.In,
        Out = ParamCategory.Out,
        PassArray = ParamCategory.PassArray,
        FillArray = ParamCategory.FillArray,
        ReceiveArray = ParamCategory.ReceiveArray,
    }

    [JsonConstructor]
    public ParameterNullabilityInfo(string name, TypeRefNullabilityInfo type)
    {
        Name = name;
        Type = type;
    }

    public ParameterNullabilityInfo(ParameterDefinition parameter)
    {
        Name = parameter.Name;
        Kind = (ParameterKind)parameter.GetCategory();

        if (parameter.ParameterType is ArrayType array)
        {
            Type = new TypeRefNullabilityInfo(array.ElementType);
        }
        else if (parameter.ParameterType is ByReferenceType byRef)
        {
            if (byRef.ElementType is ArrayType outArray)
            {
                Type = new TypeRefNullabilityInfo(outArray.ElementType);
            }
            else
            {
                Type = new TypeRefNullabilityInfo(byRef.ElementType);
            }
        }
        else
        {
            Type = new TypeRefNullabilityInfo(parameter.ParameterType);
        }
    }

    public ParameterNullabilityInfo(ParameterDefinition parameter, ParameterNullabilityInfo old)
    {
        Name = parameter.Name;
        Kind = (ParameterKind)parameter.GetCategory();

        if (parameter.ParameterType is ArrayType array)
        {
            Type = new TypeRefNullabilityInfo(array.ElementType, old.Type);
        }
        else if (parameter.ParameterType is ByReferenceType byRef)
        {
            if (byRef.ElementType is ArrayType outArray)
            {
                Type = new TypeRefNullabilityInfo(outArray.ElementType, old.Type);
            }
            else
            {
                Type = new TypeRefNullabilityInfo(byRef.ElementType, old.Type);
            }
        }
        else
        {
            Type = new TypeRefNullabilityInfo(parameter.ParameterType, old.Type);
        }
    }

    public string Name { get; }

    public ParameterKind Kind { get; }
    public TypeRefNullabilityInfo Type { get; }
}

class TypeRefNullabilityInfo
{
    [JsonConstructor]
    public TypeRefNullabilityInfo(
        string name,
        IList<TypeRefNullabilityInfo>? args = default,
        bool allowNull = false,
        bool maybeNull = false
    )
    {
        Name = name;
        Args = args;
        AllowNull = allowNull;
        MaybeNull = maybeNull;
    }

    public TypeRefNullabilityInfo(TypeReference type)
    {
        if (type is GenericInstanceType generic)
        {
            Name = generic.ElementType.FullName;
            Args = generic.GenericArguments.Select(a => new TypeRefNullabilityInfo(a)).ToList();
        }
        else
        {
            Name = type.FullName;
        }
    }

    public TypeRefNullabilityInfo(TypeReference type, TypeRefNullabilityInfo old)
    {
        if (type is GenericInstanceType generic)
        {
            Name = generic.ElementType.FullName;
            Args = generic
                .GenericArguments.Select(
                    (a, i) => new TypeRefNullabilityInfo(a, old.Args?[i] ?? new(a))
                )
                .ToList();
        }
        else
        {
            Name = type.FullName;
        }

        AllowNull = old.AllowNull;
        MaybeNull = old.MaybeNull;
    }

    public string Name { get; }

    [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
    public IList<TypeRefNullabilityInfo>? Args { get; }

    [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
    public bool AllowNull { get; }

    [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
    public bool MaybeNull { get; }
}
