// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Storage.h")
#include "py.Windows.Storage.h"
#endif

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>

#include <winrt/Windows.ApplicationModel.Calls.Provider.h>

namespace py::proj::Windows::ApplicationModel::Calls::Provider
{}

namespace py::impl::Windows::ApplicationModel::Calls::Provider
{}

namespace py::wrapper::Windows::ApplicationModel::Calls::Provider
{
    using PhoneCallOrigin = py::winrt_wrapper<winrt::Windows::ApplicationModel::Calls::Provider::PhoneCallOrigin>;
    using PhoneCallOriginManager = py::winrt_wrapper<winrt::Windows::ApplicationModel::Calls::Provider::PhoneCallOriginManager>;
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Calls::Provider::PhoneCallOrigin>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.calls.provider";
        static constexpr const char* type_name = "PhoneCallOrigin";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Calls::Provider::PhoneCallOriginManager>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.calls.provider";
        static constexpr const char* type_name = "PhoneCallOriginManager";
    };
}