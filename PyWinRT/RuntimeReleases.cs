using System.Text.RegularExpressions;

/// <summary>
/// The first <c>winrt-runtime</c> release that has each thing a generated
/// projection needs of it, so that a distribution can say which release it
/// requires.
/// </summary>
/// <remarks>
/// A projection package needs three things of the runtime: that it reads the
/// table format the tables are written to, that it has the trampolines for the
/// call shapes they name (the census, which records its own releases), and that
/// it has every name the generated <c>__init__.py</c> uses when it is imported.
/// The last two lists here are the ones a change to this tool can move: a new
/// format minor, or a new function the generated code calls, is listed with
/// the release that will first ship it, and a name that is not listed stops
/// the run rather than being assumed old.
/// </remarks>
static partial class RuntimeReleases
{
    /// <summary>
    /// The first release that reads each minor of
    /// <see cref="TableWriter.FormatMajor"/>, indexed by the minor.
    /// </summary>
    private static readonly string[] formatMinors = ["4.0.0"];

    /// <summary>
    /// The first release that has each name a generated <c>__init__.py</c>
    /// can use at run time.
    /// </summary>
    private static readonly Dictionary<string, string> names = new(StringComparer.Ordinal)
    {
        ["winrt.runtime._internals.alias_field"] = "4.0.0",
        ["winrt.runtime._internals.alias_method"] = "4.0.0",
        ["winrt.runtime._internals.alias_static_method"] = "4.0.0",
        ["winrt.runtime._internals.load_projection"] = "4.0.0",
        ["winrt.runtime._internals.mixin_mapping"] = "4.0.0",
        ["winrt.runtime._internals.mixin_mutable_mapping"] = "4.0.0",
        ["winrt.runtime._internals.mixin_mutable_sequence"] = "4.0.0",
        ["winrt.runtime._internals.mixin_sequence"] = "4.0.0",
        ["winrt.system.Array"] = "4.0.0",
        ["winrt.system.Char16"] = "4.0.0",
        ["winrt.system.Double"] = "4.0.0",
        ["winrt.system.Int16"] = "4.0.0",
        ["winrt.system.Int32"] = "4.0.0",
        ["winrt.system.Int64"] = "4.0.0",
        ["winrt.system.Int8"] = "4.0.0",
        ["winrt.system.Object"] = "4.0.0",
        ["winrt.system.ReadableBuffer"] = "4.0.0",
        ["winrt.system.Single"] = "4.0.0",
        ["winrt.system.UInt16"] = "4.0.0",
        ["winrt.system.UInt32"] = "4.0.0",
        ["winrt.system.UInt64"] = "4.0.0",
        ["winrt.system.UInt8"] = "4.0.0",
        ["winrt.system.WriteableBuffer"] = "4.0.0",
    };

    /// <summary>
    /// The first release that reads the table format this tool writes.
    /// </summary>
    public static string TableFormat => formatMinors[TableWriter.FormatMinor];

    /// <summary>
    /// The releases that first have the runtime names that
    /// <paramref name="source"/>, the text of a generated <c>__init__.py</c>,
    /// uses.
    /// </summary>
    /// <exception cref="InvalidOperationException">
    /// The source uses a name that is not listed.
    /// </exception>
    public static IEnumerable<string> OfNamesIn(string source) =>
        RuntimeName()
            .Matches(source)
            .Select(m => m.Value)
            .Distinct(StringComparer.Ordinal)
            .Select(name =>
                names.TryGetValue(name, out var release)
                    ? release
                    : throw new InvalidOperationException(
                        $"{name} is not in RuntimeReleases; add it with the "
                            + "winrt-runtime release it first ships in"
                    )
            )
            .ToList();

    /// <summary>
    /// The newest of <paramref name="releases"/>, or null when one of them is
    /// null, which is a release not named yet, or when there are none.
    /// </summary>
    public static string? Newest(IEnumerable<string?> releases)
    {
        string? newest = null;

        foreach (var release in releases)
        {
            if (release is null)
            {
                return null;
            }

            if (newest is null || Version.Parse(release) > Version.Parse(newest))
            {
                newest = release;
            }
        }

        return newest;
    }

    [GeneratedRegex(@"\bwinrt\.(?:runtime\._internals|system)\.\w+")]
    private static partial Regex RuntimeName();
}
