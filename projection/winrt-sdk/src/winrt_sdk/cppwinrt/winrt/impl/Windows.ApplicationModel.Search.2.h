// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_ApplicationModel_Search_2_H
#define WINRT_Windows_ApplicationModel_Search_2_H
#include "winrt/impl/Windows.Foundation.Collections.1.h"
#include "winrt/impl/Windows.ApplicationModel.Search.1.h"
WINRT_EXPORT namespace winrt::Windows::ApplicationModel::Search
{
    struct WINRT_IMPL_EMPTY_BASES LocalContentSuggestionSettings : winrt::Windows::ApplicationModel::Search::ILocalContentSuggestionSettings
    {
        LocalContentSuggestionSettings(std::nullptr_t) noexcept {}
        LocalContentSuggestionSettings(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ILocalContentSuggestionSettings(ptr, take_ownership_from_abi) {}
        LocalContentSuggestionSettings();
    };
    struct WINRT_IMPL_EMPTY_BASES SearchPane : winrt::Windows::ApplicationModel::Search::ISearchPane
    {
        SearchPane(std::nullptr_t) noexcept {}
        SearchPane(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchPane(ptr, take_ownership_from_abi) {}
        static auto GetForCurrentView();
        static auto HideThisApplication();
    };
    struct WINRT_IMPL_EMPTY_BASES SearchPaneQueryChangedEventArgs : winrt::Windows::ApplicationModel::Search::ISearchPaneQueryChangedEventArgs
    {
        SearchPaneQueryChangedEventArgs(std::nullptr_t) noexcept {}
        SearchPaneQueryChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchPaneQueryChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES SearchPaneQueryLinguisticDetails : winrt::Windows::ApplicationModel::Search::ISearchPaneQueryLinguisticDetails
    {
        SearchPaneQueryLinguisticDetails(std::nullptr_t) noexcept {}
        SearchPaneQueryLinguisticDetails(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchPaneQueryLinguisticDetails(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES SearchPaneQuerySubmittedEventArgs : winrt::Windows::ApplicationModel::Search::ISearchPaneQuerySubmittedEventArgs,
        impl::require<SearchPaneQuerySubmittedEventArgs, winrt::Windows::ApplicationModel::Search::ISearchPaneQuerySubmittedEventArgsWithLinguisticDetails>
    {
        SearchPaneQuerySubmittedEventArgs(std::nullptr_t) noexcept {}
        SearchPaneQuerySubmittedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchPaneQuerySubmittedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES SearchPaneResultSuggestionChosenEventArgs : winrt::Windows::ApplicationModel::Search::ISearchPaneResultSuggestionChosenEventArgs
    {
        SearchPaneResultSuggestionChosenEventArgs(std::nullptr_t) noexcept {}
        SearchPaneResultSuggestionChosenEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchPaneResultSuggestionChosenEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES SearchPaneSuggestionsRequest : winrt::Windows::ApplicationModel::Search::ISearchPaneSuggestionsRequest
    {
        SearchPaneSuggestionsRequest(std::nullptr_t) noexcept {}
        SearchPaneSuggestionsRequest(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchPaneSuggestionsRequest(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES SearchPaneSuggestionsRequestDeferral : winrt::Windows::ApplicationModel::Search::ISearchPaneSuggestionsRequestDeferral
    {
        SearchPaneSuggestionsRequestDeferral(std::nullptr_t) noexcept {}
        SearchPaneSuggestionsRequestDeferral(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchPaneSuggestionsRequestDeferral(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES SearchPaneSuggestionsRequestedEventArgs : winrt::Windows::ApplicationModel::Search::ISearchPaneSuggestionsRequestedEventArgs
    {
        SearchPaneSuggestionsRequestedEventArgs(std::nullptr_t) noexcept {}
        SearchPaneSuggestionsRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchPaneSuggestionsRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES SearchPaneVisibilityChangedEventArgs : winrt::Windows::ApplicationModel::Search::ISearchPaneVisibilityChangedEventArgs
    {
        SearchPaneVisibilityChangedEventArgs(std::nullptr_t) noexcept {}
        SearchPaneVisibilityChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchPaneVisibilityChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES SearchQueryLinguisticDetails : winrt::Windows::ApplicationModel::Search::ISearchQueryLinguisticDetails
    {
        SearchQueryLinguisticDetails(std::nullptr_t) noexcept {}
        SearchQueryLinguisticDetails(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchQueryLinguisticDetails(ptr, take_ownership_from_abi) {}
        SearchQueryLinguisticDetails(param::iterable<hstring> const& queryTextAlternatives, uint32_t queryTextCompositionStart, uint32_t queryTextCompositionLength);
    };
    struct WINRT_IMPL_EMPTY_BASES SearchSuggestionCollection : winrt::Windows::ApplicationModel::Search::ISearchSuggestionCollection
    {
        SearchSuggestionCollection(std::nullptr_t) noexcept {}
        SearchSuggestionCollection(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchSuggestionCollection(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES SearchSuggestionsRequest : winrt::Windows::ApplicationModel::Search::ISearchSuggestionsRequest
    {
        SearchSuggestionsRequest(std::nullptr_t) noexcept {}
        SearchSuggestionsRequest(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchSuggestionsRequest(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES SearchSuggestionsRequestDeferral : winrt::Windows::ApplicationModel::Search::ISearchSuggestionsRequestDeferral
    {
        SearchSuggestionsRequestDeferral(std::nullptr_t) noexcept {}
        SearchSuggestionsRequestDeferral(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Search::ISearchSuggestionsRequestDeferral(ptr, take_ownership_from_abi) {}
    };
}
#endif
