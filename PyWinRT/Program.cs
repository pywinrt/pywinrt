using System.CommandLine;
using System.CommandLine.Builder;
using System.CommandLine.Help;
using System.CommandLine.Parsing;
using Mono.Cecil;

var inputOption = new Option<string[]>(
    "--input",
    CommandReader.ParseSpec,
    default,
    "Windows metadata to include in projection"
)
{
    Arity = ArgumentArity.ZeroOrMore,
    ArgumentHelpName = "spec",
};

var referenceOption = new Option<string[]>(
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

var verboseOption = new Option<bool>("--verbose", "Show detailed progress information");

var rootCommand = new RootCommand("Generate Python projection for Windows Runtime types");
rootCommand.AddOption(inputOption);
rootCommand.AddOption(referenceOption);
rootCommand.AddOption(outputOption);
rootCommand.AddOption(includeOption);
rootCommand.AddOption(excludeOption);
rootCommand.AddOption(headerPathOption);
rootCommand.AddOption(verboseOption);

rootCommand.SetHandler(
    async (input, reference, output, include, exclude, headerPath, verbose) =>
    {
        var resolver = new MetadataResolver();
        var types = new List<TypeDefinition>();

        foreach (var file in input)
        {
            var assembly = AssemblyDefinition.ReadAssembly(
                file,
                new ReaderParameters { MetadataResolver = resolver }
            );

            resolver.Register(assembly);

            types.AddRange(
                assembly
                    .MainModule.Types.Where(TypeExtensions.IsWindowsRuntime)
                    .Where(t => !t.IsExclusiveTo())
                    .Where(t => Filter.Includes(t.FullName, include, exclude))
            );
        }

        foreach (var file in reference)
        {
            var assembly = AssemblyDefinition.ReadAssembly(
                file,
                new ReaderParameters { MetadataResolver = resolver }
            );

            resolver.Register(assembly);
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

        foreach (var group in types.GroupBy(t => t.Namespace))
        {
            tasks.Add(
                Task.Run(() =>
                {
                    FileWriters.WriteNamespaceFiles(output, headerPath, group.Key, group);
                })
            );
        }

        await Task.WhenAll(tasks);
    },
    inputOption,
    referenceOption,
    outputOption,
    includeOption,
    excludeOption,
    headerPathOption,
    verboseOption
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
                        "  <path>              Path to winmd file or recursively scanned folder"
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
