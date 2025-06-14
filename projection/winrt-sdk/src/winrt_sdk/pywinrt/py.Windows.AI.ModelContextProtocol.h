// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.UI.h>

#include <winrt/Windows.AI.ModelContextProtocol.h>

namespace py::proj::Windows::AI::ModelContextProtocol
{
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::AI::ModelContextProtocol::ModelContextProtocolClientContext>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ai.modelcontextprotocol.ModelContextProtocolClientContext";
        static constexpr const char* module_name = "winrt.windows.ai.modelcontextprotocol";
        static constexpr const char* type_name = "ModelContextProtocolClientContext";
    };

    template<>
    struct py_type<winrt::Windows::AI::ModelContextProtocol::ModelContextProtocolServerCatalog>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ai.modelcontextprotocol.ModelContextProtocolServerCatalog";
        static constexpr const char* module_name = "winrt.windows.ai.modelcontextprotocol";
        static constexpr const char* type_name = "ModelContextProtocolServerCatalog";
    };

    template<>
    struct py_type<winrt::Windows::AI::ModelContextProtocol::ModelContextProtocolServerInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ai.modelcontextprotocol.ModelContextProtocolServerInfo";
        static constexpr const char* module_name = "winrt.windows.ai.modelcontextprotocol";
        static constexpr const char* type_name = "ModelContextProtocolServerInfo";
    };

    template<>
    struct py_type<winrt::Windows::AI::ModelContextProtocol::IModelContextProtocolServer>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ai.modelcontextprotocol._IModelContextProtocolServer";
        static constexpr const char* module_name = "winrt.windows.ai.modelcontextprotocol";
        static constexpr const char* type_name = "_IModelContextProtocolServer";
    };
}

#if __has_include("py.Windows.ApplicationModel.h")
#include "py.Windows.ApplicationModel.h"
#endif

#if __has_include("py.Windows.UI.h")
#include "py.Windows.UI.h"
#endif

namespace py::impl::Windows::AI::ModelContextProtocol
{
}

namespace py::wrapper::Windows::AI::ModelContextProtocol
{
    using ModelContextProtocolClientContext = py::winrt_wrapper<winrt::Windows::AI::ModelContextProtocol::ModelContextProtocolClientContext>;
    using ModelContextProtocolServerCatalog = py::winrt_wrapper<winrt::Windows::AI::ModelContextProtocol::ModelContextProtocolServerCatalog>;
    using ModelContextProtocolServerInfo = py::winrt_wrapper<winrt::Windows::AI::ModelContextProtocol::ModelContextProtocolServerInfo>;
    using IModelContextProtocolServer = py::winrt_wrapper<winrt::Windows::AI::ModelContextProtocol::IModelContextProtocolServer>;
}

namespace py
{
}
