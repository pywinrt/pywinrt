// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");

#if __has_include("py.Microsoft.UI.Xaml.h")
#include "py.Microsoft.UI.Xaml.h"
#endif

#if __has_include("py.Windows.ApplicationModel.Background.h")
#include "py.Windows.ApplicationModel.Background.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Graphics.Imaging.h")
#include "py.Windows.Graphics.Imaging.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.ApplicationModel.Background.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>

namespace py::proj::Microsoft::UI::Xaml::Media::Imaging
{}

namespace py::impl::Microsoft::UI::Xaml::Media::Imaging
{
    struct DownloadProgressEventHandler
    {
        static winrt::Microsoft::UI::Xaml::Media::Imaging::DownloadProgressEventHandler get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)](auto param0, auto param1)
            {
                auto gil = py::ensure_gil();

                py::pyobj_handle py_param0{ py::convert(param0) };

                if (!py_param0) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw std::invalid_argument("param0");
                }

                py::pyobj_handle py_param1{ py::convert(param1) };

                if (!py_param1) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw std::invalid_argument("param1");
                }

                py::pyobj_handle args{ PyTuple_Pack(2, py_param0.get(), py_param1.get()) };

                if (!args) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }

                py::pyobj_handle return_value{ PyObject_CallObject(delegate.callable(), args.get()) };

                if (!return_value)
                {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }
            };
        };
    };
}

namespace py::wrapper::Microsoft::UI::Xaml::Media::Imaging
{
    using BitmapImage = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage>;
    using BitmapSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapSource>;
    using DownloadProgressEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::DownloadProgressEventArgs>;
    using RenderTargetBitmap = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::RenderTargetBitmap>;
    using SoftwareBitmapSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::SoftwareBitmapSource>;
    using SurfaceImageSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::SurfaceImageSource>;
    using SvgImageSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::SvgImageSource>;
    using SvgImageSourceFailedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::SvgImageSourceFailedEventArgs>;
    using SvgImageSourceOpenedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::SvgImageSourceOpenedEventArgs>;
    using VirtualSurfaceImageSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource>;
    using WriteableBitmap = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::WriteableBitmap>;
    using XamlRenderingBackgroundTask = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Media::Imaging::XamlRenderingBackgroundTask>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapCreateOptions> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Xaml::Media::Imaging::DecodePixelType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Xaml::Media::Imaging::SvgImageSourceLoadStatus> = "i";


    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapCreateOptions>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "BitmapCreateOptions";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::DecodePixelType>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "DecodePixelType";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::SvgImageSourceLoadStatus>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SvgImageSourceLoadStatus";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "BitmapImage";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "BitmapSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::DownloadProgressEventArgs>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "DownloadProgressEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::RenderTargetBitmap>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "RenderTargetBitmap";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::SoftwareBitmapSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SoftwareBitmapSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::SurfaceImageSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SurfaceImageSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::SvgImageSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SvgImageSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::SvgImageSourceFailedEventArgs>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SvgImageSourceFailedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::SvgImageSourceOpenedEventArgs>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SvgImageSourceOpenedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "VirtualSurfaceImageSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::WriteableBitmap>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "WriteableBitmap";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Media::Imaging::XamlRenderingBackgroundTask>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.media.imaging";
        static constexpr const char* type_name = "XamlRenderingBackgroundTask";
    };
    template <>
    struct delegate_python_type<winrt::Microsoft::UI::Xaml::Media::Imaging::DownloadProgressEventHandler>
    {
        using type = py::impl::Microsoft::UI::Xaml::Media::Imaging::DownloadProgressEventHandler;
    };

}