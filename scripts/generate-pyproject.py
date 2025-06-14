from itertools import chain
from pathlib import Path
from typing import List

PYPROJECT_TOML_TEMPLATE = """\
# WARNING: Please don't edit this file. It was automatically generated.

[build-system]
requires = ["setuptools>=78"{extra_requires}]
build-backend = "setuptools.build_meta"

[project]
name = "{package_name}"
description = "{description}"
readme = "README.md"
license = "MIT"
classifiers = [
    "Operating System :: Microsoft :: Windows",
    "Programming Language :: Python :: 3",
    "Programming Language :: Python :: Implementation :: CPython",
    "Intended Audience :: Developers",
]
dynamic = ["version"{extra_dynamic}]
requires-python = ">=3.9"

[project.urls]
# Homepage = "https://github.com/pywinrt/pywinrt"
Documentation = "https://pywinrt.readthedocs.io"
Repository = "https://github.com/pywinrt/pywinrt"
Changelog = "https://github.com/pywinrt/pywinrt/blob/main/CHANGELOG.md"

[tool.setuptools.dynamic]
version = {{ file = "pywinrt-version.txt" }}{dependencies}{optional_dependencies}
{find_src}
"""

COMPONENT_PACKAGE_FIND_SRC = """
[tool.setuptools.packages.find]
exclude = ["cppwrint"]
"""

SDK_PACKAGE_TEMPLATE = """\
[tool.setuptools.package-data]
"*" = ["*.h"]
"""

BINARY_PACKAGE_TEMPLATE = """\
[tool.setuptools.package-data]
"*" = ["*.pyi", "py.typed"{component_package_data}]

[tool.cibuildwheel]
# use local winrt-sdk build dependency
environment = {{ PYTHONPATH="{relative}/winrt-sdk/src{extra_python_path}" }}
# don't install winrt-sdk from PyPI
build-frontend = {{ name = "build[uv]", args = ["--skip-dependency-check", "--no-isolation"] }}
before-build = "uv pip install setuptools"
# don't build for PyPy
skip = "pp*"
# suppress warnings about ARM64 testing
test-skip = "*-win_arm64"

[tool.cibuildwheel.windows]
archs = ["x86", "AMD64", "ARM64"]{extra_cibuildwheel_windows}
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
from setuptools.command.build_ext import build_ext
from winrt_sdk import get_include_dirs{extra_imports}

{extra_init}
class build_ext_ex(build_ext):
    def build_extension(self, ext):
        if self.compiler.compiler_type == "msvc":
            ext.extra_compile_args = ["/std:c++20", "/permissive-"]
        elif self.compiler.compiler_type == "mingw32":
            ext.extra_compile_args = ["-std=c++20", "-D_WIN32_WINNT=_WIN32_WINNT_WIN10"]
            # GCC 15 complains about some things that it didn't in the past
            ext.extra_compile_args += ["-Wno-strict-aliasing", "-Wno-template-body"]
            ext.extra_link_args = ["-loleaut32"]
        else:
            raise ValueError(f"Unsupported compiler: {{self.compiler.compiler_type}}")
{extra_build}
        build_ext.build_extension(self, ext)


setup(
    cmdclass={{"build_ext": build_ext_ex}},
    ext_modules=[
        Extension(
            "{root_package}.{ext_module}",
            sources=[{sources}],
            include_dirs=get_include_dirs(){extra_include_dirs},
            libraries=["windowsapp"{extra_libraries}],
        ){extra_extension}
    ],
)
"""

EXTRA_EXT_MODULES = """,
        Extension(
            "{root_package}.{ext_module}",
            sources=[{sources}],
            include_dirs=get_include_dirs(){extra_include_dirs},
            libraries=["windowsapp"],
        ),
"""

APP_SDK_INIT = """
try:
    import os, pathlib

    WINDOWS_APP_SDK_PATH = pathlib.Path(os.environ["WINDOWS_APP_SDK_PATH"]).resolve()
    print(f"Using Windows App SDK from {WINDOWS_APP_SDK_PATH}")
except KeyError:
    raise RuntimeError("Please set WINDOWS_APP_SDK_PATH environment variable")
"""

APP_SDK_EXTRA_BUILD = """
        target = self.plat_name.replace("32", "-x86").replace("amd", "x").replace("win", "win10")
        ext.library_dirs = [os.fspath(WINDOWS_APP_SDK_PATH / "lib" / target)]
"""

README_TEMPLATE = """\
<!-- warning: Please don't edit this file. It was automatically generated. -->

# {package_name}

"""

BINARY_README_TEMPLATE = """\
{important}Windows Runtime (WinRT) APIs for for the `{namespace}` namespace.

This package provides the `{module_name}` module.
"""

WINUI3_README_IMPORTANT = """\
**IMPORTANT**: Packages in the `winui3-*` namespace cannot be used without the
Windows App Runtime. This has to be installed manually by the end user. Read the
[PyWinRT winui3 documentation](https://pywinrt.readthedocs.io/en/latest/api/winui3/index.html)
for more information.

"""

SDK_README_TEMPLATE = """\
This package provides PyWinRT header files for {sdk_name}.

This package is a build time dependency, not a runtime dependency.

Example use in a `pyproject.toml` file:

```toml
[build-system]
requires = ["setuptools", "winrt-sdk"{extra_requires}]
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
    return name in ["winrt-sdk", "winrt-Microsoft.UI.Xaml", "winrt-WindowsAppSDK"]


def is_app_sdk_interop_package(name: str) -> bool:
    return name in [
        "winui3-Microsoft.UI.Interop",
        "winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap",
    ]


def is_app_sdk_bootstrap_package(name: str) -> bool:
    return (
        name == "winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap"
    )


def is_webview2_package(name: str) -> bool:
    return name.startswith("webview2-Microsoft.Web.WebView2.")


def is_microsoft_ui_xaml_package(name: str) -> bool:
    return name.startswith("winui2-Microsoft.UI.Xaml")


def is_windows_app_package(name: str) -> bool:
    return name.startswith("winui3-Microsoft.") or is_app_sdk_interop_package(name)


def is_component_package(name: str) -> bool:
    return name in ["test-winrt-TestComponent"] or is_webview2_package(name)


def is_dispatcher_queue_package(name: str) -> bool:
    return name == "winrt-Windows.System.Interop"


def is_direct3d11_package(name: str) -> bool:
    return name == "winrt-Windows.Graphics.DirectX.Direct3D11.Interop"


def winrt_ns_to_py_package(ns: str) -> str:
    return ".".join(avoid_keyword(x.lower()) for x in ns.split("."))


def write_project_files(
    package_path: Path,
    module_name: str,
    ext_module_name: str,
    sources: List[str],
    second_ext_source_file: str | None = None,
) -> None:
    package_name = package_path.name
    relative_package_path = package_path.relative_to(PROJECTION_PATH)
    relative = "/".join([".."] * len(relative_package_path.parts))
    has_requirements = (package_path / "requirements.txt").exists()
    has_all_requirements = (package_path / "all-requirements.txt").exists()
    root_package = module_name.split(".")[0]

    with open(package_path / "pyproject.toml", "w", newline="\n") as f:
        f.write(
            PYPROJECT_TOML_TEMPLATE.format(
                extra_requires=("" if is_sdk_package(package_name) else ', "winrt-sdk"')
                + (
                    ', "winrt-Microsoft.UI.Xaml"'
                    if is_microsoft_ui_xaml_package(package_name)
                    else ""
                )
                + (
                    ', "winrt-WindowsAppSDK"'
                    if is_windows_app_package(package_name)
                    else ""
                ),
                package_name=package_name,
                description=(
                    "Windows Runtime SDK for Python header files"
                    if is_sdk_package(package_name)
                    else "Python projection of Windows Runtime (WinRT) APIs"
                ),
                module_name=module_name,
                extra_dynamic=(', "dependencies"' if has_requirements else "")
                + (', "optional-dependencies"' if has_all_requirements else ""),
                dependencies=DEPENDENCIES if has_requirements else "",
                optional_dependencies=(
                    OPTIONAL_DEPENDENCIES if has_all_requirements else ""
                ),
                find_src=(
                    FIND_SRC
                    if (package_path / "src").exists()
                    else (
                        COMPONENT_PACKAGE_FIND_SRC
                        if is_component_package(package_name)
                        else ""
                    )
                ),
            )
        )

        if is_sdk_package(package_name):
            f.write(SDK_PACKAGE_TEMPLATE)
        else:
            f.write(
                BINARY_PACKAGE_TEMPLATE.format(
                    component_package_data=(
                        ', "*.h"' if is_component_package(package_name) else ""
                    ),
                    relative=relative,
                    extra_python_path=(
                        f";{relative}/winrt-Microsoft.UI.Xaml/src"
                        if is_microsoft_ui_xaml_package(package_name)
                        else ""
                    )
                    + (
                        f";{relative}/winrt-WindowsAppSDK/src"
                        if is_windows_app_package(package_name)
                        else ""
                    ),
                    extra_cibuildwheel_windows=(
                        '\nrepair-wheel-command = "python scripts/add_msvcp140_dll.py {wheel} {dest_dir}"'
                        if package_name == "winrt-runtime"
                        else (
                            '\nrepair-wheel-command = "python scripts/add_bootstrap_dll.py {wheel} {dest_dir}"'
                            if is_app_sdk_bootstrap_package(package_name)
                            else (
                                '\nrepair-wheel-command = "python scripts/add_webview2_dll.py {wheel} {dest_dir}"'
                                if is_webview2_package(package_name)
                                else ""
                            )
                        )
                    ),
                )
            )

    if not is_sdk_package(package_name):
        with open(package_path / "setup.py", "w", newline="\n") as f:
            f.write(
                SETUP_PY_TEMPLATE.format(
                    extra_init=(
                        APP_SDK_INIT if is_app_sdk_interop_package(package_name) else ""
                    ),
                    root_package=root_package,
                    ext_module=ext_module_name,
                    sources=", ".join(f'"{x}"' for x in sources),
                    extra_imports=(
                        "\nfrom winrt_microsoft_ui_xaml import get_include_dirs as get_winui2_include_dirs"
                        if is_microsoft_ui_xaml_package(package_name)
                        else ""
                    )
                    + (
                        "\nfrom winrt_windows_app_sdk import get_include_dirs as get_app_sdk_include_dirs"
                        if is_windows_app_package(package_name)
                        else ""
                    ),
                    extra_include_dirs=(
                        " + get_winui2_include_dirs()"
                        if is_microsoft_ui_xaml_package(package_name)
                        else ""
                    )
                    + (
                        "+ get_app_sdk_include_dirs()"
                        if is_windows_app_package(package_name)
                        else ""
                    )
                    + (
                        '+ [os.fspath(WINDOWS_APP_SDK_PATH / "include")]'
                        if is_app_sdk_interop_package(package_name)
                        else ""
                    )
                    + (
                        f' + ["./{root_package}/cppwinrt", "./{root_package}/pywinrt"]'
                        if is_component_package(package_name)
                        else ""
                    ),
                    extra_build=(
                        APP_SDK_EXTRA_BUILD
                        if is_app_sdk_interop_package(package_name)
                        else ""
                    ),
                    extra_libraries=(
                        ', "Microsoft.WindowsAppRuntime.Bootstrap"'
                        if is_app_sdk_bootstrap_package(package_name)
                        else ""
                    )
                    + (
                        ', "CoreMessaging"'
                        if is_dispatcher_queue_package(package_name)
                        else ""
                    )
                    + (', "D3D11"' if is_direct3d11_package(package_name) else ""),
                    extra_extension=(
                        EXTRA_EXT_MODULES.format(
                            root_package=root_package,
                            ext_module=f"{ext_module_name}_2",
                            sources=f'"{second_ext_source_file}"',
                            extra_include_dirs=(
                                " + get_winui2_include_dirs()"
                                if is_microsoft_ui_xaml_package(package_name)
                                else ""
                            )
                            + (
                                " + get_app_sdk_include_dirs()"
                                if is_windows_app_package(package_name)
                                else ""
                            ),
                        )
                        if second_ext_source_file
                        and (package_path / second_ext_source_file).exists()
                        else ""
                    ),
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
                        sdk_name=(
                            "the Windows SDK"
                            if package_name == "winrt-sdk"
                            else package_name.removeprefix("winrt-")
                        ),
                        extra_requires=(
                            "" if package_name == "winrt-sdk" else f', "{package_name}"'
                        ),
                        extra_imports=(
                            "\nfrom winrt_microsoft_ui_xaml import get_include_dirs as get_winui2_include_dirs"
                            if is_microsoft_ui_xaml_package(package_name)
                            else ""
                        )
                        + (
                            "\nfrom winrt_windows_app_sdk import get_include_dirs as get_app_sdk_include_dirs"
                            if is_windows_app_package(package_name)
                            else ""
                        ),
                        extra_include_dirs=(
                            " + get_winui2_include_dirs()"
                            if is_microsoft_ui_xaml_package(package_name)
                            else ""
                        )
                        + (
                            "+ get_app_sdk_include_dirs()"
                            if is_windows_app_package(package_name)
                            else ""
                        ),
                    )
                )
            else:
                f.write(
                    BINARY_README_TEMPLATE.format(
                        important=(
                            WINUI3_README_IMPORTANT
                            if is_windows_app_package(package_name)
                            else ""
                        ),
                        namespace=package_name.removeprefix("winrt-"),
                        module_name=module_name,
                    )
                )


write_project_files(
    PROJECTION_PATH / "winrt-runtime",
    "winrt.system",
    "_winrt",
    [
        "_winrt.cpp",
        "_winrt_array.cpp",
        "_winrt_box.cpp",
        "_winrt_buffer.cpp",
        "runtime.cpp",
    ],
)

for package_path in chain(
    (PROJECTION_PATH / "interop").glob("winrt-*"),
    (PROJECTION_PATH / "interop").glob("winui3-*"),
    (PROJECTION_PATH / "winrt").glob("winrt-*"),
    (PROJECTION_PATH / "winui2").glob("winui2-*"),
    (PROJECTION_PATH / "winui3").glob("winui3-*"),
    (PROJECTION_PATH / "webview2").glob("webview2-*"),
    [
        PROJECTION_PATH / "winrt-sdk",
        PROJECTION_PATH / "winrt-Microsoft.UI.Xaml",
        PROJECTION_PATH / "winrt-WindowsAppSDK",
        PROJECTION_PATH / "test-winrt" / "test-winrt-TestComponent",
    ],
):
    root_package = package_path.name[: package_path.name.rindex("-")]
    namespace = package_path.name.removeprefix(f"{root_package}-")
    module_name = (
        f"{root_package.replace('-', '_')}.{winrt_ns_to_py_package(namespace)}"
    )
    ext_module_name = f"_{module_name.replace('.', '_')}"
    source_file = f"py.{namespace}.cpp"
    second_ext_source_file = f"py.{namespace}_2.cpp"

    write_project_files(
        package_path,
        module_name,
        ext_module_name,
        [source_file],
        second_ext_source_file,
    )
