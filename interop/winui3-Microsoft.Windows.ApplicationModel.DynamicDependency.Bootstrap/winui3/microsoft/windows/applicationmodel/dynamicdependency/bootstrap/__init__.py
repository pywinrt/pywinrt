import enum

from winui3 import (
    _winui3_microsoft_windows_applicationmodel_dynamicdependency_bootstrap as bootstrap,
)

__all__ = ["InitializeOptions", "initialize"]


class InitializeOptions(enum.IntFlag):
    NONE = 0
    """Default behavior"""

    ON_ERROR_DEBUG_BREAK = 1
    """If not successful call DebugBreak()"""

    ON_ERROR_DEBUG_BREAK_IF_DEBUGGER_ATTACHED = 2
    """If not successful call DebugBreak() if a debugger is attached to the process"""

    ON_ERROR_FAIL_FAST = 4
    """If not successful perform a fail-fast"""

    ON_NO_MATCH_SHOW_UI = 8
    """If a compatible Windows App Runtime framework package is not found show UI"""

    ON_PACKAGE_IDENTITY_NOOP = 16
    """Do nothing (do not error) if the process has package identity"""


def initialize(
    version: str = bootstrap.RELEASE_VERSION,
    min_version: str = bootstrap.RUNTIME_VERSION,
    options: InitializeOptions = InitializeOptions.NONE,
) -> bootstrap.Shutdown:
    """
    Initialize the Windows App SDK runtime.

    Args:
        version: The Windows App SDK *product* version to load, e.g. "1.5" or "1.5-preview".
        min_version: The minimum version of the Windows App SDK *runtime* to load, e.g. "5001.70.1338.0".
        options: Initialization option flags.

    Returns:
        A context manager that will shutdown the Windows App SDK runtime when exited.
    """
    # Split the version into major.minor and optional tag.
    try:
        ver, tag = version.split("-")
    except ValueError:
        ver, tag = version, None

    ver_maj, ver_min = ver.split(".")

    # encode in format required by low-level API
    ver_maj_min = (int(ver_maj) << 16) | int(ver_min)

    min_ver_maj, min_ver_min, min_ver_patch, min_ver_build = min_version.split(".")

    # encode in format required by low-level API
    min_ver_quad = (
        (int(min_ver_maj) << 48)
        | (int(min_ver_min) << 32)
        | (int(min_ver_patch) << 16)
        | int(min_ver_build)
    )

    return bootstrap.initialize(ver_maj_min, tag, min_ver_quad, options)
