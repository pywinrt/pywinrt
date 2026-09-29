import argparse
import ast
import csv
import io
import pathlib
import tokenize
from collections.abc import Iterator
from typing import NamedTuple

SCRIPT_DIR = pathlib.Path(__file__).parent.resolve()


def read_table(name: str) -> list[dict[str, str]]:
    with open(SCRIPT_DIR / name, newline="") as f:
        return list(csv.DictReader(f, dialect="unix"))


# old method name: (qualified names of the types that had it, new name, whether
# the old name is now another method)
METHODS: dict[str, tuple[list[str], str, bool]] = {}

for item in read_table("methods.csv"):
    types = METHODS.setdefault(
        item["method"], ([], item["new_name"], item["old_name_reused"] == "yes")
    )[0]
    types.append(f"{item['module']}.{item['type']}")

# the properties whose value is an HResult
HRESULT_PROPERTIES = {item["property"] for item in read_table("values.csv")}

# what each format string winrt.system.Array took is spelled as now
ARRAY_FORMATS = {
    "?": "bool",
    "b": "winrt.system.Int8",
    "B": "winrt.system.UInt8",
    "h": "winrt.system.Int16",
    "H": "winrt.system.UInt16",
    "i": "winrt.system.Int32",
    "I": "winrt.system.UInt32",
    "q": "winrt.system.Int64",
    "Q": "winrt.system.UInt64",
    "f": "winrt.system.Single",
    "d": "winrt.system.Double",
    "u": "winrt.system.Char16",
}

# the top-level packages that are winrt again
PACKAGES = {"winui3", "webview2"}


class Match(NamedTuple):
    line: int
    column: int
    name: str
    rename_to: str


class Edit(NamedTuple):
    line: int
    column: int
    old: str
    new: str


def column(lines: list[str], line: int, offset: int) -> int:
    """
    The character column of the UTF-8 byte @p offset that ast reports.
    """
    return len(lines[line - 1].encode()[:offset].decode())


def attribute_column(lines: list[str], node: ast.Attribute) -> int:
    """
    The column of the attribute name itself, rather than of the expression
    it is looked up on.
    """
    assert node.end_lineno is not None and node.end_col_offset is not None
    return column(lines, node.end_lineno, node.end_col_offset) - len(node.attr)


def receiver_name(node: ast.expr) -> str | None:
    if isinstance(node, ast.Name):
        return node.id

    if isinstance(node, ast.Attribute):
        return node.attr

    return None


def renamed_package(module: str) -> str | None:
    package, dot, rest = module.partition(".")

    if package not in PACKAGES:
        return None

    return f"winrt{dot}{rest}"


def find(tree: ast.AST, lines: list[str]) -> Iterator[Match]:
    for node in ast.walk(tree):
        if isinstance(node, ast.Attribute):
            method = METHODS.get(node.attr)

            if method is not None:
                types, new_name, reused = method
                name = f"{types[0]}.{node.attr}"

                if len(types) > 1:
                    name += f" (and {len(types) - 1} other types)"

                if reused:
                    new_name += " (v4 uses the old name for something else)"

                assert node.end_lineno is not None
                yield Match(
                    node.end_lineno, attribute_column(lines, node), name, new_name
                )

            if node.attr == "value":
                receiver = receiver_name(node.value)
                assert node.end_lineno is not None

                if receiver in HRESULT_PROPERTIES:
                    yield Match(
                        node.end_lineno,
                        attribute_column(lines, node),
                        "winrt.windows.foundation.HResult.value",
                        f"{receiver}, without .value: an HResult is an int",
                    )
                elif receiver is not None and "token" in receiver.lower():
                    yield Match(
                        node.end_lineno,
                        attribute_column(lines, node),
                        "winrt.windows.foundation.EventRegistrationToken.value",
                        f"{receiver}, without .value: an EventRegistrationToken is an int",
                    )

        elif isinstance(node, ast.Call):
            if receiver_name(node.func) != "Array" or not node.args:
                continue

            first = node.args[0]

            if not isinstance(first, ast.Constant) or not isinstance(first.value, str):
                continue

            element_type = ARRAY_FORMATS.get(first.value)

            if element_type is None:
                continue

            yield Match(
                first.lineno,
                column(lines, first.lineno, first.col_offset),
                f'winrt.system.Array("{first.value}", ...)',
                f"{element_type}, or the enum type for an array of enums",
            )

        elif isinstance(node, ast.Import):
            for alias in node.names:
                new_module = renamed_package(alias.name)

                if new_module is not None:
                    yield Match(
                        alias.lineno,
                        column(lines, alias.lineno, alias.col_offset),
                        alias.name,
                        new_module,
                    )

        elif isinstance(node, ast.ImportFrom):
            if node.level or node.module is None:
                continue

            new_module = renamed_package(node.module)

            if new_module is not None:
                yield Match(
                    node.lineno,
                    column(lines, node.lineno, node.col_offset),
                    node.module,
                    new_module,
                )


def package_edits(source: str, tree: ast.AST, lines: list[str]) -> list[Edit]:
    """
    The edits that turn the winui3 and webview2 packages into winrt: the
    first name of each module an import statement names, and each use of a
    package that a plain import statement binds.
    """
    # the kind of import statement each line is part of
    statements: dict[int, type[ast.stmt]] = {}
    bound: set[str] = set()

    for node in ast.walk(tree):
        if isinstance(node, ast.Import):
            for alias in node.names:
                package = alias.name.partition(".")[0]

                if package in PACKAGES and alias.asname is None:
                    bound.add(package)

        if isinstance(node, (ast.Import, ast.ImportFrom)):
            assert node.end_lineno is not None

            for line in range(node.lineno, node.end_lineno + 1):
                statements[line] = type(node)

    edits = []
    previous = ""

    for token in tokenize.generate_tokens(io.StringIO(source).readline):
        if token.type in (tokenize.NL, tokenize.COMMENT):
            continue

        line, col = token.start
        kind = statements.get(line)

        if token.type == tokenize.NAME and token.string in PACKAGES:
            # an ImportFrom names its module straight after "from", and what
            # follows "import" there are the names imported from it
            if (kind is ast.Import and previous in ("import", ",")) or (
                kind is ast.ImportFrom and previous == "from"
            ):
                edits.append(Edit(line, col, token.string, "winrt"))

        previous = token.string

    for node in ast.walk(tree):
        if isinstance(node, ast.Name) and node.id in bound:
            edits.append(
                Edit(
                    node.lineno,
                    column(lines, node.lineno, node.col_offset),
                    node.id,
                    "winrt",
                )
            )

    return edits


def fix(path: pathlib.Path, source: str, edits: list[Edit]) -> None:
    lines = source.splitlines(keepends=True)

    for edit in sorted(edits, reverse=True):
        text = lines[edit.line - 1]
        assert text[edit.column : edit.column + len(edit.old)] == edit.old
        lines[edit.line - 1] = (
            text[: edit.column] + edit.new + text[edit.column + len(edit.old) :]
        )

    path.write_bytes("".join(lines).encode())


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Find what may need changing to move code from PyWinRT v3 to v4."
    )
    parser.add_argument(
        "files", help="Files to inspect", nargs="+", type=pathlib.Path, metavar="file"
    )
    parser.add_argument(
        "--fix",
        help="Rewrite the winui3 and webview2 packages to winrt in place",
        action="store_true",
    )
    args = parser.parse_args()

    for path in args.files:
        source = path.read_bytes().decode()
        tree = ast.parse(source, path)
        lines = source.splitlines()

        for match in sorted(find(tree, lines)):
            print(f"{path}:{match.line}:{match.column + 1}")
            print("possible match:", match.name)
            print("rename to:", match.rename_to)

        if args.fix:
            edits = package_edits(source, tree, lines)

            if edits:
                fix(path, source, edits)
                print(f"{path}: rewrote {len(edits)} package names")
