# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from . import _winrt_windows_graphics_imaging

class BitmapAlphaMode(enum.IntEnum):
    PREMULTIPLIED = 0
    STRAIGHT = 1
    IGNORE = 2

class BitmapBufferAccessMode(enum.IntEnum):
    READ = 0
    READ_WRITE = 1
    WRITE = 2

class BitmapFlip(enum.IntEnum):
    NONE = 0
    HORIZONTAL = 1
    VERTICAL = 2

class BitmapInterpolationMode(enum.IntEnum):
    NEAREST_NEIGHBOR = 0
    LINEAR = 1
    CUBIC = 2
    FANT = 3

class BitmapPixelFormat(enum.IntEnum):
    UNKNOWN = 0
    RGBA16 = 12
    RGBA8 = 30
    GRAY16 = 57
    GRAY8 = 62
    BGRA8 = 87
    NV12 = 103
    P010 = 104
    YUY2 = 107

class BitmapRotation(enum.IntEnum):
    NONE = 0
    CLOCKWISE90_DEGREES = 1
    CLOCKWISE180_DEGREES = 2
    CLOCKWISE270_DEGREES = 3

class ColorManagementMode(enum.IntEnum):
    DO_NOT_COLOR_MANAGE = 0
    COLOR_MANAGE_TO_S_RGB = 1

class ExifOrientationMode(enum.IntEnum):
    IGNORE_EXIF_ORIENTATION = 0
    RESPECT_EXIF_ORIENTATION = 1

class JpegSubsamplingMode(enum.IntEnum):
    DEFAULT = 0
    Y4_CB2_CR0 = 1
    Y4_CB2_CR2 = 2
    Y4_CB4_CR4 = 3

class PngFilterMode(enum.IntEnum):
    AUTOMATIC = 0
    NONE = 1
    SUB = 2
    UP = 3
    AVERAGE = 4
    PAETH = 5
    ADAPTIVE = 6

class TiffCompressionMode(enum.IntEnum):
    AUTOMATIC = 0
    NONE = 1
    CCITT3 = 2
    CCITT4 = 3
    LZW = 4
    RLE = 5
    ZIP = 6
    LZWH_DIFFERENCING = 7

BitmapBounds = _winrt_windows_graphics_imaging.BitmapBounds
BitmapPlaneDescription = _winrt_windows_graphics_imaging.BitmapPlaneDescription
BitmapSize = _winrt_windows_graphics_imaging.BitmapSize
BitmapBuffer = _winrt_windows_graphics_imaging.BitmapBuffer
BitmapCodecInformation = _winrt_windows_graphics_imaging.BitmapCodecInformation
BitmapDecoder = _winrt_windows_graphics_imaging.BitmapDecoder
BitmapEncoder = _winrt_windows_graphics_imaging.BitmapEncoder
BitmapFrame = _winrt_windows_graphics_imaging.BitmapFrame
BitmapProperties = _winrt_windows_graphics_imaging.BitmapProperties
BitmapPropertiesView = _winrt_windows_graphics_imaging.BitmapPropertiesView
BitmapPropertySet = _winrt_windows_graphics_imaging.BitmapPropertySet
winrt.system._mixin_mutable_mapping(BitmapPropertySet)
BitmapTransform = _winrt_windows_graphics_imaging.BitmapTransform
BitmapTypedValue = _winrt_windows_graphics_imaging.BitmapTypedValue
ImageStream = _winrt_windows_graphics_imaging.ImageStream
PixelDataProvider = _winrt_windows_graphics_imaging.PixelDataProvider
SoftwareBitmap = _winrt_windows_graphics_imaging.SoftwareBitmap
IBitmapFrame = _winrt_windows_graphics_imaging.IBitmapFrame
IBitmapFrameWithSoftwareBitmap = _winrt_windows_graphics_imaging.IBitmapFrameWithSoftwareBitmap
IBitmapPropertiesView = _winrt_windows_graphics_imaging.IBitmapPropertiesView