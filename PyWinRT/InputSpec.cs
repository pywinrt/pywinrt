/// <summary>
/// One metadata file named on the command line.
/// </summary>
/// <param name="File">The path of the winmd file.</param>
/// <param name="Package">The Python root package its namespaces belong to.</param>
/// <param name="Distribution">
/// The distribution its namespaces are published in, when the metadata is part of a set
/// that is released together, or <c>null</c> when each namespace is published on its own.
/// </param>
/// <remarks>
/// A distribution is a directory the generator writes into, so which one a namespace
/// belongs to has to be known before anything is written. It comes from the command line
/// because it is a fact about how the metadata is distributed rather than about the
/// metadata: the Windows App SDK publishes a NuGet package per component and a namespace
/// follows the component it comes from, while the Windows SDK is one download whose
/// namespaces PyWinRT publishes one at a time.
/// </remarks>
readonly record struct InputSpec(string File, string Package, string? Distribution);
