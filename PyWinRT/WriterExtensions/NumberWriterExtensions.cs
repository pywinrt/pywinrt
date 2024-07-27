using System.CodeDom.Compiler;

static class NumberWriterExtensions
{
    static void WriteNumberAdd(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"static PyObject* _add_{type.Name}(PyObject* left, PyObject* right) noexcept");
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"auto _left = py::converter<{type.CppWinrtType}>::convert_to(left);");
                w.WriteLine(
                    $"auto _right = py::converter<{type.CppWinrtType}>::convert_to(right);"
                );
                w.WriteBlankLine();
                w.WriteLine($"auto _result = _left + _right;");
                w.WriteLine($"return py::convert(_result);");
            },
            () =>
            {
                w.WriteLine("py::to_PyErr();");
                w.WriteBlankLine();
                w.WriteLine("if (PyErr_ExceptionMatches(PyExc_TypeError))");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine("PyErr_Clear();");
                w.WriteLine("Py_RETURN_NOTIMPLEMENTED;");
                w.Indent--;
                w.WriteLine("}");
                w.WriteBlankLine();
            }
        );
        w.Indent--;
        w.WriteLine("}");
    }

    public static void WriteNumberMethods(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteNumberAdd(type);
    }
}
