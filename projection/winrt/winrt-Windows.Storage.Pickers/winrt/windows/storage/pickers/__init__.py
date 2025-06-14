# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.runtime._internals
import winrt.system
from winrt._winrt_windows_storage_pickers import (
    FileExtensionVector,
    FileOpenPicker,
    FilePickerFileTypesOrderedMap,
    FilePickerSelectedFilesArray,
    FileSavePicker,
    FolderPicker,
)

__all__ = [
    "PickerLocationId",
    "PickerViewMode",
    "FileExtensionVector",
    "FileOpenPicker",
    "FilePickerFileTypesOrderedMap",
    "FilePickerSelectedFilesArray",
    "FileSavePicker",
    "FolderPicker",
]

class PickerLocationId(enum.IntEnum):
    DOCUMENTS_LIBRARY = 0
    COMPUTER_FOLDER = 1
    DESKTOP = 2
    DOWNLOADS = 3
    HOME_GROUP = 4
    MUSIC_LIBRARY = 5
    PICTURES_LIBRARY = 6
    VIDEOS_LIBRARY = 7
    OBJECTS_3D = 8
    UNSPECIFIED = 9

class PickerViewMode(enum.IntEnum):
    LIST = 0
    THUMBNAIL = 1

winrt.runtime._internals.mixin_mutable_sequence(FileExtensionVector)
winrt.runtime._internals.mixin_mutable_mapping(FilePickerFileTypesOrderedMap)
winrt.runtime._internals.mixin_sequence(FilePickerSelectedFilesArray)
