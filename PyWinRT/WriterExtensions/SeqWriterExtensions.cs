using System.CodeDom.Compiler;

static class SeqWriterExtensions
{
    public static void WriteSeqGenericInterfaceImpl(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine("Py_ssize_t seq_length() noexcept override");
        w.WriteLine("{");
        w.Indent++;
        w.WriteSeqLengthBody(type);
        w.Indent--;
        w.WriteLine("}");

        w.WriteLine("PyObject* seq_item(Py_ssize_t i) noexcept override");
        w.WriteLine("{");
        w.Indent++;
        w.WriteSeqItemBody(type);
        w.Indent--;
        w.WriteLine("}");

        w.WriteLine("PyObject* seq_subscript(PyObject* slice) noexcept override");
        w.WriteLine("{");
        w.Indent++;
        w.WriteSeqSubscriptBody(type);
        w.Indent--;
        w.WriteLine("}");

        if (type.IsPyMutableSequence)
        {
            w.WriteLine("int seq_assign(Py_ssize_t i, PyObject* value) noexcept override");
            w.WriteLine("{");
            w.Indent++;
            w.WriteSeqAssignBody(type);
            w.Indent--;
            w.WriteLine("}");
        }
    }

    public static void WriteSeqMethodFunctions(
        this IndentedTextWriter w,
        ProjectedType type,
        Action<string, Action> writeBody
    )
    {
        w.WriteBlankLine();
        w.WriteLine(
            $"static Py_ssize_t _seq_length_{type.Name}({type.CppPyWrapperType}* self) noexcept"
        );
        writeBody("seq_length()", () => w.WriteSeqLengthBody(type));

        w.WriteBlankLine();
        w.WriteLine(
            $"static PyObject* _seq_item_{type.Name}({type.CppPyWrapperType}* self, Py_ssize_t i) noexcept"
        );
        writeBody("seq_item(i)", () => w.WriteSeqItemBody(type));

        w.WriteBlankLine();
        w.WriteLine(
            $"static PyObject* _seq_subscript_{type.Name}({type.CppPyWrapperType}* self, PyObject* slice) noexcept"
        );
        writeBody("seq_subscript(slice)", () => w.WriteSeqSubscriptBody(type));

        if (type.IsPyMutableSequence)
        {
            w.WriteBlankLine();
            w.WriteLine(
                $"static int _seq_assign_{type.Name}({type.CppPyWrapperType}* self, Py_ssize_t i, PyObject* value) noexcept"
            );
            writeBody("seq_assign(i, value)", () => w.WriteSeqAssignBody(type));
        }
    }

    public static void WriteSeqLengthBody(this IndentedTextWriter w, ProjectedType type)
    {
        var self = type.IsGeneric ? "_obj" : "self->obj";

        w.WriteTryCatch(
            () => w.WriteLine($"return static_cast<Py_ssize_t>({self}.Size());"),
            catchReturn: "-1"
        );
    }

    public static void WriteSeqItemBody(this IndentedTextWriter w, ProjectedType type)
    {
        var self = type.IsGeneric ? "_obj" : "self->obj";

        w.WriteTryCatch(
            () => w.WriteLine($"return py::convert({self}.GetAt(static_cast<uint32_t>(i)));")
        );
    }

    public static void WriteSeqSubscriptBody(this IndentedTextWriter w, ProjectedType type)
    {
        var method = type.Methods.First(m => m.Name == "GetAt");
        var collectionType = method.Method.ReturnType.ToCppTypeName(method.GenericArgMap);

        var seqItemInvoke = type.IsGeneric
            ? "seq_item(i)"
            : $"_seq_item_{type.Name.ToNonGeneric()}(self, i)";

        var self = type.IsGeneric ? "_obj" : "self->obj";

        w.WriteTryCatch(() =>
        {
            w.WriteLine("if (PyIndex_Check(slice))");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("pyobj_handle index{PyNumber_Index(slice)};");
            w.WriteBlankLine();
            w.WriteLine("if (!index)");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("return nullptr;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("auto i = PyNumber_AsSsize_t(index.get(), PyExc_IndexError);");
            w.WriteBlankLine();
            w.WriteLine("if (i == -1 && PyErr_Occurred())");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("return nullptr;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine($"return {seqItemInvoke};");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("if (!PySlice_Check(slice))");
            w.WriteLine("{");
            w.Indent++;
            // FIXME: put on single line
            w.WriteLine("PyErr_Format(");
            w.Indent++;
            w.WriteLine("PyExc_TypeError,");
            w.WriteLine("\"indicies must be integers, not '%s'\",");
            w.WriteLine("Py_TYPE(slice)->tp_name);");
            w.Indent--;
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("Py_ssize_t start, stop, step, length;");
            w.WriteBlankLine();
            // FIXME: put on single line
            w.WriteLine($"if (PySlice_GetIndicesEx(");
            w.Indent++;
            w.Indent++;
            w.WriteLine($"slice, {self}.Size(), &start, &stop, &step, &length)");
            w.Indent--;
            w.WriteLine("< 0)");
            w.Indent--;
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("return nullptr;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("if (step != 1)");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine(
                "PyErr_SetString(PyExc_NotImplementedError, \"slices with step other than 1 are not implemented\");"
            );
            w.WriteLine("return nullptr;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine(
                $"winrt::com_array<{collectionType}> items(static_cast<uint32_t>(length), empty_instance<{collectionType}>::get());"
            );
            w.WriteBlankLine();
            w.WriteLine($"auto count = {self}.GetMany(static_cast<uint32_t>(start), items);");
            w.WriteBlankLine();
            w.WriteLine("if (count != static_cast<uint32_t>(length))");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine(
                "PyErr_Format(PyExc_RuntimeError, \"returned count %d did not match requested length %zd\", count, length);"
            );
            w.WriteLine("return nullptr;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("return convert(items);");
        });
    }

    private static void WriteSeqAssignBody(this IndentedTextWriter w, ProjectedType type)
    {
        var method = type.Methods.First(m => m.Name == "SetAt");
        var collectionType = method
            .Method.Parameters[1]
            .ParameterType.ToCppTypeName(method.GenericArgMap);

        var self = type.IsGeneric ? "_obj" : "self->obj";

        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"if (!value)");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine($"{self}.RemoveAt(static_cast<uint32_t>(i));");
                w.Indent--;
                w.WriteLine("}");
                w.WriteLine("else");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine(
                    $"{self}.SetAt(static_cast<uint32_t>(i), py::convert_to<{collectionType}>(value));"
                );
                w.Indent--;
                w.WriteLine("}");
                w.WriteBlankLine();
                w.WriteLine("return 0;");
            },
            catchReturn: "-1"
        );
    }
}
