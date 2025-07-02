/// <summary>
/// Qualified namespace.
/// </summary>
/// <param name="PyPackage">The Python root package name.</param>
/// <param name="Namespace">The WinRT namespace</param>
readonly record struct QualifiedNamespace(Package PyPackage, string Namespace)
    : IComparable<QualifiedNamespace>
{
    /// <summary>
    /// Root Python module name (same as <see cref="PyPackage"/> with dashes replaced by underscore).
    /// </summary>
    public readonly string PyPackageModule = PyPackage.Name.Replace("-", "_");

    /// <summary>
    /// Converts a WinRT dotted namespace to a python extension module name.
    /// </summary>
    public readonly string NsModuleName =
        $"_{PyPackage.Name.Replace("-", "_")}_{string.Join("_", Namespace.ToLowerInvariant().Split(".").Select(s => s.ToPythonIdentifier()))}";

    /// <summary>
    /// Converts a WinRT dotted namespace to a Python dotted module name.
    /// </summary>
    public readonly string PyModuleName =
        $"{PyPackage.Name.Replace("-", "_")}.{string.Join(".", Namespace.ToLowerInvariant().Split(".").Select(s => s.ToPythonIdentifier()))}";

    /// <summary>
    /// Converts a WinRT dotted namespace to a Python dotted module name.
    /// </summary>
    public readonly string PyModuleAlias = Namespace.ToLowerInvariant().Replace(".", "_");

    public int CompareTo(QualifiedNamespace other)
    {
        var packageComparison = string.Compare(
            PyPackage.Name,
            other.PyPackage.Name,
            StringComparison.Ordinal
        );
        if (packageComparison != 0)
        {
            return packageComparison;
        }

        return string.Compare(Namespace, other.Namespace, StringComparison.Ordinal);
    }
}
