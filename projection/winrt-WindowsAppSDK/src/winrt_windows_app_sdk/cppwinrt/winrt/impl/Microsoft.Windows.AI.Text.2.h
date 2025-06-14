// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_Windows_AI_Text_2_H
#define WINRT_Microsoft_Windows_AI_Text_2_H
#include "winrt/impl/Windows.Foundation.1.h"
#include "winrt/impl/Microsoft.Windows.AI.Text.1.h"
WINRT_EXPORT namespace winrt::Microsoft::Windows::AI::Text
{
    struct WINRT_IMPL_EMPTY_BASES LanguageModel : winrt::Microsoft::Windows::AI::Text::ILanguageModel,
        impl::require<LanguageModel, winrt::Windows::Foundation::IClosable>
    {
        LanguageModel(std::nullptr_t) noexcept {}
        LanguageModel(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::AI::Text::ILanguageModel(ptr, take_ownership_from_abi) {}
        static auto GetReadyState();
        static auto EnsureReadyAsync();
        static auto CreateAsync();
    };
    struct WINRT_IMPL_EMPTY_BASES LanguageModelContext : winrt::Microsoft::Windows::AI::Text::ILanguageModelContext,
        impl::require<LanguageModelContext, winrt::Windows::Foundation::IClosable>
    {
        LanguageModelContext(std::nullptr_t) noexcept {}
        LanguageModelContext(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::AI::Text::ILanguageModelContext(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES LanguageModelOptions : winrt::Microsoft::Windows::AI::Text::ILanguageModelOptions
    {
        LanguageModelOptions(std::nullptr_t) noexcept {}
        LanguageModelOptions(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::AI::Text::ILanguageModelOptions(ptr, take_ownership_from_abi) {}
        LanguageModelOptions();
    };
    struct WINRT_IMPL_EMPTY_BASES LanguageModelResponseResult : winrt::Microsoft::Windows::AI::Text::ILanguageModelResponseResult
    {
        LanguageModelResponseResult(std::nullptr_t) noexcept {}
        LanguageModelResponseResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::AI::Text::ILanguageModelResponseResult(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES TextRewriter : winrt::Microsoft::Windows::AI::Text::ITextRewriter
    {
        TextRewriter(std::nullptr_t) noexcept {}
        TextRewriter(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::AI::Text::ITextRewriter(ptr, take_ownership_from_abi) {}
        explicit TextRewriter(winrt::Microsoft::Windows::AI::Text::LanguageModel const& languageModel);
    };
    struct WINRT_IMPL_EMPTY_BASES TextSummarizer : winrt::Microsoft::Windows::AI::Text::ITextSummarizer
    {
        TextSummarizer(std::nullptr_t) noexcept {}
        TextSummarizer(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::AI::Text::ITextSummarizer(ptr, take_ownership_from_abi) {}
        explicit TextSummarizer(winrt::Microsoft::Windows::AI::Text::LanguageModel const& languageModel);
    };
    struct WINRT_IMPL_EMPTY_BASES TextToTableConverter : winrt::Microsoft::Windows::AI::Text::ITextToTableConverter
    {
        TextToTableConverter(std::nullptr_t) noexcept {}
        TextToTableConverter(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::AI::Text::ITextToTableConverter(ptr, take_ownership_from_abi) {}
        explicit TextToTableConverter(winrt::Microsoft::Windows::AI::Text::LanguageModel const& languageModel);
    };
    struct WINRT_IMPL_EMPTY_BASES TextToTableResponseResult : winrt::Microsoft::Windows::AI::Text::ITextToTableResponseResult
    {
        TextToTableResponseResult(std::nullptr_t) noexcept {}
        TextToTableResponseResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::AI::Text::ITextToTableResponseResult(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES TextToTableRow : winrt::Microsoft::Windows::AI::Text::ITextToTableRow
    {
        TextToTableRow(std::nullptr_t) noexcept {}
        TextToTableRow(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::AI::Text::ITextToTableRow(ptr, take_ownership_from_abi) {}
    };
}
#endif
