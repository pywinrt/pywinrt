// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Effects.h>
#include <winrt/Windows.Media.MediaProperties.h>
#include <winrt/Windows.Media.Transcoding.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.UI.h>

#include <winrt/Windows.Media.Editing.h>

namespace py::proj::Windows::Media::Editing
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Editing::MediaTrimmingPreference> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Editing::VideoFramePrecision> = "i";


    template<>
    struct py_type<winrt::Windows::Media::Editing::MediaTrimmingPreference>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.editing.MediaTrimmingPreference";
        static constexpr const char* module_name = "winrt.windows.media.editing";
        static constexpr const char* type_name = "MediaTrimmingPreference";
    };

    template<>
    struct py_type<winrt::Windows::Media::Editing::VideoFramePrecision>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.editing.VideoFramePrecision";
        static constexpr const char* module_name = "winrt.windows.media.editing";
        static constexpr const char* type_name = "VideoFramePrecision";
    };

    template<>
    struct py_type<winrt::Windows::Media::Editing::BackgroundAudioTrack>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.editing.BackgroundAudioTrack";
        static constexpr const char* module_name = "winrt.windows.media.editing";
        static constexpr const char* type_name = "BackgroundAudioTrack";
    };

    template<>
    struct py_type<winrt::Windows::Media::Editing::EmbeddedAudioTrack>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.editing.EmbeddedAudioTrack";
        static constexpr const char* module_name = "winrt.windows.media.editing";
        static constexpr const char* type_name = "EmbeddedAudioTrack";
    };

    template<>
    struct py_type<winrt::Windows::Media::Editing::MediaClip>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.editing.MediaClip";
        static constexpr const char* module_name = "winrt.windows.media.editing";
        static constexpr const char* type_name = "MediaClip";
    };

    template<>
    struct py_type<winrt::Windows::Media::Editing::MediaComposition>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.editing.MediaComposition";
        static constexpr const char* module_name = "winrt.windows.media.editing";
        static constexpr const char* type_name = "MediaComposition";
    };

    template<>
    struct py_type<winrt::Windows::Media::Editing::MediaOverlay>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.editing.MediaOverlay";
        static constexpr const char* module_name = "winrt.windows.media.editing";
        static constexpr const char* type_name = "MediaOverlay";
    };

    template<>
    struct py_type<winrt::Windows::Media::Editing::MediaOverlayLayer>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.editing.MediaOverlayLayer";
        static constexpr const char* module_name = "winrt.windows.media.editing";
        static constexpr const char* type_name = "MediaOverlayLayer";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Graphics.DirectX.Direct3D11.h")
#include "py.Windows.Graphics.DirectX.Direct3D11.h"
#endif

#if __has_include("py.Windows.Graphics.Imaging.h")
#include "py.Windows.Graphics.Imaging.h"
#endif

#if __has_include("py.Windows.Media.Core.h")
#include "py.Windows.Media.Core.h"
#endif

#if __has_include("py.Windows.Media.Effects.h")
#include "py.Windows.Media.Effects.h"
#endif

#if __has_include("py.Windows.Media.MediaProperties.h")
#include "py.Windows.Media.MediaProperties.h"
#endif

#if __has_include("py.Windows.Media.Transcoding.h")
#include "py.Windows.Media.Transcoding.h"
#endif

#if __has_include("py.Windows.Storage.h")
#include "py.Windows.Storage.h"
#endif

#if __has_include("py.Windows.UI.h")
#include "py.Windows.UI.h"
#endif

namespace py::impl::Windows::Media::Editing
{
}

namespace py::wrapper::Windows::Media::Editing
{
    using BackgroundAudioTrack = py::winrt_wrapper<winrt::Windows::Media::Editing::BackgroundAudioTrack>;
    using EmbeddedAudioTrack = py::winrt_wrapper<winrt::Windows::Media::Editing::EmbeddedAudioTrack>;
    using MediaClip = py::winrt_wrapper<winrt::Windows::Media::Editing::MediaClip>;
    using MediaComposition = py::winrt_wrapper<winrt::Windows::Media::Editing::MediaComposition>;
    using MediaOverlay = py::winrt_wrapper<winrt::Windows::Media::Editing::MediaOverlay>;
    using MediaOverlayLayer = py::winrt_wrapper<winrt::Windows::Media::Editing::MediaOverlayLayer>;
}

namespace py
{
}
