/// <summary>
/// Represents a input package or a referenced package.
/// </summary>
public record Package
{
    public required string Name { get; init; }
    public required bool IsInputPackage { get; init; }
    /// <summary>
    /// For a input package, this will be the package's version number.
    /// For a reference package, this will be the package's version requirement.
    /// </summary>
    public string? Version { get; init; }
}
