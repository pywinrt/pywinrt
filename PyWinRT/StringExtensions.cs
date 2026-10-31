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

    // Replace Prc16K with Prc16k, as the PRC10_ENVELOPE beside it is written
    [GeneratedRegex(@"Prc(\d+)K")]
    private static partial Regex PrcRegex();

    // Replace SRgb with Srgb
    [GeneratedRegex(@"SRgb")]
    private static partial Regex SRgbRegex();

    // Replace RC2128Bit with Rc2_128Bit, the RC2 cipher with a 128-bit key
    [GeneratedRegex(@"RC2(\d+)Bit")]
    private static partial Regex Rc2Regex();

    // Replace BC6H16Float with Bc6h_16Float, the BC6H format with 16-bit floats
    [GeneratedRegex(@"BC6H(\d+)")]
    private static partial Regex Bc6hRegex();

    // Replace USim, CSim and ISim with Usim, Csim and Isim
    [GeneratedRegex(@"^([UCI])Sim$")]
    private static partial Regex SimRegex();

    // Put a screen resolution such as 1080p in a word of its own
    [GeneratedRegex(@"(?<=[A-Za-z])(\d+)([Pp])(?![a-z0-9])")]
    private static partial Regex ResolutionRegex();

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
        str = PrcRegex().Replace(str, "Prc${1}k");
        str = SRgbRegex().Replace(str, "Srgb");
        str = SimRegex().Replace(str, "${1}sim");
        str = Rc2Regex().Replace(str, "Rc2_${1}Bit");
        str = Bc6hRegex().Replace(str, "Bc6h_${1}");
        str = ResolutionRegex().Replace(str, "_${1}${2}");
        str = UiRegex().Replace(str, "Ui");
        str = UwpRegex().Replace(str, "Uwp");
        str = WebView2Regex().Replace(str, "Webview2");
        str = InterfacePrefixRegex().Replace(str, interfacePrefixEvaluator);

        var sb = new StringBuilder(str.Length + 8);

        for (var i = 0; i < str.Length; i++)
        {
            if (i > 0 && str[i - 1] != '_' && StartsWord(str, i))
            {
                sb.Append('_');
            }

            sb.Append(str[i]);
        }

        return sb.ToString();
    }

    /// <summary>
    /// Tests whether the character at <paramref name="i"/> starts a word of
    /// a PascalCase name, as one that follows another character.
    /// </summary>
    /// <remarks>
    /// A capital after a lowercase letter starts a word. A run of capitals is
    /// one word, as an acronym is, except that the last of them starts the
    /// next word when a lowercase letter follows it, so XYFocusDown is
    /// xy_focus_down, and an acronym keeps the s of a plural that ends the
    /// name, so GetFileURLs is get_file_urls. A capital on its own after a
    /// number belongs to the number, as in Resolution720P, and such a number
    /// is a word of its own after a lowercase letter, so that is
    /// resolution_720p, while one that follows a capital stays part of it, as
    /// in BC6HTypeless, which is bc6h_typeless.
    /// </remarks>
    private static bool StartsWord(string str, int i)
    {
        var c = str[i];
        var previous = str[i - 1];

        if (char.IsDigit(c))
        {
            return char.IsLower(previous) && EndsInLetter(str, i);
        }

        if (!char.IsUpper(c))
        {
            return false;
        }

        if (char.IsDigit(previous))
        {
            return !JoinsNumber(str, i);
        }

        if (!char.IsUpper(previous))
        {
            return true;
        }

        if (i + 1 == str.Length || !char.IsLower(str[i + 1]))
        {
            return false;
        }

        // the s of a plural acronym at the end of the name
        return !(str[i + 1] == 's' && i + 2 == str.Length);
    }

    /// <summary>
    /// Tests whether the capital at <paramref name="i"/>, which follows a
    /// digit, is a letter on its own that belongs to the number: one that the
    /// end of the name, an underscore or the start of a word follows.
    /// </summary>
    private static bool JoinsNumber(string str, int i)
    {
        if (i + 1 == str.Length || str[i + 1] == '_')
        {
            return true;
        }

        return char.IsUpper(str[i + 1]) && i + 2 < str.Length && char.IsLower(str[i + 2]);
    }

    /// <summary>
    /// Tests whether the number that starts at <paramref name="i"/> ends in a
    /// capital that belongs to it.
    /// </summary>
    private static bool EndsInLetter(string str, int i)
    {
        while (i < str.Length && char.IsDigit(str[i]))
        {
            i++;
        }

        return i < str.Length && char.IsUpper(str[i]) && JoinsNumber(str, i);
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
    /// Quotes <paramref name="str"/> as a double-quoted Python string literal.
    /// </summary>
    public static string ToPythonStringLiteral(this string str)
    {
        var sb = new StringBuilder(str.Length + 2);

        sb.Append('"');

        foreach (var c in str)
        {
            switch (c)
            {
                case '\\':
                    sb.Append(@"\\");
                    break;
                case '"':
                    sb.Append("\\\"");
                    break;
                case '\n':
                    sb.Append(@"\n");
                    break;
                case '\r':
                    sb.Append(@"\r");
                    break;
                case '\t':
                    sb.Append(@"\t");
                    break;
                default:
                    if (char.IsControl(c))
                    {
                        sb.Append($"\\x{(int)c:x2}");
                    }
                    else
                    {
                        sb.Append(c);
                    }
                    break;
            }
        }

        sb.Append('"');

        return sb.ToString();
    }

    /// <summary>
    /// Converts a WinRT dotted namespace to a C++- :: namespace.
    /// </summary>
    /// <summary>
    /// Strips generic bits from type names (e.g '1)
    /// </summary>
    public static string ToNonGeneric(this string name)
    {
        var index = name.LastIndexOf('`');
        return index == -1 ? name : name[..index];
    }
}
