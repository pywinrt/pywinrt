using Mono.Cecil;

/// <summary>
/// WinRT type category.
/// </summary>
enum Category
{
    /// <summary>
    /// Enumeration type.
    /// </summary>
    Enum,

    /// <summary>
    /// Structure type.
    /// </summary>
    Struct,

    /// <summary>
    /// Interface type.
    /// </summary>
    Interface,

    /// <summary>
    /// Runtime class type.
    /// </summary>
    Class,

    /// <summary>
    /// Delegate type.
    /// </summary>
    Delegate,
}

static class CategoryExtensions
{
    /// <summary>
    /// Gets the WinRT type category.
    /// </summary>
    public static Category GetCategory(this TypeDefinition type)
    {
        if (type.IsEnum)
        {
            return Category.Enum;
        }

        if (type.IsValueType)
        {
            return Category.Struct;
        }

        if (type.IsInterface)
        {
            return Category.Interface;
        }

        if (type.IsDelegate())
        {
            return Category.Delegate;
        }

        return Category.Class;
    }
}
