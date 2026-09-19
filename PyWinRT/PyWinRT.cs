using System.Reflection;

static class PyWinRT
{
    private static readonly Lazy<string> versionString = new(GetVersionString);

    /// <summary>
    /// Gets the version of the Python/WinRT tool. (X.Y.Z format)
    /// </summary>
    public static string VersionString => versionString.Value;

    private static string GetVersionString()
    {
        var version = Assembly
            .GetEntryAssembly()
            ?.GetCustomAttribute<AssemblyInformationalVersionAttribute>()
            ?.InformationalVersion!;

        var index = version!.IndexOf('+');
        if (index != -1)
        {
            return version[..index];
        }

        return version;
    }
}
