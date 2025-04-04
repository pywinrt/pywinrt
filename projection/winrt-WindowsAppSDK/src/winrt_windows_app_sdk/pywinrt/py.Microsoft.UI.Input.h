// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Content.h>
#include <winrt/Microsoft.UI.Dispatching.h>

#include <winrt/Microsoft.UI.Input.h>

namespace py::proj::Microsoft::UI::Input
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::CrossSlidingState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::DraggingState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::FocusNavigationReason> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::FocusNavigationResult> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::GestureSettings> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::HoldingState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::InputActivationState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::InputPointerSourceDeviceKinds> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::InputSystemCursorShape> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::MoveSizeOperation> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::NonClientRegionKind> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::PointerDeviceType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::PointerUpdateKind> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::VirtualKeyStates> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::CrossSlideThresholds> = "T{f:selection_start:f:speed_bump_start:f:speed_bump_end:f:rearrange_start:}";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::ManipulationDelta> = "T{T{f:x:f:y:}:translation:f:scale:f:rotation:f:expansion:}";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::ManipulationVelocities> = "T{T{f:x:f:y:}:linear:f:angular:f:expansion:}";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Input::PhysicalKeyStatus> = "T{I:repeat_count:I:scan_code:?:is_extended_key:?:is_menu_key_down:?:was_key_down:?:is_key_released:}";


    template<>
    struct py_type<winrt::Microsoft::UI::Input::CrossSlidingState>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.CrossSlidingState";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "CrossSlidingState";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::DraggingState>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.DraggingState";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "DraggingState";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::FocusNavigationReason>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.FocusNavigationReason";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "FocusNavigationReason";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::FocusNavigationResult>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.FocusNavigationResult";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "FocusNavigationResult";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::GestureSettings>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.GestureSettings";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "GestureSettings";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::HoldingState>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.HoldingState";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "HoldingState";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputActivationState>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputActivationState";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputActivationState";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputPointerSourceDeviceKinds>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputPointerSourceDeviceKinds";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputPointerSourceDeviceKinds";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputSystemCursorShape>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputSystemCursorShape";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputSystemCursorShape";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::MoveSizeOperation>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.MoveSizeOperation";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "MoveSizeOperation";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::NonClientRegionKind>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.NonClientRegionKind";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "NonClientRegionKind";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::PointerDeviceType>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.PointerDeviceType";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "PointerDeviceType";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::PointerUpdateKind>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.PointerUpdateKind";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "PointerUpdateKind";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::VirtualKeyStates>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.VirtualKeyStates";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "VirtualKeyStates";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::CharacterReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.CharacterReceivedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "CharacterReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::ContextMenuKeyEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.ContextMenuKeyEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "ContextMenuKeyEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::CrossSlidingEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.CrossSlidingEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "CrossSlidingEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::DraggingEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.DraggingEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "DraggingEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::EnteredMoveSizeEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.EnteredMoveSizeEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "EnteredMoveSizeEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::EnteringMoveSizeEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.EnteringMoveSizeEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "EnteringMoveSizeEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::ExitedMoveSizeEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.ExitedMoveSizeEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "ExitedMoveSizeEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::FocusChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.FocusChangedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "FocusChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::FocusNavigationRequest>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.FocusNavigationRequest";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "FocusNavigationRequest";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::FocusNavigationRequestEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.FocusNavigationRequestEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "FocusNavigationRequestEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::GestureRecognizer>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.GestureRecognizer";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "GestureRecognizer";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::HoldingEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.HoldingEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "HoldingEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputActivationListener>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputActivationListener";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputActivationListener";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputActivationListenerActivationChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputActivationListenerActivationChangedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputActivationListenerActivationChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputCursor>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputCursor";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputCursor";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputCustomCursor>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputCustomCursor";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputCustomCursor";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputDesktopNamedResourceCursor>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputDesktopNamedResourceCursor";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputDesktopNamedResourceCursor";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputDesktopResourceCursor>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputDesktopResourceCursor";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputDesktopResourceCursor";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputFocusController>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputFocusController";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputFocusController";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputFocusNavigationHost>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputFocusNavigationHost";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputFocusNavigationHost";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputKeyboardSource>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputKeyboardSource";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputKeyboardSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputLightDismissAction>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputLightDismissAction";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputLightDismissAction";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputLightDismissEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputLightDismissEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputLightDismissEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputNonClientPointerSource>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputNonClientPointerSource";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputNonClientPointerSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputObject>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputObject";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputObject";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputPointerSource>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputPointerSource";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputPointerSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputPreTranslateKeyboardSource>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputPreTranslateKeyboardSource";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputPreTranslateKeyboardSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::InputSystemCursor>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.InputSystemCursor";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "InputSystemCursor";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::KeyEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.KeyEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "KeyEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::ManipulationCompletedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.ManipulationCompletedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "ManipulationCompletedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::ManipulationInertiaStartingEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.ManipulationInertiaStartingEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "ManipulationInertiaStartingEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::ManipulationStartedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.ManipulationStartedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "ManipulationStartedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::ManipulationUpdatedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.ManipulationUpdatedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "ManipulationUpdatedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::MouseWheelParameters>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.MouseWheelParameters";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "MouseWheelParameters";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::NonClientCaptionTappedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.NonClientCaptionTappedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "NonClientCaptionTappedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::NonClientPointerEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.NonClientPointerEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "NonClientPointerEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::NonClientRegionsChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.NonClientRegionsChangedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "NonClientRegionsChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::PointerEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.PointerEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "PointerEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::PointerPoint>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.PointerPoint";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "PointerPoint";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::PointerPointProperties>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.PointerPointProperties";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "PointerPointProperties";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::PointerPredictor>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.PointerPredictor";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "PointerPredictor";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::RightTappedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.RightTappedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "RightTappedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::TappedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.TappedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "TappedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::WindowRectChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.WindowRectChangedEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "WindowRectChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::WindowRectChangingEventArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.WindowRectChangingEventArgs";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "WindowRectChangingEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::IPointerPointTransform>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input._IPointerPointTransform";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "_IPointerPointTransform";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::CrossSlideThresholds>
    {
        static constexpr std::string_view from_tuple = "winui3._winui3_microsoft_ui_input.CrossSlideThresholds_from_tuple";
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.CrossSlideThresholds";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "CrossSlideThresholds";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::ManipulationDelta>
    {
        static constexpr std::string_view from_tuple = "winui3._winui3_microsoft_ui_input.ManipulationDelta_from_tuple";
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.ManipulationDelta";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "ManipulationDelta";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::ManipulationVelocities>
    {
        static constexpr std::string_view from_tuple = "winui3._winui3_microsoft_ui_input.ManipulationVelocities_from_tuple";
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.ManipulationVelocities";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "ManipulationVelocities";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Input::PhysicalKeyStatus>
    {
        static constexpr std::string_view from_tuple = "winui3._winui3_microsoft_ui_input.PhysicalKeyStatus_from_tuple";
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.input.PhysicalKeyStatus";
        static constexpr const char* module_name = "winui3.microsoft.ui.input";
        static constexpr const char* type_name = "PhysicalKeyStatus";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Graphics.h")
#include "py.Windows.Graphics.h"
#endif

#if __has_include("py.Windows.System.h")
#include "py.Windows.System.h"
#endif

#if __has_include("py.Windows.UI.Core.h")
#include "py.Windows.UI.Core.h"
#endif

#if __has_include("py.Microsoft.UI.h")
#include "py.Microsoft.UI.h"
#endif

#if __has_include("py.Microsoft.UI.Content.h")
#include "py.Microsoft.UI.Content.h"
#endif

#if __has_include("py.Microsoft.UI.Dispatching.h")
#include "py.Microsoft.UI.Dispatching.h"
#endif

namespace py::impl::Microsoft::UI::Input
{
}

namespace py::wrapper::Microsoft::UI::Input
{
    using CharacterReceivedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::CharacterReceivedEventArgs>;
    using ContextMenuKeyEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::ContextMenuKeyEventArgs>;
    using CrossSlidingEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::CrossSlidingEventArgs>;
    using DraggingEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::DraggingEventArgs>;
    using EnteredMoveSizeEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::EnteredMoveSizeEventArgs>;
    using EnteringMoveSizeEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::EnteringMoveSizeEventArgs>;
    using ExitedMoveSizeEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::ExitedMoveSizeEventArgs>;
    using FocusChangedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::FocusChangedEventArgs>;
    using FocusNavigationRequest = py::winrt_wrapper<winrt::Microsoft::UI::Input::FocusNavigationRequest>;
    using FocusNavigationRequestEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::FocusNavigationRequestEventArgs>;
    using GestureRecognizer = py::winrt_wrapper<winrt::Microsoft::UI::Input::GestureRecognizer>;
    using HoldingEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::HoldingEventArgs>;
    using InputActivationListener = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputActivationListener>;
    using InputActivationListenerActivationChangedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputActivationListenerActivationChangedEventArgs>;
    using InputCursor = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputCursor>;
    using InputCustomCursor = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputCustomCursor>;
    using InputDesktopNamedResourceCursor = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputDesktopNamedResourceCursor>;
    using InputDesktopResourceCursor = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputDesktopResourceCursor>;
    using InputFocusController = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputFocusController>;
    using InputFocusNavigationHost = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputFocusNavigationHost>;
    using InputKeyboardSource = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputKeyboardSource>;
    using InputLightDismissAction = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputLightDismissAction>;
    using InputLightDismissEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputLightDismissEventArgs>;
    using InputNonClientPointerSource = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputNonClientPointerSource>;
    using InputObject = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputObject>;
    using InputPointerSource = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputPointerSource>;
    using InputPreTranslateKeyboardSource = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputPreTranslateKeyboardSource>;
    using InputSystemCursor = py::winrt_wrapper<winrt::Microsoft::UI::Input::InputSystemCursor>;
    using KeyEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::KeyEventArgs>;
    using ManipulationCompletedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::ManipulationCompletedEventArgs>;
    using ManipulationInertiaStartingEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::ManipulationInertiaStartingEventArgs>;
    using ManipulationStartedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::ManipulationStartedEventArgs>;
    using ManipulationUpdatedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::ManipulationUpdatedEventArgs>;
    using MouseWheelParameters = py::winrt_wrapper<winrt::Microsoft::UI::Input::MouseWheelParameters>;
    using NonClientCaptionTappedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::NonClientCaptionTappedEventArgs>;
    using NonClientPointerEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::NonClientPointerEventArgs>;
    using NonClientRegionsChangedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::NonClientRegionsChangedEventArgs>;
    using PointerEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::PointerEventArgs>;
    using PointerPoint = py::winrt_wrapper<winrt::Microsoft::UI::Input::PointerPoint>;
    using PointerPointProperties = py::winrt_wrapper<winrt::Microsoft::UI::Input::PointerPointProperties>;
    using PointerPredictor = py::winrt_wrapper<winrt::Microsoft::UI::Input::PointerPredictor>;
    using RightTappedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::RightTappedEventArgs>;
    using TappedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::TappedEventArgs>;
    using WindowRectChangedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::WindowRectChangedEventArgs>;
    using WindowRectChangingEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Input::WindowRectChangingEventArgs>;
    using IPointerPointTransform = py::winrt_wrapper<winrt::Microsoft::UI::Input::IPointerPointTransform>;
    using CrossSlideThresholds = py::winrt_struct_wrapper<winrt::Microsoft::UI::Input::CrossSlideThresholds>;
    using ManipulationDelta = py::winrt_struct_wrapper<winrt::Microsoft::UI::Input::ManipulationDelta>;
    using ManipulationVelocities = py::winrt_struct_wrapper<winrt::Microsoft::UI::Input::ManipulationVelocities>;
    using PhysicalKeyStatus = py::winrt_struct_wrapper<winrt::Microsoft::UI::Input::PhysicalKeyStatus>;
}

namespace py
{
}
