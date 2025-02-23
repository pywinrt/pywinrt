using System.CodeDom.Compiler;
using Mono.Cecil;

static class StructWriterExtensions
{
    public static void WritePythonStructTyping(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        IReadOnlyDictionary<string, string> packageMap
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
            w.WriteLine("@_property");
            w.WriteLine(
                $"def {field.Name.ToPythonIdentifier()}(self) -> {field.FieldType.ToPyTypeName(ns, new TypeRefNullabilityInfo(field.FieldType), packageMap)}: ..."
            );
        }

        w.WriteLine(
            $"def __new__(cls, {string.Join(", ", type.Type.Fields.Select(f => $"{f.Name.ToPythonIdentifier()}: {f.FieldType.ToPyTypeName(ns, new TypeRefNullabilityInfo(f.FieldType), packageMap)} = {f.FieldType.GetDefaultPyValue(ns, packageMap)}"))}) -> {type.Name}: ..."
        );

        w.WriteLine($"def __replace__(self, /, **changes: typing.Any) -> {type.Name}: ...");

        if (type.Type.IsCustomNumeric())
        {
            if (type.Name != "Plane")
            {
                w.WriteNumberSlotMethodsPyTyping(type);
            }

            w.WriteNumberMethodPyTyping(type);
        }

        if (type.Type.Fields.Count > 1)
        {
            w.WriteLine(
                $"def unpack(self) -> {type.Type.ToPyTupleTyping(ns, packageMap, isUnpack: true)}: ..."
            );
        }

        w.Indent--;
        w.WriteBlankLine();
    }

    public static void WriteStruct(
        this IndentedTextWriter w,
        ProjectedType type,
        QualifiedNamespace ns,
        string moduleSuffix
    )
    {
        w.WriteLine($"// ----- {type.Name} struct --------------------");
        w.WriteBlankLine();
        w.WriteStructFromTuple(type);
        w.WriteStructConstructor(type);
        w.WriteDeallocFunction(type);

        if (type.Type.IsCustomNumeric())
        {
            w.WriteNumberFactoryFunctionMethodDefs(type);
            w.WriteNumberCommonValueMethods(type);
            w.WriteNumberMethods(type);
        }

        w.WriteBlankLine();
        w.WriteAssignArrayMethod(type);
        w.WriteBlankLine();
        w.WriteStructReplaceMethod(type);

        if (type.Type.Fields.Count > 1)
        {
            w.WriteBlankLine();
            w.WriteStructUnpackMethod(type);
        }

        w.WriteMethodTable(type);

        foreach (var field in type.Type.Fields)
        {
            w.WriteStructGetFunction(type, field);
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
        w.WriteTypeSpec(type, ns, moduleSuffix);

        if (type.PyRequiresMetaclass)
        {
            w.WriteMetaclass(type, ns, moduleSuffix);
        }
    }

    static void WriteStructFromTuple(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"{type.CppWinrtType} {type.Name}_from_tuple(PyObject* tuple)");
        w.WriteBlock(() =>
        {
            w.WriteLine($"if (PyTuple_GET_SIZE(tuple) != {type.Type.Fields.Count})");
            w.WriteBlock(() =>
            {
                w.WriteLine("PyErr_SetString(PyExc_TypeError, \"Incorrect number of fields\");");
                w.WriteLine("throw python_exception();");
            });
            w.WriteBlankLine();

            w.WriteLine($"{type.CppWinrtType} result{{}};");
            w.WriteBlankLine();

            for (var i = 0; i < type.Type.Fields.Count; i++)
            {
                var field = type.Type.Fields[i];
                w.WriteLine(
                    $"result.{field.ToWinrtFieldName()} = py::convert_to<{field.FieldType.ToCppTypeName()}>(tuple, {i});"
                );
            }

            w.WriteBlankLine();
            w.WriteLine("return result;");
        });
    }

    static void WriteStructConstructor(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteBlankLine();
        w.WriteLine(
            $"PyObject* _new_{type.Name}(PyTypeObject* subclass, PyObject* args, PyObject* kwds) noexcept"
        );
        w.WriteBlock(() =>
        {
            w.WriteLine($"pyobj_handle self_obj{{(subclass->tp_alloc(subclass, 0))}};");

            w.WriteLine("if (!self_obj)");
            w.WriteBlock(() => w.WriteLine("return nullptr;"));
            w.WriteBlankLine();
            w.WriteLine(
                $"auto self = reinterpret_cast<winrt_struct_wrapper<{type.CppWinrtType}>*>(self_obj.get());"
            );
            w.WriteLine("std::construct_at(&self->obj);");
            w.WriteBlankLine();
            w.WriteLine("auto tuple_size = PyTuple_GET_SIZE(args);");
            w.WriteLine("if ((tuple_size == 0) && (!kwds))");
            w.WriteBlock(() =>
            {
                w.WriteLine("return self_obj.detach();");
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
                $"if (!PyArg_ParseTupleAndKeywords(args, kwds, \"|{type.Type.ToStructFieldFormat()}\", const_cast<char**>(kwlist){type.Type.ToStructFieldParseParameterList()}))"
            );
            w.WriteBlock(() => w.WriteLine("return nullptr;"));
            w.WriteBlankLine();
            w.WriteTryCatch(() =>
            {
                foreach (var field in type.Type.Fields)
                {
                    w.WriteLine(
                        $"self->obj.{field.ToWinrtFieldName()} = {field.ToStructFieldInitializer()};"
                    );
                }
                w.WriteBlankLine();
                w.WriteLine("return self_obj.detach();");
            });
        });
    }

    static void WriteStructGetFunction(
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

    private static void WriteStructReplaceMethod(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine(
            $"PyObject* _replace_{type.Name}({type.CppPyWrapperType}* self, PyObject* args, PyObject* kwds) noexcept"
        );
        w.WriteBlock(
            () =>
                w.WriteTryCatch(() =>
                {
                    foreach (var field in type.Type.Fields)
                    {
                        w.WriteLine(
                            $"{field.FieldType.ToStructFieldType()} _{field.Name}{{{field.ToStructFieldPreInitializer()}}};"
                        );
                    }

                    w.WriteBlankLine();
                    w.WriteLine(
                        $"static const char* kwlist[] = {{{type.Type.ToStructFieldKeywordList()}nullptr}};"
                    );
                    w.WriteLine(
                        $"if (!PyArg_ParseTupleAndKeywords(args, kwds, \"|${type.Type.ToStructFieldFormat()}\", const_cast<char**>(kwlist){type.Type.ToStructFieldParseParameterList()}))"
                    );
                    w.WriteBlock(() => w.WriteLine("return nullptr;"));
                    w.WriteBlankLine();

                    w.WriteLine($"auto copy = self->obj;");

                    foreach (var field in type.Type.Fields)
                    {
                        w.WriteLine(
                            $"copy.{field.ToWinrtFieldName()} = {field.ToStructFieldInitializer(replace: true)};"
                        );
                    }
                    w.WriteBlankLine();
                    w.WriteLine($"return convert(copy);");
                })
        );
    }

    private static void WriteStructUnpackMethod(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine(
            $"PyObject* unpack_{type.Name}({type.CppPyWrapperType}* self, PyObject* /*unused*/) noexcept"
        );
        w.WriteBlock(() =>
        {
            if (type.Type.Fields.Any(f => f.FieldType.IsStruct()))
            {
                w.WriteLine("py::pyobj_handle unpack_str{PyUnicode_InternFromString(\"unpack\")};");
                w.WriteLine("if (!unpack_str)");
                w.WriteBlock(() => w.WriteLine("return nullptr;"));
                w.WriteBlankLine();
            }

            foreach (var field in type.Type.Fields)
            {
                w.WriteLine(
                    $"py::pyobj_handle {field.Name}{{convert(self->obj.{field.ToWinrtFieldName()})}};"
                );
                w.WriteLine($"if (!{field.Name})");
                w.WriteBlock(() => w.WriteLine("return nullptr;"));
                w.WriteBlankLine();

                if (field.FieldType.IsStruct())
                {
                    w.WriteLine(
                        $"{field.Name}.attach(PyObject_CallMethodNoArgs({field.Name}.get(), unpack_str.get()));"
                    );
                    w.WriteLine($"if (!{field.Name})");
                    w.WriteBlock(() => w.WriteLine("return nullptr;"));
                    w.WriteBlankLine();
                }
            }

            w.WriteLine($"pyobj_handle tuple{{PyTuple_New({type.Type.Fields.Count})}};");
            w.WriteLine("if (!tuple)");
            w.WriteBlock(() => w.WriteLine("return nullptr;"));
            w.WriteBlankLine();

            foreach (var (i, field) in type.Type.Fields.Select((f, i) => (i, f)))
            {
                w.WriteLine($"PyTuple_SET_ITEM(tuple.get(), {i}, {field.Name}.detach());");
            }

            w.WriteBlankLine();
            w.WriteLine("return tuple.detach();");
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
