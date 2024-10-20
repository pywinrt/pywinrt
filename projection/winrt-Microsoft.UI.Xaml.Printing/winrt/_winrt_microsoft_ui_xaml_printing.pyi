# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.microsoft.ui.dispatching as microsoft_ui_dispatching
import winrt.microsoft.ui.xaml as microsoft_ui_xaml
import winrt.windows.foundation as windows_foundation
import winrt.windows.graphics.printing as windows_graphics_printing
import winrt.windows.ui.core as windows_ui_core

from winrt.microsoft.ui.xaml.printing import PreviewPageCountType
from winrt.microsoft.ui.xaml.printing import AddPagesEventHandler, GetPreviewPageEventHandler, PaginateEventHandler

Self = typing.TypeVar('Self')

@typing.final
class AddPagesEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> AddPagesEventArgs: ...
    def __new__(cls: typing.Type[AddPagesEventArgs]) -> AddPagesEventArgs: ...
    @_property
    def print_task_options(self) -> typing.Optional[windows_graphics_printing.PrintTaskOptions]: ...

@typing.final
class GetPreviewPageEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> GetPreviewPageEventArgs: ...
    def __new__(cls: typing.Type[GetPreviewPageEventArgs]) -> GetPreviewPageEventArgs: ...
    @_property
    def page_number(self) -> winrt.system.Int32: ...

@typing.final
class PaginateEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PaginateEventArgs: ...
    def __new__(cls: typing.Type[PaginateEventArgs]) -> PaginateEventArgs: ...
    @_property
    def current_preview_page_number(self) -> winrt.system.Int32: ...
    @_property
    def print_task_options(self) -> typing.Optional[windows_graphics_printing.PrintTaskOptions]: ...

@typing.final
class PrintDocument_Static(type):
    @_property
    def document_source_property(cls) -> typing.Optional[microsoft_ui_xaml.DependencyProperty]: ...

@typing.final
class PrintDocument(winrt.system.Object, metaclass=PrintDocument_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PrintDocument: ...
    def __new__(cls: typing.Type[PrintDocument]) -> PrintDocument: ...
    def add_page(self, page_visual: typing.Optional[microsoft_ui_xaml.UIElement], /) -> None: ...
    def add_pages_complete(self) -> None: ...
    def clear_value(self, dp: typing.Optional[microsoft_ui_xaml.DependencyProperty], /) -> None: ...
    def get_animation_base_value(self, dp: typing.Optional[microsoft_ui_xaml.DependencyProperty], /) -> typing.Optional[winrt.system.Object]: ...
    def get_value(self, dp: typing.Optional[microsoft_ui_xaml.DependencyProperty], /) -> typing.Optional[winrt.system.Object]: ...
    def invalidate_preview(self) -> None: ...
    def read_local_value(self, dp: typing.Optional[microsoft_ui_xaml.DependencyProperty], /) -> typing.Optional[winrt.system.Object]: ...
    def register_property_changed_callback(self, dp: typing.Optional[microsoft_ui_xaml.DependencyProperty], callback: typing.Optional[microsoft_ui_xaml.DependencyPropertyChangedCallback], /) -> winrt.system.Int64: ...
    def set_preview_page(self, page_number: winrt.system.Int32, page_visual: typing.Optional[microsoft_ui_xaml.UIElement], /) -> None: ...
    def set_preview_page_count(self, count: winrt.system.Int32, type: PreviewPageCountType, /) -> None: ...
    def set_value(self, dp: typing.Optional[microsoft_ui_xaml.DependencyProperty], value: typing.Optional[winrt.system.Object], /) -> None: ...
    def unregister_property_changed_callback(self, dp: typing.Optional[microsoft_ui_xaml.DependencyProperty], token: winrt.system.Int64, /) -> None: ...
    def add_add_pages(self, handler: typing.Optional[AddPagesEventHandler], /) -> windows_foundation.EventRegistrationToken: ...
    def remove_add_pages(self, token: windows_foundation.EventRegistrationToken, /) -> None: ...
    def add_get_preview_page(self, handler: typing.Optional[GetPreviewPageEventHandler], /) -> windows_foundation.EventRegistrationToken: ...
    def remove_get_preview_page(self, token: windows_foundation.EventRegistrationToken, /) -> None: ...
    def add_paginate(self, handler: typing.Optional[PaginateEventHandler], /) -> windows_foundation.EventRegistrationToken: ...
    def remove_paginate(self, token: windows_foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def document_source(self) -> typing.Optional[windows_graphics_printing.IPrintDocumentSource]: ...
    @_property
    def dispatcher(self) -> typing.Optional[windows_ui_core.CoreDispatcher]: ...
    @_property
    def dispatcher_queue(self) -> typing.Optional[microsoft_ui_dispatching.DispatcherQueue]: ...

