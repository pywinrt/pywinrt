using System.CodeDom.Compiler;
using System.Collections.ObjectModel;
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
        NamespaceNullabilityInfo nullabilityInfo,
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

        foreach (var type in members.Classes.Concat(members.Interfaces).Concat(members.Delegates))
        {
            nullabilityInfo.AddOrUpdateType(
                type.Type,
                () => new TypeNullabilityInfo(type.Type),
                old => new TypeNullabilityInfo(type.Type, old)
            );
        }

        var nullabilityMap = new Dictionary<string, MethodNullabilityInfo>(
            nullabilityInfo
                .Types.SelectMany(t => t.Methods)
                .Select(m => new KeyValuePair<string, MethodNullabilityInfo>(m.Signature, m))
        ).AsReadOnly();

        WriteNamespaceCpp(nsPackageDir, ns, members, componentDlls, 0);
        WriteNamespaceCpp(nsPackageDir, ns, members, componentDlls, 1);
        WriteNamespaceH(headerDir, ns, members, componentDlls);
        WriteNamespaceDunderInitPy(nsDir, ns, nullabilityMap, members, componentDlls);
        WriteNamespacePyi(nsWinrtDir, ns, nullabilityMap, members, 0);
        WriteNamespacePyi(nsWinrtDir, ns, nullabilityMap, members, 1);
        WritePyWinRTVersionTxt(nsPackageDir);
        WriteRequirementsTxt(nsPackageDir, members);

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

    private static void WriteRequirementsTxt(DirectoryInfo nsPackageDir, Members members)
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.WriteLicense("#");
        w.WriteBlankLine();
        w.WriteLine($"winrt-runtime=={PyWinRT.VersionString}");

        foreach (var ns in members.GetRequiredNamespaces())
        {
            w.WriteLine($"winrt-{ns}=={PyWinRT.VersionString}");
        }

        sw.WriteFileIfChanged(nsPackageDir, "requirements.txt");
    }

    private static void WritePyWinRTVersionTxt(DirectoryInfo nsPackageDir)
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.Write(PyWinRT.VersionString);

        sw.WriteFileIfChanged(nsPackageDir, "pywinrt-version.txt");
    }

    private static void WriteNamespacePyi(
        DirectoryInfo nsWinrtDir,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        Members members,
        int dependencyDepth
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };
        bool didWriteClass = false;

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

        // handle circular dependencies by importing sibling modules of the same namespace
        for (int depth = 0; depth < 2; depth++)
        {
            if (depth == dependencyDepth)
            {
                // don't import this module to itself
                continue;
            }

            var dependencyTypes = members
                .Structs.Where(s => !s.Type.IsCustomizedStruct())
                .Concat(members.Classes)
                .Concat(members.Interfaces)
                .Where(t => t.CircularDependencyDepth == depth);

            if (!dependencyTypes.Any())
            {
                // nothing to import
                continue;
            }

            var suffix = depth == 0 ? "" : $"_{depth + 1}";
            w.WriteLine($"from .{ns.ToNsModuleName()}{suffix} import (");
            w.Indent++;
            foreach (var type in dependencyTypes)
            {
                if (type.Category == Category.Interface)
                {
                    w.WriteLine($"Implements{type.Name},");
                }

                w.WriteLine($"{type.Name},");
            }
            w.Indent--;
            w.WriteLine(")");
            w.WriteBlankLine();
        }

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

        foreach (
            var type in members.Structs.Where(s =>
                !s.Type.IsCustomizedStruct() && s.CircularDependencyDepth == dependencyDepth
            )
        )
        {
            w.WritePythonStructTyping(type, ns);
            didWriteClass = true;
        }

        foreach (
            var type in members
                .Classes.Concat(members.Interfaces)
                .Where(t => t.CircularDependencyDepth == dependencyDepth)
        )
        {
            w.WritePythonClassTyping(type, ns, nullabilityMap);
            didWriteClass = true;
        }

        // only write the file if we wrote at least one class
        if (!didWriteClass)
        {
            return;
        }

        var moduleSuffix = dependencyDepth == 0 ? "" : $"_{dependencyDepth + 1}";
        sw.WriteFileIfChanged(nsWinrtDir, $"{ns.ToNsModuleName()}{moduleSuffix}.pyi");
    }

    private static void WriteNamespaceDunderInitPy(
        DirectoryInfo nsDir,
        string ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
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

        for (int depth = 0; depth < 2; depth++)
        {
            var dependencyModuleTypes = allExtensionTypes.Where(t =>
                t.CircularDependencyDepth == depth
            );
            var suffix = depth == 0 ? "" : $"_{depth + 1}";

            if (dependencyModuleTypes.Any())
            {
                w.WriteLine($"from winrt.{ns.ToNsModuleName()}{suffix} import (");
                w.Indent++;

                foreach (var type in dependencyModuleTypes)
                {
                    w.WriteLine($"{type.Name},");

                    if (type.Category == Category.Interface)
                    {
                        w.WriteLine($"Implements{type.Name},");
                    }
                }

                w.Indent--;
                w.WriteLine(")");
            }

            // REVISIT: Composable classes also have to inherit metaclass, so we need to
            // make these accessible. For now though, they are only available as type
            // hints and can't actually be imported at runtime. In the future, if we
            // allow subclassing in Python, we can change this.

            var composableTypes = dependencyModuleTypes.Where(t => t.IsComposable);
            if (composableTypes.Any())
            {
                w.WriteLine("from typing import TYPE_CHECKING");
                w.WriteLine("if TYPE_CHECKING:");
                w.Indent++;
                w.WriteLine($"from winrt.{ns.ToNsModuleName()}{suffix} import (");
                w.Indent++;

                foreach (var type in composableTypes)
                {
                    w.WriteLine($"{type.Name}_Static,");
                }

                w.Indent--;
                w.WriteLine(")");
                w.Indent--;
            }
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

            if (type.Category == Category.Interface)
            {
                w.WriteLine($"\"Implements{type.Name}\",");
            }
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
            var nullabilityInfo = nullabilityMap.GetValueOrDefault(
                invoke.ToString(),
                new MethodNullabilityInfo(invoke)
            );
            var paramTypes = invoke
                .Parameters.Where(p => p.IsPythonInParam())
                .Select(p =>
                    p.ToPyCallbackInParamTyping(
                        ns,
                        nullabilityInfo.Parameters[p.Index].Type,
                        quoteImportedTypes: true
                    )
                );

            // REVISIT: We will likely need to implement a ToPyCallbackInParamTyping()
            // instead of ToPyReturnTyping(). For now, this isn't a problem outside
            // of the TestComponent modules since most callbacks only return None or bool.
            w.WriteLine(
                $"{type.Name} = typing.Callable[[{string.Join(", ", paramTypes)}], {invoke.ToPyReturnTyping(ns, nullabilityInfo, quoteImportedTypes: true)}]"
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
        bool componentDlls,
        int dependencyDepth
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };
        bool didWriteClass = false;

        var moduleSuffix = dependencyDepth == 0 ? "" : $"_{dependencyDepth + 1}";

        w.WriteLicense();
        w.WriteBlankLine();

        w.WriteLine($"#include \"py.{ns}.h\"");
        w.WriteBlankLine();

        w.WriteLine($"namespace py::cpp::{ns.ToCppNamespace()}");
        w.WriteLine("{");
        w.Indent++;
        var i = 0;

        foreach (
            var t in members
                .Classes.Concat(members.Interfaces)
                .Where(t => t.CircularDependencyDepth == dependencyDepth)
        )
        {
            if (i++ > 0)
            {
                w.WriteBlankLine();
            }

            w.WriteInspectableType(t, componentDlls, moduleSuffix);

            if (t.Category == Category.Interface)
            {
                w.WriteBlankLine();
                w.WriteImplementsInterfaceImpl(t, moduleSuffix);
            }

            didWriteClass = true;
        }

        foreach (
            var t in members.Structs.Where(s =>
                !s.Type.IsCustomizedStruct() && s.CircularDependencyDepth == dependencyDepth
            )
        )
        {
            if (i++ > 0)
            {
                w.WriteBlankLine();
            }

            w.WriteStruct(t, moduleSuffix);
            didWriteClass = true;
        }

        if (i++ > 0)
        {
            w.WriteBlankLine();
        }

        w.WriteNamespaceInitialization(ns, moduleSuffix);

        w.Indent--;
        w.WriteLine($"}} // py::cpp::{ns.ToCppNamespace()}");
        w.WriteBlankLine();

        w.WriteNamespaceModuleInitFunction(ns, members, dependencyDepth, moduleSuffix);

        // only write the file if we wrote at least one class
        if (!didWriteClass)
        {
            return;
        }

        sw.WriteFileIfChanged(nsPackageDir, $"py.{ns}{moduleSuffix}.cpp");
    }
}
