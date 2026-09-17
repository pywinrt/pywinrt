using System.CodeDom.Compiler;
using System.Collections.ObjectModel;

static class MapWriterExtensions
{
    public static string WriteMapPythonSpecialMethods(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        IReadOnlyDictionary<string, string> packageMap,
        out string keyParamType
    )
    {
        var method = type.GetMethod("Lookup", 1);
        var nullabilityInfo = nullabilityMap.GetValueOrDefault(
            method.Signature,
            new MethodNullabilityInfo(method.Method)
        );
        keyParamType = method
            .Method.Parameters[0]
            .ToPyInParamTyping(
                ns,
                nullabilityInfo.Parameters[0].Type,
                packageMap,
                method.GenericArgMap
            );
        var valueReturnType = method.Method.ToPyReturnTyping(
            ns,
            nullabilityInfo,
            packageMap,
            method.GenericArgMap
        );

        w.WriteLine("def __len__(self) -> int: ...");
        w.WriteLine($"def __iter__(self) -> _cabc.Iterator[{keyParamType}]: ...");
        w.WriteLine("def __contains__(self, key: object) -> bool: ...");
        w.WriteLine($"def __getitem__(self, key: {keyParamType}) -> {valueReturnType}: ...");
        return keyParamType;
    }

    public static void WriteMutableMapPythonSpecialMethods(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        IReadOnlyDictionary<string, string> packageMap,
        string keyParamType
    )
    {
        var setMethod = type.GetMethod("Insert", 2);
        var setNullabilityInfo = nullabilityMap.GetValueOrDefault(
            setMethod.Signature,
            new MethodNullabilityInfo(setMethod.Method)
        );
        var valParamType = setMethod
            .Method.Parameters[1]
            .ToPyInParamTyping(
                ns,
                setNullabilityInfo.Parameters[1].Type,
                packageMap,
                setMethod.GenericArgMap
            );

        w.WriteLine(
            $"def __setitem__(self, key: {keyParamType}, value: {valParamType}) -> None: ..."
        );
        w.WriteLine($"def __delitem__(self, key: {keyParamType}) -> None: ...");
    }
}
