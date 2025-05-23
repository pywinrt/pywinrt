// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h>
#include <winrt/Windows.Devices.Bluetooth.Rfcomm.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Devices.Radios.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Networking.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Windows.Devices.Bluetooth.h>

namespace py::proj::Windows::Devices::Bluetooth
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Bluetooth::BluetoothAddressType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Bluetooth::BluetoothCacheMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Bluetooth::BluetoothConnectionStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Bluetooth::BluetoothError> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Bluetooth::BluetoothLEPreferredConnectionParametersRequestStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Bluetooth::BluetoothMajorClass> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Bluetooth::BluetoothMinorClass> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Bluetooth::BluetoothServiceCapabilities> = "I";


    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothAddressType>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothAddressType";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothAddressType";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothCacheMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothCacheMode";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothCacheMode";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothConnectionStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothConnectionStatus";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothConnectionStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothError>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothError";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothError";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothLEPreferredConnectionParametersRequestStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothLEPreferredConnectionParametersRequestStatus";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothLEPreferredConnectionParametersRequestStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothMajorClass>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothMajorClass";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothMajorClass";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothMinorClass>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothMinorClass";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothMinorClass";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothServiceCapabilities>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothServiceCapabilities";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothServiceCapabilities";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothAdapter>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothAdapter";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothAdapter";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothClassOfDevice>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothClassOfDevice";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothClassOfDevice";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothDevice>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothDevice";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothDevice";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothDeviceId>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothDeviceId";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothDeviceId";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothLEAppearance>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothLEAppearance";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothLEAppearance";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothLEAppearanceCategories>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothLEAppearanceCategories";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothLEAppearanceCategories";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothLEAppearanceSubcategories>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothLEAppearanceSubcategories";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothLEAppearanceSubcategories";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothLEConnectionParameters>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothLEConnectionParameters";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothLEConnectionParameters";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothLEConnectionPhy>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothLEConnectionPhy";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothLEConnectionPhy";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothLEConnectionPhyInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothLEConnectionPhyInfo";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothLEConnectionPhyInfo";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothLEDevice>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothLEDevice";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothLEDevice";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothLEPreferredConnectionParameters>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothLEPreferredConnectionParameters";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothLEPreferredConnectionParameters";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothLEPreferredConnectionParametersRequest>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothLEPreferredConnectionParametersRequest";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothLEPreferredConnectionParametersRequest";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothSignalStrengthFilter>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothSignalStrengthFilter";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothSignalStrengthFilter";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Bluetooth::BluetoothUuidHelper>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.bluetooth.BluetoothUuidHelper";
        static constexpr const char* module_name = "winrt.windows.devices.bluetooth";
        static constexpr const char* type_name = "BluetoothUuidHelper";
    };
}

#if __has_include("py.Windows.Devices.Bluetooth.GenericAttributeProfile.h")
#include "py.Windows.Devices.Bluetooth.GenericAttributeProfile.h"
#endif

#if __has_include("py.Windows.Devices.Bluetooth.Rfcomm.h")
#include "py.Windows.Devices.Bluetooth.Rfcomm.h"
#endif

#if __has_include("py.Windows.Devices.Enumeration.h")
#include "py.Windows.Devices.Enumeration.h"
#endif

#if __has_include("py.Windows.Devices.Radios.h")
#include "py.Windows.Devices.Radios.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Networking.h")
#include "py.Windows.Networking.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

namespace py::impl::Windows::Devices::Bluetooth
{
}

namespace py::wrapper::Windows::Devices::Bluetooth
{
    using BluetoothAdapter = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothAdapter>;
    using BluetoothClassOfDevice = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothClassOfDevice>;
    using BluetoothDevice = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothDevice>;
    using BluetoothDeviceId = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothDeviceId>;
    using BluetoothLEAppearance = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothLEAppearance>;
    using BluetoothLEAppearanceCategories = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothLEAppearanceCategories>;
    using BluetoothLEAppearanceSubcategories = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothLEAppearanceSubcategories>;
    using BluetoothLEConnectionParameters = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothLEConnectionParameters>;
    using BluetoothLEConnectionPhy = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothLEConnectionPhy>;
    using BluetoothLEConnectionPhyInfo = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothLEConnectionPhyInfo>;
    using BluetoothLEDevice = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothLEDevice>;
    using BluetoothLEPreferredConnectionParameters = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothLEPreferredConnectionParameters>;
    using BluetoothLEPreferredConnectionParametersRequest = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothLEPreferredConnectionParametersRequest>;
    using BluetoothSignalStrengthFilter = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothSignalStrengthFilter>;
    using BluetoothUuidHelper = py::winrt_wrapper<winrt::Windows::Devices::Bluetooth::BluetoothUuidHelper>;
}

namespace py
{
}
