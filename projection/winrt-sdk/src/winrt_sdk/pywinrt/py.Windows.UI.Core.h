// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.System.h")
#include "py.Windows.System.h"
#endif

#if __has_include("py.Windows.UI.h")
#include "py.Windows.UI.h"
#endif

#if __has_include("py.Windows.UI.Composition.h")
#include "py.Windows.UI.Composition.h"
#endif

#if __has_include("py.Windows.UI.Input.h")
#include "py.Windows.UI.Input.h"
#endif

#if __has_include("py.Windows.UI.Popups.h")
#include "py.Windows.UI.Popups.h"
#endif

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.UI.Popups.h>

#include <winrt/Windows.UI.Core.h>

namespace py::proj::Windows::UI::Core
{}

namespace py::impl::Windows::UI::Core
{
    struct DispatchedHandler
    {
        static winrt::Windows::UI::Core::DispatchedHandler get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)]()
            {
                winrt::handle_type<py::gil_state_traits> gil_state{ PyGILState_Ensure() };

                py::pyobj_handle args{ nullptr };
                py::pyobj_handle return_value{ PyObject_CallObject(delegate.callable(), args.get()) };

                if (!return_value)
                {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }
            };
        };
    };

    struct IdleDispatchedHandler
    {
        static winrt::Windows::UI::Core::IdleDispatchedHandler get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)](auto param0)
            {
                winrt::handle_type<py::gil_state_traits> gil_state{ PyGILState_Ensure() };

                py::pyobj_handle py_param0{ py::convert(param0) };

                py::pyobj_handle args{ PyTuple_Pack(1, py_param0.get()) };

                if (!args) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }

                py::pyobj_handle return_value{ PyObject_CallObject(delegate.callable(), args.get()) };

                if (!return_value)
                {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }
            };
        };
    };
}

namespace py::wrapper::Windows::UI::Core
{
    using AcceleratorKeyEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::AcceleratorKeyEventArgs>;
    using AutomationProviderRequestedEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::AutomationProviderRequestedEventArgs>;
    using BackRequestedEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::BackRequestedEventArgs>;
    using CharacterReceivedEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::CharacterReceivedEventArgs>;
    using ClosestInteractiveBoundsRequestedEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::ClosestInteractiveBoundsRequestedEventArgs>;
    using CoreAcceleratorKeys = py::winrt_wrapper<winrt::Windows::UI::Core::CoreAcceleratorKeys>;
    using CoreComponentInputSource = py::winrt_wrapper<winrt::Windows::UI::Core::CoreComponentInputSource>;
    using CoreCursor = py::winrt_wrapper<winrt::Windows::UI::Core::CoreCursor>;
    using CoreDispatcher = py::winrt_wrapper<winrt::Windows::UI::Core::CoreDispatcher>;
    using CoreIndependentInputSource = py::winrt_wrapper<winrt::Windows::UI::Core::CoreIndependentInputSource>;
    using CoreIndependentInputSourceController = py::winrt_wrapper<winrt::Windows::UI::Core::CoreIndependentInputSourceController>;
    using CoreWindow = py::winrt_wrapper<winrt::Windows::UI::Core::CoreWindow>;
    using CoreWindowDialog = py::winrt_wrapper<winrt::Windows::UI::Core::CoreWindowDialog>;
    using CoreWindowEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::CoreWindowEventArgs>;
    using CoreWindowFlyout = py::winrt_wrapper<winrt::Windows::UI::Core::CoreWindowFlyout>;
    using CoreWindowPopupShowingEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::CoreWindowPopupShowingEventArgs>;
    using CoreWindowResizeManager = py::winrt_wrapper<winrt::Windows::UI::Core::CoreWindowResizeManager>;
    using IdleDispatchedHandlerArgs = py::winrt_wrapper<winrt::Windows::UI::Core::IdleDispatchedHandlerArgs>;
    using InputEnabledEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::InputEnabledEventArgs>;
    using KeyEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::KeyEventArgs>;
    using PointerEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::PointerEventArgs>;
    using SystemNavigationManager = py::winrt_wrapper<winrt::Windows::UI::Core::SystemNavigationManager>;
    using TouchHitTestingEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::TouchHitTestingEventArgs>;
    using VisibilityChangedEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::VisibilityChangedEventArgs>;
    using WindowActivatedEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::WindowActivatedEventArgs>;
    using WindowSizeChangedEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::WindowSizeChangedEventArgs>;
    using ICoreAcceleratorKeys = py::winrt_wrapper<winrt::Windows::UI::Core::ICoreAcceleratorKeys>;
    using ICoreInputSourceBase = py::winrt_wrapper<winrt::Windows::UI::Core::ICoreInputSourceBase>;
    using ICorePointerInputSource = py::winrt_wrapper<winrt::Windows::UI::Core::ICorePointerInputSource>;
    using ICorePointerInputSource2 = py::winrt_wrapper<winrt::Windows::UI::Core::ICorePointerInputSource2>;
    using ICorePointerRedirector = py::winrt_wrapper<winrt::Windows::UI::Core::ICorePointerRedirector>;
    using ICoreWindow = py::winrt_wrapper<winrt::Windows::UI::Core::ICoreWindow>;
    using ICoreWindowEventArgs = py::winrt_wrapper<winrt::Windows::UI::Core::ICoreWindowEventArgs>;
    using IInitializeWithCoreWindow = py::winrt_wrapper<winrt::Windows::UI::Core::IInitializeWithCoreWindow>;
    using CorePhysicalKeyStatus = py::winrt_struct_wrapper<winrt::Windows::UI::Core::CorePhysicalKeyStatus>;
    using CoreProximityEvaluation = py::winrt_struct_wrapper<winrt::Windows::UI::Core::CoreProximityEvaluation>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::AppViewBackButtonVisibility> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreAcceleratorKeyEventType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreCursorType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreDispatcherPriority> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreIndependentInputFilters> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreInputDeviceTypes> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreProcessEventsOption> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreProximityEvaluationScore> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreVirtualKeyStates> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreWindowActivationMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreWindowActivationState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreWindowFlowDirection> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CorePhysicalKeyStatus> = "T{I:repeat_count:I:scan_code:?:is_extended_key:?:is_menu_key_down:?:was_key_down:?:is_key_released:}";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::Core::CoreProximityEvaluation> = "T{i:score:T{f:x:f:y:}:adjusted_point:}";


    template<>
    struct py_type<winrt::Windows::UI::Core::AppViewBackButtonVisibility>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "AppViewBackButtonVisibility";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreAcceleratorKeyEventType>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreAcceleratorKeyEventType";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreCursorType>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreCursorType";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreDispatcherPriority>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreDispatcherPriority";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreIndependentInputFilters>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreIndependentInputFilters";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreInputDeviceTypes>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreInputDeviceTypes";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreProcessEventsOption>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreProcessEventsOption";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreProximityEvaluationScore>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreProximityEvaluationScore";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreVirtualKeyStates>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreVirtualKeyStates";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreWindowActivationMode>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreWindowActivationMode";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreWindowActivationState>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreWindowActivationState";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreWindowFlowDirection>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreWindowFlowDirection";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::AcceleratorKeyEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "AcceleratorKeyEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::AutomationProviderRequestedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "AutomationProviderRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::BackRequestedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "BackRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CharacterReceivedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CharacterReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::ClosestInteractiveBoundsRequestedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "ClosestInteractiveBoundsRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreAcceleratorKeys>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreAcceleratorKeys";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreComponentInputSource>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreComponentInputSource";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreCursor>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreCursor";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreDispatcher>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreDispatcher";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreIndependentInputSource>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreIndependentInputSource";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreIndependentInputSourceController>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreIndependentInputSourceController";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreWindow>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreWindow";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreWindowDialog>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreWindowDialog";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreWindowEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreWindowEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreWindowFlyout>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreWindowFlyout";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreWindowPopupShowingEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreWindowPopupShowingEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreWindowResizeManager>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreWindowResizeManager";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::IdleDispatchedHandlerArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "IdleDispatchedHandlerArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::InputEnabledEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "InputEnabledEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::KeyEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "KeyEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::PointerEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "PointerEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::SystemNavigationManager>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "SystemNavigationManager";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::TouchHitTestingEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "TouchHitTestingEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::VisibilityChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "VisibilityChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::WindowActivatedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "WindowActivatedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::WindowSizeChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "WindowSizeChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::ICoreAcceleratorKeys>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "ICoreAcceleratorKeys";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::ICoreInputSourceBase>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "ICoreInputSourceBase";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::ICorePointerInputSource>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "ICorePointerInputSource";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::ICorePointerInputSource2>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "ICorePointerInputSource2";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::ICorePointerRedirector>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "ICorePointerRedirector";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::ICoreWindow>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "ICoreWindow";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::ICoreWindowEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "ICoreWindowEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::IInitializeWithCoreWindow>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "IInitializeWithCoreWindow";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CorePhysicalKeyStatus>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CorePhysicalKeyStatus";
    };

    template<>
    struct py_type<winrt::Windows::UI::Core::CoreProximityEvaluation>
    {
        static constexpr const char* module_name = "winrt.windows.ui.core";
        static constexpr const char* type_name = "CoreProximityEvaluation";
    };
    template <>
    struct delegate_python_type<winrt::Windows::UI::Core::DispatchedHandler>
    {
        using type = py::impl::Windows::UI::Core::DispatchedHandler;
    };

    template <>
    struct delegate_python_type<winrt::Windows::UI::Core::IdleDispatchedHandler>
    {
        using type = py::impl::Windows::UI::Core::IdleDispatchedHandler;
    };

}