# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winui3._winui3_microsoft_windows_applicationmodel_windowsappruntime import (
    WindowsAppRuntimeVersion,
    DeploymentInitializeOptions,
    DeploymentManager,
    DeploymentResult,
    ReleaseInfo,
    RuntimeCompatibilityOptions,
    RuntimeInfo,
)

__all__ = [
    "DeploymentStatus",
    "RuntimeCompatibilityChange",
    "WindowsAppRuntimeVersion",
    "DeploymentInitializeOptions",
    "DeploymentManager",
    "DeploymentResult",
    "ReleaseInfo",
    "RuntimeCompatibilityOptions",
    "RuntimeInfo",
]

class DeploymentStatus(enum.IntEnum):
    UNKNOWN = 0
    OK = 1
    PACKAGE_INSTALL_REQUIRED = 2
    PACKAGE_INSTALL_FAILED = 3

class RuntimeCompatibilityChange(enum.IntEnum):
    NONE = 0

