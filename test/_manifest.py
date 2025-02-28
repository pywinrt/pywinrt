# mypy: ignore-errors
# Don't care so much about this file since it is just helpers and it fails
# if we run mypy on Linux on CI because of the wintypes imports.

import ctypes
import enum
import struct
import sys
import uuid
from ctypes import wintypes
from typing import Generator

"""
Helper functions for test_xaml_hosting.py
"""

if sys.version_info < (3, 12):
    wintypes.BYTE = ctypes.c_ubyte

QueryActCtxW = ctypes.WINFUNCTYPE(
    wintypes.BOOL,
    wintypes.DWORD,
    wintypes.HANDLE,
    ctypes.c_void_p,
    wintypes.ULONG,
    ctypes.c_void_p,
    ctypes.c_size_t,
    ctypes.POINTER(ctypes.c_size_t),
    use_last_error=True,
)(("QueryActCtxW", ctypes.windll.kernel32))


# https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-queryactctxw
class QUERY_ACTCTX_FLAG(enum.IntFlag):
    NONE = 0
    USE_ACTIVE_ACTCTX = 0x00000001
    ACTCTX_IS_HMODULE = 0x00000002
    ACTCTX_IS_ADDRESS = 0x00000004


ActivationContextBasicInformation = 1
ActivationContextDetailedInformation = 2
AssemblyDetailedInformationInActivationContext = 3
FileInformationInAssemblyOfAssemblyInActivationContext = 4
RunlevelInformationInActivationContext = 5
CompatibilityInformationInActivationContext = 6


# https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-activation_context_detailed_information
class ACTIVATION_CONTEXT_DETAILED_INFORMATION(ctypes.Structure):
    _fields_ = [
        ("dwFlags", wintypes.DWORD),
        ("ulFormatVersion", wintypes.DWORD),
        ("ulAssemblyCount", wintypes.DWORD),
        ("ulRootManifestPathType", wintypes.DWORD),
        ("ulRootManifestPathChars", wintypes.DWORD),
        ("ulRootConfigurationPathType", wintypes.DWORD),
        ("ulRootConfigurationPathChars", wintypes.DWORD),
        ("ulAppDirPathType", wintypes.DWORD),
        ("ulAppDirPathChars", wintypes.DWORD),
        ("lpRootManifestPath", wintypes.LPCWSTR),
        ("lpRootConfigurationPath", wintypes.LPCWSTR),
        ("lpAppDirPath", wintypes.LPCWSTR),
    ]


class GUID(ctypes.Structure):
    _fields_ = [
        ("Data1", wintypes.DWORD),
        ("Data2", wintypes.WORD),
        ("Data3", wintypes.WORD),
        ("Data4", (wintypes.BYTE * 8)),
    ]

    def to_uuid(self) -> uuid.UUID:
        return uuid.UUID(
            fields=(
                self.Data1,
                self.Data2,
                self.Data3,
                self.Data4[0],
                self.Data4[1],
                int.from_bytes(self.Data4[2:], "big"),
            )
        )


# https://learn.microsoft.com/en-us/windows/win32/api/winnt/ne-winnt-actctx_compatibility_element_type
class ACTCTX_COMPATIBILITY_ELEMENT_TYPE(enum.IntEnum):
    UNKNOWN = 0
    OS = 1
    MITIGATION = 2
    MAXVERSIONTESTED = 3


# https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-compatibility_context_element
class COMPATIBILITY_CONTEXT_ELEMENT(ctypes.Structure):
    _fields_ = [
        ("Id", GUID),
        ("Type", wintypes.UINT),
        ("MaxVersionTested", ctypes.c_ulonglong),
    ]


# https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-activation_context_compatibility_information
def ACTIVATION_CONTEXT_COMPATIBILITY_INFORMATION(size: int):
    count = (size - ctypes.sizeof(wintypes.DWORD)) // ctypes.sizeof(
        COMPATIBILITY_CONTEXT_ELEMENT
    )

    class ACTIVATION_CONTEXT_COMPATIBILITY_INFORMATION(ctypes.Structure):
        _fields_ = [
            ("ElementCount", wintypes.DWORD),
            ("Elements", COMPATIBILITY_CONTEXT_ELEMENT * count),
        ]

    return ACTIVATION_CONTEXT_COMPATIBILITY_INFORMATION


# https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
CreateWindowExW = ctypes.WINFUNCTYPE(
    wintypes.HWND,
    wintypes.DWORD,
    wintypes.LPCWSTR,
    wintypes.LPCWSTR,
    wintypes.HWND,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    wintypes.HWND,
    wintypes.HMENU,
    wintypes.HINSTANCE,
    ctypes.c_void_p,
    use_last_error=True,
)(("CreateWindowExW", ctypes.windll.user32))

# https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-
ERROR_INSUFFICIENT_BUFFER = 122


def unpack_version(version: int) -> tuple[int, int, int, int]:
    return struct.unpack(">HHHH", version.to_bytes(8, "big"))


def get_activation_context_detailed_information() -> tuple[str, str, str]:
    size = ctypes.c_size_t()

    # First time to get size
    if QueryActCtxW(
        QUERY_ACTCTX_FLAG.USE_ACTIVE_ACTCTX,
        None,
        None,
        ActivationContextDetailedInformation,
        None,
        0,
        ctypes.byref(size),
    ):
        raise RuntimeError("should have failed")

    err = ctypes.get_last_error()
    if err != ERROR_INSUFFICIENT_BUFFER:
        raise ctypes.WinError(err)

    buffer = ctypes.create_string_buffer(size.value)

    if not QueryActCtxW(
        QUERY_ACTCTX_FLAG.USE_ACTIVE_ACTCTX,
        None,
        None,
        ActivationContextDetailedInformation,
        buffer,
        len(buffer),
        None,
    ):
        raise ctypes.WinError(ctypes.get_last_error())

    info = ACTIVATION_CONTEXT_DETAILED_INFORMATION.from_buffer_copy(buffer)

    return info.lpRootManifestPath, info.lpRootConfigurationPath, info.lpAppDirPath


def get_activation_context_compatibility_information() -> Generator[
    tuple[uuid.UUID, ACTCTX_COMPATIBILITY_ELEMENT_TYPE, tuple[int, int, int, int]],
    None,
    None,
]:
    size = ctypes.c_size_t()

    # First time to get size
    if QueryActCtxW(
        QUERY_ACTCTX_FLAG.USE_ACTIVE_ACTCTX,
        None,
        None,
        ActivationContextDetailedInformation,
        None,
        0,
        ctypes.byref(size),
    ):
        raise RuntimeError("should have failed")

    err = ctypes.get_last_error()
    if err != ERROR_INSUFFICIENT_BUFFER:
        raise ctypes.WinError(err)

    info = ACTIVATION_CONTEXT_COMPATIBILITY_INFORMATION(size.value)()

    if not QueryActCtxW(
        QUERY_ACTCTX_FLAG.USE_ACTIVE_ACTCTX,
        None,
        None,
        CompatibilityInformationInActivationContext,
        ctypes.byref(info),
        ctypes.sizeof(info),
        None,
    ):
        raise ctypes.WinError(ctypes.get_last_error())

    for element in info.Elements[: info.ElementCount]:
        yield (
            element.Id.to_uuid(),
            ACTCTX_COMPATIBILITY_ELEMENT_TYPE(element.Type),
            unpack_version(element.MaxVersionTested),
        )


def is_maxversiontested_in_manifest() -> bool:
    for _, type, _ in get_activation_context_compatibility_information():
        if type == ACTCTX_COMPATIBILITY_ELEMENT_TYPE.MAXVERSIONTESTED:
            return True

    return False


def create_window():
    # Use "Button" builtin class to avoid registering a new window class. This
    # is just to get a hwnd and we don't actually display it.
    hwnd = CreateWindowExW(0, "Button", None, None, 0, 0, 0, 0, None, None, None, None)
    if not hwnd:
        raise ctypes.WinError(ctypes.get_last_error())
    return hwnd
