# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.ui.xaml.interop as windows_ui_xaml_interop
import winui3.microsoft.ui.xaml.markup as microsoft_ui_xaml_markup

Self = typing.TypeVar('Self')

@typing.final
class XamlControlsXamlMetaDataProvider_Static(winrt._winrt.IInspectable_Static):
    # System.Void Microsoft.UI.Xaml.XamlTypeInfo.XamlControlsXamlMetaDataProvider::Initialize()
    def initialize(cls) -> None: ...

@typing.final
class XamlControlsXamlMetaDataProvider(winrt.system.Object, microsoft_ui_xaml_markup.IXamlMetadataProvider, metaclass=XamlControlsXamlMetaDataProvider_Static):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Microsoft.UI.Xaml.Markup.IXamlType Microsoft.UI.Xaml.XamlTypeInfo.XamlControlsXamlMetaDataProvider::GetXamlType(Windows.UI.Xaml.Interop.TypeName)
    def get_xaml_type(self, type: typing.Union[windows_ui_xaml_interop.TypeName, typing.Tuple[str, windows_ui_xaml_interop.TypeKind]], /) -> microsoft_ui_xaml_markup.IXamlType: ...
    # Microsoft.UI.Xaml.Markup.IXamlType Microsoft.UI.Xaml.XamlTypeInfo.XamlControlsXamlMetaDataProvider::GetXamlType(System.String)
    def get_xaml_type_by_full_name(self, full_name: str, /) -> microsoft_ui_xaml_markup.IXamlType: ...
    # Microsoft.UI.Xaml.Markup.XmlnsDefinition[] Microsoft.UI.Xaml.XamlTypeInfo.XamlControlsXamlMetaDataProvider::GetXmlnsDefinitions()
    def get_xmlns_definitions(self) -> winrt.system.Array[microsoft_ui_xaml_markup.XmlnsDefinition]: ...

