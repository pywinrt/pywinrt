from importlib.resources import files
from typing import List


def get_include_dirs() -> List[str]:
    """
    Gets the include directories for Microsoft.UI.Xaml (winui2).

    This is intended to be used with the ``include_dirs`` argument of
    ``setuptools.setup().``
    """
    package_dir = files("winrt_microsoft_ui_xaml")

    try:
        webview2_dir = files("webview2")
    except ModuleNotFoundError:
        # hack for building on CI without binary packages
        webview2_dir = package_dir.joinpath(
            "..",
            "..",
            "..",
            "webview2",
            "webview2-Microsoft.Web.WebView2.Core",
            "webview2",
        )

    return [str(package_dir.joinpath(d)) for d in ["cppwinrt", "pywinrt"]] + [
        str(webview2_dir.joinpath(d)) for d in ["cppwinrt", "pywinrt"]
    ]
