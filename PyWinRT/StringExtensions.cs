using System.Collections.ObjectModel;
using System.Text;

static class StringExtensions
{
    // https://docs.python.org/3/reference/lexical_analysis.html#keywords
    internal static readonly ReadOnlyCollection<string> pythonKeywords =
        new(
            [
                "and",
                "as",
                "assert",
                "async",
                "await",
                "break",
                "class",
                "continue",
                "def",
                "del",
                "elif",
                "else",
                "except",
                "finally",
                "for",
                "from",
                "global",
                "if",
                "import",
                "in",
                "is",
                "lambda",
                "nonlocal",
                "not",
                "or",
                "pass",
                "raise",
                "return",
                "try",
                "while",
                "with",
                "yield"
            ]
        );

    static readonly ReadOnlyCollection<string> snakeCaseExceptions = new(["UInt", "IPAddress"]);

    static string ToSnakeCase(this string str)
    {
        var sb = new StringBuilder();

        foreach (var (i, c) in str.Select((c, i) => (i, c)))
        {
            if (
                i > 0
                && char.IsUpper(c)
                && str[i - 1] != '_'
                && !snakeCaseExceptions.Any(e => str[(i - 1)..].StartsWith(e))
            )
            {
                sb.Append('_');
            }

            sb.Append(c);
        }

        return sb.ToString();
    }

    /// <summary>
    /// Writes a Python identifier name, avoiding Python keywords.
    /// </summary>
    /// <remarks>
    /// If <paramref name="str"/> is a Python keyword, a trailing underscore is added.
    /// </remarks>
    public static string ToPythonIdentifier(this string str)
    {
        var identifier = str.ToSnakeCase().ToLowerInvariant();

        if (pythonKeywords.Contains(identifier))
        {
            return $"{identifier}_";
        }

        return identifier;
    }

    public static string ToPythonConstant(this string str) => str.ToSnakeCase().ToUpperInvariant();

    /// <summary>
    /// Converts a WinRT dotted namespace to a C++- :: namespace.
    /// </summary>
    public static string ToCppNamespace(this string ns) => ns.Replace(".", "::");

    /// <summary>
    /// Converts a WinRT dotted namespace to a python extension module name.
    /// </summary>
    public static string ToNsModuleName(this string ns) =>
        $"_winrt_{string.Join("_", ns.ToLowerInvariant().Split(".").Select(s => s.ToPythonIdentifier()))}";

    /// <summary>
    /// Converts a WinRT dotted namespace to a Python dotted module name.
    /// </summary>
    public static string ToPyModuleName(this string ns) =>
        $"winrt.{string.Join(".", ns.ToLowerInvariant().Split(".").Select(s => s.ToPythonIdentifier()))}";

    /// <summary>
    /// Converts a WinRT dotted namespace to a Python dotted module name.
    /// </summary>
    public static string ToPyModuleAlias(this string ns) => ns.ToLowerInvariant().Replace(".", "_");

    /// <summary>
    /// Strips generic bits from type names (e.g '1)
    /// </summary>
    public static string ToNonGeneric(this string name)
    {
        var index = name.LastIndexOf('`');
        return index == -1 ? name : name[..index];
    }
}
