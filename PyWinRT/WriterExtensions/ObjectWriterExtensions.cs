using System.CodeDom.Compiler;
using System.Collections.ObjectModel;
using Mono.Cecil;

static class ObjectWriterExtensions
{
    /// <summary>
    /// Writes the __new__ of a class with no public constructor, which the
    /// runtime refuses to create, as it refuses a Python class derived from one.
    /// </summary>
    /// <remarks>
    /// Without it, the class would have a callable __new__ from object or from
    /// its base class, though a WinRT constructor is not inherited. A parameter
    /// that no argument can be given for is what both mypy and pyright refuse a
    /// call for: neither refuses the call for a NoReturn return, and mypy does
    /// not refuse it for typeshed's <c>__new__: None</c>. Self rather than
    /// NoReturn keeps the code after a refused call checked. The docstring is
    /// what an editor shows for the call.
    /// </remarks>
    static void WriteNoConstructor(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine("def __new__(cls, _: typing.Never, /) -> typing.Self:");
        w.Indent++;
        w.WriteLine(
            type.IsStatic
                ? "\"\"\"A class of static members only, so there is nothing to create.\"\"\""
                : "\"\"\"WinRT gives this class no constructor; an instance comes from a method or property that returns one.\"\"\""
        );
        w.Indent--;
    }

    public static void WritePythonClassTyping(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        IReadOnlyDictionary<string, string> packageMap
    )
    {
        var metaclass = "";

        if (type.PyRequiresMetaclass)
        {
            var baseType = "winrt._winrt.Object_Static";

            if (type.Type.BaseType is TypeReference b && b.Namespace != "System")
            {
                baseType =
                    $"{b.ToPyTypeName(ns, new TypeRefNullabilityInfo(b), packageMap)}_Static";
            }

            if (!type.IsComposable)
            {
                w.WriteLine("@typing.final");
            }

            w.WriteLine($"class {type.Name}_Static({baseType}):");
            w.Indent++;

            var hasMembers = false;

            // pyright checks the keywords of a class statement against the
            // metaclass's __new__ when the metaclass declares one, and
            // ABCMeta's, which winrt._winrt.Object_Static inherits in the
            // stubs, takes any keyword, so the runtime_class_name keyword
            // that __init_subclass__ takes below is declared here as well.
            if (type.IsComposable && type.Type.BaseType is null or { FullName: "System.Object" })
            {
                w.WriteLine(
                    $"def __new__(mcls, name: str, bases: tuple[type, ...], namespace: dict[str, typing.Any], /, *, runtime_class_name: str = ...) -> {type.Name}_Static: ..."
                );

                hasMembers = true;
            }

            foreach (var group in type.MethodGroups.Where(g => g.IsStatic))
            {
                // The stubs declare winrt._winrt.Object_Static as an ABCMeta,
                // so a static register() is an incompatible override of
                // ABCMeta.register(), which is not there at run time. mypy
                // reports an overloaded method on its first decorator and
                // pyright on its last definition. A method that is not
                // overloaded is one line, where pyright honors mypy's ignore.
                var shadowsAbcMeta = group.PyName == "register";
                var mypyIgnore = shadowsAbcMeta ? "  # type: ignore[override]" : "";
                var pyrightIgnore = shadowsAbcMeta
                    ? "  # pyright: ignore[reportIncompatibleMethodOverride]"
                    : "";

                foreach (var method in group.Overloads)
                {
                    if (group.IsOverloaded)
                    {
                        w.WriteLine(
                            $"@typing.overload{(method == group.Overloads[0] ? mypyIgnore : "")}"
                        );
                    }

                    if (
                        type.IsComposable
                        && !group.IsOverridable
                        && method.IsExclusiveTo
                        // mypy rule: @typing.final can only be applied to the first overload
                        && method == group.Overloads[0]
                    )
                    {
                        w.WriteLine("@typing.final");
                    }

                    w.WritePythonMethodTyping(
                        method,
                        ns,
                        nullabilityMap,
                        packageMap,
                        "cls",
                        ignoreComment: group.IsOverloaded
                            ? method == group.Overloads[^1]
                                ? pyrightIgnore
                                : ""
                            : mypyIgnore
                    );

                    hasMembers = true;
                }

                foreach (var alias in group.Aliases)
                {
                    foreach (var method in alias.Methods)
                    {
                        if (alias.Methods.Count > 1)
                        {
                            w.WriteLine("@typing.overload");
                        }

                        if (
                            type.IsComposable
                            && !group.IsOverridable
                            && method.IsExclusiveTo
                            // mypy rule: @typing.final can only be applied to the first overload
                            && method == alias.Methods[0]
                        )
                        {
                            w.WriteLine("@typing.final");
                        }

                        w.WritePythonMethodTyping(
                            method,
                            ns,
                            nullabilityMap,
                            packageMap,
                            "cls",
                            aliasPyName: alias.PyName,
                            aliasTarget: group.PyName
                        );
                    }
                }
            }

            foreach (var evt in type.Events.Where(e => e.IsStatic))
            {
                if (type.IsComposable)
                {
                    w.WriteLine("@typing.final");
                }

                w.WritePythonMethodTyping(evt.AddMethod, ns, nullabilityMap, packageMap, "cls");

                if (type.IsComposable)
                {
                    w.WriteLine("@typing.final");
                }

                w.WritePythonMethodTyping(evt.RemoveMethod, ns, nullabilityMap, packageMap, "cls");

                hasMembers = true;
            }

            foreach (var prop in type.Properties.Where(p => p.GetMethod.IsStatic))
            {
                w.WritePythonPropertyTyping(type, prop, ns, nullabilityMap, packageMap, "cls");

                hasMembers = true;
            }

            if (!hasMembers)
            {
                w.WriteLine("...");
            }

            w.Indent--;
            w.WriteBlankLine();

            metaclass = $", metaclass={type.Name}_Static";
        }

        if (type.Category == Category.Interface)
        {
            // This is to make mypy happy so that we have to import it in the
            // __init__.py files. It is just used internally though, so not
            // doing full typing.
            w.WriteLine("@typing.final");
            w.WriteLine($"class {type.PyWrapperTypeName}: ...");
            w.WriteBlankLine();

            // This is the type users will actually use.
            w.WritePythonImplementsInterfaceTyping(type, ns, nullabilityMap, packageMap);
            return;
        }

        var collection = "";

        if (type.IsPyMapping)
        {
            var method = type.GetMethod("Lookup", 1);
            var nullabilityInfo = nullabilityMap.GetValueOrDefault(
                method.Signature,
                new MethodNullabilityInfo(method.Method)
            );
            var keyType = method
                .Method.Parameters[0]
                .ToPyInParamTyping(
                    ns,
                    nullabilityInfo.Parameters[0].Type,
                    packageMap,
                    method.GenericArgMap
                );
            var valueType = method.Method.ToPyReturnTyping(
                ns,
                nullabilityInfo,
                packageMap,
                method.GenericArgMap
            );

            if (type.IsPyMutableMapping)
            {
                collection = $", _cabc.MutableMapping[{keyType}, {valueType}]";
            }
            else
            {
                collection = $", _cabc.Mapping[{keyType}, {valueType}]";
            }
        }
        else if (type.IsPySequence)
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

            if (type.IsPyMutableSequence)
            {
                collection = $", _cabc.MutableSequence[{elementType}]";
            }
            else
            {
                collection = $", _cabc.Sequence[{elementType}]";
            }
        }

        var interfaceTypes = type.Interfaces.Where(i => !i.IsPythonCollection);

        if (type.Category == Category.Interface)
        {
            interfaceTypes = interfaceTypes.Prepend(type.Type);
        }

        // NB: although there are no collection types here, usePythonCollectionTypes
        // is also used for IBuffer. Due to metaclass conflicts, we need to
        // inherit from IBuffer instead of the projection type winrt.system.Buffer.
        var interfaces = string.Join(
            "",
            interfaceTypes.Select(i =>
                $", {i.ToPyTypeName(ns, new TypeRefNullabilityInfo(i), packageMap, usePythonCollectionTypes: false)}"
            )
        );

        if (!type.IsComposable)
        {
            w.WriteLine("@typing.final");
        }

        if (type.IsDeprecated)
        {
            w.WriteDeprecated(type.DeprecatedMessage);
        }

        // Every interface derives from winrt.system.Object in the stubs, so
        // Object comes after a class's interfaces: ahead of them is an order no
        // MRO allows.
        var bases = type.Type.BaseType is { FullName: not "System.Object" } explicitBase
            ? explicitBase.ToPyTypeName(ns, new TypeRefNullabilityInfo(explicitBase), packageMap)
                + interfaces
                + collection
            : $"{interfaces}{collection}, winrt.system.Object"[2..];

        w.WriteLine($"class {type.Name}{type.Type.PyTypeParameters}({bases}{metaclass}):");
        w.Indent++;

        if (type.IsStatic)
        {
            w.WriteNoConstructor(type);
            w.Indent--;
            w.WriteBlankLine();
            return;
        }

        var didWriteLine = false;

        if (type.IsGeneric)
        {
            w.WriteLine("def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...");
            didWriteLine = true;
        }

        // A Python class derived from a composable class is the only kind of
        // class Python can make a runtime class of, so the name its instances
        // give WinRT is taken there rather than by winrt.system.Object, which
        // an implementation of an interface derives from too. A composable
        // class only derives from another, so the first in a hierarchy is
        // where it is declared.
        if (type.IsComposable && type.Type.BaseType is null or { FullName: "System.Object" })
        {
            w.WriteLine(
                "def __init_subclass__(cls, *, runtime_class_name: str = ...) -> None: ..."
            );
            didWriteLine = true;
        }

        if (type.IsPyCloseable)
        {
            w.WriteLine("def __enter__(self) -> typing.Self: ...");
            w.WriteLine(
                "def __exit__(self, exc_type: type[BaseException] | None, exc_value: BaseException | None, traceback: types.TracebackType | None) -> None: ..."
            );
            didWriteLine = true;
        }

        if (type.IsPyBuffer)
        {
            w.WriteLine("def __buffer__(self, flags: int, /) -> memoryview: ...");
            w.WriteLine("def __release_buffer__(self, view: memoryview, /) -> None: ...");
            w.WriteLine("def __len__(self) -> int: ...");
            didWriteLine = true;
        }

        if (type.IsPyMemoryBuffer)
        {
            w.WriteLine("def __buffer__(self, flags: int, /) -> memoryview: ...");
            w.WriteLine("def __release_buffer__(self, view: memoryview, /) -> None: ...");
            didWriteLine = true;
        }

        if (type.IsPyMapping)
        {
            w.WriteMapPythonSpecialMethods(
                type,
                ns,
                nullabilityMap,
                packageMap,
                out var keyParamType
            );

            if (type.IsPyMutableMapping)
            {
                w.WriteMutableMapPythonSpecialMethods(
                    type,
                    ns,
                    nullabilityMap,
                    packageMap,
                    keyParamType
                );
            }

            didWriteLine = true;
        }
        else if (type.IsPySequence)
        {
            w.WriteSeqPythonSpecialMethods(
                type,
                ns,
                nullabilityMap,
                packageMap,
                isMutable: type.IsPyMutableSequence
            );

            if (type.IsPyMutableSequence)
            {
                w.WriteMutableSeqPythonSpecialMethods(type, ns, nullabilityMap, packageMap);
            }

            didWriteLine = true;
        }
        else if (type.IsPyIterator)
        {
            var prop = type.Properties.Single(p => p.Name == "Current");
            var nullabilityInfo = nullabilityMap.GetValueOrDefault(
                prop.GetMethod.Signature,
                new MethodNullabilityInfo(prop.GetMethod.Method)
            );
            var nextType = prop.Property.PropertyType.ToPyTypeName(
                ns,
                nullabilityInfo.Return.Type,
                packageMap,
                useBufferProtocol: false
            );
            w.WriteLine("def __iter__(self) -> typing.Self: ...");
            w.WriteLine($"def __next__(self) -> {nextType}: ...");
            didWriteLine = true;
        }
        else if (type.IsPyIterable)
        {
            var method = type.GetMethod("First", 0);
            var nullabilityInfo = nullabilityMap.GetValueOrDefault(
                method.Signature,
                new MethodNullabilityInfo(method.Method)
            );
            var iterType = method.Method.ToPyReturnTyping(
                ns,
                nullabilityInfo,
                packageMap,
                method.GenericArgMap
            );

            // HACK: there isn't a nice way to get the resolved generic arg type,
            // so scrape it from the iter type.
            var elementType = iterType[(iterType.IndexOf('[', StringComparison.Ordinal) + 1)..^1];

            w.WriteLine($"def __iter__(self) -> _cabc.Iterator[{elementType}]: ...");
            didWriteLine = true;
        }

        foreach (var ctor in type.Constructors)
        {
            var paramList = "";
            var nullabilityInfo = nullabilityMap.GetValueOrDefault(
                ctor.Signature,
                new MethodNullabilityInfo(ctor.Method)
            );

            if (ctor.Method.Parameters.Any(p => p.IsPythonInParam))
            {
                paramList =
                    $", {string.Join(", ", ctor.Method.Parameters.Where(p => p.IsPythonInParam).Select(p => $"{p.Name.ToPythonIdentifier()}: {p.ToPyInParamTyping(ns, nullabilityInfo.Parameters[p.Index].Type, packageMap)}"))}";
            }

            if (type.Constructors.Count(m => m.Name == ctor.Name) > 1)
            {
                w.WriteLine("@typing.overload");
            }

            // a deprecated class already reports its instantiation
            if (ctor.IsDeprecated && !type.IsDeprecated)
            {
                w.WriteDeprecated(ctor.DeprecatedMessage);
            }

            w.WriteLine($"def __new__(cls{paramList}) -> typing.Self: ...");
            didWriteLine = true;
        }

        if (type.Constructors.Count == 0)
        {
            w.WriteNoConstructor(type);
            didWriteLine = true;
        }

        foreach (var group in type.MethodGroups.Where(g => !g.IsStatic))
        {
            foreach (var method in group.StubOverloads)
            {
                if (group.IsOverloaded)
                {
                    w.WriteLine("@typing.overload");
                }

                if (
                    type.IsComposable
                    && !group.IsOverridable
                    && method.IsExclusiveTo
                    // mypy rule: @typing.final can only be applied to the first overload
                    && method == group.StubOverloads[0]
                )
                {
                    // HACK: There are a couple of problematic methods. Subclasses of
                    // DependencyObject like to override SetValue with a different
                    // parameter type. Subclasses of FlyoutBase like to override ShowAt.
                    var typeIgnore = method.IsProblematicOverride ? "  # type: ignore[misc]" : "";

                    w.WriteLine($"@typing.final{typeIgnore}");
                }

                w.WritePythonMethodTyping(method, ns, nullabilityMap, packageMap);
                didWriteLine = true;
            }

            foreach (var alias in group.Aliases)
            {
                foreach (var method in alias.Methods)
                {
                    if (alias.Methods.Count > 1)
                    {
                        w.WriteLine("@typing.overload");
                    }

                    if (
                        type.IsComposable
                        && !group.IsOverridable
                        && method.IsExclusiveTo
                        // mypy rule: @typing.final can only be applied to the first overload
                        && method == alias.Methods[0]
                    )
                    {
                        var typeIgnore = method.IsProblematicOverride
                            ? "  # type: ignore[misc]"
                            : "";

                        w.WriteLine($"@typing.final{typeIgnore}");
                    }

                    w.WritePythonMethodTyping(
                        method,
                        ns,
                        nullabilityMap,
                        packageMap,
                        aliasPyName: alias.PyName,
                        aliasTarget: group.PyName
                    );

                    didWriteLine = true;
                }
            }
        }

        foreach (var evt in type.Events.Where(e => !e.IsStatic))
        {
            if (type.IsComposable)
            {
                w.WriteLine("@typing.final");
            }

            w.WritePythonMethodTyping(evt.AddMethod, ns, nullabilityMap, packageMap);

            if (type.IsComposable)
            {
                w.WriteLine("@typing.final");
            }

            w.WritePythonMethodTyping(evt.RemoveMethod, ns, nullabilityMap, packageMap);
            didWriteLine = true;
        }

        foreach (var prop in type.Properties.Where(p => !p.IsStatic))
        {
            w.WritePythonPropertyTyping(type, prop, ns, nullabilityMap, packageMap);

            didWriteLine = true;
        }

        if (!didWriteLine)
        {
            w.WriteLine("...");
        }

        w.Indent--;
        w.WriteBlankLine();
    }
}
