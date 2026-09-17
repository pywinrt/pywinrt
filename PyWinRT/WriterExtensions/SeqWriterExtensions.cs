using System.CodeDom.Compiler;
using System.Collections.ObjectModel;

static class SeqWriterExtensions
{
    public static void WriteSeqPythonSpecialMethods(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        IReadOnlyDictionary<string, string> packageMap
    )
    {
        var method = type.GetMethod("GetAt", 1);
        var nullabilityInfo = nullabilityMap.GetValueOrDefault(
            method.Signature,
            new MethodNullabilityInfo(method.Method)
        );
        var elementType = method.Method.ToPyReturnTyping(
            ns,
            nullabilityInfo,
            packageMap,
            method.GenericArgMap
        );

        w.WriteLine("def __len__(self) -> int: ...");
        w.WriteLine($"def __iter__(self) -> _cabc.Iterator[{elementType}]: ...");
        w.WriteLine("@typing.overload");
        w.WriteLine($"def __getitem__(self, index: typing.SupportsIndex) -> {elementType}: ...");
        w.WriteLine("@typing.overload");
        w.WriteLine(
            $"def __getitem__(self, index: slice) -> winrt.system.Array[{elementType}]: ..."
        );
    }

    public static void WriteMutableSeqPythonSpecialMethods(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        IReadOnlyDictionary<string, string> packageMap
    )
    {
        var setMethod = type.GetMethod("SetAt", 2);
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

        w.WriteLine("@typing.overload");
        w.WriteLine($"def __delitem__(self, index: typing.SupportsIndex) -> None: ...");
        w.WriteLine("@typing.overload");
        w.WriteLine($"def __delitem__(self, index: slice) -> None: ...");
        w.WriteLine("@typing.overload");
        w.WriteLine(
            $"def __setitem__(self, index: typing.SupportsIndex, value: {valParamType}) -> None: ..."
        );
        w.WriteLine("@typing.overload");
        w.WriteLine(
            $"def __setitem__(self, index: slice, value: _cabc.Iterable[{valParamType}]) -> None: ..."
        );
    }
}
