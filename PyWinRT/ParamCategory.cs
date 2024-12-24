using Mono.Cecil;

/// <summary>
/// WinRT parameter category.
/// </summary>
enum ParamCategory
{
    /// <summary>
    /// Input parameter.
    /// </summary>
    In,

    /// <summary>
    /// Output parameter.
    /// </summary>
    Out,

    /// <summary>
    /// Array input parameter to be read.
    /// </summary>
    PassArray,

    /// <summary>
    /// Array output parameter to be filled.
    /// </summary>
    FillArray,

    /// <summary>
    /// Array output parameter.
    /// </summary>
    ReceiveArray,
}

static class ParamCategoryExtensions
{
    /// <summary>
    /// Gets the WinRT parameter category.
    /// </summary>
    public static ParamCategory GetCategory(this ParameterDefinition param)
    {
        return param switch
        {
            { IsReturnValue: true, ParameterType.IsArray: true } => ParamCategory.ReceiveArray,
            { IsReturnValue: true } => ParamCategory.Out,
            { ParameterType.IsArray: true, IsIn: true } x => ParamCategory.PassArray,
            { ParameterType.IsByReference: true, ParameterType: var type, IsOut: true }
                when ((ByReferenceType)type).ElementType.IsArray
                => ParamCategory.ReceiveArray,
            { ParameterType.IsArray: true, IsOut: true } => ParamCategory.FillArray,
            { ParameterType.IsArray: true } => throw new NotImplementedException(),
            { IsIn: true } => ParamCategory.In,
            { IsOut: true } => ParamCategory.Out,
            _
                => throw new NotImplementedException(
                    $"Unknown parameter category for '{param.Name}' in method {param.Method}"
                ),
        };
    }
}
