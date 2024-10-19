# SPDX-License-Identifier: MIT
# Copyright (c) 2024 David Lechner <david@pybricks.com>

import ctypes
from ctypes import wintypes
import uuid


class GUID(ctypes.Structure):
    _fields_ = [
        ("Data1", ctypes.c_ulong),
        ("Data2", ctypes.c_ushort),
        ("Data3", ctypes.c_ushort),
        ("Data4", ctypes.c_ubyte * 8),
    ]


class IUnknown(ctypes.c_void_p):
    QueryInterface = ctypes.WINFUNCTYPE(
        int, ctypes.POINTER(GUID), ctypes.POINTER(wintypes.LPVOID)
    )(0, "QueryInterface")
    AddRef = ctypes.WINFUNCTYPE(wintypes.ULONG)(1, "AddRef")
    Release = ctypes.WINFUNCTYPE(wintypes.ULONG)(2, "Release")

    def query_interface(self, iid: uuid.UUID | str) -> "IUnknown":
        if isinstance(iid, str):
            iid = uuid.UUID(iid)

        ppv = wintypes.LPVOID()
        _iid = GUID.from_buffer_copy(iid.bytes_le)
        ret = self.QueryInterface(self, ctypes.byref(_iid), ctypes.byref(ppv))

        if ret:
            raise ctypes.WinError(ret)

        return IUnknown(ppv.value)

    def __del__(self):
        IUnknown.Release(self)
