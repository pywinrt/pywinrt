# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property
from abc import abstractmethod

import winrt._winrt
import winrt.system
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections
import winrt.windows.storage as windows_storage
import winrt.windows.storage.streams as windows_storage_streams

Self = typing.TypeVar('Self')

@typing.final
class LocalContentSuggestionSettings(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.Boolean Windows.ApplicationModel.Search.LocalContentSuggestionSettings::get_Enabled()
    @_property
    def enabled(self) -> bool: ...
    # System.Void Windows.ApplicationModel.Search.LocalContentSuggestionSettings::put_Enabled(System.Boolean)
    @enabled.setter
    def enabled(self, value: bool) -> None: ...
    # System.String Windows.ApplicationModel.Search.LocalContentSuggestionSettings::get_AqsFilter()
    @_property
    def aqs_filter(self) -> str: ...
    # System.Void Windows.ApplicationModel.Search.LocalContentSuggestionSettings::put_AqsFilter(System.String)
    @aqs_filter.setter
    def aqs_filter(self, value: str) -> None: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Storage.StorageFolder> Windows.ApplicationModel.Search.LocalContentSuggestionSettings::get_Locations()
    @_property
    def locations(self) -> typing.MutableSequence[windows_storage.StorageFolder]: ...
    # Windows.Foundation.Collections.IVector`1<System.String> Windows.ApplicationModel.Search.LocalContentSuggestionSettings::get_PropertiesToMatch()
    @_property
    def properties_to_match(self) -> typing.MutableSequence[str]: ...

@typing.final
class SearchPane_Static(winrt._winrt.IInspectable_Static):
    # Windows.ApplicationModel.Search.SearchPane Windows.ApplicationModel.Search.SearchPane::GetForCurrentView()
    # @deprecated("ISearchPaneStatics may be altered or unavailable for releases after Windows 10.")
    def get_for_current_view(cls) -> SearchPane: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::HideThisApplication()
    # @deprecated("ISearchPaneStaticsWithHideThisApplication may be altered or unavailable for releases after Windows 10.")
    def hide_this_application(cls) -> None: ...

@typing.final
class SearchPane(winrt.system.Object, metaclass=SearchPane_Static):
    # System.Void Windows.ApplicationModel.Search.SearchPane::SetLocalContentSuggestionSettings(Windows.ApplicationModel.Search.LocalContentSuggestionSettings)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def set_local_content_suggestion_settings(self, settings: LocalContentSuggestionSettings, /) -> None: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::Show()
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def show_overload_default(self) -> None: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::Show(System.String)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def show_overload_with_query(self, query: str, /) -> None: ...
    # System.Boolean Windows.ApplicationModel.Search.SearchPane::TrySetQueryText(System.String)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def try_set_query_text(self, query: str, /) -> bool: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Search.SearchPane::add_QueryChanged(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Search.SearchPane,Windows.ApplicationModel.Search.SearchPaneQueryChangedEventArgs>)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def add_query_changed(self, handler: windows_foundation.TypedEventHandler[SearchPane, SearchPaneQueryChangedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::remove_QueryChanged(Windows.Foundation.EventRegistrationToken)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def remove_query_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Search.SearchPane::add_QuerySubmitted(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Search.SearchPane,Windows.ApplicationModel.Search.SearchPaneQuerySubmittedEventArgs>)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def add_query_submitted(self, handler: windows_foundation.TypedEventHandler[SearchPane, SearchPaneQuerySubmittedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::remove_QuerySubmitted(Windows.Foundation.EventRegistrationToken)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def remove_query_submitted(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Search.SearchPane::add_ResultSuggestionChosen(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Search.SearchPane,Windows.ApplicationModel.Search.SearchPaneResultSuggestionChosenEventArgs>)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def add_result_suggestion_chosen(self, handler: windows_foundation.TypedEventHandler[SearchPane, SearchPaneResultSuggestionChosenEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::remove_ResultSuggestionChosen(Windows.Foundation.EventRegistrationToken)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def remove_result_suggestion_chosen(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Search.SearchPane::add_SuggestionsRequested(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Search.SearchPane,Windows.ApplicationModel.Search.SearchPaneSuggestionsRequestedEventArgs>)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def add_suggestions_requested(self, handler: windows_foundation.TypedEventHandler[SearchPane, SearchPaneSuggestionsRequestedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::remove_SuggestionsRequested(Windows.Foundation.EventRegistrationToken)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def remove_suggestions_requested(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Search.SearchPane::add_VisibilityChanged(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Search.SearchPane,Windows.ApplicationModel.Search.SearchPaneVisibilityChangedEventArgs>)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def add_visibility_changed(self, handler: windows_foundation.TypedEventHandler[SearchPane, SearchPaneVisibilityChangedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::remove_VisibilityChanged(Windows.Foundation.EventRegistrationToken)
    # @deprecated("ISearchPane may be altered or unavailable for releases after Windows 10.")
    def remove_visibility_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # System.Boolean Windows.ApplicationModel.Search.SearchPane::get_ShowOnKeyboardInput()
    @_property
    def show_on_keyboard_input(self) -> bool: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::put_ShowOnKeyboardInput(System.Boolean)
    @show_on_keyboard_input.setter
    def show_on_keyboard_input(self, value: bool) -> None: ...
    # System.Boolean Windows.ApplicationModel.Search.SearchPane::get_SearchHistoryEnabled()
    @_property
    def search_history_enabled(self) -> bool: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::put_SearchHistoryEnabled(System.Boolean)
    @search_history_enabled.setter
    def search_history_enabled(self, value: bool) -> None: ...
    # System.String Windows.ApplicationModel.Search.SearchPane::get_SearchHistoryContext()
    @_property
    def search_history_context(self) -> str: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::put_SearchHistoryContext(System.String)
    @search_history_context.setter
    def search_history_context(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Search.SearchPane::get_PlaceholderText()
    @_property
    def placeholder_text(self) -> str: ...
    # System.Void Windows.ApplicationModel.Search.SearchPane::put_PlaceholderText(System.String)
    @placeholder_text.setter
    def placeholder_text(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Search.SearchPane::get_Language()
    @_property
    def language(self) -> str: ...
    # System.String Windows.ApplicationModel.Search.SearchPane::get_QueryText()
    @_property
    def query_text(self) -> str: ...
    # System.Boolean Windows.ApplicationModel.Search.SearchPane::get_Visible()
    @_property
    def visible(self) -> bool: ...

@typing.final
class SearchPaneQueryChangedEventArgs(winrt.system.Object, ISearchPaneQueryChangedEventArgs):
    # System.String Windows.ApplicationModel.Search.SearchPaneQueryChangedEventArgs::get_Language()
    @_property
    def language(self) -> str: ...
    # Windows.ApplicationModel.Search.SearchPaneQueryLinguisticDetails Windows.ApplicationModel.Search.SearchPaneQueryChangedEventArgs::get_LinguisticDetails()
    @_property
    def linguistic_details(self) -> SearchPaneQueryLinguisticDetails: ...
    # System.String Windows.ApplicationModel.Search.SearchPaneQueryChangedEventArgs::get_QueryText()
    @_property
    def query_text(self) -> str: ...

@typing.final
class SearchPaneQueryLinguisticDetails(winrt.system.Object):
    # Windows.Foundation.Collections.IVectorView`1<System.String> Windows.ApplicationModel.Search.SearchPaneQueryLinguisticDetails::get_QueryTextAlternatives()
    @_property
    def query_text_alternatives(self) -> typing.Sequence[str]: ...
    # System.UInt32 Windows.ApplicationModel.Search.SearchPaneQueryLinguisticDetails::get_QueryTextCompositionLength()
    @_property
    def query_text_composition_length(self) -> winrt.system.UInt32: ...
    # System.UInt32 Windows.ApplicationModel.Search.SearchPaneQueryLinguisticDetails::get_QueryTextCompositionStart()
    @_property
    def query_text_composition_start(self) -> winrt.system.UInt32: ...

@typing.final
class SearchPaneQuerySubmittedEventArgs(winrt.system.Object):
    # System.String Windows.ApplicationModel.Search.SearchPaneQuerySubmittedEventArgs::get_Language()
    @_property
    def language(self) -> str: ...
    # System.String Windows.ApplicationModel.Search.SearchPaneQuerySubmittedEventArgs::get_QueryText()
    @_property
    def query_text(self) -> str: ...
    # Windows.ApplicationModel.Search.SearchPaneQueryLinguisticDetails Windows.ApplicationModel.Search.SearchPaneQuerySubmittedEventArgs::get_LinguisticDetails()
    @_property
    def linguistic_details(self) -> SearchPaneQueryLinguisticDetails: ...

@typing.final
class SearchPaneResultSuggestionChosenEventArgs(winrt.system.Object):
    # System.String Windows.ApplicationModel.Search.SearchPaneResultSuggestionChosenEventArgs::get_Tag()
    @_property
    def tag(self) -> str: ...

@typing.final
class SearchPaneSuggestionsRequest(winrt.system.Object):
    # Windows.ApplicationModel.Search.SearchPaneSuggestionsRequestDeferral Windows.ApplicationModel.Search.SearchPaneSuggestionsRequest::GetDeferral()
    # @deprecated("ISearchPaneSuggestionsRequest may be altered or unavailable for releases after Windows 10.")
    def get_deferral(self) -> SearchPaneSuggestionsRequestDeferral: ...
    # System.Boolean Windows.ApplicationModel.Search.SearchPaneSuggestionsRequest::get_IsCanceled()
    @_property
    def is_canceled(self) -> bool: ...
    # Windows.ApplicationModel.Search.SearchSuggestionCollection Windows.ApplicationModel.Search.SearchPaneSuggestionsRequest::get_SearchSuggestionCollection()
    @_property
    def search_suggestion_collection(self) -> SearchSuggestionCollection: ...

@typing.final
class SearchPaneSuggestionsRequestDeferral(winrt.system.Object):
    # System.Void Windows.ApplicationModel.Search.SearchPaneSuggestionsRequestDeferral::Complete()
    # @deprecated("ISearchPaneSuggestionsRequestDeferral may be altered or unavailable for releases after Windows 10.")
    def complete(self) -> None: ...

@typing.final
class SearchPaneSuggestionsRequestedEventArgs(winrt.system.Object, ISearchPaneQueryChangedEventArgs):
    # System.String Windows.ApplicationModel.Search.SearchPaneSuggestionsRequestedEventArgs::get_Language()
    @_property
    def language(self) -> str: ...
    # Windows.ApplicationModel.Search.SearchPaneQueryLinguisticDetails Windows.ApplicationModel.Search.SearchPaneSuggestionsRequestedEventArgs::get_LinguisticDetails()
    @_property
    def linguistic_details(self) -> SearchPaneQueryLinguisticDetails: ...
    # System.String Windows.ApplicationModel.Search.SearchPaneSuggestionsRequestedEventArgs::get_QueryText()
    @_property
    def query_text(self) -> str: ...
    # Windows.ApplicationModel.Search.SearchPaneSuggestionsRequest Windows.ApplicationModel.Search.SearchPaneSuggestionsRequestedEventArgs::get_Request()
    @_property
    def request(self) -> SearchPaneSuggestionsRequest: ...

@typing.final
class SearchPaneVisibilityChangedEventArgs(winrt.system.Object):
    # System.Boolean Windows.ApplicationModel.Search.SearchPaneVisibilityChangedEventArgs::get_Visible()
    @_property
    def visible(self) -> bool: ...

@typing.final
class SearchQueryLinguisticDetails(winrt.system.Object):
    def __new__(cls: typing.Type[Self], query_text_alternatives: typing.Iterable[str], query_text_composition_start: winrt.system.UInt32, query_text_composition_length: winrt.system.UInt32) -> Self: ...
    # Windows.Foundation.Collections.IVectorView`1<System.String> Windows.ApplicationModel.Search.SearchQueryLinguisticDetails::get_QueryTextAlternatives()
    @_property
    def query_text_alternatives(self) -> typing.Sequence[str]: ...
    # System.UInt32 Windows.ApplicationModel.Search.SearchQueryLinguisticDetails::get_QueryTextCompositionLength()
    @_property
    def query_text_composition_length(self) -> winrt.system.UInt32: ...
    # System.UInt32 Windows.ApplicationModel.Search.SearchQueryLinguisticDetails::get_QueryTextCompositionStart()
    @_property
    def query_text_composition_start(self) -> winrt.system.UInt32: ...

@typing.final
class SearchSuggestionCollection(winrt.system.Object):
    # System.Void Windows.ApplicationModel.Search.SearchSuggestionCollection::AppendQuerySuggestion(System.String)
    def append_query_suggestion(self, text: str, /) -> None: ...
    # System.Void Windows.ApplicationModel.Search.SearchSuggestionCollection::AppendQuerySuggestions(Windows.Foundation.Collections.IIterable`1<System.String>)
    def append_query_suggestions(self, suggestions: typing.Iterable[str], /) -> None: ...
    # System.Void Windows.ApplicationModel.Search.SearchSuggestionCollection::AppendResultSuggestion(System.String,System.String,System.String,Windows.Storage.Streams.IRandomAccessStreamReference,System.String)
    def append_result_suggestion(self, text: str, detail_text: str, tag: str, image: windows_storage_streams.IRandomAccessStreamReference, image_alternate_text: str, /) -> None: ...
    # System.Void Windows.ApplicationModel.Search.SearchSuggestionCollection::AppendSearchSeparator(System.String)
    def append_search_separator(self, label: str, /) -> None: ...
    # System.UInt32 Windows.ApplicationModel.Search.SearchSuggestionCollection::get_Size()
    @_property
    def size(self) -> winrt.system.UInt32: ...

@typing.final
class SearchSuggestionsRequest(winrt.system.Object):
    # Windows.ApplicationModel.Search.SearchSuggestionsRequestDeferral Windows.ApplicationModel.Search.SearchSuggestionsRequest::GetDeferral()
    def get_deferral(self) -> SearchSuggestionsRequestDeferral: ...
    # System.Boolean Windows.ApplicationModel.Search.SearchSuggestionsRequest::get_IsCanceled()
    @_property
    def is_canceled(self) -> bool: ...
    # Windows.ApplicationModel.Search.SearchSuggestionCollection Windows.ApplicationModel.Search.SearchSuggestionsRequest::get_SearchSuggestionCollection()
    @_property
    def search_suggestion_collection(self) -> SearchSuggestionCollection: ...

@typing.final
class SearchSuggestionsRequestDeferral(winrt.system.Object):
    # System.Void Windows.ApplicationModel.Search.SearchSuggestionsRequestDeferral::Complete()
    def complete(self) -> None: ...

@typing.final
class _ISearchPaneQueryChangedEventArgs: ...

class ISearchPaneQueryChangedEventArgs(winrt._winrt.IInspectable):
    # System.String Windows.ApplicationModel.Search.ISearchPaneQueryChangedEventArgs::get_Language()
    @_property
    @abstractmethod
    def language(self) -> str: ...
    # Windows.ApplicationModel.Search.SearchPaneQueryLinguisticDetails Windows.ApplicationModel.Search.ISearchPaneQueryChangedEventArgs::get_LinguisticDetails()
    @_property
    @abstractmethod
    def linguistic_details(self) -> SearchPaneQueryLinguisticDetails: ...
    # System.String Windows.ApplicationModel.Search.ISearchPaneQueryChangedEventArgs::get_QueryText()
    @_property
    @abstractmethod
    def query_text(self) -> str: ...

