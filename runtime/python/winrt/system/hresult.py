"""Common HRESULT error codes."""

from winrt.windows.foundation import HResult

# https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-
_ERROR_FILE_NOT_FOUND = 2

# https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--1000-1299-
_ERROR_CANCELLED = 1223


# https://learn.microsoft.com/en-us/windows/win32/api/winerror/nf-winerror-hresult_facility
_FACILITY_WIN32 = 7


# https://learn.microsoft.com/en-us/windows/win32/api/winerror/nf-winerror-hresult_from_win32
def _HRESULT_FROM_WIN32(error_code: int) -> HResult:
    x = HResult(error_code)
    return (
        x
        if x <= 0
        else HResult((x & 0x0000FFFF) | (_FACILITY_WIN32 << 16) | 0x80000000)
    )


# common errors from CppWinRT
S_OK = HResult(0)
E_FAIL = HResult(0x80004005)
E_ACCESSDENIED = HResult(0x80070005)
E_NOTIMPL = HResult(0x80004001)
E_INVALIDARG = HResult(0x80070057)
E_BOUNDS = HResult(0x8000000B)
E_NOINTERFACE = HResult(0x80004002)
E_CHANGED_STATE = HResult(0x8000000C)
E_ILLEGAL_METHOD_CALL = HResult(0x8000000E)
E_ILLEGAL_STATE_CHANGE = HResult(0x8000000D)
E_ILLEGAL_DELEGATE_ASSIGNMENT = HResult(0x80000018)
E_OUTOFMEMORY = HResult(0x8007000E)
CLASS_E_CLASSNOTAVAILABLE = HResult(0x80040111)
CO_E_NOTINITIALIZED = HResult(0x800401F0)
REGDB_E_CLASSNOTREG = HResult(0x80040154)
RPC_E_WRONG_THREAD = HResult(0x8001010E)
WIN32_ERROR_FILE_NOT_FOUND = _HRESULT_FROM_WIN32(_ERROR_FILE_NOT_FOUND)
WIN32_ERROR_CANCELLED = _HRESULT_FROM_WIN32(_ERROR_CANCELLED)

# Errors defined by PyWinRT
PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION = HResult(0xA0EE4005)
