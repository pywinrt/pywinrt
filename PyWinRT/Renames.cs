using System.Collections.Concurrent;
using System.Text.Json;
using System.Text.RegularExpressions;
using Mono.Cecil;

/// <summary>
/// The Python names a projection published and has since renamed, each of
/// which keeps working as a deprecated alias of the name it has now.
/// </summary>
/// <remarks>
/// The list is a JSON object keyed by the release of the projection that
/// first published the new names, such as <c>4!2.6.4</c>, because how long a
/// name has been deprecated decides when its alias moves to the next stage of
/// the deprecation and when it is removed. Each release maps the full name of
/// a WinRT type, such as <c>Windows.UI.Xaml.UIElement</c>, to an object that
/// maps each old name of a member of that type to its name now. A member is a
/// method, a property, an event, named without its <c>add_</c> and
/// <c>remove_</c>, or a member of an enum. Only a projection that published
/// the old names has code that uses them, so a projection has no list unless
/// it asks for one.
/// </remarks>
sealed partial class Renames
{
    private readonly Dictionary<string, Dictionary<string, string>> byType;
    private readonly ConcurrentDictionary<string, byte> used = new(StringComparer.Ordinal);

    private Renames(Dictionary<string, Dictionary<string, string>> byType)
    {
        this.byType = byType;
    }

    /// <summary>
    /// The list of a projection that renamed nothing.
    /// </summary>
    public static Renames None { get; } = new([]);

    /// <summary>
    /// Reads the list in <paramref name="file"/>, or gives <see cref="None"/>
    /// when there is no file.
    /// </summary>
    public static Renames Load(FileInfo? file)
    {
        if (file is null)
        {
            return None;
        }

        using var stream = file.OpenRead();
        using var document = JsonDocument.Parse(stream);

        var byType = new Dictionary<string, Dictionary<string, string>>(StringComparer.Ordinal);

        foreach (var release in document.RootElement.EnumerateObject())
        {
            if (!ReleaseRegex().IsMatch(release.Name))
            {
                throw new JsonException(
                    $"{file.FullName}: {release.Name} is not the release of a projection"
                );
            }

            foreach (var type in release.Value.EnumerateObject())
            {
                if (!byType.TryGetValue(type.Name, out var names))
                {
                    names = new(StringComparer.Ordinal);
                    byType.Add(type.Name, names);
                }

                foreach (var name in type.Value.EnumerateObject())
                {
                    var now =
                        name.Value.GetString()
                        ?? throw new JsonException(
                            $"{file.FullName}: {type.Name}: {name.Name} is not renamed to a name"
                        );

                    if (!names.TryAdd(name.Name, now))
                    {
                        throw new JsonException(
                            $"{file.FullName}: {type.Name}: {name.Name} is renamed more than once"
                        );
                    }
                }
            }
        }

        return new(byType);
    }

    // a projection's version, epoch included, as in 4!2.6.4 or 4!10.0.28000.2705.1
    [GeneratedRegex(@"^\d+!\d+(\.\d+)*$")]
    private static partial Regex ReleaseRegex();

    /// <summary>
    /// Gets the old names of the members of <paramref name="type"/>, each
    /// mapped to the member's name now.
    /// </summary>
    public IReadOnlyDictionary<string, string> Of(TypeDefinition type)
    {
        if (!byType.TryGetValue(type.FullName, out var names))
        {
            return new Dictionary<string, string>();
        }

        used.TryAdd(type.FullName, 0);

        return names;
    }

    /// <summary>
    /// Gets the types in the list that no type of the projection asked for,
    /// sorted, which means the list names a type that is not there.
    /// </summary>
    public IEnumerable<string> Unused =>
        byType.Keys.Where(k => !used.ContainsKey(k)).Order(StringComparer.Ordinal);
}
