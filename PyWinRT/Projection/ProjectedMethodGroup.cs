/// <summary>
/// A deprecated Python name that is aliased to a method of a
/// <see cref="ProjectedMethodGroup"/>.
/// </summary>
/// <param name="PyName">The deprecated Python name.</param>
/// <param name="Methods">
/// The overloads that the alias forwards to, sorted by number of Python
/// arguments.
/// </param>
/// <remarks>
/// These exist so that code written for pywinrt v3.x, where the WinRT
/// <c>Overload</c> attribute was always used for the Python method name, keeps
/// working now that overloads are called using the WinRT method name and the
/// number of arguments.
/// </remarks>
record MethodAlias(string PyName, IReadOnlyList<ProjectedMethod> Methods);

/// <summary>
/// A group of WinRT methods that are projected as a single Python method.
/// </summary>
/// <remarks>
/// WinRT overloads methods by the number of arguments, so all overloads of a
/// method share one Python method that dispatches on the number of arguments.
/// </remarks>
class ProjectedMethodGroup
{
    public ProjectedMethodGroup(
        IReadOnlyList<ProjectedMethod> overloads,
        IReadOnlyList<MethodAlias> aliases
    )
    {
        Overloads = overloads;
        Aliases = aliases;

        var first = overloads[0];

        // Protected and overridable methods are projected separately from
        // public methods of the same name, so the C++ function name needs a
        // suffix to avoid a name collision. WinRT names are PascalCase, so a
        // lowercase suffix cannot collide with the name of another method.
        Name = first.IsPublic ? first.Name : $"{first.Name}_protected";
        PyName = first.PyName;
        IsStatic = first.IsStatic;
        IsPublic = first.IsPublic;
        IsProtected = first.IsProtected;
        IsOverridable = first.IsOverridable;
    }

    /// <summary>
    /// Gets the name used for the C++ function that implements this group. This
    /// is unique within the projected type.
    /// </summary>
    public string Name { get; }

    /// <summary>
    /// Gets the Python name shared by all overloads in this group.
    /// </summary>
    public string PyName { get; }

    /// <summary>
    /// Gets a value indicating whether the methods are static.
    /// </summary>
    public bool IsStatic { get; }

    /// <summary>
    /// Gets a value indicating whether the methods are public.
    /// </summary>
    public bool IsPublic { get; }

    /// <summary>
    /// Gets a value indicating if the methods have WinRT protected semantics.
    /// </summary>
    public bool IsProtected { get; }

    /// <summary>
    /// Gets a value indicating if the methods have WinRT overridable semantics.
    /// </summary>
    public bool IsOverridable { get; }

    /// <summary>
    /// Gets the overloads in this group, sorted by number of Python arguments.
    /// </summary>
    public IReadOnlyList<ProjectedMethod> Overloads { get; }

    /// <summary>
    /// Gets the deprecated aliases of methods in this group, if any.
    /// </summary>
    public IReadOnlyList<MethodAlias> Aliases { get; }

    /// <summary>
    /// Gets a value indicating whether this group has more than one overload
    /// and therefore requires <c>@typing.overload</c> in the type stubs.
    /// </summary>
    public bool IsOverloaded => Overloads.Count > 1;
}
