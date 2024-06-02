using Mono.Cecil;

class ProjectedProperty(
    PropertyDefinition property,
    IReadOnlyList<TypeReference> inheritance,
    IReadOnlyDictionary<GenericParameter, TypeReference>? genericArgMap
)
{
    // TODO: this should eventually made private
    public readonly PropertyDefinition Property = property;

    /// <summary>
    /// Gets the name of the property.
    /// </summary>
    public string Name { get; } = property.Name;

    /// <summary>
    /// Gets a value indicating whether the property is static.
    /// </summary>
    public bool IsStatic { get; } = property.GetMethod.IsStatic;

    /// <summary>/
    /// Gets the inherence chain of the property.
    /// </summary>
    /// <remarks>
    /// The last item in the list is the declaring type of the property.
    /// </remarks>
    public IReadOnlyList<TypeReference> Inheritance { get; } = inheritance;

    /// <summary>
    /// Gets a map of generic parameters to their type arguments.
    /// </summary>
    /// <remarks>
    /// This value is non-null only if the property is defined by an interface
    /// with generic parameters and is referenced by a class or interface
    /// that provides type arguments for those parameters.
    /// </remarks>
    public IReadOnlyDictionary<GenericParameter, TypeReference>? GenericArgMap { get; } =
        genericArgMap;

    /// <summary>
    /// Gets the get method of the property.
    /// </summary>
    public ProjectedMethod GetMethod { get; } =
        new ProjectedMethod(property.GetMethod, inheritance, genericArgMap);

    /// <summary>
    /// Gets the set method of the property.
    /// </summary>
    public ProjectedMethod? SetMethod { get; } =
        property.SetMethod is null || !property.SetMethod.IsPublic
            ? default
            : new ProjectedMethod(property.SetMethod, inheritance, genericArgMap);
}
