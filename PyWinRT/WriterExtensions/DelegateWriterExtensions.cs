using System.CodeDom.Compiler;
using Mono.Cecil;

static class DelegateWriterExtensions
{
    public static void WriteDelegateTypeMapper(this IndentedTextWriter w, ProjectedType type)
    {
        var args = type.IsGeneric
            ? $"<{string.Join(", ", type.Type.GenericParameters.Select(p => p.Name))}>"
            : "";

        w.WriteLine(
            $"template <{string.Join(", ", type.Type.GenericParameters.Select(p => $"typename {p.Name}"))}>"
        );
        w.WriteLine($"struct delegate_python_type<{type.CppWinrtType}>");
        w.WriteBlock(
            () => w.WriteLine($"using type = py::impl::{type.CppNamespace}::{type.Name}{args};"),
            ";"
        );
        w.WriteBlankLine();
    }

    public static void WriteDelegateCallableWrapper(this IndentedTextWriter w, ProjectedType type)
    {
        var invoke = type.Type.Methods.Single(m => m.IsSpecialName && m.Name == "Invoke");

        if (type.IsGeneric)
        {
            w.WriteLine(
                $"template <{string.Join(", ", type.Type.GenericParameters.Select(p => $"typename {p.Name}"))}>"
            );
        }

        w.WriteLine($"struct {type.Name}");
        w.WriteBlock(
            () =>
            {
                // TODO: include write_delegate_template_args
                w.WriteLine($"static {type.CppWinrtType} get(PyObject* callable)");
                w.WriteBlock(
                    () =>
                    {
                        w.WriteLine("py::delegate_callable _delegate{ callable };");
                        w.WriteBlankLine();
                        w.WriteLine(
                            $"return [delegate = std::move(_delegate)]({string.Join(", ", invoke.Parameters.Select(p => p.ToDelegateParam()))})"
                        );
                        w.WriteBlock(
                            () => w.WriteDelegateInvoke(invoke, "delegate.callable()"),
                            ";"
                        );
                    },
                    ";"
                );
            },
            ";"
        );
    }

    public static void WriteDelegateInvoke(
        this IndentedTextWriter w,
        MethodDefinition method,
        string callable,
        Action? writeGetCallable = default,
        bool ensureGil = true,
        IReadOnlyDictionary<GenericParameter, TypeReference>? map = default
    )
    {
        if (ensureGil)
        {
            w.WriteLine("auto gil = py::ensure_gil();");
            w.WriteBlankLine();
        }

        w.WriteLine("try");
        w.WriteBlock(() =>
        {
            if (writeGetCallable is not null)
            {
                writeGetCallable();
            }

            var pythonInParams = method.Parameters.Where(p => p.IsPythonInParam()).ToList();

            foreach (var param in pythonInParams)
            {
                w.WriteLine(
                    $"py::pyobj_handle py_{param.ToParamName()}{{py::convert({param.ToParamName()})}};"
                );
                w.WriteLine($"if (!py_{param.ToParamName()})");
                w.WriteBlock(() => w.WriteLine("throw python_exception();"));
                w.WriteBlankLine();
            }

            if (pythonInParams.Count > 1)
            {
                w.WriteLine(
                    $"py::pyobj_handle args{{PyTuple_Pack({pythonInParams.Count}, {string.Join(", ", pythonInParams.Select(p => $"py_{p.ToParamName()}.get()"))})}};"
                );
                w.WriteLine("if (!args)");
                w.WriteBlock(() => w.WriteLine("throw python_exception();"));
                w.WriteBlankLine();
                w.WriteLine(
                    $"py::pyobj_handle return_value{{PyObject_CallObject({callable}, args.get())}};"
                );
            }
            else if (pythonInParams.Count == 1)
            {
                w.WriteLine(
                    $"py::pyobj_handle return_value{{PyObject_CallOneArg({callable}, py_{pythonInParams[0].ToParamName()}.get())}};"
                );
            }
            else
            {
                w.WriteLine($"py::pyobj_handle return_value{{PyObject_CallNoArgs({callable})}};");
            }

            w.WriteLine("if (!return_value)");
            w.WriteBlock(() => w.WriteLine("throw python_exception();"));

            var i = method.ReturnType.FullName == "System.Void" ? 0 : 1;

            foreach (var param in method.Parameters.Where(p => p.IsPythonOutParam()))
            {
                switch (param.GetCategory())
                {
                    case ParamCategory.Out:
                        w.WriteBlankLine();
                        w.WriteLine(
                            $"{param.ToParamName()} = py::convert_to<{param.ParameterType.ToCppTypeName(map)}>(return_value.get(), {i});"
                        );
                        break;
                    case ParamCategory.ReceiveArray:
                        w.WriteBlankLine();
                        w.WriteLine(
                            $"auto {param.ToParamName()}_buf = py::convert_to<py::pybuf_view<{param.ParameterType.ToCppTypeName(map)}, false>>(return_value.get(), {i});"
                        );
                        w.WriteLine(
                            $"{param.ToParamName()} = winrt::com_array<{param.ParameterType.ToCppTypeName(map)}>{{{param.ToParamName()}_buf.begin(), {param.ToParamName()}_buf.end()}};"
                        );
                        break;
                    default:
                        throw new ArgumentException("Invalid param category");
                }

                i++;
            }

            if (method.ReturnType.FullName != "System.Void")
            {
                var index = "";

                if (i > 1)
                {
                    // delegate has out params and a return value so
                    // the return value is the first item in the tuple
                    index = ", 0";
                }

                w.WriteBlankLine();

                if (method.ReturnType.IsArray)
                {
                    w.WriteLine(
                        $"auto return_buf = py::convert_to<py::pybuf_view<{method.ReturnType.ToCppTypeName(map)}, false>>(return_value.get(){index});"
                    );
                    w.WriteLine(
                        $"return winrt::com_array<{method.ReturnType.ToCppTypeName(map)}>{{return_buf.begin(), return_buf.end()}};"
                    );
                }
                else
                {
                    w.WriteLine(
                        $"return py::convert_to<{method.ReturnType.ToCppTypeName(map)}>(return_value.get(){index});"
                    );
                }
            }
        });
        w.WriteLine("catch (python_exception)");
        w.WriteBlock(() => w.WriteLine("py::write_unraisable_and_throw();"));
    }
}
