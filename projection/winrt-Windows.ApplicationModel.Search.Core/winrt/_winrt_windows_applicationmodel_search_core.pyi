# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.applicationmodel.search
import winrt.windows.foundation
import winrt.windows.foundation.collections
import winrt.windows.storage.streams

from winrt.windows.applicationmodel.search.core import SearchSuggestionKind

Self = typing.TypeVar('Self')

@typing.final
class RequestingFocusOnKeyboardInputEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> RequestingFocusOnKeyboardInputEventArgs: ...

@typing.final
class SearchSuggestion(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SearchSuggestion: ...
    @_property
    def detail_text(self) -> str: ...
    @_property
    def image(self) -> typing.Optional[winrt.windows.storage.streams.IRandomAccessStreamReference]: ...
    @_property
    def image_alternate_text(self) -> str: ...
    @_property
    def kind(self) -> SearchSuggestionKind: ...
    @_property
    def tag(self) -> str: ...
    @_property
    def text(self) -> str: ...

@typing.final
class SearchSuggestionManager(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SearchSuggestionManager: ...
    def __new__(cls: typing.Type[SearchSuggestionManager]) -> SearchSuggestionManager:...
    @typing.overload
    def add_to_history(self, query_text: str, /) -> None: ...
    @typing.overload
    def add_to_history(self, query_text: str, language: str, /) -> None: ...
    def clear_history(self) -> None: ...
    def set_local_content_suggestion_settings(self, settings: typing.Optional[winrt.windows.applicationmodel.search.LocalContentSuggestionSettings], /) -> None: ...
    @typing.overload
    def set_query(self, query_text: str, /) -> None: ...
    @typing.overload
    def set_query(self, query_text: str, language: str, /) -> None: ...
    @typing.overload
    def set_query(self, query_text: str, language: str, linguistic_details: typing.Optional[winrt.windows.applicationmodel.search.SearchQueryLinguisticDetails], /) -> None: ...
    def add_requesting_focus_on_keyboard_input(self, handler: winrt.windows.foundation.TypedEventHandler[SearchSuggestionManager, RequestingFocusOnKeyboardInputEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_requesting_focus_on_keyboard_input(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_suggestions_requested(self, handler: winrt.windows.foundation.TypedEventHandler[SearchSuggestionManager, SearchSuggestionsRequestedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_suggestions_requested(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def search_history_enabled(self) -> bool: ...
    @search_history_enabled.setter
    def search_history_enabled(self, value: bool) -> None: ...
    @_property
    def search_history_context(self) -> str: ...
    @search_history_context.setter
    def search_history_context(self, value: str) -> None: ...
    @_property
    def suggestions(self) -> typing.Optional[winrt.windows.foundation.collections.IObservableVector[SearchSuggestion]]: ...

@typing.final
class SearchSuggestionsRequestedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SearchSuggestionsRequestedEventArgs: ...
    @_property
    def language(self) -> str: ...
    @_property
    def linguistic_details(self) -> typing.Optional[winrt.windows.applicationmodel.search.SearchQueryLinguisticDetails]: ...
    @_property
    def query_text(self) -> str: ...
    @_property
    def request(self) -> typing.Optional[winrt.windows.applicationmodel.search.SearchSuggestionsRequest]: ...

