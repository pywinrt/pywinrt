# SPDX-License-Identifier: MIT
# Copyright 2024 David Lechner <david@pybricks.com>

"""
This is an example of how to use PyWinRT to capture a screen image from a
command line program.

Run this script as a module ``py -m samples.screen_capture`` and select a
window to capture. If the window is minimized, activate it, otherwise the
capture will time out. The captured image will be saved as
``pywinrt example screen capture.png`` in the current directory.
"""

import contextlib
from pathlib import Path
from threading import Event

from winrt.windows.graphics.capture import (
    GraphicsCaptureItem,
    Direct3D11CaptureFramePool,
)
from winrt.windows.graphics.directx import DirectXPixelFormat
from winrt.windows.graphics.directx.direct3d11.interop import (
    create_direct3d11_device_from_dxgi_device,
)
from winrt.windows.graphics.imaging import BitmapEncoder, SoftwareBitmap
from winrt.windows.storage import CreationCollisionOption, FileAccessMode, StorageFolder
from winrt.windows.ui.windowmanagement import WindowServices

from .d3d11 import D3D_DRIVER_TYPE, D3D11_CREATE_DEVICE_FLAG, D3D11CreateDevice
from .sync import wait_for

capture_items: list[GraphicsCaptureItem] = []

for win_id in WindowServices.find_all_top_level_window_ids():
    item = GraphicsCaptureItem.try_create_from_window_id(win_id)

    if item is None:
        continue

    capture_items.append(item)
    print(len(capture_items), item.display_name, sep=". ")

print()
selection = input("Select a window to capture: ")
selected_item = capture_items[int(selection) - 1]

with contextlib.ExitStack() as stack:
    dxgi, _, context = D3D11CreateDevice(
        DriverType=D3D_DRIVER_TYPE.HARDWARE,
        Flags=D3D11_CREATE_DEVICE_FLAG.BGRA_SUPPORT,
    )

    device = stack.enter_context(create_direct3d11_device_from_dxgi_device(dxgi.value))

    frame_pool = stack.enter_context(
        Direct3D11CaptureFramePool.create_free_threaded(
            device,
            DirectXPixelFormat.R8_G8_B8_A8_UINT_NORMALIZED,
            1,
            selected_item.size,
        )
    )

    session = stack.enter_context(frame_pool.create_capture_session(selected_item))

    event = Event()
    token = frame_pool.add_frame_arrived(lambda _, __: event.set())
    stack.callback(frame_pool.remove_frame_arrived, token)

    session.start_capture()

    # FIXME: this times out if the window is minimized
    if not event.wait(10):
        print("Timed out waiting for frame - is window minimized?")
        exit(1)

    frame = frame_pool.try_get_next_frame()
    assert frame is not None, "This shouldn't happen since we waited for the event."

    stack.enter_context(frame)
    bitmap = stack.enter_context(
        wait_for(SoftwareBitmap.create_copy_from_surface_async(frame.surface))
    )
    folder = wait_for(
        StorageFolder.get_folder_from_path_async(str(Path(".").resolve()))
    )
    file = wait_for(
        folder.create_file_async(
            "pywinrt example screen capture.png",
            CreationCollisionOption.REPLACE_EXISTING,
        )
    )
    stream = stack.enter_context(wait_for(file.open_async(FileAccessMode.READ_WRITE)))
    encoder = wait_for(BitmapEncoder.create_async(BitmapEncoder.png_encoder_id, stream))
    encoder.set_software_bitmap(bitmap)
    wait_for(encoder.flush_async())
