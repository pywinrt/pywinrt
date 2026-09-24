"""
The version that every distribution in this tree is published with.

A package's version says which metadata it was generated from, not which
generator read that metadata, so the distributions fall into families and each
family takes its version from its own source: the Windows SDK packages from
the NuGet package their winmd files come from, winrt-runtime from the file it
keeps its own version in. scripts/generate-pyproject.py writes what this
module computes into the packaging of each one.
"""

import json
import re
from pathlib import Path

REPO_PATH = Path(__file__).parent.parent

TOOLS_JSON_PATH = REPO_PATH / ".config" / "_tools.json"
RUNTIME_PATH = REPO_PATH / "runtime"
RUNTIME_VERSION_PATH = RUNTIME_PATH / "version.txt"
ABI_HEADER_PATH = RUNTIME_PATH / "python" / "winrt" / "include" / "pywinrt" / "abi.h"
TABLE_HEADER_PATH = RUNTIME_PATH / "src" / "table.h"

# Which NuGet package each family of generated packages takes its version
# from, keyed by the directory under projection/ that holds the family. The
# interop packages under interop/ are named for the same families and follow
# them, since each one is compiled against that family's headers and released
# with it.
NUGET_PACKAGES = {
    "winrt": "Microsoft.Windows.SDK.CPP",
    "winui2": "Microsoft.UI.Xaml",
    "winui3": "Microsoft.WindowsAppSDK",
    "webview2": "Microsoft.Web.WebView2",
    "test-winrt": "PyWinRT.TestWinRT",
}

# NuGet spells a prerelease as a label on the end of the version and PyWinRT
# publishes it as the PEP 440 phase of the same name, in the order upstream
# uses them. Nothing maps onto .devN or .postN, which are left for PyWinRT's
# own use: a re-release of one upstream version is a .postN of it.
PRERELEASE_PHASES = {
    "experimental": "a",
    "preview": "b",
    "prerelease": "rc",
}

NUGET_VERSION = re.compile(
    r"(?P<release>\d+(?:\.\d+)*)"
    rf"(?:-(?P<phase>{'|'.join(PRERELEASE_PHASES)})\.?(?P<number>\d*))?"
)


def nuget_to_pep440(version: str) -> str:
    """
    Converts an upstream NuGet version to the version PyWinRT publishes the
    packages generated from it with.

    Both schemes spell the release segments the same way, so only a prerelease
    label has to be translated. A version with something else on the end of it
    raises rather than being passed through: PEP 440 would read it differently
    from the way NuGet meant it, and it would then sort wrongly on PyPI with
    nothing to say so.
    """
    match = NUGET_VERSION.fullmatch(version)

    if not match:
        raise ValueError(f"cannot convert NuGet version {version!r} to PEP 440")

    if not match["phase"]:
        return match["release"]

    phase = PRERELEASE_PHASES[match["phase"]]

    # NuGet's own prerelease labels may carry no number, and PEP 440 reads a
    # phase with no number as zero, so it is written out rather than left off.
    return f"{match['release']}{phase}{match['number'] or 0}"


def read_uint16_constant(header: Path, name: str) -> int:
    """
    Reads a constant that the runtime declares in one of its headers.
    """
    match = re.search(
        rf"(?:const|constexpr) uint16_t {name} = (\d+);",
        header.read_text(encoding="utf-8"),
    )

    if not match:
        raise RuntimeError(f"{header} does not declare {name}")

    return int(match.group(1))


def runtime_table_format() -> tuple[int, int]:
    """
    Reads the projection table format that the runtime in this tree reads.
    """
    return (
        read_uint16_constant(TABLE_HEADER_PATH, "format_major"),
        read_uint16_constant(TABLE_HEADER_PATH, "format_minor"),
    )


def runtime_version() -> str:
    """
    Reads the version winrt-runtime and winrt-table-compiler are published
    with, which is hand-maintained and bumped in the release commit.
    """
    return RUNTIME_VERSION_PATH.read_text(encoding="utf-8").strip()


def compatibility_generation() -> int:
    """
    The one number that says which generation of PyWinRT a package belongs to.

    The runtime's C ABI major, the projection table format major and the
    major of winrt-runtime's own version are the same number, because each of
    them breaks every projection package at once: a package built for one
    generation works with no runtime of another. It is the epoch of every
    upstream-versioned family for the same reason - an epoch sorts above
    everything published before it, so pip tries the packages of the current
    generation first - and it is what the runtime requirement is capped at.

    Reading all three here is what keeps them from drifting apart.
    """
    abi_major = read_uint16_constant(ABI_HEADER_PATH, "runtime_abi_version_major")
    table_major, _ = runtime_table_format()
    package_major = int(runtime_version().split(".")[0])

    if table_major != abi_major:
        raise RuntimeError(
            f"{TABLE_HEADER_PATH.name} is table format major {table_major}"
            f" and {ABI_HEADER_PATH.name} is ABI major {abi_major}"
        )

    if package_major != abi_major:
        raise RuntimeError(
            f"{RUNTIME_VERSION_PATH.name} is major {package_major}"
            f" and {ABI_HEADER_PATH.name} is ABI major {abi_major}"
        )

    return abi_major


def nuget_versions() -> dict[str, str]:
    """
    The upstream version of each family's NuGet package, keyed the way
    NUGET_PACKAGES is.
    """
    with open(TOOLS_JSON_PATH) as tools_file:
        tools = json.load(tools_file)

    # KeyError here means a family names a NuGet package that fetch-tools.ps1
    # does not download
    return {family: tools[nuget] for family, nuget in NUGET_PACKAGES.items()}


def family_versions() -> dict[str, str]:
    """
    The version each family of generated packages is published with, keyed the
    way NUGET_PACKAGES is.
    """
    generation = compatibility_generation()

    return {
        family: f"{generation}!{nuget_to_pep440(version)}"
        for family, version in nuget_versions().items()
    }


def runtime_requirement() -> str:
    """
    What a package that the runtime executes requires of it.

    The floor is the runtime this tree generates for, since a package may use
    anything the runtime of its own release has. The cap is the only upper
    bound anywhere in PyWinRT's metadata, and it is there because the
    incompatibility behind it is known rather than guessed: a package of one
    compatibility generation is refused by the runtime of the next, so
    without the cap pip would upgrade the runtime on its own into an import
    error instead of moving it and the packages together.
    """
    return f"winrt-runtime>={runtime_version()},<{compatibility_generation() + 1}"


def table_compiler_requirement() -> str:
    """
    What a projection package requires of the compiler that builds it.

    A source distribution compiles its own tables, so a compiler of the next
    generation would write a wheel whose table the runtime that same wheel
    requires cannot read. That is the same known incompatibility the runtime
    requirement is capped for, so it is capped the same way; being a build
    requirement, it never reaches a wheel.
    """
    return (
        f"winrt-table-compiler>={runtime_version()},<{compatibility_generation() + 1}"
    )
