using System.Buffers;

/// <summary>
/// Destination for the bytes of a file that is being produced incrementally.
/// </summary>
abstract class ByteSink
{
    public abstract void Write(ReadOnlySpan<byte> bytes);
}

/// <summary>
/// Writes files only if their content changed, without ever holding the
/// whole content of a file in memory.
/// </summary>
/// <remarks>
/// Not writing unchanged files avoids triggering file watchers and
/// unnecessary rebuilds.
///
/// The content is produced by a callback so that it can be produced twice:
/// once to compare against the existing file and, only if it differs, again
/// to write it. This avoids allocating a buffer for the whole file. Such
/// buffers end up on the large object heap, and enough of them trigger
/// expensive blocking gen2 garbage collections that stall all threads.
/// </remarks>
static class FileContent
{
    public static void WriteIfChanged(
        DirectoryInfo directory,
        string fileName,
        Action<ByteSink> produce
    )
    {
        var filePath = Path.Combine(directory.FullName, fileName);

        // NB: opening a file for writing is much more expensive than opening
        // it read-only on Windows, so only do that when needed.
        if (File.Exists(filePath))
        {
            using var file = new FileStream(
                filePath,
                FileMode.Open,
                FileAccess.Read,
                FileShare.Read,
                bufferSize: 1,
                FileOptions.SequentialScan
            );

            using var sink = new CompareSink(file);

            produce(sink);

            if (sink.IsEqual && sink.IsAtEnd)
            {
                return;
            }
        }
        else
        {
            Directory.CreateDirectory(directory.FullName);
        }

        using var output = new FileStream(
            filePath,
            FileMode.Create,
            FileAccess.Write,
            FileShare.None,
            bufferSize: 64 * 1024
        );

        produce(new StreamSink(output));
    }

    /// <summary>
    /// Compares the bytes written to it against the bytes of a file.
    /// </summary>
    /// <remarks>
    /// The file is read in large blocks since the bytes are often written
    /// in many small pieces.
    /// </remarks>
    private sealed class CompareSink(FileStream file) : ByteSink, IDisposable
    {
        private readonly byte[] buffer = ArrayPool<byte>.Shared.Rent(64 * 1024);
        private int bufferPosition;
        private int bufferLength;

        /// <summary>
        /// <c>false</c> if any of the bytes written so far differed from the file.
        /// </summary>
        public bool IsEqual { get; private set; } = true;

        /// <summary>
        /// <c>true</c> if all bytes of the file have been compared.
        /// </summary>
        public bool IsAtEnd => bufferPosition == bufferLength && file.Position == file.Length;

        public override void Write(ReadOnlySpan<byte> bytes)
        {
            while (IsEqual && !bytes.IsEmpty)
            {
                if (bufferPosition == bufferLength)
                {
                    bufferLength = file.Read(buffer, 0, buffer.Length);
                    bufferPosition = 0;

                    if (bufferLength <= 0)
                    {
                        IsEqual = false;
                        return;
                    }
                }

                var count = Math.Min(bufferLength - bufferPosition, bytes.Length);

                if (!buffer.AsSpan(bufferPosition, count).SequenceEqual(bytes[..count]))
                {
                    IsEqual = false;
                    return;
                }

                bufferPosition += count;
                bytes = bytes[count..];
            }
        }

        public void Dispose() => ArrayPool<byte>.Shared.Return(buffer);
    }

    private sealed class StreamSink(Stream stream) : ByteSink
    {
        public override void Write(ReadOnlySpan<byte> bytes) => stream.Write(bytes);
    }
}
