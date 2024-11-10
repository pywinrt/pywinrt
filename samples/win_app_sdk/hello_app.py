from winrt.microsoft.windows.applicationmodel.dynamicdependency.bootstrap import (
    initialize,
    InitializeOptions,
)
from winrt.microsoft.ui.xaml import (
    Application,
    HorizontalAlignment,
    VerticalAlignment,
    Window,
)
from winrt.microsoft.ui.xaml.controls import Button, StackPanel
from winrt.system import box_string


def init(_):
    # Always have to create an application object even if you don't use it.
    # (There seems to be a bug where Window() will segfault if
    # Application.current is None.)
    app = Application()

    # Build the UI
    button = Button()
    button.add_click(lambda s, e: print("Hello, world!"))
    button.content = box_string("Click me")
    button.horizontal_alignment = HorizontalAlignment.CENTER

    button2 = Button()
    button2.add_click(lambda s, e: app.exit())
    button2.content = box_string("Exit")
    button2.horizontal_alignment = HorizontalAlignment.CENTER

    panel = StackPanel()
    panel.children.append(button)
    panel.children.append(button2)
    panel.vertical_alignment = VerticalAlignment.CENTER
    panel.horizontal_alignment = HorizontalAlignment.CENTER
    panel.spacing = 10

    window = Window()
    window.content = panel

    # Show it!
    window.activate()


# This is the main entry point for the application. To use the Windows App SDK
# outside of a packaged app, you have to bootstrap it using the initialize
# function. The ON_NO_MATCH_SHOW_UI option will show a dialog if the required
# version of the Windows App runtime is not installed.
with initialize(options=InitializeOptions.ON_NO_MATCH_SHOW_UI):
    Application.start(init)
