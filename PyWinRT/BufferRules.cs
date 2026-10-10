/// <summary>
/// The <c>IBuffer</c> parameters that WinRT writes into, which the metadata
/// cannot say: an <c>IBuffer</c> is an interface pointer whatever the callee
/// does with the memory behind it.
/// </summary>
/// <remarks>
/// WinRT only reads an <c>IBuffer</c> unless a rule here or a curated fact says
/// otherwise, so the runtime gives a Python buffer passed to any other
/// parameter a read-only view, and <c>bytes</c> is taken without a copy where
/// WinRT reads. A Python buffer passed to one that WinRT fills gets a writable
/// view, and a read-only one is refused, since WinRT would write into an object
/// that Python promises never changes. A name does not say which:
/// <c>CopyToBuffer</c> writes into its buffer, but the static
/// <c>CryptographicBuffer.CopyToByteArray</c> copies out of its own. The rules
/// are for <c>Windows.Foundation</c> and <c>Windows.Storage</c>, whose
/// interfaces every component implements; a member of any other namespace is a
/// <c>fillBuffer</c> fact in its family's nullability file.
/// </remarks>
static class BufferRules
{
    /// <summary>
    /// The parameter WinRT writes into, by the interface member it belongs to,
    /// as the full name of the interface that declares it and its WinRT name.
    /// </summary>
    private static readonly Dictionary<string, string> fillBuffers = new(StringComparer.Ordinal)
    {
        ["Windows.Storage.Streams.IInputStream.ReadAsync"] = "buffer",
    };

    /// <summary>
    /// Gets whether WinRT writes into the buffer passed as the parameter of
    /// <paramref name="method"/> named <paramref name="parameter"/>, as it
    /// fills a fill array, by rule or by what <paramref name="nullabilityMap"/>
    /// says.
    /// </summary>
    public static bool IsFillBuffer(
        this ProjectedMethod method,
        string parameter,
        IReadOnlyDictionary<string, MethodNullabilityInfo> nullabilityMap
    ) =>
        method.InterfaceMembers.Any(m => fillBuffers.GetValueOrDefault(m) == parameter)
        || (
            nullabilityMap
                .GetValueOrDefault(method.Signature)
                ?.Parameters.Any(p => p.Name == parameter && p.FillBuffer)
            ?? false
        );
}
