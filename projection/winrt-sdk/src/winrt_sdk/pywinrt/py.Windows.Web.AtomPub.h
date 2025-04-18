// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Security.Credentials.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Web.Syndication.h>

#include <winrt/Windows.Web.AtomPub.h>

namespace py::proj::Windows::Web::AtomPub
{
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::Web::AtomPub::AtomPubClient>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.web.atompub.AtomPubClient";
        static constexpr const char* module_name = "winrt.windows.web.atompub";
        static constexpr const char* type_name = "AtomPubClient";
    };

    template<>
    struct py_type<winrt::Windows::Web::AtomPub::ResourceCollection>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.web.atompub.ResourceCollection";
        static constexpr const char* module_name = "winrt.windows.web.atompub";
        static constexpr const char* type_name = "ResourceCollection";
    };

    template<>
    struct py_type<winrt::Windows::Web::AtomPub::ServiceDocument>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.web.atompub.ServiceDocument";
        static constexpr const char* module_name = "winrt.windows.web.atompub";
        static constexpr const char* type_name = "ServiceDocument";
    };

    template<>
    struct py_type<winrt::Windows::Web::AtomPub::Workspace>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.web.atompub.Workspace";
        static constexpr const char* module_name = "winrt.windows.web.atompub";
        static constexpr const char* type_name = "Workspace";
    };
}

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

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

#if __has_include("py.Windows.Web.Syndication.h")
#include "py.Windows.Web.Syndication.h"
#endif

namespace py::impl::Windows::Web::AtomPub
{
}

namespace py::wrapper::Windows::Web::AtomPub
{
    using AtomPubClient = py::winrt_wrapper<winrt::Windows::Web::AtomPub::AtomPubClient>;
    using ResourceCollection = py::winrt_wrapper<winrt::Windows::Web::AtomPub::ResourceCollection>;
    using ServiceDocument = py::winrt_wrapper<winrt::Windows::Web::AtomPub::ServiceDocument>;
    using Workspace = py::winrt_wrapper<winrt::Windows::Web::AtomPub::Workspace>;
}

namespace py
{
}
