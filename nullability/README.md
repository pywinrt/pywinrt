# WinRT nullability

WinRT metadata does not include nullability information. This is a crowd-sourced
effort to provide nullability information.


## Buffers that WinRT fills

The metadata does not say whether a method writes into an `IBuffer` it is given
or only reads it, either. The projection assumes it only reads, so a read-only
Python buffer such as `bytes` is passed without a copy. A parameter that WinRT
fills is marked with `"fillBuffer": true`, beside its `"type"`, so that a
writable buffer such as a `bytearray` is required there instead:

```json
{
  "name": "buffer",
  "kind": "in",
  "type": {
    "name": "Windows.Storage.Streams.IBuffer"
  },
  "fillBuffer": true
}
```

The generator marks `IInputStream.ReadAsync()` itself, since every stream, in
any component, implements it.
