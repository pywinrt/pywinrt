// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Devices.Lights.h")
#include "py.Windows.Devices.Lights.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Graphics.Imaging.h")
#include "py.Windows.Graphics.Imaging.h"
#endif

#if __has_include("py.Windows.UI.h")
#include "py.Windows.UI.h"
#endif

#include <winrt/Windows.Devices.Lights.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.UI.h>

#include <winrt/Windows.Devices.Lights.Effects.h>

namespace py::proj::Windows::Devices::Lights::Effects
{}

namespace py::impl::Windows::Devices::Lights::Effects
{}

namespace py::wrapper::Windows::Devices::Lights::Effects
{
    using LampArrayBitmapEffect = py::winrt_wrapper<winrt::Windows::Devices::Lights::Effects::LampArrayBitmapEffect>;
    using LampArrayBitmapRequestedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Lights::Effects::LampArrayBitmapRequestedEventArgs>;
    using LampArrayBlinkEffect = py::winrt_wrapper<winrt::Windows::Devices::Lights::Effects::LampArrayBlinkEffect>;
    using LampArrayColorRampEffect = py::winrt_wrapper<winrt::Windows::Devices::Lights::Effects::LampArrayColorRampEffect>;
    using LampArrayCustomEffect = py::winrt_wrapper<winrt::Windows::Devices::Lights::Effects::LampArrayCustomEffect>;
    using LampArrayEffectPlaylist = py::winrt_wrapper<winrt::Windows::Devices::Lights::Effects::LampArrayEffectPlaylist>;
    using LampArraySolidEffect = py::winrt_wrapper<winrt::Windows::Devices::Lights::Effects::LampArraySolidEffect>;
    using LampArrayUpdateRequestedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Lights::Effects::LampArrayUpdateRequestedEventArgs>;
    using ILampArrayEffect = py::winrt_wrapper<winrt::Windows::Devices::Lights::Effects::ILampArrayEffect>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Lights::Effects::LampArrayEffectCompletionBehavior> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Lights::Effects::LampArrayEffectStartMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Lights::Effects::LampArrayRepetitionMode> = "i";


    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArrayEffectCompletionBehavior>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArrayEffectCompletionBehavior";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArrayEffectStartMode>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArrayEffectStartMode";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArrayRepetitionMode>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArrayRepetitionMode";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArrayBitmapEffect>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArrayBitmapEffect";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArrayBitmapRequestedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArrayBitmapRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArrayBlinkEffect>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArrayBlinkEffect";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArrayColorRampEffect>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArrayColorRampEffect";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArrayCustomEffect>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArrayCustomEffect";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArrayEffectPlaylist>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArrayEffectPlaylist";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArraySolidEffect>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArraySolidEffect";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::LampArrayUpdateRequestedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "LampArrayUpdateRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Lights::Effects::ILampArrayEffect>
    {
        static constexpr const char* module_name = "winrt.windows.devices.lights.effects";
        static constexpr const char* type_name = "ILampArrayEffect";
    };
}