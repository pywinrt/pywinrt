using System.CodeDom.Compiler;

static class NumberWriterExtensions
{
    private record ParamInfo(string Name, string PyType)
    {
        public string CppWinrtType => cppWinrtTypeFromPyType[PyType];
    };

    private record MethodInfo(
        string Name,
        string ReturnPyType,
        IReadOnlyList<ParamInfo> Parameters
    );

    private static readonly IReadOnlyDictionary<string, string> cppWinrtTypeFromPyType =
        new Dictionary<string, string>
        {
            { "float", "float" },
            { "Vector2", "winrt::Windows::Foundation::Numerics::float2" },
            { "Vector3", "winrt::Windows::Foundation::Numerics::float3" },
            { "Vector4", "winrt::Windows::Foundation::Numerics::float4" },
            { "Matrix3x2", "winrt::Windows::Foundation::Numerics::float3x2" },
            { "Matrix4x4", "winrt::Windows::Foundation::Numerics::float4x4" },
            { "Plane", "winrt::Windows::Foundation::Numerics::plane" },
            { "Quaternion", "winrt::Windows::Foundation::Numerics::quaternion" }
        };

    private static readonly IReadOnlyDictionary<
        string,
        IReadOnlyCollection<MethodInfo>
    > extraMethods = new Dictionary<string, IReadOnlyCollection<MethodInfo>>
    {
        {
            "Vector2",
            new List<MethodInfo>
            {
                new("length", "float", []),
                new("length_squared", "float", []),
                new("distance", "float", [new ParamInfo("value", "Vector2")]),
                new("distance_squared", "float", [new ParamInfo("value", "Vector2")]),
                new("dot", "float", [new ParamInfo("value", "Vector2")]),
                new("normalize", "Vector2", []),
                new("reflect", "Vector2", [new ParamInfo("normal", "Vector2")]),
                new("min", "Vector2", [new ParamInfo("value", "Vector2")]),
                new("max", "Vector2", [new ParamInfo("value", "Vector2")]),
                new(
                    "clamp",
                    "Vector2",
                    [new ParamInfo("min", "Vector2"), new ParamInfo("max", "Vector2")]
                ),
                new(
                    "lerp",
                    "Vector2",
                    [new ParamInfo("value", "Vector2"), new ParamInfo("amount", "float")]
                ),
                new("transform", "Vector2", [new ParamInfo("matrix", "Matrix3x2")]),
                new("transform", "Vector2", [new ParamInfo("matrix", "Matrix4x4")]),
                new("transform_normal", "Vector2", [new ParamInfo("matrix", "Matrix3x2")]),
                new("transform_normal", "Vector2", [new ParamInfo("matrix", "Matrix4x4")]),
                new("transform", "Vector2", [new ParamInfo("rotation", "Quaternion")]),
                new("transform4", "Vector4", [new ParamInfo("matrix", "Matrix4x4")]),
                new("transform4", "Vector4", [new ParamInfo("rotation", "Quaternion")])
            }
        },
        {
            "Vector3",
            new List<MethodInfo>
            {
                new("length", "float", []),
                new("length_squared", "float", []),
                new("distance", "float", [new ParamInfo("value", "Vector3")]),
                new("distance_squared", "float", [new ParamInfo("value", "Vector3")]),
                new("dot", "float", [new ParamInfo("value", "Vector3")]),
                new("cross", "Vector3", [new ParamInfo("value", "Vector3")]),
                new("normalize", "Vector3", []),
                new("reflect", "Vector3", [new ParamInfo("normal", "Vector3")]),
                new("min", "Vector3", [new ParamInfo("value", "Vector3")]),
                new("max", "Vector3", [new ParamInfo("value", "Vector3")]),
                new(
                    "clamp",
                    "Vector3",
                    [new ParamInfo("min", "Vector3"), new ParamInfo("max", "Vector3")]
                ),
                new(
                    "lerp",
                    "Vector3",
                    [new ParamInfo("value", "Vector3"), new ParamInfo("amount", "float")]
                ),
                new("transform", "Vector3", [new ParamInfo("matrix", "Matrix4x4")]),
                new("transform_normal", "Vector3", [new ParamInfo("matrix", "Matrix4x4")]),
                new("transform", "Vector3", [new ParamInfo("rotation", "Quaternion")]),
                new("transform4", "Vector4", [new ParamInfo("matrix", "Matrix4x4")]),
                new("transform4", "Vector4", [new ParamInfo("rotation", "Quaternion")]),
            }
        },
        {
            "Vector4",
            new List<MethodInfo>
            {
                new("length", "float", []),
                new("length_squared", "float", []),
                new("distance", "float", [new ParamInfo("value", "Vector4")]),
                new("distance_squared", "float", [new ParamInfo("value", "Vector4")]),
                new("dot", "float", [new ParamInfo("value", "Vector4")]),
                new("normalize", "Vector4", []),
                new("min", "Vector4", [new ParamInfo("value", "Vector4")]),
                new("max", "Vector4", [new ParamInfo("value", "Vector4")]),
                new(
                    "clamp",
                    "Vector4",
                    [new ParamInfo("min", "Vector4"), new ParamInfo("max", "Vector4")]
                ),
                new(
                    "lerp",
                    "Vector4",
                    [new ParamInfo("value", "Vector4"), new ParamInfo("amount", "float")]
                ),
                new("transform", "Vector4", [new ParamInfo("matrix", "Matrix4x4")]),
                new("transform", "Vector4", [new ParamInfo("rotation", "Quaternion")]),
            }
        },
        {
            "Matrix3x2",
            new List<MethodInfo>
            {
                new("is_identity", "bool", []),
                new("determinant", "float", []),
                new("translation", "Vector2", []),
                // new("invert", "Matrix3x2", []),
                new(
                    "lerp",
                    "Matrix3x2",
                    [new ParamInfo("value", "Matrix3x2"), new ParamInfo("amount", "float")]
                ),
            }
        },
        {
            "Matrix4x4",
            new List<MethodInfo>
            {
                new("is_identity", "bool", []),
                new("determinant", "float", []),
                new("translation", "Vector2", []),
                // new("invert", "Matrix4x4", []),
                new("transform", "Matrix4x4", [new ParamInfo("rotation", "Quaternion")]),
                // new("transform", "Vector3", [new ParamInfo("value", "Vector4")])
                new(
                    "lerp",
                    "Matrix4x4",
                    [new ParamInfo("value", "Matrix4x4"), new ParamInfo("amount", "float")]
                )
            }
        },
        {
            "Plane",
            new List<MethodInfo>
            {
                new("normalize", "Plane", []),
                new("transform", "Plane", [new ParamInfo("matrix", "Matrix4x4")]),
                new("transform", "Plane", [new ParamInfo("rotation", "Quaternion")]),
                new("dot", "float", [new ParamInfo("value", "Vector4")]),
                new("dot_coordinate", "float", [new ParamInfo("value", "Vector3")]),
                new("dot_normal", "float", [new ParamInfo("value", "Vector3")])
            }
        },
        {
            "Quaternion",
            new List<MethodInfo>
            {
                new("is_identity", "bool", []),
                new("length", "float", []),
                new("length_squared", "float", []),
                new("dot", "float", [new ParamInfo("value", "Quaternion")]),
                new("normalize", "Quaternion", []),
                // new("conjugate", "Quaternion", []),
                // new("invert", "Quaternion", []),
                new(
                    "slerp",
                    "Quaternion",
                    [new ParamInfo("value", "Quaternion"), new ParamInfo("amount", "float")]
                ),
                new(
                    "lerp",
                    "Quaternion",
                    [new ParamInfo("value", "Quaternion"), new ParamInfo("amount", "float")]
                ),
            }
        }
    };

    public static void WriteNumberMethodPyTyping(this IndentedTextWriter w, ProjectedType type)
    {
        foreach (var method in extraMethods[type.Name])
        {
            var parameters = string.Join(
                "",
                method.Parameters.Select(p => $", {p.Name}: {p.PyType}")
            );

            if (extraMethods[type.Name].Count(m => m.Name == method.Name) > 1)
            {
                w.WriteLine("@typing.overload");
            }

            w.WriteLine($"def {method.Name}(self{parameters}) -> {method.ReturnPyType}: ...");
        }
    }

    public static void WriteNumberMethods(this IndentedTextWriter w, ProjectedType type)
    {
        foreach (var overloads in extraMethods[type.Name].GroupBy(m => m.Name))
        {
            var method = overloads.First();
            var args = method.Parameters.Count switch
            {
                0 => "/*unused*/",
                1 => "arg",
                _ => "args"
            };

            w.WriteBlankLine();
            w.WriteLine(
                $"static PyObject* {method.Name}_{type.Name}(winrt_struct_wrapper<{type.CppWinrtType}>* self, PyObject* {args}) noexcept"
            );
            w.WriteLine("{");
            w.Indent++;
            w.WriteTryCatch(() =>
            {
                if (overloads.Count() == 1)
                {
                    switch (method.Parameters.Count)
                    {
                        case 0:
                            w.WriteLine(
                                $"auto _result = winrt::Windows::Foundation::Numerics::{method.Name}(self->obj);"
                            );
                            w.WriteLine("return py::convert(_result);");

                            break;
                        case 1:
                            var param = method.Parameters[0];
                            w.WriteLine(
                                $"auto _arg = py::converter<{param.CppWinrtType}>::convert_to(arg);"
                            );
                            w.WriteLine(
                                $"auto _result = winrt::Windows::Foundation::Numerics::{method.Name}(self->obj, _arg);"
                            );
                            w.WriteLine("return py::convert(_result);");

                            break;
                        case 2:
                            var param0 = method.Parameters[0];
                            var param1 = method.Parameters[1];

                            w.WriteLine(
                                $"auto _arg0 = py::convert_to<{param0.CppWinrtType}>(args, 0);"
                            );
                            w.WriteLine(
                                $"auto _arg1 = py::convert_to<{param1.CppWinrtType}>(args, 1);"
                            );
                            w.WriteLine(
                                $"auto _result = winrt::Windows::Foundation::Numerics::{method.Name}(self->obj, _arg0, _arg1);"
                            );
                            w.WriteLine("return py::convert(_result);");

                            break;
                        default:
                            throw new NotImplementedException();
                    }
                }
                else
                {
                    switch (method.Parameters.Count)
                    {
                        case 1:
                            foreach (var (i, overload) in overloads.Select((o, i) => (i, o)))
                            {
                                var param = overload.Parameters[0];

                                if (i > 0)
                                {
                                    w.WriteBlankLine();
                                }

                                w.WriteLine(
                                    $"if (std::string_view(Py_TYPE(arg)->tp_name) == \"winrt._winrt_windows_foundation_numerics.{param.PyType}\")"
                                );
                                w.WriteLine("{");
                                w.Indent++;
                                w.WriteLine(
                                    $"auto _arg = py::converter<{param.CppWinrtType}>::convert_to(arg);"
                                );
                                w.WriteLine(
                                    $"auto _result = winrt::Windows::Foundation::Numerics::{overload.Name}(self->obj, _arg);"
                                );
                                w.WriteLine("return py::convert(_result);");
                                w.Indent--;
                                w.WriteLine("}");
                            }

                            w.WriteBlankLine();
                            w.WriteLine(
                                $"PyErr_Format(PyExc_TypeError, \"Expecting one of {string.Join(", ", overloads.Select(o => $"'winrt._winrt_windows_foundation_numerics.{o.Parameters[0].PyType}'"))} but got '%s'\", Py_TYPE(arg)->tp_name);"
                            );
                            w.WriteLine("return nullptr;");
                            break;
                        default:
                            throw new NotImplementedException();
                    }
                }
            });
            w.Indent--;
            w.WriteLine("}");
        }
    }

    public static void WriteNumberMethodDefs(this IndentedTextWriter w, ProjectedType type)
    {
        foreach (var method in extraMethods[type.Name].DistinctBy(m => m.Name))
        {
            var flags = method.Parameters.Count switch
            {
                0 => "METH_NOARGS",
                1 => "METH_O",
                _ => "METH_VARARGS",
            };

            w.WriteLine(
                $"{{ \"{method.Name}\", reinterpret_cast<PyCFunction>({method.Name}_{type.Name}), {flags}, nullptr }},"
            );
        }
    }

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

    static void WriteNumberAbs(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"static PyObject* _abs_{type.Name}(PyObject* operand) noexcept");
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(
            () =>
            {
                w.WriteLine(
                    $"auto _operand = py::converter<{type.CppWinrtType}>::convert_to(operand);"
                );
                w.WriteLine(
                    $"auto _result = winrt::Windows::Foundation::Numerics::length(_operand);"
                );
                w.WriteLine($"return py::convert(_result);");
            },
            w.WriteReturnNotImplementedOnTypeError
        );
        w.Indent--;
        w.WriteLine("}");
    }

    public static void WriteNumberSlotMethods(this IndentedTextWriter w, ProjectedType type)
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

        if (!type.Name.StartsWith("Matrix", StringComparison.Ordinal) && type.Name != "Plane")
        {
            w.WriteBlankLine();
            w.WriteNumberAbs(type);
        }
    }

    public static void WriteNumberSlots(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"{{ Py_nb_add, reinterpret_cast<void*>(_add_{type.Name}) }},");
        w.WriteLine($"{{ Py_nb_subtract, reinterpret_cast<void*>(_sub_{type.Name}) }},");
        w.WriteLine($"{{ Py_nb_multiply, reinterpret_cast<void*>(_mul_{type.Name}) }},");

        if (!type.Name.StartsWith("Matrix", StringComparison.Ordinal))
        {
            w.WriteLine($"{{ Py_nb_true_divide, reinterpret_cast<void*>(_truediv_{type.Name}) }},");
        }

        w.WriteLine($"{{ Py_nb_negative, reinterpret_cast<void*>(_neg_{type.Name}) }},");

        if (!type.Name.StartsWith("Matrix", StringComparison.Ordinal) && type.Name != "Plane")
        {
            w.WriteLine($"{{ Py_nb_absolute, reinterpret_cast<void*>(_abs_{type.Name}) }},");
        }
    }
}
