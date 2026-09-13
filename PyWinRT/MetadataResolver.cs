using Mono.Cecil;

class MetadataResolver : Mono.Cecil.MetadataResolver
{
    // NB: resolving by assembly doesn't always work, e.g. WindowsAppSDK links
    // a "windows" assembly that doesn't actually exist. So we just don't bother
    // with assembly resolution and resolve by type name instead.
    class NullAssemblyResolver : IAssemblyResolver
    {
        public void Dispose() { }

        public AssemblyDefinition? Resolve(AssemblyNameReference name)
        {
            return null;
        }

        public AssemblyDefinition? Resolve(AssemblyNameReference name, ReaderParameters parameters)
        {
            return null;
        }
    }

    private readonly Dictionary<string, TypeDefinition> registeredTypes = [];

    public MetadataResolver()
        : base(new NullAssemblyResolver()) { }

    public void Register(AssemblyDefinition assembly)
    {
        foreach (var type in assembly.MainModule.Types)
        {
            registeredTypes[type.FullName] = type;
        }
    }

    /// <summary>
    /// Resolves <paramref name="type"/> to a type definition without throwing
    /// if the type cannot be resolved.
    /// </summary>
    /// <remarks>
    /// Fundamental types in the <c>System</c> namespace (e.g. <c>System.Object</c>)
    /// are never resolvable since we don't load <c>mscorlib</c>, so this is
    /// short-circuited to avoid the cost of the lookup.
    /// </remarks>
    public TypeDefinition? TryResolve(TypeReference type)
    {
        var elementType = type.GetElementType();

        if (elementType.Namespace == "System")
        {
            return null;
        }

        var resolvedType = base.Resolve(type);

        // If type was in the same assembly, it should be resolved already
        if (resolvedType is not null)
        {
            return resolvedType;
        }

        // Otherwise look it up in the types we've registered
        if (registeredTypes.TryGetValue(elementType.FullName, out var typeDefinition))
        {
            return typeDefinition;
        }

        return null;
    }

    public override TypeDefinition Resolve(TypeReference type)
    {
        // NB: ResolutionException is what Mono.Cecil expects when it resolves
        // types internally, e.g. when reading custom attribute arguments, so
        // it can handle the failure gracefully.
        return TryResolve(type) ?? throw new ResolutionException(type);
    }
}
