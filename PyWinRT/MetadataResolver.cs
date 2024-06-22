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

    public override TypeDefinition Resolve(TypeReference type)
    {
        var resolvedType = base.Resolve(type);

        // If type was in the same assembly, it should be resolved already
        if (resolvedType is not null)
        {
            return resolvedType;
        }

        // Otherwise look it up in the types we've registered
        if (registeredTypes.TryGetValue(type.GetElementType().FullName, out var typeDefinition))
        {
            return typeDefinition;
        }

        throw new Exception($"Could not resolve type: {type.FullName}");
    }
}
