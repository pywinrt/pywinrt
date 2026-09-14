from ctypes import HRESULT as _HRESULT

"""Common HRESULT error codes."""

# NB: have to use ctypes.HRESULT to convert unsigned values to signed int


# https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-
_ERROR_FILE_NOT_FOUND = 2

# https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--1000-1299-
_ERROR_CANCELLED = 1223


# https://learn.microsoft.com/en-us/windows/win32/api/winerror/nf-winerror-hresult_facility
_FACILITY_WIN32 = 7


# https://learn.microsoft.com/en-us/windows/win32/api/winerror/nf-winerror-hresult_from_win32
def _HRESULT_FROM_WIN32(error_code: int) -> _HRESULT:
    x = _HRESULT(error_code).value
    return _HRESULT(
        x if x <= 0 else ((x & 0x0000FFFF) | (_FACILITY_WIN32 << 16) | 0x80000000)
    )


# common errors from CppWinRT
S_OK = 0
E_FAIL = _HRESULT(0x80004005).value
E_ACCESSDENIED = _HRESULT(0x80070005).value
E_NOTIMPL = _HRESULT(0x80004001).value
E_INVALIDARG = _HRESULT(0x80070057).value
E_BOUNDS = _HRESULT(0x8000000B).value
E_NOINTERFACE = _HRESULT(0x80004002).value
E_CHANGED_STATE = _HRESULT(0x8000000C).value
E_ILLEGAL_METHOD_CALL = _HRESULT(0x8000000E).value
E_ILLEGAL_STATE_CHANGE = _HRESULT(0x8000000D).value
E_ILLEGAL_DELEGATE_ASSIGNMENT = _HRESULT(0x80000018).value
E_OUTOFMEMORY = _HRESULT(0x8007000E).value
CLASS_E_CLASSNOTAVAILABLE = _HRESULT(0x80040111).value
CO_E_NOTINITIALIZED = _HRESULT(0x800401F0).value
REGDB_E_CLASSNOTREG = _HRESULT(0x80040154).value
RPC_E_WRONG_THREAD = _HRESULT(0x8001010E).value
WIN32_ERROR_FILE_NOT_FOUND = _HRESULT_FROM_WIN32(_ERROR_FILE_NOT_FOUND).value
WIN32_ERROR_CANCELLED = _HRESULT_FROM_WIN32(_ERROR_CANCELLED).value

# Errors defined by PyWinRT
PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION = _HRESULT(0xA0EE4005).value
