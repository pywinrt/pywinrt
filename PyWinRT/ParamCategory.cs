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
                when ((ByReferenceType)type).ElementType.IsArray => ParamCategory.ReceiveArray,
            { ParameterType.IsArray: true, IsOut: true } => ParamCategory.FillArray,
            { ParameterType.IsArray: true } => throw new NotImplementedException(),
            { IsIn: true } => ParamCategory.In,
            { IsOut: true } => ParamCategory.Out,
            _ => throw new NotImplementedException(
                $"Unknown parameter category for '{param.Name}' in method {param.Method}"
            ),
        };
    }

    /// <summary>
    /// Gets whether the ABI passes a pointer to the parameter's value rather
    /// than the value itself, which is what metadata spells <c>[in] ref</c>
    /// and C++/WinRT spells <c>T const&amp;</c>.
    /// </summary>
    /// <remarks>
    /// Every other category is already a pointer, so only an input can answer
    /// this. It makes no difference on x64, where a struct wider than a
    /// register is passed as a pointer to a copy whichever way it is written,
    /// and all the difference on x86, where a by-value struct is pushed inline
    /// and the callee pops it.
    /// </remarks>
    public static bool IsPassedByReference(this ParameterDefinition param)
    {
        if (param.GetCategory() != ParamCategory.In)
        {
            return false;
        }

        // The metadata spells one of these `T& modopt(IsConst)`, so the
        // by-reference part is inside the modifier rather than outside it.
        var type = param.ParameterType;

        while (type is OptionalModifierType or RequiredModifierType)
        {
            type = ((TypeSpecification)type).ElementType;
        }

        return type.IsByReference;
    }
}
