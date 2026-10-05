using Mono.Cecil;

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
        StubOverloads = OrderForStub(overloads);

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
    /// Gets the overloads in the order the type stubs list them: the ones a
    /// base type the stub derives from has overloads of come first, so that a
    /// type checker comparing them with the base finds them before the ones the
    /// type adds, and the rest follow; each in order of Python arguments.
    /// </summary>
    /// <remarks>
    /// The order only matters to pyright, which stops comparing at the first
    /// overload the base has no match for
    /// (https://github.com/microsoft/pyright/issues/11836). The overloads take
    /// different numbers of arguments, so callers see no difference.
    /// </remarks>
    public IReadOnlyList<ProjectedMethod> StubOverloads { get; }

    /// <summary>
    /// Gets the deprecated aliases of methods in this group, if any.
    /// </summary>
    public IReadOnlyList<MethodAlias> Aliases { get; }

    /// <summary>
    /// Gets a value indicating whether this group has more than one overload
    /// and therefore requires <c>@typing.overload</c> in the type stubs.
    /// </summary>
    public bool IsOverloaded => Overloads.Count > 1;

    private static IReadOnlyList<ProjectedMethod> OrderForStub(
        IReadOnlyList<ProjectedMethod> overloads
    )
    {
        var overloadedBases = overloads
            .GroupBy(InheritedFrom)
            .Where(g => g.Key is not null && g.Count() > 1)
            .Select(g => g.Key)
            .ToHashSet();

        if (overloadedBases.Count == 0)
        {
            return overloads;
        }

        return
        [
            .. overloads.Where(m => overloadedBases.Contains(InheritedFrom(m))),
            .. overloads.Where(m => !overloadedBases.Contains(InheritedFrom(m))),
        ];
    }

    /// <summary>
    /// The interface that <paramref name="method"/> belongs to, if the stub of
    /// the type it is a member of derives from that interface; an interface
    /// that is exclusive to a class is not one of the class's bases.
    /// </summary>
    private static string? InheritedFrom(ProjectedMethod method)
    {
        var declaring = method.Method.DeclaringType.Resolve();

        if (declaring.IsInterface)
        {
            return declaring.IsExclusiveTo ? null : declaring.FullName;
        }

        // A runtime class lists every method of the interfaces it implements as
        // its own, and says which interface method each one implements.
        foreach (var implemented in method.Method.Overrides)
        {
            var iface = implemented.DeclaringType.Resolve();

            if (!iface.IsExclusiveTo)
            {
                return iface.FullName;
            }
        }

        return null;
    }
}
