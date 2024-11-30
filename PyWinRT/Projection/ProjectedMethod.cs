using System.Diagnostics.CodeAnalysis;
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

    private static string GetName(MethodDefinition method) =>
        (
            method.CustomAttributes.SingleOrDefault(a =>
                a.AttributeType.FullName == "Windows.Foundation.Metadata.OverloadAttribute"
            ) is
            { ConstructorArguments: { Count: 1 } args }
        )
            ? args[0].Value as string ?? throw new InvalidOperationException()
            : method.Name;

    /// <summary>
    /// Gets the projected name of the method. For many overloaded methods, this
    /// is different from the C++/WinRT name.
    /// </summary>
    public string Name { get; } = GetName(method);

    /// <summary>
    /// Gets the C++/WinRT name of the method.
    /// </summary>
    public string CppName { get; } =
        method.IsSpecialName ? method.Name.Substring(method.Name.IndexOf('_') + 1) : method.Name;

    /// <summary>
    /// Gets the Python name of the method.
    /// </summary>
    public string PyName { get; } =
        (method.IsPublic ? "" : "_")
        + GetName(method).ToPythonIdentifier(isTypeMethod: method.IsStatic);

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

    /// <summary>
    /// Gets a value indicating if the method has WinRT protected semantics
    /// </summary>
    public bool IsProtected { get; } =
        method.IsFamily
        && method
            .Overrides.SelectMany(o =>
                method.DeclaringType.Interfaces.Where(i =>
                    i.InterfaceType.FullName == o.DeclaringType.FullName
                )
            )
            .Any(i =>
                i.CustomAttributes.Any(a =>
                    a.AttributeType.FullName == "Windows.Foundation.Metadata.ProtectedAttribute"
                )
            );

    /// <summary>
    /// Gets a value indicating if the method has WinRT overridable semantics
    /// </summary>
    public bool IsOverridable { get; } =
        method.IsFamily
        && method
            .Overrides.SelectMany(o =>
                method.DeclaringType.Interfaces.Where(i =>
                    i.InterfaceType.FullName == o.DeclaringType.FullName
                )
            )
            .Any(i =>
                i.CustomAttributes.Any(a =>
                    a.AttributeType.FullName == "Windows.Foundation.Metadata.OverridableAttribute"
                )
            );

    /// <summary>
    /// Gets a value indicating whether the method is deprecated.
    /// </summary>
    [MemberNotNullWhen(true, nameof(DeprecatedMessage))]
    public bool IsDeprecated { get; } =
        method.CustomAttributes.Any(a =>
            a.AttributeType.FullName == "Windows.Foundation.Metadata.DeprecatedAttribute"
        );

    /// <summary>
    /// Gets the message associated with the deprecation of the method.
    /// </summary>
    public string? DeprecatedMessage { get; } =
        method
            .CustomAttributes.SingleOrDefault(a =>
                a.AttributeType.FullName == "Windows.Foundation.Metadata.DeprecatedAttribute"
            )
            ?.ConstructorArguments[0]
            .Value as string;
}
