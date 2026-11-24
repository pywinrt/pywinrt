import argparse
import os
import re
import shutil
import subprocess
import sys
import textwrap
from dataclasses import dataclass
from pathlib import Path

DESCRIPTION = """
Checks the compatibility promise between two versions of the tree.

The promise is that a projection table is read by any winrt-runtime of its
format major whose format minor is no older than the table's, and that a
runtime which cannot read it refuses it with an ImportError rather than
crashing. Nothing in the ordinary test suite can check that, because it builds
both halves from the same source at the same time. This script takes the table
from an older commit and pairs it with the current runtime.

It needs the current tree already built and installed with CMake, the way
test.yaml builds it:

    cmake -S . -B _build/projection
    cmake --build _build/projection --config Release
    cmake --install _build/projection --prefix _install/projection

    py scripts/abi-compat.py

The baseline half comes from a git worktree of an older ref, which has to be
4.0 or newer, since nothing older has a table. Its TestComponent table,
compiled by the baseline's own winrt.table, goes on PYTHONPATH in front of the
current install, and the current runtime reads it. ``test_winrt`` is a
namespace package, so the baseline's module shadows the current one. Nothing
is built here: a projection package is data.

What it expects is computed from the two versions rather than written down, so
that the same script keeps working as the format moves: it either runs the
tests and requires them to pass, or requires a refusal that names the
mismatch.

The interop modules of the current tree share no C ABI with the runtime, so
nothing compiled in it is checked against an older runtime.
"""

PROJECT_DIR = Path(__file__).parent.parent

# The package whose table is paired with the current runtime, relative to the
# root of a tree, and the table, relative to the package.
TEST_COMPONENT_PACKAGE = Path("projection/test-winrt/test-winrt-TestComponent")
TEST_COMPONENT_TABLE = Path("test_winrt/testcomponent/_table.pywinrt.txt")

# TestComponent's own tests, in a tree whose tests are grouped by what they
# need: the modules of test/projection that test_test_component.py was split
# into. A tree older than the grouping has test/test_test_component.py alone.
TEST_COMPONENT_TESTS = (
    "test_test_component",
    "test_composable",
    "test_async",
    "test_async_await",
)

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


def newest_runtime_tag() -> str:
    """
    The newest release tag that names a runtime to compare against.

    A release is per family, so only the runtime family's tags carry one.
    """
    tags = git("tag", "--list", "wheels/runtime/*", "--sort=-creatordate").splitlines()

    if not tags:
        raise SystemExit(
            "no wheels/runtime/* tag to use as a baseline; pass --baseline"
        )

    return tags[0]


@dataclass
class Tree:
    """
    One version of the repository: either the working tree or a worktree of an
    older ref.
    """

    root: Path
    name: str

    @property
    def test_component_package(self) -> Path:
        return self.root / TEST_COMPONENT_PACKAGE

    @property
    def table_header(self) -> Path:
        return self.root / "runtime/src/table.h"

    @property
    def table_format(self) -> tuple[int, int]:
        """
        The table format this tree's runtime reads.
        """
        source = self.table_header.read_text(encoding="utf-8")

        def constant(name: str) -> int:
            match = re.search(rf"\b{name}\s*=\s*(\d+)", source)

            if not match:
                raise SystemExit(f"could not read {name} from {self.table_header}")

            return int(match.group(1))

        return constant("format_major"), constant("format_minor")

    @property
    def test_component_table(self) -> Path:
        return self.test_component_package / TEST_COMPONENT_TABLE

    @property
    def test_component_tests(self) -> list[str]:
        """
        This tree's tests of TestComponent, as the module names unittest takes.
        A module that the tree does not have yet is left out, so any commit
        from 4.0 on can be the baseline. A tree with none of them is refused,
        since unittest given no module names discovers tests instead.
        """
        projection = self.root / "test/projection"

        if not projection.is_dir():
            if not (self.root / "test/test_test_component.py").is_file():
                raise SystemExit(
                    f"{self.name} at {self.root} has none of TestComponent's"
                    " tests: no test/projection and no"
                    " test/test_test_component.py"
                )

            return ["test.test_test_component"]

        tests = [
            f"test.projection.{name}"
            for name in TEST_COMPONENT_TESTS
            if (projection / f"{name}.py").is_file()
        ]

        if not tests:
            raise SystemExit(
                f"{self.name} at {self.root} has none of TestComponent's tests:"
                f" no {', '.join(f'{name}.py' for name in TEST_COMPONENT_TESTS)}"
                " in test/projection"
            )

        return tests

    @property
    def table_compiler_pythonpath(self) -> str:
        """
        What this tree's winrt.table needs on PYTHONPATH: the directory of
        winrt-table-compiler.
        """
        return os.fspath(self.root / "table")


def describe(tree: Tree) -> str:
    major, minor = tree.table_format

    return f"{tree.name}: table format {major}.{minor}"


def add_worktree(ref: str, path: Path) -> Tree:
    """
    Checks the baseline out into its own worktree, reusing one that is already
    at the right commit. A baseline checkout is thousands of generated headers,
    so re-creating it on every run makes the script unpleasant to iterate on.
    """
    commit = git("rev-parse", f"{ref}^{{commit}}")

    # Nothing older than 4.0 has a table, and so nothing that this checks.
    table = (TEST_COMPONENT_PACKAGE / TEST_COMPONENT_TABLE).as_posix()
    has_table = subprocess.run(
        ["git", "cat-file", "-e", f"{commit}:{table}"],
        cwd=PROJECT_DIR,
        capture_output=True,
        check=False,
    )

    if has_table.returncode != 0:
        raise SystemExit(
            f"{ref} has no projection table, so it cannot be the baseline: it"
            " has to be 4.0 or newer"
        )

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


def is_satisfied(required: tuple[int, int], provided: tuple[int, int]) -> bool:
    """
    Whether something requiring the first version is accepted by a runtime
    providing the second: the same major and a minor no newer, which is the
    table format's rule.
    """
    return required[0] == provided[0] and required[1] <= provided[1]


def table_refusal_message(required: tuple[int, int], provided: tuple[int, int]) -> str:
    return TABLE_MAJOR_REFUSAL if required[0] != provided[0] else TABLE_MINOR_REFUSAL


def table_format_of(table: Path) -> tuple[int, int]:
    """
    The format a table's text form is written to, from the header line that
    says so.
    """
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
        raise SystemExit("check failed: no clean refusal")


def run_tests(tests: list[str], *, pythonpath: str, cwd: Path) -> None:
    run(
        [sys.executable, "-X", "dev", "-m", "unittest", "-v", *tests],
        cwd=cwd,
        env=dict(os.environ, PYTHONPATH=pythonpath),
    )


def check_old_table(
    baseline: Tree, current: Tree, current_install: Path, work: Path, tests: list[str]
) -> None:
    """
    Old table, new runtime, running the baseline's @p tests. The baseline's
    TestComponent package comes first on PYTHONPATH, so it shadows the one in
    the current install. Everything else, including winrt._winrt, comes from
    the current install.

    Nothing is built here. A projection package is a table and an __init__.py,
    so the old half is a copy of the baseline's package with its table
    compiled by the baseline's own winrt.table - the writer that wrote the
    text, paired with the reader being checked.
    """
    print()
    print("=== old table, new runtime ===", flush=True)

    required = table_format_of(baseline.test_component_table)
    provided = current.table_format

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
        env=dict(os.environ, PYTHONPATH=baseline.table_compiler_pythonpath),
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
        run_tests(tests, pythonpath=pythonpath, cwd=baseline.root)
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


def main() -> None:
    parser = argparse.ArgumentParser(
        description=DESCRIPTION, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--baseline",
        metavar="REF",
        help="the git ref, 4.0 or newer, to take the old half from (default: the"
        " newest release tag that names a runtime)",
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
        help="where the worktree and the compiled table go (default: %(default)s)",
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

    baseline_ref = args.baseline or newest_runtime_tag()
    work = args.work_dir.resolve()

    current = Tree(PROJECT_DIR, "current")
    baseline = add_worktree(baseline_ref, work / "baseline")

    try:
        # read first, so that a refused tree stops before anything is copied
        # or compiled
        tests = baseline.test_component_tests

        print()
        print(describe(current))
        print(describe(baseline))
        print(flush=True)

        check_old_table(baseline, current, current_install, work, tests)
    finally:
        if args.remove_worktree:
            run(["git", "worktree", "remove", "--force", os.fspath(baseline.root)])

    print()
    print("passed", flush=True)


if __name__ == "__main__":
    main()
