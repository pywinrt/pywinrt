using System.CodeDom.Compiler;

static class MapWriterExtensions
{
    public static void WriteMapDunderIterBody(this IndentedTextWriter w, ProjectedType type)
    {
        // Mappings are a special case since WinRT iterates `KeyValuePair`s
        // while Python iterates only keys. So we have to wrap the iterator
        // in a helper class to get only the keys.

        w.WriteTryCatch(() =>
        {
            var method = type.Methods.Single(m => m.Name == "First");
            var self = type.GetMethodInvokeContext(method);

            w.WriteLine("py::pyobj_handle iter{py::convert([&]()");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("auto _gil = py::release_gil();");
            w.WriteLine($"return {self}First();");
            w.Indent--;
            w.WriteLine("}())};");
            w.WriteBlankLine();
            w.WriteLine("if (!iter)");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("return nullptr;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("return py::wrap_mapping_iter(iter.get());");
        });
    }

    public static void WriteMapGenericInterfaceDecl(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine("virtual int map_contains(PyObject* key) noexcept = 0;");
        w.WriteLine("virtual Py_ssize_t map_length() noexcept = 0;");
        w.WriteLine("virtual PyObject* map_subscript(PyObject* key) noexcept = 0;");

        if (type.IsPyMutableMapping)
        {
            w.WriteLine("virtual int map_assign(PyObject* key, PyObject* value) noexcept = 0;");
        }
    }

    public static void WriteMapGenericInterfaceImpl(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine("int map_contains(PyObject* key) noexcept override");
        w.WriteLine("{");
        w.Indent++;
        w.WriteMapContainsBody(type);
        w.Indent--;
        w.WriteLine("}");

        w.WriteLine("Py_ssize_t map_length() noexcept override");
        w.WriteLine("{");
        w.Indent++;
        w.WriteMapLengthBody(type);
        w.Indent--;
        w.WriteLine("}");

        w.WriteLine("PyObject* map_subscript(PyObject* key) noexcept override");
        w.WriteLine("{");
        w.Indent++;
        w.WriteMapSubscriptBody(type);
        w.Indent--;
        w.WriteLine("}");

        if (type.IsPyMutableMapping)
        {
            w.WriteLine("int map_assign(PyObject* key, PyObject* value) noexcept override");
            w.WriteLine("{");
            w.Indent++;
            w.WriteMapAssignBody(type);
            w.Indent--;
            w.WriteLine("}");
        }
    }

    public static void WriteMapMethodFunctions(
        this IndentedTextWriter w,
        ProjectedType type,
        Action<string, Action> writeBody
    )
    {
        w.WriteBlankLine();
        w.WriteLine(
            $"static int _map_contains_{type.Name}({type.CppPyWrapperType}* self, PyObject* key) noexcept"
        );
        writeBody("map_contains(key)", () => w.WriteMapContainsBody(type));

        w.WriteBlankLine();
        w.WriteLine(
            $"static Py_ssize_t _map_length_{type.Name}({type.CppPyWrapperType}* self) noexcept"
        );
        writeBody("map_length()", () => w.WriteMapLengthBody(type));

        w.WriteBlankLine();
        w.WriteLine(
            $"static PyObject* _map_subscript_{type.Name}({type.CppPyWrapperType}* self, PyObject* key) noexcept"
        );
        writeBody("map_subscript(key)", () => w.WriteMapSubscriptBody(type));

        if (type.IsPyMutableMapping)
        {
            w.WriteBlankLine();
            w.WriteLine(
                $"static int _map_assign_{type.Name}({type.CppPyWrapperType}* self, PyObject* key, PyObject* value) noexcept"
            );
            writeBody("map_assign(key, value)", () => w.WriteMapAssignBody(type));
        }
    }

    private static void WriteMapContainsBody(this IndentedTextWriter w, ProjectedType type)
    {
        var method = type.Methods.Single(m => m.Name == "HasKey");
        var keyType = method.Method.Parameters[0].ParameterType.ToCppTypeName(method.GenericArgMap);
        var self = type.GetMethodInvokeContext(method);

        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"auto _key = py::convert_to<{keyType}>(key);");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine("auto _gil = py::release_gil();");
                w.WriteLine($"return static_cast<int>({self}HasKey(_key));");
                w.Indent--;
                w.WriteLine("}");
            },
            catchReturn: "-1"
        );
    }

    private static void WriteMapLengthBody(this IndentedTextWriter w, ProjectedType type)
    {
        var self = type.GetMethodInvokeContext(
            type.Properties.Single(m => m.Name == "Size").GetMethod
        );

        w.WriteTryCatch(
            () =>
            {
                w.WriteLine("auto _gil = py::release_gil();");
                w.WriteLine($"return static_cast<Py_ssize_t>({self}Size());");
            },
            catchReturn: "-1"
        );
    }

    /// <summary>
    /// Writes the body of the mp_subscript slot for the __getitem__ special method.
    /// </summary>
    private static void WriteMapSubscriptBody(this IndentedTextWriter w, ProjectedType type)
    {
        var method = type.Methods.Single(m => m.Name == "Lookup");
        var keyType = method.Method.Parameters[0].ParameterType.ToCppTypeName(method.GenericArgMap);
        var self = type.GetMethodInvokeContext(method);

        w.WriteTryCatch(() =>
        {
            w.WriteLine($"auto _key = py::convert_to<{keyType}>(key);");
            // we use the CppWinRT extension TryLookup so we can raise
            // KeyError on failure.
            w.WriteLine("auto value = [&]()");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("auto _gil = py::release_gil();");
            w.WriteLine($"return {self}TryLookup(_key);");
            w.Indent--;
            w.WriteLine("}();");
            w.WriteBlankLine();
            w.WriteLine("if (!value) {");
            w.Indent++;

            // there isn't a way to differentiate between a failed lookup
            // and a null value for reference types, so we have to check
            // check HasKey to avoid raising KeyError on actual null values.
            w.WriteLine(
                "if constexpr (std::is_base_of_v<winrt::Windows::Foundation::IUnknown, decltype(value)>)"
            );
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("auto has_key = [&]()");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("auto _gil = py::release_gil();");
            w.WriteLine($"return {self}HasKey(_key);");
            w.Indent--;
            w.WriteLine("}();");
            w.WriteBlankLine();
            w.WriteLine("if (has_key)");
            w.WriteLine("{");
            w.Indent++;
            w.WriteLine("Py_RETURN_NONE;");
            w.Indent--;
            w.WriteLine("}");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("PyErr_SetObject(PyExc_KeyError, key);");
            w.WriteLine("return nullptr;");
            w.Indent--;
            w.WriteLine("}");
            w.WriteBlankLine();
            w.WriteLine("return py::convert(value);");
        });
    }

    /// <summary>
    /// Writes the body of the mp_ass_subscript slot for the __setitem__ and __delitem__
    /// special methods.
    /// </summary>
    private static void WriteMapAssignBody(this IndentedTextWriter w, ProjectedType type)
    {
        var method = type.Methods.Single(m => m.Name == "Lookup");
        var keyType = method.Method.Parameters[0].ParameterType.ToCppTypeName(method.GenericArgMap);
        var valueType = method.Method.ReturnType.ToCppTypeName(method.GenericArgMap);
        var self = type.GetMethodInvokeContext(method);

        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"auto _key = py::convert_to<{keyType}>(key);");
                w.WriteBlankLine();
                w.WriteLine("if (value == nullptr) {");
                w.Indent++;
                w.WriteLine("bool did_remove;");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine("auto _gil = py::release_gil();");
                w.WriteLine($"did_remove = {self}TryRemove(_key);");
                w.Indent--;
                w.WriteLine("}");
                w.WriteLine("if (!did_remove) {");
                w.Indent++;
                w.WriteLine("PyErr_SetObject(PyExc_KeyError, key);");
                w.WriteLine("return -1;");
                w.Indent--;
                w.WriteLine("}");
                w.WriteBlankLine();
                w.WriteLine("return 0;");
                w.Indent--;
                w.WriteLine("}");
                w.WriteBlankLine();
                w.WriteLine($"auto _value = py::convert_to<{valueType}>(value);");
                w.WriteLine("{");
                w.Indent++;
                w.WriteLine("auto _gil = py::release_gil();");
                w.WriteLine($"{self}Insert(_key, _value);");
                w.Indent--;
                w.WriteLine("}");
                w.WriteBlankLine();
                w.WriteLine("return 0;");
            },
            catchReturn: "-1"
        );
    }
}
