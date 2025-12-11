using System.Collections.Concurrent;
using System.CommandLine;
using System.CommandLine.Builder;
using System.CommandLine.Help;
using System.CommandLine.Invocation;
using System.CommandLine.Parsing;
using System.Text.Encodings.Web;
using System.Text.Json;
using System.Text.Json.Serialization;
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
        var nullabilityInfoPath = invocationContext.ParseResult.GetValueForOption(
            nullabilityJsonPathOption
        );
        var componentDlls = invocationContext.ParseResult.GetValueForOption(componentDllsOption);
        var verbose = invocationContext.ParseResult.GetValueForOption(verboseOption);

        var inputPackage = default(string);

        foreach (var (file, package) in input)
        {
            if (inputPackage is null)
            {
                inputPackage = package;
            }
            else if (inputPackage != package)
            {
                throw new Exception("All input packages must be the same python package");
            }

            var assembly = AssemblyDefinition.ReadAssembly(
                file,
                new ReaderParameters { MetadataResolver = resolver }
            );

            resolver.Register(assembly);
            packageMap.Add(assembly.Modules.Single().Name, package);

            types.AddRange(
                assembly
                    .MainModule.Types.Where(TypeExtensions.IsWindowsRuntime)
                    .Where(t => !t.IsExclusiveTo())
                    .Where(t => Filter.Includes(t.FullName, include, exclude))
            );
        }

        if (inputPackage is null)
        {
            throw new Exception("At least one input package is required");
        }

        foreach (var (file, package) in reference)
        {
            var assembly = AssemblyDefinition.ReadAssembly(
                file,
                new ReaderParameters { MetadataResolver = resolver }
            );

            resolver.Register(assembly);
            packageMap.Add(assembly.Modules.Single().Name, package);

            if (package == inputPackage)
            {
                throw new Exception(
                    $"Reference package ({package}) must not match input package for {file}"
                );
            }
        }

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
        }

        var tasks = new List<Task>();

        // if we are building the base projection (not user components),
        // then emit some extra files
        if (reference.Length == 0)
        {
            tasks.Add(
                Task.Run(() =>
                {
                    FileWriters.WriteBaseFiles(headerPath ?? output);
                })
            );
        }

        var oldNullabilityInfo = default(List<NamespaceNullabilityInfo>);

        if (nullabilityInfoPath is not null && nullabilityInfoPath.Exists)
        {
            using var stream = nullabilityInfoPath.OpenRead();
            oldNullabilityInfo = JsonSerializer.Deserialize<List<NamespaceNullabilityInfo>>(
                stream,
                new JsonSerializerOptions
                {
                    PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
                    Converters = { new JsonStringEnumConverter(JsonNamingPolicy.CamelCase) }
                }
            );
        }

        if (oldNullabilityInfo is null)
        {
            oldNullabilityInfo = new();
        }

        var nullabilityInfo = new ConcurrentDictionary<string, NamespaceNullabilityInfo>(
            oldNullabilityInfo.ToDictionary(i => i.Namespace)
        );

        foreach (var group in types.GroupBy(t => t.Namespace))
        {
            tasks.Add(
                Task.Run(() =>
                {
                    FileWriters.WriteNamespaceFiles(
                        output,
                        headerPath,
                        new QualifiedNamespace(inputPackage, group.Key),
                        nullabilityInfo.GetOrAdd(
                            group.Key,
                            _ => new NamespaceNullabilityInfo(group.Key, [])
                        ),
                        packageMap,
                        group,
                        componentDlls
                    );
                })
            );
        }

        await Task.WhenAll(tasks);

        if (nullabilityInfoPath is not null)
        {
            using var stream = nullabilityInfoPath.Create();

            JsonSerializer.Serialize(
                stream,
                nullabilityInfo
                    .OrderBy(i => i.Key, StringComparer.Ordinal)
                    .Select(i => i.Value)
                    .ToList(),
                new JsonSerializerOptions
                {
                    WriteIndented = true,
                    PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
                    Converters = { new JsonStringEnumConverter(JsonNamingPolicy.CamelCase) },
                    Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping,
                }
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
