<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in `winrt-runtime`, the package that every PyWinRT projection
package runs on.

The releases before v4.0 are in the [changelog at the root of the
repository](../CHANGELOG.md).

## [Unreleased]

### Fixed
- Fixed iterating over a WinRT map, or its keys or items, leaking memory.
