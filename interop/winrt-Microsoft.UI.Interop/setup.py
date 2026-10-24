# WARNING: Please don't edit this file. It was automatically generated.
# setuptools does not annotate the build_ext methods this file overrides.
# pyright: reportUnknownMemberType=false

import hashlib
import io
import os
import pathlib
import shutil
import urllib.request
import zipfile

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

INCLUDE_DIRS = [
    os.fspath(pathlib.Path(__file__).parent / "cppwinrt"),
]

# Each Windows App SDK component the module compiles against, named by the
# environment variable that points at an unpacked copy of it: what it is
# unpacked as, where nuget.org serves it, and what it has to hash to.
APP_SDK_PACKAGES = {
    "WASDK_INTERACTIVE_EXPERIENCES_PATH": (
        "Microsoft.WindowsAppSDK.InteractiveExperiences.2.1.9",
        "https://api.nuget.org/v3-flatcontainer/microsoft.windowsappsdk.interactiveexperiences/2.1.9/microsoft.windowsappsdk.interactiveexperiences.2.1.9.nupkg",
        "f9159d73e3e46717f159606e43b4acf8336aaff5bdd4d395fe3a93fdb41437ac",
    ),
}


def unpack_app_sdk_package(env: str) -> pathlib.Path:
    """
    Where the component named by @p env is unpacked: the copy the environment
    variable points at, or else one downloaded from nuget.org into build/.
    """
    if env in os.environ:
        return pathlib.Path(os.environ[env]).resolve()

    name, url, sha256 = APP_SDK_PACKAGES[env]
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


WINDOWS_APP_SDK_PATHS: dict[str, pathlib.Path] = {}

for name in APP_SDK_PACKAGES:
    WINDOWS_APP_SDK_PATHS[name] = unpack_app_sdk_package(name)
    print(f"Using {name} from {WINDOWS_APP_SDK_PATHS[name]}")
    INCLUDE_DIRS.append(os.fspath(WINDOWS_APP_SDK_PATHS[name] / "include"))


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
            "winrt._winrt_microsoft_ui_interop",
            sources=["py.Microsoft.UI.Interop.cpp"],
            include_dirs=INCLUDE_DIRS,
            libraries=["windowsapp"],
        )
    ],
)
