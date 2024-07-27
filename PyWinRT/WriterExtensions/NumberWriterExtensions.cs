using System.CodeDom.Compiler;

static class NumberWriterExtensions
{
    static void WriteReturnNotImplementedOnTypeError(this IndentedTextWriter w)
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
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    static void WriteNumberSub(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"static PyObject* _sub_{type.Name}(PyObject* left, PyObject* right) noexcept");
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
                w.WriteLine($"auto _result = _left - _right;");
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    static void WriteNumberMul(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"static PyObject* _mul_{type.Name}(PyObject* left, PyObject* right) noexcept");
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                if (
                    type.Name != "Matrix3x2"
                    && type.Name != "Matrix4x4"
                    && type.Name != "Quaternion"
                )
                {
                    w.WriteLine("py::pyobj_handle left_float{PyNumber_Float(left)};");
                    w.WriteLine("if (left_float)");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("auto _left_float = PyFloat_AsDouble(left_float.get());");
                    w.WriteLine("if (_left_float == -1 && PyErr_Occurred())");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("return nullptr;");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteLine(
                        $"auto _result = static_cast<float>(_left_float) * py::converter<{type.CppWinrtType}>::convert_to(right);"
                    );
                    w.WriteLine($"return py::convert(_result);");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteLine("else");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("PyErr_Clear();");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteBlankLine();
                }

                w.WriteLine($"auto _left = py::converter<{type.CppWinrtType}>::convert_to(left);");
                w.WriteBlankLine();
                w.WriteLine("py::pyobj_handle right_float{PyNumber_Float(right)};");
                w.WriteLine("if (right_float)");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine("auto _right_float = PyFloat_AsDouble(right_float.get());");
                w.WriteLine("if (_right_float == -1 && PyErr_Occurred())");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine("return nullptr;");
                w.Indent--;
                w.WriteLine("}");
                w.WriteBlankLine();
                w.WriteLine($"auto _result = _left * static_cast<float>(_right_float);");
                w.WriteLine($"return py::convert(_result);");
                w.Indent--;
                w.WriteLine("}");
                w.WriteLine("else");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine("PyErr_Clear();");
                w.Indent--;
                w.WriteLine("}");
                w.WriteBlankLine();
                w.WriteLine(
                    $"auto _right = py::converter<{type.CppWinrtType}>::convert_to(right);"
                );
                w.WriteBlankLine();
                w.WriteLine($"auto _result = _left * _right;");
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    static void WriteNumberDiv(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine(
            $"static PyObject* _truediv_{type.Name}(PyObject* left, PyObject* right) noexcept"
        );
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"auto _left = py::converter<{type.CppWinrtType}>::convert_to(left);");

                if (type.Name != "Quaternion")
                {
                    w.WriteBlankLine();
                    w.WriteLine("py::pyobj_handle right_float{PyNumber_Float(right)};");
                    w.WriteLine("if (right_float)");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("auto _right_float = PyFloat_AsDouble(right_float.get());");
                    w.WriteLine("if (_right_float == -1 && PyErr_Occurred())");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("return nullptr;");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteBlankLine();
                    w.WriteLine($"auto _result = _left / static_cast<float>(_right_float);");
                    w.WriteLine($"return py::convert(_result);");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteLine("else");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("PyErr_Clear();");
                    w.Indent--;
                    w.WriteLine("}");
                }

                w.WriteLine(
                    $"auto _right = py::converter<{type.CppWinrtType}>::convert_to(right);"
                );
                w.WriteBlankLine();
                w.WriteLine($"auto _result = _left / _right;");
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    static void WriteNumberNeg(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"static PyObject* _neg_{type.Name}(PyObject* operand) noexcept");
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                w.WriteLine(
                    $"auto _operand = py::converter<{type.CppWinrtType}>::convert_to(operand);"
                );
                w.WriteLine($"auto _result = -_operand;");
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    public static void WriteNumberMethods(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteNumberAdd(type);
        w.WriteBlankLine();
        w.WriteNumberSub(type);
        w.WriteBlankLine();
        w.WriteNumberMul(type);

        if (!type.Name.StartsWith("Matrix", StringComparison.Ordinal))
        {
            w.WriteBlankLine();
            w.WriteNumberDiv(type);
        }

        w.WriteBlankLine();
        w.WriteNumberNeg(type);
    }
}
