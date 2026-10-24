## Moving a pin

Each family of generated packages follows one upstream NuGet package, and
`.config/upstream/upstream.csproj` pins it. Nothing builds that project. It is
what Dependabot reads, and as soon as a new release of one of the four is out,
Dependabot opens a pull request that moves its pin and nothing else. Then
`.github/workflows/upstream.yaml` finishes the pull request: it runs

    py .\scripts\move-upstream-pins.py

on the pull request's branch, pushes what that generates as a commit of its
own, and comments with what merging it would release. The script copies each
moved pin into `.config/_tools.json`, which everything else reads, deletes the
family's `fixes.json`, fetches the packages and generates everything again.
Moving a pin by hand is the same two steps: edit `upstream.csproj`, then run
the script. The four pins in `_tools.json` are never edited by hand, and the
generation refuses a `_tools.json` that disagrees with `upstream.csproj`.

GitHub holds the pull request's checks until a maintainer approves them,
since the commit was pushed with the workflow's token. Before merging:

- Approve the checks, and see that they pass.
- Read the comment. A package added for a new namespace is a project new to
  PyPI, and the comment says what that costs: a pending publisher for each,
  for a family that is published by trusted publishing, or days of the daily
  limit on new projects for the others (see Releasing below).
- A change outside `projection/`, which the comment lists, is a change to a
  package that is released on its own, such as the C++/WinRT headers that
  `winrt-runtime` compiles.

Merge with "Rebase and merge", which keeps the pin and the regeneration as two
commits. Merging releases nothing. To release the family, add a dated section
for its new version to the family's changelog, with "Updated WebView2 to
1.0.4314.35, from 1.0.4191.47." or the like and whatever API the release
added, then push the tag that `release-tags.py` prints, as Releasing below
describes.

If the regeneration fails, the comment links the run, and the pull request
moves the pin alone. The usual cause is a release that uses a call shape the
census in `runtime/src/shapes.json` did not have: the generation then refuses
until the census names the `winrt-runtime` release that will ship it, and that
release goes out before the family's.

If `main` moves on and the pull request falls behind or conflicts, comment
`@dependabot recreate` on it. Dependabot rebuilds its commit on `main`, and the
workflow generates everything again.

The Windows App SDK is pinned by its metapackage, which is empty: its
component packages are published separately, and `.config/_tools.json`
records the version of each one. Microsoft's own build refuses a project that
references a set of components it did not ship together, so the set belongs to
a release rather than to us. For the App SDK, the script takes the component
set from the nuspec of the pinned release and checks it against the list the
Runtime component carries before it writes any of them.

## Building the Nuget package

To build the nuget package for the `PyWinRT.exe` tool.

    dotnet pack PyWinRT

## Releasing

A release publishes one *unit* rather than everything, and a unit is named
after the directory its sources are in. There are two kinds.

A family of generated packages — `winrt`, `winui2`, `wasdk` and `webview2`,
the directories under `projection/` — is released on its upstream's schedule
and takes its version from the NuGet package its metadata came from, which
`.config/upstream/upstream.csproj` pins. A package that goes with a family is
released with it and has its version too: the package under `redist/` that
redistributes a family's `.dll`, such as `winrt-Microsoft.Web.WebView2.Dll`
with `webview2`, and an interop module that compiles against a family other
than the Windows SDK (below). Moving a pin is a change of its own, made the
way the section above describes.

A package of a family that changes before the pin next moves, because the
generator now writes it differently, is released again on its own with a number
on the family's version, and only the packages that changed are. Run

    py .\scripts\fix-versions.py

It compares each family with the newest of its release tags, writes the
packages that differ into the family's `fixes.json` at the next number, such as
`4!10.0.28000.2705.1`, and regenerates the packaging. The family's tag at that
version releases those packages alone. A family that has not been released at
its pin is left alone, since its changes go out with that release. When the pin
next moves, `generate-pyproject.py` refuses a `fixes.json` until it is deleted.

A package written by hand has no upstream release to follow, so it keeps its
own `version.txt` and is released on its own: `runtime`, `table` and each
directory under `interop/` that compiles against the Windows SDK alone. Bump
one when that package changes, in semver, keeping the major — it is the
compatibility generation, and the build refuses a tree whose hand-written
versions disagree about it. Moving a pin does *not* bump these: a new Windows
SDK leaves an interop module alone unless something it compiles against
actually changed, which the diff shows because those packages carry the
headers they compile.

An interop module that compiles against any other family is released with
that family and has its version, since that family's release is what its users
choose: today `interop/winrt-Microsoft.UI.Interop` and
`interop/winrt-Microsoft.WindowsAppRuntime.Bootstrap`, with `wasdk`. The
bootstrap would have to be anyway, because its wheel carries
`Microsoft.WindowsAppRuntime.Bootstrap.dll` out of the pin and loads that
release by default, so moving the pin changes it. `FAMILY_RELEASED_INTEROP` in
`scripts/versions.py` lists each such module with its family, and the build
refuses one that is not listed. They have no `version.txt`. To release a fix to
one between two pins, add one holding the family's version with a fourth number
on the end, such as `4!2.5.1.1`: that makes it a unit of its own, with a tag of
its own. When the pin next moves, `generate-pyproject.py` refuses the file until
it is deleted, and the package goes back to being released with the family. A
fix cannot follow a preview of the family, since a fourth number would sort
above the final release.

An interop package requires the newest `winrt-runtime` that one of the
`winrt.runtime.interop` functions it calls first shipped in, not the runtime of
the tree, so that a fix to it can be installed without upgrading the runtime.
`RUNTIME_FUNCTIONS` in `scripts/versions.py` records that version for each
function. Adding a function for the interop packages to call means exporting it
from `winrt.runtime.interop`, listing it there with the version it first ships
in, and bumping `runtime/version.txt` to that version in the same change:
`generate-pyproject.py` refuses a call to a function that is not listed, and a
listed version the runtime has not reached.

Either way, regenerate the packaging afterwards so that every package carries
the version it is published with:

    py .\scripts\generate-pyproject.py

Then print the tag that releases each unit and push the one you mean:

    py .\scripts\release-tags.py

    git push origin wheels/webview2/e4v1.0.4191.47

A version in a tag carries a `v`, and the epoch goes in front of that as
`e4`, since a literal `!` is a history expansion in a shell. The tag has to
carry the version the tree already carries: the build refuses a tag that
names anything else rather than publishing a version nobody asked for.

Pushing the tag runs `.github/workflows/wheels.yaml`, which builds that unit
and uploads it to PyPI. Release `runtime` before anything else and `winrt`
before the units that depend on it, because a compiled wheel's import test
installs the packages it depends on; `release-tags.py` prints them in that
order.

The upload authenticates one of two ways, and `build-bdist.py --plan` answers
`trusted=true|false` for a tag. `winui2`, `webview2` and `wasdk`, the units
in `TRUSTED_PUBLISHING` in `scripts/packages.py`, upload with PyPI trusted
publishing: each of their projects names `wheels.yaml` as its publisher, so a
project new to PyPI needs its pending publisher created there before the tag
is pushed. Every other unit — `runtime`, `table`, `winrt` and the six interop
modules that compile against the Windows SDK — uploads with the account's
token, the `PYPI_API_TOKEN` secret. One upload authenticates one way, so a
run whose units are on both sides is refused before anything is uploaded.
PyPI also lets the account create only a few new projects a day, about four
in 2026, so a tag that adds many names can be refused partway through the
upload. Once the limit resets, re-run the run's `publish` job rather than
pushing the tag again: it uploads the same files, and PyPI accepts a file it
already has when the bytes are the same.

The `PyWinRT` tool on NuGet carries no version in the tree. Pushing
`nuget/PyWinRT/v<version>` runs `.github/workflows/publish-nuget.yaml`, which
packs the tool with that version and pushes it with NuGet trusted publishing.

Once the upload is done, the `release` job of either workflow creates the
tag's GitHub release, whose notes are the section of the unit's changelog
that the tag's version heads. The Windows SDK family's release is marked
latest and no other is. `scripts/release-notes.py` prints the notes for any
tag `release-tags.py` prints, and `--title` prints the release's title:

    py .\scripts\release-notes.py wheels/runtime/v4.0.2

The same workflow can be started by hand, which is how everything else is
done. Such a run creates no GitHub release, whatever it publishes:

- `family` and `packages` are the build filters in `CONTRIBUTING.md`. `family`
  groups by the upstream a package was built against, so it is wider than a
  release: it covers the interop modules built against a family, which a
  release of that family leaves alone.
- `publish` says where the result is uploaded, and is `none` by default. A
  `none` run is a compile test of a branch, and it is what builds the
  wheelhouse a release candidate is tested from; `testpypi` is the dry run
  that proves the token path, and carries the test token whichever way the
  unit publishes on PyPI.
- `python` builds for one interpreter only, which back-fills wheels for a new
  CPython without a version change: the packages that are already published
  keep their versions and only the missing builds are uploaded.
