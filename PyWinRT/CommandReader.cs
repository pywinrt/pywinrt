using System.CommandLine.Parsing;
using System.Diagnostics;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Xml;
using Microsoft.Win32;

enum XmlRequirement
{
    Required,
    Optional
}

class CommandReader
{
    static void AddFilesFromXml(
        IDictionary<string, string> files,
        string sdkVersion,
        string xmlPath,
        string sdkPath,
        XmlRequirement xmlPathRequirement
    )
    {
        if (!File.Exists(xmlPath) && xmlPathRequirement == XmlRequirement.Optional)
        {
            Debug.WriteLine($"Skipping not found xml file: {xmlPath}");
            return;
        }

        using var stream = File.OpenRead(xmlPath);
        using var reader = XmlReader.Create(stream);

        while (reader.Read())
        {
            if (reader.NodeType != XmlNodeType.Element)
            {
                continue;
            }

            if (reader.LocalName != "ApiContract")
            {
                continue;
            }

            var path = Path.Combine(sdkPath, "References", sdkVersion);

            reader.MoveToAttribute("name");
            var name = reader.Value;
            path = Path.Combine(path, name);

            reader.MoveToAttribute("version");
            var version = reader.Value;
            path = Path.Combine(path, version);

            reader.MoveToAttribute("name");
            name = reader.Value;
            path = Path.Combine(path, name);

            path += ".winmd";
            files.TryAdd(path, "winrt");
        }
    }

    static RegistryKey OpenSdk()
    {
        return Registry.LocalMachine.OpenSubKey(
                @"SOFTWARE\Microsoft\Windows Kits\Installed Roots"
            // https://task.ms/29349404 - The SDK sometimes stores the 64 bit
            // location into KitsRoot10 which is wrong, this breaks 64-bit
            // cppwinrt.exe, so work around this by forcing to use the WoW64 hive.
            // RegistryRights.ReadKey | (RegistryRights)Wow32Key
            ) ?? throw new Exception("Could not find the Windows SDK in the registry");
    }

    static string GetSdkPath()
    {
        var key = OpenSdk();

        if (key.GetValue("KitsRoot10") is not string path)
        {
            throw new Exception("Could not find the Windows SDK path in the registry");
        }

        return path;
    }

    static string GetModulePath()
    {
        return Assembly.GetEntryAssembly()!.Location;
    }

    static string GetSdkVersion()
    {
        var modulePath = GetModulePath();
        var sdkPath = GetSdkPath();
        var rx = new Regex(@"((\d+)\.(\d+)\.(\d+)\.(\d+))");
        var match = rx.Match(modulePath);

        if (match.Success)
        {
            var path = Path.Combine(
                sdkPath,
                "Platforms",
                "UAP",
                match.Groups[1].Value,
                "Platform.xml"
            );

            if (File.Exists(path))
            {
                return match.Groups[1].Value;
            }
        }

        using var key = OpenSdk();
        var result = default(Version);

        foreach (var subkey in key.GetSubKeyNames())
        {
            if (!rx.IsMatch(subkey))
            {
                continue;
            }

            var path = Path.Combine(sdkPath, "Platforms", "UAP", subkey, "Platform.xml");

            if (!File.Exists(path))
            {
                continue;
            }

            var version = new Version(subkey);
            if (version > result)
            {
                result = version;
            }
        }

        if (result == null)
        {
            throw new Exception("Could not find the Windows SDK");
        }

        return result.ToString();
    }

    public static (string, string)[] ParseSpec(ArgumentResult result)
    {
        var files = new SortedDictionary<string, string>(StringComparer.Ordinal);

        foreach (var token in result.Tokens)
        {
            if (token.Value == null)
            {
                continue;
            }

            if (token.Value is string value)
            {
                // or the special value "local" which means to use the local Windows WinMetadata directory
                if (value == "local")
                {
                    var local = Path.Combine(
                        Environment.GetEnvironmentVariable("WINDIR")
                            ?? throw new Exception("WINDIR environment variable missing"),
                        "System32",
                        "WinMetadata"
                    );

                    foreach (var file in Directory.EnumerateFiles(local))
                    {
                        files.Add(file, "winrt");
                    }

                    continue;
                }

                // or the special value "sdk" or "0.0.0.0" version which means to use the local Windows SDK
                var sdkVersion = default(string);

                if (value == "sdk" || value == "sdk+")
                {
                    sdkVersion = GetSdkVersion();
                }
                else
                {
                    var rx = new Regex(@"^((\d+)\.(\d+)\.(\d+)\.(\d+))\+?$");
                    var match = rx.Match(value);

                    if (match.Success)
                    {
                        sdkVersion = match.Groups[1].Value;
                    }
                }

                if (sdkVersion is not null)
                {
                    var sdkPath = GetSdkPath();
                    var xmlPath = Path.Combine(
                        sdkPath,
                        "Platforms",
                        "UAP",
                        sdkVersion,
                        "Platform.xml"
                    );

                    AddFilesFromXml(files, sdkVersion, xmlPath, sdkPath, XmlRequirement.Required);

                    if (value[^1] != '+')
                    {
                        continue;
                    }

                    foreach (
                        var item in Directory.EnumerateDirectories(
                            Path.Combine(sdkPath, "Extension SDKs")
                        )
                    )
                    {
                        xmlPath = Path.Combine(item, sdkVersion, "SDKManifest.xml");

                        AddFilesFromXml(
                            files,
                            sdkVersion,
                            xmlPath,
                            sdkPath,
                            XmlRequirement.Optional
                        );
                    }

                    continue;
                }

                var split = value.Split(";");
                if (split.Length != 2)
                {
                    result.ErrorMessage = $"Invalid spec '{value}'";
                    return [];
                }

                var package = split[0];
                var path = split[1];

                // spec can be path to a directory containing .winmd files
                if (Directory.Exists(path))
                {
                    foreach (var file in Directory.EnumerateFiles(path, "*.winmd"))
                    {
                        files.TryAdd(file, package);
                    }

                    continue;
                }

                // or path to a .winmd file
                if (File.Exists(path))
                {
                    files.TryAdd(path, package);
                    continue;
                }

                result.ErrorMessage = $"Path '{value}' is not a file or directory";
                return [];
            }
        }

        return [.. files.Select((kvp) => (kvp.Key, kvp.Value))];
    }
}
