"""
Writes the tables that inspect_source.py reads.

methods.csv maps each method name that v3.2.1 published and v4 does not to
the name v4 gives the method. The generator knows the mapping: with
--legacy-method-aliases it writes an alias_method() call for each old name it
can keep, and a warning for each one that now names another method. So this
runs it over every family into a scratch directory, collects both, and keeps
the names that v3.2.1's stubs really had. An old name for a method that is
newer than v3.2.1 is no name anybody's code uses.

values.csv lists the properties that hand back an HResult, whose .value is
deprecated.

distributions.csv says what each distribution v3.2.1 published is in v4: the
same name, the component that carries its namespace now, or nothing, and the
floor to require it at. A namespace is found in the "namespaces" of each v4
deps.json, within its top-level package, since WinUI 2 and the App SDK define
the same Microsoft.UI.Xaml namespaces. The floor is the version in the tree,
so the table is written from the release commit of 4.0.0 and then left alone:
any later v4 release satisfies it.

It also reports each v3.2.1 method that v4 has under neither its old name nor
one in the table, which is the "Removed" list of README.md to check.

Run it from the repository root after `dotnet build PyWinRT -c Release` and
scripts/fetch-tools.ps1, with the v3.2.1 tag fetched.
"""

import ast
import csv
import io
import json
import keyword
import re
import shutil
import subprocess
import sys
import tarfile
import tomllib
from collections.abc import Iterable, Sequence
from pathlib import Path

SCRIPT_PATH = Path(__file__).parent.resolve()
REPO_PATH = SCRIPT_PATH.parent.parent

sys.path.insert(0, str(REPO_PATH / "scripts"))

import app_sdk  # type: ignore[import-not-found]  # noqa: E402

V3_TAG = "v3.2.1"

PYWINRT_EXE = REPO_PATH / "PyWinRT" / "bin" / "Release" / "net10.0" / "PyWinRT.exe"
SHAPES_JSON = REPO_PATH / "runtime" / "src" / "shapes.json"
OUTPUT_PATH = REPO_PATH / "_build" / "3to4"

with open(REPO_PATH / ".config" / "_tools.json") as f:
    TOOLS = json.load(f)


def tool_path(package: str) -> Path:
    return REPO_PATH / "_tools" / f"{package}.{TOOLS[package]}"


WINDOWS_SDK = (
    tool_path("Microsoft.Windows.SDK.CPP") / "c" / "References" / "10.0.28000.0"
)
WEBVIEW2 = (
    tool_path("Microsoft.Web.WebView2") / "lib" / "Microsoft.Web.WebView2.Core.winmd"
)
WINUI2 = tool_path("Microsoft.UI.Xaml") / "lib" / "uap10.0"

# The families that pywinrt v3.2.1 published, as scripts/generate-pywinrt.py
# generates them: the top-level Python package, what is projected and what is
# only referenced.
FAMILIES = {
    "winrt": ("winrt", [f"winrt;{WINDOWS_SDK}"], []),
    "webview2": (
        "winrt",
        [f"winrt;Microsoft.Web.WebView2;{WEBVIEW2}"],
        [f"winrt;{WINDOWS_SDK}"],
    ),
    "winui2": (
        "winui2",
        [f"winui2;Microsoft.UI.Xaml;{WINUI2}"],
        [f"winrt;Microsoft.Web.WebView2;{WEBVIEW2}", f"winrt;{WINDOWS_SDK}"],
    ),
    "wasdk": (
        "winrt",
        [f"winrt;{name};{path}" for name, path in app_sdk.metadata_inputs()],
        [f"winrt;Microsoft.Web.WebView2;{WEBVIEW2}", f"winrt;{WINDOWS_SDK}"],
    ),
}

# The top-level packages of v3.2.1 and what they are in v4.
V3_PACKAGES = {
    "winrt": "winrt",
    "winui3": "winrt",
    "webview2": "winrt",
    "winui2": "winui2",
}

ALIAS_CALL = re.compile(
    r'alias_(?:static_)?method\((\w+), "(\w+)", "(\w+)"\)',
)

IMPLEMENTATION = re.compile(r"_I[A-Z]\w*")

REUSED_WARNING = re.compile(
    r"warning: ([\w.`]+): .* can no longer be called as (\w+)\(\), use (\w+)\(\) instead"
)


def module_name(package_path: Path, init_path: Path) -> str:
    """
    The module that @p init_path, an __init__.py or __init__.pyi in the
    distribution directory @p package_path, is the source of.
    """
    return ".".join(init_path.relative_to(package_path).parent.parts)


def namespace_module(prefix: str, namespace: str) -> str:
    """
    The module that @p namespace is projected as, under the top-level
    package @p prefix.
    """
    parts = [part.lower() for part in namespace.split(".")]
    return ".".join([prefix] + [f"{p}_" if keyword.iskeyword(p) else p for p in parts])


def split_full_name(prefix: str, full_name: str) -> tuple[str, str]:
    namespace, _, name = full_name.rpartition(".")
    return namespace_module(prefix, namespace), name.partition("`")[0]


def generate(family: str) -> set[tuple[str, str, str, str, str]]:
    """
    Runs the generator over @p family with the old method names on, and
    returns (module, type, old name, new name, reused) for each of them.
    """
    prefix, inputs, references = FAMILIES[family]
    output = OUTPUT_PATH / family

    shutil.rmtree(output, ignore_errors=True)

    args: list[str | Path] = [PYWINRT_EXE]

    for spec in inputs:
        args += ["--input", spec]

    for spec in references:
        args += ["--reference", spec]

    args += ["--shapes", SHAPES_JSON, "--output", output, "--legacy-method-aliases"]

    print(f"Generating {family}", file=sys.stderr)

    result = subprocess.run(args, capture_output=True, text=True, check=True)

    rows = set()

    for init_path in output.glob("*/**/__init__.py"):
        module = module_name(output / init_path.relative_to(output).parts[0], init_path)

        for match in ALIAS_CALL.finditer(init_path.read_text()):
            type_name, old_name, new_name = match.groups()
            # an interface's methods are aliased on the class that implements
            # it, which is private; the stubs name the interface
            if IMPLEMENTATION.fullmatch(type_name):
                type_name = type_name[1:]

            rows.add((module, type_name, old_name, new_name, "no"))

    for match in REUSED_WARNING.finditer(result.stderr):
        full_name, old_name, new_name = match.groups()
        rows.add((*split_full_name(prefix, full_name), old_name, new_name, "yes"))

    return rows


def is_property(function: ast.FunctionDef) -> bool:
    for decorator in function.decorator_list:
        # the stubs import the builtin as _property
        if isinstance(decorator, ast.Name) and decorator.id in (
            "property",
            "_property",
        ):
            return True

        if isinstance(decorator, ast.Attribute) and decorator.attr == "setter":
            return True

    return False


def methods(stub: ast.Module) -> dict[str, set[str]]:
    """
    The method names of each class in @p stub, with those of its _Static
    metaclass counted as its own.
    """
    result: dict[str, set[str]] = {}

    for node in stub.body:
        if not isinstance(node, ast.ClassDef):
            continue

        names = result.setdefault(node.name.removesuffix("_Static"), set())

        for member in node.body:
            if not isinstance(member, ast.FunctionDef):
                continue

            if member.name.startswith("__") or is_property(member):
                continue

            names.add(member.name)

    return result


def v3_projection() -> tarfile.TarFile:
    """
    The projection directory of v3.2.1.
    """
    archive = subprocess.run(
        ["git", "archive", V3_TAG, "--", "projection"],
        cwd=REPO_PATH,
        capture_output=True,
        check=True,
    ).stdout

    return tarfile.open(fileobj=io.BytesIO(archive))


def read_member(tar: tarfile.TarFile, member: tarfile.TarInfo) -> bytes:
    file = tar.extractfile(member)
    assert file is not None
    return file.read()


def v3_methods(tar: tarfile.TarFile) -> dict[tuple[str, str], set[str]]:
    """
    The methods of each (module, type) in v3.2.1, with the module named the
    way v4 names it.
    """
    result: dict[tuple[str, str], set[str]] = {}

    for member in tar.getmembers():
        # projection/<family>/<package>-<Namespace>/<package>/_<package>_<ns>.pyi
        parts = member.name.split("/")

        if (
            len(parts) != 5
            or not parts[4].startswith("_")
            or not parts[4].endswith(".pyi")
        ):
            continue

        package, _, namespace = parts[2].partition("-")

        # the interop modules are written by hand and have no aliases
        if parts[1] == "interop":
            continue

        if parts[3] != package or package not in V3_PACKAGES:
            continue

        module = namespace_module(V3_PACKAGES[package], namespace)

        for type_name, names in methods(ast.parse(read_member(tar, member))).items():
            result.setdefault((module, type_name), set()).update(names)

    return result


def v3_distributions(tar: tarfile.TarFile) -> list[str]:
    """
    The name of each distribution that v3.2.1 published.
    """
    result = []

    for member in tar.getmembers():
        if not member.name.endswith("/pyproject.toml"):
            continue

        name = tomllib.loads(read_member(tar, member).decode())["project"]["name"]

        # the test component is never published
        if not name.startswith("test-winrt"):
            result.append(name)

    return sorted(result)


def v4_distributions() -> tuple[dict[str, str], dict[tuple[str, str], str]]:
    """
    The version of each distribution in the tree, and the distribution that
    carries each (top-level package, namespace).
    """
    paths = subprocess.run(
        ["git", "ls-files", "*pyproject.toml"],
        cwd=REPO_PATH,
        capture_output=True,
        text=True,
        check=True,
    ).stdout.split()

    versions = {}
    owners = {}

    for path in paths:
        directory = (REPO_PATH / path).parent
        project = tomllib.loads((directory / "pyproject.toml").read_text())
        name = project["project"]["name"]

        if "version" in project["project"]:
            version = project["project"]["version"]
        else:
            version_file = project["tool"]["setuptools"]["dynamic"]["version"]["file"]
            version = (directory / version_file).read_text().strip()

        versions[name] = version

        deps_path = directory / "deps.json"

        if deps_path.exists():
            package = "winui2" if path.startswith("projection/winui2/") else "winrt"

            for namespace in json.loads(deps_path.read_text())["namespaces"]:
                owners[(package, namespace)] = name

    return versions, owners


def distribution_rows(v3_names: list[str]) -> list[tuple[str, str, str]]:
    """
    (v3 distribution, v4 distribution, floor) for each of @p v3_names, with
    the last two empty for a distribution that v4 does not have.
    """
    versions, owners = v4_distributions()
    rows = []

    for name in v3_names:
        new_name = name

        if new_name not in versions:
            prefix, _, namespace = name.partition("-")

            if prefix in ("winui3", "webview2"):
                # the hand-written modules only changed their prefix
                new_name = f"winrt-{namespace}"

            # a winrt- name that is not in the tree any more is gone: the
            # namespace of winrt-Microsoft.UI.Xaml is in the App SDK, but in
            # v3.2.1 that was the package of its C++ headers
            if new_name not in versions and prefix != "winrt":
                new_name = owners.get((V3_PACKAGES[prefix], namespace), "")

        if new_name not in versions:
            new_name = ""

        floor = f">={versions[new_name]}" if new_name else ""
        rows.append((name, new_name, floor))

    return rows


def v4_stubs() -> dict[str, ast.Module]:
    """
    The committed v4 stub of each module.
    """
    result = {}

    for family in FAMILIES:
        family_path = REPO_PATH / "projection" / family

        for stub_path in family_path.glob("*/**/__init__.pyi"):
            package_path = family_path / stub_path.relative_to(family_path).parts[0]
            result[module_name(package_path, stub_path)] = ast.parse(
                stub_path.read_bytes()
            )

    return result


def v4_methods(stubs: dict[str, ast.Module]) -> dict[tuple[str, str], set[str]]:
    """
    The methods of each (module, type) in @p stubs, including the deprecated
    old names that the Windows SDK keeps.
    """
    return {
        (module, type_name): names
        for module, stub in stubs.items()
        for type_name, names in methods(stub).items()
    }


def hresult_properties(stubs: dict[str, ast.Module]) -> set[tuple[str, str, str]]:
    """
    (module, type, property) for each property in @p stubs that hands back an
    HResult.
    """
    result = set()

    for module, stub in stubs.items():
        for node in stub.body:
            if not isinstance(node, ast.ClassDef):
                continue

            for member in node.body:
                if not isinstance(member, ast.FunctionDef):
                    continue

                if not is_property(member) or member.returns is None:
                    continue

                if ast.unparse(member.returns).rpartition(".")[2] == "HResult":
                    result.add((module, node.name, member.name))

    return result


def write_csv(path: Path, header: list[str], rows: Iterable[Sequence[str]]) -> None:
    with open(path, "w", newline="") as f:
        writer = csv.writer(f, dialect="unix", quoting=csv.QUOTE_MINIMAL)
        writer.writerow(header)
        writer.writerows(rows)


def main() -> None:
    generated: set[tuple[str, str, str, str, str]] = set()

    for family in FAMILIES:
        generated |= generate(family)

    stubs = v4_stubs()

    with v3_projection() as tar:
        before = v3_methods(tar)
        v3_names = v3_distributions(tar)

    after = v4_methods(stubs)

    table = sorted(
        row for row in generated if row[2] in before.get((row[0], row[1]), set())
    )

    print(
        f"{len(table)} renamed methods, {len(generated) - len(table)} old names "
        "that v3.2.1 did not have",
        file=sys.stderr,
    )

    write_csv(
        SCRIPT_PATH / "methods.csv",
        ["module", "type", "method", "new_name", "old_name_reused"],
        table,
    )

    write_csv(
        SCRIPT_PATH / "values.csv",
        ["module", "type", "property"],
        sorted(hresult_properties(stubs)),
    )

    distributions = distribution_rows(v3_names)

    write_csv(
        SCRIPT_PATH / "distributions.csv",
        ["distribution", "new_distribution", "floor"],
        distributions,
    )

    for name, new_name, _ in distributions:
        if not new_name:
            print(f"distribution not in v4: {name}")

    renamed = {(module, type_name, old) for module, type_name, old, *_ in table}
    old_names = {old for _, _, old, *_ in table}

    for (module, type_name), names in sorted(before.items()):
        if (module, type_name) not in after:
            print(f"type not in v4: {module}.{type_name}")
            continue

        for name in sorted(names - after[(module, type_name)]):
            if (module, type_name, name) in renamed:
                continue

            # inspect_source.py matches on the name alone, so it still finds
            # one that the table has for another type
            found = " (in the table for another type)" if name in old_names else ""
            print(f"not in v4: {module}.{type_name}.{name}{found}")


if __name__ == "__main__":
    main()
