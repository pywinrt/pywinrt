using System.Reflection;

static class PyWinRT
{
    private static readonly Lazy<string> versionString = new(GetVersionString);

    /// <summary>
    /// Gets the version of the Python/WinRT tool. (X.Y.Z format)
    /// </summary>
    public static string VersionString => versionString.Value;

    /// <summary>
    /// The major version of the winrt-runtime ABI (pywinrt/abi.h) that the
    /// generated code is written against.
    /// </summary>
    /// <remarks>
    /// A module refuses to import against a runtime with a different major
    /// version, so this only changes when the capsule changes in a way that
    /// breaks binary compatibility.
    /// </remarks>
    public const int RequiredAbiMajor = 4;

    /// <summary>
    /// The lowest minor version of the winrt-runtime ABI (pywinrt/abi.h) that
    /// the generated code can be compiled against.
    /// </summary>
    /// <remarks>
    /// The capsule is append-only within a major version, so this is bumped in
    /// the commit that first emits a call to a newer entry point - including
    /// one reached through an inline header, since the generated code carries
    /// those along.
    /// </remarks>
    public const int RequiredAbiMinor = 3;

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
