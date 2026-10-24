/// <summary>
/// What one namespace contributes to the deps.json of the distribution it is published in.
/// </summary>
/// <param name="Namespace">The WinRT namespace.</param>
/// <param name="Required">
/// The namespaces of the base classes of this one's classes, which a consumer cannot do
/// without, since importing a class's module imports its base class.
/// </param>
/// <param name="Referenced">
/// Every namespace whose types this one names: in a struct's fields, as a base class or an
/// interface a type implements or inherits, and in the signature of a member or a delegate,
/// type arguments included. It is what the <c>[all]</c> extra of a package offers.
/// </param>
/// <param name="CensusRevision">
/// The revision of the shape census the namespace's table needs, which a winrt-runtime
/// has to be built from, or from a later one, to load it.
/// </param>
/// <param name="Runtime">
/// The first winrt-runtime release that has everything the namespace needs of it, or null
/// while the census names no release of <paramref name="CensusRevision"/>.
/// </param>
/// <remarks>
/// Several namespaces may be published in one distribution, so this is handed back rather
/// than written where it is found: one deps.json is written per distribution once every
/// namespace in the run has been generated.
/// </remarks>
sealed record NamespaceDependencies(
    string Namespace,
    IReadOnlyCollection<QualifiedNamespace> Required,
    IReadOnlyCollection<QualifiedNamespace> Referenced,
    int CensusRevision,
    string? Runtime
);
