using System.CodeDom.Compiler;
using System.Collections.ObjectModel;

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
            w.WriteBlock(
                () =>
                {
                    w.WriteLine("auto _gil = py::release_gil();");
                    w.WriteLine($"return {self}First();");
                },
                "())};"
            );
            w.WriteBlankLine();
            w.WriteLine("if (!iter)");
            w.WriteBlock(() => w.WriteLine("return nullptr;"));
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
        w.WriteBlock(() => w.WriteMapContainsBody(type));

        w.WriteLine("Py_ssize_t map_length() noexcept override");
        w.WriteBlock(() => w.WriteMapLengthBody(type));

        w.WriteLine("PyObject* map_subscript(PyObject* key) noexcept override");
        w.WriteBlock(() => w.WriteMapSubscriptBody(type));

        if (type.IsPyMutableMapping)
        {
            w.WriteLine("int map_assign(PyObject* key, PyObject* value) noexcept override");
            w.WriteBlock(() => w.WriteMapAssignBody(type));
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
                w.WriteBlock(() =>
                {
                    w.WriteLine("auto _gil = py::release_gil();");
                    w.WriteLine($"return static_cast<int>({self}HasKey(_key));");
                });
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
            w.WriteBlock(
                () =>
                {
                    w.WriteLine("auto _gil = py::release_gil();");
                    w.WriteLine($"return {self}TryLookup(_key);");
                },
                "();"
            );
            w.WriteBlankLine();
            w.WriteLine("if (!value)");
            w.WriteBlock(() =>
            {
                // there isn't a way to differentiate between a failed lookup
                // and a null value for reference types, so we have to check
                // check HasKey to avoid raising KeyError on actual null values.
                w.WriteLine(
                    "if constexpr (std::is_base_of_v<winrt::Windows::Foundation::IUnknown, decltype(value)>)"
                );
                w.WriteBlock(() =>
                {
                    w.WriteLine("auto has_key = [&]()");
                    w.WriteBlock(
                        () =>
                        {
                            w.WriteLine("auto _gil = py::release_gil();");
                            w.WriteLine($"return {self}HasKey(_key);");
                        },
                        "();"
                    );
                    w.WriteBlankLine();
                    w.WriteLine("if (has_key)");
                    w.WriteBlock(() => w.WriteLine("Py_RETURN_NONE;"));
                });
                w.WriteBlankLine();
                w.WriteLine("PyErr_SetObject(PyExc_KeyError, key);");
                w.WriteLine("return nullptr;");
            });
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
                w.WriteLine("if (!value)");
                w.WriteBlock(() =>
                {
                    w.WriteLine("bool did_remove;");
                    w.WriteBlock(() =>
                    {
                        w.WriteLine("auto _gil = py::release_gil();");
                        w.WriteLine($"did_remove = {self}TryRemove(_key);");
                    });
                    w.WriteLine("if (!did_remove)");
                    w.WriteBlock(() =>
                    {
                        w.WriteLine("PyErr_SetObject(PyExc_KeyError, key);");
                        w.WriteLine("return -1;");
                    });
                    w.WriteBlankLine();
                    w.WriteLine("return 0;");
                });
                w.WriteBlankLine();
                w.WriteLine($"auto _value = py::convert_to<{valueType}>(value);");
                w.WriteBlock(() =>
                {
                    w.WriteLine("auto _gil = py::release_gil();");
                    w.WriteLine($"{self}Insert(_key, _value);");
                });
                w.WriteBlankLine();
                w.WriteLine("return 0;");
            },
            catchReturn: "-1"
        );
    }

    public static string WriteMapPythonSpecialMethods(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        out string keyParamType
    )
    {
        var method = type.Methods.Single(m => m.Name == "Lookup");
        var nullabilityInfo = nullabilityMap.GetValueOrDefault(
            method.Signature,
            new MethodNullabilityInfo(method.Method)
        );
        keyParamType = method
            .Method.Parameters[0]
            .ToPyInParamTyping(ns, nullabilityInfo.Parameters[0].Type, method.GenericArgMap);
        var valueReturnType = method.Method.ToPyReturnTyping(
            ns,
            nullabilityInfo,
            method.GenericArgMap
        );

        w.WriteLine("def __len__(self) -> int: ...");
        w.WriteLine($"def __iter__(self) -> typing.Iterator[{keyParamType}]: ...");
        w.WriteLine("def __contains__(self, key: object) -> bool: ...");
        w.WriteLine($"def __getitem__(self, key: {keyParamType}) -> {valueReturnType}: ...");
        return keyParamType;
    }

    public static void WriteMutableMapPythonSpecialMethods(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        string keyParamType
    )
    {
        var setMethod = type.Methods.Single(m => m.Name == "Insert");
        var setNullabilityInfo = nullabilityMap.GetValueOrDefault(
            setMethod.Signature,
            new MethodNullabilityInfo(setMethod.Method)
        );
        var valParamType = setMethod
            .Method.Parameters[1]
            .ToPyInParamTyping(ns, setNullabilityInfo.Parameters[1].Type, setMethod.GenericArgMap);

        w.WriteLine(
            $"def __setitem__(self, key: {keyParamType}, value: {valParamType}) -> None: ..."
        );
        w.WriteLine($"def __delitem__(self, key: {keyParamType}) -> None: ...");
    }
}
