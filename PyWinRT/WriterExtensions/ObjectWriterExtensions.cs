using System.CodeDom.Compiler;
using Mono.Cecil;

static class ObjectWriterExtensions
{
    public static void WritePythonClassTyping(
        this IndentedTextWriter w,
        ProjectedType type,
        string ns
    )
    {
        var metaclass = "";

        if (type.PyRequiresMetaclass)
        {
            var baseType = "type";

            if (type.Type.BaseType is TypeReference b && b.Namespace != "System")
            {
                baseType = $"{b.ToPyTypeName(ns)}_Static";
            }

            if (!type.IsComposable)
            {
                w.WriteLine("@typing.final");
            }

            w.WriteLine($"class {type.Name}_Static({baseType}):");
            w.Indent++;

            var hasMembers = false;

            foreach (var method in type.Methods.Where(m => m.IsStatic))
            {
                if (type.Methods.Count(m => m.Name == method.Name) > 1)
                {
                    w.WriteLine("@typing.overload");
                }

                if (
                    type.IsComposable
                    && !method.IsOverridable
                    && method.IsExclusiveTo
                    // mypy rule: @typing.final can only be applied to the first overload
                    && method == type.Methods.FirstOrDefault(m => m.Name == method.Name, method)
                )
                {
                    w.WriteLine("@typing.final");
                }

                w.WritePythonMethodTyping(method, ns, "cls");

                hasMembers = true;
            }

            foreach (var evt in type.Events.Where(e => e.IsStatic))
            {
                if (type.IsComposable)
                {
                    w.WriteLine("@typing.final");
                }

                w.WritePythonMethodTyping(evt.AddMethod, ns, "cls");

                if (type.IsComposable)
                {
                    w.WriteLine("@typing.final");
                }

                w.WritePythonMethodTyping(evt.RemoveMethod, ns, "cls");

                hasMembers = true;
            }

            foreach (var prop in type.Type.Properties.Where(p => p.GetMethod.IsStatic))
            {
                var name = prop.Name.ToPythonIdentifier(isTypeMethod: true);
                var propType = prop.PropertyType.ToPyTypeName(ns);

                // HACK: work around https://github.com/microsoft/cppwinrt/issues/1287
                // so far, this is the only case in the entire Windows SDK where
                // a property is entirely replaced with one of the same name
                var typeIgnore =
                    type.Namespace == "Windows.UI.Xaml.Controls.Maps"
                    && type.Name == "MapControl"
                    && prop.Name == "StyleProperty"
                        ? "  # type: ignore[misc]"
                        : "";

                w.WriteLine($"@_property{typeIgnore}");

                if (type.IsComposable && prop.SetMethod == null)
                {
                    w.WriteLine("@typing.final");
                }

                w.WriteLine($"def {name}(cls) -> {propType}: ...");

                if (prop.SetMethod is not null)
                {
                    var setType = prop.SetMethod.Parameters[0].ToPyInParamTyping(ns);

                    w.WriteLine($"@{name}.setter");

                    if (type.IsComposable)
                    {
                        w.WriteLine("@typing.final");
                    }

                    w.WriteLine($"def {name}(cls, value: {setType}) -> None: ...");
                }

                hasMembers = true;
            }

            if (!hasMembers)
            {
                w.WriteLine("pass");
            }

            w.Indent--;
            w.WriteBlankLine();

            metaclass = $", metaclass={type.Name}_Static";
        }

        if (type.Category == Category.Interface)
        {
            var generic2 = "";

            if (type.IsGeneric)
            {
                generic2 =
                    $"typing.Generic[{string.Join(", ", type.Type.GenericParameters.Select(p => p.ToPyTypeName(ns)))}]";
            }

            w.WriteLine($"class Implements{type.Name}({generic2}):");
            w.Indent++;
            w.WriteLine("pass");
            w.Indent--;
            w.WriteBlankLine();
        }

        var collection = "";

        if (type.IsPyMapping)
        {
            var method = type.Methods.Single(m => m.Name == "Lookup");
            var keyType = method.Method.Parameters[0].ToPyInParamTyping(ns, method.GenericArgMap);
            var valueType = method.Method.ToPyReturnTyping(ns, method.GenericArgMap);

            if (type.IsPyMutableMapping)
            {
                collection = $", winrt._winrt.MutableMapping[{keyType}, {valueType}]";
            }
            else
            {
                collection = $", winrt._winrt.Mapping[{keyType}, {valueType}]";
            }
        }
        else if (type.IsPySequence)
        {
            var method = type.Methods.Single(m => m.Name == "GetAt");
            var elementType = method.Method.ToPyReturnTyping(ns, method.GenericArgMap);

            if (type.IsPyMutableSequence)
            {
                collection = $", winrt._winrt.MutableSequence[{elementType}]";
            }
            else
            {
                collection = $", winrt._winrt.Sequence[{elementType}]";
            }
        }

        var interfaceTypes = type.Interfaces.AsEnumerable();

        if (type.Category == Category.Interface)
        {
            interfaceTypes = interfaceTypes.Prepend(type.Type);
        }

        var interfaces = string.Join(
            "",
            interfaceTypes.Select(i => $", {i.ToPyTypeName(ns, implementsInterface: true)}")
        );

        var generic = "";

        // typing.Generic is redundant when there is a collection type
        if (type.IsGeneric && string.IsNullOrEmpty(collection))
        {
            generic =
                $", typing.Generic[{string.Join(", ", type.Type.GenericParameters.Select(p => p.ToPyTypeName(ns)))}]";
        }

        if (!type.IsComposable)
        {
            w.WriteLine("@typing.final");
        }

        w.WriteLine(
            $"class {type.Name}({type.Type.BaseType?.ToPyTypeName(ns) ?? "winrt.system.Object"}{interfaces}{collection}{generic}{metaclass}):"
        );
        w.Indent++;

        if (type.IsStatic)
        {
            w.WriteLine("pass");
            w.Indent--;
            w.WriteBlankLine();
            return;
        }

        if (type.IsGeneric)
        {
            w.WriteLine("def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...");
        }

        if (type.IsPyCloseable)
        {
            w.WriteLine("def __enter__(self: Self) -> Self: ...");
            w.WriteLine("def __exit__(self, *args) -> None: ...");
        }

        if (type.IsPyStringable)
        {
            w.WriteLine("def __str__(self) -> str: ...");
        }

        if (type.IsPyBuffer)
        {
            w.WriteLine("def __buffer__(self, flags: int, /) -> memoryview: ...");
            w.WriteLine("def __release_buffer__(self, view: memoryview, /) -> None: ...");
        }

        if (type.IsPyMapping)
        {
            var method = type.Methods.Single(m => m.Name == "Lookup");
            var keyParamType = method
                .Method.Parameters[0]
                .ToPyInParamTyping(ns, method.GenericArgMap);
            var valueReturnType = method.Method.ToPyReturnTyping(ns, method.GenericArgMap);

            w.WriteLine("def __len__(self) -> int: ...");
            w.WriteLine($"def __iter__(self) -> typing.Iterator[{keyParamType}]: ...");
            w.WriteLine("def __contains__(self, key: object) -> bool: ...");
            w.WriteLine($"def __getitem__(self, key: {keyParamType}) -> {valueReturnType}: ...");

            if (type.IsPyMutableMapping)
            {
                var setMethod = type.Methods.Single(m => m.Name == "Insert");
                var valParamType = setMethod
                    .Method.Parameters[1]
                    .ToPyInParamTyping(ns, setMethod.GenericArgMap);

                w.WriteLine(
                    $"def __setitem__(self, key: {keyParamType}, value: {valParamType}) -> None: ..."
                );
                w.WriteLine($"def __delitem__(self, key: {keyParamType}) -> None: ...");
            }
        }
        else if (type.IsPySequence)
        {
            var method = type.Methods.Single(m => m.Name == "First");
            var iterType = method.Method.ToPyReturnTyping(ns, method.GenericArgMap);
            method = type.Methods.Single(m => m.Name == "GetAt");
            var elementType = method.Method.ToPyReturnTyping(ns, method.GenericArgMap);

            w.WriteLine("def __len__(self) -> int: ...");
            w.WriteLine($"def __iter__(self) -> {iterType}: ...");
            w.WriteLine("@typing.overload");
            w.WriteLine(
                $"def __getitem__(self, index: typing.SupportsIndex) -> {elementType}: ..."
            );
            w.WriteLine("@typing.overload");
            w.WriteLine(
                $"def __getitem__(self, index: slice) -> winrt.system.Array[{elementType}]: ..."
            );

            if (type.IsPyMutableSequence)
            {
                var setMethod = type.Methods.Single(m => m.Name == "SetAt");
                var valParamType = setMethod
                    .Method.Parameters[1]
                    .ToPyInParamTyping(ns, setMethod.GenericArgMap);

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
        else if (type.IsPyIterator)
        {
            var prop = type.Type.Properties.Single(p => p.Name == "Current");
            var nextType = prop.PropertyType.ToPyTypeName(ns);
            w.WriteLine("def __iter__(self: Self) -> Self: ...");
            w.WriteLine($"def __next__(self) -> {nextType}: ...");
        }
        else if (type.IsPyIterable)
        {
            var method = type.Methods.Single(m => m.Name == "First");
            var iterType = method.Method.ToPyReturnTyping(ns, method.GenericArgMap);
            w.WriteLine($"def __iter__(self) -> {iterType}: ...");
        }

        if (type.IsPyAwaitable)
        {
            var returnType = "None";

            if (
                type.Type.Namespace == "Windows.Foundation"
                && (
                    type.Type.Name == "IAsyncOperation`1"
                    || type.Type.Name == "IAsyncOperationWithProgress`2"
                )
            )
            {
                returnType = type.Type.GenericParameters[0].ToPyTypeName(ns);
            }
            else
            {
                foreach (var ii in type.Type.Interfaces)
                {
                    // REVISIT: this is probably not as robust as it should
                    // be since it assumes that any class that implements
                    // IAsyncOperation* will reference it this way. We may
                    // need to make this recursive and/or check TypeDef or TypeRef.

                    if (ii.InterfaceType is not GenericInstanceType genericInst)
                    {
                        continue;
                    }

                    if (
                        genericInst.Namespace != "Windows.Foundation"
                        || (
                            genericInst.Name != "IAsyncOperation`1"
                            && genericInst.Name != "IAsyncOperationWithProgress`2"
                        )
                    )
                    {
                        continue;
                    }

                    returnType = genericInst.GenericArguments[0].ToPyTypeName(ns);

                    break;
                }
            }

            w.WriteLine(
                $"def __await__(self) -> typing.Generator[typing.Any, None, {returnType}]: ..."
            );
        }

        if (!type.IsGeneric)
        {
            w.WriteLine("@staticmethod");
            w.WriteLine($"def _from(obj: winrt.system.Object, /) -> {type.Name}: ...");
        }

        foreach (var ctor in type.Constructors)
        {
            var paramList = "";

            if (ctor.Method.Parameters.Any(p => p.IsPythonInParam()))
            {
                paramList =
                    $", {string.Join(", ", ctor.Method.Parameters.Where(p => p.IsPythonInParam()).Select(p => $"{p.Name.ToPythonIdentifier()}: {p.ToPyInParamTyping(ns)}"))}";
            }

            if (type.Constructors.Count(m => m.Name == ctor.Name) > 1)
            {
                w.WriteLine("@typing.overload");
            }

            w.WriteLine($"def __new__(cls: typing.Type[Self]{paramList}) -> Self: ...");
        }

        foreach (var method in type.Methods.Where(m => !m.IsStatic))
        {
            if (type.Methods.Count(m => m.Name == method.Name) > 1)
            {
                w.WriteLine("@typing.overload");
            }

            if (
                type.IsComposable
                && !method.IsOverridable
                && method.IsExclusiveTo
                // mypy rule: @typing.final can only be applied to the first overload
                && method == type.Methods.FirstOrDefault(m => m.Name == method.Name, method)
            )
            {
                // HACK: There are a couple of problematic methods. Subclasses of
                // DependencyObject like to override SetValue with a different
                // parameter type. Subclasses of FlyoutBase like to override ShowAt.
                var typeIgnore = method.IsProblematicOverride() ? "  # type: ignore[misc]" : "";

                w.WriteLine($"@typing.final{typeIgnore}");
            }

            w.WritePythonMethodTyping(method, ns);
        }

        foreach (var evt in type.Events.Where(e => !e.IsStatic))
        {
            if (type.IsComposable)
            {
                w.WriteLine("@typing.final");
            }

            w.WritePythonMethodTyping(evt.AddMethod, ns);

            if (type.IsComposable)
            {
                w.WriteLine("@typing.final");
            }

            w.WritePythonMethodTyping(evt.RemoveMethod, ns);
        }

        foreach (var prop in type.Properties.Where(p => !p.IsStatic))
        {
            var name = prop.Name.ToPythonIdentifier();
            var propType = prop.Property.PropertyType.ToPyTypeName(ns, prop.GenericArgMap);

            // HACK: work around https://github.com/microsoft/cppwinrt/issues/1287
            // so far, this is the only case in the entire Windows SDK where
            // a property is entirely replaced with one of the same name
            var typeIgnore =
                type.Namespace == "Windows.UI.Xaml.Controls.Maps"
                && type.Name == "MapControl"
                && prop.Name == "Style"
                    ? "  # type: ignore[override]"
                    : "";

            w.WriteLine($"@_property{typeIgnore}");

            if (type.IsComposable && prop.SetMethod == null)
            {
                w.WriteLine("@typing.final");
            }

            w.WriteLine($"def {name}(self) -> {propType}: ...");

            if (prop.SetMethod is not null)
            {
                var setType = prop.SetMethod.Method.Parameters[0].ToPyInParamTyping(ns);

                w.WriteLine($"@{name}.setter");

                if (type.IsComposable)
                {
                    w.WriteLine("@typing.final");
                }

                w.WriteLine($"def {name}(self, value: {setType}) -> None: ...");
            }
        }

        w.Indent--;
        w.WriteBlankLine();
    }

    public static void WriteComposableTypeImpl(this IndentedTextWriter w, ProjectedType type)
    {
        w.WriteLine($"struct PyWinrt{type.Name};");
        w.WriteLine(
            $"using BasePyWinrt{type.Name} = {type.CppWinrtType}T<PyWinrt{type.Name}, py::IPywinrtObject>;"
        );
        w.WriteBlankLine();

        w.WriteLine($"struct PyWinrt{type.Name} : py::py_obj_ref, BasePyWinrt{type.Name}");
        w.WriteLine("{");
        w.Indent++;

        foreach (var ctor in type.Constructors)
        {
            var paramList = string.Join(
                ", ",
                ctor.Method.Parameters.Select(p => $"{p.ParameterType.ToCppTypeName()} {p.Name}")
                    .Prepend("PyObject* py_obj")
            );
            var argList = string.Join(", ", ctor.Method.Parameters.Select(p => p.Name));

            w.WriteLine(
                $"PyWinrt{type.Name}({paramList}) : py::py_obj_ref(py_obj), BasePyWinrt{type.Name}({argList}) {{}}"
            );
        }

        w.WriteBlankLine();
        w.WriteLine("int32_t GetPyObject(PyObject*& obj) override");
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine("obj = py::py_obj_ref::get_py_obj();");
        w.WriteLine("return 0;");
        w.Indent--;
        w.WriteLine("}");

        w.WriteBlankLine();
        w.WriteLine(
            "int32_t GetComposableInner(winrt::Windows::Foundation::IInspectable& inner) override"
        );
        w.WriteLine("{");
        w.Indent++;
        if (type.IsComposable)
        {
            w.WriteLine("inner = m_inner;");
            w.WriteLine("return winrt::impl::error_ok;");
        }
        else
        {
            w.WriteLine("return winrt::impl::error_not_implemented;");
        }
        w.Indent--;
        w.WriteLine("}");

        w.WriteBlankLine();
        w.WriteLine(
            $"static void toggle_reference(PyWinrt{type.Name}* instance, bool is_last_reference)"
        );
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine("py::py_obj_ref::toggle_reference(instance, is_last_reference);");
        w.Indent--;
        w.WriteLine("}");

        w.WriteBlankLine();
        w.WriteLine(
            "int32_t query_interface_tearoff(winrt::guid const& id, void** result) const noexcept override"
        );
        w.WriteLine("{");
        w.Indent++;
        w.WriteLine("return py::py_obj_ref::query_interface_tearoff(id, result);");
        w.Indent--;
        w.WriteLine("}");

        foreach (var method in type.Methods.Where(m => m.IsOverridable))
        {
            w.WriteBlankLine();

            var returnType = method.Method.ReturnType.ToCppTypeName();
            var paramList = string.Join(
                ", ",
                method.Method.Parameters.Select(p => p.ToDelegateParam())
            );

            w.WriteLine($"{returnType} {method.CppName}({paramList})");
            w.WriteLine("{");
            w.Indent++;
            w.WriteDelegateInvoke(
                method.Method,
                "method.get()",
                () =>
                {
                    w.WriteLine("py::pyobj_handle self{get_py_obj()};");
                    w.WriteBlankLine();
                    w.WriteLine(
                        $"py::pyobj_handle method{{PyObject_GetAttrString(self.get(), \"{method.PyName}\")}};"
                    );
                    w.WriteLine("if (!method)");
                    w.WriteLine("{");
                    w.Indent++;
                    w.WriteLine("throw python_exception();");
                    w.Indent--;
                    w.WriteLine("}");
                    w.WriteBlankLine();
                }
            );
            w.Indent--;
            w.WriteLine("}");
        }

        w.Indent--;
        w.WriteLine("};");
    }
}
