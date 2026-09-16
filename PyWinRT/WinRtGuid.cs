using System.Security.Cryptography;
using System.Text;
using Mono.Cecil;

/// <summary>
/// Computes WinRT type signatures and the GUIDs of parameterized interfaces
/// and delegates, the same way C++/WinRT does at compile time.
/// </summary>
/// <remarks>
/// C++/WinRT derives the GUID of e.g. <c>IAsyncOperation&lt;StorageFile&gt;</c>
/// from a constexpr SHA-1 of the type's WinRT signature string, in every
/// translation unit that uses the type. That is a large part of the frontend
/// time of a generated module, so the generator computes the same GUIDs here
/// and writes them into the generated headers as explicit specializations.
/// The algorithm mirrors <c>winrt::impl::signature</c> and
/// <c>winrt::impl::generate_guid</c> in C++/WinRT's <c>base.h</c>.
/// </remarks>
static class WinRtGuid
{
    // winrt::impl::generate_guid namespace GUID: { 0xd57af411, 0x737b, 0xc042, ... }
    // in memory order is this GUID in network order.
    private static readonly byte[] PInterfaceNamespace = ToNetworkOrder(
        new Guid("11f47ad5-7b73-42c0-abae-878b1e16adee")
    );

    /// <summary>
    /// Gets the WinRT signature of <paramref name="type"/>.
    /// </summary>
    public static string GetSignature(TypeReference type)
    {
        switch (type)
        {
            case GenericInstanceType gen:
                return $"pinterface({GetGuidAttribute(gen.ElementType.Resolve()):B};{string.Join(";", gen.GenericArguments.Select(GetSignature))})";
            case { FullName: "System.Boolean" }:
                return "b1";
            case { FullName: "System.SByte" }:
                return "i1";
            case { FullName: "System.Int16" }:
                return "i2";
            case { FullName: "System.Int32" }:
                return "i4";
            case { FullName: "System.Int64" }:
                return "i8";
            case { FullName: "System.Byte" }:
                return "u1";
            case { FullName: "System.UInt16" }:
                return "u2";
            case { FullName: "System.UInt32" }:
                return "u4";
            case { FullName: "System.UInt64" }:
                return "u8";
            case { FullName: "System.Single" }:
                return "f4";
            case { FullName: "System.Double" }:
                return "f8";
            case { FullName: "System.Char" }:
                return "c2";
            case { FullName: "System.Guid" }:
                return "g16";
            case { FullName: "System.String" }:
                return "string";
            case { FullName: "System.Object" }:
                return "cinterface(IInspectable)";
            case { FullName: "System.Type" }:
                return "struct(Windows.UI.Xaml.Interop.TypeName;string;enum(Windows.UI.Xaml.Interop.TypeKind;i4))";
        }

        var def = type.Resolve();

        if (def.IsEnum)
        {
            var underlying = def.Fields.Single(f => f.Name == "value__").FieldType.FullName;
            return $"enum({def.FullName};{(underlying == "System.UInt32" ? "u4" : "i4")})";
        }

        if (def.IsValueType)
        {
            return $"struct({def.FullName};{string.Join(";", def.Fields.Where(f => !f.IsStatic).Select(f => GetSignature(f.FieldType)))})";
        }

        if (def.IsInterface)
        {
            return GetGuidAttribute(def).ToString("B");
        }

        if (def.IsDelegate())
        {
            return $"delegate({GetGuidAttribute(def):B})";
        }

        var defaultInterface = def
            .Interfaces.Single(i =>
                i.CustomAttributes.Any(a =>
                    a.AttributeType.FullName == "Windows.Foundation.Metadata.DefaultAttribute"
                )
            )
            .InterfaceType;

        return $"rc({def.FullName};{GetSignature(defaultInterface)})";
    }

    /// <summary>
    /// Gets the GUID of a parameterized interface or delegate instance.
    /// </summary>
    public static Guid GetGuid(GenericInstanceType type)
    {
        var data = PInterfaceNamespace.Concat(Encoding.UTF8.GetBytes(GetSignature(type))).ToArray();
        var hash = SHA1.HashData(data);

        // RFC 4122 version 5, variant 1
        hash[6] = (byte)((hash[6] & 0x0f) | 0x50);
        hash[8] = (byte)((hash[8] & 0x3f) | 0x80);

        return FromNetworkOrder(hash);
    }

    /// <summary>
    /// Formats <paramref name="guid"/> as a C++/WinRT <c>winrt::guid</c>
    /// aggregate initializer.
    /// </summary>
    public static string ToCppInitializer(Guid guid)
    {
        var b = guid.ToByteArray();
        var data1 = BitConverter.ToUInt32(b, 0);
        var data2 = BitConverter.ToUInt16(b, 4);
        var data3 = BitConverter.ToUInt16(b, 6);
        var data4 = string.Join(",", b.Skip(8).Select(x => $"0x{x:X2}"));

        return $"{{ 0x{data1:X8},0x{data2:X4},0x{data3:X4},{{ {data4} }} }}";
    }

    /// <summary>
    /// Gets the other parameterized instances that the Python runtime uses to
    /// implement or consume <paramref name="type"/>, e.g. the completed
    /// handler of an async operation or the iterator of a collection.
    /// </summary>
    public static IEnumerable<GenericInstanceType> GetRelatedInstances(GenericInstanceType type)
    {
        var def = type.ElementType.Resolve();
        var args = type.GenericArguments.ToArray();

        IEnumerable<GenericInstanceType> Make(params string[] names)
        {
            foreach (var name in names)
            {
                var (ns, typeName) = name.Split(':') is [var n, var t]
                    ? (n, t)
                    : (def.Namespace, name);
                var related = def.Module.GetType(ns, typeName);

                if (related is null)
                {
                    continue;
                }

                var instance = new GenericInstanceType(related);

                foreach (var arg in args)
                {
                    instance.GenericArguments.Add(arg);
                }

                yield return instance;
            }
        }

        switch (def.FullName)
        {
            case "Windows.Foundation.IAsyncOperation`1":
                return Make("AsyncOperationCompletedHandler`1");
            case "Windows.Foundation.IAsyncActionWithProgress`1":
                return Make(
                    "AsyncActionWithProgressCompletedHandler`1",
                    "AsyncActionProgressHandler`1"
                );
            case "Windows.Foundation.IAsyncOperationWithProgress`2":
                return Make(
                    "AsyncOperationWithProgressCompletedHandler`2",
                    "AsyncOperationProgressHandler`2"
                );
            case "Windows.Foundation.Collections.IIterable`1":
                return Make("IIterator`1");
            case "Windows.Foundation.Collections.IVectorView`1":
                return Make("IIterable`1");
            case "Windows.Foundation.Collections.IVector`1":
                return Make("IVectorView`1", "IIterable`1");
            case "Windows.Foundation.Collections.IObservableVector`1":
                return Make("IVector`1", "VectorChangedEventHandler`1");
            case "Windows.Foundation.Collections.IMapView`2":
            {
                var pair = def.Module.GetType("Windows.Foundation.Collections", "IKeyValuePair`2");
                var iterable = def.Module.GetType("Windows.Foundation.Collections", "IIterable`1");

                if (pair is null || iterable is null)
                {
                    return [];
                }

                var pairInstance = new GenericInstanceType(pair);
                pairInstance.GenericArguments.Add(args[0]);
                pairInstance.GenericArguments.Add(args[1]);
                var iterableInstance = new GenericInstanceType(iterable);
                iterableInstance.GenericArguments.Add(pairInstance);

                return [pairInstance, iterableInstance];
            }
            case "Windows.Foundation.Collections.IMap`2":
                return Make("IMapView`2");
            case "Windows.Foundation.Collections.IObservableMap`2":
                return Make("IMap`2", "MapChangedEventHandler`2");
            default:
                return [];
        }
    }

    private static Guid GetGuidAttribute(TypeDefinition type)
    {
        var args = type
            .CustomAttributes.Single(a =>
                a.AttributeType.FullName == "Windows.Foundation.Metadata.GuidAttribute"
            )
            .ConstructorArguments.Select(a => a.Value)
            .ToArray();

        return new Guid(
            (uint)args[0],
            (ushort)args[1],
            (ushort)args[2],
            (byte)args[3],
            (byte)args[4],
            (byte)args[5],
            (byte)args[6],
            (byte)args[7],
            (byte)args[8],
            (byte)args[9],
            (byte)args[10]
        );
    }

    private static byte[] ToNetworkOrder(Guid guid)
    {
        var b = guid.ToByteArray();
        SwapGuidFields(b);
        return b;
    }

    private static Guid FromNetworkOrder(byte[] hash)
    {
        var b = hash.Take(16).ToArray();
        SwapGuidFields(b);
        return new Guid(b);
    }

    private static void SwapGuidFields(byte[] b)
    {
        Array.Reverse(b, 0, 4);
        Array.Reverse(b, 4, 2);
        Array.Reverse(b, 6, 2);
    }
}
