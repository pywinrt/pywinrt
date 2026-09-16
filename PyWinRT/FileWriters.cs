using System.CodeDom.Compiler;
using System.Collections.Concurrent;
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

        sw.WriteFileIfChanged(path, "version.h");
    }

    internal static void WriteNamespaceFiles(
        DirectoryInfo outputPath,
        DirectoryInfo? headerPath,
        QualifiedNamespace ns,
        Func<NamespaceNullabilityInfo> getNullabilityInfo,
        IReadOnlyDictionary<string, string> packageMap,
        IEnumerable<TypeDefinition> typeDefinitions,
        bool componentDlls,
        ConcurrentDictionary<
            string,
            ConcurrentDictionary<string, GenericInstanceType>
        > genericInstances
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

        // NB: this may block until the nullability info has been loaded, so
        // it is deferred until after the (independent) analysis of the types.
        var nullabilityInfo = getNullabilityInfo();

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

        // Warm the caches that are shared by the writers below so that they
        // are read-only while the writers run in parallel.
        members.GetReferencedNamespaces(packageMap, includeDelegates: true);
        members.GetReferencedNamespaces(packageMap, includeInheritedInterfaces: true);
        members.GetFullHeaderNamespaces(packageMap);

        var instances = genericInstances.GetOrAdd(headerDir.FullName, _ => new());

        foreach (var instance in members.GetGenericInstances())
        {
            instances.TryAdd(instance.ToCppTypeName(), instance);
        }

        // The generated files are independent of each other, so write them
        // in parallel. This helps the largest namespaces, which would
        // otherwise be the last ones still running at the end.
        Parallel.Invoke(
            () => WriteNamespaceCpp(nsPackageDir, ns, packageMap, members, componentDlls, 0),
            () => WriteNamespaceCpp(nsPackageDir, ns, packageMap, members, componentDlls, 1),
            () => WriteNamespaceH(headerDir, ns, packageMap, members, componentDlls),
            () => WriteNamespaceTypesH(headerDir, ns, members),
            () =>
                WriteNamespaceDunderInitPy(
                    nsDir,
                    ns,
                    nullabilityMap,
                    packageMap,
                    members,
                    componentDlls
                ),
            () => WriteNamespacePyi(rootDir, ns, nullabilityMap, packageMap, members, 0),
            () => WriteNamespacePyi(rootDir, ns, nullabilityMap, packageMap, members, 1),
            () => WriteDepsJson(nsPackageDir, packageMap, members)
        );
    }

    private static void WriteDepsJson(
        DirectoryInfo nsPackageDir,
        IReadOnlyDictionary<string, string> packageMap,
        Members members
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.WriteBlock(() =>
        {
            w.Write("\"pywinrt\": ");
            w.WriteBlock(() => w.WriteLine($"\"version\": \"{PyWinRT.VersionString}\""), ",");
            w.WriteLine("\"required\": [");
            w.Indent++;
            var requiredNamespaces = members.GetRequiredNamespaces(packageMap);
            foreach (
                var (ns, isLast) in requiredNamespaces.Select(
                    (n, i) => (n, i == requiredNamespaces.Count - 1)
                )
            )
            {
                w.WriteLine($"\"{ns.PyPackage}-{ns.Namespace}\"{(isLast ? "" : ",")}");
            }
            w.Indent--;
            w.WriteLine("],");
            w.WriteLine("\"referenced\": [");
            w.Indent++;
            var referencedNamespaces = members.GetReferencedNamespaces(
                packageMap,
                includeDelegates: true
            );
            foreach (
                var (ns, isLast) in referencedNamespaces.Select(
                    (n, i) => (n, i == referencedNamespaces.Count - 1)
                )
            )
            {
                w.WriteLine($"\"{ns.PyPackage}-{ns.Namespace}\"{(isLast ? "" : ",")}");
            }
            w.Indent--;
            w.WriteLine("]");
        });

        sw.WriteFileIfChanged(nsPackageDir, "deps.json");
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
        // The stdlib imports depend on what the type hints below turn out
        // to use, so the rest of the file is written first and the imports
        // are prepended once we can see it.
        using var bodySw = new StringWriter();
        using var w = new IndentedTextWriter(bodySw) { NewLine = "\n" };
        bool didWriteClass = false;

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
                .Structs.Where(s => !s.Type.IsCustomizedStruct)
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
                !s.Type.IsCustomizedStruct && s.CircularDependencyDepth == dependencyDepth
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

        var body = bodySw.ToString();

        using var sw = new StringWriter();
        using var hw = new IndentedTextWriter(sw) { NewLine = "\n" };

        hw.WriteLicense("#");
        hw.WriteBlankLine();

        var wroteImports = hw.WriteStdlibImports(body);

        if (members.Interfaces.Count != 0)
        {
            hw.WriteLine("from abc import abstractmethod");
            wroteImports = true;
        }

        if (
            members
                .Classes.Concat(members.Interfaces)
                .Where(t => t.CircularDependencyDepth == dependencyDepth)
                .Any(t => t.MethodGroups.Any(g => g.Aliases.Count != 0))
        )
        {
            hw.WriteLine("from typing_extensions import deprecated");
            wroteImports = true;
        }

        if (wroteImports)
        {
            hw.WriteBlankLine();
        }

        hw.Write(body);

        var moduleSuffix = dependencyDepth == 0 ? "" : $"_{dependencyDepth + 1}";
        sw.WriteFileIfChanged(nsWinrtDir, $"{ns.NsModuleName}{moduleSuffix}.pyi");
    }

    /// <summary>
    /// Writes the <c>import</c> lines for the standard library modules that
    /// <paramref name="body"/> actually refers to.
    /// </summary>
    /// <returns><c>true</c> if any import was written.</returns>
    private static bool WriteStdlibImports(this IndentedTextWriter w, string body)
    {
        var wroteAny = false;

        // collections.abc is aliased because WinRT has members named
        // "collections" that would shadow the module inside a class body.
        if (body.Contains("_cabc."))
        {
            wroteAny = true;
            w.WriteLine("import collections.abc as _cabc");
        }

        if (body.Contains("datetime."))
        {
            wroteAny = true;
            w.WriteLine("import datetime");
        }

        if (body.Contains("enum."))
        {
            wroteAny = true;
            w.WriteLine("import enum");
        }

        if (body.Contains("types."))
        {
            wroteAny = true;
            w.WriteLine("import types");
        }

        if (body.Contains("typing."))
        {
            wroteAny = true;
            w.WriteLine("import typing");
        }

        if (body.Contains("_uuid."))
        {
            wroteAny = true;
            w.WriteLine("import uuid as _uuid");
        }

        if (body.Contains("_property"))
        {
            wroteAny = true;
            w.WriteLine("from builtins import property as _property");
        }

        return wroteAny;
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
        // The stdlib imports depend on what the enums and delegate type
        // aliases below turn out to use, so the rest of the file is written
        // first and the imports are prepended once we can see it.
        using var bodySw = new StringWriter();
        using var w = new IndentedTextWriter(bodySw) { NewLine = "\n" };

        var allExtensionTypes = members
            .Structs.Where(s => !s.Type.IsCustomizedStruct)
            .Concat(members.Classes)
            .Concat(members.Interfaces);

        if (
            componentDlls
            || allExtensionTypes.Any(t =>
                t.IsPySequence || t.IsPyMapping || t.MethodGroups.Any(g => g.Aliases.Count != 0)
            )
        )
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
                $"class {type.Name}(enum.{(type.Type.HasFlagsAttribute ? "IntFlag" : "IntEnum")}):"
            );

            w.Indent++;

            foreach (var field in type.Type.Fields)
            {
                if (field.Constant is not null)
                {
                    var value = type.Type.HasFlagsAttribute
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

        // Methods that were renamed in pywinrt v3.0 are still callable by the
        // name they had then, with a deprecation warning.
        foreach (var type in allExtensionTypes)
        {
            foreach (var group in type.MethodGroups)
            {
                foreach (var alias in group.Aliases)
                {
                    var function = group.IsStatic ? "alias_static_method" : "alias_method";

                    w.WriteLine(
                        $"winrt.runtime._internals.{function}({type.PyWrapperTypeName}, \"{alias.PyName}\", \"{group.PyName}\")"
                    );
                }
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
                .Parameters.Where(p => p.IsPythonInParam)
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

        var body = bodySw.ToString();

        using var sw = new StringWriter();
        using var hw = new IndentedTextWriter(sw) { NewLine = "\n" };

        hw.WriteLicense("#");
        hw.WriteBlankLine();

        if (hw.WriteStdlibImports(body))
        {
            hw.WriteBlankLine();
        }

        hw.Write(body);

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
        var fullHeaderNamespaces = members.GetFullHeaderNamespaces(packageMap);

        w.WriteLicense();
        w.WriteBlankLine();
        w.WriteLine("#pragma once");
        w.WriteLine();
        w.WriteLine("#include <pywinrt/base.h>");
        w.WriteLine(
            $"static_assert(winrt::check_version(PYWINRT_VERSION, \"{PyWinRT.VersionString}\"), \"Mismatched Py/WinRT headers.\");"
        );

        // The GUIDs of the parameterized interfaces used by this package have
        // to be specialized before any full C++/WinRT header implicitly
        // instantiates them (pywinrt/base.h already includes the winrt-sdk one).
        w.WriteLine($"#if __has_include(\"py.{ns.PyPackageModule}.guids.h\")");
        w.WriteLine($"#include \"py.{ns.PyPackageModule}.guids.h\"");
        w.WriteLine("#endif");
        w.WriteBlankLine();

        // Only the Python type names of the referenced namespaces are needed
        // to convert their types, so include the light types header of each
        // one.
        w.WriteLine($"#include \"py.{ns.Namespace}.types.h\"");

        foreach (var rns in referencedNamespaces)
        {
            w.WriteLine($"#if __has_include(\"py.{rns.Namespace}.types.h\")");
            w.WriteLine($"#include \"py.{rns.Namespace}.types.h\"");
            w.WriteLine("#endif");
        }

        w.WriteBlankLine();

        // The C++/WinRT header of this namespace already includes the
        // declarations (impl/*.2.h) of every namespace it references, which is
        // all that is needed to wrap and unwrap objects of those types. The
        // full (and much larger) header of another namespace is only needed
        // for its delegates, generic types and the interfaces whose methods
        // are called.
        foreach (var rns in fullHeaderNamespaces)
        {
            w.WriteLine($"#include <winrt/{rns.Namespace}.h>");
        }

        w.WriteBlankLine();
        w.WriteLine($"#include <winrt/{ns.Namespace}.h>");

        // Likewise, the full generated header of another namespace is only
        // needed for its delegate and generic interface wrappers. Everything
        // else comes from the types headers.
        foreach (var rns in referencedNamespaces.Where(fullHeaderNamespaces.Contains))
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
                    .Concat(members.Structs.Where(s => !s.Type.IsCustomizedStruct))
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

    /// <summary>
    /// Writes the light <c>py.Namespace.types.h</c> header, which only maps
    /// the C++/WinRT types of a namespace to their Python type names.
    /// </summary>
    /// <remarks>
    /// This is what other namespaces include to convert types from this
    /// namespace. It depends only on the C++/WinRT type declarations
    /// (<c>impl/*.2.h</c>), not on the full C++/WinRT header, which keeps the
    /// include fan-out (and compile time) of every module that references
    /// this namespace small.
    /// </remarks>
    private static void WriteNamespaceTypesH(
        DirectoryInfo headerDir,
        QualifiedNamespace ns,
        Members members
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        w.WriteLicense();
        w.WriteBlankLine();
        w.WriteLine("#pragma once");
        w.WriteLine();
        w.WriteLine("#include <pywinrt/base.h>");
        w.WriteLine($"#include <winrt/impl/{ns.Namespace}.2.h>");
        w.WriteBlankLine();

        // The Python wrapper of a generic interface is its abstract "proj"
        // type, which the py_type specializations below refer to.
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

            foreach (var type in members.Structs.Where(s => !s.Type.IsCustomizedStruct))
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

        sw.WriteFileIfChanged(headerDir, $"py.{ns.Namespace}.types.h");
    }

    /// <summary>
    /// Writes <c>py.Package.guids.h</c>: explicit specializations of
    /// <c>winrt::impl::guid_v</c> for every parameterized interface and
    /// delegate instance used by the namespaces of a package, preceded by
    /// forward declarations of the types they mention.
    /// </summary>
    /// <remarks>
    /// C++/WinRT otherwise computes each of these GUIDs with a constexpr
    /// SHA-1 in every translation unit that uses the type, which is a large
    /// part of the frontend time of a generated module. Forward declarations
    /// instead of the C++/WinRT headers keep the header cheap enough to live
    /// in the precompiled header (pywinrt/base.h includes the winrt-sdk one), and
    /// putting it there also guarantees that the specializations come before
    /// any implicit instantiation. The specializations are guarded by a macro
    /// so that the headers of two packages can both be included. Compiling
    /// with <c>PYWINRT_VERIFY_GUIDS</c> defined checks every value against
    /// the C++/WinRT computation.
    /// </remarks>
    internal static void WritePInterfaceGuidsH(
        DirectoryInfo headerDir,
        string pyPackageModule,
        IEnumerable<GenericInstanceType> instances
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        var sorted = instances.OrderBy(i => i.ToCppTypeName(), StringComparer.Ordinal).ToList();

        // every non-generic type and generic definition mentioned, by C++ namespace
        var declarations = new SortedDictionary<string, SortedSet<string>>(StringComparer.Ordinal);
        var namespaces = new SortedSet<string>(StringComparer.Ordinal);

        void Declare(TypeReference type)
        {
            if (type is GenericInstanceType gen)
            {
                foreach (var arg in gen.GenericArguments)
                {
                    Declare(arg);
                }

                type = gen.ElementType;
            }

            if (
                type.Namespace == "System"
                || type.Namespace.StartsWith("System.")
                || type.FullName
                    is "Windows.Foundation.EventRegistrationToken"
                        or "Windows.Foundation.HResult"
            )
            {
                return;
            }

            var def = type.Resolve();

            if (def.IsCustomizedStruct || def.IsCustomNumeric)
            {
                // these are defined by C++/WinRT's <winrt/base.h>: DateTime and
                // TimeSpan directly, the Numerics types through the
                // <windowsnumerics.impl.h> that it includes
                return;
            }

            namespaces.Add(def.Namespace);

            var decl = def switch
            {
                { IsEnum: true } =>
                    $"enum class {def.Name} : {(def.Fields.Single(f => f.Name == "value__").FieldType.FullName == "System.UInt32" ? "uint32_t" : "int32_t")};",
                // NB: the WINRT_IMPL_EMPTY_BASES (__declspec(empty_bases))
                // that C++/WinRT puts on generic types has to be repeated
                // here: it only applies if it is on the first declaration,
                // and without it these types get a different object layout.
                { HasGenericParameters: true } =>
                    $"template <{string.Join(", ", def.GenericParameters.Select(p => $"typename {p.Name}"))}> struct WINRT_IMPL_EMPTY_BASES {def.Name.ToNonGeneric()};",
                _ => $"struct {def.Name};",
            };

            declarations.TryAdd(def.Namespace.ToCppNamespace(), new(StringComparer.Ordinal));
            declarations[def.Namespace.ToCppNamespace()].Add(decl);
        }

        foreach (var instance in sorted)
        {
            Declare(instance);
        }

        w.WriteLicense();
        w.WriteBlankLine();
        w.WriteLine("#pragma once");
        w.WriteLine();
        w.WriteLine("#include <winrt/base.h>");
        w.WriteBlankLine();

        w.WriteLine("#ifndef PYWINRT_GUID_EQUAL");
        w.WriteLine("#define PYWINRT_GUID_EQUAL");
        w.WriteLine("namespace py");
        w.WriteBlock(() =>
        {
            w.WriteLine("// winrt::operator== is not constexpr, so the checks below need this");
            w.WriteLine(
                "constexpr bool guid_equal(winrt::guid const& a, winrt::guid const& b) noexcept"
            );
            w.WriteBlock(() =>
            {
                w.WriteLine("if (a.Data1 != b.Data1 || a.Data2 != b.Data2 || a.Data3 != b.Data3)");
                w.WriteBlock(() => w.WriteLine("return false;"));
                w.WriteBlankLine();
                w.WriteLine("for (size_t i = 0; i < 8; i++)");
                w.WriteBlock(() =>
                {
                    w.WriteLine("if (a.Data4[i] != b.Data4[i])");
                    w.WriteBlock(() => w.WriteLine("return false;"));
                });
                w.WriteBlankLine();
                w.WriteLine("return true;");
            });
        });
        w.WriteLine("#endif");
        w.WriteBlankLine();

        // Verifying a GUID means computing it the C++/WinRT way, which needs
        // the full definition of every type in the signature, so that mode
        // pulls in the real headers instead of the forward declarations.
        w.WriteLine("#ifdef PYWINRT_VERIFY_GUIDS");
        foreach (var ns in namespaces)
        {
            w.WriteLine($"#include <winrt/{ns}.h>");
        }
        w.WriteLine("#else");
        foreach (var (cppNamespace, decls) in declarations)
        {
            w.WriteLine($"namespace winrt::{cppNamespace}");
            w.WriteBlock(() =>
            {
                foreach (var decl in decls)
                {
                    w.WriteLine(decl);
                }
            });
            w.WriteBlankLine();
        }
        w.WriteLine("#endif");
        w.WriteBlankLine();

        w.WriteLine("namespace winrt::impl");
        w.WriteBlock(() =>
        {
            foreach (var type in sorted)
            {
                var cppType = type.ToCppTypeName();
                var guid = WinRtGuid.GetGuid(type);

                w.WriteLine($"#ifndef PYWINRT_GUID_{guid:N}");
                w.WriteLine($"#define PYWINRT_GUID_{guid:N}");
                w.WriteLine(
                    $"template <> inline constexpr guid guid_v<{cppType}>{WinRtGuid.ToCppInitializer(guid)};"
                );
                w.WriteLine("#ifdef PYWINRT_VERIFY_GUIDS");
                w.WriteLine(
                    $"static_assert(py::guid_equal(guid_v<{cppType}>, pinterface_guid<{cppType}>::value));"
                );
                w.WriteLine("#endif");
                w.WriteLine("#endif");
            }
        });

        sw.WriteFileIfChanged(headerDir, $"py.{pyPackageModule}.guids.h");
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
                        !s.Type.IsCustomizedStruct && s.CircularDependencyDepth == dependencyDepth
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
