using Mono.Cecil;

/// <summary>
/// Python argument convention.
/// </summary>
enum ArgumentConvention
{
    /// <summary>
    /// No arguments.
    /// </summary>
    None,

    /// <summary>
    /// Single argument.
    /// </summary>
    Single,

    /// <summary>
    /// Variable number of arguments.
    /// </summary>
    Variable,
}

static class ArgumentConventionExtensions
{
    /// <summary>
    /// Gets the Python argument convention for a method.
    /// </summary>
    public static ArgumentConvention GetArgumentConvention(this MethodDefinition method) =>
        method switch
        {
            { IsConstructor: true, Parameters.Count: 0 } => ArgumentConvention.None,
            { IsConstructor: true } => ArgumentConvention.Variable,
            { IsSpecialName: true, Parameters.Count: 0 } => ArgumentConvention.None,
            { IsSpecialName: true } => ArgumentConvention.Single,
            _ => ArgumentConvention.Variable,
        };
}
