using System.Collections.Concurrent;
using System.Text;
using System.Text.RegularExpressions;

static partial class StringExtensions
{
    // https://docs.python.org/3/reference/lexical_analysis.html#keywords
    internal static readonly HashSet<string> pythonKeywords = new([
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
        "yield",
    ]);

    // These conversions are done many times for the same identifiers
    // (once per generated file per use), so the results are cached.
    private static readonly ConcurrentDictionary<string, string> pythonIdentifierCache = new(
        Environment.ProcessorCount * 4,
        1 << 16
    );

    private static readonly ConcurrentDictionary<string, string> pythonConstantCache = new(
        Environment.ProcessorCount * 4,
        1 << 14
    );

    // Regexes applied in order by ToSnakeCase(). These are source-generated
    // and applied in order because the built-in static Regex.Replace() cache
    // only holds 15 entries, so using it here thrashed the cache and
    // dominated the runtime of the tool.

    // Replace acronyms
    [GeneratedRegex(@"^([A-Z])([A-Z0-9]+)$")]
    private static partial Regex AcronymRegex();

    // Replace memory size
    [GeneratedRegex(@"(\d+)(M|G)B")]
    private static partial Regex MemorySizeRegex();

    // Replace 3MF with _3mf
    [GeneratedRegex(@"3MF")]
    private static partial Regex ThreeMfRegex();

    // Replace ARM with Arm
    [GeneratedRegex(@"ARM")]
    private static partial Regex ArmRegex();

    // replace DB with Db, ignore ID
    [GeneratedRegex(@"(?<!I)DB")]
    private static partial Regex DbRegex();

    // Replace DRM with Drm
    [GeneratedRegex(@"DRM")]
    private static partial Regex DrmRegex();

    // Replace DirectX with Directx
    [GeneratedRegex(@"DirectX")]
    private static partial Regex DirectXRegex();

    // replace D3D with D3d
    [GeneratedRegex(@"(D(?:irect)?)3D")]
    private static partial Regex D3DRegex();

    // Replace 3D with _3d
    [GeneratedRegex(@"(?<!\d)3D(?!ay)")]
    private static partial Regex ThreeDRegex();

    // Replace DOM with Dom
    [GeneratedRegex(@"DOM")]
    private static partial Regex DomRegex();

    // Replace EU with Eu
    [GeneratedRegex(@"EU")]
    private static partial Regex EuRegex();

    // Replace ID with Id, ignore UI
    [GeneratedRegex(@"(?<![A-Z])ID")]
    private static partial Regex IdRegex();

    // Replace IO with Io
    [GeneratedRegex(@"(?<![A-Z])IO")]
    private static partial Regex IoRegex();

    // Replace iOS with Ios
    [GeneratedRegex(@"iOS")]
    private static partial Regex IosRegex();

    // Replace IP with Ip, ignore UI
    [GeneratedRegex(@"(?<!U)IP(?=[A-Zv]|$)")]
    private static partial Regex IpRegex();

    // Replace MD5 with Md5
    [GeneratedRegex(@"MD5")]
    private static partial Regex Md5Regex();

    // Replace ML with Ml
    [GeneratedRegex(@"ML")]
    private static partial Regex MlRegex();

    // Replace NS with Ns
    [GeneratedRegex(@"NS")]
    private static partial Regex NsRegex();

    // Replace NT with Nt
    [GeneratedRegex(@"NT")]
    private static partial Regex NtRegex();

    // Replace OEM with Oem
    [GeneratedRegex(@"OEM")]
    private static partial Regex OemRegex();

    // Replace OK with Ok
    [GeneratedRegex(@"OK")]
    private static partial Regex OkRegex();

    // Replace OS with Os
    [GeneratedRegex(@"(?<![A-Z])OS")]
    private static partial Regex OsRegex();

    // Replace PPC with Ppc
    [GeneratedRegex(@"PPC")]
    private static partial Regex PpcRegex();

    // Replace PC with Pc
    [GeneratedRegex(@"PC")]
    private static partial Regex PcRegex();

    // Replace UI with Ui (also handles UInt)
    [GeneratedRegex(@"UI(?!nfo)")]
    private static partial Regex UiRegex();

    // Replace UWP with Uwp
    [GeneratedRegex(@"UWP")]
    private static partial Regex UwpRegex();

    // Replace WebView2 with Webview2
    [GeneratedRegex(@"WebView2")]
    private static partial Regex WebView2Regex();

    // Fix up interface prefix, also fixes IR, IM, IBeam
    [GeneratedRegex(@"(?<![A-Z])I([A-Z])(?!im)")]
    private static partial Regex InterfacePrefixRegex();

    private static readonly MatchEvaluator acronymEvaluator = m =>
        $"{m.Groups[1].Value}{m.Groups[2].Value.ToLowerInvariant()}";

    private static readonly MatchEvaluator interfacePrefixEvaluator = m =>
        $"I{m.Groups[1].Value.ToLowerInvariant()}";

    static string ToSnakeCase(this string str)
    {
        str = AcronymRegex().Replace(str, acronymEvaluator);
        str = MemorySizeRegex().Replace(str, "_${1}${2}b");
        str = ThreeMfRegex().Replace(str, "_3mf");
        str = ArmRegex().Replace(str, "Arm");
        str = DbRegex().Replace(str, "Db");
        str = DrmRegex().Replace(str, "Drm");
        str = DirectXRegex().Replace(str, "Directx");
        str = D3DRegex().Replace(str, "${1}3d");
        str = ThreeDRegex().Replace(str, "_3d");
        str = DomRegex().Replace(str, "Dom");
        str = EuRegex().Replace(str, "Eu");
        str = IdRegex().Replace(str, "Id");
        str = IoRegex().Replace(str, "Io");
        str = IosRegex().Replace(str, "Ios");
        str = IpRegex().Replace(str, "Ip");
        str = Md5Regex().Replace(str, "Md5");
        str = MlRegex().Replace(str, "Ml");
        str = NsRegex().Replace(str, "Ns");
        str = NtRegex().Replace(str, "Nt");
        str = OemRegex().Replace(str, "Oem");
        str = OkRegex().Replace(str, "Ok");
        str = OsRegex().Replace(str, "Os");
        str = PpcRegex().Replace(str, "Ppc");
        str = PcRegex().Replace(str, "Pc");
        str = UiRegex().Replace(str, "Ui");
        str = UwpRegex().Replace(str, "Uwp");
        str = WebView2Regex().Replace(str, "Webview2");
        str = InterfacePrefixRegex().Replace(str, interfacePrefixEvaluator);

        var sb = new StringBuilder(str.Length + 8);

        for (var i = 0; i < str.Length; i++)
        {
            var c = str[i];

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
        var identifier = pythonIdentifierCache.GetOrAdd(str, ToPythonIdentifierUncached);

        // types have a method named mro, so for metaclasses, we have to avoid this
        if (isTypeMethod && identifier == "mro")
        {
            return "mro_";
        }

        return identifier;
    }

    private static string ToPythonIdentifierUncached(string str)
    {
        var identifier = str.ToSnakeCase().ToLowerInvariant();

        if (pythonKeywords.Contains(identifier))
        {
            return $"{identifier}_";
        }

        return identifier;
    }

    public static string ToPythonConstant(this string str) =>
        pythonConstantCache.GetOrAdd(str, static s => s.ToSnakeCase().ToUpperInvariant());

    /// <summary>
    /// Converts a WinRT dotted namespace to a C++- :: namespace.
    /// </summary>
    public static string ToCppNamespace(this string ns) => ns.Replace(".", "::");

    /// <summary>
    /// Strips generic bits from type names (e.g '1)
    /// </summary>
    public static string ToNonGeneric(this string name)
    {
        var index = name.LastIndexOf('`');
        return index == -1 ? name : name[..index];
    }
}
