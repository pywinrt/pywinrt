# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt import _winrt_microsoft_ui_xaml_automation_text

__all__ = [
    "TextPatternRangeEndpoint",
    "TextUnit",
]

class TextPatternRangeEndpoint(enum.IntEnum):
    START = 0
    END = 1

class TextUnit(enum.IntEnum):
    CHARACTER = 0
    FORMAT = 1
    WORD = 2
    LINE = 3
    PARAGRAPH = 4
    PAGE = 5
    DOCUMENT = 6
