<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in the `winrt-Windows.*` packages, which project the
Windows SDK, and in the hand-written interop modules beside them.

Everything that affects every PyWinRT package - `winrt-runtime`, the code
generator, the versioning scheme and the packaging - is in the [changelog at
the root of the repository](../../CHANGELOG.md), which is also where the
releases before v4.0 are.

## [Unreleased]

### Changed
- BREAKING: `TileUpdateManagerForUser.create_tile_updater_for_application()` is
  now `create_tile_updater_for_application_for_user()`. The old name is the
  overload that takes an application id, so it could not be kept as an alias.
- Updated Windows SDK to 10.0.28000.2705. Seven namespaces are new and have a
  package each: `Windows.AI.Agents.Mcp`, `Windows.ApplicationModel.Preview`,
  `Windows.Management.Update.Cluster`, `Windows.System.Power.Thermal`,
  `Windows.UI.Input.Preview.Text`, `Windows.UI.Shell.CompanionWindows` and
  `Windows.UI.Shell.Tasks`.
- REMOVED: `winrt-Windows.AI.ModelContextProtocol`. The SDK no longer has that
  namespace. `Windows.AI.Agents.Mcp` covers Model Context Protocol in this
  release, but it is not the same API: the classes for enumerating and
  describing servers are gone and what is there now filters messages.

### Fixed
- Fixed the type hint of `Matrix4x4.translation()`, which said `Vector2`
  where the value is a `Vector3`.
- Fixed building the `Windows.Graphics.DirectX.Direct3D11` interop module on
  MSYS2. mingw-w64 now ships an interop header, so the `__has_include` check
  stopped selecting our own declarations, but the `.idl` that header is
  generated from is missing `CreateDirect3D11SurfaceFromDXGISurface`, which is
  now declared separately there.
