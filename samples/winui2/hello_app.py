from typing import Tuple, Union

from typing_extensions import override

from winrt.runtime import init_apartment, ApartmentType
from winrt.system import Array
from winrt.windows.applicationmodel.activation import LaunchActivatedEventArgs
from winrt.windows.ui.xaml import (
    Application,
    ApplicationInitializationCallbackParams,
    FrameworkElement,
    Window,
)
from winrt.windows.ui.xaml.controls import Button
from winrt.windows.ui.xaml.interop import TypeKind, TypeName
from winrt.windows.ui.xaml.markup import (
    IXamlMetadataProvider,
    IXamlType,
    XamlReader,
    XmlnsDefinition,
)
from winui2.microsoft.ui.xaml.controls import XamlControlsResources
from winui2.microsoft.ui.xaml.xamltypeinfo import XamlControlsXamlMetaDataProvider

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


init_apartment(ApartmentType.SINGLE_THREADED)

# FIXME: this crashes before calling init() - likely due to these APIs requiring
# being called in a UWP context. We need to figure out how to create a UWP context
# or convert this to use some other UI framework as a base with XAML Islands.
Application.start(init)
