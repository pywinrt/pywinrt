using System.Reflection;

static class PyWinRT
{
    /// <summary>
    /// Gets the version of the Python/WinRT tool. (X.Y.Z format)
    /// </summary>
    public static string VersionString
    {
        get
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
}
