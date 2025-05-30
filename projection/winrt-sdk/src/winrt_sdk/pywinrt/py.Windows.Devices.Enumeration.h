// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.ApplicationModel.Background.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Security.Credentials.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Popups.h>

#include <winrt/Windows.Devices.Enumeration.h>

namespace py::proj::Windows::Devices::Enumeration
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DeviceAccessStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DeviceClass> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DeviceInformationKind> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DevicePairingAddPairingSetMemberStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DevicePairingKinds> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DevicePairingProtectionLevel> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DevicePairingResultStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DevicePickerDisplayStatusOptions> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DeviceUnpairingResultStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DeviceWatcherEventKind> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::DeviceWatcherStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Enumeration::Panel> = "i";


    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceAccessStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceAccessStatus";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceAccessStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceClass>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceClass";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceClass";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceInformationKind>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceInformationKind";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceInformationKind";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePairingAddPairingSetMemberStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePairingAddPairingSetMemberStatus";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePairingAddPairingSetMemberStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePairingKinds>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePairingKinds";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePairingKinds";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePairingProtectionLevel>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePairingProtectionLevel";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePairingProtectionLevel";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePairingResultStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePairingResultStatus";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePairingResultStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePickerDisplayStatusOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePickerDisplayStatusOptions";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePickerDisplayStatusOptions";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceUnpairingResultStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceUnpairingResultStatus";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceUnpairingResultStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceWatcherEventKind>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceWatcherEventKind";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceWatcherEventKind";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceWatcherStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceWatcherStatus";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceWatcherStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::Panel>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.Panel";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "Panel";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceAccessChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceAccessChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceAccessChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceAccessInformation>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceAccessInformation";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceAccessInformation";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceConnectionChangeTriggerDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceConnectionChangeTriggerDetails";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceConnectionChangeTriggerDetails";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceDisconnectButtonClickedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceDisconnectButtonClickedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceDisconnectButtonClickedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceInformation>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceInformation";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceInformation";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceInformationCollection>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceInformationCollection";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceInformationCollection";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceInformationCustomPairing>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceInformationCustomPairing";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceInformationCustomPairing";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceInformationPairing>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceInformationPairing";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceInformationPairing";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceInformationUpdate>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceInformationUpdate";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceInformationUpdate";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePairingRequestedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePairingRequestedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePairingRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePairingResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePairingResult";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePairingResult";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePairingSetMembersRequestedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePairingSetMembersRequestedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePairingSetMembersRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePicker>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePicker";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePicker";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePickerAppearance>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePickerAppearance";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePickerAppearance";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DevicePickerFilter>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DevicePickerFilter";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DevicePickerFilter";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceSelectedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceSelectedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceSelectedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceThumbnail>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceThumbnail";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceThumbnail";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceUnpairingResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceUnpairingResult";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceUnpairingResult";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceWatcher>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceWatcher";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceWatcher";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceWatcherEvent>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceWatcherEvent";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceWatcherEvent";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::DeviceWatcherTriggerDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.DeviceWatcherTriggerDetails";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "DeviceWatcherTriggerDetails";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::EnclosureLocation>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration.EnclosureLocation";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "EnclosureLocation";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::IDeviceEnumerationSettings>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration._IDeviceEnumerationSettings";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "_IDeviceEnumerationSettings";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Enumeration::IDevicePairingSettings>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.enumeration._IDevicePairingSettings";
        static constexpr const char* module_name = "winrt.windows.devices.enumeration";
        static constexpr const char* type_name = "_IDevicePairingSettings";
    };
}

#if __has_include("py.Windows.ApplicationModel.Background.h")
#include "py.Windows.ApplicationModel.Background.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Security.Credentials.h")
#include "py.Windows.Security.Credentials.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

#if __has_include("py.Windows.UI.h")
#include "py.Windows.UI.h"
#endif

#if __has_include("py.Windows.UI.Popups.h")
#include "py.Windows.UI.Popups.h"
#endif

namespace py::impl::Windows::Devices::Enumeration
{
}

namespace py::wrapper::Windows::Devices::Enumeration
{
    using DeviceAccessChangedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceAccessChangedEventArgs>;
    using DeviceAccessInformation = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceAccessInformation>;
    using DeviceConnectionChangeTriggerDetails = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceConnectionChangeTriggerDetails>;
    using DeviceDisconnectButtonClickedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceDisconnectButtonClickedEventArgs>;
    using DeviceInformation = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceInformation>;
    using DeviceInformationCollection = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceInformationCollection>;
    using DeviceInformationCustomPairing = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceInformationCustomPairing>;
    using DeviceInformationPairing = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceInformationPairing>;
    using DeviceInformationUpdate = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceInformationUpdate>;
    using DevicePairingRequestedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DevicePairingRequestedEventArgs>;
    using DevicePairingResult = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DevicePairingResult>;
    using DevicePairingSetMembersRequestedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DevicePairingSetMembersRequestedEventArgs>;
    using DevicePicker = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DevicePicker>;
    using DevicePickerAppearance = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DevicePickerAppearance>;
    using DevicePickerFilter = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DevicePickerFilter>;
    using DeviceSelectedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceSelectedEventArgs>;
    using DeviceThumbnail = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceThumbnail>;
    using DeviceUnpairingResult = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceUnpairingResult>;
    using DeviceWatcher = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceWatcher>;
    using DeviceWatcherEvent = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceWatcherEvent>;
    using DeviceWatcherTriggerDetails = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::DeviceWatcherTriggerDetails>;
    using EnclosureLocation = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::EnclosureLocation>;
    using IDeviceEnumerationSettings = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::IDeviceEnumerationSettings>;
    using IDevicePairingSettings = py::winrt_wrapper<winrt::Windows::Devices::Enumeration::IDevicePairingSettings>;
}

namespace py
{
}
