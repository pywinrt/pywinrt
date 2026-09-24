"""
The little bit of NuGet that the tooling here reads directly.

scripts/fetch-tools.ps1 installs packages with nuget.exe, which is the right
tool for a package that is going to be unpacked whole. What is left over is
asking the feed what exists and reading one file out of a package that is too
big to download for it, and that is what this is for.
"""

import io
import json
import urllib.request
import xml.etree.ElementTree as ET
import zipfile

FLAT_CONTAINER = "https://api.nuget.org/v3-flatcontainer"

NUSPEC_NS = {"": "http://schemas.microsoft.com/packaging/2013/05/nuspec.xsd"}


class HttpFile(io.RawIOBase):
    """
    The seekable file that ``zipfile`` wants, served by range requests.

    A nupkg is a zip and a zip is read from its end, so one file can be taken
    out of one without downloading the rest. Microsoft.WindowsAppSDK.Runtime
    is 161 MB and PyWinRT needs one header from it.
    """

    def __init__(self, url: str) -> None:
        self.url = url
        self.position = 0

        with urllib.request.urlopen(
            urllib.request.Request(url, method="HEAD")
        ) as response:
            self.length = int(response.headers["Content-Length"])

    def readable(self) -> bool:
        return True

    def seekable(self) -> bool:
        return True

    def tell(self) -> int:
        return self.position

    def seek(self, offset: int, whence: int = io.SEEK_SET) -> int:
        if whence == io.SEEK_SET:
            self.position = offset
        elif whence == io.SEEK_CUR:
            self.position += offset
        else:
            self.position = self.length + offset

        return self.position

    def read(self, size: int = -1) -> bytes:
        if size < 0:
            size = self.length - self.position

        if size == 0 or self.position >= self.length:
            return b""

        last = min(self.position + size, self.length) - 1
        request = urllib.request.Request(
            self.url, headers={"Range": f"bytes={self.position}-{last}"}
        )

        with urllib.request.urlopen(request) as response:
            data = response.read()

        self.position += len(data)

        return data


def package_url(package: str, version: str, suffix: str) -> str:
    name = package.lower()

    return f"{FLAT_CONTAINER}/{name}/{version}/{name}{suffix}"


def stable_versions(package: str) -> list[str]:
    """
    Every version of @p package that carries no prerelease label.
    """
    with urllib.request.urlopen(f"{FLAT_CONTAINER}/{package.lower()}/index.json") as f:
        versions = json.load(f)

    return [v for v in versions["versions"] if "-" not in v]


def dependencies(package: str, version: str) -> dict[str, str]:
    """
    What @p package depends on, as the lowest version each dependency admits.

    A nuspec spells a dependency as a minimum or as a range, and both are read
    here as the version the release was built against.
    """
    with urllib.request.urlopen(package_url(package, version, ".nuspec")) as f:
        nuspec = ET.parse(f).getroot()

    result: dict[str, str] = {}

    for dependency in nuspec.iterfind(".//dependency", NUSPEC_NS):
        spec = dependency.attrib["version"]

        # "[2.1.74, 3.0.0)" and "[2.5.1]" both start at their lower bound
        floor = spec.lstrip("[(").split(",")[0].rstrip("])").strip()

        result[dependency.attrib["id"]] = floor

    return result


def read_file(package: str, version: str, path: str) -> bytes:
    """
    Reads one file out of a package without downloading the whole of it.
    """
    url = package_url(package, version, f".{version}.nupkg")

    with zipfile.ZipFile(HttpFile(url)) as nupkg:
        return nupkg.read(path)
