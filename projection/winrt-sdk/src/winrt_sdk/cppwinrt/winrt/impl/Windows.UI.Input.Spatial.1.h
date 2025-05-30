// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_UI_Input_Spatial_1_H
#define WINRT_Windows_UI_Input_Spatial_1_H
#include "winrt/impl/Windows.UI.Input.Spatial.0.h"
WINRT_EXPORT namespace winrt::Windows::UI::Input::Spatial
{
    struct WINRT_IMPL_EMPTY_BASES ISpatialGestureRecognizer :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialGestureRecognizer>
    {
        ISpatialGestureRecognizer(std::nullptr_t = nullptr) noexcept {}
        ISpatialGestureRecognizer(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialGestureRecognizerFactory :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialGestureRecognizerFactory>
    {
        ISpatialGestureRecognizerFactory(std::nullptr_t = nullptr) noexcept {}
        ISpatialGestureRecognizerFactory(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialHoldCanceledEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialHoldCanceledEventArgs>
    {
        ISpatialHoldCanceledEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialHoldCanceledEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialHoldCompletedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialHoldCompletedEventArgs>
    {
        ISpatialHoldCompletedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialHoldCompletedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialHoldStartedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialHoldStartedEventArgs>
    {
        ISpatialHoldStartedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialHoldStartedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteraction :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteraction>
    {
        ISpatialInteraction(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteraction(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionController :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionController>
    {
        ISpatialInteractionController(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionController(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionController2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionController2>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialInteractionController2, winrt::Windows::UI::Input::Spatial::ISpatialInteractionController>
    {
        ISpatialInteractionController2(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionController2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionController3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionController3>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialInteractionController3, winrt::Windows::UI::Input::Spatial::ISpatialInteractionController, winrt::Windows::UI::Input::Spatial::ISpatialInteractionController2>
    {
        ISpatialInteractionController3(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionController3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionControllerProperties :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionControllerProperties>
    {
        ISpatialInteractionControllerProperties(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionControllerProperties(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionDetectedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionDetectedEventArgs>
    {
        ISpatialInteractionDetectedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionDetectedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionDetectedEventArgs2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionDetectedEventArgs2>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialInteractionDetectedEventArgs2, winrt::Windows::UI::Input::Spatial::ISpatialInteractionDetectedEventArgs>
    {
        ISpatialInteractionDetectedEventArgs2(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionDetectedEventArgs2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionManager :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionManager>
    {
        ISpatialInteractionManager(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionManager(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionManagerStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionManagerStatics>
    {
        ISpatialInteractionManagerStatics(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionManagerStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionManagerStatics2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionManagerStatics2>
    {
        ISpatialInteractionManagerStatics2(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionManagerStatics2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSource :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSource>
    {
        ISpatialInteractionSource(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSource(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSource2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSource2>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialInteractionSource2, winrt::Windows::UI::Input::Spatial::ISpatialInteractionSource>
    {
        ISpatialInteractionSource2(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSource2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSource3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSource3>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialInteractionSource3, winrt::Windows::UI::Input::Spatial::ISpatialInteractionSource, winrt::Windows::UI::Input::Spatial::ISpatialInteractionSource2>
    {
        ISpatialInteractionSource3(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSource3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSource4 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSource4>
    {
        ISpatialInteractionSource4(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSource4(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSourceEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSourceEventArgs>
    {
        ISpatialInteractionSourceEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSourceEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSourceEventArgs2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSourceEventArgs2>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialInteractionSourceEventArgs2, winrt::Windows::UI::Input::Spatial::ISpatialInteractionSourceEventArgs>
    {
        ISpatialInteractionSourceEventArgs2(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSourceEventArgs2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSourceLocation :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSourceLocation>
    {
        ISpatialInteractionSourceLocation(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSourceLocation(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSourceLocation2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSourceLocation2>
    {
        ISpatialInteractionSourceLocation2(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSourceLocation2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSourceLocation3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSourceLocation3>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialInteractionSourceLocation3, winrt::Windows::UI::Input::Spatial::ISpatialInteractionSourceLocation2>
    {
        ISpatialInteractionSourceLocation3(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSourceLocation3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSourceProperties :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSourceProperties>
    {
        ISpatialInteractionSourceProperties(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSourceProperties(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSourceState :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSourceState>
    {
        ISpatialInteractionSourceState(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSourceState(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSourceState2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSourceState2>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialInteractionSourceState2, winrt::Windows::UI::Input::Spatial::ISpatialInteractionSourceState>
    {
        ISpatialInteractionSourceState2(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSourceState2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialInteractionSourceState3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialInteractionSourceState3>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialInteractionSourceState3, winrt::Windows::UI::Input::Spatial::ISpatialInteractionSourceState, winrt::Windows::UI::Input::Spatial::ISpatialInteractionSourceState2>
    {
        ISpatialInteractionSourceState3(std::nullptr_t = nullptr) noexcept {}
        ISpatialInteractionSourceState3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialManipulationCanceledEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialManipulationCanceledEventArgs>
    {
        ISpatialManipulationCanceledEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialManipulationCanceledEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialManipulationCompletedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialManipulationCompletedEventArgs>
    {
        ISpatialManipulationCompletedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialManipulationCompletedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialManipulationDelta :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialManipulationDelta>
    {
        ISpatialManipulationDelta(std::nullptr_t = nullptr) noexcept {}
        ISpatialManipulationDelta(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialManipulationStartedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialManipulationStartedEventArgs>
    {
        ISpatialManipulationStartedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialManipulationStartedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialManipulationUpdatedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialManipulationUpdatedEventArgs>
    {
        ISpatialManipulationUpdatedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialManipulationUpdatedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialNavigationCanceledEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialNavigationCanceledEventArgs>
    {
        ISpatialNavigationCanceledEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialNavigationCanceledEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialNavigationCompletedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialNavigationCompletedEventArgs>
    {
        ISpatialNavigationCompletedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialNavigationCompletedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialNavigationStartedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialNavigationStartedEventArgs>
    {
        ISpatialNavigationStartedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialNavigationStartedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialNavigationUpdatedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialNavigationUpdatedEventArgs>
    {
        ISpatialNavigationUpdatedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialNavigationUpdatedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialPointerInteractionSourcePose :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialPointerInteractionSourcePose>
    {
        ISpatialPointerInteractionSourcePose(std::nullptr_t = nullptr) noexcept {}
        ISpatialPointerInteractionSourcePose(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialPointerInteractionSourcePose2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialPointerInteractionSourcePose2>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialPointerInteractionSourcePose2, winrt::Windows::UI::Input::Spatial::ISpatialPointerInteractionSourcePose>
    {
        ISpatialPointerInteractionSourcePose2(std::nullptr_t = nullptr) noexcept {}
        ISpatialPointerInteractionSourcePose2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialPointerPose :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialPointerPose>
    {
        ISpatialPointerPose(std::nullptr_t = nullptr) noexcept {}
        ISpatialPointerPose(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialPointerPose2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialPointerPose2>,
        impl::require<winrt::Windows::UI::Input::Spatial::ISpatialPointerPose2, winrt::Windows::UI::Input::Spatial::ISpatialPointerPose>
    {
        ISpatialPointerPose2(std::nullptr_t = nullptr) noexcept {}
        ISpatialPointerPose2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialPointerPose3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialPointerPose3>
    {
        ISpatialPointerPose3(std::nullptr_t = nullptr) noexcept {}
        ISpatialPointerPose3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialPointerPoseStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialPointerPoseStatics>
    {
        ISpatialPointerPoseStatics(std::nullptr_t = nullptr) noexcept {}
        ISpatialPointerPoseStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialRecognitionEndedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialRecognitionEndedEventArgs>
    {
        ISpatialRecognitionEndedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialRecognitionEndedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialRecognitionStartedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialRecognitionStartedEventArgs>
    {
        ISpatialRecognitionStartedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialRecognitionStartedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISpatialTappedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISpatialTappedEventArgs>
    {
        ISpatialTappedEventArgs(std::nullptr_t = nullptr) noexcept {}
        ISpatialTappedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
}
#endif
