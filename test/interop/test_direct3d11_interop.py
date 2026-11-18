import ctypes
import unittest

from winrt.windows.graphics import SizeInt32
from winrt.windows.graphics.capture import Direct3D11CaptureFramePool
from winrt.windows.graphics.directx import DirectXPixelFormat
from winrt.windows.graphics.directx.direct3d11 import IDirect3DDevice
from winrt.windows.graphics.directx.direct3d11.interop import (
    D3d11CreateDeviceFlag,
    D3dDriverType,
    create_direct3d11_device,
    get_dxgi_device_from_object,
)

E_INVALIDARG = -2147024809

# IUnknown::Release, the third method of every COM interface
_release = ctypes.WINFUNCTYPE(ctypes.c_ulong)(2, "Release")


class TestDirect3D11Interop(unittest.TestCase):
    def test_create_device(self) -> None:
        device = create_direct3d11_device()
        self.assertIsInstance(device, IDirect3DDevice)

    def test_create_warp_device(self) -> None:
        device = create_direct3d11_device(
            driver_type=D3dDriverType.WARP,
            flags=D3d11CreateDeviceFlag.BGRA_SUPPORT
            | D3d11CreateDeviceFlag.SINGLE_THREADED,
        )
        self.assertIsInstance(device, IDirect3DDevice)

    def test_dxgi_device(self) -> None:
        device = create_direct3d11_device()

        dxgi_device = get_dxgi_device_from_object(device)
        self.assertNotEqual(dxgi_device, 0)
        _release(ctypes.c_void_p(dxgi_device))

    def test_capture_frame_pool(self) -> None:
        device = create_direct3d11_device()

        with Direct3D11CaptureFramePool.create_free_threaded(
            device, DirectXPixelFormat.B8_G8_R8_A8_UINT_NORMALIZED, 1, SizeInt32(64, 64)
        ) as frame_pool:
            self.assertIsNotNone(frame_pool)

    def test_bad_driver_type(self) -> None:
        # UNKNOWN needs an adapter, which create_direct3d11_device() never passes
        with self.assertRaises(OSError) as cm:
            create_direct3d11_device(driver_type=0)  # type: ignore[arg-type]

        self.assertEqual(cm.exception.winerror, E_INVALIDARG)

    def test_driver_type_out_of_range(self) -> None:
        for driver_type in (-1, 6, 2**31):
            with self.subTest(driver_type=driver_type):
                with self.assertRaises((ValueError, OverflowError)):
                    create_direct3d11_device(driver_type=driver_type)  # type: ignore[arg-type]

        with self.assertRaisesRegex(ValueError, "99 is not a valid D3dDriverType"):
            create_direct3d11_device(driver_type=99)  # type: ignore[arg-type]

    def test_flags_out_of_range(self) -> None:
        with self.assertRaises(ValueError):
            create_direct3d11_device(flags=-1)  # type: ignore[arg-type]

        with self.assertRaises(OverflowError):
            create_direct3d11_device(flags=1 << 32)  # type: ignore[arg-type]
