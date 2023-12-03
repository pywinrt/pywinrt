from itertools import chain
from pathlib import Path
from typing import List

PYPROJECT_TOML_TEMPLATE = """\
# WARNING: Please don't edit this file. It was automatically generated.

[build-system]
requires = ["setuptools", "setuptools-scm"{requires_winrt_sdk}{requires_windows_app_sdk}]
build-backend = "setuptools.build_meta"

[project]
name = "{package_name}"
description = "{description}"
readme = "README.md"
license = {{ text = "MIT" }}
classifiers = [
    "Operating System :: Microsoft :: Windows",
    "Programming Language :: Python :: 3",
    "Programming Language :: Python :: Implementation :: CPython",
    "Intended Audience :: Developers",
]
dynamic = ["version"{extra_dynamic}]
requires-python = ">=3.9,<3.13"

[project.urls]
# Homepage = "https://github.com/pywinrt/pywinrt"
Documentation = "https://pywinrt.readthedocs.io"
Repository = "https://github.com/pywinrt/pywinrt"
Changelog = "https://github.com/pywinrt/pywinrt/blob/main/CHANGELOG.md"

[tool.setuptools.dynamic]
version = {{ file = "pywinrt-version.txt" }}{dependencies}{optional_dependencies}
{find_src}
"""

SDK_PACKAGE_TEMPLATE = """\
[tool.setuptools.package-data]
"*" = ["*.h"]
"""

BINARY_PACKAGE_TEMPLATE = """\
[tool.setuptools.package-data]
"*" = ["*.pyi", "py.typed"]

[tool.cibuildwheel]
# use local winrt-sdk build dependency
environment = {{ PYTHONPATH="{relative}/winrt-sdk/src{extra_python_path}" }}
# don't install winrt-sdk from PyPI
build-frontend = {{ name = "build", args = ["--skip-dependency-check", "--no-isolation"] }}
# don't build for PyPy
skip = "pp*"
# suppress warnings about ARM64 testing
test-skip = "*-win_arm64"

[tool.cibuildwheel.windows]
archs = ["x86", "AMD64", "ARM64"]
"""

DEPENDENCIES = """
dependencies = { file = "requirements.txt" }"""

OPTIONAL_DEPENDENCIES = """
optional-dependencies.all = { file = "all-requirements.txt" }"""

FIND_SRC = """
[tool.setuptools.packages.find]
where = ["src"]
"""

SETUP_PY_TEMPLATE = """\
# WARNING: Please don't edit this file. It was automatically generated.

from setuptools import Extension, setup
from winrt_sdk import get_include_dirs{extra_imports}

setup(
    ext_modules=[
        Extension(
            "winrt.{ext_module}",
            sources=[{sources}],
            include_dirs=get_include_dirs(){extra_include_dirs},
            extra_compile_args=["/std:c++20", "/permissive-"],
            libraries=["windowsapp"],
        )
    ],
)
"""

README_TEMPLATE = """\
<!-- warning: Please don't edit this file. It was automatically generated. -->

# {package_name}

"""

BINARY_README_TEMPLATE = """\
Windows Runtime (WinRT) APIs for for the `{namespace}` namespace.

This package provides the `{module_name}` module.
"""

SDK_README_TEMPLATE = """\
This package provides PyWinRT header files for {sdk_name}.

This package is a build time dependency, not a runtime dependency.

Example use in a `pyproject.toml` file:

```toml
[build-system]
requires = ["setuptools", "setuptools-scm", "winrt-sdk"{extra_requires}]
build-backend = "setuptools.build_meta"
```

Then in your `setup.py`:

```python
from setuptools import setup
from winrt_sdk import get_include_dirs{extra_imports}

setup(
    ...
    include_dirs=get_include_dirs(){extra_include_dirs}
)
```

For the runtime package, use `winrt-runtime` and the various namespace packages
instead.
"""

PROJECTION_PATH = (Path(__file__).parent.parent / "projection").resolve()

PYTHON_KEYWORDS = {
    "and",
    "as",
    "assert",
    "async",
    "await",
    "break",
    "class",
    "continue",
    "def",
    "del",
    "elif",
    "else",
    "except",
    "finally",
    "for",
    "from",
    "global",
    "if",
    "import",
    "in",
    "is",
    "lambda",
    "nonlocal",
    "not",
    "or",
    "pass",
    "raise",
    "return",
    "try",
    "while",
    "with",
    "yield",
}


def avoid_keyword(name: str) -> str:
    if name in PYTHON_KEYWORDS:
        return f"{name}_"

    return name


def is_sdk_package(name: str) -> bool:
    return name in ["winrt-sdk", "winrt-WindowsAppSDK"]


def is_windows_app_package(name: str) -> bool:
    return name.startswith("winrt-Microsoft.")


def winrt_ns_to_py_package(ns: str) -> str:
    return ".".join(avoid_keyword(x.lower()) for x in ns.split("."))


def write_project_files(
    package_path: Path,
    module_name: str,
    ext_module_name: str,
    sources: List[str],
) -> None:
    package_name = package_path.name
    relative_package_path = package_path.relative_to(PROJECTION_PATH)
    relative = "/".join([".."] * len(relative_package_path.parts))
    has_requirements = (package_path / "requirements.txt").exists()
    has_all_requirements = (package_path / "all-requirements.txt").exists()

    with open(package_path / "pyproject.toml", "w", newline="\n") as f:
        f.write(
            PYPROJECT_TOML_TEMPLATE.format(
                requires_winrt_sdk=""
                if is_sdk_package(package_name)
                else ', "winrt-sdk"',
                requires_windows_app_sdk=', "winrt-WindowsAppSDK"'
                if is_windows_app_package(package_name)
                else "",
                package_name=package_name,
                description="Windows Runtime SDK for Python header files"
                if is_sdk_package(package_name)
                else "Python projection of Windows Runtime (WinRT) APIs",
                module_name=module_name,
                extra_dynamic=(', "dependencies"' if has_requirements else "")
                + (', "optional-dependencies"' if has_all_requirements else ""),
                dependencies=DEPENDENCIES if has_requirements else "",
                optional_dependencies=OPTIONAL_DEPENDENCIES
                if has_all_requirements
                else "",
                find_src=FIND_SRC if (package_path / "src").exists() else "",
            )
        )

        if is_sdk_package(package_name):
            f.write(SDK_PACKAGE_TEMPLATE)
        else:
            f.write(
                BINARY_PACKAGE_TEMPLATE.format(
                    relative=relative,
                    extra_python_path=f";{relative}/winrt-WindowsAppSDK/src"
                    if is_windows_app_package(package_name)
                    else "",
                )
            )

    if not is_sdk_package(package_name):
        with open(package_path / "setup.py", "w", newline="\n") as f:
            f.write(
                SETUP_PY_TEMPLATE.format(
                    ext_module=ext_module_name,
                    sources=", ".join(f'"{x}"' for x in sources),
                    extra_imports="\nfrom winrt_windows_app_sdk import get_include_dirs as get_app_sdk_include_dirs"
                    if is_windows_app_package(package_name)
                    else "",
                    extra_include_dirs="+ get_app_sdk_include_dirs()"
                    if is_windows_app_package(package_name)
                    else "",
                )
            )

    if package_name != "winrt-runtime":
        if not is_sdk_package(package_name):
            with open(
                package_path / Path(*module_name.split(".")) / "py.typed",
                "w",
                newline="\n",
            ) as f:
                pass

        with open(package_path / "README.md", "w", newline="\n") as f:
            f.write(
                README_TEMPLATE.format(
                    package_name=package_name,
                )
            )

            if is_sdk_package(package_name):
                f.write(
                    SDK_README_TEMPLATE.format(
                        sdk_name="the Windows SDK"
                        if package_name == "winrt-sdk"
                        else package_name.removeprefix("winrt-"),
                        extra_requires=""
                        if package_name == "winrt-sdk"
                        else f', "{package_name}"',
                        extra_imports="\nfrom winrt_windows_app_sdk import get_include_dirs as get_app_sdk_include_dirs"
                        if is_windows_app_package(package_name)
                        else "",
                        extra_include_dirs="+ get_app_sdk_include_dirs()"
                        if is_windows_app_package(package_name)
                        else "",
                    )
                )
            else:
                f.write(
                    BINARY_README_TEMPLATE.format(
                        namespace=package_name.removeprefix("winrt-"),
                        module_name=module_name,
                    )
                )


write_project_files(
    PROJECTION_PATH / "winrt-runtime",
    "winrt.system",
    "_winrt",
    ["_winrt.cpp", "_winrt_array.cpp", "runtime.cpp"],
)

for package_path in chain(
    (PROJECTION_PATH / "interop").glob("winrt-*"),
    (PROJECTION_PATH).glob("winrt-Windows.*"),
    (PROJECTION_PATH).glob("winrt-Microsoft.*"),
    [
        PROJECTION_PATH / "winrt-sdk",
        PROJECTION_PATH / "winrt-WindowsAppSDK",
        PROJECTION_PATH / "winrt-TestComponent",
    ],
):
    namespace = package_path.name.removeprefix("winrt-")
    module_name = f"winrt.{winrt_ns_to_py_package(namespace)}"
    ext_module_name = f"_{module_name.replace('.', '_')}"
    source_file = f"py.{namespace}.cpp"

    write_project_files(package_path, module_name, ext_module_name, [source_file])
