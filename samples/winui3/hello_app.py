# /// script
# dependencies = [
#   "typing-extensions>=4.5",
#   "winrt-runtime>=3.1",
#   "winrt-Windows.Foundation>=3.1",
#   "winrt-Windows.Foundation.Collections>=3.1",
#   "winrt-Windows.UI.Xaml.Interop>=3.1",
#   "winui3-Microsoft.UI.Xaml>=3.1",
#   "winui3-Microsoft.UI.Xaml.Controls>=3.1",
#   "winui3-Microsoft.UI.Xaml.Markup>=3.1",
#   "winui3-Microsoft.UI.Xaml.XamlTypeInfo>=3.1",
#   "winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap>=3.1",
# ]
# ///

from typing import Tuple, Union

from typing_extensions import override
from winrt.system import Array
from winrt.windows.ui.xaml.interop import TypeKind, TypeName
from winui3.microsoft.ui.xaml import (
    Application,
    ApplicationInitializationCallbackParams,
    FrameworkElement,
    LaunchActivatedEventArgs,
    Window,
)
from winui3.microsoft.ui.xaml.controls import Button, XamlControlsResources
from winui3.microsoft.ui.xaml.markup import (
    IXamlMetadataProvider,
    IXamlType,
    XamlReader,
    XmlnsDefinition,
)
from winui3.microsoft.ui.xaml.xamltypeinfo import XamlControlsXamlMetaDataProvider
from winui3.microsoft.windows.applicationmodel.dynamicdependency.bootstrap import (
    InitializeOptions,
    initialize,
)

# XAML can be inline like this or saved in a separate file. Or you can do
# everything programmatically if you rather.
XAML = """
<Window xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
    <StackPanel VerticalAlignment="Center" HorizontalAlignment="Center" Spacing="10">
        <Button Name="click" Content="Click me" HorizontalAlignment="Center"/>
        <Button Name="exit" Content="Exit" HorizontalAlignment="Center"/>
    </StackPanel>
</Window>
"""


# The Windows App Runtime depends on IXamlMetadataProvider being
# implemented on the Application class, so always include this.
class App(Application, IXamlMetadataProvider):
    def __init__(self) -> None:
        self._provider = XamlControlsXamlMetaDataProvider()

    @override
    def _on_launched(self, args: LaunchActivatedEventArgs) -> None:
        # Have to add some default resources here, otherwise the
        # app will crash when trying to create menus
        resources = XamlControlsResources()
        self.resources.merged_dictionaries.append(resources)

        # Load the XAML
        window = XamlReader.load(XAML).as_(Window)

        # Wire up events
        content = window.content.as_(FrameworkElement)

        button = content.find_name("click").as_(Button)
        button.add_click(lambda s, e: print("Hello, world!"))

        exit_button = content.find_name("exit").as_(Button)
        exit_button.add_click(lambda s, e: self.exit())

        # Show it!
        window.activate()

    @override
    def get_xaml_type(self, type: Union[TypeName, Tuple[str, TypeKind]]) -> IXamlType:
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


# This is the main entry point for the application. To use the Windows App SDK
# outside of a packaged app, you have to bootstrap it using the initialize
# function. The ON_NO_MATCH_SHOW_UI option will show a dialog if the required
# version of the Windows App runtime is not installed. If you publish your app
# with an installer, the installer can install the runtime, then users will
# never see this dialog.
with initialize(options=InitializeOptions.ON_NO_MATCH_SHOW_UI):
    Application.start(init)
