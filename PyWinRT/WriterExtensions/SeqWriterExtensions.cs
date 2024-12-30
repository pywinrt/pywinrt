using System.CodeDom.Compiler;
using System.Collections.ObjectModel;

static class SeqWriterExtensions
{
    public static void WriteSeqGenericInterfaceImpl(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine("Py_ssize_t seq_length() noexcept override");
        w.WriteBlock(() => w.WriteSeqLengthBody(type));

        w.WriteLine("PyObject* seq_item(Py_ssize_t i) noexcept override");
        w.WriteBlock(() => w.WriteSeqItemBody(type));

        w.WriteLine("PyObject* seq_subscript(PyObject* slice) noexcept override");
        w.WriteBlock(() => w.WriteSeqSubscriptBody(type));

        if (type.IsPyMutableSequence)
        {
            w.WriteLine("int seq_assign(Py_ssize_t i, PyObject* value) noexcept override");
            w.WriteBlock(() => w.WriteSeqAssignBody(type));
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

    public static void WriteSeqItemBody(this IndentedTextWriter w, ProjectedType type)
    {
        var self = type.GetMethodInvokeContext(type.Methods.Single(m => m.Name == "GetAt"));

        w.WriteTryCatch(() =>
        {
            w.WriteLine("return py::convert([&]()");
            w.WriteBlock(
                () =>
                {
                    w.WriteLine("auto _gil = py::release_gil();");
                    w.WriteLine($"return {self}GetAt(static_cast<uint32_t>(i));");
                },
                "());"
            );
        });
    }

    public static void WriteSeqSubscriptBody(this IndentedTextWriter w, ProjectedType type)
    {
        var method = type.Methods.First(m => m.Name == "GetAt");
        var collectionType = method.Method.ReturnType.ToCppTypeName(method.GenericArgMap);

        var seqItemInvoke = type.IsGeneric
            ? "seq_item(i)"
            : $"_seq_item_{type.Name.ToNonGeneric()}(self, i)";

        var self = type.GetMethodInvokeContext(method);

        w.WriteTryCatch(() =>
        {
            w.WriteLine("if (PyIndex_Check(slice))");
            w.WriteBlock(() =>
            {
                w.WriteLine("pyobj_handle index{PyNumber_Index(slice)};");
                w.WriteBlankLine();
                w.WriteLine("if (!index)");
                w.WriteBlock(() => w.WriteLine("return nullptr;"));
                w.WriteBlankLine();
                w.WriteLine("auto i = PyNumber_AsSsize_t(index.get(), PyExc_IndexError);");
                w.WriteBlankLine();
                w.WriteLine("if (i == -1 && PyErr_Occurred())");
                w.WriteBlock(() => w.WriteLine("return nullptr;"));
                w.WriteBlankLine();
                w.WriteLine($"return {seqItemInvoke};");
            });
            w.WriteBlankLine();
            w.WriteLine("if (!PySlice_Check(slice))");
            w.WriteBlock(
                () =>
                    w.WriteLine(
                        "PyErr_Format(PyExc_TypeError, \"indices must be integers, not '%s'\", Py_TYPE(slice)->tp_name);"
                    )
            );
            w.WriteBlankLine();
            w.WriteLine("Py_ssize_t start, stop, step, length;");
            w.WriteBlankLine();
            w.WriteLine("auto size = [&]()");
            w.WriteBlock(
                () =>
                {
                    w.WriteLine("auto _gil = py::release_gil();");
                    w.WriteLine($"return {self}Size();");
                },
                "();"
            );
            w.WriteLine(
                "if (PySlice_GetIndicesEx(slice, size, &start, &stop, &step, &length) < 0)"
            );
            w.WriteBlock(() => w.WriteLine("return nullptr;"));
            w.WriteBlankLine();
            w.WriteLine("if (step != 1)");
            w.WriteBlock(() =>
            {
                w.WriteLine(
                    "PyErr_SetString(PyExc_NotImplementedError, \"slices with step other than 1 are not implemented\");"
                );
                w.WriteLine("return nullptr;");
            });
            w.WriteBlankLine();
            w.WriteLine(
                $"winrt::com_array<{collectionType}> items(static_cast<uint32_t>(length), empty_instance<{collectionType}>::get());"
            );
            w.WriteBlankLine();
            w.WriteLine("auto count = [&]()");
            w.WriteBlock(
                () =>
                {
                    w.WriteLine("auto _gil = py::release_gil();");
                    w.WriteLine($"return {self}GetMany(static_cast<uint32_t>(start), items);");
                },
                "();"
            );
            w.WriteBlankLine();
            w.WriteLine("if (count != static_cast<uint32_t>(length))");
            w.WriteBlock(() =>
            {
                w.WriteLine(
                    "PyErr_Format(PyExc_RuntimeError, \"returned count %d did not match requested length %zd\", count, length);"
                );
                w.WriteLine("return nullptr;");
            });
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

        var self = type.GetMethodInvokeContext(method);

        w.WriteTryCatch(
            () =>
            {
                w.WriteLine($"if (!value)");
                w.WriteBlock(() =>
                {
                    w.WriteLine("auto _gil = py::release_gil();");
                    w.WriteLine($"{self}RemoveAt(static_cast<uint32_t>(i));");
                });
                w.WriteLine("else");
                w.WriteBlock(() =>
                {
                    w.WriteLine($"auto _value = py::convert_to<{collectionType}>(value);");
                    w.WriteBlock(() =>
                    {
                        w.WriteLine("auto _gil = py::release_gil();");
                        w.WriteLine($"{self}SetAt(static_cast<uint32_t>(i), _value);");
                    });
                });
                w.WriteBlankLine();
                w.WriteLine("return 0;");
            },
            catchReturn: "-1"
        );
    }

    public static void WriteSeqPythonSpecialMethods(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap
    )
    {
        var method = type.Methods.Single(m => m.Name == "GetAt");
        var nullabilityInfo = nullabilityMap.GetValueOrDefault(
            method.Signature,
            new MethodNullabilityInfo(method.Method)
        );
        var elementType = method.Method.ToPyReturnTyping(ns, nullabilityInfo, method.GenericArgMap);

        w.WriteLine("def __len__(self) -> int: ...");
        w.WriteLine($"def __iter__(self) -> typing.Iterator[{elementType}]: ...");
        w.WriteLine("@typing.overload");
        w.WriteLine($"def __getitem__(self, index: typing.SupportsIndex) -> {elementType}: ...");
        w.WriteLine("@typing.overload");
        w.WriteLine(
            $"def __getitem__(self, index: slice) -> winrt.system.Array[{elementType}]: ..."
        );
    }

    public static void WriteMutableSeqPythonSpecialMethods(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap
    )
    {
        var setMethod = type.Methods.Single(m => m.Name == "SetAt");
        var setNullabilityInfo = nullabilityMap.GetValueOrDefault(
            setMethod.Signature,
            new MethodNullabilityInfo(setMethod.Method)
        );
        var valParamType = setMethod
            .Method.Parameters[1]
            .ToPyInParamTyping(ns, setNullabilityInfo.Parameters[1].Type, setMethod.GenericArgMap);

        w.WriteLine("@typing.overload");
        w.WriteLine($"def __delitem__(self, index: typing.SupportsIndex) -> None: ...");
        w.WriteLine("@typing.overload");
        w.WriteLine($"def __delitem__(self, index: slice) -> None: ...");
        w.WriteLine("@typing.overload");
        w.WriteLine(
            $"def __setitem__(self, index: typing.SupportsIndex, value: {valParamType}) -> None: ..."
        );
        w.WriteLine("@typing.overload");
        w.WriteLine(
            $"def __setitem__(self, index: slice, value: typing.Iterable[{valParamType}]) -> None: ..."
        );
    }
}
