"""
The upstream family each interop module is released with.

A generated package belongs to the family whose directory under projection/
holds it. A package written by hand belongs to the Windows SDK unless it takes
something out of the NuGet packages of another family, and is then released
with that family, since what it carries moves whenever the family's pin does.
This says which family each of those NuGet packages is part of, and which of
them each interop module compiles against. scripts/packages.py says which
NuGet package a package that redistributes a .dll takes it out of.

scripts/versions.py imports this, so it imports no other script.
"""

from typing import NamedTuple


class NuGetPackage(NamedTuple):
    """
    A NuGet package that a build reads something out of.
    """

    name: str
    # the family whose release it is part of
    family: str


# The unpacked NuGet packages that a build reads something out of, named by
# the environment variable the build looks for. The Windows App SDK and
# WebView2 are redistributed with an app rather than part of Windows, so the
# interop modules that call them take their headers, and the App SDK's import
# libraries, from these. The App SDK is a metapackage over components published
# separately, so each module names the one it needs. WebView2 is also where its
# component .dll comes from.
# A build that finds no variable downloads the one package it needs itself,
# checked against a hash, as a build from the source distribution does, so
# pointing at an unpacked copy only keeps a build in this tree off the network.
NUGET_ENVIRONMENT = {
    "WASDK_FOUNDATION_PATH": NuGetPackage(
        "Microsoft.WindowsAppSDK.Foundation", "wasdk"
    ),
    "WASDK_INTERACTIVE_EXPERIENCES_PATH": NuGetPackage(
        "Microsoft.WindowsAppSDK.InteractiveExperiences", "wasdk"
    ),
    "WEBVIEW2_PATH": NuGetPackage("Microsoft.Web.WebView2", "webview2"),
}

# The NuGet packages each hand-written module compiles against, named by the
# environment variable above that says where fetch-tools.ps1 unpacked each
# one. The App SDK is a metapackage, so what a module includes is spread over
# several of them. Compiling against a family's packages is also what puts a
# module in that family: these are released with it rather than with the
# Windows SDK that the other interop modules follow.
INTEROP_NUGET_PACKAGES = {
    # Microsoft.UI.Interop.h
    "winrt-Microsoft.UI.Interop": ["WASDK_INTERACTIVE_EXPERIENCES_PATH"],
    # MddBootstrap.h and its import library; the release it expects is written
    # into the package by scripts/generate-pyproject.py
    "winrt-Microsoft.WindowsAppRuntime.Bootstrap": ["WASDK_FOUNDATION_PATH"],
    # WebView2Interop.h, which includes WebView2.h
    "winrt-Microsoft.Web.WebView2.Interop": ["WEBVIEW2_PATH"],
}

# The family of the interop modules that compile against no other family's
# NuGet packages, which are each released on their own.
WINDOWS_SDK_FAMILY = "winrt"


def interop_family(distribution: str) -> str:
    """
    The family a hand-written interop module is built against: the one its
    NuGet packages are part of, or the Windows SDK when it has none.
    """
    found = {
        NUGET_ENVIRONMENT[env].family
        for env in INTEROP_NUGET_PACKAGES.get(distribution, [])
    }

    if len(found) > 1:
        raise RuntimeError(
            f"{distribution} compiles against the NuGet packages of"
            f" {', '.join(sorted(found))}, and is released with only one family"
        )

    return found.pop() if found else WINDOWS_SDK_FAMILY


# The interop modules that are released with a family rather than on their
# own, keyed by directory under interop/: every one that compiles against a
# family other than the Windows SDK. Such a family's release is what its users
# choose a version of, so each package of it carries that version, and the
# interop modules that go with a set of its packages are the ones with their
# version. The Windows App SDK bootstrap module has to besides: its wheel
# carries Microsoft.WindowsAppRuntime.Bootstrap.dll out of the pin and loads
# that release of the App SDK unless told otherwise, so what it carries changes
# whenever the pin moves, even where its source does not. None of them has a
# version.txt. A fix to one between two pins adds one, which makes that package
# a unit of its own until the pin moves (see versions.interop_version()).
FAMILY_RELEASED_INTEROP = {
    name: family
    for name in INTEROP_NUGET_PACKAGES
    if (family := interop_family(name)) != WINDOWS_SDK_FAMILY
}
