from datetime import timedelta
from random import randint
from tkinter import Canvas, Tk, Button

from winrt.windows.foundation.numerics import Vector2, Vector3
from winrt.windows.system.interop import create_dispatcher_queue_controller
from winrt.windows.ui import Color
from winrt.windows.ui.composition import Compositor, ContainerVisual
from winrt.windows.ui.composition.interop import create_desktop_window_target

"""
https://learn.microsoft.com/en-us/windows/apps/desktop/modernize/using-the-visual-layer-with-win32

Example from link above ported to PyWinRT an integrated with Tk
"""

root = Tk()
root.geometry("900x660+50+50")

canvas = Canvas()
canvas.create_rectangle(10, 10, 890, 610)
canvas.pack(side="top", fill="both", expand=True)

# The WinRT Composition APIs require a dispatch queue to be
# integrated with the Windows message loop that was set up by Tk().
# We must keep a handle to the controller object to keep it from
# being destroyed. Even though we don't use the controller variable
# the compositor object needs it behind the scenes.
controller = create_dispatcher_queue_controller()

compositor = Compositor()
target = create_desktop_window_target(compositor, canvas.winfo_id())
c_root = compositor.create_container_visual()
c_root.relative_size_adjustment = Vector2(1, 1)
c_root.offset = Vector3(124, 12, 0)
target.root = c_root


def add_element(size: float, x: float, y: float) -> None:
    visuals = ContainerVisual._from(target.root).children
    element = compositor.create_sprite_visual()

    r = randint(0, 255)
    g = randint(0, 255)
    b = randint(0, 255)

    element.brush = compositor.create_color_brush(Color(255, r, g, b))
    element.size = Vector2(size, size)
    element.offset = Vector3(x, y, 0)

    animation = compositor.create_vector3_key_frame_animation()
    bottom = 600 - element.size.y
    animation.insert_key_frame(1, Vector3(element.offset.x, bottom, 0))

    animation.duration = timedelta(seconds=2)
    animation.delay_time = timedelta(seconds=3)
    element.start_animation("Offset", animation)
    visuals.insert_at_top(element)


def on_button_click() -> None:
    size = randint(50, 200)
    x = randint(0, 400)
    y = randint(0, 300)
    add_element(size, x, y)


button = Button(root, text="Click Me", command=on_button_click)
button.pack(side="bottom", pady=10)

root.mainloop()
