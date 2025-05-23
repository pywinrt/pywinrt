// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Networking.Connectivity.h>

#include <winrt/Windows.Networking.h>

namespace py::proj::Windows::Networking
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Networking::DomainNameType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Networking::HostNameSortOptions> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Networking::HostNameType> = "i";


    template<>
    struct py_type<winrt::Windows::Networking::DomainNameType>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.networking.DomainNameType";
        static constexpr const char* module_name = "winrt.windows.networking";
        static constexpr const char* type_name = "DomainNameType";
    };

    template<>
    struct py_type<winrt::Windows::Networking::HostNameSortOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.networking.HostNameSortOptions";
        static constexpr const char* module_name = "winrt.windows.networking";
        static constexpr const char* type_name = "HostNameSortOptions";
    };

    template<>
    struct py_type<winrt::Windows::Networking::HostNameType>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.networking.HostNameType";
        static constexpr const char* module_name = "winrt.windows.networking";
        static constexpr const char* type_name = "HostNameType";
    };

    template<>
    struct py_type<winrt::Windows::Networking::EndpointPair>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.networking.EndpointPair";
        static constexpr const char* module_name = "winrt.windows.networking";
        static constexpr const char* type_name = "EndpointPair";
    };

    template<>
    struct py_type<winrt::Windows::Networking::HostName>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.networking.HostName";
        static constexpr const char* module_name = "winrt.windows.networking";
        static constexpr const char* type_name = "HostName";
    };
}

#if __has_include("py.Windows.Networking.Connectivity.h")
#include "py.Windows.Networking.Connectivity.h"
#endif

namespace py::impl::Windows::Networking
{
}

namespace py::wrapper::Windows::Networking
{
    using EndpointPair = py::winrt_wrapper<winrt::Windows::Networking::EndpointPair>;
    using HostName = py::winrt_wrapper<winrt::Windows::Networking::HostName>;
}

namespace py
{
}
