
static class Filter
{
    public static bool Includes(string type, string[] includes, string[] excludes)
    {
        if (includes.Length == 0 && excludes.Length == 0)
        {
            return true;
        }

        foreach (var exclude in excludes)
        {
            if (Match(type, exclude))
            {
                return false;
            }
        }

        foreach (var include in includes)
        {
            if (Match(type, include))
            {
                return true;
            }
        }

        return false;
    }

    private static bool Match(string type, string match)
    {
        if (match.Length <= type.Length)
        {
            return type.StartsWith(match);
        }

        if (!match.StartsWith(type))
        {
            return false;
        }

        if (match[type.Length] != '.')
        {
            return false;
        }

        return type.EndsWith(match[(type.Length + 1)..]);
    }
}
