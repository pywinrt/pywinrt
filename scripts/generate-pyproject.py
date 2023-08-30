from glob import iglob
from pathlib import Path

PYPROJECT_TOML_TEMPLATE = """\
# WARNING: Please don't edit this file. It was automatically generated.

[build-system]
requires = ["setuptools", "setuptools-scm", "winrt-sdk"]
build-backend = "setuptools.build_meta"

[project]
name = "{package_name}"
description = "Python projection of Windows Runtime (WinRT) APIs"
license = {{ text = "MIT" }}
classifiers = [
    "Operating System :: Microsoft :: Windows",
    "Programming Language :: Python :: 3",
    "Target Audience :: Developers",
]
dynamic = ["version"{extra_dynamic}]
requires-python = ">=3.9,<3.13"{dependencies}

[project.urls]
# Homepage = "https://github.com/pywrint/pywrinrt"
# Documentation = "https://readthedocs.org"
Repository = "https://github.com/pywrint/pywrinrt"
Changelog = "https://github.com/pywinrt/pywinrt/blob/main/CHANGELOG.md"

[tool.setuptools.dynamic]
version = {{ file = "{relative}/winrt-sdk/version.txt" }}{optional_dependencies}
{find_src}
[tool.cibuildwheel]
# use local winrt-sdk build dependency
environment = {{ PYTHONPATH="{relative}/winrt-sdk/src" }}
# don't install winrt-sdk from PyPI
build-frontend = {{ name = "build", args = ["--skip-dependency-check", "--no-isolation"] }}
# don't build for PyPy
skip = "pp*"

[tool.cibuildwheel.windows]
archs = ["x86", "AMD64", "ARM64"]
"""

OPTIONAL_DEPENDENCIES = """
optional-dependencies.all = { file = "all-requirements.txt" }"""

FIND_SRC = """
[tool.setuptools.packages.find]
where = ["src"]
"""

PYPROJECT_DEFAULTS = dict(
    dependencies='\ndependencies = ["winrt-runtime==0.0.0"]',
    optional_dependencies="",
    find_src="",
    relative="..",
    extra_dynamic="",
)

SETUP_PY_TEMPLATE = """\
# WARNING: Please don't edit this file. It was automatically generated.

from setuptools import Extension, setup
from winrt_sdk import get_include_dirs

setup(
    ext_modules=[
        Extension(
            "winrt.{ext_module}",
            sources=[{sources}],
            include_dirs=get_include_dirs(),
            extra_compile_args=["/std:c++20", "/permissive-"],
            libraries=["windowsapp"],
        )
    ],{package_data}
)
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


def winrt_ns_to_py_package(ns: str) -> str:
    return ".".join(avoid_keyword(x.lower()) for x in ns.split("."))


with open(PROJECTION_PATH / "winrt-runtime" / "pyproject.toml", "w", newline="\n") as f:
    f.write(
        PYPROJECT_TOML_TEMPLATE.format(
            package_name="winrt-runtime",
            module_name="winrt.system",
            **(PYPROJECT_DEFAULTS | dict(dependencies="", find_src=FIND_SRC)),
        )
    )

with open(PROJECTION_PATH / "winrt-runtime" / "setup.py", "w", newline="\n") as f:
    f.write(
        SETUP_PY_TEMPLATE.format(
            ext_module="_winrt",
            sources='"_winrt.cpp", "_winrt_array.cpp", "runtime.cpp"',
            package_data="",
        )
    )

for package_path in iglob(str(PROJECTION_PATH / "interop" / "winrt-*")):
    package = Path(package_path).name
    ns = package.removeprefix("winrt-")
    module = f"winrt.{winrt_ns_to_py_package(ns)}"

    with open(
        PROJECTION_PATH / "interop" / package / "pyproject.toml", "w", newline="\n"
    ) as f:
        f.write(
            PYPROJECT_TOML_TEMPLATE.format(
                package_name=package,
                module_name=module,
                **(PYPROJECT_DEFAULTS | dict(relative="../..")),
            )
        )

    with open(
        PROJECTION_PATH / "interop" / package / "setup.py", "w", newline="\n"
    ) as f:
        f.write(
            SETUP_PY_TEMPLATE.format(
                ext_module=f"_{module.replace('.', '_')}",
                sources=f'"py.{ns}.cpp"',
                package_data='\n    package_data={"winrt": ["*.pyi"]},',
            )
        )

for package_path in iglob(str(PROJECTION_PATH / "winrt-Windows.*")):
    package = Path(package_path).name
    ns = package.removeprefix("winrt-")
    module = f"winrt.{winrt_ns_to_py_package(ns)}"

    with open(PROJECTION_PATH / package / "pyproject.toml", "w", newline="\n") as f:
        optional_dependencies = (
            dict(
                optional_dependencies=OPTIONAL_DEPENDENCIES,
                extra_dynamic=', "optional-dependencies"',
            )
            if (PROJECTION_PATH / package / "all-requirements.txt").exists()
            else {}
        )

        f.write(
            PYPROJECT_TOML_TEMPLATE.format(
                package_name=package,
                module_name=module,
                **(PYPROJECT_DEFAULTS | optional_dependencies),
            )
        )

    with open(PROJECTION_PATH / package / "setup.py", "w", newline="\n") as f:
        f.write(
            SETUP_PY_TEMPLATE.format(
                ext_module=f"_{module.replace('.', '_')}",
                sources=f'"py.{ns}.cpp"',
                package_data='\n    package_data={"winrt": ["*.pyi"]},',
            )
        )

with open(
    PROJECTION_PATH / "winrt-TestComponent" / "pyproject.toml", "w", newline="\n"
) as f:
    f.write(
        PYPROJECT_TOML_TEMPLATE.format(
            package_name="winrt-TestComponent",
            module_name="winrt.testcomponent",
            **(PYPROJECT_DEFAULTS | dict(find_src=FIND_SRC)),
        )
    )

    with open(
        PROJECTION_PATH / "winrt-TestComponent" / "setup.py", "w", newline="\n"
    ) as f:
        f.write(
            SETUP_PY_TEMPLATE.format(
                ext_module="_winrt_testcomponent",
                sources='"py.TestComponent.cpp"',
                package_data='\n    package_data={"winrt": ["*.pyi"]},',
            )
        )
