using System.CodeDom.Compiler;
using System.Collections.Concurrent;
using System.Collections.ObjectModel;
using Mono.Cecil;

static class FileWriters
{
    internal static NamespaceDependencies? WriteNamespaceFiles(
        DirectoryInfo outputPath,
        QualifiedNamespace ns,
        string distribution,
        Func<NamespaceNullabilityInfo> getNullabilityInfo,
        IReadOnlyDictionary<string, string> packageMap,
        IEnumerable<TypeDefinition> typeDefinitions,
        bool componentDlls,
        string? dllPackage,
        bool legacyMethodAliases,
        Census census
    )
    {
        var nsPackageName = $"{ns.PyPackage}-{distribution}";
        var nsPackageDir = new DirectoryInfo(Path.Combine(outputPath.FullName, nsPackageName));
        var rootDir = new DirectoryInfo(Path.Combine(nsPackageDir.FullName, ns.PyPackageModule));
        var nsDir = rootDir;

        foreach (var nsSegment in ns.Namespace.Split('.'))
        {
            var segment = nsSegment.ToLowerInvariant().ToPythonIdentifier();
            nsDir = new DirectoryInfo(Path.Combine(nsDir.FullName, segment));
        }

        var members = new Members(typeDefinitions, legacyMethodAliases);

        if (
            !members
                .Structs.Concat(members.Enums)
                .Concat(members.Classes)
                .Concat(members.Interfaces)
                .Concat(members.Delegates)
                .Any()
        )
        {
            return null;
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
                    componentDlls,
                    dllPackage
                ),
            () => WriteNamespacePyi(nsDir, ns, nullabilityMap, packageMap, members),
            () => TableWriter.Write(nsDir, ns, packageMap, members, census)
        );

        // Several namespaces may be published in one distribution, so what
        // each of them depends on is handed back and written once the whole
        // run is done rather than into a file they would race for.
        return new NamespaceDependencies(
            ns.Namespace,
            members.GetRequiredNamespaces(packageMap),
            members.GetReferencedNamespaces(packageMap, includeDelegates: true)
        );
    }

    /// <summary>
    /// Writes the deps.json of one distribution.
    /// </summary>
    /// <remarks>
    /// It names the namespaces the distribution provides and the other
    /// distributions its types hand back, rather than namespaces, because a
    /// distribution is what the packaging script turns into a requirement.
    /// What the distribution provides itself is left out of both lists.
    /// </remarks>
    internal static void WriteDepsJson(
        DirectoryInfo packageDir,
        IReadOnlyCollection<string> namespaces,
        IReadOnlyCollection<string> required,
        IReadOnlyCollection<string> referenced
    )
    {
        using var sw = new StringWriter();
        using var w = new IndentedTextWriter(sw) { NewLine = "\n" };

        void writeList(string name, IReadOnlyCollection<string> items, string end)
        {
            w.WriteLine($"\"{name}\": [");
            w.Indent++;

            foreach (var (item, isLast) in items.Select((n, i) => (n, i == items.Count - 1)))
            {
                w.WriteLine($"\"{item}\"{(isLast ? "" : ",")}");
            }

            w.Indent--;
            w.WriteLine($"]{end}");
        }

        w.WriteBlock(() =>
        {
            // The table format the tables are written to, so that packaging
            // tooling can read the runtime requirement without reading one.
            // The same pair heads every table itself.
            w.Write("\"table_format\": ");
            w.WriteBlock(
                () =>
                {
                    w.WriteLine($"\"major\": {TableWriter.FormatMajor},");
                    w.WriteLine($"\"minor\": {TableWriter.FormatMinor}");
                },
                ","
            );
            writeList("namespaces", namespaces, ",");
            writeList("required", required, ",");
            writeList("referenced", referenced, "");
        });

        sw.WriteFileIfChanged(packageDir, "deps.json");
    }

    /// <summary>
    /// Writes the <c>__all__</c> of a namespace, which is every name it binds.
    /// </summary>
    private static void WriteAll(
        this IndentedTextWriter w,
        Members members,
        IEnumerable<ProjectedType> allExtensionTypes
    )
    {
        w.WriteLine("__all__ = [");
        w.Indent++;

        foreach (var type in members.Enums.Concat(allExtensionTypes).Concat(members.Delegates))
        {
            w.WriteLine($"\"{type.Name}\",");
        }

        w.Indent--;
        w.WriteLine("]");
    }

    /// <summary>
    /// Writes the enums of a namespace as Python enum classes, for the stub.
    /// </summary>
    /// <remarks>
    /// The constants are in the table and the runtime builds the class, so
    /// this is not what defines an enum. What it is for is
    /// <c>__init__.pyi</c>, which a type checker reads instead of the module
    /// and so has to spell out everything the module binds.
    /// </remarks>
    private static void WriteEnums(this IndentedTextWriter w, Members members)
    {
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
    }

    /// <summary>
    /// Writes the delegates of a namespace as <c>typing.Callable</c> aliases.
    /// </summary>
    /// <param name="forStub">
    /// Whether this is <c>__init__.pyi</c> rather than <c>__init__.py</c>. A
    /// stub is read and not executed, so it says <c>typing.TypeAlias</c> and
    /// names a type from another namespace directly. The aliases in
    /// <c>__init__.py</c> are assignments evaluated when the module is
    /// imported, and it only imports those namespaces while type checking -
    /// not every package a namespace refers to is necessarily installed - so
    /// there the names are quoted.
    /// </param>
    /// <remarks>
    /// A delegate is a Python callable, so what the projection binds is the
    /// signature it has to have.
    /// </remarks>
    private static void WriteDelegateAliases(
        this IndentedTextWriter w,
        QualifiedNamespace ns,
        Members members,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        IReadOnlyDictionary<string, string> packageMap,
        bool forStub
    )
    {
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
                        quoteImportedTypes: !forStub
                    )
                );

            // REVISIT: We will likely need to implement a ToPyCallbackInParamTyping()
            // instead of ToPyReturnTyping(). For now, this isn't a problem outside
            // of the TestComponent modules since most callbacks only return None or bool.
            var alias = forStub ? ": typing.TypeAlias" : "";

            w.WriteLine(
                $"{type.Name}{alias} = typing.Callable[[{string.Join(", ", paramTypes)}], {invoke.ToPyReturnTyping(ns.Namespace, nullabilityInfo, packageMap, quoteImportedTypes: !forStub)}]"
            );
        }
    }

    /// <summary>
    /// The namespaces other than <paramref name="ns"/> that the delegates of
    /// <paramref name="members"/> name in their signatures.
    /// </summary>
    private static SortedSet<QualifiedNamespace> GetDelegateReferencedNamespaces(
        this Members members,
        IReadOnlyDictionary<string, string> packageMap,
        QualifiedNamespace ns
    )
    {
        return new SortedSet<QualifiedNamespace>(
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
    }

    /// <summary>
    /// Writes the type stub of a namespace, which sits beside its
    /// <c>__init__.py</c> as <c>__init__.pyi</c>.
    /// </summary>
    /// <remarks>
    /// This is the inline-stub layout that PEP 561 describes and that type
    /// checkers prefer over the <c>.py</c>. It is also the only layout a
    /// projection package can have: the module used to re-export an extension
    /// module's contents, and the stub went beside that extension module, but
    /// the types come from the table now and there is no second module to
    /// describe. So a stub has to say everything the module has - the enums
    /// the runtime builds and the delegate aliases <c>__init__.py</c> defines
    /// included - because a type checker reads the stub instead of the module
    /// and not as well as it.
    /// </remarks>
    private static void WriteNamespacePyi(
        DirectoryInfo nsDir,
        QualifiedNamespace ns,
        ReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap,
        IReadOnlyDictionary<string, string> packageMap,
        Members members
    )
    {
        // The stdlib imports depend on what the type hints below turn out
        // to use, so the rest of the file is written first and the imports
        // are prepended once we can see it.
        using var bodySw = new StringWriter();
        using var w = new IndentedTextWriter(bodySw) { NewLine = "\n" };

        var allExtensionTypes = members
            .Structs.Where(s => !s.Type.IsCustomizedStruct)
            .Concat(members.Classes)
            .Concat(members.Interfaces);

        w.WriteLine("import winrt._winrt");
        w.WriteLine("import winrt.system");

        var referencedNamespaces = new SortedSet<QualifiedNamespace>(
            members.GetReferencedNamespaces(packageMap, includeInheritedInterfaces: true)
        );
        referencedNamespaces.UnionWith(members.GetDelegateReferencedNamespaces(packageMap, ns));

        foreach (var rns in referencedNamespaces)
        {
            w.WriteLine($"import {rns.PyModuleName} as {rns.PyModuleAlias}");
        }

        w.WriteBlankLine();
        w.WriteAll(members, allExtensionTypes);

        foreach (
            var type in members
                .Interfaces.SelectMany(i => i.Type.GenericParameters.Select(p => p.Name))
                .Concat(
                    members.Delegates.SelectMany(d => d.Type.GenericParameters.Select(p => p.Name))
                )
                .Distinct()
                .Order()
        )
        {
            w.WriteLine($"{type} = typing.TypeVar('{type}')");
        }

        w.WriteEnums(members);
        w.WriteBlankLine();

        foreach (var type in members.Structs.Where(s => !s.Type.IsCustomizedStruct))
        {
            w.WritePythonStructTyping(type, ns.Namespace, packageMap);
        }

        foreach (var type in members.Classes.Concat(members.Interfaces))
        {
            w.WritePythonClassTyping(type, ns.Namespace, nullabilityMap, packageMap);
        }

        w.WriteDelegateAliases(ns, members, nullabilityMap, packageMap, forStub: true);

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
                .Any(t => t.MethodGroups.Any(g => g.Aliases.Count != 0))
            || members.Structs.Any(s => !s.Type.IsCustomizedStruct && s.IsPyInteger)
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

        sw.WriteFileIfChanged(nsDir, "__init__.pyi");
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
        bool componentDlls,
        string? dllPackage
    )
    {
        // The stdlib imports depend on what the delegate type aliases below
        // turn out to use, so the rest of the file is written first and the
        // imports are prepended once we can see it.
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

        var delegateReferencedNamespaces = members.GetDelegateReferencedNamespaces(packageMap, ns);

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

        // The component .dlls are redistributed in a distribution of their
        // own, which puts them on the DLL search path as it is imported. That
        // has to happen before the first type here is activated, and this is
        // the only module the user is expected to import.
        if (!string.IsNullOrEmpty(dllPackage))
        {
            w.WriteBlankLine();
            w.WriteLine($"import {dllPackage}");
        }

        w.WriteBlankLine();
        w.WriteAll(members, allExtensionTypes);

        foreach (
            var type in members
                .Delegates.SelectMany(d => d.Type.GenericParameters.Select(p => p.Name))
                .Distinct()
                .Order()
        )
        {
            w.WriteLine($"{type} = typing.TypeVar('{type}')");
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

        w.WriteDelegateAliases(ns, members, nullabilityMap, packageMap, forStub: false);

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
