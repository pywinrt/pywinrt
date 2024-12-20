import ast
import argparse
import csv
import pathlib

SCRIPT_DIR = pathlib.Path(__file__).parent.resolve()

ENUMS = {
    item["member"]: (f"{item['module']}.{item['type']}", item["new_name"])
    for item in csv.DictReader(
        (SCRIPT_DIR / "enums.csv").read_text(newline="").splitlines(), dialect="unix"
    )
}

EVENTS = {
    item["method"]: (f"{item['module']}.{item['type']}", item["new_name"])
    for item in csv.DictReader(
        (SCRIPT_DIR / "events.csv").read_text(newline="").splitlines(), dialect="unix"
    )
}

METHODS = {
    (item["method"], int(item["num_args"])): (
        f"{item['module']}.{item['type']}",
        item["new_name"],
    )
    for item in csv.DictReader(
        (SCRIPT_DIR / "methods.csv").read_text(newline="").splitlines(), dialect="unix"
    )
}

PROPERTIES = {
    item["property"]: (f"{item['module']}.{item['type']}", item["new_name"])
    for item in csv.DictReader(
        (SCRIPT_DIR / "properties.csv").read_text(newline="").splitlines(),
        dialect="unix",
    )
}


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("file", help="File to parse", type=argparse.FileType("r"))
    args = parser.parse_args()

    tree = ast.parse(args.file.read(), args.file.name)

    for node in ast.walk(tree):
        if isinstance(node, ast.Attribute):
            match = ENUMS.get(node.attr)

            if match is not None:
                print(f"{args.file.name}:{node.lineno}:{node.col_offset}")
                print(
                    "possible match:",
                    f"{match[0]}.{node.attr}",
                )
                print("rename to:", match[1])

            match = PROPERTIES.get(node.attr)

            if match is not None:
                print(f"{args.file.name}:{node.lineno}:{node.col_offset}")
                print(
                    "possible match:",
                    f"{match[0]}.{node.attr}",
                )
                print("rename to:", match[1])

        if isinstance(node, ast.Call):
            if not isinstance(node.func, ast.Attribute):
                continue

            match = METHODS.get((node.func.attr, len(node.args)))

            if match is not None:
                qualified_type, new_name = match

                print(f"{args.file.name}:{node.func.lineno}:{node.func.col_offset}")
                print(
                    "possible match:",
                    f"{qualified_type}.{node.func.attr}",
                )
                print("rename to:", new_name)

            if len(node.args) == 1:
                match = EVENTS.get(node.func.attr)

                if match is not None:
                    qualified_type, new_name = match

                    print(f"{args.file.name}:{node.func.lineno}:{node.func.col_offset}")
                    print(
                        "possible match:",
                        f"{qualified_type}.{node.func.attr}",
                    )
                    print("rename to:", new_name)
