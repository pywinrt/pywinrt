using System.CodeDom.Compiler;
using Mono.Cecil;

static class StructWriterExtensions
{
    public static void WriteStruct(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"// ----- {type.Name} struct --------------------");
        w.WriteStructConstructor(type);
        w.WriteDeallocFunction(type);
        w.WriteAssignArrayMethod(type);
        w.WriteMethodTable(type);
        foreach (var field in type.Type.Fields)
        {
            w.WriteStructGetSetFunction(type, field);
        }
        w.WriteGetSetTable(type);
        w.WriteStructEqualityMethods(type);
        w.WriteTypeSlotTable(type);
        w.WriteTypeSpec(type);
    }

    static void WriteStructConstructor(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteBlankLine();
        w.WriteLine(
            $"winrt_struct_wrapper<{type.CppWinrtType}>* _new_{type.Name}(PyTypeObject* subclass, PyObject* /*unused*/, PyObject* /*unused*/) noexcept"
        );
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine(
            $"auto self = reinterpret_cast<winrt_struct_wrapper<{type.CppWinrtType}>*>(subclass->tp_alloc(subclass, 0));"
        );
        w.WriteBlankLine();
        w.WriteLine("if (!self)");
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine("return nullptr;");
        w.Indent--;
        w.WriteLine("}");
        w.WriteBlankLine();
        w.WriteLine("std::construct_at(&self->obj);");
        w.WriteBlankLine();
        w.WriteLine("return self;");
        w.Indent--;
        w.WriteLine("}");

        w.WriteBlankLine();
        w.WriteLine(
            $"int _init_{type.Name}(winrt_struct_wrapper<{type.CppWinrtType}>* self, PyObject* args, PyObject* kwds) noexcept"
        );
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine("auto tuple_size = PyTuple_Size(args);");
        w.WriteBlankLine();
        w.WriteLine("if ((tuple_size == 0) && (kwds == nullptr))");
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine("self->obj = {};");
        w.WriteLine("return 0;");
        w.Indent--;
        w.WriteLine("}");
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
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine("return -1;");
        w.Indent--;
        w.WriteLine("}");
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
        w.Indent--;
        w.WriteLine("}");
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
        w.WriteLine("{");
        w.Indent++;

        w.WriteTryCatch(() =>
        {
            w.WriteLine($"return py::convert(self->obj.{field.ToWinrtFieldName()});");
        });

        w.Indent--;
        w.WriteLine("}");

        w.WriteBlankLine();
        w.WriteLine(
            $"static int {type.Name}_set_{field.Name}({type.CppPyWrapperType}* self, PyObject* arg, void* /*unused*/) noexcept"
        );
        w.WriteLine("{");
        w.Indent++;

        w.WriteLine("if (arg == nullptr)");
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine("PyErr_SetString(PyExc_AttributeError, \"can't delete attribute\");");
        w.WriteLine("return -1;");
        w.Indent--;
        w.WriteLine("}");
        w.WriteBlankLine();

        w.WriteTryCatch(
            () =>
            {
                w.WriteLine(
                    $"self->obj.{field.ToWinrtFieldName()} = py::converter<{field.FieldType.ToCppTypeName()}>::convert_to(arg);"
                );
                w.WriteLine("return 0;");
            },
            catchReturn: "-1"
        );

        w.Indent--;
        w.WriteLine("}");
    }

    static void WriteStructEqualityMethods(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteBlankLine();
        w.WriteLine(
            $"static PyObject* _richcompare_{type.Name}({type.CppPyWrapperType}* self, PyObject* other, int op) noexcept"
        );
        w.WriteLine("{");
        w.Indent++;
        w.WriteTryCatch(() =>
        {
            w.WriteLine($"auto _other = py::converter<{type.CppWinrtType}>::convert_to(other);");
            w.WriteBlankLine();
            w.WriteLine("if (op == Py_EQ)");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("if (self->obj == _other)");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("Py_RETURN_TRUE;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("Py_RETURN_FALSE;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("if (op == Py_NE)");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("if (self->obj != _other)");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("Py_RETURN_TRUE;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("Py_RETURN_FALSE;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("Py_RETURN_NOTIMPLEMENTED;");
        });
        w.Indent--;
        w.WriteLine("}");
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
