using Mono.Cecil;

/// <summary>
/// A projected method.
/// </summary>
/// <remarks>
/// This is a wrapper around a <see cref="MethodDefinition"/> that provides
/// information relevant to the Python projection of the WinRT method.
/// </remarks>
class ProjectedMethod(
    MethodDefinition method,
    IEnumerable<TypeReference> inheritance,
    IReadOnlyDictionary<GenericParameter, TypeReference>? genericArgMap
)
{
    // TODO: this should eventually made private
    public readonly MethodDefinition Method = method;

    /// <summary>
    /// Gets the name of the method.
    /// </summary>
    public string Name { get; } = method.Name;

    /// <summary>
    /// Gets the inherence chain of the method.
    /// </summary>
    /// <remarks>
    /// The last item in the list is the declaring type of the method.
    /// </remarks>
    public IReadOnlyList<TypeReference> Inheritance { get; } = inheritance.ToList();

    /// <summary>
    /// Gets a map of generic parameters to their type arguments.
    /// </summary>
    /// <remarks>
    /// This value is non-null only if the method is defined by an interface
    /// with generic parameters and is referenced by a class or interface
    /// that provides type arguments for those parameters.
    /// </remarks>
    public IReadOnlyDictionary<GenericParameter, TypeReference>? GenericArgMap { get; } =
        genericArgMap;

    /// <summary>
    /// Gets a value indicating whether the method is the default overload.
    /// </summary>
    public bool IsDefaultOverload { get; } =
        method.CustomAttributes.Any(a =>
            a.AttributeType.FullName == "Windows.Foundation.Metadata.DefaultOverloadAttribute"
        );

    /// <summary>
    /// Gets a value indicating whether the method is a constructor.
    /// </summary>
    public bool IsConstructor { get; } = method.IsConstructor;

    /// <summary>
    /// Gets a value indicating whether the method is a special name.
    /// </summary>
    public bool IsSpecialName { get; } = method.IsSpecialName;

    /// <summary>
    /// Gets a value indicating whether the method is static.
    /// </summary>
    public bool IsStatic { get; } = method.IsStatic;
}
