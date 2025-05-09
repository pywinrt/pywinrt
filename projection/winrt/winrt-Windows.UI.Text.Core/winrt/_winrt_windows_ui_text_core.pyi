# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections
import winrt.windows.globalization as windows_globalization
import winrt.windows.ui.text as windows_ui_text
import winrt.windows.ui.viewmanagement as windows_ui_viewmanagement

from winrt.windows.ui.text.core import CoreTextFormatUpdatingReason, CoreTextFormatUpdatingResult, CoreTextInputPaneDisplayPolicy, CoreTextInputScope, CoreTextSelectionUpdatingResult, CoreTextTextUpdatingResult

Self = typing.TypeVar('Self')

@typing.final
class CoreTextRange:
    @_property
    def start_caret_position(self) -> winrt.system.Int32: ...
    @_property
    def end_caret_position(self) -> winrt.system.Int32: ...
    def __new__(cls, start_caret_position: winrt.system.Int32 = 0, end_caret_position: winrt.system.Int32 = 0) -> CoreTextRange: ...
    def __replace__(self, /, **changes: typing.Any) -> CoreTextRange: ...
    def unpack(self) -> typing.Tuple[winrt.system.Int32, winrt.system.Int32]: ...

@typing.final
class CoreTextCompositionCompletedEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.UI.Text.Core.CoreTextCompositionCompletedEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.UI.Text.Core.CoreTextCompositionSegment> Windows.UI.Text.Core.CoreTextCompositionCompletedEventArgs::get_CompositionSegments()
    @_property
    def composition_segments(self) -> typing.Sequence[CoreTextCompositionSegment]: ...
    # System.Boolean Windows.UI.Text.Core.CoreTextCompositionCompletedEventArgs::get_IsCanceled()
    @_property
    def is_canceled(self) -> bool: ...

@typing.final
class CoreTextCompositionSegment(winrt.system.Object):
    # System.String Windows.UI.Text.Core.CoreTextCompositionSegment::get_PreconversionString()
    @_property
    def preconversion_string(self) -> str: ...
    # Windows.UI.Text.Core.CoreTextRange Windows.UI.Text.Core.CoreTextCompositionSegment::get_Range()
    @_property
    def range(self) -> CoreTextRange: ...

@typing.final
class CoreTextCompositionStartedEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.UI.Text.Core.CoreTextCompositionStartedEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # System.Boolean Windows.UI.Text.Core.CoreTextCompositionStartedEventArgs::get_IsCanceled()
    @_property
    def is_canceled(self) -> bool: ...

@typing.final
class CoreTextEditContext(winrt.system.Object):
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::NotifyFocusEnter()
    def notify_focus_enter(self) -> None: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::NotifyFocusLeave()
    def notify_focus_leave(self) -> None: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::NotifyLayoutChanged()
    def notify_layout_changed(self) -> None: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::NotifySelectionChanged(Windows.UI.Text.Core.CoreTextRange)
    def notify_selection_changed(self, selection: typing.Union[CoreTextRange, typing.Tuple[winrt.system.Int32, winrt.system.Int32]], /) -> None: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::NotifyTextChanged(Windows.UI.Text.Core.CoreTextRange,System.Int32,Windows.UI.Text.Core.CoreTextRange)
    def notify_text_changed(self, modified_range: typing.Union[CoreTextRange, typing.Tuple[winrt.system.Int32, winrt.system.Int32]], new_length: winrt.system.Int32, new_selection: typing.Union[CoreTextRange, typing.Tuple[winrt.system.Int32, winrt.system.Int32]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextEditContext::add_CompositionCompleted(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextEditContext,Windows.UI.Text.Core.CoreTextCompositionCompletedEventArgs>)
    def add_composition_completed(self, handler: windows_foundation.TypedEventHandler[CoreTextEditContext, CoreTextCompositionCompletedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::remove_CompositionCompleted(Windows.Foundation.EventRegistrationToken)
    def remove_composition_completed(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextEditContext::add_CompositionStarted(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextEditContext,Windows.UI.Text.Core.CoreTextCompositionStartedEventArgs>)
    def add_composition_started(self, handler: windows_foundation.TypedEventHandler[CoreTextEditContext, CoreTextCompositionStartedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::remove_CompositionStarted(Windows.Foundation.EventRegistrationToken)
    def remove_composition_started(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextEditContext::add_FocusRemoved(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextEditContext,System.Object>)
    def add_focus_removed(self, handler: windows_foundation.TypedEventHandler[CoreTextEditContext, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::remove_FocusRemoved(Windows.Foundation.EventRegistrationToken)
    def remove_focus_removed(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextEditContext::add_FormatUpdating(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextEditContext,Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs>)
    def add_format_updating(self, handler: windows_foundation.TypedEventHandler[CoreTextEditContext, CoreTextFormatUpdatingEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::remove_FormatUpdating(Windows.Foundation.EventRegistrationToken)
    def remove_format_updating(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextEditContext::add_LayoutRequested(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextEditContext,Windows.UI.Text.Core.CoreTextLayoutRequestedEventArgs>)
    def add_layout_requested(self, handler: windows_foundation.TypedEventHandler[CoreTextEditContext, CoreTextLayoutRequestedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::remove_LayoutRequested(Windows.Foundation.EventRegistrationToken)
    def remove_layout_requested(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextEditContext::add_SelectionRequested(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextEditContext,Windows.UI.Text.Core.CoreTextSelectionRequestedEventArgs>)
    def add_selection_requested(self, handler: windows_foundation.TypedEventHandler[CoreTextEditContext, CoreTextSelectionRequestedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::remove_SelectionRequested(Windows.Foundation.EventRegistrationToken)
    def remove_selection_requested(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextEditContext::add_SelectionUpdating(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextEditContext,Windows.UI.Text.Core.CoreTextSelectionUpdatingEventArgs>)
    def add_selection_updating(self, handler: windows_foundation.TypedEventHandler[CoreTextEditContext, CoreTextSelectionUpdatingEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::remove_SelectionUpdating(Windows.Foundation.EventRegistrationToken)
    def remove_selection_updating(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextEditContext::add_TextRequested(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextEditContext,Windows.UI.Text.Core.CoreTextTextRequestedEventArgs>)
    def add_text_requested(self, handler: windows_foundation.TypedEventHandler[CoreTextEditContext, CoreTextTextRequestedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::remove_TextRequested(Windows.Foundation.EventRegistrationToken)
    def remove_text_requested(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextEditContext::add_TextUpdating(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextEditContext,Windows.UI.Text.Core.CoreTextTextUpdatingEventArgs>)
    def add_text_updating(self, handler: windows_foundation.TypedEventHandler[CoreTextEditContext, CoreTextTextUpdatingEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::remove_TextUpdating(Windows.Foundation.EventRegistrationToken)
    def remove_text_updating(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextEditContext::add_NotifyFocusLeaveCompleted(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextEditContext,System.Object>)
    def add_notify_focus_leave_completed(self, handler: windows_foundation.TypedEventHandler[CoreTextEditContext, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::remove_NotifyFocusLeaveCompleted(Windows.Foundation.EventRegistrationToken)
    def remove_notify_focus_leave_completed(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # System.String Windows.UI.Text.Core.CoreTextEditContext::get_Name()
    @_property
    def name(self) -> str: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::put_Name(System.String)
    @name.setter
    def name(self, value: str) -> None: ...
    # System.Boolean Windows.UI.Text.Core.CoreTextEditContext::get_IsReadOnly()
    @_property
    def is_read_only(self) -> bool: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::put_IsReadOnly(System.Boolean)
    @is_read_only.setter
    def is_read_only(self, value: bool) -> None: ...
    # Windows.UI.Text.Core.CoreTextInputScope Windows.UI.Text.Core.CoreTextEditContext::get_InputScope()
    @_property
    def input_scope(self) -> CoreTextInputScope: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::put_InputScope(Windows.UI.Text.Core.CoreTextInputScope)
    @input_scope.setter
    def input_scope(self, value: CoreTextInputScope) -> None: ...
    # Windows.UI.Text.Core.CoreTextInputPaneDisplayPolicy Windows.UI.Text.Core.CoreTextEditContext::get_InputPaneDisplayPolicy()
    @_property
    def input_pane_display_policy(self) -> CoreTextInputPaneDisplayPolicy: ...
    # System.Void Windows.UI.Text.Core.CoreTextEditContext::put_InputPaneDisplayPolicy(Windows.UI.Text.Core.CoreTextInputPaneDisplayPolicy)
    @input_pane_display_policy.setter
    def input_pane_display_policy(self, value: CoreTextInputPaneDisplayPolicy) -> None: ...

@typing.final
class CoreTextFormatUpdatingEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.UI.Text.Core.CoreTextFormatUpdatingResult Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs::get_Result()
    @_property
    def result(self) -> CoreTextFormatUpdatingResult: ...
    # System.Void Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs::put_Result(Windows.UI.Text.Core.CoreTextFormatUpdatingResult)
    @result.setter
    def result(self, value: CoreTextFormatUpdatingResult) -> None: ...
    # Windows.Foundation.IReference`1<Windows.UI.ViewManagement.UIElementType> Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs::get_BackgroundColor()
    @_property
    def background_color(self) -> typing.Optional[windows_ui_viewmanagement.UIElementType]: ...
    # System.Boolean Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs::get_IsCanceled()
    @_property
    def is_canceled(self) -> bool: ...
    # Windows.UI.Text.Core.CoreTextRange Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs::get_Range()
    @_property
    def range(self) -> CoreTextRange: ...
    # Windows.UI.Text.Core.CoreTextFormatUpdatingReason Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs::get_Reason()
    @_property
    def reason(self) -> CoreTextFormatUpdatingReason: ...
    # Windows.Foundation.IReference`1<Windows.UI.ViewManagement.UIElementType> Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs::get_TextColor()
    @_property
    def text_color(self) -> typing.Optional[windows_ui_viewmanagement.UIElementType]: ...
    # Windows.Foundation.IReference`1<Windows.UI.ViewManagement.UIElementType> Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs::get_UnderlineColor()
    @_property
    def underline_color(self) -> typing.Optional[windows_ui_viewmanagement.UIElementType]: ...
    # Windows.Foundation.IReference`1<Windows.UI.Text.UnderlineType> Windows.UI.Text.Core.CoreTextFormatUpdatingEventArgs::get_UnderlineType()
    @_property
    def underline_type(self) -> typing.Optional[windows_ui_text.UnderlineType]: ...

@typing.final
class CoreTextLayoutBounds(winrt.system.Object):
    # Windows.Foundation.Rect Windows.UI.Text.Core.CoreTextLayoutBounds::get_TextBounds()
    @_property
    def text_bounds(self) -> windows_foundation.Rect: ...
    # System.Void Windows.UI.Text.Core.CoreTextLayoutBounds::put_TextBounds(Windows.Foundation.Rect)
    @text_bounds.setter
    def text_bounds(self, value: typing.Union[windows_foundation.Rect, typing.Tuple[winrt.system.Single, winrt.system.Single, winrt.system.Single, winrt.system.Single]]) -> None: ...
    # Windows.Foundation.Rect Windows.UI.Text.Core.CoreTextLayoutBounds::get_ControlBounds()
    @_property
    def control_bounds(self) -> windows_foundation.Rect: ...
    # System.Void Windows.UI.Text.Core.CoreTextLayoutBounds::put_ControlBounds(Windows.Foundation.Rect)
    @control_bounds.setter
    def control_bounds(self, value: typing.Union[windows_foundation.Rect, typing.Tuple[winrt.system.Single, winrt.system.Single, winrt.system.Single, winrt.system.Single]]) -> None: ...

@typing.final
class CoreTextLayoutRequest(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.UI.Text.Core.CoreTextLayoutRequest::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # System.Boolean Windows.UI.Text.Core.CoreTextLayoutRequest::get_IsCanceled()
    @_property
    def is_canceled(self) -> bool: ...
    # Windows.UI.Text.Core.CoreTextLayoutBounds Windows.UI.Text.Core.CoreTextLayoutRequest::get_LayoutBounds()
    @_property
    def layout_bounds(self) -> CoreTextLayoutBounds: ...
    # Windows.UI.Text.Core.CoreTextRange Windows.UI.Text.Core.CoreTextLayoutRequest::get_Range()
    @_property
    def range(self) -> CoreTextRange: ...
    # Windows.UI.Text.Core.CoreTextLayoutBounds Windows.UI.Text.Core.CoreTextLayoutRequest::get_LayoutBoundsVisualPixels()
    @_property
    def layout_bounds_visual_pixels(self) -> CoreTextLayoutBounds: ...

@typing.final
class CoreTextLayoutRequestedEventArgs(winrt.system.Object):
    # Windows.UI.Text.Core.CoreTextLayoutRequest Windows.UI.Text.Core.CoreTextLayoutRequestedEventArgs::get_Request()
    @_property
    def request(self) -> CoreTextLayoutRequest: ...

@typing.final
class CoreTextSelectionRequest(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.UI.Text.Core.CoreTextSelectionRequest::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.UI.Text.Core.CoreTextRange Windows.UI.Text.Core.CoreTextSelectionRequest::get_Selection()
    @_property
    def selection(self) -> CoreTextRange: ...
    # System.Void Windows.UI.Text.Core.CoreTextSelectionRequest::put_Selection(Windows.UI.Text.Core.CoreTextRange)
    @selection.setter
    def selection(self, value: typing.Union[CoreTextRange, typing.Tuple[winrt.system.Int32, winrt.system.Int32]]) -> None: ...
    # System.Boolean Windows.UI.Text.Core.CoreTextSelectionRequest::get_IsCanceled()
    @_property
    def is_canceled(self) -> bool: ...

@typing.final
class CoreTextSelectionRequestedEventArgs(winrt.system.Object):
    # Windows.UI.Text.Core.CoreTextSelectionRequest Windows.UI.Text.Core.CoreTextSelectionRequestedEventArgs::get_Request()
    @_property
    def request(self) -> CoreTextSelectionRequest: ...

@typing.final
class CoreTextSelectionUpdatingEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.UI.Text.Core.CoreTextSelectionUpdatingEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.UI.Text.Core.CoreTextSelectionUpdatingResult Windows.UI.Text.Core.CoreTextSelectionUpdatingEventArgs::get_Result()
    @_property
    def result(self) -> CoreTextSelectionUpdatingResult: ...
    # System.Void Windows.UI.Text.Core.CoreTextSelectionUpdatingEventArgs::put_Result(Windows.UI.Text.Core.CoreTextSelectionUpdatingResult)
    @result.setter
    def result(self, value: CoreTextSelectionUpdatingResult) -> None: ...
    # System.Boolean Windows.UI.Text.Core.CoreTextSelectionUpdatingEventArgs::get_IsCanceled()
    @_property
    def is_canceled(self) -> bool: ...
    # Windows.UI.Text.Core.CoreTextRange Windows.UI.Text.Core.CoreTextSelectionUpdatingEventArgs::get_Selection()
    @_property
    def selection(self) -> CoreTextRange: ...

@typing.final
class CoreTextServicesConstants_Static(winrt._winrt.IInspectable_Static):
    # System.Char Windows.UI.Text.Core.CoreTextServicesConstants::get_HiddenCharacter()
    @_property
    def hidden_character(cls) -> winrt.system.Char16: ...

@typing.final
class CoreTextServicesConstants(winrt.system.Object, metaclass=CoreTextServicesConstants_Static):
    pass

@typing.final
class CoreTextServicesManager_Static(winrt._winrt.IInspectable_Static):
    # Windows.UI.Text.Core.CoreTextServicesManager Windows.UI.Text.Core.CoreTextServicesManager::GetForCurrentView()
    def get_for_current_view(cls) -> CoreTextServicesManager: ...

@typing.final
class CoreTextServicesManager(winrt.system.Object, metaclass=CoreTextServicesManager_Static):
    # Windows.UI.Text.Core.CoreTextEditContext Windows.UI.Text.Core.CoreTextServicesManager::CreateEditContext()
    def create_edit_context(self) -> CoreTextEditContext: ...
    # Windows.Foundation.EventRegistrationToken Windows.UI.Text.Core.CoreTextServicesManager::add_InputLanguageChanged(Windows.Foundation.TypedEventHandler`2<Windows.UI.Text.Core.CoreTextServicesManager,System.Object>)
    def add_input_language_changed(self, handler: windows_foundation.TypedEventHandler[CoreTextServicesManager, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.UI.Text.Core.CoreTextServicesManager::remove_InputLanguageChanged(Windows.Foundation.EventRegistrationToken)
    def remove_input_language_changed(self, cookie: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Globalization.Language Windows.UI.Text.Core.CoreTextServicesManager::get_InputLanguage()
    @_property
    def input_language(self) -> windows_globalization.Language: ...

@typing.final
class CoreTextTextRequest(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.UI.Text.Core.CoreTextTextRequest::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # System.String Windows.UI.Text.Core.CoreTextTextRequest::get_Text()
    @_property
    def text(self) -> str: ...
    # System.Void Windows.UI.Text.Core.CoreTextTextRequest::put_Text(System.String)
    @text.setter
    def text(self, value: str) -> None: ...
    # System.Boolean Windows.UI.Text.Core.CoreTextTextRequest::get_IsCanceled()
    @_property
    def is_canceled(self) -> bool: ...
    # Windows.UI.Text.Core.CoreTextRange Windows.UI.Text.Core.CoreTextTextRequest::get_Range()
    @_property
    def range(self) -> CoreTextRange: ...

@typing.final
class CoreTextTextRequestedEventArgs(winrt.system.Object):
    # Windows.UI.Text.Core.CoreTextTextRequest Windows.UI.Text.Core.CoreTextTextRequestedEventArgs::get_Request()
    @_property
    def request(self) -> CoreTextTextRequest: ...

@typing.final
class CoreTextTextUpdatingEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.UI.Text.Core.CoreTextTextUpdatingEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.UI.Text.Core.CoreTextTextUpdatingResult Windows.UI.Text.Core.CoreTextTextUpdatingEventArgs::get_Result()
    @_property
    def result(self) -> CoreTextTextUpdatingResult: ...
    # System.Void Windows.UI.Text.Core.CoreTextTextUpdatingEventArgs::put_Result(Windows.UI.Text.Core.CoreTextTextUpdatingResult)
    @result.setter
    def result(self, value: CoreTextTextUpdatingResult) -> None: ...
    # Windows.Globalization.Language Windows.UI.Text.Core.CoreTextTextUpdatingEventArgs::get_InputLanguage()
    @_property
    def input_language(self) -> windows_globalization.Language: ...
    # System.Boolean Windows.UI.Text.Core.CoreTextTextUpdatingEventArgs::get_IsCanceled()
    @_property
    def is_canceled(self) -> bool: ...
    # Windows.UI.Text.Core.CoreTextRange Windows.UI.Text.Core.CoreTextTextUpdatingEventArgs::get_NewSelection()
    @_property
    def new_selection(self) -> CoreTextRange: ...
    # Windows.UI.Text.Core.CoreTextRange Windows.UI.Text.Core.CoreTextTextUpdatingEventArgs::get_Range()
    @_property
    def range(self) -> CoreTextRange: ...
    # System.String Windows.UI.Text.Core.CoreTextTextUpdatingEventArgs::get_Text()
    @_property
    def text(self) -> str: ...

