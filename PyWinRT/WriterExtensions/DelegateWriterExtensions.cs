using System.CodeDom.Compiler;

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
        w.WriteLine("{");
        w.Indent++;

        w.WriteLine($"using type = py::impl::{type.CppNamespace}::{type.Name}{args};");

        w.Indent--;
        w.WriteLine("};");
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
        w.WriteLine("{");
        w.Indent++;

        // TODO: include write_delegate_template_args
        w.WriteLine($"static {type.CppWinrtType} get(PyObject* callable)");
        w.WriteLine("{");
        w.Indent++;

        w.WriteLine("py::delegate_callable _delegate{ callable };");
        w.WriteBlankLine();
        w.WriteLine(
            $"return [delegate = std::move(_delegate)]({string.Join(", ", invoke.Parameters.Select(p => p.ToDelegateParam()))})"
        );
        w.WriteLine("{");
        w.Indent++;

        w.WriteLine("auto gil = py::ensure_gil();");
        w.WriteBlankLine();

        foreach (var param in invoke.Parameters.Where(p => p.IsPythonInParam()))
        {
            w.WriteLine(
                $"py::pyobj_handle py_{param.ToParamName()}{{ py::convert({param.ToParamName()}) }};"
            );
            w.WriteBlankLine();
            w.WriteLine($"if (!py_{param.ToParamName()}) {{");
            w.Indent++;
            w.WriteLine("PyErr_WriteUnraisable(delegate.callable());");
            w.WriteLine($"throw std::invalid_argument(\"{param.ToParamName()}\");");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
        }

        if (invoke.Parameters.Any(p => p.IsPythonInParam()))
        {
            w.WriteLine(
                $"py::pyobj_handle args{{ PyTuple_Pack({invoke.Parameters.Count(p => p.IsPythonInParam())}, {string.Join(", ", invoke.Parameters.Where(p => p.IsPythonInParam()).Select(p => $"py_{p.ToParamName()}.get()"))}) }};"
            );
            w.WriteBlankLine();
            w.WriteLine("if (!args) {");
            w.Indent++;
            w.WriteLine("PyErr_WriteUnraisable(delegate.callable());");
            w.WriteLine("throw winrt::hresult_error();");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
        }
        else
        {
            w.WriteLine("py::pyobj_handle args{ nullptr };");
        }

        w.WriteLine(
            "py::pyobj_handle return_value{ PyObject_CallObject(delegate.callable(), args.get()) };"
        );

        w.WriteBlankLine();
        w.WriteLine("if (!return_value)");
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine("PyErr_WriteUnraisable(delegate.callable());");
        w.WriteLine("throw winrt::hresult_error();");
        w.Indent--;
        w.WriteLine("}");

        var i = invoke.ReturnType.FullName == "System.Void" ? 0 : 1;

        foreach (var param in invoke.Parameters.Where(p => p.IsPythonOutParam()))
        {
            switch (param.GetCategory())
            {
                case ParamCategory.Out:
                    w.WriteBlankLine();
                    w.WriteLine(
                        $"{param.ToParamName()} = py::convert_to<{param.ParameterType.ToCppTypeName()}>(return_value.get(), {i});"
                    );
                    break;
                case ParamCategory.ReceiveArray:
                    w.WriteBlankLine();
                    w.WriteLine(
                        $"auto {param.ToParamName()}_buf = py::convert_to<py::pybuf_view<{param.ParameterType.ToCppTypeName()}, false>>(return_value.get(), {i});"
                    );
                    w.WriteLine(
                        $"{param.ToParamName()} = winrt::com_array<{param.ParameterType.ToCppTypeName()}>{{{param.ToParamName()}_buf.begin(), {param.ToParamName()}_buf.end()}};"
                    );
                    break;
                default:
                    throw new ArgumentException("Invalid param category");
            }

            i++;
        }

        if (invoke.ReturnType.FullName != "System.Void")
        {
            var index = "";

            if (i > 1)
            {
                // delegate has out params and a return value so
                // the return value is the first item in the tuple
                index = ", 0";
            }

            w.WriteBlankLine();

            if (invoke.ReturnType.IsArray)
            {
                w.WriteLine(
                    $"auto return_buf = py::convert_to<py::pybuf_view<{invoke.ReturnType.ToCppTypeName()}, false>>(return_value.get(){index});"
                );
                w.WriteLine(
                    $"return winrt::com_array<{invoke.ReturnType.ToCppTypeName()}>{{return_buf.begin(), return_buf.end()}};"
                );
            }
            else
            {
                w.WriteLine(
                    $"return py::convert_to<{invoke.ReturnType.ToCppTypeName()}>(return_value.get(){index});"
                );
            }
        }

        w.Indent--;
        w.WriteLine("};");
        w.Indent--;
        w.WriteLine("};");
        w.Indent--;
        w.WriteLine("};");
    }
}
