// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"


#include <winrt/Windows.Foundation.Metadata.h>

namespace py::proj::Windows::Foundation::Metadata
{}

namespace py::impl::Windows::Foundation::Metadata
{}

namespace py::wrapper::Windows::Foundation::Metadata
{
    using ApiInformation = py::winrt_wrapper<winrt::Windows::Foundation::Metadata::ApiInformation>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Metadata::AttributeTargets> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Metadata::CompositionType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Metadata::DeprecationType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Metadata::FeatureStage> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Metadata::GCPressureAmount> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Metadata::MarshalingType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Metadata::Platform> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Metadata::ThreadingModel> = "i";


    template<>
    struct py_type<winrt::Windows::Foundation::Metadata::AttributeTargets>
    {
        static constexpr const char* module_name = "winrt.windows.foundation.metadata";
        static constexpr const char* type_name = "AttributeTargets";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Metadata::CompositionType>
    {
        static constexpr const char* module_name = "winrt.windows.foundation.metadata";
        static constexpr const char* type_name = "CompositionType";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Metadata::DeprecationType>
    {
        static constexpr const char* module_name = "winrt.windows.foundation.metadata";
        static constexpr const char* type_name = "DeprecationType";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Metadata::FeatureStage>
    {
        static constexpr const char* module_name = "winrt.windows.foundation.metadata";
        static constexpr const char* type_name = "FeatureStage";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Metadata::GCPressureAmount>
    {
        static constexpr const char* module_name = "winrt.windows.foundation.metadata";
        static constexpr const char* type_name = "GCPressureAmount";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Metadata::MarshalingType>
    {
        static constexpr const char* module_name = "winrt.windows.foundation.metadata";
        static constexpr const char* type_name = "MarshalingType";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Metadata::Platform>
    {
        static constexpr const char* module_name = "winrt.windows.foundation.metadata";
        static constexpr const char* type_name = "Platform";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Metadata::ThreadingModel>
    {
        static constexpr const char* module_name = "winrt.windows.foundation.metadata";
        static constexpr const char* type_name = "ThreadingModel";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Metadata::ApiInformation>
    {
        static constexpr const char* module_name = "winrt.windows.foundation.metadata";
        static constexpr const char* type_name = "ApiInformation";
    };
}