using System.CodeDom.Compiler;
using System.Collections.ObjectModel;
using Mono.Cecil;

static class InterfaceWriterExtensions
{
    public static void WritePythonImplementsInterfaceTyping(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        IReadOnlyDictionary<string, string> packageMap
    )
    {
        // NB: overloads of the same method can come from more than one required
        // interface, in which case all of them have to be declared here so that
        // the complete set of overloads is visible to type checkers
        var methodGroups = type
            .MethodGroups.Where(g =>
                g.Overloads.Any(m => m.Method.DeclaringType.FullName == type.Type.FullName)
            )
            .ToList();
        var methods = methodGroups.SelectMany(g => g.Overloads);
        var events = type.Events.Where(e => e.Event.DeclaringType.FullName == type.Type.FullName);
        var properties = type.Properties.Where(p =>
            p.Property.DeclaringType.FullName == type.Type.FullName
        );

        var hasMembers = methods.Any() || events.Any() || properties.Any();

        var baseTypes = type
            .Interfaces.Select(i =>
                i.ToPyTypeName(
                    ns,
                    new TypeRefNullabilityInfo(i),
                    packageMap,
                    usePythonCollectionTypes: false
                )
            )
            .ToList();

        var mixin = type switch
        {
            { Namespace: "Windows.Foundation.Collections", Name: "IMap" } =>
                "winrt._winrt.MutableMapping[K, V]",
            { Namespace: "Windows.Foundation.Collections", Name: "IMapView" } =>
                "winrt._winrt.Mapping[K, V]",
            { Namespace: "Windows.Foundation.Collections", Name: "IVector" } =>
                "winrt._winrt.MutableSequence[T]",
            { Namespace: "Windows.Foundation.Collections", Name: "IVectorView" } =>
                "winrt._winrt.Sequence[T]",
            _ => null,
        };

        if (mixin is not null)
        {
            baseTypes.Add(mixin);
        }

        baseTypes.Add("winrt._winrt.IInspectable");

        // typing.Generic is written last because that is where a stub is
        // expected to name it and where the type parameters read best
        if (type.IsGeneric && !type.Interfaces.Any(i => i.ContainsGenericParameter))
        {
            baseTypes.Add(
                $"typing.Generic[{string.Join(", ", type.Type.GenericParameters.Select(p => p.ToPyTypeName(ns, new TypeRefNullabilityInfo(p), packageMap)))}]"
            );
        }

        // work around https://github.com/python/mypy/issues/17091
        // we can't use abc.ABCMeta because it will cause errors about conflicting metaclasses
        var typeIgnore = hasMembers ? "" : "  # type: ignore[misc]";

        w.WriteLine($"class {type.Name}({string.Join(", ", baseTypes)}):{typeIgnore}");
        w.Indent++;

        if (type.Namespace == "Windows.Foundation" && type.Name == "IClosable")
        {
            w.WriteLine("def __enter__(self) -> typing.Self: ...");
            w.WriteLine(
                "def __exit__(self, exc_type: type[BaseException] | None, exc_value: BaseException | None, traceback: types.TracebackType | None) -> None: ..."
            );
        }

        if (
            (type.Namespace == "Windows.Foundation" && type.Name == "IMemoryBufferReference")
            || (type.Namespace == "Windows.Storage.Streams" && type.Name == "IBuffer")
        )
        {
            w.WriteLine("def __buffer__(self, flags: int, /) -> memoryview: ...");
            w.WriteLine("def __release_buffer__(self, view: memoryview, /) -> None: ...");
        }

        if (
            type.Namespace == "Windows.Foundation.Collections"
            && (type.Name == "IMap" || type.Name == "IMapView")
        )
        {
            w.WriteMapPythonSpecialMethods(
                type,
                ns,
                nullabilityMap,
                packageMap,
                out var keyParamType
            );

            if (type.Name == "IMap")
            {
                w.WriteMutableMapPythonSpecialMethods(
                    type,
                    ns,
                    nullabilityMap,
                    packageMap,
                    keyParamType
                );
            }
        }

        if (
            type.Namespace == "Windows.Foundation.Collections"
            && (type.Name == "IVector" || type.Name == "IVectorView")
        )
        {
            w.WriteSeqPythonSpecialMethods(type, ns, nullabilityMap, packageMap);

            if (type.Name == "IVector")
            {
                w.WriteMutableSeqPythonSpecialMethods(type, ns, nullabilityMap, packageMap);
            }
        }

        if (
            type.Namespace == "Windows.Foundation"
            && (
                type.Name == "IAsyncAction"
                || type.Name == "IAsyncActionWithProgress"
                || type.Name == "IAsyncOperation"
                || type.Name == "IAsyncOperationWithProgress"
            )
        )
        {
            var returnType = "None";

            if (
                type.Namespace == "Windows.Foundation"
                && (type.Name == "IAsyncOperation" || type.Name == "IAsyncOperationWithProgress")
            )
            {
                returnType = type
                    .Type.GenericParameters[0]
                    .ToPyTypeName(
                        ns,
                        new TypeRefNullabilityInfo(type.Type.GenericParameters[0]),
                        packageMap
                    );
            }

            var statusProp = type.Properties.Single(p => p.Name == "Status");
            var statusType = statusProp.GetMethod.Method.ReturnType;
            var statusTypeName = statusType.ToPyTypeName(
                ns,
                new TypeRefNullabilityInfo(statusType),
                packageMap
            );

            w.WriteLine("@typing.final");
            w.WriteLine(
                // Generator's send and return types both default to None, so
                // IAsyncAction only needs the yield type.
                $"def __await__(self) -> _cabc.Generator[typing.Any{(returnType == "None" ? "" : $", None, {returnType}")}]: ..."
            );
            w.WriteLine("@typing.final");
            w.WriteLine($"def get(self) -> {returnType}: ...");
            w.WriteLine("@typing.final");
            w.WriteLine($"def wait(self, timeout: float) -> {statusTypeName}: ...");
        }

        foreach (var group in methodGroups)
        {
            foreach (var method in group.Overloads)
            {
                // mypy rule: all overloads of an abstract method must be abstract
                if (group.IsOverloaded)
                {
                    w.WriteLine("@typing.overload");
                }

                w.WritePythonMethodTyping(method, ns, nullabilityMap, packageMap, isAbstract: true);
            }

            // aliases are not abstract so that implementations don't have to
            // provide the deprecated name
            foreach (var alias in group.Aliases)
            {
                foreach (var method in alias.Methods)
                {
                    if (alias.Methods.Count > 1)
                    {
                        w.WriteLine("@typing.overload");
                    }

                    w.WritePythonMethodTyping(
                        method,
                        ns,
                        nullabilityMap,
                        packageMap,
                        aliasPyName: alias.PyName,
                        aliasTarget: group.PyName
                    );
                }
            }
        }

        foreach (var evt in events)
        {
            w.WritePythonMethodTyping(
                evt.AddMethod,
                ns,
                nullabilityMap,
                packageMap,
                isAbstract: true
            );
            w.WritePythonMethodTyping(
                evt.RemoveMethod,
                ns,
                nullabilityMap,
                packageMap,
                isAbstract: true
            );
        }

        foreach (var prop in properties)
        {
            w.WritePythonPropertyTyping(
                type,
                prop,
                ns,
                nullabilityMap,
                packageMap,
                isAbstract: true
            );
        }

        if (!hasMembers)
        {
            w.WriteLine("...");
        }

        w.Indent--;
        w.WriteBlankLine();
    }
}
