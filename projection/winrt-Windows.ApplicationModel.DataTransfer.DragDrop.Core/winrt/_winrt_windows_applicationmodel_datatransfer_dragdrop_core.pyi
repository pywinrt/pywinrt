# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.applicationmodel.datatransfer
import winrt.windows.applicationmodel.datatransfer.dragdrop
import winrt.windows.foundation
import winrt.windows.graphics.imaging

from winrt.windows.applicationmodel.datatransfer.dragdrop.core import CoreDragUIContentMode

Self = typing.TypeVar('Self')

class CoreDragDropManager_Static(type):
    def get_for_current_view(cls) -> typing.Optional[CoreDragDropManager]: ...

class CoreDragDropManager(winrt.system.Object, metaclass=CoreDragDropManager_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> CoreDragDropManager: ...
    def add_target_requested(self, value: winrt.windows.foundation.TypedEventHandler[CoreDragDropManager, CoreDropOperationTargetRequestedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_target_requested(self, value: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def are_concurrent_operations_enabled(self) -> bool: ...
    @are_concurrent_operations_enabled.setter
    def are_concurrent_operations_enabled(self, value: bool) -> None: ...

class CoreDragInfo(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> CoreDragInfo: ...
    @_property
    def data(self) -> typing.Optional[winrt.windows.applicationmodel.datatransfer.DataPackageView]: ...
    @_property
    def modifiers(self) -> winrt.windows.applicationmodel.datatransfer.dragdrop.DragDropModifiers: ...
    @_property
    def position(self) -> winrt.windows.foundation.Point: ...
    @_property
    def allowed_operations(self) -> winrt.windows.applicationmodel.datatransfer.DataPackageOperation: ...

class CoreDragOperation(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> CoreDragOperation: ...
    def __new__(cls: typing.Type[CoreDragOperation]) -> CoreDragOperation:...
    @typing.overload
    def set_drag_u_i_content_from_software_bitmap(self, software_bitmap: typing.Optional[winrt.windows.graphics.imaging.SoftwareBitmap], /) -> None: ...
    @typing.overload
    def set_drag_u_i_content_from_software_bitmap(self, software_bitmap: typing.Optional[winrt.windows.graphics.imaging.SoftwareBitmap], anchor_point: winrt.windows.foundation.Point, /) -> None: ...
    def set_pointer_id(self, pointer_id: winrt.system.UInt32, /) -> None: ...
    def start_async(self) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.applicationmodel.datatransfer.DataPackageOperation]: ...
    @_property
    def drag_u_i_content_mode(self) -> CoreDragUIContentMode: ...
    @drag_u_i_content_mode.setter
    def drag_u_i_content_mode(self, value: CoreDragUIContentMode) -> None: ...
    @_property
    def data(self) -> typing.Optional[winrt.windows.applicationmodel.datatransfer.DataPackage]: ...
    @_property
    def allowed_operations(self) -> winrt.windows.applicationmodel.datatransfer.DataPackageOperation: ...
    @allowed_operations.setter
    def allowed_operations(self, value: winrt.windows.applicationmodel.datatransfer.DataPackageOperation) -> None: ...

class CoreDragUIOverride(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> CoreDragUIOverride: ...
    def clear(self) -> None: ...
    @typing.overload
    def set_content_from_software_bitmap(self, software_bitmap: typing.Optional[winrt.windows.graphics.imaging.SoftwareBitmap], /) -> None: ...
    @typing.overload
    def set_content_from_software_bitmap(self, software_bitmap: typing.Optional[winrt.windows.graphics.imaging.SoftwareBitmap], anchor_point: winrt.windows.foundation.Point, /) -> None: ...
    @_property
    def is_glyph_visible(self) -> bool: ...
    @is_glyph_visible.setter
    def is_glyph_visible(self, value: bool) -> None: ...
    @_property
    def is_content_visible(self) -> bool: ...
    @is_content_visible.setter
    def is_content_visible(self, value: bool) -> None: ...
    @_property
    def is_caption_visible(self) -> bool: ...
    @is_caption_visible.setter
    def is_caption_visible(self, value: bool) -> None: ...
    @_property
    def caption(self) -> str: ...
    @caption.setter
    def caption(self, value: str) -> None: ...

class CoreDropOperationTargetRequestedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> CoreDropOperationTargetRequestedEventArgs: ...
    def set_target(self, target: typing.Optional[ICoreDropOperationTarget], /) -> None: ...

class ICoreDropOperationTarget(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ICoreDropOperationTarget: ...
    def drop_async(self, drag_info: typing.Optional[CoreDragInfo], /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.applicationmodel.datatransfer.DataPackageOperation]: ...
    def enter_async(self, drag_info: typing.Optional[CoreDragInfo], drag_u_i_override: typing.Optional[CoreDragUIOverride], /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.applicationmodel.datatransfer.DataPackageOperation]: ...
    def leave_async(self, drag_info: typing.Optional[CoreDragInfo], /) -> winrt.windows.foundation.IAsyncAction: ...
    def over_async(self, drag_info: typing.Optional[CoreDragInfo], drag_u_i_override: typing.Optional[CoreDragUIOverride], /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.applicationmodel.datatransfer.DataPackageOperation]: ...
