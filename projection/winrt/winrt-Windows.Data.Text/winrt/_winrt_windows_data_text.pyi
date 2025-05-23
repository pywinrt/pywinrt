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
import winrt.windows.ui.text.core as windows_ui_text_core

from winrt.windows.data.text import AlternateNormalizationFormat, TextPredictionOptions, UnicodeGeneralCategory, UnicodeNumericType
from winrt.windows.data.text import SelectableWordSegmentsTokenizingHandler, WordSegmentsTokenizingHandler

Self = typing.TypeVar('Self')

@typing.final
class TextSegment:
    @_property
    def start_position(self) -> winrt.system.UInt32: ...
    @_property
    def length(self) -> winrt.system.UInt32: ...
    def __new__(cls, start_position: winrt.system.UInt32 = 0, length: winrt.system.UInt32 = 0) -> TextSegment: ...
    def __replace__(self, /, **changes: typing.Any) -> TextSegment: ...
    def unpack(self) -> typing.Tuple[winrt.system.UInt32, winrt.system.UInt32]: ...

@typing.final
class AlternateWordForm(winrt.system.Object):
    # System.String Windows.Data.Text.AlternateWordForm::get_AlternateText()
    @_property
    def alternate_text(self) -> str: ...
    # Windows.Data.Text.AlternateNormalizationFormat Windows.Data.Text.AlternateWordForm::get_NormalizationFormat()
    @_property
    def normalization_format(self) -> AlternateNormalizationFormat: ...
    # Windows.Data.Text.TextSegment Windows.Data.Text.AlternateWordForm::get_SourceTextSegment()
    @_property
    def source_text_segment(self) -> TextSegment: ...

@typing.final
class SelectableWordSegment(winrt.system.Object):
    # Windows.Data.Text.TextSegment Windows.Data.Text.SelectableWordSegment::get_SourceTextSegment()
    @_property
    def source_text_segment(self) -> TextSegment: ...
    # System.String Windows.Data.Text.SelectableWordSegment::get_Text()
    @_property
    def text(self) -> str: ...

@typing.final
class SelectableWordsSegmenter(winrt.system.Object):
    def __new__(cls: typing.Type[Self], language: str) -> Self: ...
    # Windows.Data.Text.SelectableWordSegment Windows.Data.Text.SelectableWordsSegmenter::GetTokenAt(System.String,System.UInt32)
    def get_token_at(self, text: str, start_index: winrt.system.UInt32, /) -> SelectableWordSegment: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Data.Text.SelectableWordSegment> Windows.Data.Text.SelectableWordsSegmenter::GetTokens(System.String)
    def get_tokens(self, text: str, /) -> typing.Sequence[SelectableWordSegment]: ...
    # System.Void Windows.Data.Text.SelectableWordsSegmenter::Tokenize(System.String,System.UInt32,Windows.Data.Text.SelectableWordSegmentsTokenizingHandler)
    def tokenize(self, text: str, start_index: winrt.system.UInt32, handler: SelectableWordSegmentsTokenizingHandler, /) -> None: ...
    # System.String Windows.Data.Text.SelectableWordsSegmenter::get_ResolvedLanguage()
    @_property
    def resolved_language(self) -> str: ...

@typing.final
class SemanticTextQuery(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], aqs_filter: str) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], aqs_filter: str, filter_language: str) -> Self: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Data.Text.TextSegment> Windows.Data.Text.SemanticTextQuery::Find(System.String)
    def find(self, content: str, /) -> typing.Sequence[TextSegment]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Data.Text.TextSegment> Windows.Data.Text.SemanticTextQuery::FindInProperty(System.String,System.String)
    def find_in_property(self, property_content: str, property_name: str, /) -> typing.Sequence[TextSegment]: ...

@typing.final
class TextConversionGenerator(winrt.system.Object):
    def __new__(cls: typing.Type[Self], language_tag: str) -> Self: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<System.String>> Windows.Data.Text.TextConversionGenerator::GetCandidatesAsync(System.String)
    def get_candidates_async(self, input: str, /) -> windows_foundation.IAsyncOperation[typing.Sequence[str]]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<System.String>> Windows.Data.Text.TextConversionGenerator::GetCandidatesAsync(System.String,System.UInt32)
    def get_candidates_with_max_count_async(self, input: str, max_candidates: winrt.system.UInt32, /) -> windows_foundation.IAsyncOperation[typing.Sequence[str]]: ...
    # System.Boolean Windows.Data.Text.TextConversionGenerator::get_LanguageAvailableButNotInstalled()
    @_property
    def language_available_but_not_installed(self) -> bool: ...
    # System.String Windows.Data.Text.TextConversionGenerator::get_ResolvedLanguage()
    @_property
    def resolved_language(self) -> str: ...

@typing.final
class TextPhoneme(winrt.system.Object):
    # System.String Windows.Data.Text.TextPhoneme::get_DisplayText()
    @_property
    def display_text(self) -> str: ...
    # System.String Windows.Data.Text.TextPhoneme::get_ReadingText()
    @_property
    def reading_text(self) -> str: ...

@typing.final
class TextPredictionGenerator(winrt.system.Object):
    def __new__(cls: typing.Type[Self], language_tag: str) -> Self: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<System.String>> Windows.Data.Text.TextPredictionGenerator::GetCandidatesAsync(System.String)
    def get_candidates_async(self, input: str, /) -> windows_foundation.IAsyncOperation[typing.Sequence[str]]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<System.String>> Windows.Data.Text.TextPredictionGenerator::GetCandidatesAsync(System.String,System.UInt32)
    def get_candidates_with_max_count_async(self, input: str, max_candidates: winrt.system.UInt32, /) -> windows_foundation.IAsyncOperation[typing.Sequence[str]]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<System.String>> Windows.Data.Text.TextPredictionGenerator::GetCandidatesAsync(System.String,System.UInt32,Windows.Data.Text.TextPredictionOptions,Windows.Foundation.Collections.IIterable`1<System.String>)
    def get_candidates_with_parameters_async(self, input: str, max_candidates: winrt.system.UInt32, prediction_options: TextPredictionOptions, previous_strings: typing.Iterable[str], /) -> windows_foundation.IAsyncOperation[typing.Sequence[str]]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<System.String>> Windows.Data.Text.TextPredictionGenerator::GetNextWordCandidatesAsync(System.UInt32,Windows.Foundation.Collections.IIterable`1<System.String>)
    def get_next_word_candidates_async(self, max_candidates: winrt.system.UInt32, previous_strings: typing.Iterable[str], /) -> windows_foundation.IAsyncOperation[typing.Sequence[str]]: ...
    # System.Boolean Windows.Data.Text.TextPredictionGenerator::get_LanguageAvailableButNotInstalled()
    @_property
    def language_available_but_not_installed(self) -> bool: ...
    # System.String Windows.Data.Text.TextPredictionGenerator::get_ResolvedLanguage()
    @_property
    def resolved_language(self) -> str: ...
    # Windows.UI.Text.Core.CoreTextInputScope Windows.Data.Text.TextPredictionGenerator::get_InputScope()
    @_property
    def input_scope(self) -> windows_ui_text_core.CoreTextInputScope: ...
    # System.Void Windows.Data.Text.TextPredictionGenerator::put_InputScope(Windows.UI.Text.Core.CoreTextInputScope)
    @input_scope.setter
    def input_scope(self, value: windows_ui_text_core.CoreTextInputScope) -> None: ...

@typing.final
class TextReverseConversionGenerator(winrt.system.Object):
    def __new__(cls: typing.Type[Self], language_tag: str) -> Self: ...
    # Windows.Foundation.IAsyncOperation`1<System.String> Windows.Data.Text.TextReverseConversionGenerator::ConvertBackAsync(System.String)
    def convert_back_async(self, input: str, /) -> windows_foundation.IAsyncOperation[str]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<Windows.Data.Text.TextPhoneme>> Windows.Data.Text.TextReverseConversionGenerator::GetPhonemesAsync(System.String)
    def get_phonemes_async(self, input: str, /) -> windows_foundation.IAsyncOperation[typing.Sequence[TextPhoneme]]: ...
    # System.Boolean Windows.Data.Text.TextReverseConversionGenerator::get_LanguageAvailableButNotInstalled()
    @_property
    def language_available_but_not_installed(self) -> bool: ...
    # System.String Windows.Data.Text.TextReverseConversionGenerator::get_ResolvedLanguage()
    @_property
    def resolved_language(self) -> str: ...

@typing.final
class UnicodeCharacters_Static(winrt._winrt.IInspectable_Static):
    # System.UInt32 Windows.Data.Text.UnicodeCharacters::GetCodepointFromSurrogatePair(System.UInt32,System.UInt32)
    def get_codepoint_from_surrogate_pair(cls, high_surrogate: winrt.system.UInt32, low_surrogate: winrt.system.UInt32, /) -> winrt.system.UInt32: ...
    # Windows.Data.Text.UnicodeGeneralCategory Windows.Data.Text.UnicodeCharacters::GetGeneralCategory(System.UInt32)
    def get_general_category(cls, codepoint: winrt.system.UInt32, /) -> UnicodeGeneralCategory: ...
    # Windows.Data.Text.UnicodeNumericType Windows.Data.Text.UnicodeCharacters::GetNumericType(System.UInt32)
    def get_numeric_type(cls, codepoint: winrt.system.UInt32, /) -> UnicodeNumericType: ...
    # System.Void Windows.Data.Text.UnicodeCharacters::GetSurrogatePairFromCodepoint(System.UInt32,System.Char&,System.Char&)
    def get_surrogate_pair_from_codepoint(cls, codepoint: winrt.system.UInt32, /) -> typing.Tuple[winrt.system.Char16, winrt.system.Char16]: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsAlphabetic(System.UInt32)
    def is_alphabetic(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsCased(System.UInt32)
    def is_cased(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsGraphemeBase(System.UInt32)
    def is_grapheme_base(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsGraphemeExtend(System.UInt32)
    def is_grapheme_extend(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsHighSurrogate(System.UInt32)
    def is_high_surrogate(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsIdContinue(System.UInt32)
    def is_id_continue(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsIdStart(System.UInt32)
    def is_id_start(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsLowSurrogate(System.UInt32)
    def is_low_surrogate(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsLowercase(System.UInt32)
    def is_lowercase(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsNoncharacter(System.UInt32)
    def is_noncharacter(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsSupplementary(System.UInt32)
    def is_supplementary(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsUppercase(System.UInt32)
    def is_uppercase(cls, codepoint: winrt.system.UInt32, /) -> bool: ...
    # System.Boolean Windows.Data.Text.UnicodeCharacters::IsWhitespace(System.UInt32)
    def is_whitespace(cls, codepoint: winrt.system.UInt32, /) -> bool: ...

@typing.final
class UnicodeCharacters(winrt.system.Object, metaclass=UnicodeCharacters_Static):
    pass

@typing.final
class WordSegment(winrt.system.Object):
    # Windows.Foundation.Collections.IVectorView`1<Windows.Data.Text.AlternateWordForm> Windows.Data.Text.WordSegment::get_AlternateForms()
    @_property
    def alternate_forms(self) -> typing.Sequence[AlternateWordForm]: ...
    # Windows.Data.Text.TextSegment Windows.Data.Text.WordSegment::get_SourceTextSegment()
    @_property
    def source_text_segment(self) -> TextSegment: ...
    # System.String Windows.Data.Text.WordSegment::get_Text()
    @_property
    def text(self) -> str: ...

@typing.final
class WordsSegmenter(winrt.system.Object):
    def __new__(cls: typing.Type[Self], language: str) -> Self: ...
    # Windows.Data.Text.WordSegment Windows.Data.Text.WordsSegmenter::GetTokenAt(System.String,System.UInt32)
    def get_token_at(self, text: str, start_index: winrt.system.UInt32, /) -> WordSegment: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Data.Text.WordSegment> Windows.Data.Text.WordsSegmenter::GetTokens(System.String)
    def get_tokens(self, text: str, /) -> typing.Sequence[WordSegment]: ...
    # System.Void Windows.Data.Text.WordsSegmenter::Tokenize(System.String,System.UInt32,Windows.Data.Text.WordSegmentsTokenizingHandler)
    def tokenize(self, text: str, start_index: winrt.system.UInt32, handler: WordSegmentsTokenizingHandler, /) -> None: ...
    # System.String Windows.Data.Text.WordsSegmenter::get_ResolvedLanguage()
    @_property
    def resolved_language(self) -> str: ...

