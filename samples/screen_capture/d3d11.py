# SPDX-License-Identifier: MIT
# Copyright (c) 2024 David Lechner <david@pybricks.com>

import contextlib
import ctypes
import enum
from ctypes import wintypes

from .iunknown import IUnknown

__all__ = [
    "D3D_DRIVER_TYPE",
    "D3D11_CREATE_DEVICE_FLAG",
    "D3D_FEATURE_LEVEL",
    "D3D11CreateDevice",
]

IN = 1
OUT = 2

# https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type
#
# typedef enum D3D_DRIVER_TYPE {
#   D3D_DRIVER_TYPE_UNKNOWN = 0,
#   D3D_DRIVER_TYPE_HARDWARE,
#   D3D_DRIVER_TYPE_REFERENCE,
#   D3D_DRIVER_TYPE_NULL,
#   D3D_DRIVER_TYPE_SOFTWARE,
#   D3D_DRIVER_TYPE_WARP
# } ;


class D3D_DRIVER_TYPE(enum.IntEnum):
    UNKNOWN = 0
    HARDWARE = 1
    REFERENCE = 2
    NULL = 3
    SOFTWARE = 4
    WARP = 5


# https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag
#
# typedef enum D3D11_CREATE_DEVICE_FLAG {
#   D3D11_CREATE_DEVICE_SINGLETHREADED = 0x1,
#   D3D11_CREATE_DEVICE_DEBUG = 0x2,
#   D3D11_CREATE_DEVICE_SWITCH_TO_REF = 0x4,
#   D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS = 0x8,
#   D3D11_CREATE_DEVICE_BGRA_SUPPORT = 0x20,
#   D3D11_CREATE_DEVICE_DEBUGGABLE = 0x40,
#   D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY = 0x80,
#   D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT = 0x100,
#   D3D11_CREATE_DEVICE_VIDEO_SUPPORT = 0x800
# } ;


class D3D11_CREATE_DEVICE_FLAG(enum.IntFlag):
    SINGLETHREADED = 0x1
    DEBUG = 0x2
    SWITCH_TO_REF = 0x4
    PREVENT_INTERNAL_THREADING_OPTIMIZATIONS = 0x8
    BGRA_SUPPORT = 0x20
    DEBUGGABLE = 0x40
    PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY = 0x80
    DISABLE_GPU_TIMEOUT = 0x100
    VIDEO_SUPPORT = 0x800


# https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_feature_level
#
# typedef enum D3D_FEATURE_LEVEL {
#   D3D_FEATURE_LEVEL_1_0_GENERIC,
#   D3D_FEATURE_LEVEL_1_0_CORE,
#   D3D_FEATURE_LEVEL_9_1,
#   D3D_FEATURE_LEVEL_9_2,
#   D3D_FEATURE_LEVEL_9_3,
#   D3D_FEATURE_LEVEL_10_0,
#   D3D_FEATURE_LEVEL_10_1,
#   D3D_FEATURE_LEVEL_11_0,
#   D3D_FEATURE_LEVEL_11_1,
#   D3D_FEATURE_LEVEL_12_0,
#   D3D_FEATURE_LEVEL_12_1,
#   D3D_FEATURE_LEVEL_12_2
# } ;


class D3D_FEATURE_LEVEL(enum.IntEnum):
    LEVEL_1_0_GENERIC = 0x1000
    LEVEL_1_0_CORE = 0x1001
    LEVEL_9_1 = 0x9100
    LEVEL_9_2 = 0x9200
    LEVEL_9_3 = 0x9300
    LEVEL_10_0 = 0xA000
    LEVEL_10_1 = 0xA100
    LEVEL_11_0 = 0xB000
    LEVEL_11_1 = 0xB100
    LEVEL_12_0 = 0xC000
    LEVEL_12_1 = 0xC100
    LEVEL_12_2 = 0xC200


# not sure where this is officially defined or if the value would ever change

D3D11_SDK_VERSION = 7

# https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
#
# HRESULT D3D11CreateDevice(
#   [in, optional]  IDXGIAdapter            *pAdapter,
#                   D3D_DRIVER_TYPE         DriverType,
#                   HMODULE                 Software,
#                   UINT                    Flags,
#   [in, optional]  const D3D_FEATURE_LEVEL *pFeatureLevels,
#                   UINT                    FeatureLevels,
#                   UINT                    SDKVersion,
#   [out, optional] ID3D11Device            **ppDevice,
#   [out, optional] D3D_FEATURE_LEVEL       *pFeatureLevel,
#   [out, optional] ID3D11DeviceContext     **ppImmediateContext
# );


D3D11CreateDevice = ctypes.WINFUNCTYPE(
    wintypes.INT,
    wintypes.LPVOID,
    wintypes.UINT,
    wintypes.LPVOID,
    wintypes.UINT,
    ctypes.POINTER(wintypes.UINT),
    wintypes.UINT,
    wintypes.UINT,
    ctypes.POINTER(IUnknown),
    ctypes.POINTER(wintypes.UINT),
    ctypes.POINTER(IUnknown),
)(
    ("D3D11CreateDevice", ctypes.windll.d3d11),
    (
        (IN, "pAdapter", None),
        (IN, "DriverType", D3D_DRIVER_TYPE.UNKNOWN),
        (IN, "Software", None),
        (IN, "Flags", 0),
        (IN, "pFeatureLevels", None),
        (IN, "FeatureLevels", 0),
        (IN, "SDKVersion", D3D11_SDK_VERSION),
        (OUT, "ppDevice"),
        (OUT, "pFeatureLevel"),
        (OUT, "ppImmediateContext"),
    ),
)


def errcheck(
    result: int,
    func,  # type: ignore
    args: tuple[
        wintypes.LPVOID,
        wintypes.UINT,
        wintypes.LPVOID,
        wintypes.UINT,
        wintypes.UINT,
        wintypes.UINT,
        wintypes.UINT,
        IUnknown,
        wintypes.UINT,
        IUnknown,
    ],
):
    if result:
        raise ctypes.WinError(result)

    return (args[7], D3D_FEATURE_LEVEL(args[8].value), args[9])


D3D11CreateDevice.errcheck = errcheck  # type: ignore

# https://learn.microsoft.com/en-us/windows/win32/api/dxgicommon/ns-dxgicommon-dxgi_sample_desc


class DXGI_SAMPLE_DESC(ctypes.Structure):
    _fields_ = [
        ("Count", wintypes.UINT),
        ("Quality", wintypes.UINT),
    ]


# https://learn.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_surface_desc


class DXGI_SURFACE_DESC(ctypes.Structure):
    _fields_ = [
        ("Width", wintypes.UINT),
        ("Height", wintypes.UINT),
        ("Format", wintypes.UINT),
        ("SampleDesc", DXGI_SAMPLE_DESC),
    ]


# https://learn.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_mapped_rect


class DXGI_MAPPED_RECT(ctypes.Structure):
    _fields_ = [
        ("Pitch", wintypes.INT),
        ("pBits", ctypes.POINTER(wintypes.BYTE)),
    ]


# https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nn-dxgi-idxgisurface


class DXGI_MAP(enum.IntFlag):
    READ = 1
    WRITE = 2
    DISCARD = 4


class IDXGISurface(IUnknown):
    GetDesc = ctypes.WINFUNCTYPE(wintypes.INT, ctypes.POINTER(DXGI_SURFACE_DESC))(
        8, "GetDesc"
    )
    Map = ctypes.WINFUNCTYPE(
        wintypes.INT, ctypes.POINTER(DXGI_MAPPED_RECT), wintypes.UINT
    )(9, "Map")
    Unmap = ctypes.WINFUNCTYPE(wintypes.INT)(10, "Unmap")

    @contextlib.contextmanager
    def map(self, flags: DXGI_MAP = DXGI_MAP.READ):
        locked_rect = DXGI_MAPPED_RECT()
        ret = IDXGISurface.Map(self, ctypes.byref(locked_rect), flags)
        if ret.value:
            raise ctypes.WinError(ret.value)

        try:
            yield locked_rect.Pitch
        finally:
            ret = IDXGISurface.Unmap(self)
            if ret.value:
                raise ctypes.WinError(ret.value)
