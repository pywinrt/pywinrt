using System.Collections.Concurrent;
using System.Diagnostics.CodeAnalysis;
using Mono.Cecil;

/// <summary>
/// A projected method.
/// </summary>
/// <remarks>
/// This is a wrapper around a <see cref="MethodDefinition"/> that provides
/// information relevant to the Python projection of the WinRT method.
/// </remarks>
class ProjectedMethod
{
    /// <summary>
    /// Information about a method that depends only on the method definition
    /// and not on the type that is projecting it.
    /// </summary>
    /// <remarks>
    /// Interface methods are projected once for each class that implements
    /// the interface, so this is cached to avoid recomputing it for each class.
    /// </remarks>
    private sealed class MethodInfo
    {
        public MethodInfo(MethodDefinition method)
        {
            var overloadName = default(string);
            var deprecated = default(CustomAttribute);

            foreach (var attr in method.CustomAttributes)
            {
                switch (attr.AttributeType.FullName)
                {
                    case "Windows.Foundation.Metadata.OverloadAttribute":
                        if (attr.ConstructorArguments.Count == 1)
                        {
                            if (overloadName is not null)
                            {
                                throw new InvalidOperationException(
                                    $"Multiple overload attributes on {method}"
                                );
                            }

                            overloadName =
                                attr.ConstructorArguments[0].Value as string
                                ?? throw new InvalidOperationException();
                        }
                        break;
                    case "Windows.Foundation.Metadata.DefaultOverloadAttribute":
                        IsDefaultOverload = true;
                        break;
                    case "Windows.Foundation.Metadata.DeprecatedAttribute":
                        if (deprecated is not null)
                        {
                            throw new InvalidOperationException(
                                $"Multiple deprecated attributes on {method}"
                            );
                        }

                        deprecated = attr;
                        IsDeprecated = true;
                        break;
                }
            }

            BaseName = method.Name;
            OverloadName = overloadName;
            CppName = method.IsSpecialName
                ? method.Name.Substring(method.Name.IndexOf('_') + 1)
                : method.Name;
            Signature = method.ToString();
            DeprecatedMessage = deprecated?.ConstructorArguments[0].Value as string;

            foreach (var o in method.Overrides)
            {
                if (o.DeclaringType.Resolve().IsExclusiveTo())
                {
                    IsExclusiveTo = true;
                }

                if (method.IsFamily)
                {
                    foreach (var i in method.DeclaringType.Interfaces)
                    {
                        if (i.InterfaceType.FullName != o.DeclaringType.FullName)
                        {
                            continue;
                        }

                        foreach (var a in i.CustomAttributes)
                        {
                            switch (a.AttributeType.FullName)
                            {
                                case "Windows.Foundation.Metadata.ProtectedAttribute":
                                    IsProtected = true;
                                    break;
                                case "Windows.Foundation.Metadata.OverridableAttribute":
                                    IsOverridable = true;
                                    break;
                            }
                        }
                    }
                }
            }
        }

        public string BaseName { get; }
        public string? OverloadName { get; }
        public string CppName { get; }
        public string Signature { get; }
        public bool IsDefaultOverload { get; }
        public bool IsExclusiveTo { get; }
        public bool IsProtected { get; }
        public bool IsOverridable { get; }
        public bool IsDeprecated { get; }
        public string? DeprecatedMessage { get; }
    }

    private static readonly ConcurrentDictionary<MethodDefinition, MethodInfo> methodInfoCache =
        new(Environment.ProcessorCount * 4, 1 << 17, ReferenceEqualityComparer.Instance);

    private readonly MethodInfo info;

    /// <param name="projectedName">
    /// The name to project the method as or <c>null</c> to use the WinRT method
    /// name. This is used for overloads that would otherwise be shadowed by
    /// another overload with the same number of arguments and for overridable
    /// methods, which are always projected using the name from the
    /// <c>Overload</c> attribute.
    /// </param>
    public ProjectedMethod(
        MethodDefinition method,
        IEnumerable<TypeReference> inheritance,
        IReadOnlyDictionary<GenericParameter, TypeReference>? genericArgMap,
        string? projectedName = null
    )
    {
        Method = method;
        info = methodInfoCache.GetOrAdd(method, static m => new MethodInfo(m));
        Inheritance = inheritance.ToList();
        GenericArgMap = genericArgMap;
        Name = projectedName ?? info.BaseName;
        PyName = ToPyName(Name);
        LegacyPyName = ToPyName(info.OverloadName ?? info.BaseName);
    }

    private string ToPyName(string name) =>
        (Method.IsPublic ? "" : "_") + name.ToPythonIdentifier(isTypeMethod: Method.IsStatic);

    // TODO: this should eventually made private
    public readonly MethodDefinition Method;

    /// <summary>
    /// Gets the projected name of the method. For shadowed overloads and for
    /// overridable methods, this is the name from the <c>Overload</c> attribute
    /// instead of the WinRT method name.
    /// </summary>
    public string Name { get; }

    /// <summary>
    /// Gets the WinRT name of the method, without regard to the <c>Overload</c>
    /// attribute.
    /// </summary>
    public string BaseName => info.BaseName;

    /// <summary>
    /// Gets the name from the <c>Overload</c> attribute or <c>null</c> if the
    /// method does not have that attribute.
    /// </summary>
    public string? OverloadName => info.OverloadName;

    /// <summary>
    /// Gets the C++/WinRT name of the method.
    /// </summary>
    public string CppName => info.CppName;

    /// <summary>
    /// Gets the kind of member the method is projected as, as the name of a
    /// <c>py::member_kind</c> enumerator.
    /// </summary>
    public string MemberKind =>
        IsSpecialName switch
        {
            true when BaseName.StartsWith("get_", StringComparison.Ordinal) => "property",
            true when BaseName.StartsWith("put_", StringComparison.Ordinal) => "property",
            true when BaseName.StartsWith("add_", StringComparison.Ordinal) => "event",
            true when BaseName.StartsWith("remove_", StringComparison.Ordinal) => "event",
            _ => "method",
        };

    /// <summary>
    /// Gets the Python name of the method.
    /// </summary>
    public string PyName { get; }

    /// <summary>
    /// Gets the Python name that this method had in pywinrt v3.x, where the
    /// <c>Overload</c> attribute was always used for the method name.
    /// </summary>
    /// <remarks>
    /// When this differs from <see cref="PyName"/>, a deprecated alias is
    /// generated so that code written for pywinrt v3.x keeps working.
    /// </remarks>
    public string LegacyPyName { get; }

    /// <summary>
    /// Gets the number of parameters that are passed from Python when calling
    /// the method.
    /// </summary>
    /// <remarks>
    /// This is computed on demand since not all methods have parameters that
    /// can be projected, e.g. the constructors of metadata attributes.
    /// </remarks>
    public int PyInParamCount =>
        pyInParamCount ??= Method.Parameters.Count(p => p.IsPythonInParam());

    private int? pyInParamCount;

    /// <summary>
    /// Gets the signature of the method.
    /// </summary>
    public string Signature => info.Signature;

    /// <summary>
    /// Gets the inherence chain of the method.
    /// </summary>
    /// <remarks>
    /// The last item in the list is the declaring type of the method.
    /// </remarks>
    public IReadOnlyList<TypeReference> Inheritance { get; }

    /// <summary>
    /// Gets a map of generic parameters to their type arguments.
    /// </summary>
    /// <remarks>
    /// This value is non-null only if the method is defined by an interface
    /// with generic parameters and is referenced by a class or interface
    /// that provides type arguments for those parameters.
    /// </remarks>
    public IReadOnlyDictionary<GenericParameter, TypeReference>? GenericArgMap { get; }

    /// <summary>
    /// Gets a value indicating whether the method is the default overload.
    /// </summary>
    public bool IsDefaultOverload => info.IsDefaultOverload;

    /// <summary>
    /// Gets a value indicating whether the method is a constructor.
    /// </summary>
    public bool IsConstructor => Method.IsConstructor;

    /// <summary>
    /// Gets a value indicating whether the method is a special name.
    /// </summary>
    public bool IsSpecialName => Method.IsSpecialName;

    /// <summary>
    /// Gets a value indicating whether the method is static.
    /// </summary>
    public bool IsStatic => Method.IsStatic;

    /// <summary>
    /// Gets a value indicating whether the method is public.
    /// </summary>
    public bool IsPublic => Method.IsPublic;

    /// <summary>
    /// Gets a value indicating whether the method is implementing an exclusive interface.
    /// </summary>
    public bool IsExclusiveTo => info.IsExclusiveTo;

    /// <summary>
    /// Gets a value indicating if the method has WinRT protected semantics
    /// </summary>
    public bool IsProtected => info.IsProtected;

    /// <summary>
    /// Gets a value indicating if the method has WinRT overridable semantics
    /// </summary>
    public bool IsOverridable => info.IsOverridable;

    /// <summary>
    /// Gets a value indicating whether the method is deprecated.
    /// </summary>
    [MemberNotNullWhen(true, nameof(DeprecatedMessage))]
    public bool IsDeprecated => info.IsDeprecated;

    /// <summary>
    /// Gets the message associated with the deprecation of the method.
    /// </summary>
    public string? DeprecatedMessage => info.DeprecatedMessage;
}
