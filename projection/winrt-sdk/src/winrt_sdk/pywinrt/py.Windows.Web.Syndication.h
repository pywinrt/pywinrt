// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Data.Xml.Dom.h")
#include "py.Windows.Data.Xml.Dom.h"
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

#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Security.Credentials.h>

#include <winrt/Windows.Web.Syndication.h>

namespace py::proj::Windows::Web::Syndication
{}

namespace py::impl::Windows::Web::Syndication
{}

namespace py::wrapper::Windows::Web::Syndication
{
    using SyndicationAttribute = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationAttribute>;
    using SyndicationCategory = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationCategory>;
    using SyndicationClient = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationClient>;
    using SyndicationContent = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationContent>;
    using SyndicationError = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationError>;
    using SyndicationFeed = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationFeed>;
    using SyndicationGenerator = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationGenerator>;
    using SyndicationItem = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationItem>;
    using SyndicationLink = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationLink>;
    using SyndicationNode = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationNode>;
    using SyndicationPerson = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationPerson>;
    using SyndicationText = py::winrt_wrapper<winrt::Windows::Web::Syndication::SyndicationText>;
    using ISyndicationClient = py::winrt_wrapper<winrt::Windows::Web::Syndication::ISyndicationClient>;
    using ISyndicationNode = py::winrt_wrapper<winrt::Windows::Web::Syndication::ISyndicationNode>;
    using ISyndicationText = py::winrt_wrapper<winrt::Windows::Web::Syndication::ISyndicationText>;
    using RetrievalProgress = py::winrt_struct_wrapper<winrt::Windows::Web::Syndication::RetrievalProgress>;
    using TransferProgress = py::winrt_struct_wrapper<winrt::Windows::Web::Syndication::TransferProgress>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Web::Syndication::SyndicationErrorStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Web::Syndication::SyndicationFormat> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Web::Syndication::SyndicationTextType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Web::Syndication::RetrievalProgress> = "T{I:bytes_retrieved:I:total_bytes_to_retrieve:}";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Web::Syndication::TransferProgress> = "T{I:bytes_sent:I:total_bytes_to_send:I:bytes_retrieved:I:total_bytes_to_retrieve:}";


    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationErrorStatus>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationErrorStatus";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationFormat>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationFormat";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationTextType>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationTextType";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationAttribute>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationAttribute";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationCategory>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationCategory";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationClient>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationClient";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationContent>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationContent";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationError>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationError";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationFeed>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationFeed";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationGenerator>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationGenerator";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationItem>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationItem";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationLink>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationLink";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationNode>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationNode";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationPerson>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationPerson";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::SyndicationText>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "SyndicationText";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::ISyndicationClient>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "ISyndicationClient";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::ISyndicationNode>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "ISyndicationNode";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::ISyndicationText>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "ISyndicationText";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::RetrievalProgress>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "RetrievalProgress";
    };

    template<>
    struct py_type<winrt::Windows::Web::Syndication::TransferProgress>
    {
        static constexpr const char* module_name = "winrt.windows.web.syndication";
        static constexpr const char* type_name = "TransferProgress";
    };
}