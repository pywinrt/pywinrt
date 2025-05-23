// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Devices.Bluetooth.h>
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>
#include <winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h>
#include <winrt/Windows.Devices.Bluetooth.Rfcomm.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Networking.Sockets.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Windows.Devices.Bluetooth.Background.h>

namespace py::proj::Windows::Devices::Bluetooth::Background
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Bluetooth::Background::BluetoothEventTriggeringMode> = "i";


    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::Background::BluetoothEventTriggeringMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.background.BluetoothEventTriggeringMode";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth.background";
        static constexpr const char* type_name = "BluetoothEventTriggeringMode";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::Background::BluetoothLEAdvertisementPublisherTriggerDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.background.BluetoothLEAdvertisementPublisherTriggerDetails";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth.background";
        static constexpr const char* type_name = "BluetoothLEAdvertisementPublisherTriggerDetails";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::Background::BluetoothLEAdvertisementWatcherTriggerDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.background.BluetoothLEAdvertisementWatcherTriggerDetails";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth.background";
        static constexpr const char* type_name = "BluetoothLEAdvertisementWatcherTriggerDetails";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::Background::GattCharacteristicNotificationTriggerDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.background.GattCharacteristicNotificationTriggerDetails";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth.background";
        static constexpr const char* type_name = "GattCharacteristicNotificationTriggerDetails";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::Background::GattServiceProviderConnection>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.background.GattServiceProviderConnection";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth.background";
        static constexpr const char* type_name = "GattServiceProviderConnection";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::Background::GattServiceProviderTriggerDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.background.GattServiceProviderTriggerDetails";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth.background";
        static constexpr const char* type_name = "GattServiceProviderTriggerDetails";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::Background::RfcommConnectionTriggerDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.background.RfcommConnectionTriggerDetails";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth.background";
        static constexpr const char* type_name = "RfcommConnectionTriggerDetails";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::Background::RfcommInboundConnectionInformation>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.background.RfcommInboundConnectionInformation";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth.background";
        static constexpr const char* type_name = "RfcommInboundConnectionInformation";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::Background::RfcommOutboundConnectionInformation>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.background.RfcommOutboundConnectionInformation";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth.background";
        static constexpr const char* type_name = "RfcommOutboundConnectionInformation";
    };
}

#if __has_include("py.Windows.Devices.Bluetooth.h")
#include "py.Windows.Devices.Bluetooth.h"
#endif

#if __has_include("py.Windows.Devices.Bluetooth.Advertisement.h")
#include "py.Windows.Devices.Bluetooth.Advertisement.h"
#endif

#if __has_include("py.Windows.Devices.Bluetooth.GenericAttributeProfile.h")
#include "py.Windows.Devices.Bluetooth.GenericAttributeProfile.h"
#endif

#if __has_include("py.Windows.Devices.Bluetooth.Rfcomm.h")
#include "py.Windows.Devices.Bluetooth.Rfcomm.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Networking.Sockets.h")
#include "py.Windows.Networking.Sockets.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

namespace py::impl::Windows::Devices::Bluetooth::Background
{
}

namespace py::wrapper::Windows::Devices::Bluetooth::Background
{
    using BluetoothLEAdvertisementPublisherTriggerDetails = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::Background::BluetoothLEAdvertisementPublisherTriggerDetails>;
    using BluetoothLEAdvertisementWatcherTriggerDetails = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::Background::BluetoothLEAdvertisementWatcherTriggerDetails>;
    using GattCharacteristicNotificationTriggerDetails = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::Background::GattCharacteristicNotificationTriggerDetails>;
    using GattServiceProviderConnection = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::Background::GattServiceProviderConnection>;
    using GattServiceProviderTriggerDetails = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::Background::GattServiceProviderTriggerDetails>;
    using RfcommConnectionTriggerDetails = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::Background::RfcommConnectionTriggerDetails>;
    using RfcommInboundConnectionInformation = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::Background::RfcommInboundConnectionInformation>;
    using RfcommOutboundConnectionInformation = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::Background::RfcommOutboundConnectionInformation>;
}

namespace py
{
}
