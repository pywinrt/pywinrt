# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_storage_compression import (
    Compressor,
    Decompressor,
)

__all__ = [
    "CompressAlgorithm",
    "Compressor",
    "Decompressor",
]

class CompressAlgorithm(enum.IntEnum):
    INVALID_ALGORITHM = 0
    NULL_ALGORITHM = 1
    MSZIP = 2
    XPRESS = 3
    XPRESS_HUFF = 4
    LZMS = 5

