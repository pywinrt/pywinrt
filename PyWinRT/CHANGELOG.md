<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in `PyWinRT`, the tool on NuGet that generates a Python
projection of a WinRT component.

The releases before v4.0 are in the [changelog at the root of the
repository](../CHANGELOG.md).

## [Unreleased]

### Added
- `--renames <file>` keeps each name a projection renamed, as a deprecated alias.

### Changed
- A name keeps an acronym in one word, as in `xy_focus_down`.
- A letter that goes with a number stays with it, as in `resolution_720p`.

### Fixed
- Fixed a type stub importing `property` when it declares no property.

## [4.0.2] - 2026-10-24

### Changed
- The type stubs declare the constructors of derivable classes as `__init__`.

## [4.0.1] - 2026-10-24

### Fixed
- Fixed `deps.json` not requiring the namespace of a struct nested in a struct.

## [4.0.0] - 2026-10-23

See the [4.0 release notes][4.0] for what changed in PyWinRT as a whole.

### Added
- The tool ships the ABI call shape census its `winrt-runtime` was built from.
- `--shapes <file>` names another census to generate against.
- `--emit-shapes <dir>` adds the shapes a component needs to a census of yours.
- `--dll-package` names the package that redistributes a component's `.dll`s.
- `--legacy-method-aliases` keeps the v3 names of overloads, deprecated.
- `--input` and `--reference` take a distribution: `<package>;<dist>;<path>`.
- WinRT's `[Deprecated]` becomes `@deprecated` in the type stubs.

### Changed
- BREAKING: A projection is a table and type stubs, so it needs no compiler.
- BREAKING: Writes `deps.json` rather than `requirements.txt` and `version.txt`.
- A member whose call shape the census lacks is reported and raises if called.
- Overloaded methods share one name again and are called by argument count.
- An unknown property in a nullability file stops the run, naming it.
- The `completed` and `progress` getters of async operations may be `None`.
- A class that implements `IMemoryBuffer` is a buffer in its table and stubs.
- A `fillBuffer` fact marks an `IBuffer` parameter that WinRT writes into.
- The changes to the type stubs it writes are in each family's changelog.
- The tool targets .NET 10, which it needs to run.
- The tool generates a projection much faster.

### Removed
- Removed `--header-path`, since nothing it writes is a header.

### Fixed
- Fixed methods being dropped when two overloads could not be told apart.
- Fixed `--include` losing the kind of a parameter in the nullability file.

[4.0]: https://pywinrt.readthedocs.io/en/latest/release-notes/4.0/
