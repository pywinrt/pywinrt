// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.UI.WindowManagement.h>

#include <winrt/Windows.UI.Input.Preview.h>

namespace py::proj::Windows::UI::Input::Preview
{
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::UI::Input::Preview::InputActivationListenerPreview>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.input.preview.InputActivationListenerPreview";
        static constexpr const char* module_name = "winrt.windows.ui.input.preview";
        static constexpr const char* type_name = "InputActivationListenerPreview";
    };
}

#if __has_include("py.Windows.UI.Input.h")
#include "py.Windows.UI.Input.h"
#endif

#if __has_include("py.Windows.UI.WindowManagement.h")
#include "py.Windows.UI.WindowManagement.h"
#endif

namespace py::impl::Windows::UI::Input::Preview
{
}

namespace py::wrapper::Windows::UI::Input::Preview
{
    using InputActivationListenerPreview = py::winrt_wrapper<winrt::Windows::UI::Input::Preview::InputActivationListenerPreview>;
}

namespace py
{
}
