from winrt.microsoft.ui.xaml import (
    Application,
    ApplicationInitializationCallbackParams,
    HorizontalAlignment,
    LaunchActivatedEventArgs,
    VerticalAlignment,
    Window,
)
from winrt.microsoft.ui.xaml.controls import Button, StackPanel
from winrt.microsoft.windows.applicationmodel.dynamicdependency.bootstrap import (
    InitializeOptions,
    initialize,
)
from winrt.system import box_string


class App(Application):
    def _on_launched(self, args: LaunchActivatedEventArgs) -> None:
        # Build the UI
        button = Button()
        button.add_click(lambda s, e: print("Hello, world!"))
        button.content = box_string("Click me")
        button.horizontal_alignment = HorizontalAlignment.CENTER

        button2 = Button()
        button2.add_click(lambda s, e: self.exit())
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


def init(_: ApplicationInitializationCallbackParams) -> None:
    # This implicitly sets Application.current to this object so we don't need
    # to keep a reference to it.
    App()


# This is the main entry point for the application. To use the Windows App SDK
# outside of a packaged app, you have to bootstrap it using the initialize
# function. The ON_NO_MATCH_SHOW_UI option will show a dialog if the required
# version of the Windows App runtime is not installed.
with initialize(options=InitializeOptions.ON_NO_MATCH_SHOW_UI):
    Application.start(init)
