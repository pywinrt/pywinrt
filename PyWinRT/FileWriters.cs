using System.CodeDom.Compiler;
using System.Collections.Concurrent;
using System.Collections.ObjectModel;
using Mono.Cecil;

static class FileWriters
{
    internal static void WriteNamespaceFiles(
        DirectoryInfo outputPath,
        QualifiedNamespace ns,
        Func<NamespaceNullabilityInfo> getNullabilityInfo,
        IReadOnlyDictionary<string, string> packageMap,
        IEnumerable<TypeDefinition> typeDefinitions,
        bool componentDlls,
        Census census
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

        // The generated files are independent of each other, so write them
        // in parallel. This helps the largest namespaces, which would
        // otherwise be the last ones still running at the end.
        Parallel.Invoke(
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
            () => WriteDepsJson(nsPackageDir, packageMap, members),
            () => TableWriter.Write(nsDir, ns, packageMap, members, census)
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
            // The ABI the generated code was written against, so that packaging
            // tooling can read the runtime requirement without compiling
            // anything. The same pair is asserted in the generated headers.
            w.Write("\"runtime_abi\": ");
            w.WriteBlock(
                () =>
                {
                    w.WriteLine($"\"major\": {PyWinRT.RequiredAbiMajor},");
                    w.WriteLine($"\"minor\": {PyWinRT.RequiredAbiMinor}");
                },
                ","
            );
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
            || (
                dependencyDepth == 0
                && members.Structs.Any(s => !s.Type.IsCustomizedStruct && s.IsPyInteger)
            )
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

        w.WriteLine("import winrt.runtime._internals");
        w.WriteLine("import winrt.system");

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

        // The classes, interfaces and structs of this namespace are built from
        // the table beside this file rather than imported from an extension
        // module, so this has to come before anything below that names one.
        w.WriteBlankLine();
        w.WriteLine("winrt.runtime._internals.load_projection(__spec__)");

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

        // HResult and EventRegistrationToken were projected as structs with
        // one field in pywinrt v3.x, and are the integer itself now, so the
        // name that field had still reads it.
        foreach (var type in allExtensionTypes.Where(t => t.IsPyInteger))
        {
            foreach (var field in type.Type.Fields)
            {
                w.WriteLine(
                    $"winrt.runtime._internals.alias_field({type.Name}, \"{field.Name.ToPythonIdentifier()}\")"
                );
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
}
