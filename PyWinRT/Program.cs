using System.Collections.Concurrent;
using System.CommandLine;
using System.CommandLine.Builder;
using System.CommandLine.Help;
using System.CommandLine.Invocation;
using System.CommandLine.Parsing;
using System.Diagnostics;
using Mono.Cecil;

var inputOption = new Option<(string, string)[]>(
    "--input",
    CommandReader.ParseSpec,
    default,
    "Windows metadata to include in projection"
)
{
    Arity = ArgumentArity.ZeroOrMore,
    ArgumentHelpName = "spec",
};

var referenceOption = new Option<(string, string)[]>(
    "--reference",
    CommandReader.ParseSpec,
    default,
    "Windows metadata to reference from projection"
)
{
    Arity = ArgumentArity.ZeroOrMore,
    ArgumentHelpName = "spec"
};

var outputOption = new Option<DirectoryInfo>(
    "--output",
    () => new DirectoryInfo(Directory.GetCurrentDirectory()),
    "Location of generated projection"
)
{
    Arity = ArgumentArity.ZeroOrOne,
    ArgumentHelpName = "path"
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

var headerPathOption = new Option<DirectoryInfo?>("--header-path", "Install headers in custom path")
{
    Arity = ArgumentArity.ZeroOrOne,
    ArgumentHelpName = "path",
};

var baseHeaderPathOption = new Option<DirectoryInfo?>(
    "--base-header-path",
    "Install the version header in custom path"
)
{
    Arity = ArgumentArity.ZeroOrOne,
    ArgumentHelpName = "path",
};

var nullabilityJsonPathOption = new Option<FileInfo?>(
    "--nullability-json",
    "Nullability information JSON file"
)
{
    Arity = ArgumentArity.ZeroOrOne,
    ArgumentHelpName = "path",
};

var componentDllsOption = new Option<bool>(
    "--component-dlls",
    "Set this flag when generating projection for user components that will ship with the required .dlls in the Python package"
);

var verboseOption = new Option<bool>("--verbose", "Show detailed progress information");

var rootCommand = new RootCommand("Generate Python projection for Windows Runtime types");
rootCommand.AddOption(inputOption);
rootCommand.AddOption(referenceOption);
rootCommand.AddOption(outputOption);
rootCommand.AddOption(includeOption);
rootCommand.AddOption(excludeOption);
rootCommand.AddOption(headerPathOption);
rootCommand.AddOption(baseHeaderPathOption);
rootCommand.AddOption(nullabilityJsonPathOption);
rootCommand.AddOption(componentDllsOption);
rootCommand.AddOption(verboseOption);

rootCommand.SetHandler(
    async (InvocationContext invocationContext) =>
    {
        var resolver = new MetadataResolver();
        var types = new List<TypeDefinition>();
        var packageMap = new Dictionary<string, string>();

        var input = invocationContext.ParseResult.GetValueForOption(inputOption)!;
        var reference = invocationContext.ParseResult.GetValueForOption(referenceOption)!;
        var output = invocationContext.ParseResult.GetValueForOption(outputOption)!;
        var include = invocationContext.ParseResult.GetValueForOption(includeOption)!;
        var exclude = invocationContext.ParseResult.GetValueForOption(excludeOption)!;
        var headerPath = invocationContext.ParseResult.GetValueForOption(headerPathOption);
        var baseHeaderPath = invocationContext.ParseResult.GetValueForOption(
            baseHeaderPathOption
        );
        var nullabilityInfoPath = invocationContext.ParseResult.GetValueForOption(
            nullabilityJsonPathOption
        );
        var componentDlls = invocationContext.ParseResult.GetValueForOption(componentDllsOption);
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
                    spec.Item1,
                    new ReaderParameters { MetadataResolver = resolver }
                )
            )
            .ToList();

        var referenceAssemblies = reference
            .AsParallel()
            .AsOrdered()
            .Select(spec =>
                AssemblyDefinition.ReadAssembly(
                    spec.Item1,
                    new ReaderParameters { MetadataResolver = resolver }
                )
            )
            .ToList();

        foreach (var ((file, package), assembly) in input.Zip(inputAssemblies))
        {
            if (inputPackage is null)
            {
                inputPackage = package;
            }
            else if (inputPackage != package)
            {
                throw new Exception("All input packages must be the same python package");
            }

            resolver.Register(assembly);
            packageMap.Add(assembly.Modules.Single().Name, package);
        }

        if (inputPackage is null)
        {
            throw new Exception("At least one input package is required");
        }

        foreach (var ((file, package), assembly) in reference.Zip(referenceAssemblies))
        {
            resolver.Register(assembly);
            packageMap.Add(assembly.Modules.Single().Name, package);

            if (package == inputPackage)
            {
                throw new Exception(
                    $"Reference package ({package}) must not match input package for {file}"
                );
            }
        }

        var loadTime = stopwatch.Elapsed;
        stopwatch.Restart();

        var tasks = new List<Task>();

        // if we are building the base projection (not user components),
        // then emit some extra files
        if (reference.Length == 0)
        {
            tasks.Add(
                Task.Run(() =>
                {
                    FileWriters.WriteBaseFiles(baseHeaderPath ?? headerPath ?? output);
                })
            );
        }

        foreach (var assembly in inputAssemblies)
        {
            types.AddRange(
                assembly
                    .MainModule.Types.Where(TypeExtensions.IsWindowsRuntime)
                    .Where(t => !t.IsExclusiveTo())
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
            Console.WriteLine($"Header Path: {headerPath?.FullName ?? "<default>"}");
            Console.WriteLine(
                $"Base Header Path: {baseHeaderPath?.FullName ?? "<default>"}"
            );
            Console.WriteLine($"Loaded metadata in {loadTime.TotalMilliseconds:F0} ms");
            Console.WriteLine($"Filtered types in {filterTime.TotalMilliseconds:F0} ms");
        }

        var namespaceTimes = new ConcurrentBag<(string Namespace, TimeSpan Elapsed)>();

        // parameterized interface instances per header directory, for the
        // per-package GUID header written after all namespaces are done
        var genericInstances =
            new ConcurrentDictionary<string, ConcurrentDictionary<string, GenericInstanceType>>();

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

                    FileWriters.WriteNamespaceFiles(
                        output,
                        headerPath,
                        new QualifiedNamespace(inputPackage, groupNamespace),
                        () => nullabilityFileTask.Result.GetOrAdd(groupNamespace),
                        packageMap,
                        group,
                        componentDlls,
                        genericInstances
                    );

                    namespaceTimes.Add((groupNamespace, nsStopwatch.Elapsed));
                })
            );
        }

        var preloadTime = stopwatch.Elapsed;
        Thread.CurrentThread.Priority = priority;

        await Task.WhenAll(tasks);

        foreach (var (headerDir, instances) in genericInstances)
        {
            FileWriters.WritePInterfaceGuidsH(
                new DirectoryInfo(headerDir),
                new QualifiedNamespace(inputPackage, "").PyPackageModule,
                instances.Values
            );
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
