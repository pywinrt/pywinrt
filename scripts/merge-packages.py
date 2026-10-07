import argparse
import pathlib
import shutil

DESCRIPTION = """
Merges the packages of this tree into _typing as they would be installed, which
is what mypy, pyright and an editor read: an interop package is a subpackage
of a projection package, so neither can be resolved from the source tree.
"""

ROOT = pathlib.Path(__file__).resolve().parent.parent
TYPING = ROOT / "_typing"

# the top-level packages the distributions of this tree install
PACKAGES = ("winrt", "winui2", "test_winrt")


def add(
    found: dict[pathlib.PurePath, pathlib.Path],
    distribution: pathlib.Path,
    patterns: tuple[str, ...],
) -> None:
    """
    Adds the files of @p distribution's top-level packages that match one of
    @p patterns to @p found, keyed by where they are installed.
    """
    for name in PACKAGES:
        package = distribution / name

        for pattern in patterns:
            for path in package.rglob(pattern):
                installed = path.relative_to(distribution)

                if installed in found:
                    raise SystemExit(
                        f"{installed} is in both {found[installed].relative_to(ROOT)} "
                        f"and {path.relative_to(ROOT)}"
                    )

                found[installed] = path


def sources() -> dict[pathlib.PurePath, pathlib.Path]:
    """
    Every file to merge, keyed by where it is installed.
    """
    found: dict[pathlib.PurePath, pathlib.Path] = {}

    # Only the stubs of a projection package: its __init__.py binds the names
    # it exports from the projection table at import time, which a type
    # checker cannot follow, and the stub beside it is what both checkers
    # read. So every .py merged is code rather than glue. The py.typed
    # markers come too: pyright holds an installed package that has one to
    # what its modules export.
    for distribution in sorted(ROOT.glob("projection/*/*")):
        add(found, distribution, ("*.pyi", "py.typed"))

    # the runtime, the table compiler and the interop packages live outside of
    # projection/, so winrt.system, winrt.runtime and winrt._winrt come from
    # runtime/python and winrt.table from table/
    for distribution in [
        *sorted(ROOT.glob("interop/*")),
        *sorted(ROOT.glob("redist/*")),
        ROOT / "runtime" / "python",
        ROOT / "table",
    ]:
        add(found, distribution, ("*.py", "*.pyi", "py.typed"))

    return found


def merge(target: pathlib.Path) -> tuple[int, int]:
    """
    Makes @p target hold exactly the merged packages, copying only the files
    that changed. Returns how many files were copied and how many removed.
    """
    wanted = sources()
    removed = 0

    # in reverse order a directory comes after everything in it, so it is empty
    # by the time it is reached if nothing in it is wanted
    if target.exists():
        for path in sorted(target.rglob("*"), reverse=True):
            if path.is_dir():
                if not any(path.iterdir()):
                    path.rmdir()

                continue

            if path.relative_to(target) not in wanted:
                path.unlink()
                removed += 1

    copied = 0

    for installed, source in wanted.items():
        copy = target / installed
        stat = source.stat()

        # copy2() keeps the modification time, so an unchanged file matches
        if copy.exists():
            copied_stat = copy.stat()

            if (copied_stat.st_size, copied_stat.st_mtime_ns) == (
                stat.st_size,
                stat.st_mtime_ns,
            ):
                continue

        copy.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, copy)
        copied += 1

    return copied, removed


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=DESCRIPTION)
    parser.parse_args()

    copied, removed = merge(TYPING)

    print(f"_typing: {copied} files copied, {removed} removed")
