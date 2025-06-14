# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winui3._winui3_microsoft_windows_ai_text import (
    LanguageModel,
    LanguageModelContext,
    LanguageModelOptions,
    LanguageModelResponseResult,
    TextRewriter,
    TextSummarizer,
    TextToTableConverter,
    TextToTableResponseResult,
    TextToTableRow,
)

__all__ = [
    "LanguageModelResponseStatus",
    "LanguageModel",
    "LanguageModelContext",
    "LanguageModelOptions",
    "LanguageModelResponseResult",
    "TextRewriter",
    "TextSummarizer",
    "TextToTableConverter",
    "TextToTableResponseResult",
    "TextToTableRow",
]

class LanguageModelResponseStatus(enum.IntEnum):
    COMPLETE = 0
    IN_PROGRESS = 1
    BLOCKED_BY_POLICY = 2
    PROMPT_LARGER_THAN_CONTEXT = 3
    PROMPT_BLOCKED_BY_CONTENT_MODERATION = 4
    RESPONSE_BLOCKED_BY_CONTENT_MODERATION = 5
    ERROR = 6

