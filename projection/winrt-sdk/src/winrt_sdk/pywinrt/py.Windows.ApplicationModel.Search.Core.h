// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.ApplicationModel.Search.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Windows.ApplicationModel.Search.Core.h>

namespace py::proj::Windows::ApplicationModel::Search::Core
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::ApplicationModel::Search::Core::SearchSuggestionKind> = "i";


    template<>
    struct py_type<winrt::Windows::ApplicationModel::Search::Core::SearchSuggestionKind>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.applicationmodel.search.core.SearchSuggestionKind";
        static constexpr const char* module_name = "winrt.windows.applicationmodel.search.core";
        static constexpr const char* type_name = "SearchSuggestionKind";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Search::Core::RequestingFocusOnKeyboardInputEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.applicationmodel.search.core.RequestingFocusOnKeyboardInputEventArgs";
        static constexpr const char* module_name = "winrt.windows.applicationmodel.search.core";
        static constexpr const char* type_name = "RequestingFocusOnKeyboardInputEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Search::Core::SearchSuggestion>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.applicationmodel.search.core.SearchSuggestion";
        static constexpr const char* module_name = "winrt.windows.applicationmodel.search.core";
        static constexpr const char* type_name = "SearchSuggestion";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Search::Core::SearchSuggestionManager>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.applicationmodel.search.core.SearchSuggestionManager";
        static constexpr const char* module_name = "winrt.windows.applicationmodel.search.core";
        static constexpr const char* type_name = "SearchSuggestionManager";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Search::Core::SearchSuggestionsRequestedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.applicationmodel.search.core.SearchSuggestionsRequestedEventArgs";
        static constexpr const char* module_name = "winrt.windows.applicationmodel.search.core";
        static constexpr const char* type_name = "SearchSuggestionsRequestedEventArgs";
    };
}

#if __has_include("py.Windows.ApplicationModel.Search.h")
#include "py.Windows.ApplicationModel.Search.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

namespace py::impl::Windows::ApplicationModel::Search::Core
{
}

namespace py::wrapper::Windows::ApplicationModel::Search::Core
{
    using RequestingFocusOnKeyboardInputEventArgs = py::winrt_wrapper<winrt::Windows::ApplicationModel::Search::Core::RequestingFocusOnKeyboardInputEventArgs>;
    using SearchSuggestion = py::winrt_wrapper<winrt::Windows::ApplicationModel::Search::Core::SearchSuggestion>;
    using SearchSuggestionManager = py::winrt_wrapper<winrt::Windows::ApplicationModel::Search::Core::SearchSuggestionManager>;
    using SearchSuggestionsRequestedEventArgs = py::winrt_wrapper<winrt::Windows::ApplicationModel::Search::Core::SearchSuggestionsRequestedEventArgs>;
}

namespace py
{
}
