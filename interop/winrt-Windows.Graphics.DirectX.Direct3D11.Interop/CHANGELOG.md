<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in `winrt-Windows.Graphics.DirectX.Direct3D11.Interop`, which
provides the `winrt.windows.graphics.directx.direct3d11.interop` module.

The releases before v4.0 are in the [changelog at the root of the
repository](../../CHANGELOG.md).

## [4.1.0] - 2026-10-24

### Added
- `create_direct3d11_device()` creates the `IDirect3DDevice` that WinRT APIs take.

## [4.0.0] - 2026-10-23

See the [4.0 release notes][4.0] for what changed in PyWinRT as a whole.

### Added
- Wheels are published for Python 3.14 and 3.15.
- Wheels are published for the free-threaded Python 3.14t and 3.15t.

### Changed
- Versioned on its own from 4.0.0, rather than with every PyWinRT package.
- Reaches the runtime through `winrt.runtime.interop` alone.
- Builds from its sdist with nothing from PyWinRT installed.
- `MSVCP140.dll` is no longer needed.
- The wheels carry the text of their license.

### Removed
- BREAKING: Python 3.9 and 3.10 are no longer supported.

### Fixed
- Fixed the package declaring no dependency on what it imports.
- Fixed `create_direct3d11_surface_from_dxgi_surface()` typed as an `int`.
- Fixed building with mingw-w64, whose interop header lacks a function.

[4.0]: https://pywinrt.readthedocs.io/en/latest/release-notes/4.0/
