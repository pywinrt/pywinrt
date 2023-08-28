from glob import glob
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
build-frontend = "build; args: --skip-dependency-check --no-isolation"
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
            "{ext_module}",
            sources=[{sources}],
            include_dirs=get_include_dirs(),
            extra_compile_args=["/std:c++20", "/permissive-"],
            libraries=["windowsapp"],
        )
    ]
)
"""

PROJECTION_PATH = (Path(__file__).parent.parent / "projection").resolve()


with open(PROJECTION_PATH / "winrt-runtime" / "pyproject.toml", "w") as f:
    f.write(
        PYPROJECT_TOML_TEMPLATE.format(
            package_name="winrt-runtime",
            module_name="winrt.system",
            **(PYPROJECT_DEFAULTS | dict(dependencies="", find_src=FIND_SRC)),
        )
    )

with open(PROJECTION_PATH / "winrt-runtime" / "setup.py", "w") as f:
    f.write(
        SETUP_PY_TEMPLATE.format(
            ext_module="winrt._winrt",
            sources='"_winrt.cpp", "_winrt_array.cpp", "runtime.cpp"',
        )
    )

for package in glob("winrt-*", root_dir=str(PROJECTION_PATH / "interop")):
    ns = package.removeprefix("winrt-")
    module = f"winrt.{ns.lower()}"

    with open(PROJECTION_PATH / "interop" / package / "pyproject.toml", "w") as f:
        f.write(
            PYPROJECT_TOML_TEMPLATE.format(
                package_name=package,
                module_name=module,
                **(PYPROJECT_DEFAULTS | dict(relative="../..")),
            )
        )

    with open(PROJECTION_PATH / "interop" / package / "setup.py", "w") as f:
        f.write(
            SETUP_PY_TEMPLATE.format(
                ext_module=f"{module}._{module.replace('.', '_')}",
                sources=f'"py.{ns}.cpp"',
            )
        )

for package in glob("winrt-Windows.*", root_dir=str(PROJECTION_PATH)):
    ns = package.removeprefix("winrt-")
    module = f"winrt.{ns.lower()}"

    with open(PROJECTION_PATH / package / "pyproject.toml", "w") as f:
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

    with open(PROJECTION_PATH / package / "setup.py", "w") as f:
        f.write(
            SETUP_PY_TEMPLATE.format(
                ext_module=f"{module}._{module.replace('.', '_')}",
                sources=f'"py.{ns}.cpp"',
            )
        )

with open(PROJECTION_PATH / "winrt-TestComponent" / "pyproject.toml", "w") as f:
    f.write(
        PYPROJECT_TOML_TEMPLATE.format(
            package_name="winrt-TestComponent",
            module_name="winrt.testcomponent",
            **(PYPROJECT_DEFAULTS | dict(find_src=FIND_SRC)),
        )
    )

    with open(PROJECTION_PATH / "winrt-TestComponent" / "setup.py", "w") as f:
        f.write(
            SETUP_PY_TEMPLATE.format(
                ext_module="_winrt.testcomponent._winrt_testcomponent",
                sources='"py.TestComponent.cpp"',
            )
        )
