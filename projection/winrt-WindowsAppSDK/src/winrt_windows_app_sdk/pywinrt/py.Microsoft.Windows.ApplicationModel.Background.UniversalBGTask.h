// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.ApplicationModel.Background.h>

#include <winrt/Microsoft.Windows.ApplicationModel.Background.UniversalBGTask.h>

namespace py::proj::Microsoft::Windows::ApplicationModel::Background::UniversalBGTask
{
}

namespace py
{

    template<>
    struct py_type<winrt::Microsoft::Windows::ApplicationModel::Background::UniversalBGTask::Task>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.applicationmodel.background.universalbgtask.Task";
        static constexpr const char* module_name = "winui3.microsoft.windows.applicationmodel.background.universalbgtask";
        static constexpr const char* type_name = "Task";
    };
}

#if __has_include("py.Windows.ApplicationModel.Background.h")
#include "py.Windows.ApplicationModel.Background.h"
#endif

namespace py::impl::Microsoft::Windows::ApplicationModel::Background::UniversalBGTask
{
}

namespace py::wrapper::Microsoft::Windows::ApplicationModel::Background::UniversalBGTask
{
    using Task = py::winrt_wrapper<winrt::Microsoft::Windows::ApplicationModel::Background::UniversalBGTask::Task>;
}

namespace py
{
}
