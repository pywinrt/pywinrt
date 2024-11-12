using System.Collections.ObjectModel;
using System.Text;
using System.Text.RegularExpressions;

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

    static string ToSnakeCase(this string str)
    {
        var sb = new StringBuilder();

        // Replace acronyms
        str = Regex.Replace(
            str,
            @"^([A-Z])([A-Z0-9]+)$",
            m => $"{m.Groups[1].Value}{m.Groups[2].Value.ToLowerInvariant()}"
        );
        // Replace memory size
        str = Regex.Replace(str, @"(\d+)(M|G)B", "_${1}${2}b");
        // Replace 3MF with _3mf
        str = Regex.Replace(str, @"3MF", "_3mf");
        // Replace ARM with Arm
        str = Regex.Replace(str, @"ARM", "Arm");
        // replace DB with Db, ignore ID
        str = Regex.Replace(str, @"(?<!I)DB", "Db");
        // Replace DRM with Drm
        str = Regex.Replace(str, @"DRM", "Drm");
        // Replace DirectX with Directx
        str = Regex.Replace(str, @"DirectX", "Directx");
        // replace D3D with D3d
        str = Regex.Replace(str, @"(D(?:irect)?)3D", "${1}3d");
        // Replace 3D with _3d
        str = Regex.Replace(str, @"(?<!\d)3D(?!ay)", "_3d");
        // Replace DOM with Dom
        str = Regex.Replace(str, @"DOM", "Dom");
        // Replace EU with Eu
        str = Regex.Replace(str, @"EU", "Eu");
        // Replace ID with Id, ignore UI, IDR
        str = Regex.Replace(str, @"(?<![A-Z])ID(?!R$)", "Id");
        // Replace IO with Io
        str = Regex.Replace(str, @"(?<![A-Z])IO", "Io");
        // Replace iOS with Ios
        str = Regex.Replace(str, @"iOS", "Ios");
        // Replace IP with Ip, ignore UI, GIP
        str = Regex.Replace(str, @"(?<![GU])IP(?=[A-Zv]|$)", "Ip");
        // Replace LTR/RTL with Ltr/Rtl
        str = Regex.Replace(str, @"LTR", "Ltr");
        str = Regex.Replace(str, @"RTL", "Rtl");
        // Replace MD5 with Md5
        str = Regex.Replace(str, @"MD5", "Md5");
        // Replace NS with Ns
        str = Regex.Replace(str, @"NS", "Ns");
        // Replace NT with Nt
        str = Regex.Replace(str, @"NT", "Nt");
        // Replace OEM with Oem
        str = Regex.Replace(str, @"OEM", "Oem");
        // Replace OK with Ok
        str = Regex.Replace(str, @"OK", "Ok");
        // Replace OS with Os
        str = Regex.Replace(str, @"(?<![A-Z])OS", "Os");
        // Replace UI with Ui (also handles UInt)
        str = Regex.Replace(str, @"UI(?!nfo)", "Ui");
        // Replace UWP with Uwp
        str = Regex.Replace(str, @"UWP", "Uwp");
        // Fix up interface prefix, also fixes IR, IM, IBeam
        str = Regex.Replace(
            str,
            @"(?<![A-Z])I([A-Z])(?!im)",
            m => $"I{m.Groups[1].Value.ToLowerInvariant()}"
        );

        foreach (var (i, c) in str.Select((c, i) => (i, c)))
        {
            if (i > 0 && char.IsUpper(c) && str[i - 1] != '_')
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
    public static string ToPythonIdentifier(this string str, bool isTypeMethod = false)
    {
        var identifier = str.ToSnakeCase().ToLowerInvariant();

        if (pythonKeywords.Contains(identifier))
        {
            return $"{identifier}_";
        }

        // types have a method named mro, so for metaclasses, we have to avoid this
        if (isTypeMethod && identifier == "mro")
        {
            return "mro_";
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
