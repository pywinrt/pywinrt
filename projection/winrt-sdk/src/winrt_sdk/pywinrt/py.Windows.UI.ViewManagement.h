// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Popups.h>
#include <winrt/Windows.UI.WindowManagement.h>

#include <winrt/Windows.UI.ViewManagement.h>

namespace py::proj::Windows::UI::ViewManagement
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::ApplicationViewBoundsMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::ApplicationViewMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::ApplicationViewOrientation> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::ApplicationViewState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::ApplicationViewSwitchingOptions> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::ApplicationViewWindowingMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::FullScreenSystemOverlayMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::HandPreference> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::ScreenCaptureDisabledBehavior> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::UIColorType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::UIElementType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::UserInteractionMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::UI::ViewManagement::ViewSizePreference> = "i";


    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewBoundsMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewBoundsMode";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewBoundsMode";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewMode";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewMode";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewOrientation>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewOrientation";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewOrientation";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewState>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewState";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewState";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewSwitchingOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewSwitchingOptions";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewSwitchingOptions";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewWindowingMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewWindowingMode";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewWindowingMode";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::FullScreenSystemOverlayMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.FullScreenSystemOverlayMode";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "FullScreenSystemOverlayMode";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::HandPreference>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.HandPreference";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "HandPreference";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ScreenCaptureDisabledBehavior>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ScreenCaptureDisabledBehavior";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ScreenCaptureDisabledBehavior";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::UIColorType>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.UIColorType";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "UIColorType";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::UIElementType>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.UIElementType";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "UIElementType";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::UserInteractionMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.UserInteractionMode";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "UserInteractionMode";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ViewSizePreference>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ViewSizePreference";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ViewSizePreference";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::AccessibilitySettings>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.AccessibilitySettings";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "AccessibilitySettings";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ActivationViewSwitcher>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ActivationViewSwitcher";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ActivationViewSwitcher";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationView>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationView";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationView";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewConsolidatedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewConsolidatedEventArgs";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewConsolidatedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewScaling>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewScaling";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewScaling";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewSwitcher>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewSwitcher";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewSwitcher";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewTitleBar>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewTitleBar";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewTitleBar";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ApplicationViewTransferContext>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ApplicationViewTransferContext";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ApplicationViewTransferContext";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::InputPane>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.InputPane";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "InputPane";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::InputPaneVisibilityEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.InputPaneVisibilityEventArgs";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "InputPaneVisibilityEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ProjectionManager>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ProjectionManager";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ProjectionManager";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::UISettings>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.UISettings";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "UISettings";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::UISettingsAnimationsEnabledChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.UISettingsAnimationsEnabledChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "UISettingsAnimationsEnabledChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::UISettingsAutoHideScrollBarsChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.UISettingsAutoHideScrollBarsChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "UISettingsAutoHideScrollBarsChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::UISettingsMessageDurationChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.UISettingsMessageDurationChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "UISettingsMessageDurationChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::UIViewSettings>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.UIViewSettings";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "UIViewSettings";
    };

    template<>
    struct py_type<winrt::Windows::UI::ViewManagement::ViewModePreferences>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.viewmanagement.ViewModePreferences";
        static constexpr const char* module_name = "winrt.windows.ui.viewmanagement";
        static constexpr const char* type_name = "ViewModePreferences";
    };
}

#if __has_include("py.Windows.Devices.Enumeration.h")
#include "py.Windows.Devices.Enumeration.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.UI.h")
#include "py.Windows.UI.h"
#endif

#if __has_include("py.Windows.UI.Core.h")
#include "py.Windows.UI.Core.h"
#endif

#if __has_include("py.Windows.UI.Popups.h")
#include "py.Windows.UI.Popups.h"
#endif

#if __has_include("py.Windows.UI.WindowManagement.h")
#include "py.Windows.UI.WindowManagement.h"
#endif

namespace py::impl::Windows::UI::ViewManagement
{
}

namespace py::wrapper::Windows::UI::ViewManagement
{
    using AccessibilitySettings = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::AccessibilitySettings>;
    using ActivationViewSwitcher = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::ActivationViewSwitcher>;
    using ApplicationView = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::ApplicationView>;
    using ApplicationViewConsolidatedEventArgs = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::ApplicationViewConsolidatedEventArgs>;
    using ApplicationViewScaling = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::ApplicationViewScaling>;
    using ApplicationViewSwitcher = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::ApplicationViewSwitcher>;
    using ApplicationViewTitleBar = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::ApplicationViewTitleBar>;
    using ApplicationViewTransferContext = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::ApplicationViewTransferContext>;
    using InputPane = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::InputPane>;
    using InputPaneVisibilityEventArgs = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::InputPaneVisibilityEventArgs>;
    using ProjectionManager = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::ProjectionManager>;
    using UISettings = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::UISettings>;
    using UISettingsAnimationsEnabledChangedEventArgs = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::UISettingsAnimationsEnabledChangedEventArgs>;
    using UISettingsAutoHideScrollBarsChangedEventArgs = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::UISettingsAutoHideScrollBarsChangedEventArgs>;
    using UISettingsMessageDurationChangedEventArgs = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::UISettingsMessageDurationChangedEventArgs>;
    using UIViewSettings = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::UIViewSettings>;
    using ViewModePreferences = py::winrt_wrapper<winrt::Windows::UI::ViewManagement::ViewModePreferences>;
}

namespace py
{
}
