"""
The version that every distribution in this tree is published with.

A generated package's version says which metadata it was generated from,
not which generator read that metadata, so the generated distributions fall
into families and each family takes its version from the NuGet package its
winmd files come from. A package that is written by hand has no upstream
release to follow, so it keeps a version.txt of its own and is released on
its own. Either way the thing a version releases is named after the directory
its sources are in, and scripts/generate-pyproject.py writes what this module
computes into the packaging of each package.
"""

import ast
import json
import re
from collections.abc import Iterable
from pathlib import Path

REPO_PATH = Path(__file__).parent.parent

TOOLS_JSON_PATH = REPO_PATH / ".config" / "_tools.json"
TOOLS_PATH = REPO_PATH / "_tools"
RUNTIME_PATH = REPO_PATH / "runtime"
RUNTIME_VERSION_PATH = RUNTIME_PATH / "version.txt"
TABLE_PATH = REPO_PATH / "table"
TABLE_VERSION_PATH = TABLE_PATH / "version.txt"
INTEROP_PATH = REPO_PATH / "interop"
PROJECTION_PATH = REPO_PATH / "projection"
TABLE_HEADER_PATH = RUNTIME_PATH / "src" / "table.h"
SHAPES_PATH = RUNTIME_PATH / "src" / "shapes.json"
SHAPES_HEADER_PATH = RUNTIME_PATH / "src" / "shapes-generated.h"
INTEROP_HEADER_PATH = INTEROP_PATH / "interop.h"

# The lineage of the call shape census that the runtime of each compatibility
# generation is built from. A table names each member's call shape by an id
# that means something only in the census that issued it, and a runtime refuses
# a table of another lineage, so a census started afresh - which is what
# PyWinRT.exe does when it finds no shapes.json to extend - would leave every
# package of the generation unreadable by the runtime built from it.
CENSUS_LINEAGES = {4: "2e30db26-66c8-4551-85b9-a537376d8f99"}

# The winrt-runtime version that each winrt.runtime.interop function an interop
# package calls first shipped in. An interop package requires the newest of the
# ones it calls rather than the runtime of this tree, so that a fix to one can
# reach someone who keeps an older runtime (see interop_runtime_requirement()).
# InterfaceCapsule is not a function, but an annotation that names it needs it
# at run time just the same.
RUNTIME_FUNCTIONS = {
    "InterfaceCapsule": "4.0.0",
    "as_interface": "4.0.0",
    "get_activation_factory": "4.0.0",
    "hresult_error": "4.0.0",
    "wrap_interface": "4.0.0",
}

# The modules an interop package reaches those functions through: the public
# one, and the extension module that implements them.
RUNTIME_MODULES = {"winrt.runtime.interop", "winrt._winrt"}

# Which NuGet package each family of generated packages takes its version
# from, keyed by the directory under projection/ that holds the family.
NUGET_PACKAGES = {
    "winrt": "Microsoft.Windows.SDK.CPP",
    "winui2": "Microsoft.UI.Xaml",
    "wasdk": "Microsoft.WindowsAppSDK",
    "webview2": "Microsoft.Web.WebView2",
    "test-winrt": "PyWinRT.TestWinRT",
}

# The family that winrt-runtime is released as, which is a family of one:
# its version is the hand-maintained one in runtime/version.txt rather than an
# upstream package's, so it is not one of the families above.
RUNTIME_FAMILY = "runtime"


# The packages written by hand that are released with a family rather than on
# their own, keyed by directory under interop/: every interop module that
# compiles against a family other than the Windows SDK, which
# packages.generated() holds to the family it builds each one against. Such a
# family's release is what its users choose a version of, so each package of
# it carries that version, and the interop modules that go with a set of its
# packages are the ones with their version. The Windows App SDK bootstrap
# module has to besides: its wheel carries
# Microsoft.WindowsAppRuntime.Bootstrap.dll out of the pin and loads that
# release of the App SDK unless told otherwise, so what it carries changes
# whenever the pin moves, even where its source does not. None of them has a
# version.txt. A fix to one between two pins adds one, which makes that
# package a unit of its own until the pin moves (see interop_version()).
FAMILY_RELEASED_INTEROP = {
    "winrt-Microsoft.UI.Interop": "wasdk",
    "winrt-Microsoft.WindowsAppRuntime.Bootstrap": "wasdk",
}

# A family version that a fix to one of those packages, or to a generated
# package, can follow: a final release, to which the fix adds a number, a
# fourth after the App SDK's three and a fifth after the Windows SDK's four. A
# prerelease cannot take one, since the number goes before the phase and would
# sort above the final release.
FIXABLE_FAMILY_VERSION = re.compile(r"\d+!\d+(?:\.\d+)+")

# The file under a family's directory in projection/ that gives a package of
# the family that changed after the family was released the version of a fix
# (see family_fixes()), which scripts/fix-versions.py writes.
FIXES_NAME = "fixes.json"

# The test component is projected into this tree for the test suite and is
# never uploaded anywhere, so it is versioned like a family without being one
# that is released.
UNPUBLISHED_FAMILIES = frozenset({"test-winrt"})

# The tag that releases one unit, which .github/workflows/wheels.yaml
# builds and publishes that unit from. The version in it carries a "v", and
# an epoch goes in front of that as "e<epoch>", since a literal "!" is a
# history expansion in an interactive shell and is percent-encoded in a URL.
TAG_PREFIX = "wheels"

# NuGet spells a prerelease as a label on the end of the version and PyWinRT
# publishes it as the PEP 440 phase of the same name, in the order upstream
# uses them. Nothing maps onto .devN or .postN, which are left for PyWinRT's
# own use: a re-release of one upstream version is a .postN of it. A fix to
# one package that is released with a family is a number on the end instead
# (see interop_version() and family_fixes()), since it changes what the
# package does.
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


def version_files() -> list[Path]:
    """
    Every version.txt in the tree, one per package that is written rather
    than generated.

    There is no upstream release for one of these to follow, so what its
    version says is what changed in that package: each is semver with the
    compatibility generation as the major, bumped by hand, and released on
    its own rather than with anything else. The directory a file sits in is
    the unit its version releases.

    A package that is released with a family is not one of these, even while
    a fix to it has a version.txt: that version is the family's with a number
    on the end (see interop_version()).
    """
    return [
        RUNTIME_VERSION_PATH,
        TABLE_VERSION_PATH,
        *sorted(
            path
            for path in INTEROP_PATH.glob("winrt-*/version.txt")
            if path.parent.name not in FAMILY_RELEASED_INTEROP
        ),
    ]


def read_version(path: Path) -> str:
    """
    Reads a hand-maintained version.txt.
    """
    return path.read_text(encoding="utf-8").strip()


def check_fix_version(version: str, family_version: str) -> None:
    """
    Checks the version of a fix to a package that is released with a family,
    which is the family's version with a number on the end.

    A version that does not follow the family's current version is what a
    fix left behind when the pin moved, and the error says to delete it, so
    the package goes back to being released with the family.
    """
    if not FIXABLE_FAMILY_VERSION.fullmatch(family_version):
        raise ValueError(
            f"a fix is released only after a final release,"
            f" and the family is {family_version}"
        )

    if not re.fullmatch(rf"{re.escape(family_version)}\.[1-9]\d*", version):
        raise ValueError(
            f"a fix is {family_version}.N while the family is {family_version},"
            f" and this one is {version}"
        )


def interop_release(name: str) -> str:
    """
    The unit that releases an interop package, keyed by its directory.

    That is the package itself, unless it is released with a family and has
    no fix of its own waiting to be released.
    """
    if name in FAMILY_RELEASED_INTEROP:
        if not (INTEROP_PATH / name / "version.txt").exists():
            return FAMILY_RELEASED_INTEROP[name]

    return name


def interop_version(name: str) -> str:
    """
    The version an interop package is published with, keyed by its directory.

    One that is released with a family takes the family's version, or the
    version of a fix to it while its version.txt says there is one.
    """
    path = INTEROP_PATH / name / "version.txt"

    if name not in FAMILY_RELEASED_INTEROP:
        return read_version(path)

    family_version = family_versions()[FAMILY_RELEASED_INTEROP[name]]

    if not path.exists():
        return family_version

    version = read_version(path)

    try:
        check_fix_version(version, family_version)
    except ValueError as error:
        raise RuntimeError(
            f"{path.relative_to(INTEROP_PATH.parent).as_posix()} is the version of a fix to"
            f" {name}, but {error}; delete it to release {name} with the family"
        ) from error

    return version


def runtime_version() -> str:
    """
    Reads the version winrt-runtime is published with, which is
    hand-maintained and bumped in the release commit.
    """
    return read_version(RUNTIME_VERSION_PATH)


def table_compiler_version() -> str:
    """
    Reads the version winrt-table-compiler is published with.

    It is the runtime's generation but not the runtime's version: the
    compiler is released when the compiler changes, and a runtime fix that
    leaves it alone leaves this alone (see table_compiler_requirement()).
    """
    return read_version(TABLE_VERSION_PATH)


def compatibility_generation() -> int:
    """
    The one number that says which generation of PyWinRT a package belongs to.

    The projection table format major and the major of every hand-written
    package's own version are the same number, because each of them breaks
    every projection package at once: a package built for one generation
    works with no runtime of another. It is the epoch of every
    upstream-versioned family for the same reason - an epoch sorts above
    everything published before it, so pip tries the packages of the current
    generation first - and it is what the runtime requirement is capped at.

    Reading them all here is what keeps them from drifting apart. The
    hand-written packages are versioned and released one at a time, so the
    major is the only thing they have to agree on and the only thing checked.
    """
    table_major, _ = runtime_table_format()

    for path in version_files():
        package_major = int(read_version(path).split(".")[0])

        if package_major != table_major:
            raise RuntimeError(
                f"{path.parent.name} is version major {package_major}"
                f" and {TABLE_HEADER_PATH.name} is table format major {table_major}"
            )

    return table_major


def nuget_package_versions() -> dict[str, str]:
    """
    The version of every NuGet package that scripts/fetch-tools.ps1
    downloads, keyed by package name.
    """
    with open(TOOLS_JSON_PATH) as tools_file:
        return json.load(tools_file)


def unpacked_nuget_path(package: str) -> Path:
    """
    Where scripts/fetch-tools.ps1 left a NuGet package that it unpacked.
    """
    return TOOLS_PATH / f"{package}.{nuget_package_versions()[package]}"


def windows_sdk_metadata_path() -> Path:
    """
    The directory of Windows SDK .winmd files in the unpacked
    Microsoft.Windows.SDK.CPP package, which is named after the SDK with the
    package's last number as 0: c/References/10.0.28000.0 in 10.0.28000.2705.
    """
    package = "Microsoft.Windows.SDK.CPP"
    major, minor, build, _ = nuget_package_versions()[package].split(".")
    path = (
        unpacked_nuget_path(package) / "c" / "References" / f"{major}.{minor}.{build}.0"
    )

    if not path.is_dir():
        raise RuntimeError(
            f"{path} is not a directory; run scripts/fetch-tools.ps1, or check how"
            " the package lays out its References if the pin moved"
        )

    return path


def nuget_versions() -> dict[str, str]:
    """
    The upstream version of each family's NuGet package, keyed the way
    NUGET_PACKAGES is.
    """
    tools = nuget_package_versions()

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


def family_fixes(family: str) -> dict[str, str]:
    """
    The version of a fix that each package of a generated family carries,
    keyed by distribution, for the packages that have one.

    A package that changed after its family was released at the current pin,
    such as one whose type hints the generator now writes differently, is
    released again with a number on the family's version, and only that
    package is: the rest of the family is what was released. The family's
    fixes.json says which packages those are, and scripts/fix-versions.py
    writes it. Every requirement stays on the family's version, which a fix
    satisfies, so nothing else is regenerated for one.

    A fix that does not follow the family's current version is what the pin
    left behind when it moved, and is refused until the file is deleted.
    """
    path = PROJECTION_PATH / family / FIXES_NAME

    if not path.exists():
        return {}

    fixes: dict[str, str] = json.loads(path.read_text(encoding="utf-8"))
    family_version = family_versions()[family]
    name = path.relative_to(PROJECTION_PATH.parent).as_posix()

    for distribution, version in fixes.items():
        if not (PROJECTION_PATH / family / distribution / "deps.json").exists():
            raise RuntimeError(f"{name} names {distribution}, which is not in {family}")

        try:
            check_fix_version(version, family_version)
        except ValueError as error:
            raise RuntimeError(
                f"{name} gives {distribution} the version of a fix, but {error};"
                f" delete it to release {family} with the family's version"
            ) from error

    return fixes


def fix_number(version: str, family_version: str) -> int:
    """
    The number a fix adds to its family's version, or 0 for the family's
    version itself.
    """
    if version == family_version:
        return 0

    check_fix_version(version, family_version)

    return int(version.rpartition(".")[2])


def next_fixes(
    family: str, released: Iterable[str], changed: Iterable[str]
) -> dict[str, str]:
    """
    What a family's fixes.json says once the packages that changed after its
    newest release are given the next fix.

    released is the versions of the family that have been released at the
    current pin, the family's own among them, and changed the distributions
    whose files differ from the newest of those. A fix that is written but
    not released yet is worked out again rather than added to.
    """
    family_version = family_versions()[family]
    newest = max(fix_number(version, family_version) for version in released)
    fix = f"{family_version}.{newest + 1}"
    fixes = {
        distribution: version
        for distribution, version in family_fixes(family).items()
        if fix_number(version, family_version) <= newest
    } | dict.fromkeys(changed, fix)

    return dict(sorted(fixes.items()))


def published_versions() -> dict[str, str]:
    """
    The version each release unit is published with, keyed by unit and in the
    order a release goes in.

    A unit is named after the directory its sources are in: a family of
    generated packages under projection/, or one package that is written by
    hand. The order is what an upload can resolve - winrt-runtime first
    because everything requires it, then the compiler that builds a
    projection package, then the generated families, then the interop
    modules, each of which names a projection package.

    A family with fixes is published at its newest one, which releases the
    packages that carry it (see family_fixes()).
    """
    runtime, table, *interop = version_files()

    return (
        {path.parent.name: read_version(path) for path in (runtime, table)}
        | {
            family: max(
                [version, *family_fixes(family).values()],
                key=lambda fix: fix_number(fix, version),
            )
            for family, version in family_versions().items()
            if family not in UNPUBLISHED_FAMILIES
        }
        | {path.parent.name: read_version(path) for path in interop}
        | {
            name: interop_version(name)
            for name in sorted(FAMILY_RELEASED_INTEROP)
            if interop_release(name) == name
        }
    )


def release_tags() -> dict[str, str]:
    """
    The tag that releases each unit, keyed the way published_versions() is.
    """
    return {
        unit: release_tag(unit, version)
        for unit, version in published_versions().items()
    }


def release_tag(unit: str, version: str) -> str:
    """
    The tag that releases a unit at a version.
    """
    epoch, _, release = version.rpartition("!")
    spelled = f"e{epoch}v{release}" if epoch else f"v{release}"

    return f"{TAG_PREFIX}/{unit}/{spelled}"


def unit_of_tag(tag: str) -> str:
    """
    The release unit that a tag publishes.

    The version in the tag is matched against what this tree publishes rather
    than parsed out of it, so a tag pushed at the wrong commit - or written
    with the version of the family it was copied from - fails the release
    instead of publishing something nobody asked for.
    """
    units = {name: unit for unit, name in release_tags().items()}

    if tag not in units:
        raise ValueError(
            f"{tag} releases nothing this tree publishes;"
            f" the tags it publishes are {', '.join(sorted(units))}"
        )

    return units[tag]


def check_census() -> None:
    """
    Checks that the call shape census is the one of this generation, and that
    the runtime of this tree is built from every revision of it.

    A table needs a runtime built from the revision of the census it names or
    a later one, and shapes.json records the first winrt-runtime release of
    each revision. PyWinRT.exe cannot know that release when it appends a
    revision, so it leaves it out, and no package is generated until a person
    names it: a package floors its runtime requirement at the release of the
    revision its tables need, among others, so every revision has to name
    one, and none can be later than the runtime of this tree, which would be a
    release that has not happened.
    """
    generation = compatibility_generation()
    shapes = "runtime/src/shapes.json"

    with open(SHAPES_PATH, encoding="utf-8") as census_file:
        census = json.load(census_file)

    if census["lineage"] != CENSUS_LINEAGES[generation]:
        raise RuntimeError(
            f"{shapes} is census {census['lineage']}, and every package of"
            f" generation {generation} was generated against census"
            f" {CENSUS_LINEAGES[generation]}, which the runtime would refuse them"
            " for; restore it from git"
        )

    # the runtime is built from the header, so it is the header that has to
    # be this census
    header = SHAPES_HEADER_PATH.read_text(encoding="utf-8")
    built_lineage = re.search(r'census_lineage\[\] = "([^"]*)";', header)
    built_revision = re.search(r"census_revision = (\d+);", header)

    if (
        built_lineage and built_lineage[1],
        built_revision and int(built_revision[1]),
    ) != (
        census["lineage"],
        census["revision"],
    ):
        raise RuntimeError(
            f"{SHAPES_HEADER_PATH.name} was not written from {shapes} as it"
            " stands; generate the projection again"
        )

    current = runtime_version()
    version_txt = "runtime/version.txt"
    previous = f"{generation}.0.0"

    for revision, entry in enumerate(census["revisions"], start=1):
        runtime = entry.get("runtime")

        if runtime is None:
            raise RuntimeError(
                f"revision {revision} of the call shape census is in no winrt-runtime"
                f" release yet; set its runtime in {shapes} to the release that will"
                f" ship it, and {version_txt} to at least that"
            )

        if release_of(runtime)[0] != generation:
            raise RuntimeError(
                f"{shapes} says winrt-runtime {runtime} ships revision {revision},"
                f" which is not of generation {generation}"
            )

        if release_of(runtime) < release_of(previous):
            raise RuntimeError(
                f"{shapes} says winrt-runtime {runtime} ships revision {revision},"
                f" but an earlier revision is in {previous}"
            )

        if release_of(runtime) > release_of(current):
            raise RuntimeError(
                f"{shapes} says winrt-runtime {runtime} ships revision {revision},"
                f" but the runtime of this tree is {current}; set {version_txt} to"
                " at least that"
            )

        previous = runtime


def runtime_requirement(floor: str) -> str:
    """
    What a projection package requires of the runtime, given the first
    release that has everything it needs of it.

    PyWinRT.exe writes that release into the package's deps.json: the newest
    of the ones that first read its table format, first have the census
    revision its tables need, and first have the runtime names its modules
    use. Flooring there rather than at the runtime of this tree means a fix
    to the runtime does not raise every package's requirement, as
    interop_runtime_requirement() does for an interop package. The census
    part is a release only because check_census() makes shapes.json name one,
    and a floor above the runtime of this tree is a release that has not
    happened, so both are refused.

    The cap and the ~= between the Windows App SDK components, which
    Microsoft releases as one set, are the only upper bounds in PyWinRT's
    metadata. The cap is there because the incompatibility behind it is known
    rather than guessed: a package of one compatibility generation is refused
    by the runtime of the next, so without the cap pip would upgrade the
    runtime on its own into an import error instead of moving it and the
    packages together.
    """
    check_census()

    generation = compatibility_generation()
    current = runtime_version()

    if release_of(floor)[0] != generation:
        raise RuntimeError(f"winrt-runtime {floor} is not of generation {generation}")

    if release_of(floor) > release_of(current):
        raise RuntimeError(
            f"winrt-runtime {floor} is later than the runtime of this tree, {current}"
        )

    return f"winrt-runtime>={floor},<{generation + 1}"


def family_requirement(package: str, version: str, pinned: bool) -> str:
    """
    A requirement on another package that this one's types name, at the
    version of its family.

    A family published as NuGet components is released as a set - Microsoft's
    own build refuses a project that mixes versions of them - so a component
    pins the others of its family to its own release. ~= says so, and unlike
    the == that #137 rejected, it still admits a .postN of either one. ~=
    cannot follow a prerelease with another number, so a prerelease is pinned
    as the range from it to the next prerelease of its phase, which admits the
    same.

    Every other requirement is a floor. A package of another family follows
    its own upstream on its own schedule, and within the Windows SDK a newer
    package of a base class serves an older derived one, which is what such a
    requirement is. A package written by hand takes the floor even on its own
    family: it carries a version of its own and is released when it changes.
    """
    if not pinned:
        return f"{package}>={version}"

    prerelease = re.fullmatch(r"(.*(?:a|b|rc))(\d+)", version)

    if prerelease:
        later = f"{prerelease[1]}{int(prerelease[2]) + 1}"

        return f"{package}>={version},<{later}"

    return f"{package}~={version}.0"


def table_compiler_requirement() -> str:
    """
    What a projection package requires of the compiler that builds it.

    A source distribution compiles its own tables, so a compiler of the next
    generation would write a wheel whose table the runtime that same wheel
    requires cannot read. That is the same known incompatibility the runtime
    requirement is capped for, so it is capped the same way; being a build
    requirement, it never reaches a wheel.

    The floor is the generation rather than this tree's runtime version,
    because what the compiler has to be able to do is write the table format,
    and the format does not move when the runtime is patched. Flooring it at
    the runtime version would mean that a runtime fix could not be released
    on its own: a package regenerated afterwards would ask for a compiler at
    the new version, so a winrt-table-compiler with nothing in it changed
    would have to be published to satisfy it. What the floor gives up is a
    compiler that is of this generation but too old for the format the table
    is written in, and that one refuses the table and says so rather than
    writing something wrong.
    """
    generation = compatibility_generation()

    return f"winrt-table-compiler>={generation}.0.0,<{generation + 1}"


def runtime_functions_called(package_path: Path) -> set[str]:
    """
    The runtime functions an interop package calls.

    Its Python code is read for how it imports one of RUNTIME_MODULES and what
    it takes from it, and interop.h is read for the calls it makes from C,
    since every interop module compiles a copy of it.
    """
    called = set(
        re.findall(
            r'PyObject_CallMethod\(\s*runtime,\s*"(\w+)"',
            INTEROP_HEADER_PATH.read_text(encoding="utf-8"),
        )
    )

    for path in package_path.glob("winrt/**/*.py"):
        tree = ast.parse(path.read_text(encoding="utf-8"), filename=str(path))
        aliases: set[str] = set()

        for node in ast.walk(tree):
            if isinstance(node, ast.Import):
                # without an alias, "import winrt.runtime.interop" is reached
                # as winrt.runtime.interop.name, which the loop below looks for
                aliases.update(
                    alias.asname
                    for alias in node.names
                    if alias.asname and alias.name in RUNTIME_MODULES
                )
            elif isinstance(node, ast.ImportFrom) and node.module in RUNTIME_MODULES:
                called.update(alias.name for alias in node.names)
            elif isinstance(node, ast.ImportFrom):
                aliases.update(
                    alias.asname or alias.name
                    for alias in node.names
                    if f"{node.module}.{alias.name}" in RUNTIME_MODULES
                )

        for node in ast.walk(tree):
            if not isinstance(node, ast.Attribute):
                continue

            owner = dotted_name(node.value)

            if owner in aliases or owner in RUNTIME_MODULES:
                called.add(node.attr)

    return called


def dotted_name(node: ast.expr) -> str | None:
    """
    The dotted name that an expression such as ``winrt.runtime.interop``
    spells, or None for an expression that is not one.
    """
    if isinstance(node, ast.Name):
        return node.id

    if isinstance(node, ast.Attribute):
        owner = dotted_name(node.value)

        return None if owner is None else f"{owner}.{node.attr}"

    return None


def release_of(version: str) -> tuple[int, ...]:
    """
    A version of the form the hand-written packages carry, as something that
    compares the way the versions do.
    """
    if not re.fullmatch(r"[0-9]+(\.[0-9]+)*", version):
        raise ValueError(
            f"{version!r} is not a release of the form the hand-written packages"
            " carry, numbers joined by dots such as 4.0.0"
        )

    return tuple(int(part) for part in version.split("."))


def interop_runtime_requirement(functions: Iterable[str]) -> str:
    """
    What an interop package that calls these functions requires of the runtime.

    The floor is the newest runtime that one of them first shipped in, and
    never below the generation, so a fix to an interop package that calls
    nothing new can be installed without upgrading the runtime. The cap is the
    one runtime_requirement() explains.

    A function that is not in RUNTIME_FUNCTIONS stops the generation rather
    than being assumed old, and so does one listed with a version this tree's
    runtime has not reached, which is a release that has not happened yet.
    """
    generation = compatibility_generation()
    current = runtime_version()
    floor = f"{generation}.0.0"

    for function in sorted(functions):
        if function not in RUNTIME_FUNCTIONS:
            raise RuntimeError(
                f"winrt.runtime.interop.{function} is not in RUNTIME_FUNCTIONS in"
                " scripts/versions.py; add it with the winrt-runtime version"
                " it first ships in"
            )

        since = RUNTIME_FUNCTIONS[function]

        if release_of(since) > release_of(current):
            raise RuntimeError(
                f"winrt.runtime.interop.{function} is listed as shipping in"
                f" winrt-runtime {since}, but the runtime of this tree is {current}"
            )

        if release_of(since) > release_of(floor):
            floor = since

    return f"winrt-runtime>={floor},<{generation + 1}"
