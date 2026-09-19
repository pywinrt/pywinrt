"""
Checks the runtime ABI promise between two versions of the tree.

The promise is that a projection module compiled against runtime ABI major M
and minor m loads on any winrt-runtime that declares major M and a minor no
older than m, and that a runtime which cannot satisfy it refuses the module
with a RuntimeError rather than crashing. Nothing in the ordinary test suite
can check that, because it builds both halves from the same source at the same
time; this script builds one half from an older commit and pairs the two.

It needs the current tree already built and installed with CMake, the way
test.yaml builds it:

    cmake -S projection -B _build/projection -DPYWINRT_FULL_PROJECTION=OFF
    cmake --build _build/projection --config Release
    cmake --install _build/projection --prefix _install/projection

    py scripts/abi-compat.py

The baseline half is built from a git worktree of an older ref with
``pip wheel``, and the resulting wheels are unpacked rather than installed, so
that each scenario can put the two halves on PYTHONPATH in the order it wants.
Both ``winrt`` and ``test_winrt`` are namespace packages, so when the same
module exists in both halves the earlier entry on PYTHONPATH wins.

There are two contracts and one rule each: a compiled consumer against the C
ABI in ``pywinrt/abi.h``, and a projection table against the table format in
``runtime/src/table-format.md``. The three scenarios cover both.

  A. old table, new runtime - the baseline's TestComponent table, compiled by
     the baseline's own table.py, read by the current runtime. Nothing is
     compiled here: a projection package is data.
  B. new module, old runtime - the current TestComponent against the baseline
     runtime.
  C. old headers, new hand-written code - a compile-only build of an interop
     package against the baseline's copy of the pywinrt headers.

What each one expects is computed from the two versions rather than written
down, so that the same script keeps working as either contract moves: A and B
either run the tests and require them to pass, or require a refusal that names
the mismatch, and A and C are skipped when the baseline predates what they
check.
"""

import argparse
import os
import re
import shutil
import subprocess
import sys
import textwrap
import zipfile
from dataclasses import dataclass
from pathlib import Path

PROJECT_DIR = Path(__file__).parent.parent

# The message py::import_winrt_runtime() raises through when the runtime cannot
# satisfy the module. Both halves of the pair have to agree on the wording for
# this to be checkable, which they have since the check was introduced.
MAJOR_REFUSAL = "ABI major version mismatch"
MINOR_REFUSAL = "ABI minor version mismatch"

# The messages table.cpp raises through when a runtime will not read a table.
# Both halves have to agree on the wording for this to be checkable, which they
# have since the table format was introduced.
TABLE_MAJOR_REFUSAL = "cannot be read by this runtime"
TABLE_MINOR_REFUSAL = "is newer than this runtime"


def run(
    args: list[str], *, cwd: Path | None = None, env: dict[str, str] | None = None
) -> None:
    print(f"+ {subprocess.list2cmdline(args)}", flush=True)
    subprocess.check_call(args, cwd=cwd, env=env)


def git(*args: str) -> str:
    return subprocess.check_output(
        ["git", *args], cwd=PROJECT_DIR, text=True, encoding="utf-8"
    ).strip()


def newest_wheels_tag() -> str:
    tags = git("tag", "--list", "wheels/*", "--sort=-creatordate").splitlines()

    if not tags:
        raise SystemExit("no wheels/* tag to use as a baseline; pass --baseline")

    return tags[0]


@dataclass
class Tree:
    """
    One version of the repository: either the working tree or a worktree of an
    older ref. The layout moved between 3.x and 4.0 - the runtime package was
    under projection/ and the pywinrt headers shipped in winrt-sdk - so every
    path the script needs is looked up rather than spelled once.

    Half of this exists only to find things in a 3.x tree. Once the newest
    wheels/* tag is a 4.x release, no baseline this script is ever pointed at
    has the old layout, and every property below collapses to the one branch
    that is left. Delete the other branch then rather than carrying it.
    """

    root: Path
    name: str

    @property
    def runtime_package(self) -> Path:
        new_layout = self.root / "runtime"

        return (
            new_layout
            if new_layout.is_dir()
            else self.root / "projection/winrt-runtime"
        )

    @property
    def sdk_package(self) -> Path:
        return self.root / "projection/winrt-sdk"

    @property
    def test_component_package(self) -> Path:
        return self.root / "projection/test-winrt/test-winrt-TestComponent"

    @property
    def include_dir(self) -> Path | None:
        """
        The directory winrt._include.get_include() returns, or None for a tree
        that predates the headers moving into winrt-runtime, where they were
        part of winrt-sdk and were included as <pybase.h> instead of
        <pywinrt/...>. Nothing in the current tree can be compiled against
        those, which is what makes scenario C unavailable against such a
        baseline.
        """
        include_dir = self.runtime_package / "python/winrt/include"

        return include_dir if include_dir.is_dir() else None

    @property
    def abi_header(self) -> Path:
        if (include_dir := self.include_dir) is not None:
            return include_dir / "pywinrt/abi.h"

        return self.sdk_package / "src/winrt_sdk/pywinrt/pybase.h"

    @property
    def abi_version(self) -> tuple[int, int]:
        """
        The ABI the runtime in this tree provides, which is also the ABI a
        module built in this tree requires: the inline wrappers in the headers
        call the newest entry points, so a module carries the whole of its
        headers' minor whether the generated code uses it or not.
        """
        source = self.abi_header.read_text(encoding="utf-8")

        def constant(name: str) -> int:
            match = re.search(rf"\b{name}\s*=\s*(\d+)", source)

            if not match:
                raise SystemExit(f"could not read {name} from {self.abi_header}")

            return int(match.group(1))

        return constant("runtime_abi_version_major"), constant(
            "runtime_abi_version_minor"
        )

    @property
    def table_header(self) -> Path:
        return self.runtime_package / "src/table.h"

    @property
    def table_format(self) -> tuple[int, int] | None:
        """
        The table format this tree's runtime reads, or None for a tree that
        predates the table projection, whose packages are compiled modules
        rather than data and are not party to this contract at all.
        """
        if not self.table_header.is_file():
            return None

        source = self.table_header.read_text(encoding="utf-8")

        def constant(name: str) -> int:
            match = re.search(rf"\b{name}\s*=\s*(\d+)", source)

            if not match:
                raise SystemExit(f"could not read {name} from {self.table_header}")

            return int(match.group(1))

        return constant("format_major"), constant("format_minor")

    @property
    def test_component_table(self) -> Path:
        return (
            self.test_component_package / "test_winrt/testcomponent/_table.pywinrt.txt"
        )

    @property
    def build_pythonpath(self) -> str:
        """
        What a package in this tree needs on PYTHONPATH to be built without
        build isolation: the same directories the generated cibuildwheel
        configuration puts there. For a 4.x tree that is winrt-runtime's
        package; for a 3.x one it is winrt-sdk's, which is where the pywinrt
        headers were before they moved.
        """
        paths = (
            [self.runtime_package / "python"]
            if self.include_dir is not None
            else [self.sdk_package / "src"]
        )

        return os.pathsep.join(os.fspath(p) for p in paths)


def describe(tree: Tree) -> str:
    major, minor = tree.abi_version

    return (
        f"{tree.name}: ABI {major}.{minor} ({tree.abi_header.relative_to(tree.root)})"
    )


def add_worktree(ref: str, path: Path) -> Tree:
    """
    Checks the baseline out into its own worktree, reusing one that is already
    at the right commit. A baseline checkout is thousands of generated headers,
    so re-creating it on every run makes the script unpleasant to iterate on.
    """
    commit = git("rev-parse", f"{ref}^{{commit}}")

    if path.exists():
        existing = subprocess.run(
            ["git", "-C", os.fspath(path), "rev-parse", "HEAD"],
            capture_output=True,
            text=True,
            check=False,
        )

        if existing.returncode == 0 and existing.stdout.strip() == commit:
            print(f"reusing the worktree at {path}")
            return Tree(path, f"baseline {ref}")

        run(["git", "worktree", "remove", "--force", os.fspath(path)])

    path.parent.mkdir(parents=True, exist_ok=True)
    run(["git", "worktree", "add", "--detach", os.fspath(path), commit])

    return Tree(path, f"baseline {ref}")


def build_wheel(package: Path, out_dir: Path, pythonpath: str) -> Path:
    """
    Builds one package into a wheel, without build isolation so that the
    winrt-runtime build dependency comes from the tree being built rather than
    from PyPI. The C++/WinRT headers ride inside that same package, so
    PYTHONPATH decides which tree's headers are compiled against.
    """
    env = dict(os.environ, PYTHONPATH=pythonpath)

    # A fresh directory per package, so that the wheel that comes out is
    # unambiguous even when pip serves it from its cache and writes no new
    # file into an existing one.
    if out_dir.exists():
        shutil.rmtree(out_dir)

    run(
        [
            sys.executable,
            "-m",
            "pip",
            "wheel",
            "--no-build-isolation",
            "--no-deps",
            "--wheel-dir",
            os.fspath(out_dir),
            os.fspath(package),
        ],
        env=env,
    )

    built = sorted(out_dir.glob("*.whl"))

    if len(built) != 1:
        raise SystemExit(f"expected one wheel from {package}, got {built}")

    return built[0]


def unpack_wheel(wheel: Path, dest: Path) -> Path:
    if dest.exists():
        shutil.rmtree(dest)

    with zipfile.ZipFile(wheel) as zf:
        zf.extractall(dest)

    return dest


def is_satisfied(required: tuple[int, int], provided: tuple[int, int]) -> bool:
    """
    Whether a module requiring the first ABI version loads against a runtime
    providing the second. This is the Python-side copy of the check in
    py::import_winrt_runtime().
    """
    return required[0] == provided[0] and required[1] <= provided[1]


def refusal_message(required: tuple[int, int], provided: tuple[int, int]) -> str:
    return MAJOR_REFUSAL if required[0] != provided[0] else MINOR_REFUSAL


def table_refusal_message(required: tuple[int, int], provided: tuple[int, int]) -> str:
    return TABLE_MAJOR_REFUSAL if required[0] != provided[0] else TABLE_MINOR_REFUSAL


def table_format_of(table: Path) -> tuple[int, int] | None:
    """
    The format a table's text form is written to, from the header line that
    says so, or None for a tree that carries no such table.
    """
    if not table.is_file():
        return None

    with open(table, encoding="utf-8") as f:
        header = f.readline().split()

    if len(header) != 2 or header[0] != "format":
        raise SystemExit(f"{table} does not start with a format version")

    major, _, minor = header[1].partition(".")

    return int(major), int(minor)


def expect_refusal(
    code: str, error: str, message: str, *, pythonpath: str, cwd: Path
) -> None:
    """
    Asserts that the code refuses with the named exception type carrying the
    message. It runs in a subprocess because a refusal is the good case here
    and the bad case is a crash, which this process has to survive in order to
    report it.
    """
    script = f"""
import sys

try:
{textwrap.indent(code, " " * 4)}
except {error} as e:
    if {message!r} not in str(e):
        sys.exit(f"wrong {error}: {{e}}")
    print(f"refused as expected: {{e}}")
else:
    sys.exit("no refusal, although the two halves are not compatible")
"""

    env = dict(os.environ, PYTHONPATH=pythonpath)

    print(f"+ expecting {error} {message!r} from:")
    print(textwrap.indent(code.strip(), " " * 4), flush=True)

    result = subprocess.run(
        [sys.executable, "-c", script], cwd=cwd, env=env, check=False
    )

    if result.returncode != 0:
        raise SystemExit("scenario failed: no clean refusal")


def expect_import_refusal(
    module: str, message: str, *, pythonpath: str, cwd: Path
) -> None:
    expect_refusal(
        f"import {module}", "RuntimeError", message, pythonpath=pythonpath, cwd=cwd
    )


def run_tests(test: str, *, pythonpath: str, cwd: Path) -> None:
    run(
        [sys.executable, "-X", "dev", "-m", "unittest", "-v", test],
        cwd=cwd,
        env=dict(os.environ, PYTHONPATH=pythonpath),
    )


def scenario_a(
    baseline: Tree, current: Tree, current_install: Path, work: Path
) -> None:
    """
    Old table, new runtime. The baseline's TestComponent package comes first
    on PYTHONPATH, so it shadows the one in the current install; everything
    else, including winrt._winrt, comes from the current install.

    Nothing is built here. A projection package is a table and an __init__.py,
    so the old half is a copy of the baseline's package with its table
    compiled by the baseline's own winrt/table.py - the writer that wrote the
    text, paired with the reader being checked.
    """
    print()
    print("=== scenario A - old table, new runtime ===", flush=True)

    required = table_format_of(baseline.test_component_table)
    provided = current.table_format

    if required is None or provided is None:
        print(
            f"skipped: {baseline.name} predates the table projection, so its"
            " packages are compiled modules and say nothing about a table"
            " format",
            flush=True,
        )
        return

    module_dir = work / "baseline-testcomponent"

    if module_dir.exists():
        shutil.rmtree(module_dir)

    shutil.copytree(
        baseline.test_component_package / "test_winrt", module_dir / "test_winrt"
    )

    table = module_dir / baseline.test_component_table.relative_to(
        baseline.test_component_package
    )

    # the baseline's compiler, because the text it is reading is the baseline's
    run(
        [
            sys.executable,
            "-m",
            "winrt.table",
            os.fspath(table),
            os.fspath(table.with_suffix("")),
        ],
        env=dict(os.environ, PYTHONPATH=baseline.build_pythonpath),
    )
    table.unlink()

    # The WinRT component itself is activated registration-free, out of the
    # directory of the package that imports it, and is not part of the package.
    # It is the same build in both halves - both fetch PyWinRT.TestWinRT at the
    # version scripts/fetch-tools.ps1 pins - so the copy CMake installed is the
    # right one, and it is already the right architecture.
    shutil.copy2(
        current_install / "test_winrt/testcomponent/TestComponent.dll",
        module_dir / "test_winrt/testcomponent",
    )

    pythonpath = os.pathsep.join([os.fspath(module_dir), os.fspath(current_install)])

    if is_satisfied(required, provided):
        run_tests("test.test_test_component", pythonpath=pythonpath, cwd=baseline.root)
    else:
        # The current runtime will not read the baseline's table. That is only
        # allowed to happen across a format major, or when the table says
        # something this runtime would silently ignore, and it has to say so
        # rather than crash.
        expect_refusal(
            "import test_winrt.testcomponent",
            "ImportError",
            table_refusal_message(required, provided),
            pythonpath=pythonpath,
            cwd=baseline.root,
        )


def scenario_b(
    baseline: Tree, current: Tree, current_install: Path, work: Path
) -> None:
    """
    New module, old runtime. The baseline's runtime comes first on PYTHONPATH,
    so its winrt._winrt shadows the current one; test_winrt comes from the
    current install.
    """
    print()
    print("=== scenario B - new module, old runtime ===", flush=True)

    wheel = build_wheel(
        baseline.runtime_package,
        work / "wheels/baseline-runtime",
        baseline.build_pythonpath,
    )
    runtime_dir = unpack_wheel(wheel, work / "baseline-runtime")

    pythonpath = os.pathsep.join([os.fspath(runtime_dir), os.fspath(current_install)])
    required = current.abi_version
    provided = baseline.abi_version

    if is_satisfied(required, provided):
        run_tests("test.test_test_component", pythonpath=pythonpath, cwd=PROJECT_DIR)
    else:
        expect_import_refusal(
            "test_winrt.testcomponent",
            refusal_message(required, provided),
            pythonpath=pythonpath,
            cwd=PROJECT_DIR,
        )


def scenario_c(baseline: Tree, current: Tree, work: Path) -> None:
    """
    Old headers, new hand-written code. The interop packages are not generated,
    so nothing bumps their ABI requirement for them; this compiles one of them
    against the baseline's headers to catch hand-written code that has started
    using a newer entry point without the ABI minor being bumped.
    """
    print()
    print("=== scenario C - hand-written code against old headers ===", flush=True)

    if baseline.include_dir is None:
        print(
            f"skipped: {baseline.name} ships the pywinrt headers in winrt-sdk as"
            " <pybase.h>, which nothing in the current tree includes",
            flush=True,
        )
        return

    if baseline.abi_version[0] != current.abi_version[0]:
        print(
            "skipped: the ABI major changed, so the current sources are not"
            " meant to compile against these headers",
            flush=True,
        )
        return

    # Only the pywinrt headers come from the baseline.
    pythonpath = os.fspath(baseline.runtime_package / "python")

    package = PROJECT_DIR / "interop/winrt-Windows.System.Interop"

    build_wheel(package, work / "wheels/current-interop", pythonpath)

    # setuptools leaves its intermediates in the source tree, which is the one
    # package here that is built where it lives.
    shutil.rmtree(package / "build", ignore_errors=True)


def main() -> None:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--baseline",
        metavar="REF",
        help="the git ref to build the old half from (default: the newest"
        " wheels/* tag)",
    )
    parser.add_argument(
        "--current-install",
        metavar="DIR",
        type=Path,
        default=PROJECT_DIR / "_install/projection",
        help="the CMake install prefix of the current tree (default: %(default)s)",
    )
    parser.add_argument(
        "--work-dir",
        metavar="DIR",
        type=Path,
        default=PROJECT_DIR / "_build/abi-compat",
        help="where the worktree and the built wheels go (default: %(default)s)",
    )
    parser.add_argument(
        "--scenario",
        action="append",
        choices=["a", "b", "c"],
        help="run only this scenario; may be given more than once",
    )
    parser.add_argument(
        "--remove-worktree",
        action="store_true",
        help="remove the baseline worktree afterwards instead of leaving it for"
        " the next run to reuse",
    )
    args = parser.parse_args()

    current_install = args.current_install.resolve()

    if not (current_install / "winrt").is_dir():
        raise SystemExit(
            f"{current_install} does not look like an install of the current"
            " tree; build and install it with CMake first"
        )

    baseline_ref = args.baseline or newest_wheels_tag()
    work = args.work_dir.resolve()

    current = Tree(PROJECT_DIR, "current")
    baseline = add_worktree(baseline_ref, work / "baseline")

    print()
    print(describe(current))
    print(describe(baseline))
    print(flush=True)

    scenarios = args.scenario or ["a", "b", "c"]

    try:
        if "a" in scenarios:
            scenario_a(baseline, current, current_install, work)

        if "b" in scenarios:
            scenario_b(baseline, current, current_install, work)

        if "c" in scenarios:
            scenario_c(baseline, current, work)
    finally:
        if args.remove_worktree:
            run(["git", "worktree", "remove", "--force", os.fspath(baseline.root)])

    print()
    print("all scenarios passed", flush=True)


if __name__ == "__main__":
    main()
