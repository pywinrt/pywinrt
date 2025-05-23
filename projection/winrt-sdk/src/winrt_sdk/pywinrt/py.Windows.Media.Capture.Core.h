// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Media.Capture.h>

#include <winrt/Windows.Media.Capture.Core.h>

namespace py::proj::Windows::Media::Capture::Core
{
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::Media::Capture::Core::VariablePhotoCapturedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.capture.core.VariablePhotoCapturedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.capture.core";
        static constexpr const char* type_name = "VariablePhotoCapturedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Capture::Core::VariablePhotoSequenceCapture>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.capture.core.VariablePhotoSequenceCapture";
        static constexpr const char* module_name = "winrt.windows.media.capture.core";
        static constexpr const char* type_name = "VariablePhotoSequenceCapture";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Media.Capture.h")
#include "py.Windows.Media.Capture.h"
#endif

namespace py::impl::Windows::Media::Capture::Core
{
}

namespace py::wrapper::Windows::Media::Capture::Core
{
    using VariablePhotoCapturedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Capture::Core::VariablePhotoCapturedEventArgs>;
    using VariablePhotoSequenceCapture = py::winrt_wrapper<winrt::Windows::Media::Capture::Core::VariablePhotoSequenceCapture>;
}

namespace py
{
}
