# WARNING: Please don't edit this file. It was automatically generated.
# setuptools does not annotate the build_ext methods this file overrides.
# pyright: reportUnknownMemberType=false

import hashlib
import io
import os
import pathlib
import shutil
import sysconfig
import urllib.request
import zipfile

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

INCLUDE_DIRS: list[str] = []

# Each NuGet package the module compiles against, named by the environment
# variable that points at an unpacked copy of it: what it is unpacked as, where
# nuget.org serves it, what it has to hash to and where in it the headers are.
NUGET_PACKAGES = {
    "WEBVIEW2_PATH": (
        "Microsoft.Web.WebView2.1.0.4314.35",
        "https://api.nuget.org/v3-flatcontainer/microsoft.web.webview2/1.0.4314.35/microsoft.web.webview2.1.0.4314.35.nupkg",
        "90778deb1b77e36292828b4b5833c9255c27533f40e0ddd55891ae718944edc4",
        ["build/native/include", "build/native/include-winrt"],
    ),
}


def unpack_nuget_package(env: str) -> pathlib.Path:
    """
    Where the package named by @p env is unpacked: the copy the environment
    variable points at, or else one downloaded from nuget.org into build/.
    """
    if env in os.environ:
        return pathlib.Path(os.environ[env]).resolve()

    name, url, sha256, _ = NUGET_PACKAGES[env]
    path = pathlib.Path(__file__).parent.resolve() / "build" / "nuget" / name

    if path.is_dir():
        return path

    try:
        with urllib.request.urlopen(url, timeout=120) as response:
            data = response.read()
    except OSError as error:
        raise RuntimeError(
            f"could not download {url}: {error}. Set {env} to an unpacked"
            f" copy of {name} to build without reaching the network."
        ) from error

    digest = hashlib.sha256(data).hexdigest()

    if digest != sha256:
        raise RuntimeError(f"{url} hashes to {digest} and not to {sha256}")

    # Unpacked beside where it goes and then moved there, so that a build
    # stopped halfway leaves nothing the next one would take for unpacked.
    partial = path.with_name(path.name + ".partial")
    shutil.rmtree(partial, ignore_errors=True)

    with zipfile.ZipFile(io.BytesIO(data)) as package:
        package.extractall(partial)

    partial.rename(path)

    return path


NUGET_PATHS: dict[str, pathlib.Path] = {}

for name, (_, _, _, include_dirs) in NUGET_PACKAGES.items():
    NUGET_PATHS[name] = unpack_nuget_package(name)
    print(f"Using {name} from {NUGET_PATHS[name]}")
    INCLUDE_DIRS.extend(os.fspath(NUGET_PATHS[name] / d) for d in include_dirs)

# compiled against the stable ABI of Python 3.11, except where the interpreter
# is free-threaded, which has none, or MSYS2's, whose modules link against the
# version's own DLL
STABLE_ABI = not (
    sysconfig.get_config_var("Py_GIL_DISABLED")
    or sysconfig.get_platform().startswith("mingw")
)


class build_ext_ex(build_ext):
    def build_extension(self, ext: Extension) -> None:
        # nothing in the projection uses dynamic_cast or typeid, so disabling
        # RTTI costs nothing and makes the module about 15% smaller
        if self.compiler.compiler_type == "msvc":
            ext.extra_compile_args = ["/std:c++20", "/permissive-", "/GR-"]
        elif self.compiler.compiler_type == "mingw32":
            ext.extra_compile_args = [
                "-std=c++20",
                "-fno-rtti",
                "-D_WIN32_WINNT=_WIN32_WINNT_WIN10",
            ]
            # GCC 15 complains about some things that it didn't in the past
            ext.extra_compile_args += ["-Wno-strict-aliasing", "-Wno-template-body"]
            ext.extra_link_args = ["-loleaut32"]
        else:
            raise ValueError(f"Unsupported compiler: {self.compiler.compiler_type}")

        build_ext.build_extension(self, ext)


setup(
    cmdclass={"build_ext": build_ext_ex},
    ext_modules=[
        Extension(
            "winrt._winrt_microsoft_web_webview2_interop",
            sources=["py.Microsoft.Web.WebView2.Interop.cpp"],
            include_dirs=INCLUDE_DIRS,
            libraries=["windowsapp", "ole32", "user32"],
            define_macros=[("Py_LIMITED_API", "0x030B0000")] if STABLE_ABI else [],
            py_limited_api=STABLE_ABI,
        )
    ],
    options={"bdist_wheel": {"py_limited_api": "cp311"}} if STABLE_ABI else {},
)
