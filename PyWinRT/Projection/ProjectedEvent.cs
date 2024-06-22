using Mono.Cecil;

class ProjectedEvent(
    EventDefinition @event,
    IReadOnlyList<TypeReference> inheritance,
    IReadOnlyDictionary<GenericParameter, TypeReference>? genericArgMap
)
{
    // TODO: this should eventually made private
    public readonly EventDefinition Event = @event;

    /// <summary>
    /// Gets the name of the event.
    /// </summary>
    public string Name { get; } = @event.Name;

    /// <summary>
    /// Gets a value indicating whether the event is static.
    /// </summary>
    public bool IsStatic { get; } = @event.AddMethod.IsStatic;

    /// <summary>
    /// Gets the inherence chain of the event.
    /// </summary>
    /// <remarks>
    /// The last item in the list is the declaring type of the event.
    /// </remarks>
    public IReadOnlyList<TypeReference> Inheritance { get; } = inheritance;

    /// <summary>
    /// Gets a map of generic parameters to their type arguments.
    /// </summary>
    /// <remarks>
    /// This value is non-null only if the event is defined by an interface
    /// with generic parameters and is referenced by a class or interface
    /// that provides type arguments for those parameters.
    /// </remarks>
    public IReadOnlyDictionary<GenericParameter, TypeReference>? GenericArgMap { get; } =
        genericArgMap;

    /// <summary>
    /// Gets the add method of the event.
    /// </summary>
    public ProjectedMethod AddMethod { get; } =
        new ProjectedMethod(@event.AddMethod, inheritance, genericArgMap);

    /// <summary>
    /// Gets the remove method of the event.
    /// </summary>
    public ProjectedMethod RemoveMethod { get; } =
        new ProjectedMethod(@event.RemoveMethod, inheritance, genericArgMap);
}
