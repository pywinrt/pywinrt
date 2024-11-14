using System.CodeDom.Compiler;
using Mono.Cecil;

static class FileWriters
{
    public static void WriteBaseFiles(DirectoryInfo path)
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        // just one file so far, the version header

        w.WriteLicense();
        w.WriteBlankLine();
        w.WriteLine("#pragma once");
        w.WriteLine($"#define PYWINRT_VERSION \"{PyWinRT.VersionString}\"");

        sw.WriteFileIfChanged(path, "pywinrt_version.h");
    }

    internal static void WriteNamespaceFiles(
        DirectoryInfo outputPath,
        DirectoryInfo? headerPath,
        string ns,
        IEnumerable<TypeDefinition> typeDefinitions,
        bool componentDlls
    )
    {
        var nsPackageName = $"winrt-{ns}";
        var nsPackageDir = new DirectoryInfo(Path.Combine(outputPath.FullName, nsPackageName));
        var nsWinrtDir = new DirectoryInfo(Path.Combine(nsPackageDir.FullName, "winrt"));
        var nsDir = nsWinrtDir;

        foreach (var nsSegment in ns.Split('.'))
        {
            var segment = nsSegment.ToLowerInvariant().ToPythonIdentifier();
            nsDir = new DirectoryInfo(Path.Combine(nsDir.FullName, segment));
        }

        var headerDir = headerPath ?? nsPackageDir;

        var members = new Members(typeDefinitions);

        if (
            !members
                .Structs.Concat(members.Enums)
                .Concat(members.Classes)
                .Concat(members.Interfaces)
                .Concat(members.Delegates)
                .Any()
        )
        {
            return;
        }

        WriteNamespaceCpp(nsPackageDir, ns, members, componentDlls);
        WriteNamespaceH(headerDir, ns, members, componentDlls);
        WriteNamespaceDunderInitPy(nsDir, ns, members, componentDlls);
        WriteNamespacePyi(nsWinrtDir, ns, members);
        WritePyWinRTVersionTxt(nsPackageDir);
        WriteRequirementsTxt(nsPackageDir);

        if (members.GetReferencedNamespaces(includeDelegates: true).Any())
        {
            WriteAllRequirementsTxt(nsPackageDir, members);
        }
    }

    private static void WriteAllRequirementsTxt(DirectoryInfo nsPackageDir, Members members)
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.WriteLicense("#");
        w.WriteBlankLine();

        foreach (var ns in members.GetReferencedNamespaces(includeDelegates: true))
        {
            w.WriteLine($"winrt-{ns}[all]=={PyWinRT.VersionString}");
        }

        sw.WriteFileIfChanged(nsPackageDir, "all-requirements.txt");
    }

    private static void WriteRequirementsTxt(DirectoryInfo nsPackageDir)
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.WriteLicense("#");
        w.WriteBlankLine();
        w.WriteLine($"winrt-runtime=={PyWinRT.VersionString}");

        sw.WriteFileIfChanged(nsPackageDir, "requirements.txt");
    }

    private static void WritePyWinRTVersionTxt(DirectoryInfo nsPackageDir)
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.Write(PyWinRT.VersionString);

        sw.WriteFileIfChanged(nsPackageDir, "pywinrt-version.txt");
    }

    private static void WriteNamespacePyi(DirectoryInfo nsWinrtDir, string ns, Members members)
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.WriteLicense("#");
        w.WriteBlankLine();

        w.WriteLine("import datetime");
        w.WriteLine("import sys");
        w.WriteLine("import types");
        w.WriteLine("import typing");
        w.WriteLine("import uuid as _uuid");
        w.WriteLine("from builtins import property as _property");
        w.WriteBlankLine();
        w.WriteLine("import winrt._winrt");
        w.WriteLine("import winrt.system");

        var referencedNamespaces = members.GetReferencedNamespaces(
            includeInheritedInterfaces: true
        );

        foreach (var rns in referencedNamespaces)
        {
            var moduleName = rns.ToLowerInvariant();
            var alias = moduleName.Replace(".", "_");
            w.WriteLine($"import winrt.{moduleName} as {alias}");
        }

        w.WriteBlankLine();

        if (members.Enums.Count != 0)
        {
            w.WriteLine(
                $"from {ns.ToPyModuleName()} import {string.Join(", ", members.Enums.Select(e => e.Name))}"
            );
        }

        if (members.Delegates.Count != 0)
        {
            w.WriteLine(
                $"from {ns.ToPyModuleName()} import {string.Join(", ", members.Delegates.Select(d => d.Name))}"
            );
        }

        if (members.Enums.Count != 0 || members.Delegates.Count != 0)
        {
            w.WriteBlankLine();
        }

        w.WriteLine("Self = typing.TypeVar('Self')");

        foreach (
            var type in members
                .Interfaces.SelectMany(i => i.Type.GenericParameters.Select(p => p.Name))
                .Distinct()
                .Order()
        )
        {
            w.WriteLine($"{type} = typing.TypeVar('{type}')");
        }

        w.WriteBlankLine();

        foreach (var type in members.Structs.Where(s => !s.Type.IsCustomizedStruct()))
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
                    $"{field.Name.ToPythonIdentifier()}: {field.FieldType.ToPyTypeName(ns)}"
                );
            }

            w.WriteLine(
                $"def __init__(self, {string.Join(", ", type.Type.Fields.Select(f => $"{f.Name.ToPythonIdentifier()}: {f.FieldType.ToPyTypeName(ns)} = {f.FieldType.GetDefaultPyValue(ns)}"))}) -> None: ..."
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

        foreach (var type in members.Classes.Concat(members.Interfaces))
        {
            var metaclass = "";

            if (type.PyRequiresMetaclass)
            {
                if (!type.IsComposable)
                {
                    w.WriteLine("@typing.final");
                }

                w.WriteLine($"class {type.Name}_Static(type):");
                w.Indent++;

                foreach (var method in type.Methods.Where(m => m.IsStatic))
                {
                    if (type.Methods.Count(m => m.Name == method.Name) > 1)
                    {
                        w.WriteLine("@typing.overload");
                    }

                    w.WritePythonMethodTyping(method, ns, "cls");
                }

                foreach (var evt in type.Events.Where(e => e.IsStatic))
                {
                    w.WritePythonMethodTyping(evt.AddMethod, ns, "cls");
                    w.WritePythonMethodTyping(evt.RemoveMethod, ns, "cls");
                }

                foreach (var prop in type.Type.Properties.Where(p => p.GetMethod.IsStatic))
                {
                    var name = prop.Name.ToPythonIdentifier(isTypeMethod: true);
                    var propType = prop.PropertyType.ToPyTypeName(ns);

                    w.WriteLine("@_property");
                    w.WriteLine($"def {name}(cls) -> {propType}: ...");

                    if (prop.SetMethod is not null)
                    {
                        w.WriteLine($"@{name}.setter");
                        w.WriteLine($"def {name}(cls, value: {propType}) -> None: ...");
                    }
                }

                w.Indent--;
                w.WriteBlankLine();

                metaclass = $", metaclass={type.Name}_Static";
            }

            var collection = "";

            if (type.IsPyMapping)
            {
                var method = type.Methods.Single(m => m.Name == "Lookup");
                var keyType = method
                    .Method.Parameters[0]
                    .ParameterType.ToPyTypeName(ns, method.GenericArgMap);
                var valueType = method.Method.ReturnType.ToPyTypeName(ns, method.GenericArgMap);

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
                var elementType = method.Method.ReturnType.ToPyTypeName(ns, method.GenericArgMap);

                if (type.IsPyMutableSequence)
                {
                    collection = $", winrt._winrt.MutableSequence[{elementType}]";
                }
                else
                {
                    collection = $", winrt._winrt.Sequence[{elementType}]";
                }
            }

            var interfaces = string.Join(
                "",
                type.Interfaces.Select(i => $"{i.ToPyTypeName(ns)}, ")
            );

            var generic = "";

            // typing.Generic is redundant when there is a collection type
            if (type.IsGeneric && string.IsNullOrEmpty(collection))
            {
                generic =
                    $", typing.Generic[{string.Join(", ", type.Type.GenericParameters.Select(p => p.ToPyTypeName(ns)))}]";
            }

            if (type.Category != Category.Interface && !type.IsComposable)
            {
                w.WriteLine("@typing.final");
            }

            w.WriteLine(
                $"class {type.Name}({interfaces}winrt.system.Object{collection}{generic}{metaclass}):"
            );
            w.Indent++;

            if (type.IsStatic)
            {
                w.WriteLine("pass");
                w.Indent--;
                w.WriteBlankLine();
                continue;
            }

            if (type.IsGeneric)
            {
                w.WriteLine(
                    "def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ..."
                );
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
                var keyType = method
                    .Method.Parameters[0]
                    .ParameterType.ToPyTypeName(ns, method.GenericArgMap);
                var valueType = method.Method.ReturnType.ToPyTypeName(ns, method.GenericArgMap);

                w.WriteLine("def __len__(self) -> int: ...");
                w.WriteLine($"def __iter__(self) -> typing.Iterator[{keyType}]: ...");
                w.WriteLine("def __contains__(self, key: object) -> bool: ...");
                w.WriteLine($"def __getitem__(self, key: {keyType}) -> {valueType}: ...");

                if (type.IsPyMutableMapping)
                {
                    w.WriteLine(
                        $"def __setitem__(self, key: {keyType}, value: {valueType}) -> None: ..."
                    );
                    w.WriteLine($"def __delitem__(self, key: {keyType}) -> None: ...");
                }
            }
            else if (type.IsPySequence)
            {
                var method = type.Methods.Single(m => m.Name == "First");
                var iterType = method.Method.ReturnType.ToPyTypeName(ns, method.GenericArgMap);
                method = type.Methods.Single(m => m.Name == "GetAt");
                var elementType = method.Method.ReturnType.ToPyTypeName(ns, method.GenericArgMap);

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
                    w.WriteLine("@typing.overload");
                    w.WriteLine($"def __delitem__(self, index: typing.SupportsIndex) -> None: ...");
                    w.WriteLine("@typing.overload");
                    w.WriteLine($"def __delitem__(self, index: slice) -> None: ...");
                    w.WriteLine("@typing.overload");
                    w.WriteLine(
                        $"def __setitem__(self, index: typing.SupportsIndex, value: {elementType}) -> None: ..."
                    );
                    w.WriteLine("@typing.overload");
                    w.WriteLine(
                        $"def __setitem__(self, index: slice, value: typing.Iterable[{elementType}]) -> None: ..."
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
                var iterType = method.Method.ReturnType.ToPyTypeName(ns, method.GenericArgMap);
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

                w.WriteLine(
                    $"def __new__(cls: typing.Type[{type.Name}]{paramList}) -> {type.Name}: ..."
                );
            }

            foreach (var method in type.Methods.Where(m => !m.IsStatic))
            {
                if (type.Methods.Count(m => m.Name == method.Name) > 1)
                {
                    w.WriteLine("@typing.overload");
                }

                w.WritePythonMethodTyping(method, ns);
            }

            foreach (var evt in type.Events.Where(e => !e.IsStatic))
            {
                w.WritePythonMethodTyping(evt.AddMethod, ns);
                w.WritePythonMethodTyping(evt.RemoveMethod, ns);
            }

            foreach (var prop in type.Properties.Where(p => !p.IsStatic))
            {
                var name = prop.Name.ToPythonIdentifier();
                var propType = prop.Property.PropertyType.ToPyTypeName(ns, prop.GenericArgMap);

                w.WriteLine("@_property");
                w.WriteLine($"def {name}(self) -> {propType}: ...");

                if (prop.SetMethod is not null)
                {
                    w.WriteLine($"@{name}.setter");
                    w.WriteLine($"def {name}(self, value: {propType}) -> None: ...");
                }
            }

            w.Indent--;
            w.WriteBlankLine();
        }

        sw.WriteFileIfChanged(nsWinrtDir, $"{ns.ToNsModuleName()}.pyi");
    }

    private static void WriteNamespaceDunderInitPy(
        DirectoryInfo nsDir,
        string ns,
        Members members,
        bool componentDlls
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.WriteLicense("#");
        w.WriteBlankLine();

        if (members.Enums.Count != 0)
        {
            w.WriteLine("import enum");
        }

        if (members.Delegates.Count != 0)
        {
            w.WriteLine("import typing");
            w.WriteLine("import uuid as _uuid");
        }

        if (members.Enums.Count != 0 || members.Delegates.Count != 0)
        {
            w.WriteBlankLine();
        }

        w.WriteLine("import winrt.system");

        var allExtensionTypes = members
            .Structs.Where(s => !s.Type.IsCustomizedStruct())
            .Concat(members.Classes)
            .Concat(members.Interfaces);

        if (allExtensionTypes.Any())
        {
            w.WriteLine($"from winrt.{ns.ToNsModuleName()} import (");
            w.Indent++;

            foreach (var type in allExtensionTypes)
            {
                w.WriteLine($"{type.Name},");
            }

            w.Indent--;
            w.WriteLine(")");
        }

        // Since not all packages may be installed, delegates can't safely
        // import their parameter types at runtime. So we conditionally import
        // them when type checking is enabled and quote the types to avoid
        // to avoid runtime errors.

        var delegateReferencedNamespaces = new SortedSet<string>(
            members.Delegates.SelectMany(d =>
            {
                var method = d.Type.Methods.Single(m => m.Name == "Invoke");

                return method
                    .Parameters.Select(p => p.ParameterType)
                    .Append(method.ReturnType)
                    .Where(t =>
                        !t.IsGenericParameter
                        && !(
                            t.Namespace == "Windows.Foundation.Collections"
                            && t.Name == "IIterable`1"
                        )
                    )
                    .Select(t => t.Namespace)
                    .Where(n => n != ns && n != "System");
            })
        );

        if (delegateReferencedNamespaces.Count > 0)
        {
            w.WriteBlankLine();
            w.WriteLine("if typing.TYPE_CHECKING:");
            w.Indent++;

            foreach (var n in delegateReferencedNamespaces)
            {
                w.WriteLine($"import {n.ToPyModuleName()} as {n.ToPyModuleAlias()}");
            }

            w.Indent--;
        }

        if (componentDlls)
        {
            w.WriteBlankLine();
            w.WriteLine(
                "_dll_search_path_cookie_ = winrt.system._register_dll_search_path(__file__)"
            );
        }

        w.WriteBlankLine();
        w.WriteLine($"__all__ = [");
        w.Indent++;

        foreach (var type in members.Enums.Concat(allExtensionTypes).Concat(members.Delegates))
        {
            w.WriteLine($"\"{type.Name}\",");
        }

        w.Indent--;
        w.WriteLine("]");

        foreach (
            var type in members
                .Delegates.SelectMany(d => d.Type.GenericParameters.Select(p => p.Name))
                .Distinct()
                .Order()
        )
        {
            w.WriteLine($"{type} = typing.TypeVar('{type}')");
        }

        foreach (var type in members.Enums)
        {
            w.WriteBlankLine();
            w.WriteLine(
                $"class {type.Name}(enum.{(type.Type.HasFlagsAttribute() ? "IntFlag" : "IntEnum")}):"
            );

            w.Indent++;

            foreach (var field in type.Type.Fields)
            {
                if (field.Constant is not null)
                {
                    var value = type.Type.HasFlagsAttribute()
                        ? $"0x{field.Constant:X}"
                        : field.Constant.ToString();
                    w.WriteLine($"{field.Name.ToPythonConstant()} = {value}");
                }
            }

            w.Indent--;
        }

        w.WriteBlankLine();

        foreach (var type in allExtensionTypes)
        {
            if (type.IsPyMutableMapping)
            {
                w.WriteLine($"winrt.system._mixin_mutable_mapping({type.Name})");
            }
            else if (type.IsPyMapping)
            {
                w.WriteLine($"winrt.system._mixin_mapping({type.Name})");
            }
            else if (type.IsPyMutableSequence)
            {
                w.WriteLine($"winrt.system._mixin_mutable_sequence({type.Name})");
            }
            else if (type.IsPySequence)
            {
                w.WriteLine($"winrt.system._mixin_sequence({type.Name})");
            }
        }

        foreach (var type in members.Delegates)
        {
            var invoke = type.Type.Methods.Single(m => m.Name == "Invoke");
            var paramTypes = invoke
                .Parameters.Where(p => p.IsPythonInParam())
                .Select(p => p.ToPyInParamTyping(ns, quoteImportedTypes: true));

            w.WriteLine(
                $"{type.Name} = typing.Callable[[{string.Join(", ", paramTypes)}], {invoke.ToPyReturnTyping(ns, quoteImportedTypes: true)}]"
            );
        }

        sw.WriteFileIfChanged(nsDir, "__init__.py");
    }

    private static void WriteNamespaceH(
        DirectoryInfo headerDir,
        string ns,
        Members members,
        bool componentDlls
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        var referencedNamespaces = members.GetReferencedNamespaces(includeDelegates: true);

        w.WriteLicense();
        w.WriteBlankLine();
        w.WriteLine("#pragma once");
        w.WriteLine();
        w.WriteLine("#include \"pybase.h\"");
        w.WriteLine(
            $"static_assert(winrt::check_version(PYWINRT_VERSION, \"{PyWinRT.VersionString}\"), \"Mismatched Py/WinRT headers.\");"
        );

        foreach (var rns in referencedNamespaces)
        {
            w.WriteBlankLine();
            w.WriteLine($"#if __has_include(\"py.{rns}.h\")");
            w.WriteLine($"#include \"py.{rns}.h\"");
            w.WriteLine("#endif");
        }

        w.WriteBlankLine();

        foreach (var rns in referencedNamespaces)
        {
            w.WriteLine($"#include <winrt/{rns}.h>");
        }

        w.WriteBlankLine();
        w.WriteLine($"#include <winrt/{ns}.h>");
        w.WriteBlankLine();

        w.WriteLine($"namespace py::proj::{ns.ToCppNamespace()}");
        w.WriteLine("{");
        w.Indent++;

        foreach (
            var (i, iface) in members
                .Interfaces.Where(i => i.IsGeneric)
                .Select((iface, i) => (i, iface))
        )
        {
            if (i > 0)
            {
                w.WriteBlankLine();
            }

            w.WriteGenericInterfaceDecl(iface);
        }

        w.Indent--;
        w.WriteLine("}");

        w.WriteBlankLine();
        w.WriteLine($"namespace py::impl::{ns.ToCppNamespace()}");
        w.WriteLine("{");
        w.Indent++;

        var n = 0;

        foreach (var del in members.Delegates)
        {
            if (n++ > 0)
            {
                w.WriteBlankLine();
            }

            w.WriteDelegateCallableWrapper(del);
        }

        foreach (var iface in members.Interfaces.Where(i => i.IsGeneric))
        {
            if (n++ > 0)
            {
                w.WriteBlankLine();
            }

            w.WriteGenericInterfaceImpl(iface, componentDlls);
        }

        w.Indent--;
        w.WriteLine("}");

        w.WriteBlankLine();
        w.WriteLine($"namespace py::wrapper::{ns.ToCppNamespace()}");
        w.WriteLine("{");
        w.Indent++;

        foreach (
            var type in members
                .Classes.Concat(members.Interfaces)
                .Concat(members.Structs.Where(s => !s.Type.IsCustomizedStruct()))
        )
        {
            w.WritePythonWrapperAlias(type);
        }

        w.Indent--;
        w.WriteLine("}");

        w.WriteBlankLine();
        w.WriteLine("namespace py");
        w.WriteLine("{");
        w.Indent++;

        foreach (var type in members.Enums)
        {
            w.WriteEnumBufferFormat(type);
        }

        foreach (var type in members.Structs.Where(s => !s.Type.IsCustomizedStruct()))
        {
            w.WriteStructBufferFormat(type);
        }

        foreach (
            var type in members
                .Enums.Concat(members.Classes)
                .Concat(members.Interfaces)
                .Concat(members.Structs)
        )
        {
            w.WritePyTypeSpecializationStruct(type);
        }

        foreach (var type in members.Interfaces.Where(i => i.IsGeneric))
        {
            w.WriteGenericInterfaceTypeMapper(type);
        }

        foreach (var type in members.Delegates)
        {
            w.WriteDelegateTypeMapper(type);
        }

        w.Indent--;
        w.WriteLine("}");

        sw.WriteFileIfChanged(headerDir, $"py.{ns}.h");
    }

    private static void WriteNamespaceCpp(
        DirectoryInfo nsPackageDir,
        string ns,
        Members members,
        bool componentDlls
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.WriteLicense();
        w.WriteBlankLine();

        w.WriteLine($"#include \"py.{ns}.h\"");
        w.WriteBlankLine();

        w.WriteLine($"namespace py::cpp::{ns.ToCppNamespace()}");
        w.WriteLine("{");
        w.Indent++;
        var i = 0;

        foreach (var t in members.Classes.Concat(members.Interfaces))
        {
            if (i++ > 0)
            {
                w.WriteBlankLine();
            }

            w.WriteInspectableType(t, componentDlls);
        }

        foreach (var t in members.Structs.Where(s => !s.Type.IsCustomizedStruct()))
        {
            if (i++ > 0)
            {
                w.WriteBlankLine();
            }

            w.WriteStruct(t);
        }

        if (i++ > 0)
        {
            w.WriteBlankLine();
        }

        w.WriteNamespaceInitialization(ns);

        w.Indent--;
        w.WriteLine($"}} // py::cpp::{ns.ToCppNamespace()}");
        w.WriteBlankLine();

        w.WriteNamespaceModuleInitFunction(ns, members);

        sw.WriteFileIfChanged(nsPackageDir, $"py.{ns}.cpp");
    }
}
