using System.CodeDom.Compiler;
using System.Collections.ObjectModel;
using Mono.Cecil;

static class InterfaceWriterExtensions
{
    private static void WriteImplementsInterfaceCppType(
        this IndentedTextWriter w,
        ProjectedType type
    )
    {
        w.WriteLine(
            $"struct Implements{type.Name} : py::ImplementsInterfaceT<Implements{type.Name}, {type.CppWinrtType}>"
        );
        w.WriteBlock(
            () =>
            {
                w.WriteLine($"Implements{type.Name}() = delete;");
                w.WriteLine(
                    $"Implements{type.Name}(PyObject* py_obj, winrt::impl::inspectable_abi* runtime_class) : py::ImplementsInterfaceT<Implements{type.Name}, {type.CppWinrtType}>(py_obj, runtime_class)"
                );
                w.WriteBlock(() => { });

                foreach (var method in type.Methods)
                {
                    var paramList = string.Join(
                        ", ",
                        method.Method.Parameters.Select(p =>
                            p.ToDelegateParam(method.GenericArgMap)
                        )
                    );

                    w.WriteBlankLine();
                    w.WriteLine($"auto {method.CppName}({paramList})");
                    w.WriteBlock(
                        () =>
                            w.WriteDelegateInvoke(
                                method.Method,
                                "method.get()",
                                () =>
                                {
                                    w.WriteLine("py::pyobj_handle self{this->get_py_obj()};");
                                    w.WriteBlankLine();
                                    w.WriteLine(
                                        $"py::pyobj_handle method{{PyObject_GetAttrString(self.get(), \"{method.PyName}\")}};"
                                    );
                                    w.WriteLine("if (!method)");
                                    w.WriteBlock(() => w.WriteLine("throw python_exception();"));
                                    w.WriteBlankLine();
                                },
                                ensureGil: false,
                                method.GenericArgMap
                            )
                    );
                }

                foreach (var prop in type.Properties)
                {
                    w.WriteBlankLine();
                    w.WriteLine($"auto {prop.Name}()");
                    w.WriteBlock(() =>
                    {
                        w.WriteLine("try");
                        w.WriteBlock(() =>
                        {
                            w.WriteLine("py::pyobj_handle self{this->get_py_obj()};");
                            w.WriteBlankLine();
                            w.WriteLine(
                                $"py::pyobj_handle value{{PyObject_GetAttrString(self.get(), \"{prop.Name.ToPythonIdentifier()}\")}};"
                            );
                            w.WriteLine("if (!value)");
                            w.WriteBlock(() => w.WriteLine("throw python_exception();"));
                            w.WriteBlankLine();

                            var returnType = prop.GetMethod.Method.ReturnType;
                            if (returnType.IsArray)
                            {
                                w.WriteLine(
                                    $"auto return_buf = py::convert_to<py::pybuf_view<{returnType.ToCppTypeName(prop.GenericArgMap)}, false>>(value.get());"
                                );
                                w.WriteLine(
                                    $"return winrt::com_array<{returnType.ToCppTypeName(prop.GenericArgMap)}>{{return_buf.begin(), return_buf.end()}};"
                                );
                            }
                            else
                            {
                                w.WriteLine(
                                    $"return py::convert_to<{returnType.ToCppTypeName(prop.GenericArgMap)}>(value.get());"
                                );
                            }
                        });
                        w.WriteLine("catch (python_exception)");
                        w.WriteBlock(() => w.WriteLine("py::write_unraisable_and_throw();"));
                    });

                    if (prop.SetMethod is null)
                    {
                        continue;
                    }

                    var setParamList = string.Join(
                        ", ",
                        prop.SetMethod.Method.Parameters.Select(p =>
                            p.ToDelegateParam(prop.SetMethod.GenericArgMap)
                        )
                    );

                    w.WriteBlankLine();
                    w.WriteLine($"void {prop.Name}({setParamList})");
                    w.WriteBlock(() =>
                    {
                        w.WriteLine("try");
                        w.WriteBlock(() =>
                        {
                            w.WriteLine("py::pyobj_handle self{this->get_py_obj()};");
                            w.WriteBlankLine();

                            w.WriteLine($"py::pyobj_handle value{{py::convert(param0)}};");
                            w.WriteLine("if (!value)");
                            w.WriteBlock(() => w.WriteLine("throw python_exception();"));
                            w.WriteBlankLine();

                            w.WriteLine(
                                $"if (PyObject_SetAttrString(self.get(), \"{prop.Name.ToPythonIdentifier()}\", value.get()) == -1)"
                            );
                            w.WriteBlock(() => w.WriteLine("throw python_exception();"));
                        });
                        w.WriteLine("catch (python_exception)");
                        w.WriteBlock(() => w.WriteLine("py::write_unraisable_and_throw();"));
                    });
                }

                foreach (var evt in type.Events)
                {
                    var addParamList = string.Join(
                        ", ",
                        evt.AddMethod.Method.Parameters.Select(p =>
                            p.ToDelegateParam(evt.AddMethod.GenericArgMap)
                        )
                    );

                    w.WriteBlankLine();
                    w.WriteLine($"auto {evt.Name}({addParamList})");
                    w.WriteBlock(
                        () =>
                            w.WriteDelegateInvoke(
                                evt.AddMethod.Method,
                                "method.get()",
                                () =>
                                {
                                    w.WriteLine("py::pyobj_handle self{this->get_py_obj()};");
                                    w.WriteBlankLine();
                                    w.WriteLine(
                                        $"py::pyobj_handle method{{PyObject_GetAttrString(self.get(), \"{evt.AddMethod.PyName}\")}};"
                                    );
                                    w.WriteLine("if (!method)");
                                    w.WriteBlock(() => w.WriteLine("throw python_exception();"));
                                    w.WriteBlankLine();
                                },
                                ensureGil: false,
                                evt.AddMethod.GenericArgMap
                            )
                    );

                    var removeParamList = string.Join(
                        ", ",
                        evt.RemoveMethod.Method.Parameters.Select(p =>
                            p.ToDelegateParam(evt.RemoveMethod.GenericArgMap)
                        )
                    );

                    w.WriteBlankLine();
                    w.WriteLine($"auto {evt.Name}({removeParamList})");
                    w.WriteBlock(
                        () =>
                            w.WriteDelegateInvoke(
                                evt.RemoveMethod.Method,
                                "method.get()",
                                () =>
                                {
                                    w.WriteLine("py::pyobj_handle self{this->get_py_obj()};");
                                    w.WriteBlankLine();
                                    w.WriteLine(
                                        $"py::pyobj_handle method{{PyObject_GetAttrString(self.get(), \"{evt.RemoveMethod.PyName}\")}};"
                                    );
                                    w.WriteLine("if (!method)");
                                    w.WriteBlock(() => w.WriteLine("throw python_exception();"));
                                    w.WriteBlankLine();
                                },
                                ensureGil: false,
                                evt.RemoveMethod.GenericArgMap
                            )
                    );
                }
            },
            ";"
        );
        w.WriteBlankLine();
    }

    public static void WriteImplementsInterfaceImpl(
        this IndentedTextWriter w,
        ProjectedType type,
        string moduleSuffix
    )
    {
        if (!type.IsGeneric)
        {
            w.WriteImplementsInterfaceCppType(type);
        }

        w.WriteLine(
            $"static PyObject* _guid_Implements{type.Name}(PyObject* /*unused*/, PyObject* /*unused*/) noexcept"
        );
        w.WriteBlock(
            () =>
                w.WriteTryCatch(() =>
                {
                    if (type.IsGeneric)
                    {
                        w.WriteLine(
                            "PyErr_SetString(PyExc_NotImplementedError, \"Generic types are not supported\");"
                        );
                        w.WriteLine("return nullptr;");
                    }
                    else
                    {
                        w.WriteLine($"return py::convert(winrt::guid_of<{type.CppWinrtType}>());");
                    }
                })
        );
        w.WriteBlankLine();

        w.WriteLine(
            $"static PyObject* _make_Implements{type.Name}(PyObject* /*unused*/, PyObject* args) noexcept"
        );
        w.WriteBlock(
            () =>
                w.WriteTryCatch(() =>
                {
                    w.WriteLine("PyObject* py_obj;");
                    w.WriteLine("winrt::impl::inspectable_abi* runtime_class;");
                    w.WriteBlankLine();
                    w.WriteLine("if (!PyArg_ParseTuple(args, \"On\", &py_obj, &runtime_class))");
                    w.WriteBlock(() => w.WriteLine("return nullptr;"));
                    w.WriteBlankLine();

                    if (type.IsGeneric)
                    {
                        w.WriteLine(
                            "PyErr_SetString(PyExc_NotImplementedError, \"Generic types are not supported\");"
                        );
                        w.WriteLine("return nullptr;");
                    }
                    else
                    {
                        w.WriteLine(
                            $"auto iface{{std::make_unique<Implements{type.Name}>(py_obj, runtime_class)}};"
                        );
                        w.WriteBlankLine();

                        w.WriteLine("return PyLong_FromVoidPtr(iface.release());");
                    }
                })
        );
        w.WriteBlankLine();

        w.WriteLine($"static PyMethodDef methods_Implements{type.Name}[] = {{");
        w.Indent++;
        w.WriteLine(
            $"{{ \"_guid_\", reinterpret_cast<PyCFunction>(_guid_Implements{type.Name}), METH_NOARGS | METH_STATIC, nullptr }},"
        );
        w.WriteLine(
            $"{{ \"_make_\", reinterpret_cast<PyCFunction>(_make_Implements{type.Name}), METH_VARARGS | METH_STATIC, nullptr }},"
        );
        w.WriteLine("{ }};");
        w.Indent--;
        w.WriteBlankLine();

        w.WriteLine($"static PyType_Slot type_slots_Implements{type.Name}[] = {{");
        w.Indent++;
        w.WriteLine(
            $"{{ Py_tp_methods, reinterpret_cast<void*>(methods_Implements{type.Name}) }},"
        );
        w.WriteLine("{ }};");
        w.Indent--;
        w.WriteBlankLine();

        w.WriteLine($"static PyType_Spec type_spec_Implements{type.Name} = {{");
        w.Indent++;
        w.WriteLine(
            $"\"winrt.{type.Namespace.ToNsModuleName()}{moduleSuffix}.Implements{type.Name}\","
        );
        w.WriteLine("0,");
        w.WriteLine("0,");
        w.WriteLine("Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,");
        w.WriteLine($"type_slots_Implements{type.Name}}};");
        w.Indent--;
    }

    public static void WriteGenericInterfaceDecl(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"struct {type.Name}");
        w.WriteBlock(
            () =>
            {
                w.WriteLine($"virtual ~{type.Name}() {{}};");

                foreach (var method in type.Methods.Select(m => m.Name).Distinct())
                {
                    w.WriteLine($"virtual PyObject* {method}(PyObject*) noexcept = 0;");
                }

                foreach (var property in type.Properties)
                {
                    w.WriteLine($"virtual PyObject* get_{property.Name}() noexcept = 0;");

                    if (property.SetMethod is not null)
                    {
                        w.WriteLine($"virtual int put_{property.Name}(PyObject*) noexcept = 0;");
                    }
                }

                foreach (var evt in type.Events)
                {
                    w.WriteLine($"virtual PyObject* {evt.AddMethod.Name}(PyObject*) noexcept = 0;");
                    w.WriteLine(
                        $"virtual PyObject* {evt.RemoveMethod.Name}(PyObject*) noexcept = 0;"
                    );
                }

                if (type.IsPyAwaitable)
                {
                    w.WriteLine($"virtual PyObject* dunder_await() noexcept = 0;");
                }

                if (type.IsPyIterable)
                {
                    w.WriteLine("virtual PyObject* dunder_iter() noexcept = 0;");
                }

                if (type.IsPyIterator)
                {
                    w.WriteLine("virtual PyObject* dunder_iternext() noexcept = 0;");
                }

                if (type.IsPySequence)
                {
                    w.WriteLine("virtual Py_ssize_t seq_length() noexcept = 0;");
                    w.WriteLine("virtual PyObject* seq_item(Py_ssize_t i) noexcept = 0;");
                    w.WriteLine("virtual PyObject* seq_subscript(PyObject* slice) noexcept = 0;");

                    if (type.IsPyMutableSequence)
                    {
                        w.WriteLine(
                            "virtual int seq_assign(Py_ssize_t i, PyObject* value) noexcept = 0;"
                        );
                    }
                }

                if (type.IsPyMapping)
                {
                    w.WriteMapGenericInterfaceDecl(type);
                }
            },
            ";"
        );
    }

    public static void WriteGenericInterfaceImpl(
        this IndentedTextWriter w,
        ProjectedType type,
        bool componentDlls
    )
    {
        w.WriteLine(
            $"template<{string.Join(", ", type.Type.GenericParameters.Select(p => $"typename {p.Name}"))}>"
        );
        w.WriteLine($"struct {type.Name} : public py::proj::{type.CppNamespace}::{type.Name}");
        w.WriteBlock(
            () =>
            {
                w.WriteLine($"{type.Name}({type.CppWinrtType} o) : _obj(o) {{}}");

                foreach (var methodName in type.Methods.Select(m => m.Name).Distinct())
                {
                    w.WriteLine($"PyObject* {methodName}(PyObject* args) noexcept override");
                    w.WriteBlock(() => w.WriteMethodOverloads(type, methodName, componentDlls));
                }

                foreach (var prop in type.Properties)
                {
                    w.WriteLine($"PyObject* get_{prop.Name}() noexcept override");
                    w.WriteBlock(
                        () => w.WriteTryCatch(() => w.WriteMethodBodyContents(type, prop.GetMethod))
                    );

                    if (prop.SetMethod is not null)
                    {
                        w.WriteLine($"int put_{prop.Name}(PyObject* arg) noexcept override");
                        w.WriteBlock(() =>
                        {
                            w.WriteLine("if (!arg)");
                            w.WriteBlock(() =>
                            {
                                w.WriteLine(
                                    "PyErr_SetString(PyExc_AttributeError, \"can't delete attribute\");"
                                );
                                w.WriteLine("return -1;");
                            });
                            w.WriteBlankLine();
                            w.WriteTryCatch(
                                () =>
                                    w.WriteMethodBodyContents(
                                        type,
                                        prop.SetMethod,
                                        isPropertySetter: true
                                    ),
                                catchReturn: "-1"
                            );
                        });
                    }
                }

                foreach (var evt in type.Events)
                {
                    w.WriteLine($"PyObject* {evt.AddMethod.Name}(PyObject* arg) noexcept override");
                    w.WriteBlock(
                        () => w.WriteTryCatch(() => w.WriteMethodBodyContents(type, evt.AddMethod))
                    );

                    w.WriteLine(
                        $"PyObject* {evt.RemoveMethod.Name}(PyObject* arg) noexcept override"
                    );
                    w.WriteBlock(
                        () =>
                            w.WriteTryCatch(() => w.WriteMethodBodyContents(type, evt.RemoveMethod))
                    );
                }

                if (type.IsPyAwaitable)
                {
                    w.WriteLine(
                        "PyObject* dunder_await() noexcept override { return py::dunder_await(_obj); }"
                    );
                }

                if (type.IsPyIterable)
                {
                    w.WriteIterGenericInterfaceImpl(type);
                }

                if (type.IsPySequence)
                {
                    w.WriteSeqGenericInterfaceImpl(type);
                }

                if (type.IsPyMapping)
                {
                    w.WriteMapGenericInterfaceImpl(type);
                }

                w.WriteBlankLine();
                w.WriteLine($"{type.CppWinrtType} _obj{{ nullptr }};");
            },
            ";"
        );
    }

    public static void WriteGenericInterfaceTypeMapper(
        this IndentedTextWriter w,
        ProjectedType type
    )
    {
        w.WriteLine(
            $"template <{string.Join(", ", type.Type.GenericParameters.Select(p => $"typename {p.Name}"))}>"
        );
        w.WriteLine($"struct pinterface_python_type<{type.CppWinrtType}>");
        w.WriteBlock(
            () =>
            {
                w.WriteLine($"using abstract = py::proj::{type.CppNamespace}::{type.Name};");
                w.WriteLine(
                    $"using concrete = py::impl::{type.CppNamespace}::{type.Name}<{string.Join(", ", type.Type.GenericParameters.Select(p => p.Name))}>;"
                );
            },
            ";"
        );
        w.WriteBlankLine();
    }

    public static void WritePythonImplementsInterfaceTyping(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap
    )
    {
        var methods = type.Methods.Where(m =>
            m.Method.DeclaringType.FullName == type.Type.FullName
        );
        var events = type.Events.Where(e => e.Event.DeclaringType.FullName == type.Type.FullName);
        var properties = type.Properties.Where(p =>
            p.Property.DeclaringType.FullName == type.Type.FullName
        );

        var hasMembers = methods.Any() || events.Any() || properties.Any();

        var interfaces = string.Join(
            ", ",
            type.Interfaces.Select(i =>
                i.ToPyTypeName(ns, new TypeRefNullabilityInfo(i), usePythonCollectionTypes: false)
            )
        );

        var generic = "";

        if (type.IsGeneric && !type.Interfaces.Any(i => i.ContainsGenericParameter))
        {
            generic =
                $"{(interfaces.Any() ? ", " : "")}typing.Generic[{string.Join(", ", type.Type.GenericParameters.Select(p => p.ToPyTypeName(ns, new TypeRefNullabilityInfo(p))))}]";
        }

        var mixin = type switch
        {
            { Namespace: "Windows.Foundation.Collections", Name: "IMap" }
                => ", winrt._winrt.MutableMapping[K, V]",
            { Namespace: "Windows.Foundation.Collections", Name: "IMapView" }
                => ", winrt._winrt.Mapping[K, V]",
            { Namespace: "Windows.Foundation.Collections", Name: "IVector" }
                => ", winrt._winrt.MutableSequence[T]",
            { Namespace: "Windows.Foundation.Collections", Name: "IVectorView" }
                => ", winrt._winrt.Sequence[T]",
            _ => ""
        };

        var inspectable =
            $"{(type.IsGeneric || type.Interfaces.Any() ? ", " : "")}winrt._winrt.IInspectable";

        // work around https://github.com/python/mypy/issues/17091
        // we can't use abc.ABCMeta because it will cause errors about conflicting metaclasses
        var typeIgnore = hasMembers ? "" : "  # type: ignore[misc]";

        w.WriteLine($"class {type.Name}({interfaces}{generic}{mixin}{inspectable}):{typeIgnore}");
        w.Indent++;

        if (type.Namespace == "Windows.Foundation" && type.Name == "IClosable")
        {
            w.WriteLine("def __enter__(self: Self) -> Self: ...");
            w.WriteLine(
                "def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ..."
            );
        }

        if (type.Namespace == "Windows.Foundation" && type.Name == "IStringable")
        {
            w.WriteLine("def __str__(self) -> str: ...");
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
            w.WriteMapPythonSpecialMethods(type, ns, nullabilityMap, out var keyParamType);

            if (type.Name == "IMap")
            {
                w.WriteMutableMapPythonSpecialMethods(type, ns, nullabilityMap, keyParamType);
            }
        }

        if (
            type.Namespace == "Windows.Foundation.Collections"
            && (type.Name == "IVector" || type.Name == "IVectorView")
        )
        {
            w.WriteSeqPythonSpecialMethods(type, ns, nullabilityMap);

            if (type.Name == "IVector")
            {
                w.WriteMutableSeqPythonSpecialMethods(type, ns, nullabilityMap);
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
                    .ToPyTypeName(ns, new TypeRefNullabilityInfo(type.Type.GenericParameters[0]));
            }

            w.WriteLine(
                $"def __await__(self) -> typing.Generator[typing.Any, None, {returnType}]: ..."
            );
        }

        foreach (var method in methods)
        {
            w.WritePythonMethodTyping(method, ns, nullabilityMap, isAbstract: true);
        }

        foreach (var evt in events)
        {
            w.WritePythonMethodTyping(evt.AddMethod, ns, nullabilityMap, isAbstract: true);
            w.WritePythonMethodTyping(evt.RemoveMethod, ns, nullabilityMap, isAbstract: true);
        }

        foreach (var prop in properties)
        {
            w.WritePythonPropertyTyping(type, prop, ns, nullabilityMap, isAbstract: true);
        }

        if (!hasMembers)
        {
            w.WriteLine("pass");
        }

        w.Indent--;
        w.WriteBlankLine();
    }
}
