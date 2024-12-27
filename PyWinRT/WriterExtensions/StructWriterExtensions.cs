using System.CodeDom.Compiler;
using Mono.Cecil;

static class StructWriterExtensions
{
    public static void WritePythonStructTyping(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns
    )
    {
        var metaclass = "";

        if (type.PyRequiresMetaclass)
        {
            w.WriteLine("@typing.final");
            w.WriteLine($"class {type.Name}_Static(type):");
            w.Indent++;

            var pass = true;

            if (type.Type.IsCustomNumeric())
            {
                w.WriteNumberFactoryFunctionPyTyping(type, ref pass);
                w.WriteNumberCommonValuesPyTyping(type, ref pass);
            }

            if (pass)
            {
                w.WriteLine("pass");
            }

            w.Indent--;
            w.WriteBlankLine();

            metaclass = $"(metaclass={type.Name}_Static)";
        }

        w.WriteLine("@typing.final");
        w.WriteLine($"class {type.Name}{metaclass}:");
        w.Indent++;

        foreach (var field in type.Type.Fields)
        {
            w.WriteLine(
                $"{field.Name.ToPythonIdentifier()}: {field.FieldType.ToPyTypeName(ns, new TypeRefNullabilityInfo(field.FieldType))}"
            );
        }

        w.WriteLine(
            $"def __init__(self, {string.Join(", ", type.Type.Fields.Select(f => $"{f.Name.ToPythonIdentifier()}: {f.FieldType.ToPyTypeName(ns, new TypeRefNullabilityInfo(f.FieldType))} = {f.FieldType.GetDefaultPyValue(ns)}"))}) -> None: ..."
        );

        if (type.Type.IsCustomNumeric())
        {
            if (type.Name != "Plane")
            {
                w.WriteNumberSlotMethodsPyTyping(type);
            }

            w.WriteNumberMethodPyTyping(type);
        }

        w.Indent--;
        w.WriteBlankLine();
    }

    public static void WriteStruct(
        this IndentedTextWriter w,
        ProjectedType type,
        string moduleSuffix
    )
    {
        w.WriteLine($"// ----- {type.Name} struct --------------------");
        w.WriteStructConstructor(type);
        w.WriteDeallocFunction(type);

        if (type.Type.IsCustomNumeric())
        {
            w.WriteNumberFactoryFunctionMethodDefs(type);
            w.WriteNumberCommonValueMethods(type);
            w.WriteNumberMethods(type);
        }

        w.WriteAssignArrayMethod(type);
        w.WriteMethodTable(type);

        foreach (var field in type.Type.Fields)
        {
            w.WriteStructGetSetFunction(type, field);
        }

        w.WriteGetSetTable(type);

        if (type.Type.IsCustomNumeric() && type.Name != "Plane")
        {
            w.WriteBlankLine();
            w.WriteNumberSlotMethods(type);
        }

        w.WriteStructEqualityMethods(type);
        w.WriteStructRepr(type);
        w.WriteTypeSlotTable(type);
        w.WriteTypeSpec(type, moduleSuffix);

        if (type.PyRequiresMetaclass)
        {
            w.WriteMetaclass(type, moduleSuffix);
        }
    }

    static void WriteStructConstructor(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteBlankLine();
        w.WriteLine(
            $"winrt_struct_wrapper<{type.CppWinrtType}>* _new_{type.Name}(PyTypeObject* subclass, PyObject* /*unused*/, PyObject* /*unused*/) noexcept"
        );
        w.WriteBlock(() =>
        {
            w.WriteLine(
                $"auto self = reinterpret_cast<winrt_struct_wrapper<{type.CppWinrtType}>*>(subclass->tp_alloc(subclass, 0));"
            );
            w.WriteBlankLine();
            w.WriteLine("if (!self)");
            w.WriteBlock(() => w.WriteLine("return nullptr;"));
            w.WriteBlankLine();
            w.WriteLine("std::construct_at(&self->obj);");
            w.WriteBlankLine();
            w.WriteLine("return self;");
        });

        w.WriteBlankLine();
        w.WriteLine(
            $"int _init_{type.Name}(winrt_struct_wrapper<{type.CppWinrtType}>* self, PyObject* args, PyObject* kwds) noexcept"
        );
        w.WriteBlock(() =>
        {
            w.WriteLine("auto tuple_size = PyTuple_Size(args);");
            w.WriteBlankLine();
            w.WriteLine("if ((tuple_size == 0) && (!kwds))");
            w.WriteBlock(() =>
            {
                w.WriteLine("self->obj = {};");
                w.WriteLine("return 0;");
            });
            w.WriteBlankLine();

            foreach (var field in type.Type.Fields)
            {
                w.WriteLine($"{field.FieldType.ToStructFieldType()} _{field.Name}{{}};");
            }

            w.WriteBlankLine();
            w.WriteLine(
                $"static const char* kwlist[] = {{{type.Type.ToStructFieldKeywordList()}nullptr}};"
            );
            w.WriteLine(
                $"if (!PyArg_ParseTupleAndKeywords(args, kwds, \"{type.Type.ToStructFieldFormat()}\", const_cast<char**>(kwlist){type.Type.ToStructFieldParseParameterList()}))"
            );
            w.WriteBlock(() => w.WriteLine("return -1;"));
            w.WriteBlankLine();
            w.WriteTryCatch(
                () =>
                {
                    foreach (var field in type.Type.Fields)
                    {
                        w.WriteLine(
                            $"self->obj.{field.ToWinrtFieldName()} = {field.ToStructFieldInitializer()};"
                        );
                    }
                    w.WriteBlankLine();
                    w.WriteLine("return 0;");
                },
                catchReturn: "-1"
            );
        });
    }

    static void WriteStructGetSetFunction(
        this IndentedTextWriter w,
        ProjectedType type,
        FieldDefinition field
    )
    {
        w.WriteBlankLine();
        w.WriteLine(
            $"static PyObject* {type.Name}_get_{field.Name}({type.CppPyWrapperType}* self, void* /*unused*/) noexcept"
        );
        w.WriteBlock(
            () =>
                w.WriteTryCatch(() =>
                {
                    w.WriteLine($"return py::convert(self->obj.{field.ToWinrtFieldName()});");
                })
        );

        w.WriteBlankLine();
        w.WriteLine(
            $"static int {type.Name}_set_{field.Name}({type.CppPyWrapperType}* self, PyObject* arg, void* /*unused*/) noexcept"
        );
        w.WriteBlock(() =>
        {
            w.WriteLine("if (!arg)");
            w.WriteBlock(() =>
            {
                w.WriteLine("PyErr_SetString(PyExc_AttributeError, \"can't delete attribute\");");
                w.WriteLine("return -1;");
            });
            w.WriteBlankLine();

            w.WriteTryCatch(
                () =>
                {
                    w.WriteLine(
                        $"self->obj.{field.ToWinrtFieldName()} = py::convert_to<{field.FieldType.ToCppTypeName()}>(arg);"
                    );
                    w.WriteLine("return 0;");
                },
                catchReturn: "-1"
            );
        });
    }

    static void WriteStructEqualityMethods(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteBlankLine();
        w.WriteLine(
            $"static PyObject* _richcompare_{type.Name}({type.CppPyWrapperType}* self, PyObject* other, int op) noexcept"
        );
        w.WriteBlock(() =>
        {
            w.WriteTryCatch(() =>
            {
                w.WriteLine($"auto _other = py::convert_to<{type.CppWinrtType}>(other);");
                w.WriteBlankLine();
                w.WriteLine("if (op == Py_EQ)");
                w.WriteBlock(() =>
                {
                    w.WriteLine("if (self->obj == _other)");
                    w.WriteBlock(() => w.WriteLine("Py_RETURN_TRUE;"));
                    w.WriteBlankLine();
                    w.WriteLine("Py_RETURN_FALSE;");
                });
                w.WriteBlankLine();
                w.WriteLine("if (op == Py_NE)");
                w.WriteBlock(() =>
                {
                    w.WriteLine("if (self->obj != _other)");
                    w.WriteBlock(() => w.WriteLine("Py_RETURN_TRUE;"));
                    w.WriteBlankLine();
                    w.WriteLine("Py_RETURN_FALSE;");
                });
                w.WriteBlankLine();
                w.WriteLine("Py_RETURN_NOTIMPLEMENTED;");
            });
        });
    }

    private static void WriteStructRepr(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteBlankLine();
        w.WriteLine($"static PyObject* _repr_{type.Name}(PyObject* self) noexcept");
        w.WriteBlock(() =>
        {
            foreach (var field in type.Type.Fields)
            {
                w.WriteLine(
                    $"py::pyobj_handle {field.Name}{{PyObject_GetAttrString(self, \"{field.Name.ToPythonIdentifier()}\")}};"
                );

                w.WriteLine($"if (!{field.Name})");
                w.WriteBlock(() => w.WriteLine("return nullptr;"));
                w.WriteBlankLine();
            }

            w.WriteLine(
                $"return PyUnicode_FromFormat(\"{type.Name}({string.Join(", ", type.Type.Fields.Select(f => $"{f.Name.ToPythonIdentifier()}=%R"))})\"{string.Join("", type.Type.Fields.Select(f => $", {f.Name}.get()"))});"
            );
        });
    }

    public static void WriteStructBufferFormat(this IndentedTextWriter w, ProjectedType type)
    {
        var fmt = string.Join(
            "",
            type.Type.Fields.Select(f =>
                $"{f.ToStructBufferFormat()}:{f.Name.ToPythonIdentifier()}:"
            )
        );

        w.WriteLine($"template<>");
        w.WriteLine(
            $"inline constexpr const char* buffer_format<{type.CppWinrtType}> = \"T{{{fmt}}}\";"
        );
        w.WriteBlankLine();
    }
}
