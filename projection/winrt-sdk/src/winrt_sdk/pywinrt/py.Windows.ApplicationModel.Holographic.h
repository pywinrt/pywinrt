// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Foundation.Numerics.h")
#include "py.Windows.Foundation.Numerics.h"
#endif

#if __has_include("py.Windows.Perception.Spatial.h")
#include "py.Windows.Perception.Spatial.h"
#endif

#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.Perception.Spatial.h>

#include <winrt/Windows.ApplicationModel.Holographic.h>

namespace py::proj::Windows::ApplicationModel::Holographic
{}

namespace py::impl::Windows::ApplicationModel::Holographic
{}

namespace py::wrapper::Windows::ApplicationModel::Holographic
{
    using HolographicKeyboard = py::winrt_wrapper<winrt::Windows::ApplicationModel::Holographic::HolographicKeyboard>;
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Holographic::HolographicKeyboard>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.holographic";
        static constexpr const char* type_name = "HolographicKeyboard";
    };
}