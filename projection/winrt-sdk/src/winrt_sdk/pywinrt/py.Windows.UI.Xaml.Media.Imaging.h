// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.ApplicationModel.Background.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Media.h>

#include <winrt/Windows.UI.Xaml.Media.Imaging.h>

namespace py::proj::Windows::UI::Xaml::Media::Imaging
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Xaml::Media::Imaging::BitmapCreateOptions> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Xaml::Media::Imaging::DecodePixelType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Xaml::Media::Imaging::SvgImageSourceLoadStatus> = "i";


    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::BitmapCreateOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.BitmapCreateOptions";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "BitmapCreateOptions";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::DecodePixelType>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.DecodePixelType";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "DecodePixelType";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::SvgImageSourceLoadStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.SvgImageSourceLoadStatus";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SvgImageSourceLoadStatus";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.BitmapImage";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "BitmapImage";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::BitmapSource>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.BitmapSource";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "BitmapSource";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::DownloadProgressEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.DownloadProgressEventArgs";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "DownloadProgressEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::RenderTargetBitmap>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.RenderTargetBitmap";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "RenderTargetBitmap";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::SoftwareBitmapSource>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.SoftwareBitmapSource";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SoftwareBitmapSource";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::SurfaceImageSource>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.SurfaceImageSource";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SurfaceImageSource";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::SvgImageSource>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.SvgImageSource";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SvgImageSource";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::SvgImageSourceFailedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.SvgImageSourceFailedEventArgs";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SvgImageSourceFailedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::SvgImageSourceOpenedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.SvgImageSourceOpenedEventArgs";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "SvgImageSourceOpenedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.VirtualSurfaceImageSource";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "VirtualSurfaceImageSource";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::WriteableBitmap>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.WriteableBitmap";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "WriteableBitmap";
    };

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Media::Imaging::XamlRenderingBackgroundTask>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.media.imaging.XamlRenderingBackgroundTask";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.media.imaging";
        static constexpr const char* type_name = "XamlRenderingBackgroundTask";
    };
}

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

#if __has_include("py.Windows.UI.Xaml.h")
#include "py.Windows.UI.Xaml.h"
#endif

#if __has_include("py.Windows.UI.Xaml.Media.h")
#include "py.Windows.UI.Xaml.Media.h"
#endif

namespace py::impl::Windows::UI::Xaml::Media::Imaging
{
    struct DownloadProgressEventHandler
    {
        static winrt::Windows::UI::Xaml::Media::Imaging::DownloadProgressEventHandler get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)](winrt::Windows::Foundation::IInspectable const& param0, winrt::Windows::UI::Xaml::Media::Imaging::DownloadProgressEventArgs const& param1)
            {
                auto gil = py::ensure_gil();

                try
                {
                    py::pyobj_handle py_param0{py::convert(param0)};
                    if (!py_param0)
                    {
                        throw python_exception();
                    }

                    py::pyobj_handle py_param1{py::convert(param1)};
                    if (!py_param1)
                    {
                        throw python_exception();
                    }

                    py::pyobj_handle args{PyTuple_Pack(2, py_param0.get(), py_param1.get())};
                    if (!args)
                    {
                        throw python_exception();
                    }

                    py::pyobj_handle return_value{PyObject_CallObject(delegate.callable(), args.get())};
                    if (!return_value)
                    {
                        throw python_exception();
                    }
                }
                catch (python_exception)
                {
                    py::write_unraisable_and_throw();
                }
            };
        };
    };
}

namespace py::wrapper::Windows::UI::Xaml::Media::Imaging
{
    using BitmapImage = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage>;
    using BitmapSource = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::BitmapSource>;
    using DownloadProgressEventArgs = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::DownloadProgressEventArgs>;
    using RenderTargetBitmap = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::RenderTargetBitmap>;
    using SoftwareBitmapSource = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::SoftwareBitmapSource>;
    using SurfaceImageSource = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::SurfaceImageSource>;
    using SvgImageSource = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::SvgImageSource>;
    using SvgImageSourceFailedEventArgs = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::SvgImageSourceFailedEventArgs>;
    using SvgImageSourceOpenedEventArgs = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::SvgImageSourceOpenedEventArgs>;
    using VirtualSurfaceImageSource = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource>;
    using WriteableBitmap = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::WriteableBitmap>;
    using XamlRenderingBackgroundTask = py::winrt_wrapper<winrt::Windows::UI::Xaml::Media::Imaging::XamlRenderingBackgroundTask>;
}

namespace py
{
    template <>
    struct delegate_python_type<winrt::Windows::UI::Xaml::Media::Imaging::DownloadProgressEventHandler>
    {
        using type = py::impl::Windows::UI::Xaml::Media::Imaging::DownloadProgressEventHandler;
    };

}
