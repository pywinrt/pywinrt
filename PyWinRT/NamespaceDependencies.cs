/// <summary>
/// What one namespace contributes to the deps.json of the distribution it is published in.
/// </summary>
/// <param name="Namespace">The WinRT namespace.</param>
/// <param name="Required">
/// The namespaces whose types this one derives from or implements, which a consumer cannot
/// do without.
/// </param>
/// <param name="Referenced">
/// Every namespace whose types this one hands back, which is what the <c>[all]</c> extra of
/// a package offers.
/// </param>
/// <remarks>
/// Several namespaces may be published in one distribution, so this is handed back rather
/// than written where it is found: one deps.json is written per distribution once every
/// namespace in the run has been generated.
/// </remarks>
sealed record NamespaceDependencies(
    string Namespace,
    IReadOnlyCollection<QualifiedNamespace> Required,
    IReadOnlyCollection<QualifiedNamespace> Referenced
);
