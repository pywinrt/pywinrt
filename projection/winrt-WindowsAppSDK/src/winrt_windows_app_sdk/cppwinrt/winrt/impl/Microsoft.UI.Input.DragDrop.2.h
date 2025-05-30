// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_UI_Input_DragDrop_2_H
#define WINRT_Microsoft_UI_Input_DragDrop_2_H
#include "winrt/impl/Microsoft.UI.Content.1.h"
#include "winrt/impl/Windows.Foundation.1.h"
#include "winrt/impl/Microsoft.UI.Input.DragDrop.1.h"
WINRT_EXPORT namespace winrt::Microsoft::UI::Input::DragDrop
{
    struct WINRT_IMPL_EMPTY_BASES DragDropManager : winrt::Microsoft::UI::Input::DragDrop::IDragDropManager,
        impl::require<DragDropManager, winrt::Windows::Foundation::IClosable>
    {
        DragDropManager(std::nullptr_t) noexcept {}
        DragDropManager(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Input::DragDrop::IDragDropManager(ptr, take_ownership_from_abi) {}
        static auto GetForIsland(winrt::Microsoft::UI::Content::ContentIsland const& content);
    };
    struct WINRT_IMPL_EMPTY_BASES DragInfo : winrt::Microsoft::UI::Input::DragDrop::IDragInfo
    {
        DragInfo(std::nullptr_t) noexcept {}
        DragInfo(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Input::DragDrop::IDragInfo(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES DragOperation : winrt::Microsoft::UI::Input::DragDrop::IDragOperation,
        impl::require<DragOperation, winrt::Windows::Foundation::IClosable>
    {
        DragOperation(std::nullptr_t) noexcept {}
        DragOperation(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Input::DragDrop::IDragOperation(ptr, take_ownership_from_abi) {}
        DragOperation();
    };
    struct WINRT_IMPL_EMPTY_BASES DragUIOverride : winrt::Microsoft::UI::Input::DragDrop::IDragUIOverride
    {
        DragUIOverride(std::nullptr_t) noexcept {}
        DragUIOverride(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Input::DragDrop::IDragUIOverride(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES DropOperationTargetRequestedEventArgs : winrt::Microsoft::UI::Input::DragDrop::IDropOperationTargetRequestedEventArgs
    {
        DropOperationTargetRequestedEventArgs(std::nullptr_t) noexcept {}
        DropOperationTargetRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Input::DragDrop::IDropOperationTargetRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
}
#endif
