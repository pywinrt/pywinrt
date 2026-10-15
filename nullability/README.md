# WinRT nullability

WinRT metadata does not include nullability information. This is a crowd-sourced
effort to provide nullability information.


## The files

There is one file per family of packages: `windows-sdk.json`,
`windows-app-sdk.json`, `microsoft.ui.xaml.json` (WinUI 2), `webview2.json` and
`test-winrt.json`. `PyWinRT.exe` reads the family's file when it generates the
projection (`--nullability-json`) and writes it back: every method of the
metadata gets an entry, and the facts already in the file are kept. So to add a
fact, edit the entry and run `scripts/generate-pywinrt.py`, which applies it to
the type hints and rewrites the file in its canonical form.

A file is a list of namespaces, each a list of types, each a list of methods. A
property is its `get_` and `put_` methods, an event its `add_` and `remove_`
methods, a constructor is `.ctor` and a delegate is its `Invoke` method. A
method is found by its `"signature"`, which is the join key:

```json
{
  "namespace": "Windows.Devices.Bluetooth",
  "types": [
    {
      "name": "Windows.Devices.Bluetooth.BluetoothAdapter",
      "methods": [
        {
          "signature": "Windows.Foundation.IAsyncOperation`1<Windows.Devices.Bluetooth.BluetoothAdapter> Windows.Devices.Bluetooth.BluetoothAdapter::GetDefaultAsync()",
          "name": "GetDefaultAsync",
          "return": { "type": { ... } },
          "parameters": []
        }
      ]
    }
  ]
}
```

Each parameter has a `"name"`, a `"kind"` and a `"type"`. The kind is one of
`in`, `out`, `passArray`, `fillArray` and `receiveArray`. It comes from the
metadata and is there for the reader, who needs to know an output from an input.
An array parameter's `"type"` is its element type, while an array return's is
the array's, such as `System.Double[]`. A fact on an array, parameter or return,
is about its elements (`Array[T | None]`), not the array.
A type has a `"name"` and, for a parameterized type, its `"args"`, each a type in
turn. Everything else is a fact, written only where it is true.


## Null

- `"allowNull": true` on the type of an input: the method accepts null, so the
  type hint takes `None`.
- `"maybeNull": true` on the type of a return or an output: the method may
  return null, so the type hint includes `None`.
- `"evidence"`, beside either: why it is set, quoting the documentation or
  naming a caller that checks for null.

The generator adds `None` for either, so nothing checks that `allowNull` is on
an input and `maybeNull` on an output.

The fact goes on the type that is null, which may be a type argument. An async
operation that can finish with null is marked on its result type:

```json
"return": {
  "type": {
    "name": "Windows.Foundation.IAsyncOperation`1",
    "args": [
      {
        "name": "Windows.Devices.Bluetooth.BluetoothAdapter",
        "maybeNull": true,
        "evidence": "bleak checks the result for None and reports 'No Bluetooth adapter found' (bleak/backends/winrt/scanner.py, BleakScannerWinRT.start)"
      }
    ]
  }
}
```

A reference type that neither fact marks is typed as never `None`, and a null
there still reaches Python as `None`. The generator adds `None` by rule where no
fact is needed: an `IReference<T>`, and the `completed` and `progress` getters of
an async operation.


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
