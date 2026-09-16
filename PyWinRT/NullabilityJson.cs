using System.Buffers;
using System.Collections.Concurrent;
using System.Text.Encodings.Web;
using System.Text.Json;

/// <summary>
/// The contents of a nullability info file.
/// </summary>
/// <remarks>
/// The file is a JSON array of namespaces. It can be large (tens of MB for
/// the Windows SDK), so rather than deserializing it all at once on a single
/// thread before generation can start, only the structure of the file is
/// scanned up front and each namespace is deserialized on demand by the
/// thread that needs it.
/// </remarks>
sealed class NullabilityInfoFile
{
    private readonly ConcurrentDictionary<string, Lazy<NamespaceNullabilityInfo>> namespaces = new(
        StringComparer.Ordinal
    );

    private NullabilityInfoFile() { }

    /// <summary>
    /// Loads the nullability info file, if it exists.
    /// </summary>
    public static NullabilityInfoFile Load(FileInfo? file)
    {
        var result = new NullabilityInfoFile();

        if (file is null || !file.Exists)
        {
            return result;
        }

        var bytes = File.ReadAllBytes(file.FullName);
        var reader = new Utf8JsonReader(bytes);

        if (!reader.Read() || reader.TokenType != JsonTokenType.StartArray)
        {
            throw new JsonException($"Expected array at start of {file.FullName}");
        }

        while (reader.Read() && reader.TokenType != JsonTokenType.EndArray)
        {
            if (reader.TokenType != JsonTokenType.StartObject)
            {
                throw new JsonException($"Expected object in array in {file.FullName}");
            }

            var start = (int)reader.TokenStartIndex;
            reader.Skip();
            var length = (int)reader.BytesConsumed - start;

            var lazy = new Lazy<NamespaceNullabilityInfo>(
                () => NullabilityJson.ReadNamespace(bytes.AsSpan(start, length)),
                LazyThreadSafetyMode.ExecutionAndPublication
            );

            var name =
                NullabilityJson.ReadNamespaceName(bytes.AsSpan(start, length))
                ?? lazy.Value.Namespace;

            if (!result.namespaces.TryAdd(name, lazy))
            {
                throw new JsonException($"Duplicate namespace {name} in {file.FullName}");
            }
        }

        return result;
    }

    /// <summary>
    /// Gets the nullability info for a namespace, creating it if it doesn't exist.
    /// </summary>
    public NamespaceNullabilityInfo GetOrAdd(string @namespace) =>
        namespaces
            .GetOrAdd(
                @namespace,
                static ns => new Lazy<NamespaceNullabilityInfo>(
                    () => new NamespaceNullabilityInfo(ns, []),
                    LazyThreadSafetyMode.ExecutionAndPublication
                )
            )
            .Value;

    /// <summary>
    /// Gets all namespaces sorted by name.
    /// </summary>
    public List<NamespaceNullabilityInfo> ToSortedList()
    {
        // force deserialization of anything that hasn't been used yet
        Parallel.ForEach(namespaces.Values, static lazy => _ = lazy.Value);

        return
        [
            .. namespaces
                .OrderBy(static kv => kv.Key, StringComparer.Ordinal)
                .Select(static kv => kv.Value.Value),
        ];
    }
}

/// <summary>
/// Reads and writes the JSON representation of the nullability info.
/// </summary>
/// <remarks>
/// This is hand-written rather than using <see cref="JsonSerializer"/> since
/// the schema is small and fixed and this avoids the significant startup and
/// per-object overhead of the general-purpose serializer, which was
/// noticeable for the tens of MB of nullability info of the Windows SDK.
/// The JSON is formatted identically to what the serializer produced
/// (indented, camel case property names, enum values as camel case strings,
/// default values omitted).
/// </remarks>
static class NullabilityJson
{
    private static readonly JsonWriterOptions writerOptions = new()
    {
        Indented = true,
        Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping,
    };

    /// <summary>
    /// Writes the nullability info file.
    /// </summary>
    /// <remarks>
    /// Each namespace is serialized independently in parallel and then the
    /// pieces are combined so that the result is identical to serializing
    /// the whole list at once.
    /// </remarks>
    public static void Write(FileInfo file, IReadOnlyList<NamespaceNullabilityInfo> namespaces)
    {
        var chunks = new ArrayBufferWriter<byte>[namespaces.Count];

        Parallel.For(
            0,
            namespaces.Count,
            i =>
            {
                var ns = namespaces[i];
                var types = ns.Types;

                // estimate the size to avoid repeatedly growing the buffer
                var estimate = 256 + types.Sum(t => 128 + t.Methods.Count * 640);
                var buffer = new ArrayBufferWriter<byte>(estimate);

                using (var writer = new Utf8JsonWriter(buffer, writerOptions))
                {
                    WriteNamespace(writer, ns.Namespace, types);
                }

                chunks[i] = buffer;
            }
        );

        var newLine = GetIndentedNewLine();

        FileContent.WriteIfChanged(
            file.Directory ?? new DirectoryInfo("."),
            file.Name,
            sink =>
            {
                sink.Write("["u8);

                for (var i = 0; i < chunks.Length; i++)
                {
                    if (i > 0)
                    {
                        sink.Write(","u8);
                    }

                    sink.Write(newLine);
                    sink.Write("  "u8);
                    WriteIndented(sink, chunks[i].WrittenSpan);
                }

                if (chunks.Length > 0)
                {
                    sink.Write(newLine);
                }

                sink.Write("]"u8);
            }
        );
    }

    /// <summary>
    /// Writes <paramref name="chunk"/> to <paramref name="sink"/> with each
    /// line after the first indented by one level (2 spaces), i.e. as if it
    /// had been serialized as an element of a top-level array.
    /// </summary>
    private static void WriteIndented(ByteSink sink, ReadOnlySpan<byte> chunk)
    {
        // NB: newlines within JSON strings are always escaped, so any newline
        // byte in the serialized output is formatting.
        while (chunk.IndexOf((byte)'\n') is var index && index >= 0)
        {
            sink.Write(chunk[..(index + 1)]);
            sink.Write("  "u8);
            chunk = chunk[(index + 1)..];
        }

        sink.Write(chunk);
    }

    /// <summary>
    /// Gets the newline sequence used by the JSON writer when indenting.
    /// </summary>
    private static byte[] GetIndentedNewLine()
    {
        var buffer = new ArrayBufferWriter<byte>();

        using (var writer = new Utf8JsonWriter(buffer, writerOptions))
        {
            writer.WriteStartArray();
            writer.WriteNumberValue(0);
            writer.WriteEndArray();
        }

        // the output is "[<newline>  0<newline>]"
        var sample = buffer.WrittenSpan;
        return sample[1..sample.IndexOf((byte)' ')].ToArray();
    }

    private static void WriteNamespace(
        Utf8JsonWriter writer,
        string @namespace,
        IList<TypeNullabilityInfo> types
    )
    {
        writer.WriteStartObject();
        writer.WriteString("namespace"u8, @namespace);
        writer.WriteStartArray("types"u8);

        foreach (var type in types)
        {
            writer.WriteStartObject();
            writer.WriteString("name"u8, type.Name);
            writer.WriteStartArray("methods"u8);

            foreach (var method in type.Methods)
            {
                writer.WriteStartObject();
                writer.WriteString("signature"u8, method.Signature);
                writer.WriteString("name"u8, method.Name);
                writer.WriteStartObject("return"u8);
                writer.WritePropertyName("type"u8);
                WriteTypeRef(writer, method.Return.Type);
                writer.WriteEndObject();
                writer.WriteStartArray("parameters"u8);

                foreach (var parameter in method.Parameters)
                {
                    writer.WriteStartObject();
                    writer.WriteString("name"u8, parameter.Name);
                    writer.WriteString(
                        "kind"u8,
                        parameter.Kind switch
                        {
                            ParameterNullabilityInfo.ParameterKind.In => "in"u8,
                            ParameterNullabilityInfo.ParameterKind.Out => "out"u8,
                            ParameterNullabilityInfo.ParameterKind.PassArray => "passArray"u8,
                            ParameterNullabilityInfo.ParameterKind.FillArray => "fillArray"u8,
                            ParameterNullabilityInfo.ParameterKind.ReceiveArray => "receiveArray"u8,
                            _ => throw new NotImplementedException(),
                        }
                    );
                    writer.WritePropertyName("type"u8);
                    WriteTypeRef(writer, parameter.Type);
                    writer.WriteEndObject();
                }

                writer.WriteEndArray();
                writer.WriteEndObject();
            }

            writer.WriteEndArray();
            writer.WriteEndObject();
        }

        writer.WriteEndArray();
        writer.WriteEndObject();
    }

    private static void WriteTypeRef(Utf8JsonWriter writer, TypeRefNullabilityInfo type)
    {
        writer.WriteStartObject();
        writer.WriteString("name"u8, type.Name);

        if (type.Args is not null)
        {
            writer.WriteStartArray("args"u8);

            foreach (var arg in type.Args)
            {
                WriteTypeRef(writer, arg);
            }

            writer.WriteEndArray();
        }

        if (type.AllowNull)
        {
            writer.WriteBoolean("allowNull"u8, true);
        }

        if (type.MaybeNull)
        {
            writer.WriteBoolean("maybeNull"u8, true);
        }

        writer.WriteEndObject();
    }

    /// <summary>
    /// Reads the namespace name from a serialized <see cref="NamespaceNullabilityInfo"/>
    /// without deserializing the whole thing.
    /// </summary>
    /// <returns>
    /// The name or <c>null</c> if the namespace is not the first property.
    /// </returns>
    public static string? ReadNamespaceName(ReadOnlySpan<byte> json)
    {
        var reader = new Utf8JsonReader(json);

        if (
            reader.Read()
            && reader.TokenType == JsonTokenType.StartObject
            && reader.Read()
            && reader.TokenType == JsonTokenType.PropertyName
            && reader.ValueTextEquals("namespace"u8)
            && reader.Read()
            && reader.TokenType == JsonTokenType.String
        )
        {
            return reader.GetString();
        }

        return null;
    }

    /// <summary>
    /// Reads a serialized <see cref="NamespaceNullabilityInfo"/>.
    /// </summary>
    public static NamespaceNullabilityInfo ReadNamespace(ReadOnlySpan<byte> json)
    {
        var reader = new Utf8JsonReader(json);

        if (!reader.Read())
        {
            throw new JsonException("Unexpected end of JSON");
        }

        return ReadNamespace(ref reader);
    }

    private delegate T ReadItem<T>(ref Utf8JsonReader reader);

    private static NamespaceNullabilityInfo ReadNamespace(ref Utf8JsonReader reader)
    {
        var @namespace = default(string);
        var types = default(List<TypeNullabilityInfo>);

        ExpectStartObject(ref reader);

        while (ReadPropertyName(ref reader))
        {
            if (reader.ValueTextEquals("namespace"u8))
            {
                @namespace = ReadString(ref reader);
            }
            else if (reader.ValueTextEquals("types"u8))
            {
                types = ReadList(ref reader, ReadType);
            }
            else
            {
                reader.Skip();
            }
        }

        return new NamespaceNullabilityInfo(@namespace!, types!);
    }

    private static TypeNullabilityInfo ReadType(ref Utf8JsonReader reader)
    {
        var name = default(string);
        var methods = default(List<MethodNullabilityInfo>);

        ExpectStartObject(ref reader);

        while (ReadPropertyName(ref reader))
        {
            if (reader.ValueTextEquals("name"u8))
            {
                name = ReadString(ref reader);
            }
            else if (reader.ValueTextEquals("methods"u8))
            {
                methods = ReadList(ref reader, ReadMethod);
            }
            else
            {
                reader.Skip();
            }
        }

        return new TypeNullabilityInfo(name!, methods!);
    }

    private static MethodNullabilityInfo ReadMethod(ref Utf8JsonReader reader)
    {
        var signature = default(string);
        var name = default(string);
        var @return = default(ReturnNullabilityInfo);
        var parameters = default(List<ParameterNullabilityInfo>);

        ExpectStartObject(ref reader);

        while (ReadPropertyName(ref reader))
        {
            if (reader.ValueTextEquals("signature"u8))
            {
                signature = ReadString(ref reader);
            }
            else if (reader.ValueTextEquals("name"u8))
            {
                name = ReadString(ref reader);
            }
            else if (reader.ValueTextEquals("return"u8))
            {
                reader.Read();
                @return = ReadReturn(ref reader);
            }
            else if (reader.ValueTextEquals("parameters"u8))
            {
                parameters = ReadList(ref reader, ReadParameter);
            }
            else
            {
                reader.Skip();
            }
        }

        return new MethodNullabilityInfo(signature!, name!, @return!, parameters!);
    }

    private static ReturnNullabilityInfo ReadReturn(ref Utf8JsonReader reader)
    {
        var type = default(TypeRefNullabilityInfo);

        ExpectStartObject(ref reader);

        while (ReadPropertyName(ref reader))
        {
            if (reader.ValueTextEquals("type"u8))
            {
                reader.Read();
                type = ReadTypeRef(ref reader);
            }
            else
            {
                reader.Skip();
            }
        }

        return new ReturnNullabilityInfo(type!);
    }

    private static ParameterNullabilityInfo ReadParameter(ref Utf8JsonReader reader)
    {
        var name = default(string);
        var type = default(TypeRefNullabilityInfo);

        ExpectStartObject(ref reader);

        while (ReadPropertyName(ref reader))
        {
            if (reader.ValueTextEquals("name"u8))
            {
                name = ReadString(ref reader);
            }
            else if (reader.ValueTextEquals("type"u8))
            {
                reader.Read();
                type = ReadTypeRef(ref reader);
            }
            else
            {
                // NB: this includes "kind", which is derived from the
                // metadata rather than read from the file
                reader.Skip();
            }
        }

        return new ParameterNullabilityInfo(name!, type!);
    }

    private static TypeRefNullabilityInfo ReadTypeRef(ref Utf8JsonReader reader)
    {
        var name = default(string);
        var args = default(List<TypeRefNullabilityInfo>);
        var allowNull = false;
        var maybeNull = false;

        ExpectStartObject(ref reader);

        while (ReadPropertyName(ref reader))
        {
            if (reader.ValueTextEquals("name"u8))
            {
                name = ReadString(ref reader);
            }
            else if (reader.ValueTextEquals("args"u8))
            {
                args = ReadList(ref reader, ReadTypeRef);
            }
            else if (reader.ValueTextEquals("allowNull"u8))
            {
                reader.Read();
                allowNull = reader.GetBoolean();
            }
            else if (reader.ValueTextEquals("maybeNull"u8))
            {
                reader.Read();
                maybeNull = reader.GetBoolean();
            }
            else
            {
                reader.Skip();
            }
        }

        return new TypeRefNullabilityInfo(name!, args, allowNull, maybeNull);
    }

    /// <summary>
    /// Verifies that the current token is the start of an object.
    /// </summary>
    private static void ExpectStartObject(ref Utf8JsonReader reader)
    {
        if (reader.TokenType != JsonTokenType.StartObject)
        {
            throw new JsonException($"Expected object but found {reader.TokenType}");
        }
    }

    /// <summary>
    /// Advances to the next property name in the current object.
    /// </summary>
    /// <returns>
    /// <c>true</c> if a property name was read or <c>false</c> if the end
    /// of the object was reached.
    /// </returns>
    private static bool ReadPropertyName(ref Utf8JsonReader reader)
    {
        if (!reader.Read())
        {
            throw new JsonException("Unexpected end of JSON");
        }

        return reader.TokenType switch
        {
            JsonTokenType.PropertyName => true,
            JsonTokenType.EndObject => false,
            _ => throw new JsonException($"Unexpected {reader.TokenType}"),
        };
    }

    /// <summary>
    /// Reads the string value of the current property.
    /// </summary>
    private static string? ReadString(ref Utf8JsonReader reader)
    {
        reader.Read();
        return reader.GetString();
    }

    /// <summary>
    /// Reads the array value of the current property.
    /// </summary>
    private static List<T>? ReadList<T>(ref Utf8JsonReader reader, ReadItem<T> readItem)
    {
        reader.Read();

        if (reader.TokenType == JsonTokenType.Null)
        {
            return null;
        }

        if (reader.TokenType != JsonTokenType.StartArray)
        {
            throw new JsonException($"Expected array but found {reader.TokenType}");
        }

        var list = new List<T>();

        while (reader.Read() && reader.TokenType != JsonTokenType.EndArray)
        {
            list.Add(readItem(ref reader));
        }

        return list;
    }
}
