using System.CodeDom.Compiler;
using System.Collections.ObjectModel;
using Mono.Cecil;

static class ObjectWriterExtensions
{
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
            var baseType = "winrt._winrt.IInspectable_Static";

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

            foreach (var method in type.Methods.Where(m => m.IsStatic))
            {
                if (type.Methods.Count(m => m.Name == method.Name) > 1)
                {
                    w.WriteLine("@typing.overload");
                }

                if (
                    type.IsComposable
                    && !method.IsOverridable
                    && method.IsExclusiveTo
                    // mypy rule: @typing.final can only be applied to the first overload
                    && method == type.Methods.FirstOrDefault(m => m.Name == method.Name, method)
                )
                {
                    w.WriteLine("@typing.final");
                }

                w.WritePythonMethodTyping(method, ns, nullabilityMap, packageMap, "cls");

                hasMembers = true;
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
                w.WriteLine("pass");
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
            var method = type.Methods.Single(m => m.Name == "Lookup");
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
                collection = $", winrt._winrt.MutableMapping[{keyType}, {valueType}]";
            }
            else
            {
                collection = $", winrt._winrt.Mapping[{keyType}, {valueType}]";
            }
        }
        else if (type.IsPySequence)
        {
            var method = type.Methods.Single(m => m.Name == "GetAt");
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
                collection = $", winrt._winrt.MutableSequence[{elementType}]";
            }
            else
            {
                collection = $", winrt._winrt.Sequence[{elementType}]";
            }
        }

        var interfaceTypes = type.Interfaces.Where(i => !i.IsPythonCollection());

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

        var generic = "";

        // typing.Generic is redundant when there is a collection type
        if (type.IsGeneric && string.IsNullOrEmpty(collection))
        {
            generic =
                $", typing.Generic[{string.Join(", ", type.Type.GenericParameters.Select(p => p.ToPyTypeName(ns, new TypeRefNullabilityInfo(p), packageMap)))}]";
        }

        if (!type.IsComposable)
        {
            w.WriteLine("@typing.final");
        }

        w.WriteLine(
            $"class {type.Name}({type.Type.BaseType?.ToPyTypeName(ns, new TypeRefNullabilityInfo(type.Type.BaseType), packageMap) ?? "winrt.system.Object"}{interfaces}{collection}{generic}{metaclass}):"
        );
        w.Indent++;

        if (type.IsStatic)
        {
            w.WriteLine("pass");
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

        if (type.IsPyCloseable)
        {
            w.WriteLine("def __enter__(self: Self) -> Self: ...");
            w.WriteLine(
                "def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ..."
            );
            didWriteLine = true;
        }

        if (type.IsPyStringable)
        {
            w.WriteLine("def __str__(self) -> str: ...");
            didWriteLine = true;
        }

        if (type.IsPyBuffer)
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
            w.WriteSeqPythonSpecialMethods(type, ns, nullabilityMap, packageMap);

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
                packageMap
            );
            w.WriteLine("def __iter__(self: Self) -> Self: ...");
            w.WriteLine($"def __next__(self) -> {nextType}: ...");
            didWriteLine = true;
        }
        else if (type.IsPyIterable)
        {
            var method = type.Methods.Single(m => m.Name == "First");
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

            w.WriteLine($"def __iter__(self) -> typing.Iterator[{elementType}]: ...");
            didWriteLine = true;
        }

        foreach (var ctor in type.Constructors)
        {
            var paramList = "";
            var nullabilityInfo = nullabilityMap.GetValueOrDefault(
                ctor.Signature,
                new MethodNullabilityInfo(ctor.Method)
            );

            if (ctor.Method.Parameters.Any(p => p.IsPythonInParam()))
            {
                paramList =
                    $", {string.Join(", ", ctor.Method.Parameters.Where(p => p.IsPythonInParam()).Select(p => $"{p.Name.ToPythonIdentifier()}: {p.ToPyInParamTyping(ns, nullabilityInfo.Parameters[p.Index].Type, packageMap)}"))}";
            }

            if (type.Constructors.Count(m => m.Name == ctor.Name) > 1)
            {
                w.WriteLine("@typing.overload");
            }

            w.WriteLine($"def __new__(cls: typing.Type[Self]{paramList}) -> Self: ...");
            didWriteLine = true;
        }

        foreach (var method in type.Methods.Where(m => !m.IsStatic))
        {
            if (type.Methods.Count(m => m.Name == method.Name) > 1)
            {
                w.WriteLine("@typing.overload");
            }

            if (
                type.IsComposable
                && !method.IsOverridable
                && method.IsExclusiveTo
                // mypy rule: @typing.final can only be applied to the first overload
                && method == type.Methods.FirstOrDefault(m => m.Name == method.Name, method)
            )
            {
                // HACK: There are a couple of problematic methods. Subclasses of
                // DependencyObject like to override SetValue with a different
                // parameter type. Subclasses of FlyoutBase like to override ShowAt.
                var typeIgnore = method.IsProblematicOverride() ? "  # type: ignore[misc]" : "";

                w.WriteLine($"@typing.final{typeIgnore}");
            }

            w.WritePythonMethodTyping(method, ns, nullabilityMap, packageMap);
            didWriteLine = true;
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
            w.WriteLine("pass");
        }

        w.Indent--;
        w.WriteBlankLine();
    }

    public static void WriteComposableTypeImpl(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"struct PyWinrt{type.Name};");
        w.WriteLine(
            $"using BasePyWinrt{type.Name} = {type.CppWinrtType}T<PyWinrt{type.Name}, py::IPywinrtObject>;"
        );
        w.WriteBlankLine();

        w.WriteLine($"struct PyWinrt{type.Name} : py::py_obj_ref, BasePyWinrt{type.Name}");
        w.WriteBlock(
            () =>
            {
                foreach (var ctor in type.Constructors)
                {
                    var paramList = string.Join(
                        ", ",
                        ctor.Method.Parameters.Select(p =>
                                $"{p.ParameterType.ToCppTypeName()} {p.Name}"
                            )
                            .Prepend("PyObject* py_obj")
                    );
                    var argList = string.Join(", ", ctor.Method.Parameters.Select(p => p.Name));

                    w.WriteLine(
                        $"PyWinrt{type.Name}({paramList}) : py::py_obj_ref(py_obj), BasePyWinrt{type.Name}({argList}) {{}}"
                    );
                }

                w.WriteBlankLine();
                w.WriteLine("int32_t __stdcall GetPyObject(PyObject*& obj) override");
                w.WriteBlock(() =>
                {
                    w.WriteLine("obj = py::py_obj_ref::get_py_obj();");
                    w.WriteLine("return 0;");
                });

                w.WriteBlankLine();
                w.WriteLine(
                    "int32_t __stdcall GetComposableInner(winrt::Windows::Foundation::IInspectable& inner) override"
                );
                w.WriteBlock(() =>
                {
                    if (type.IsComposable)
                    {
                        w.WriteLine("inner = m_inner;");
                        w.WriteLine("return winrt::impl::error_ok;");
                    }
                    else
                    {
                        w.WriteLine("return winrt::impl::error_not_implemented;");
                    }
                });

                w.WriteBlankLine();
                w.WriteLine(
                    $"static void toggle_reference(PyWinrt{type.Name}* instance, bool is_last_reference)"
                );
                w.WriteBlock(
                    () =>
                        w.WriteLine(
                            "py::py_obj_ref::toggle_reference(instance, is_last_reference);"
                        )
                );

                w.WriteBlankLine();
                w.WriteLine(
                    "int32_t query_interface_tearoff(winrt::guid const& id, void** result) const noexcept override"
                );
                w.WriteBlock(
                    () => w.WriteLine("return py::py_obj_ref::query_interface_tearoff(id, result);")
                );

                w.WriteBlankLine();
                w.WriteLine("std::vector<winrt::guid> get_iids_tearoff() const noexcept override");
                w.WriteBlock(() => w.WriteLine("return py::py_obj_ref::get_iids_tearoff();"));

                foreach (var method in type.Methods.Where(m => m.IsOverridable))
                {
                    w.WriteBlankLine();

                    var returnType = method.Method.ReturnType.ToCppTypeName();
                    var paramList = string.Join(
                        ", ",
                        method.Method.Parameters.Select(p => p.ToDelegateParam())
                    );

                    w.WriteLine($"{returnType} {method.CppName}({paramList})");
                    w.WriteBlock(
                        () =>
                            w.WriteDelegateInvoke(
                                method.Method,
                                "method.get()",
                                () =>
                                {
                                    w.WriteLine("py::pyobj_handle self{get_py_obj()};");
                                    w.WriteBlankLine();
                                    w.WriteLine(
                                        $"py::pyobj_handle method{{PyObject_GetAttrString(self.get(), \"{method.PyName}\")}};"
                                    );
                                    w.WriteLine("if (!method)");
                                    w.WriteBlock(() => w.WriteLine("throw python_exception();"));
                                    w.WriteBlankLine();
                                }
                            )
                    );
                }
            },
            ";"
        );
    }
}
