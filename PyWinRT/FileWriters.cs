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
        QualifiedNamespace ns,
        NamespaceNullabilityInfo nullabilityInfo,
        IReadOnlyDictionary<string, string> packageMap,
        IEnumerable<TypeDefinition> typeDefinitions,
        bool componentDlls
    )
    {
        var nsPackageName = $"{ns.PyPackage}-{ns.Namespace}";
        var nsPackageDir = new DirectoryInfo(Path.Combine(outputPath.FullName, nsPackageName));
        var rootDir = new DirectoryInfo(Path.Combine(nsPackageDir.FullName, ns.PyPackageModule));
        var nsDir = rootDir;

        foreach (var nsSegment in ns.Namespace.Split('.'))
        {
            var segment = nsSegment.ToLowerInvariant().ToPythonIdentifier();
            nsDir = new DirectoryInfo(Path.Combine(nsDir.FullName, segment));
        }

        var headerDir = headerPath ?? new DirectoryInfo(Path.Combine(rootDir.FullName, "pywinrt"));

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

        WriteNamespaceCpp(nsPackageDir, ns, packageMap, members, componentDlls, 0);
        WriteNamespaceCpp(nsPackageDir, ns, packageMap, members, componentDlls, 1);
        WriteNamespaceH(headerDir, ns, packageMap, members, componentDlls);
        WriteNamespaceDunderInitPy(nsDir, ns, nullabilityMap, packageMap, members, componentDlls);
        WriteNamespacePyi(rootDir, ns, nullabilityMap, packageMap, members, 0);
        WriteNamespacePyi(rootDir, ns, nullabilityMap, packageMap, members, 1);
        WritePyWinRTVersionTxt(nsPackageDir);
        WriteRequirementsTxt(nsPackageDir, packageMap, members);

        if (members.GetReferencedNamespaces(packageMap, includeDelegates: true).Any())
        {
            WriteAllRequirementsTxt(nsPackageDir, packageMap, members);
        }
    }

    private static void WriteAllRequirementsTxt(
        DirectoryInfo nsPackageDir,
        IReadOnlyDictionary<string, string> packageMap,
        Members members
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.WriteLicense("#");
        w.WriteBlankLine();

        foreach (var ns in members.GetReferencedNamespaces(packageMap, includeDelegates: true))
        {
            w.WriteLine($"{ns.PyPackage}-{ns.Namespace}[all]=={PyWinRT.VersionString}");
        }

        sw.WriteFileIfChanged(nsPackageDir, "all-requirements.txt");
    }

    private static void WriteRequirementsTxt(
        DirectoryInfo nsPackageDir,
        IReadOnlyDictionary<string, string> packageMap,
        Members members
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.WriteLicense("#");
        w.WriteBlankLine();
        w.WriteLine($"winrt-runtime=={PyWinRT.VersionString}");

        foreach (var ns in members.GetRequiredNamespaces(packageMap))
        {
            w.WriteLine($"{ns.PyPackage}-{ns.Namespace}=={PyWinRT.VersionString}");
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
        QualifiedNamespace ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        IReadOnlyDictionary<string, string> packageMap,
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

        if (members.Interfaces.Count != 0)
        {
            w.WriteLine("from abc import abstractmethod");
        }

        w.WriteBlankLine();
        w.WriteLine("import winrt._winrt");
        w.WriteLine("import winrt.system");

        var referencedNamespaces = members.GetReferencedNamespaces(
            packageMap,
            includeInheritedInterfaces: true
        );

        foreach (var rns in referencedNamespaces)
        {
            w.WriteLine($"import {rns.PyModuleName} as {rns.PyModuleAlias}");
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
            w.WriteLine($"from {ns.PyPackage}.{ns.NsModuleName}{suffix} import (");
            w.Indent++;

            foreach (var type in dependencyTypes)
            {
                w.WriteLine($"{type.PyWrapperTypeName},");

                if (type.Category == Category.Interface)
                {
                    w.WriteLine($"{type.Name},");
                }
            }

            w.Indent--;
            w.WriteLine(")");
            w.WriteBlankLine();
        }

        if (members.Enums.Count != 0)
        {
            w.WriteLine(
                $"from {ns.PyModuleName} import {string.Join(", ", members.Enums.Select(e => e.Name))}"
            );
        }

        if (members.Delegates.Count != 0)
        {
            w.WriteLine(
                $"from {ns.PyModuleName} import {string.Join(", ", members.Delegates.Select(d => d.Name))}"
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
            w.WritePythonStructTyping(type, ns.Namespace, packageMap);
            didWriteClass = true;
        }

        foreach (
            var type in members
                .Classes.Concat(members.Interfaces)
                .Where(t => t.CircularDependencyDepth == dependencyDepth)
        )
        {
            w.WritePythonClassTyping(type, ns.Namespace, nullabilityMap, packageMap);
            didWriteClass = true;
        }

        // only write extra files if we wrote at least one class
        if (dependencyDepth != 0 && !didWriteClass)
        {
            return;
        }

        var moduleSuffix = dependencyDepth == 0 ? "" : $"_{dependencyDepth + 1}";
        sw.WriteFileIfChanged(nsWinrtDir, $"{ns.NsModuleName}{moduleSuffix}.pyi");
    }

    private static void WriteNamespaceDunderInitPy(
        DirectoryInfo nsDir,
        QualifiedNamespace ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        IReadOnlyDictionary<string, string> packageMap,
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

        var allExtensionTypes = members
            .Structs.Where(s => !s.Type.IsCustomizedStruct())
            .Concat(members.Classes)
            .Concat(members.Interfaces);

        if (componentDlls || allExtensionTypes.Any(t => t.IsPySequence || t.IsPyMapping))
        {
            w.WriteLine("import winrt.runtime._internals");
        }

        w.WriteLine("import winrt.system");

        for (int depth = 0; depth < 2; depth++)
        {
            var dependencyModuleTypes = allExtensionTypes.Where(t =>
                t.CircularDependencyDepth == depth
            );
            var suffix = depth == 0 ? "" : $"_{depth + 1}";

            if (dependencyModuleTypes.Any())
            {
                w.WriteLine($"from {ns.PyPackageModule}.{ns.NsModuleName}{suffix} import (");
                w.Indent++;

                foreach (var type in dependencyModuleTypes)
                {
                    w.WriteLine($"{type.PyWrapperTypeName},");

                    if (type.Category == Category.Interface)
                    {
                        w.WriteLine($"{type.Name},");
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
                w.WriteLine($"from {ns.PyPackageModule}.{ns.NsModuleName}{suffix} import (");
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

        var delegateReferencedNamespaces = new SortedSet<QualifiedNamespace>(
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
                    .Select(t => t.GetQualifiedNamespace(packageMap))
                    .Where(n => n != ns && n.Namespace != "System");
            })
        );

        if (delegateReferencedNamespaces.Count > 0)
        {
            w.WriteBlankLine();
            w.WriteLine("if typing.TYPE_CHECKING:");
            w.Indent++;

            foreach (var n in delegateReferencedNamespaces)
            {
                w.WriteLine($"import {n.PyModuleName} as {n.PyModuleAlias}");
            }

            w.Indent--;
        }

        if (componentDlls)
        {
            w.WriteBlankLine();
            w.WriteLine(
                "_dll_search_path_cookie_ = winrt.runtime._internals.register_dll_search_path(__file__)"
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
                w.WriteLine(
                    $"winrt.runtime._internals.mixin_mutable_mapping({type.PyWrapperTypeName})"
                );
            }
            else if (type.IsPyMapping)
            {
                w.WriteLine($"winrt.runtime._internals.mixin_mapping({type.PyWrapperTypeName})");
            }
            else if (type.IsPyMutableSequence)
            {
                w.WriteLine(
                    $"winrt.runtime._internals.mixin_mutable_sequence({type.PyWrapperTypeName})"
                );
            }
            else if (type.IsPySequence)
            {
                w.WriteLine($"winrt.runtime._internals.mixin_sequence({type.PyWrapperTypeName})");
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
                        ns.Namespace,
                        nullabilityInfo.Parameters[p.Index].Type,
                        packageMap,
                        quoteImportedTypes: true
                    )
                );

            // REVISIT: We will likely need to implement a ToPyCallbackInParamTyping()
            // instead of ToPyReturnTyping(). For now, this isn't a problem outside
            // of the TestComponent modules since most callbacks only return None or bool.
            w.WriteLine(
                $"{type.Name} = typing.Callable[[{string.Join(", ", paramTypes)}], {invoke.ToPyReturnTyping(ns.Namespace, nullabilityInfo, packageMap, quoteImportedTypes: true)}]"
            );
        }

        sw.WriteFileIfChanged(nsDir, "__init__.py");
    }

    private static void WriteNamespaceH(
        DirectoryInfo headerDir,
        QualifiedNamespace ns,
        IReadOnlyDictionary<string, string> packageMap,
        Members members,
        bool componentDlls
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        var referencedNamespaces = members.GetReferencedNamespaces(
            packageMap,
            includeDelegates: true
        );

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
            w.WriteLine($"#include <winrt/{rns.Namespace}.h>");
        }

        w.WriteBlankLine();
        w.WriteLine($"#include <winrt/{ns.Namespace}.h>");
        w.WriteBlankLine();

        w.WriteLine($"namespace py::proj::{ns.Namespace.ToCppNamespace()}");
        w.WriteBlock(() =>
        {
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
        });

        w.WriteBlankLine();
        w.WriteLine("namespace py");
        w.WriteBlock(() =>
        {
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
                w.WritePyTypeSpecializationStruct(type, ns);
            }
        });

        foreach (var rns in referencedNamespaces)
        {
            w.WriteBlankLine();
            w.WriteLine($"#if __has_include(\"py.{rns.Namespace}.h\")");
            w.WriteLine($"#include \"py.{rns.Namespace}.h\"");
            w.WriteLine("#endif");
        }

        w.WriteBlankLine();
        w.WriteLine($"namespace py::impl::{ns.Namespace.ToCppNamespace()}");
        w.WriteBlock(() =>
        {
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
        });

        w.WriteBlankLine();
        w.WriteLine($"namespace py::wrapper::{ns.Namespace.ToCppNamespace()}");
        w.WriteBlock(() =>
        {
            foreach (
                var type in members
                    .Classes.Concat(members.Interfaces)
                    .Concat(members.Structs.Where(s => !s.Type.IsCustomizedStruct()))
            )
            {
                w.WritePythonWrapperAlias(type);
            }
        });

        w.WriteBlankLine();
        w.WriteLine("namespace py");
        w.WriteBlock(() =>
        {
            foreach (var type in members.Interfaces.Where(i => i.IsGeneric))
            {
                w.WriteGenericInterfaceTypeMapper(type);
            }

            foreach (var type in members.Delegates)
            {
                w.WriteDelegateTypeMapper(type);
            }
        });

        sw.WriteFileIfChanged(headerDir, $"py.{ns.Namespace}.h");
    }

    private static void WriteNamespaceCpp(
        DirectoryInfo nsPackageDir,
        QualifiedNamespace ns,
        IReadOnlyDictionary<string, string> packageMap,
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

        w.WriteLine($"#include \"py.{ns.Namespace}.h\"");
        w.WriteBlankLine();

        w.WriteLine($"namespace py::cpp::{ns.Namespace.ToCppNamespace()}");
        w.WriteBlock(
            () =>
            {
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

                    w.WriteInspectableType(t, componentDlls, ns, moduleSuffix);

                    if (t.Category == Category.Interface)
                    {
                        w.WriteBlankLine();
                        w.WriteImplementsInterfaceImpl(t, ns, moduleSuffix);
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

                    w.WriteStruct(t, ns, moduleSuffix);
                    didWriteClass = true;
                }

                if (i++ > 0)
                {
                    w.WriteBlankLine();
                }

                w.WriteNamespaceInitialization(ns, moduleSuffix);
            },
            $" // py::cpp::{ns.Namespace.ToCppNamespace()}"
        );
        w.WriteBlankLine();

        w.WriteNamespaceModuleInitFunction(ns, packageMap, members, dependencyDepth, moduleSuffix);

        // only write extra modules if we wrote at least one class
        if (dependencyDepth != 0 && !didWriteClass)
        {
            return;
        }

        sw.WriteFileIfChanged(nsPackageDir, $"py.{ns.Namespace}{moduleSuffix}.cpp");
    }
}
