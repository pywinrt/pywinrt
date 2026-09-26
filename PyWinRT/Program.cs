using System.Collections.Concurrent;
using System.CommandLine;
using System.CommandLine.Builder;
using System.CommandLine.Help;
using System.CommandLine.Invocation;
using System.CommandLine.Parsing;
using System.Diagnostics;
using Mono.Cecil;

var inputOption = new Option<InputSpec[]>(
    "--input",
    CommandReader.ParseSpec,
    default,
    "Windows metadata to include in projection"
)
{
    Arity = ArgumentArity.ZeroOrMore,
    ArgumentHelpName = "spec",
};

var referenceOption = new Option<InputSpec[]>(
    "--reference",
    CommandReader.ParseSpec,
    default,
    "Windows metadata to reference from projection"
)
{
    Arity = ArgumentArity.ZeroOrMore,
    ArgumentHelpName = "spec",
};

var outputOption = new Option<DirectoryInfo>(
    "--output",
    () => new DirectoryInfo(Directory.GetCurrentDirectory()),
    "Location of generated projection"
)
{
    Arity = ArgumentArity.ZeroOrOne,
    ArgumentHelpName = "path",
};

var includeOption = new Option<string[]>(
    "--include",
    "One or more prefixes to include in projection"
)
{
    Arity = ArgumentArity.ZeroOrMore,
    ArgumentHelpName = "prefix",
};

var excludeOption = new Option<string[]>(
    "--exclude",
    "One or more prefixes to exclude from projection"
)
{
    Arity = ArgumentArity.ZeroOrMore,
    ArgumentHelpName = "prefix",
};

var nullabilityJsonPathOption = new Option<FileInfo?>(
    "--nullability-json",
    "Nullability information JSON file"
)
{
    Arity = ArgumentArity.ZeroOrOne,
    ArgumentHelpName = "path",
};

var shapesOption = new Option<FileInfo?>(
    "--shapes",
    "The ABI call shape census (shapes.json) that the tables name their members' call "
        + "shapes out of. Defaults to the copy that ships beside this tool, which is the "
        + "census the matching winrt-runtime was built from."
)
{
    Arity = ArgumentArity.ZeroOrOne,
    ArgumentHelpName = "path",
};

var emitShapesOption = new Option<DirectoryInfo?>(
    "--emit-shapes",
    "Directory holding the ABI call shape census (shapes.json) and the trampoline "
        + "instantiations (shapes-generated.h) that this run merges into, which is how a "
        + "census gains a shape. A projection generated this way needs a winrt-runtime "
        + "built from the same directory. With --verbose, the merged census is reported."
)
{
    Arity = ArgumentArity.ZeroOrOne,
    ArgumentHelpName = "path",
};

var componentDllsOption = new Option<bool>(
    "--component-dlls",
    "Set this flag when generating projection for user components that will ship with the required .dlls in the Python package"
);

var dllPackageOption = new Option<string>(
    "--dll-package",
    "The Python module of the distribution that redistributes the component .dlls, when they ship in a package of their own rather than in this one"
)
{
    ArgumentHelpName = "module",
};

var legacyMethodAliasesOption = new Option<bool>(
    "--legacy-method-aliases",
    "Give each method that pywinrt v3.x projected under another name a deprecated alias with "
        + "that name. Only a projection that pywinrt v3.x published has code that calls them."
);

var verboseOption = new Option<bool>("--verbose", "Show detailed progress information");

var rootCommand = new RootCommand("Generate Python projection for Windows Runtime types");
rootCommand.AddOption(inputOption);
rootCommand.AddOption(referenceOption);
rootCommand.AddOption(outputOption);
rootCommand.AddOption(includeOption);
rootCommand.AddOption(excludeOption);
rootCommand.AddOption(nullabilityJsonPathOption);
rootCommand.AddOption(shapesOption);
rootCommand.AddOption(emitShapesOption);
rootCommand.AddOption(componentDllsOption);
rootCommand.AddOption(dllPackageOption);
rootCommand.AddOption(legacyMethodAliasesOption);
rootCommand.AddOption(verboseOption);

rootCommand.SetHandler(
    async (InvocationContext invocationContext) =>
    {
        var resolver = new MetadataResolver();
        var types = new List<TypeDefinition>();
        var packageMap = new Dictionary<string, string>();

        // Which distribution the namespaces of each metadata file are
        // published in, when the input says so. Keyed like packageMap, by the
        // name of the module the types come from.
        var distributionMap = new Dictionary<string, string>();

        var input = invocationContext.ParseResult.GetValueForOption(inputOption)!;
        var reference = invocationContext.ParseResult.GetValueForOption(referenceOption)!;
        var output = invocationContext.ParseResult.GetValueForOption(outputOption)!;
        var include = invocationContext.ParseResult.GetValueForOption(includeOption)!;
        var exclude = invocationContext.ParseResult.GetValueForOption(excludeOption)!;
        var nullabilityInfoPath = invocationContext.ParseResult.GetValueForOption(
            nullabilityJsonPathOption
        );
        var shapes = invocationContext.ParseResult.GetValueForOption(shapesOption);
        var emitShapes = invocationContext.ParseResult.GetValueForOption(emitShapesOption);
        var componentDlls = invocationContext.ParseResult.GetValueForOption(componentDllsOption);
        var dllPackage = invocationContext.ParseResult.GetValueForOption(dllPackageOption);
        var legacyMethodAliases = invocationContext.ParseResult.GetValueForOption(
            legacyMethodAliasesOption
        );
        var verbose = invocationContext.ParseResult.GetValueForOption(verboseOption);

        var inputPackage = default(string);
        var stopwatch = Stopwatch.StartNew();

        // The nullability info file can be large, so start loading it right
        // away in parallel with everything else. It isn't needed until the
        // generated files are written.
        var nullabilityLoadTime = TimeSpan.Zero;
        var nullabilityFileTask = Task.Run(() =>
        {
            var nullabilityStopwatch = Stopwatch.StartNew();
            var nullabilityFile = NullabilityInfoFile.Load(nullabilityInfoPath);
            nullabilityLoadTime = nullabilityStopwatch.Elapsed;
            return nullabilityFile;
        });

        // Reading the metadata files is independent of each other, so it can
        // be done in parallel, but registration must be done in order.
        var inputAssemblies = input
            .AsParallel()
            .AsOrdered()
            .Select(spec =>
                AssemblyDefinition.ReadAssembly(
                    spec.File,
                    new ReaderParameters { MetadataResolver = resolver }
                )
            )
            .ToList();

        var referenceAssemblies = reference
            .AsParallel()
            .AsOrdered()
            .Select(spec =>
                AssemblyDefinition.ReadAssembly(
                    spec.File,
                    new ReaderParameters { MetadataResolver = resolver }
                )
            )
            .ToList();

        foreach (var (spec, assembly) in input.Zip(inputAssemblies))
        {
            if (inputPackage is null)
            {
                inputPackage = spec.Package;
            }
            else if (inputPackage != spec.Package)
            {
                throw new Exception("All input packages must be the same python package");
            }

            resolver.Register(assembly);

            var module = assembly.Modules.Single().Name;
            packageMap.Add(module, spec.Package);

            if (spec.Distribution is not null)
            {
                distributionMap.Add(module, spec.Distribution);
            }
        }

        if (inputPackage is null)
        {
            throw new Exception("At least one input package is required");
        }

        foreach (var (spec, assembly) in reference.Zip(referenceAssemblies))
        {
            resolver.Register(assembly);

            var module = assembly.Modules.Single().Name;

            // A reference names types that someone else projects, so one
            // metadata file is an input or a reference and never both. The
            // package it belongs to says nothing about that, and is often
            // this run's own: every family in the PyWinRT tree but WinUI 2
            // publishes into winrt.
            if (packageMap.ContainsKey(module))
            {
                throw new Exception(
                    $"{module} is an input of this run, so it cannot be a reference"
                        + $" as well ({spec.File})"
                );
            }

            packageMap.Add(module, spec.Package);
        }

        var loadTime = stopwatch.Elapsed;
        stopwatch.Restart();

        if (shapes is not null && emitShapes is not null)
        {
            throw new Exception(
                "--shapes and --emit-shapes name different censuses: --shapes reads one, "
                    + "and --emit-shapes reads and extends the one in its directory"
            );
        }

        Census census;

        if (emitShapes is null)
        {
            // A table names a member's call shape by an id, and an id means
            // what the census that issued it says it means. So a run that does
            // not also build the trampolines reads a census someone else wrote
            // and never assigns an id of its own: the default is the copy that
            // ships beside this tool, which is the one the matching
            // winrt-runtime was built from.
            var file =
                shapes ?? new FileInfo(Path.Combine(AppContext.BaseDirectory, "shapes.json"));

            if (!file.Exists)
            {
                throw new Exception(
                    $"there is no ABI call shape census at {file.FullName}: the tables name "
                        + "the call shapes of their members by id, and the ids are assigned by "
                        + "the census that the winrt-runtime being generated against was built "
                        + "from. It ships beside the tool in the PyWinRT NuGet package, and in "
                        + "the PyWinRT source tree at runtime/src/shapes.json; name one with "
                        + "--shapes, or build a winrt-runtime of your own with --emit-shapes"
                );
            }

            census = Census.Load(file);

            // The census is read rather than extended, so a component that
            // makes a call it does not cover is told which shape that is here,
            // rather than when the member is first used. Taking the census
            // costs one metadata walk and is what makes the report complete.
            var needed = new ShapeCensus().Take(
                inputAssemblies.SelectMany(a => a.MainModule.Types).Where(t => t.IsWindowsRuntime)
            );

            if (census.ReportMissing(needed, Console.Error) > 0)
            {
                Console.Error.WriteLine(
                    "warning: those members are still projected, but using one raises. Add a "
                        + "member of each shape above to https://github.com/pywinrt/testwinrt "
                        + "and the next winrt-runtime release will cover it."
                );
            }
        }
        else
        {
            // The trampolines that call WinRT are compiled into winrt-runtime
            // once for the whole tree, so the census reads every type of every
            // input, including the interfaces that are exclusive to a runtime
            // class and are therefore never projected as a Python type but are
            // still what a call goes through. No run can see all of the
            // metadata at once - WinUI 2 and the Windows App SDK both define
            // Microsoft.UI.Xaml.Controls - so each one merges what it found
            // into the census file, which also holds the shape ids and is why
            // they are stable.
            var fragment = new ShapeCensus().Take(
                inputAssemblies.SelectMany(a => a.MainModule.Types).Where(t => t.IsWindowsRuntime)
            );

            census = Census.LoadOrCreate(
                new FileInfo(Path.Combine(emitShapes.FullName, "shapes.json"))
            );

            // The family a fragment belongs to is the directory this run
            // writes into - see Census.Families for why it is not the package.
            census.Merge(output.Name, fragment);
            census.Save(emitShapes, "shapes.json");
            census.WriteShapesHeader(emitShapes, "shapes-generated.h");
        }

        if (verbose)
        {
            census.WriteReport(Console.Out);
            Console.WriteLine();
        }

        var censusTime = stopwatch.Elapsed;
        stopwatch.Restart();

        var tasks = new List<Task<NamespaceDependencies?>>();

        foreach (var assembly in inputAssemblies)
        {
            types.AddRange(
                assembly
                    .MainModule.Types.Where(t => t.IsWindowsRuntime)
                    .Where(t => !t.IsExclusiveTo)
                    .Where(t => Filter.Includes(t.FullName, include, exclude))
            );
        }

        var filterTime = stopwatch.Elapsed;
        stopwatch.Restart();

        if (verbose)
        {
            Console.WriteLine($"Input:");
            foreach (var item in input)
            {
                Console.WriteLine($"    {item}");
            }

            Console.WriteLine($"Reference:");
            foreach (var item in reference)
            {
                Console.WriteLine($"    {item}");
            }

            Console.WriteLine($"Output: {output.FullName}");
            Console.WriteLine($"Include: {string.Join(";", include)}");
            Console.WriteLine($"Exclude: {string.Join(";", exclude)}");
            Console.WriteLine($"Loaded metadata in {loadTime.TotalMilliseconds:F0} ms");
            Console.WriteLine($"Took the shape census in {censusTime.TotalMilliseconds:F0} ms");
            Console.WriteLine($"Filtered types in {filterTime.TotalMilliseconds:F0} ms");
        }

        // Which distribution each namespace is published in. A namespace whose
        // types all come from metadata of one distribution belongs to it; one
        // that several contribute to is published on its own instead, and each
        // of them depends on it. That rule is what keeps a namespace from
        // being owned by two distributions at once, and it needs no list to
        // maintain: the metadata says which case a namespace is in.
        var distributions = new Dictionary<QualifiedNamespace, string>();

        // A namespace this run only references is published by whoever projects
        // it, and where that is not one distribution per namespace there is
        // nothing in the metadata to say so, so the reference has to. Without
        // it the fall-back below is the Windows SDK's layout, which would name
        // a distribution that does not exist.
        foreach (var (spec, assembly) in reference.Zip(referenceAssemblies))
        {
            if (spec.Distribution is null)
            {
                continue;
            }

            foreach (var type in assembly.MainModule.Types.Where(t => t.IsWindowsRuntime))
            {
                distributions[new QualifiedNamespace(spec.Package, type.Namespace)] =
                    spec.Distribution;
            }
        }

        foreach (
            var group in types.GroupBy(t => t.Namespace).OrderBy(g => g.Key, StringComparer.Ordinal)
        )
        {
            var ns = new QualifiedNamespace(inputPackage, group.Key);
            var owners = group
                .Select(t => distributionMap.GetValueOrDefault(t.Module.Name))
                .Distinct(StringComparer.Ordinal)
                .ToList();

            if (owners is [string only])
            {
                distributions[ns] = only;
                continue;
            }

            // Either the input says nothing about distributions, in which case
            // every namespace is published on its own, or it does and this one
            // is contributed by more than one. Saying so matters: it is a
            // layout the packaging has to cope with rather than notice.
            distributions[ns] = group.Key;

            if (distributionMap.Count > 0)
            {
                Console.WriteLine(
                    $"note: {group.Key} is contributed by "
                        + string.Join(", ", owners.Select(o => o ?? "no distribution"))
                        + ", so it is published on its own"
                );
            }
        }

        var namespaceTimes = new ConcurrentBag<(string Namespace, TimeSpan Elapsed)>();

        // Generation is pipelined: the metadata for each namespace is
        // preloaded on this thread (see ModulePreloader for why) and then the
        // namespace is handed off to the thread pool to be generated while
        // the next namespace is being preloaded. The largest namespaces are
        // started first so that they are not left running alone at the end
        // after all of the smaller ones are done.
        var preloader = new ModulePreloader();

        // The preloading on this thread gates the whole pipeline, so give it
        // priority over the thread pool threads that generate the code.
        var priority = Thread.CurrentThread.Priority;
        Thread.CurrentThread.Priority = ThreadPriority.AboveNormal;

        foreach (
            var group in types
                .GroupBy(t => t.Namespace)
                .Select(g => g.ToList())
                .OrderByDescending(g =>
                    g.Sum(t => t.GetCategory() is Category.Class or Category.Interface ? 4 : 1)
                )
        )
        {
            var groupNamespace = group[0].Namespace;

            preloader.Preload(group);

            tasks.Add(
                Task.Run(() =>
                {
                    var nsStopwatch = Stopwatch.StartNew();

                    var dependencies = FileWriters.WriteNamespaceFiles(
                        output,
                        new QualifiedNamespace(inputPackage, groupNamespace),
                        distributions[new QualifiedNamespace(inputPackage, groupNamespace)],
                        () => nullabilityFileTask.Result.GetOrAdd(groupNamespace),
                        packageMap,
                        group,
                        componentDlls,
                        dllPackage,
                        legacyMethodAliases,
                        census
                    );

                    namespaceTimes.Add((groupNamespace, nsStopwatch.Elapsed));

                    return dependencies;
                })
            );
        }

        var preloadTime = stopwatch.Elapsed;
        Thread.CurrentThread.Priority = priority;

        await Task.WhenAll(tasks);

        // One deps.json per distribution, now that every namespace in it has
        // been generated. What a package says about another package is said
        // in distributions, so the namespaces this one provides drop out of
        // both lists rather than becoming a requirement on itself.
        var written = new HashSet<string>(StringComparer.Ordinal);

        string distributionOf(QualifiedNamespace ns) =>
            $"{ns.PyPackage}-{distributions.GetValueOrDefault(ns, ns.Namespace)}";

        foreach (
            var distribution in tasks
                .Select(t => t.Result)
                .OfType<NamespaceDependencies>()
                .GroupBy(d => distributions[new QualifiedNamespace(inputPackage, d.Namespace)])
        )
        {
            var name = $"{inputPackage}-{distribution.Key}";
            var provided = distribution.Select(d => d.Namespace).ToHashSet(StringComparer.Ordinal);

            SortedSet<string> names(Func<NamespaceDependencies, IEnumerable<QualifiedNamespace>> of)
            {
                var result = new SortedSet<string>(StringComparer.Ordinal);

                foreach (var ns in distribution.SelectMany(of))
                {
                    if (ns.PyPackage == inputPackage && provided.Contains(ns.Namespace))
                    {
                        continue;
                    }

                    result.Add(distributionOf(ns));
                }

                return result;
            }

            FileWriters.WriteDepsJson(
                new DirectoryInfo(Path.Combine(output.FullName, name)),
                [.. provided.Order(StringComparer.Ordinal)],
                names(d => d.Required),
                names(d => d.Referenced)
            );

            written.Add(name);
        }

        // A namespace that upstream withdrew, or one that moved into another
        // distribution, leaves behind a directory that nothing writes any
        // more, and it is published from this tree until someone notices.
        // A deps.json is what marks a directory as one of ours, and a run that
        // was told to generate part of the projection is in no position to say
        // what the rest of it should be.
        if (include.Length == 0 && exclude.Length == 0 && output.Exists)
        {
            foreach (var dir in output.EnumerateDirectories($"{inputPackage}-*"))
            {
                if (written.Contains(dir.Name))
                {
                    continue;
                }

                if (!File.Exists(Path.Combine(dir.FullName, "deps.json")))
                {
                    continue;
                }

                Console.WriteLine($"note: removing {dir.Name}, which nothing generates now");
                dir.Delete(recursive: true);
            }
        }

        var nullabilityFile = await nullabilityFileTask;

        if (verbose)
        {
            Console.WriteLine($"Preloaded metadata in {preloadTime.TotalMilliseconds:F0} ms");
            Console.WriteLine(
                $"Loaded nullability info in {nullabilityLoadTime.TotalMilliseconds:F0} ms (in parallel)"
            );
            Console.WriteLine($"Generated projection in {stopwatch.ElapsedMilliseconds} ms");
            Console.WriteLine("Slowest namespaces:");

            foreach (var (ns, elapsed) in namespaceTimes.OrderByDescending(t => t.Elapsed).Take(10))
            {
                Console.WriteLine($"    {ns}: {elapsed.TotalMilliseconds:F0} ms");
            }

            stopwatch.Restart();
        }

        if (nullabilityInfoPath is not null)
        {
            NullabilityJson.Write(nullabilityInfoPath, nullabilityFile.ToSortedList());
        }

        if (verbose)
        {
            Console.WriteLine($"Wrote nullability info in {stopwatch.ElapsedMilliseconds} ms");
            Console.WriteLine(
                $"GC: gen0 {GC.CollectionCount(0)}, gen1 {GC.CollectionCount(1)}, gen2 {GC.CollectionCount(2)}, total pause {GC.GetTotalPauseDuration().TotalMilliseconds:F0} ms, allocated {GC.GetTotalAllocatedBytes() / (1024 * 1024)} MB"
            );
        }
    }
);

var parser = new CommandLineBuilder(rootCommand)
    .UseDefaults()
    .UseHelp(ctx =>
    {
        ctx.HelpBuilder.CustomizeLayout(_ =>
            HelpBuilder
                .Default.GetLayout()
                .Append(_ =>
                {
                    Console.WriteLine("Where <spec> is one or more of:");
                    Console.WriteLine();
                    Console.WriteLine(
                        "  <package>;<path>    Python package name and path to winmd file or recursively scanned folder"
                    );
                    Console.WriteLine(
                        "  local               Local %WinDir%\\System32\\WinMetadata folder"
                    );
                    Console.WriteLine(
                        "  sdk[+]              Current version of Windows SDK [with extensions]"
                    );
                    Console.WriteLine(
                        "  10.0.12345.0[+]     Specific version of Windows SDK [with extensions]"
                    );
                })
        );
    })
    .Build();

return await parser.InvokeAsync(args);
