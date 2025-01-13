import atexit
import tempfile
from ctypes import WinError

from typing_extensions import override

from winrt.microsoft.ui.xaml import (
    Application,
    ApplicationInitializationCallbackParams,
    LaunchActivatedEventArgs,
    Window,
)
from winrt.microsoft.ui.xaml.controls import (
    CoreWebView2InitializedEventArgs,
    Grid,
    WebView2,
    XamlControlsResources,
)
from winrt.microsoft.ui.xaml.markup import (
    IXamlMetadataProvider,
    IXamlType,
    XamlReader,
    XmlnsDefinition,
)
from winrt.microsoft.ui.xaml.xamltypeinfo import XamlControlsXamlMetaDataProvider
from winrt.microsoft.web.webview2.core import CoreWebView2Environment
from winrt.microsoft.windows.applicationmodel.dynamicdependency.bootstrap import (
    InitializeOptions,
    initialize,
)
from winrt.runtime import ApartmentType, init_apartment
from winrt.system import Array
from winrt.windows.foundation import AsyncStatus, IAsyncAction, IAsyncOperation
from winrt.windows.ui.xaml.interop import TypeName


def check_initialized(sender: WebView2, args: CoreWebView2InitializedEventArgs):
    # This can be useful debugging issues when the WebView2 control fails to
    # initialize (in which case, it will appear as empty black box).
    if args.exception.value:
        print("Initialization failed", WinError(args.exception.value))


_XAML = """
<Window xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
    <Grid>
        <Grid.RowDefinitions>
            <RowDefinition Height="Auto"/>
            <RowDefinition Height="*"/>
        </Grid.RowDefinitions>
        <Grid.ColumnDefinitions>
            <ColumnDefinition Width="*"/>
            <ColumnDefinition Width="Auto"/>
        </Grid.ColumnDefinitions>

        <WebView2 Name="webview" Grid.Row="1" Grid.ColumnSpan="2"
            HorizontalAlignment="Stretch" VerticalAlignment="Stretch"
            Source="https://pywinrt.readthedocs.io/en/latest/"/>
    </Grid>
</Window>
"""


class App(Application, IXamlMetadataProvider):
    def __init__(self) -> None:
        self._provider = XamlControlsXamlMetaDataProvider()

    @override
    def _on_launched(self, args: LaunchActivatedEventArgs) -> None:
        # Have to add some default resources here, otherwise the
        # app will crash when trying to create menus
        resources = XamlControlsResources()
        self.resources.merged_dictionaries.append(resources)

        window = XamlReader.load(_XAML).as_(Window)
        window.add_closed(lambda s, e: self.exit())

        webview = window.content.as_(Grid).find_name("webview").as_(WebView2)
        webview.add_core_webview2_initialized(check_initialized)

        # By default, WebView2 will create a directory in the .exe's directory
        # which isn't usually desireable. Here we create a temporary directory
        # since this is just example code. A real app would use some application-
        # specific user data directory and not delete it at exit.
        cache_dir = tempfile.TemporaryDirectory(ignore_cleanup_errors=True)
        atexit.register(cache_dir.cleanup)

        env_op = CoreWebView2Environment.create_with_options_async(
            "", cache_dir.name, None
        )

        def on_env(op: IAsyncOperation[CoreWebView2Environment], status: AsyncStatus):
            if status == AsyncStatus.ERROR:
                print(
                    "create_with_options_async failed:", WinError(op.error_code.value)
                )
                return

            if status != AsyncStatus.COMPLETED:
                return

            env = op.get_results()

            ensure_op = webview.ensure_core_webview2_with_environment_async(env)

            def on_ensure(op: IAsyncAction, status: AsyncStatus):
                if status == AsyncStatus.ERROR:
                    print(
                        "ensure_core_webview2_async failed:",
                        WinError(op.error_code.value),
                    )
                    return

                if status != AsyncStatus.COMPLETED:
                    return

                # Don't show the window until ready to avoid black box
                window.activate()

            ensure_op.completed = on_ensure

        env_op.completed = on_env

    @override
    def get_xaml_type(self, type: TypeName) -> IXamlType:
        return self._provider.get_xaml_type(type)

    @override
    def get_xaml_type_by_full_name(self, full_name: str) -> IXamlType:
        return self._provider.get_xaml_type_by_full_name(full_name)

    @override
    def get_xmlns_definitions(self) -> Array[XmlnsDefinition]:
        return self._provider.get_xmlns_definitions()


def init(_: ApplicationInitializationCallbackParams) -> None:
    # This implicitly sets Application.current to this object so we don't need
    # to keep a reference to it.
    App()


if __name__ == "__main__":
    init_apartment(ApartmentType.SINGLE_THREADED)

    # This is the main entry point for the application. To use the Windows App SDK
    # outside of a packaged app, you have to bootstrap it using the initialize
    # function. The ON_NO_MATCH_SHOW_UI option will show a dialog if the required
    # version of the Windows App runtime is not installed. If you publish your app
    # with an installer, the installer can install the runtime, then users will
    # never see this dialog.
    with initialize(options=InitializeOptions.ON_NO_MATCH_SHOW_UI):
        Application.start(init)
