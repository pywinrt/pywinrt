// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Graphics.DirectX.Direct3D11.h")
#include "py.Windows.Graphics.DirectX.Direct3D11.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Windows.Graphics.Imaging.h>

namespace py::proj::Windows::Graphics::Imaging
{}

namespace py::impl::Windows::Graphics::Imaging
{}

namespace py::wrapper::Windows::Graphics::Imaging
{
    using BitmapBuffer = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::BitmapBuffer>;
    using BitmapCodecInformation = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::BitmapCodecInformation>;
    using BitmapDecoder = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::BitmapDecoder>;
    using BitmapEncoder = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::BitmapEncoder>;
    using BitmapFrame = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::BitmapFrame>;
    using BitmapProperties = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::BitmapProperties>;
    using BitmapPropertiesView = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::BitmapPropertiesView>;
    using BitmapPropertySet = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::BitmapPropertySet>;
    using BitmapTransform = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::BitmapTransform>;
    using BitmapTypedValue = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::BitmapTypedValue>;
    using ImageStream = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::ImageStream>;
    using PixelDataProvider = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::PixelDataProvider>;
    using SoftwareBitmap = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::SoftwareBitmap>;
    using IBitmapFrame = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::IBitmapFrame>;
    using IBitmapFrameWithSoftwareBitmap = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::IBitmapFrameWithSoftwareBitmap>;
    using IBitmapPropertiesView = py::winrt_wrapper<winrt::Windows::Graphics::Imaging::IBitmapPropertiesView>;
    using BitmapBounds = py::winrt_struct_wrapper<winrt::Windows::Graphics::Imaging::BitmapBounds>;
    using BitmapPlaneDescription = py::winrt_struct_wrapper<winrt::Windows::Graphics::Imaging::BitmapPlaneDescription>;
    using BitmapSize = py::winrt_struct_wrapper<winrt::Windows::Graphics::Imaging::BitmapSize>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::BitmapAlphaMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::BitmapBufferAccessMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::BitmapFlip> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::BitmapInterpolationMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::BitmapPixelFormat> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::BitmapRotation> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::ColorManagementMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::ExifOrientationMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::JpegSubsamplingMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::PngFilterMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::TiffCompressionMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::BitmapBounds> = "T{I:x:I:y:I:width:I:height:}";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::BitmapPlaneDescription> = "T{i:start_index:i:width:i:height:i:stride:}";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Imaging::BitmapSize> = "T{I:width:I:height:}";


    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapAlphaMode>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapAlphaMode";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapBufferAccessMode>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapBufferAccessMode";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapFlip>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapFlip";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapInterpolationMode>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapInterpolationMode";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapPixelFormat>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapPixelFormat";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapRotation>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapRotation";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::ColorManagementMode>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "ColorManagementMode";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::ExifOrientationMode>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "ExifOrientationMode";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::JpegSubsamplingMode>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "JpegSubsamplingMode";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::PngFilterMode>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "PngFilterMode";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::TiffCompressionMode>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "TiffCompressionMode";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapBuffer>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapBuffer";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapCodecInformation>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapCodecInformation";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapDecoder>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapDecoder";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapEncoder>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapEncoder";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapFrame>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapFrame";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapProperties>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapProperties";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapPropertiesView>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapPropertiesView";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapPropertySet>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapPropertySet";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapTransform>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapTransform";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapTypedValue>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapTypedValue";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::ImageStream>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "ImageStream";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::PixelDataProvider>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "PixelDataProvider";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::SoftwareBitmap>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "SoftwareBitmap";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::IBitmapFrame>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "IBitmapFrame";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::IBitmapFrameWithSoftwareBitmap>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "IBitmapFrameWithSoftwareBitmap";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::IBitmapPropertiesView>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "IBitmapPropertiesView";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapBounds>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapBounds";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapPlaneDescription>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapPlaneDescription";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Imaging::BitmapSize>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.imaging";
        static constexpr const char* type_name = "BitmapSize";
    };
}
