# WARNING: Please don't edit this file. It was automatically generated.

import hashlib
import io
import json
import os
import shutil
import sysconfig
import urllib.request
import zipfile
from pathlib import Path
from typing import Any

from hatchling.builders.hooks.plugin.interface import BuildHookInterface

NUGET_PACKAGE = "Microsoft.Web.WebView2"
NUGET_VERSION = "1.0.4191.47"

# Where the .nupkg comes from and what it has to hash to. The build fetches it
# rather than the source distribution carrying the .dll, so that nothing
# calling itself source holds a prebuilt binary; the checksum, taken when this
# file was generated, is what makes the download worth trusting.
NUGET_URL = "https://api.nuget.org/v3-flatcontainer/microsoft.web.webview2/1.0.4191.47/microsoft.web.webview2.1.0.4191.47.nupkg"
NUGET_SHA256 = "f492bbf547d0da329553b6727435b677579b1e9f91cc9e4a1ad029366d5f23d0"

# An unpacked copy of the NuGet package. scripts/build-bdist.py and
# scripts/build-sdist.py point this at what scripts/fetch-tools.ps1
# downloaded, and it is the escape hatch for a machine that cannot reach
# nuget.org: nothing is downloaded when it is set.
NUGET_PATH_ENV = "WEBVIEW2_PATH"

# Where what the build took out of the NuGet package is kept, so that a second
# build needs neither the network nor the unpacked copy again.
PAYLOAD_DIR = "nuget"

DLL_NAME = "Microsoft.Web.WebView2.Core.dll"
LICENSE_ID = "LicenseRef-Microsoft-Web-WebView2"
MODULE_PATH = "winrt/microsoft/web/webview2/dll"
SBOM_NAME = "cyclonedx.json"

# Where in the NuGet package the .dll of each architecture is, keyed by the
# platform tag of the wheel that carries it.
DLL_PATHS = {
    "win32": "runtimes/win-x86/native_uap/Microsoft.Web.WebView2.Core.dll",
    "win_amd64": "runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll",
    "win_arm64": "runtimes/win-arm64/native_uap/Microsoft.Web.WebView2.Core.dll",
}

# Which architecture to build for. There is one wheel per architecture, so
# scripts/build-bdist.py says which; a user building the source distribution
# builds for the machine they are on.
PLAT_NAME_ENV = "PYWINRT_PLAT_NAME"


def get_plat_name() -> str:
    """
    The platform tag of the wheel being built.
    """
    plat_name = os.environ.get(PLAT_NAME_ENV) or sysconfig.get_platform()
    plat_name = plat_name.replace("-", "_").replace(".", "_")

    if plat_name not in DLL_PATHS:
        raise RuntimeError(f"{NUGET_PACKAGE} ships no .dll for {plat_name}")

    return plat_name


def download_nuget_package() -> bytes:
    """
    The .nupkg, checked against the hash this file was generated with.
    """
    try:
        with urllib.request.urlopen(NUGET_URL, timeout=120) as response:
            data = response.read()
    except OSError as error:
        raise RuntimeError(
            f"could not download {NUGET_URL}: {error}."
            f" Set {NUGET_PATH_ENV} to an unpacked copy of {NUGET_PACKAGE}"
            f" {NUGET_VERSION} to build without reaching the network."
        ) from error

    digest = hashlib.sha256(data).hexdigest()

    if digest != NUGET_SHA256:
        raise RuntimeError(f"{NUGET_URL} hashes to {digest} and not to {NUGET_SHA256}")

    return data


def get_sbom() -> dict[str, Any]:
    """
    The bill of materials that PEP 770 puts in the wheel.

    Two parties are named and kept apart. The distribution is PyWinRT's, and
    PyWinRT is an independent project rather than a Microsoft one, so it is
    the root component and Microsoft is not its supplier. The .dll inside it
    is Microsoft's, under Microsoft's own license, and is a component of that
    root rather than the root itself.
    """
    return {
        "$schema": "http://cyclonedx.org/schema/bom-1.6.schema.json",
        "bomFormat": "CycloneDX",
        "specVersion": "1.6",
        "metadata": {
            "component": {
                "type": "library",
                "name": "winrt-Microsoft.Web.WebView2.Dll",
                "version": "4!1.0.4191.47",
                "purl": "pkg:pypi/winrt-microsoft-web-webview2-dll@4!1.0.4191.47",
                "licenses": [{"expression": "MIT"}],
                "supplier": {"name": "PyWinRT"},
            }
        },
        "components": [
            {
                "type": "library",
                "name": DLL_NAME,
                "version": NUGET_VERSION,
                "purl": f"pkg:nuget/{NUGET_PACKAGE}@{NUGET_VERSION}",
                "licenses": [{"license": {"name": LICENSE_ID}}],
                "supplier": {"name": "Microsoft Corporation"},
            }
        ],
    }


class DllBuildHook(BuildHookInterface):
    """
    Puts the redistributed .dll into the wheel being built.
    """

    PLUGIN_NAME = "custom"

    def initialize(self, version: str, build_data: dict[str, Any]) -> None:
        if self.target_name != "wheel":
            return

        plat_name = get_plat_name()
        payload_path = Path(self.root) / PAYLOAD_DIR

        # A .dll is data rather than an extension module, so one wheel serves
        # every version of Python and only the architecture narrows it.
        build_data["tag"] = f"py3-none-{plat_name}"
        build_data["pure_python"] = False
        build_data["force_include"][
            os.fspath(self.stage_dll(payload_path, DLL_PATHS[plat_name]))
        ] = f"{MODULE_PATH}/{DLL_NAME}"

        sbom_path = payload_path / SBOM_NAME
        sbom_path.write_text(
            json.dumps(get_sbom(), indent=2) + "\n",
            encoding="utf-8",
            newline="\n",
        )
        build_data["sbom_files"].append(f"{PAYLOAD_DIR}/{SBOM_NAME}")

    def stage_dll(self, payload_path: Path, relative: str) -> Path:
        """
        Takes one architecture's .dll out of the NuGet package.
        """
        destination = payload_path / relative

        if destination.is_file():
            return destination

        destination.parent.mkdir(parents=True, exist_ok=True)

        nuget_path = os.environ.get(NUGET_PATH_ENV)

        if nuget_path is not None:
            shutil.copyfile(Path(nuget_path) / relative, destination)

            return destination

        # A .nupkg is a zip, and the one nuget.org serves is byte for byte the
        # one that unpacking it leaves behind, so the same hash covers both.
        with zipfile.ZipFile(io.BytesIO(download_nuget_package())) as package:
            destination.write_bytes(package.read(relative))

        return destination
