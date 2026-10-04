import argparse
import ast
import csv
import io
import pathlib
import re
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

# the structs whose product * was, and @ is
MATRICES = {"Matrix3x2", "Matrix4x4"}

MATRIX_PRODUCT = "@ for the product of two matrices; * between two of them raises"


def normalize(name: str) -> str:
    """
    A distribution name the way pip compares it (PEP 503).
    """
    return re.sub(r"[-_.]+", "-", name).lower()


# each distribution v3.2.1 published: (its v4 distribution, the floor to
# require it at), both empty if v4 has none
DISTRIBUTIONS = {
    normalize(item["distribution"]): (item["new_distribution"], item["floor"])
    for item in read_table("distributions.csv")
}

_CLAUSE = r"(?:===|[<>=!~]=|[<>])[ \t]*[\w.*!+-]+"

REQUIREMENT = re.compile(
    rf"""
    (?<![\w.-])
    (?P<name>(?:winrt|winui2|winui3|webview2)[-_.][A-Za-z0-9](?:[\w.-]*[A-Za-z0-9])?)
    (?P<extras>[ \t]*\[[^\]\n]*\])?
    (?P<specifier>[ \t]*{_CLAUSE}(?:[ \t]*,[ \t]*{_CLAUSE})*)?
    """,
    re.VERBOSE,
)

# a line that holds one requirement and nothing else, as a requirements file,
# a TOML array or a Python list writes it
REQUIREMENT_LINE = re.compile(
    r"""
    ^\s*
    (?:"(?P<double>[^"]+)"|'(?P<single>[^']+)'|(?P<bare>[^"'\#\s][^\#]*?))
    \s*,?\s*(?:\#.*)?$
    """,
    re.VERBOSE,
)


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


class Requirement(NamedTuple):
    match: Match
    # None for a distribution that v4 does not have, which has no fix
    edit: Edit | None


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

            if node.attr in MATRICES:
                assert node.end_lineno is not None
                yield Match(
                    node.end_lineno,
                    attribute_column(lines, node),
                    f"winrt.windows.foundation.numerics.{node.attr}",
                    MATRIX_PRODUCT,
                )

        elif isinstance(node, ast.Name):
            if node.id in MATRICES:
                yield Match(
                    node.lineno,
                    column(lines, node.lineno, node.col_offset),
                    f"winrt.windows.foundation.numerics.{node.id}",
                    MATRIX_PRODUCT,
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


def version_key(version: str) -> tuple[int, tuple[int, ...]]:
    """
    The epoch and the release numbers of @p version, which is all it takes to
    tell a v4 version from a v3 one.
    """
    epoch, bang, release = version.partition("!")

    if not bang:
        epoch, release = "0", version

    numbers = re.findall(r"\d+", re.split(r"[^\d.]", release, maxsplit=1)[0])
    return int(epoch), tuple(int(n) for n in numbers)


def excludes(specifier: str, floor: str) -> bool:
    """
    Whether @p specifier refuses @p floor, the first v4 version.
    """
    floor_epoch, floor_release = version_key(floor)

    for clause in specifier.split(","):
        operator, version = re.split(
            r"(?<=[<>=!~])(?=[^<>=!~])", clause.strip(), maxsplit=1
        )
        epoch, release = version_key(version.strip())
        width = max(len(release), len(floor_release))
        pad = (0,) * width
        key = (epoch, (release + pad)[:width])
        floor_key = (floor_epoch, (floor_release + pad)[:width])

        if operator == "<" and not floor_key < key:
            return True

        if operator == "<=" and floor_key > key:
            return True

        # a pin to a version of the previous generation; a pin to a v4 version
        # is left alone, since it is deliberate
        if operator in ("==", "===", "~=") and (epoch, release[:1]) < (
            floor_epoch,
            floor_release[:1],
        ):
            return True

    return False


def string_spans(source: str) -> list[tuple[tuple[int, int], tuple[int, int]]]:
    """
    Where the string literals of the Python code @p source are.
    """
    return [
        (token.start, token.end)
        for token in tokenize.generate_tokens(io.StringIO(source).readline)
        if token.type == tokenize.STRING
    ]


def find_requirements(
    source: str, lines: list[str], python: bool
) -> Iterator[Requirement]:
    """
    The requirements on a v3 distribution that v4 renamed or removed, or that
    a specifier keeps at v3. In Python code, only string literals are looked
    at; elsewhere, everything but comments.
    """
    spans = string_spans(source) if python else []

    for number, text in enumerate(lines, 1):
        for found in REQUIREMENT.finditer(text):
            start = (number, found.start())

            if python:
                if not any(begin <= start < end for begin, end in spans):
                    continue
            elif "#" in text[: found.start()]:
                continue

            entry = DISTRIBUTIONS.get(normalize(found["name"]))

            if entry is None:
                continue

            new_name, floor = entry
            match_text = found.group().strip()

            if not new_name:
                yield Requirement(
                    Match(
                        number,
                        found.start(),
                        match_text,
                        "nothing: there is no v4 distribution, see scripts/3to4/README.md",
                    ),
                    None,
                )
                continue

            specifier = found["specifier"] or ""
            renamed = normalize(new_name) != normalize(found["name"])
            excluded = bool(specifier) and excludes(specifier, floor[2:])

            if not renamed and not excluded:
                continue

            new_text = new_name if renamed else found["name"]
            new_text += found["extras"] or ""

            if specifier:
                new_text += floor if excluded or renamed else specifier

            yield Requirement(
                Match(number, found.start(), match_text, new_text),
                Edit(number, found.start(), found.group(), new_text),
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


def requirement_of(line: str) -> str | None:
    """
    The requirement on @p line, if that is all the line holds.
    """
    found = REQUIREMENT_LINE.match(line)

    if found is None:
        return None

    requirement = found["double"] or found["single"] or found["bare"]
    return " ".join(requirement.split())


def fix(
    path: pathlib.Path, source: str, edits: list[Edit], requirement_lines: set[int]
) -> int:
    """
    Applies @p edits to @p source and writes it to @p path. Of the lines in
    @p requirement_lines, one with no comment that now repeats a requirement
    listed above it, in the same run of lines that each hold one requirement,
    is dropped; several v3 names often became one v4 distribution. Returns how
    many were dropped.
    """
    lines = source.splitlines(keepends=True)

    for edit in sorted(edits, reverse=True):
        text = lines[edit.line - 1]
        assert text[edit.column : edit.column + len(edit.old)] == edit.old
        lines[edit.line - 1] = (
            text[: edit.column] + edit.new + text[edit.column + len(edit.old) :]
        )

    dropped = set()

    for number in sorted(requirement_lines):
        requirement = requirement_of(lines[number - 1])

        if requirement is None:
            continue

        # dropping the line would drop what the comment on it says
        if "#" in lines[number - 1]:
            continue

        above = number - 1

        while above >= 1 and (earlier := requirement_of(lines[above - 1])) is not None:
            if above not in dropped and earlier == requirement:
                dropped.add(number)
                break

            above -= 1

    kept = [line for number, line in enumerate(lines, 1) if number not in dropped]
    path.write_bytes("".join(kept).encode())

    return len(dropped)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Find what may need changing to move code from PyWinRT v3 to v4."
    )
    parser.add_argument(
        "files", help="Files to inspect", nargs="+", type=pathlib.Path, metavar="file"
    )
    parser.add_argument(
        "--fix",
        help="Rewrite the winui3 and webview2 packages to winrt, and the "
        "requirements on v3 distributions to v4 ones, in place",
        action="store_true",
    )
    args = parser.parse_args()

    for path in args.files:
        source = path.read_bytes().decode()
        lines = source.splitlines()
        python = path.suffix == ".py"
        tree = ast.parse(source, path) if python else None
        requirements = list(find_requirements(source, lines, python))
        matches = list(find(tree, lines)) if tree else []
        matches += [requirement.match for requirement in requirements]

        for match in sorted(matches):
            print(f"{path}:{match.line}:{match.column + 1}")
            print("possible match:", match.name)
            print("rename to:", match.rename_to)

        if args.fix:
            edits = package_edits(source, tree, lines) if tree else []
            requirement_edits = [r.edit for r in requirements if r.edit is not None]

            if edits or requirement_edits:
                dropped = fix(
                    path,
                    source,
                    edits + requirement_edits,
                    {edit.line for edit in requirement_edits},
                )

                if edits:
                    print(f"{path}: rewrote {len(edits)} package names")

                if requirement_edits:
                    print(
                        f"{path}: rewrote {len(requirement_edits)} requirements, "
                        f"dropped {dropped} that became duplicates"
                    )
