# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import typing
import uuid as _uuid

import winrt.runtime._internals
import winrt.system
from winrt._winrt_windows_ui_xaml_controls import (
    AnchorRequestedEventArgs,
    AppBar,
    AppBarElementContainer,
    AppBarSeparator,
    AutoSuggestBox,
    AutoSuggestBoxQuerySubmittedEventArgs,
    AutoSuggestBoxSuggestionChosenEventArgs,
    AutoSuggestBoxTextChangedEventArgs,
    BackClickEventArgs,
    BitmapIcon,
    BitmapIconSource,
    Border,
    CalendarDatePicker,
    CalendarDatePickerDateChangedEventArgs,
    CalendarView,
    CalendarViewDayItem,
    CalendarViewDayItemChangingEventArgs,
    CalendarViewSelectedDatesChangedEventArgs,
    CandidateWindowBoundsChangedEventArgs,
    Canvas,
    CaptureElement,
    ChoosingGroupHeaderContainerEventArgs,
    ChoosingItemContainerEventArgs,
    CleanUpVirtualizedItemEventArgs,
    ColorChangedEventArgs,
    ColorPicker,
    ColumnDefinition,
    ColumnDefinitionCollection,
    ComboBoxTextSubmittedEventArgs,
    CommandBar,
    CommandBarOverflowPresenter,
    ContainerContentChangingEventArgs,
    ContentControl,
    ContentDialog,
    ContentDialogButtonClickDeferral,
    ContentDialogButtonClickEventArgs,
    ContentDialogClosedEventArgs,
    ContentDialogClosingDeferral,
    ContentDialogClosingEventArgs,
    ContentDialogOpenedEventArgs,
    ContentLinkChangedEventArgs,
    ContentPresenter,
    ContextMenuEventArgs,
    Control,
    ControlTemplate,
    DataTemplateSelector,
    DatePickedEventArgs,
    DatePicker,
    DatePickerFlyoutItem,
    DatePickerFlyoutPresenter,
    DatePickerSelectedValueChangedEventArgs,
    DatePickerValueChangedEventArgs,
    DragItemsCompletedEventArgs,
    DragItemsStartingEventArgs,
    DropDownButtonAutomationPeer,
    DynamicOverflowItemsChangingEventArgs,
    FlyoutPresenter,
    FocusDisengagedEventArgs,
    FocusEngagedEventArgs,
    FontIcon,
    FontIconSource,
    Frame,
    Grid,
    GridViewHeaderItem,
    GroupItem,
    GroupStyle,
    GroupStyleSelector,
    HandwritingPanelClosedEventArgs,
    HandwritingPanelOpenedEventArgs,
    HandwritingView,
    HandwritingViewCandidatesChangedEventArgs,
    HandwritingViewTextSubmittedEventArgs,
    Hub,
    HubSection,
    HubSectionCollection,
    HubSectionHeaderClickEventArgs,
    IconElement,
    IconSource,
    IconSourceElement,
    Image,
    InkCanvas,
    InkToolbar,
    InkToolbarCustomPen,
    InkToolbarIsStencilButtonCheckedChangedEventArgs,
    InkToolbarPenConfigurationControl,
    IsTextTrimmedChangedEventArgs,
    ItemClickEventArgs,
    ItemCollection,
    ItemContainerGenerator,
    ItemsControl,
    ItemsPanelTemplate,
    ItemsPickedEventArgs,
    ItemsPresenter,
    ItemsStackPanel,
    ItemsWrapGrid,
    ListPickerFlyoutPresenter,
    ListViewBaseHeaderItem,
    ListViewHeaderItem,
    ListViewPersistenceHelper,
    MediaElement,
    MediaPlayerElement,
    MediaPlayerPresenter,
    MediaTransportControls,
    MediaTransportControlsHelper,
    MenuBar,
    MenuBarItem,
    MenuFlyoutItem,
    MenuFlyoutItemBase,
    MenuFlyoutPresenter,
    MenuFlyoutSeparator,
    MenuFlyoutSubItem,
    NavigationView,
    NavigationViewBackRequestedEventArgs,
    NavigationViewDisplayModeChangedEventArgs,
    NavigationViewItemInvokedEventArgs,
    NavigationViewPaneClosingEventArgs,
    NavigationViewSelectionChangedEventArgs,
    NavigationViewTemplateSettings,
    NotifyEventArgs,
    Page,
    Panel,
    ParallaxView,
    PasswordBox,
    PasswordBoxPasswordChangingEventArgs,
    PathIcon,
    PathIconSource,
    PersonPicture,
    PickerConfirmedEventArgs,
    PickerFlyoutPresenter,
    Pivot,
    PivotItem,
    PivotItemEventArgs,
    ProgressRing,
    RatingControl,
    RatingItemFontInfo,
    RatingItemImageInfo,
    RatingItemInfo,
    RefreshContainer,
    RefreshInteractionRatioChangedEventArgs,
    RefreshRequestedEventArgs,
    RefreshStateChangedEventArgs,
    RefreshVisualizer,
    RelativePanel,
    RichEditBox,
    RichEditBoxSelectionChangingEventArgs,
    RichEditBoxTextChangingEventArgs,
    RichTextBlock,
    RichTextBlockOverflow,
    RowDefinition,
    RowDefinitionCollection,
    ScrollContentPresenter,
    ScrollViewer,
    ScrollViewerView,
    ScrollViewerViewChangedEventArgs,
    ScrollViewerViewChangingEventArgs,
    SearchBox,
    SearchBoxQueryChangedEventArgs,
    SearchBoxQuerySubmittedEventArgs,
    SearchBoxResultSuggestionChosenEventArgs,
    SearchBoxSuggestionsRequestedEventArgs,
    SectionsInViewChangedEventArgs,
    SelectionChangedEventArgs,
    SemanticZoom,
    SemanticZoomLocation,
    SemanticZoomViewChangedEventArgs,
    SettingsFlyout,
    SplitButton,
    SplitButtonAutomationPeer,
    SplitButtonClickEventArgs,
    SplitView,
    SplitViewPaneClosingEventArgs,
    StackPanel,
    StyleSelector,
    SwapChainBackgroundPanel,
    SwapChainPanel,
    SwipeControl,
    SwipeItem,
    SwipeItemInvokedEventArgs,
    SwipeItems,
    SymbolIcon,
    SymbolIconSource,
    TextBlock,
    TextBox,
    TextBoxBeforeTextChangingEventArgs,
    TextBoxSelectionChangingEventArgs,
    TextBoxTextChangingEventArgs,
    TextChangedEventArgs,
    TextCompositionChangedEventArgs,
    TextCompositionEndedEventArgs,
    TextCompositionStartedEventArgs,
    TextControlCopyingToClipboardEventArgs,
    TextControlCuttingToClipboardEventArgs,
    TextControlPasteEventArgs,
    TimePickedEventArgs,
    TimePicker,
    TimePickerFlyoutPresenter,
    TimePickerSelectedValueChangedEventArgs,
    TimePickerValueChangedEventArgs,
    ToggleMenuFlyoutItem,
    ToggleSplitButton,
    ToggleSplitButtonAutomationPeer,
    ToggleSplitButtonIsCheckedChangedEventArgs,
    ToggleSwitch,
    ToolTip,
    ToolTipService,
    TreeView,
    TreeViewCollapsedEventArgs,
    TreeViewDragItemsCompletedEventArgs,
    TreeViewDragItemsStartingEventArgs,
    TreeViewExpandingEventArgs,
    TreeViewItemInvokedEventArgs,
    TreeViewItemTemplateSettings,
    TreeViewNode,
    TwoPaneView,
    UIElementCollection,
    UserControl,
    VariableSizedWrapGrid,
    Viewbox,
    VirtualizingPanel,
    WebView,
    WebViewBrush,
    WebViewContentLoadingEventArgs,
    WebViewDOMContentLoadedEventArgs,
    WebViewDeferredPermissionRequest,
    WebViewLongRunningScriptDetectedEventArgs,
    WebViewNavigationCompletedEventArgs,
    WebViewNavigationFailedEventArgs,
    WebViewNavigationStartingEventArgs,
    WebViewNewWindowRequestedEventArgs,
    WebViewPermissionRequest,
    WebViewPermissionRequestedEventArgs,
    WebViewSeparateProcessLostEventArgs,
    WebViewSettings,
    WebViewUnsupportedUriSchemeIdentifiedEventArgs,
    WebViewUnviewableContentIdentifiedEventArgs,
    WebViewWebResourceRequestedEventArgs,
    _ICommandBarElement,
    ICommandBarElement,
    _ICommandBarElement2,
    ICommandBarElement2,
    _IInsertionPanel,
    IInsertionPanel,
    _IItemContainerMapping,
    IItemContainerMapping,
    _INavigate,
    INavigate,
    _IScrollAnchorProvider,
    IScrollAnchorProvider,
    _ISemanticZoomInformation,
    ISemanticZoomInformation,
)
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from winrt._winrt_windows_ui_xaml_controls import (
        AppBar_Static,
        AppBarElementContainer_Static,
        AppBarSeparator_Static,
        BitmapIcon_Static,
        BitmapIconSource_Static,
        CalendarDatePicker_Static,
        CalendarView_Static,
        CalendarViewDayItem_Static,
        Canvas_Static,
        ColorPicker_Static,
        CommandBar_Static,
        CommandBarOverflowPresenter_Static,
        ContentControl_Static,
        ContentDialog_Static,
        ContentPresenter_Static,
        Control_Static,
        DataTemplateSelector_Static,
        DatePicker_Static,
        DropDownButtonAutomationPeer_Static,
        FlyoutPresenter_Static,
        FontIcon_Static,
        FontIconSource_Static,
        Frame_Static,
        Grid_Static,
        GridViewHeaderItem_Static,
        GroupItem_Static,
        GroupStyle_Static,
        GroupStyleSelector_Static,
        HandwritingView_Static,
        Hub_Static,
        HubSection_Static,
        IconElement_Static,
        IconSource_Static,
        IconSourceElement_Static,
        InkCanvas_Static,
        InkToolbar_Static,
        InkToolbarCustomPen_Static,
        InkToolbarPenConfigurationControl_Static,
        ItemsControl_Static,
        ListViewBaseHeaderItem_Static,
        ListViewHeaderItem_Static,
        MediaPlayerElement_Static,
        MediaPlayerPresenter_Static,
        MediaTransportControls_Static,
        MenuBar_Static,
        MenuBarItem_Static,
        MenuFlyoutItem_Static,
        MenuFlyoutItemBase_Static,
        MenuFlyoutPresenter_Static,
        MenuFlyoutSeparator_Static,
        NavigationView_Static,
        NavigationViewTemplateSettings_Static,
        Page_Static,
        Panel_Static,
        ParallaxView_Static,
        PathIcon_Static,
        PathIconSource_Static,
        PersonPicture_Static,
        Pivot_Static,
        PivotItem_Static,
        RatingControl_Static,
        RatingItemFontInfo_Static,
        RatingItemImageInfo_Static,
        RatingItemInfo_Static,
        RefreshContainer_Static,
        RefreshVisualizer_Static,
        RelativePanel_Static,
        RichEditBox_Static,
        SearchBox_Static,
        SectionsInViewChangedEventArgs_Static,
        SelectionChangedEventArgs_Static,
        SettingsFlyout_Static,
        SplitButton_Static,
        SplitButtonAutomationPeer_Static,
        SplitView_Static,
        StackPanel_Static,
        StyleSelector_Static,
        SwapChainBackgroundPanel_Static,
        SwapChainPanel_Static,
        SwipeControl_Static,
        SwipeItem_Static,
        SwipeItems_Static,
        SymbolIconSource_Static,
        TextBox_Static,
        TimePicker_Static,
        ToggleMenuFlyoutItem_Static,
        ToggleSplitButton_Static,
        ToggleSplitButtonAutomationPeer_Static,
        ToolTip_Static,
        TreeView_Static,
        TreeViewItemTemplateSettings_Static,
        TreeViewNode_Static,
        TwoPaneView_Static,
        UserControl_Static,
        VirtualizingPanel_Static,
    )
from winrt._winrt_windows_ui_xaml_controls_2 import (
    AppBarButton,
    AppBarToggleButton,
    Button,
    CheckBox,
    ComboBox,
    ComboBoxItem,
    CommandBarFlyout,
    DatePickerFlyout,
    DropDownButton,
    FlipView,
    FlipViewItem,
    Flyout,
    GridView,
    GridViewItem,
    HyperlinkButton,
    InkToolbarBallpointPenButton,
    InkToolbarCustomPenButton,
    InkToolbarCustomToggleButton,
    InkToolbarCustomToolButton,
    InkToolbarEraserButton,
    InkToolbarFlyoutItem,
    InkToolbarHighlighterButton,
    InkToolbarMenuButton,
    InkToolbarPenButton,
    InkToolbarPencilButton,
    InkToolbarRulerButton,
    InkToolbarStencilButton,
    InkToolbarToggleButton,
    InkToolbarToolButton,
    ListBox,
    ListBoxItem,
    ListPickerFlyout,
    ListView,
    ListViewBase,
    ListViewItem,
    MenuBarItemFlyout,
    MenuFlyout,
    NavigationViewItem,
    NavigationViewItemBase,
    NavigationViewItemHeader,
    NavigationViewItemSeparator,
    NavigationViewList,
    PickerFlyout,
    ProgressBar,
    RadioButton,
    Slider,
    TextCommandBarFlyout,
    TimePickerFlyout,
    TreeViewItem,
    TreeViewList,
    VirtualizingStackPanel,
    WrapGrid,
)
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from winrt._winrt_windows_ui_xaml_controls_2 import (
        AppBarButton_Static,
        AppBarToggleButton_Static,
        Button_Static,
        CheckBox_Static,
        ComboBox_Static,
        ComboBoxItem_Static,
        CommandBarFlyout_Static,
        DropDownButton_Static,
        FlipView_Static,
        FlipViewItem_Static,
        Flyout_Static,
        GridView_Static,
        GridViewItem_Static,
        HyperlinkButton_Static,
        InkToolbarBallpointPenButton_Static,
        InkToolbarCustomPenButton_Static,
        InkToolbarCustomToggleButton_Static,
        InkToolbarCustomToolButton_Static,
        InkToolbarEraserButton_Static,
        InkToolbarFlyoutItem_Static,
        InkToolbarHighlighterButton_Static,
        InkToolbarMenuButton_Static,
        InkToolbarPenButton_Static,
        InkToolbarPencilButton_Static,
        InkToolbarRulerButton_Static,
        InkToolbarStencilButton_Static,
        InkToolbarToggleButton_Static,
        InkToolbarToolButton_Static,
        ListBox_Static,
        ListBoxItem_Static,
        ListView_Static,
        ListViewBase_Static,
        ListViewItem_Static,
        MenuBarItemFlyout_Static,
        MenuFlyout_Static,
        NavigationViewItem_Static,
        NavigationViewItemBase_Static,
        NavigationViewItemHeader_Static,
        NavigationViewItemSeparator_Static,
        NavigationViewList_Static,
        ProgressBar_Static,
        RadioButton_Static,
        Slider_Static,
        TextCommandBarFlyout_Static,
        TreeViewItem_Static,
        TreeViewList_Static,
    )

if typing.TYPE_CHECKING:
    import winrt.windows.foundation as windows_foundation

__all__ = [
    "AppBarClosedDisplayMode",
    "AutoSuggestionBoxTextChangeReason",
    "BackgroundSizing",
    "CalendarViewDisplayMode",
    "CalendarViewSelectionMode",
    "CandidateWindowAlignment",
    "CharacterCasing",
    "ClickMode",
    "ColorPickerHsvChannel",
    "ColorSpectrumComponents",
    "ColorSpectrumShape",
    "ComboBoxSelectionChangedTrigger",
    "CommandBarDefaultLabelPosition",
    "CommandBarDynamicOverflowAction",
    "CommandBarLabelPosition",
    "CommandBarOverflowButtonVisibility",
    "ContentDialogButton",
    "ContentDialogPlacement",
    "ContentDialogResult",
    "ContentLinkChangeKind",
    "DisabledFormattingAccelerators",
    "HandwritingPanelPlacementAlignment",
    "IncrementalLoadingTrigger",
    "InkToolbarButtonFlyoutPlacement",
    "InkToolbarFlyoutItemKind",
    "InkToolbarInitialControls",
    "InkToolbarMenuKind",
    "InkToolbarStencilKind",
    "InkToolbarToggle",
    "InkToolbarTool",
    "ItemsUpdatingScrollMode",
    "LightDismissOverlayMode",
    "ListPickerFlyoutSelectionMode",
    "ListViewReorderMode",
    "ListViewSelectionMode",
    "NavigationViewBackButtonVisible",
    "NavigationViewDisplayMode",
    "NavigationViewOverflowLabelMode",
    "NavigationViewPaneDisplayMode",
    "NavigationViewSelectionFollowsFocus",
    "NavigationViewShoulderNavigationEnabled",
    "Orientation",
    "PanelScrollingDirection",
    "ParallaxSourceOffsetKind",
    "PasswordRevealMode",
    "PivotHeaderFocusVisualPlacement",
    "PivotSlideInAnimationGroup",
    "RefreshPullDirection",
    "RefreshVisualizerOrientation",
    "RefreshVisualizerState",
    "RequiresPointer",
    "RichEditClipboardFormat",
    "ScrollBarVisibility",
    "ScrollIntoViewAlignment",
    "ScrollMode",
    "SelectionMode",
    "SnapPointsType",
    "SplitViewDisplayMode",
    "SplitViewPanePlacement",
    "StretchDirection",
    "SwipeBehaviorOnInvoked",
    "SwipeMode",
    "Symbol",
    "TreeViewSelectionMode",
    "TwoPaneViewMode",
    "TwoPaneViewPriority",
    "TwoPaneViewTallModeConfiguration",
    "TwoPaneViewWideModeConfiguration",
    "VirtualizationMode",
    "WebViewExecutionMode",
    "WebViewPermissionState",
    "WebViewPermissionType",
    "ZoomMode",
    "AnchorRequestedEventArgs",
    "AppBar",
    "AppBarButton",
    "AppBarElementContainer",
    "AppBarSeparator",
    "AppBarToggleButton",
    "AutoSuggestBox",
    "AutoSuggestBoxQuerySubmittedEventArgs",
    "AutoSuggestBoxSuggestionChosenEventArgs",
    "AutoSuggestBoxTextChangedEventArgs",
    "BackClickEventArgs",
    "BitmapIcon",
    "BitmapIconSource",
    "Border",
    "Button",
    "CalendarDatePicker",
    "CalendarDatePickerDateChangedEventArgs",
    "CalendarView",
    "CalendarViewDayItem",
    "CalendarViewDayItemChangingEventArgs",
    "CalendarViewSelectedDatesChangedEventArgs",
    "CandidateWindowBoundsChangedEventArgs",
    "Canvas",
    "CaptureElement",
    "CheckBox",
    "ChoosingGroupHeaderContainerEventArgs",
    "ChoosingItemContainerEventArgs",
    "CleanUpVirtualizedItemEventArgs",
    "ColorChangedEventArgs",
    "ColorPicker",
    "ColumnDefinition",
    "ColumnDefinitionCollection",
    "ComboBox",
    "ComboBoxItem",
    "ComboBoxTextSubmittedEventArgs",
    "CommandBar",
    "CommandBarFlyout",
    "CommandBarOverflowPresenter",
    "ContainerContentChangingEventArgs",
    "ContentControl",
    "ContentDialog",
    "ContentDialogButtonClickDeferral",
    "ContentDialogButtonClickEventArgs",
    "ContentDialogClosedEventArgs",
    "ContentDialogClosingDeferral",
    "ContentDialogClosingEventArgs",
    "ContentDialogOpenedEventArgs",
    "ContentLinkChangedEventArgs",
    "ContentPresenter",
    "ContextMenuEventArgs",
    "Control",
    "ControlTemplate",
    "DataTemplateSelector",
    "DatePickedEventArgs",
    "DatePicker",
    "DatePickerFlyout",
    "DatePickerFlyoutItem",
    "DatePickerFlyoutPresenter",
    "DatePickerSelectedValueChangedEventArgs",
    "DatePickerValueChangedEventArgs",
    "DragItemsCompletedEventArgs",
    "DragItemsStartingEventArgs",
    "DropDownButton",
    "DropDownButtonAutomationPeer",
    "DynamicOverflowItemsChangingEventArgs",
    "FlipView",
    "FlipViewItem",
    "Flyout",
    "FlyoutPresenter",
    "FocusDisengagedEventArgs",
    "FocusEngagedEventArgs",
    "FontIcon",
    "FontIconSource",
    "Frame",
    "Grid",
    "GridView",
    "GridViewHeaderItem",
    "GridViewItem",
    "GroupItem",
    "GroupStyle",
    "GroupStyleSelector",
    "HandwritingPanelClosedEventArgs",
    "HandwritingPanelOpenedEventArgs",
    "HandwritingView",
    "HandwritingViewCandidatesChangedEventArgs",
    "HandwritingViewTextSubmittedEventArgs",
    "Hub",
    "HubSection",
    "HubSectionCollection",
    "HubSectionHeaderClickEventArgs",
    "HyperlinkButton",
    "IconElement",
    "IconSource",
    "IconSourceElement",
    "Image",
    "InkCanvas",
    "InkToolbar",
    "InkToolbarBallpointPenButton",
    "InkToolbarCustomPen",
    "InkToolbarCustomPenButton",
    "InkToolbarCustomToggleButton",
    "InkToolbarCustomToolButton",
    "InkToolbarEraserButton",
    "InkToolbarFlyoutItem",
    "InkToolbarHighlighterButton",
    "InkToolbarIsStencilButtonCheckedChangedEventArgs",
    "InkToolbarMenuButton",
    "InkToolbarPenButton",
    "InkToolbarPenConfigurationControl",
    "InkToolbarPencilButton",
    "InkToolbarRulerButton",
    "InkToolbarStencilButton",
    "InkToolbarToggleButton",
    "InkToolbarToolButton",
    "IsTextTrimmedChangedEventArgs",
    "ItemClickEventArgs",
    "ItemCollection",
    "ItemContainerGenerator",
    "ItemsControl",
    "ItemsPanelTemplate",
    "ItemsPickedEventArgs",
    "ItemsPresenter",
    "ItemsStackPanel",
    "ItemsWrapGrid",
    "ListBox",
    "ListBoxItem",
    "ListPickerFlyout",
    "ListPickerFlyoutPresenter",
    "ListView",
    "ListViewBase",
    "ListViewBaseHeaderItem",
    "ListViewHeaderItem",
    "ListViewItem",
    "ListViewPersistenceHelper",
    "MediaElement",
    "MediaPlayerElement",
    "MediaPlayerPresenter",
    "MediaTransportControls",
    "MediaTransportControlsHelper",
    "MenuBar",
    "MenuBarItem",
    "MenuBarItemFlyout",
    "MenuFlyout",
    "MenuFlyoutItem",
    "MenuFlyoutItemBase",
    "MenuFlyoutPresenter",
    "MenuFlyoutSeparator",
    "MenuFlyoutSubItem",
    "NavigationView",
    "NavigationViewBackRequestedEventArgs",
    "NavigationViewDisplayModeChangedEventArgs",
    "NavigationViewItem",
    "NavigationViewItemBase",
    "NavigationViewItemHeader",
    "NavigationViewItemInvokedEventArgs",
    "NavigationViewItemSeparator",
    "NavigationViewList",
    "NavigationViewPaneClosingEventArgs",
    "NavigationViewSelectionChangedEventArgs",
    "NavigationViewTemplateSettings",
    "NotifyEventArgs",
    "Page",
    "Panel",
    "ParallaxView",
    "PasswordBox",
    "PasswordBoxPasswordChangingEventArgs",
    "PathIcon",
    "PathIconSource",
    "PersonPicture",
    "PickerConfirmedEventArgs",
    "PickerFlyout",
    "PickerFlyoutPresenter",
    "Pivot",
    "PivotItem",
    "PivotItemEventArgs",
    "ProgressBar",
    "ProgressRing",
    "RadioButton",
    "RatingControl",
    "RatingItemFontInfo",
    "RatingItemImageInfo",
    "RatingItemInfo",
    "RefreshContainer",
    "RefreshInteractionRatioChangedEventArgs",
    "RefreshRequestedEventArgs",
    "RefreshStateChangedEventArgs",
    "RefreshVisualizer",
    "RelativePanel",
    "RichEditBox",
    "RichEditBoxSelectionChangingEventArgs",
    "RichEditBoxTextChangingEventArgs",
    "RichTextBlock",
    "RichTextBlockOverflow",
    "RowDefinition",
    "RowDefinitionCollection",
    "ScrollContentPresenter",
    "ScrollViewer",
    "ScrollViewerView",
    "ScrollViewerViewChangedEventArgs",
    "ScrollViewerViewChangingEventArgs",
    "SearchBox",
    "SearchBoxQueryChangedEventArgs",
    "SearchBoxQuerySubmittedEventArgs",
    "SearchBoxResultSuggestionChosenEventArgs",
    "SearchBoxSuggestionsRequestedEventArgs",
    "SectionsInViewChangedEventArgs",
    "SelectionChangedEventArgs",
    "SemanticZoom",
    "SemanticZoomLocation",
    "SemanticZoomViewChangedEventArgs",
    "SettingsFlyout",
    "Slider",
    "SplitButton",
    "SplitButtonAutomationPeer",
    "SplitButtonClickEventArgs",
    "SplitView",
    "SplitViewPaneClosingEventArgs",
    "StackPanel",
    "StyleSelector",
    "SwapChainBackgroundPanel",
    "SwapChainPanel",
    "SwipeControl",
    "SwipeItem",
    "SwipeItemInvokedEventArgs",
    "SwipeItems",
    "SymbolIcon",
    "SymbolIconSource",
    "TextBlock",
    "TextBox",
    "TextBoxBeforeTextChangingEventArgs",
    "TextBoxSelectionChangingEventArgs",
    "TextBoxTextChangingEventArgs",
    "TextChangedEventArgs",
    "TextCommandBarFlyout",
    "TextCompositionChangedEventArgs",
    "TextCompositionEndedEventArgs",
    "TextCompositionStartedEventArgs",
    "TextControlCopyingToClipboardEventArgs",
    "TextControlCuttingToClipboardEventArgs",
    "TextControlPasteEventArgs",
    "TimePickedEventArgs",
    "TimePicker",
    "TimePickerFlyout",
    "TimePickerFlyoutPresenter",
    "TimePickerSelectedValueChangedEventArgs",
    "TimePickerValueChangedEventArgs",
    "ToggleMenuFlyoutItem",
    "ToggleSplitButton",
    "ToggleSplitButtonAutomationPeer",
    "ToggleSplitButtonIsCheckedChangedEventArgs",
    "ToggleSwitch",
    "ToolTip",
    "ToolTipService",
    "TreeView",
    "TreeViewCollapsedEventArgs",
    "TreeViewDragItemsCompletedEventArgs",
    "TreeViewDragItemsStartingEventArgs",
    "TreeViewExpandingEventArgs",
    "TreeViewItem",
    "TreeViewItemInvokedEventArgs",
    "TreeViewItemTemplateSettings",
    "TreeViewList",
    "TreeViewNode",
    "TwoPaneView",
    "UIElementCollection",
    "UserControl",
    "VariableSizedWrapGrid",
    "Viewbox",
    "VirtualizingPanel",
    "VirtualizingStackPanel",
    "WebView",
    "WebViewBrush",
    "WebViewContentLoadingEventArgs",
    "WebViewDOMContentLoadedEventArgs",
    "WebViewDeferredPermissionRequest",
    "WebViewLongRunningScriptDetectedEventArgs",
    "WebViewNavigationCompletedEventArgs",
    "WebViewNavigationFailedEventArgs",
    "WebViewNavigationStartingEventArgs",
    "WebViewNewWindowRequestedEventArgs",
    "WebViewPermissionRequest",
    "WebViewPermissionRequestedEventArgs",
    "WebViewSeparateProcessLostEventArgs",
    "WebViewSettings",
    "WebViewUnsupportedUriSchemeIdentifiedEventArgs",
    "WebViewUnviewableContentIdentifiedEventArgs",
    "WebViewWebResourceRequestedEventArgs",
    "WrapGrid",
    "ICommandBarElement",
    "ICommandBarElement2",
    "IInsertionPanel",
    "IItemContainerMapping",
    "INavigate",
    "IScrollAnchorProvider",
    "ISemanticZoomInformation",
    "BackClickEventHandler",
    "CalendarViewDayItemChangingEventHandler",
    "CleanUpVirtualizedItemEventHandler",
    "ContextMenuOpeningEventHandler",
    "DragItemsStartingEventHandler",
    "HubSectionHeaderClickEventHandler",
    "ItemClickEventHandler",
    "ListViewItemToKeyHandler",
    "ListViewKeyToItemHandler",
    "NotifyEventHandler",
    "SectionsInViewChangedEventHandler",
    "SelectionChangedEventHandler",
    "SemanticZoomViewChangedEventHandler",
    "TextChangedEventHandler",
    "TextControlPasteEventHandler",
    "WebViewNavigationFailedEventHandler",
]

class AppBarClosedDisplayMode(enum.IntEnum):
    COMPACT = 0
    MINIMAL = 1
    HIDDEN = 2

class AutoSuggestionBoxTextChangeReason(enum.IntEnum):
    USER_INPUT = 0
    PROGRAMMATIC_CHANGE = 1
    SUGGESTION_CHOSEN = 2

class BackgroundSizing(enum.IntEnum):
    INNER_BORDER_EDGE = 0
    OUTER_BORDER_EDGE = 1

class CalendarViewDisplayMode(enum.IntEnum):
    MONTH = 0
    YEAR = 1
    DECADE = 2

class CalendarViewSelectionMode(enum.IntEnum):
    NONE = 0
    SINGLE = 1
    MULTIPLE = 2

class CandidateWindowAlignment(enum.IntEnum):
    DEFAULT = 0
    BOTTOM_EDGE = 1

class CharacterCasing(enum.IntEnum):
    NORMAL = 0
    LOWER = 1
    UPPER = 2

class ClickMode(enum.IntEnum):
    RELEASE = 0
    PRESS = 1
    HOVER = 2

class ColorPickerHsvChannel(enum.IntEnum):
    HUE = 0
    SATURATION = 1
    VALUE = 2
    ALPHA = 3

class ColorSpectrumComponents(enum.IntEnum):
    HUE_VALUE = 0
    VALUE_HUE = 1
    HUE_SATURATION = 2
    SATURATION_HUE = 3
    SATURATION_VALUE = 4
    VALUE_SATURATION = 5

class ColorSpectrumShape(enum.IntEnum):
    BOX = 0
    RING = 1

class ComboBoxSelectionChangedTrigger(enum.IntEnum):
    COMMITTED = 0
    ALWAYS = 1

class CommandBarDefaultLabelPosition(enum.IntEnum):
    BOTTOM = 0
    RIGHT = 1
    COLLAPSED = 2

class CommandBarDynamicOverflowAction(enum.IntEnum):
    ADDING_TO_OVERFLOW = 0
    REMOVING_FROM_OVERFLOW = 1

class CommandBarLabelPosition(enum.IntEnum):
    DEFAULT = 0
    COLLAPSED = 1

class CommandBarOverflowButtonVisibility(enum.IntEnum):
    AUTO = 0
    VISIBLE = 1
    COLLAPSED = 2

class ContentDialogButton(enum.IntEnum):
    NONE = 0
    PRIMARY = 1
    SECONDARY = 2
    CLOSE = 3

class ContentDialogPlacement(enum.IntEnum):
    POPUP = 0
    IN_PLACE = 1

class ContentDialogResult(enum.IntEnum):
    NONE = 0
    PRIMARY = 1
    SECONDARY = 2

class ContentLinkChangeKind(enum.IntEnum):
    INSERTED = 0
    REMOVED = 1
    EDITED = 2

class DisabledFormattingAccelerators(enum.IntFlag):
    NONE = 0x0
    BOLD = 0x1
    ITALIC = 0x2
    UNDERLINE = 0x4
    ALL = 0xFFFFFFFF

class HandwritingPanelPlacementAlignment(enum.IntEnum):
    AUTO = 0
    TOP_LEFT = 1
    TOP_RIGHT = 2
    BOTTOM_LEFT = 3
    BOTTOM_RIGHT = 4

class IncrementalLoadingTrigger(enum.IntEnum):
    NONE = 0
    EDGE = 1

class InkToolbarButtonFlyoutPlacement(enum.IntEnum):
    AUTO = 0
    TOP = 1
    BOTTOM = 2
    LEFT = 3
    RIGHT = 4

class InkToolbarFlyoutItemKind(enum.IntEnum):
    SIMPLE = 0
    RADIO = 1
    CHECK = 2
    RADIO_CHECK = 3

class InkToolbarInitialControls(enum.IntEnum):
    ALL = 0
    NONE = 1
    PENS_ONLY = 2
    ALL_EXCEPT_PENS = 3

class InkToolbarMenuKind(enum.IntEnum):
    STENCIL = 0

class InkToolbarStencilKind(enum.IntEnum):
    RULER = 0
    PROTRACTOR = 1

class InkToolbarToggle(enum.IntEnum):
    RULER = 0
    CUSTOM = 1

class InkToolbarTool(enum.IntEnum):
    BALLPOINT_PEN = 0
    PENCIL = 1
    HIGHLIGHTER = 2
    ERASER = 3
    CUSTOM_PEN = 4
    CUSTOM_TOOL = 5

class ItemsUpdatingScrollMode(enum.IntEnum):
    KEEP_ITEMS_IN_VIEW = 0
    KEEP_SCROLL_OFFSET = 1
    KEEP_LAST_ITEM_IN_VIEW = 2

class LightDismissOverlayMode(enum.IntEnum):
    AUTO = 0
    ON = 1
    OFF = 2

class ListPickerFlyoutSelectionMode(enum.IntEnum):
    SINGLE = 0
    MULTIPLE = 1

class ListViewReorderMode(enum.IntEnum):
    DISABLED = 0
    ENABLED = 1

class ListViewSelectionMode(enum.IntEnum):
    NONE = 0
    SINGLE = 1
    MULTIPLE = 2
    EXTENDED = 3

class NavigationViewBackButtonVisible(enum.IntEnum):
    COLLAPSED = 0
    VISIBLE = 1
    AUTO = 2

class NavigationViewDisplayMode(enum.IntEnum):
    MINIMAL = 0
    COMPACT = 1
    EXPANDED = 2

class NavigationViewOverflowLabelMode(enum.IntEnum):
    MORE_LABEL = 0
    NO_LABEL = 1

class NavigationViewPaneDisplayMode(enum.IntEnum):
    AUTO = 0
    LEFT = 1
    TOP = 2
    LEFT_COMPACT = 3
    LEFT_MINIMAL = 4

class NavigationViewSelectionFollowsFocus(enum.IntEnum):
    DISABLED = 0
    ENABLED = 1

class NavigationViewShoulderNavigationEnabled(enum.IntEnum):
    WHEN_SELECTION_FOLLOWS_FOCUS = 0
    ALWAYS = 1
    NEVER = 2

class Orientation(enum.IntEnum):
    VERTICAL = 0
    HORIZONTAL = 1

class PanelScrollingDirection(enum.IntEnum):
    NONE = 0
    FORWARD = 1
    BACKWARD = 2

class ParallaxSourceOffsetKind(enum.IntEnum):
    ABSOLUTE = 0
    RELATIVE = 1

class PasswordRevealMode(enum.IntEnum):
    PEEK = 0
    HIDDEN = 1
    VISIBLE = 2

class PivotHeaderFocusVisualPlacement(enum.IntEnum):
    ITEM_HEADERS = 0
    SELECTED_ITEM_HEADER = 1

class PivotSlideInAnimationGroup(enum.IntEnum):
    DEFAULT = 0
    GROUP_ONE = 1
    GROUP_TWO = 2
    GROUP_THREE = 3

class RefreshPullDirection(enum.IntEnum):
    LEFT_TO_RIGHT = 0
    TOP_TO_BOTTOM = 1
    RIGHT_TO_LEFT = 2
    BOTTOM_TO_TOP = 3

class RefreshVisualizerOrientation(enum.IntEnum):
    AUTO = 0
    NORMAL = 1
    ROTATE90_DEGREES_COUNTERCLOCKWISE = 2
    ROTATE270_DEGREES_COUNTERCLOCKWISE = 3

class RefreshVisualizerState(enum.IntEnum):
    IDLE = 0
    PEEKING = 1
    INTERACTING = 2
    PENDING = 3
    REFRESHING = 4

class RequiresPointer(enum.IntEnum):
    NEVER = 0
    WHEN_ENGAGED = 1
    WHEN_FOCUSED = 2

class RichEditClipboardFormat(enum.IntEnum):
    ALL_FORMATS = 0
    PLAIN_TEXT = 1

class ScrollBarVisibility(enum.IntEnum):
    DISABLED = 0
    AUTO = 1
    HIDDEN = 2
    VISIBLE = 3

class ScrollIntoViewAlignment(enum.IntEnum):
    DEFAULT = 0
    LEADING = 1

class ScrollMode(enum.IntEnum):
    DISABLED = 0
    ENABLED = 1
    AUTO = 2

class SelectionMode(enum.IntEnum):
    SINGLE = 0
    MULTIPLE = 1
    EXTENDED = 2

class SnapPointsType(enum.IntEnum):
    NONE = 0
    OPTIONAL = 1
    MANDATORY = 2
    OPTIONAL_SINGLE = 3
    MANDATORY_SINGLE = 4

class SplitViewDisplayMode(enum.IntEnum):
    OVERLAY = 0
    INLINE = 1
    COMPACT_OVERLAY = 2
    COMPACT_INLINE = 3

class SplitViewPanePlacement(enum.IntEnum):
    LEFT = 0
    RIGHT = 1

class StretchDirection(enum.IntEnum):
    UP_ONLY = 0
    DOWN_ONLY = 1
    BOTH = 2

class SwipeBehaviorOnInvoked(enum.IntEnum):
    AUTO = 0
    CLOSE = 1
    REMAIN_OPEN = 2

class SwipeMode(enum.IntEnum):
    REVEAL = 0
    EXECUTE = 1

class Symbol(enum.IntEnum):
    PREVIOUS = 57600
    NEXT = 57601
    PLAY = 57602
    PAUSE = 57603
    EDIT = 57604
    SAVE = 57605
    CLEAR = 57606
    DELETE = 57607
    REMOVE = 57608
    ADD = 57609
    CANCEL = 57610
    ACCEPT = 57611
    MORE = 57612
    REDO = 57613
    UNDO = 57614
    HOME = 57615
    UP = 57616
    FORWARD = 57617
    BACK = 57618
    FAVORITE = 57619
    CAMERA = 57620
    SETTING = 57621
    VIDEO = 57622
    SYNC = 57623
    DOWNLOAD = 57624
    MAIL = 57625
    FIND = 57626
    HELP = 57627
    UPLOAD = 57628
    EMOJI = 57629
    TWO_PAGE = 57630
    LEAVE_CHAT = 57631
    MAIL_FORWARD = 57632
    CLOCK = 57633
    SEND = 57634
    CROP = 57635
    ROTATE_CAMERA = 57636
    PEOPLE = 57637
    OPEN_PANE = 57638
    CLOSE_PANE = 57639
    WORLD = 57640
    FLAG = 57641
    PREVIEW_LINK = 57642
    GLOBE = 57643
    TRIM = 57644
    ATTACH_CAMERA = 57645
    ZOOM_IN = 57646
    BOOKMARKS = 57647
    DOCUMENT = 57648
    PROTECTED_DOCUMENT = 57649
    PAGE = 57650
    BULLETS = 57651
    COMMENT = 57652
    MAIL_FILLED = 57653
    CONTACT_INFO = 57654
    HANG_UP = 57655
    VIEW_ALL = 57656
    MAP_PIN = 57657
    PHONE = 57658
    VIDEO_CHAT = 57659
    SWITCH = 57660
    CONTACT = 57661
    RENAME = 57662
    PIN = 57665
    MUSIC_INFO = 57666
    GO = 57667
    KEYBOARD = 57668
    DOCK_LEFT = 57669
    DOCK_RIGHT = 57670
    DOCK_BOTTOM = 57671
    REMOTE = 57672
    REFRESH = 57673
    ROTATE = 57674
    SHUFFLE = 57675
    LIST = 57676
    SHOP = 57677
    SELECT_ALL = 57678
    ORIENTATION = 57679
    IMPORT = 57680
    IMPORT_ALL = 57681
    BROWSE_PHOTOS = 57685
    WEB_CAM = 57686
    PICTURES = 57688
    SAVE_LOCAL = 57689
    CAPTION = 57690
    STOP = 57691
    SHOW_RESULTS = 57692
    VOLUME = 57693
    REPAIR = 57694
    MESSAGE = 57695
    PAGE2 = 57696
    CALENDAR_DAY = 57697
    CALENDAR_WEEK = 57698
    CALENDAR = 57699
    CHARACTER = 57700
    MAIL_REPLY_ALL = 57701
    READ = 57702
    LINK = 57703
    ACCOUNT = 57704
    SHOW_BCC = 57705
    HIDE_BCC = 57706
    CUT = 57707
    ATTACH = 57708
    PASTE = 57709
    FILTER = 57710
    COPY = 57711
    EMOJI2 = 57712
    IMPORTANT = 57713
    MAIL_REPLY = 57714
    SLIDE_SHOW = 57715
    SORT = 57716
    MANAGE = 57720
    ALL_APPS = 57721
    DISCONNECT_DRIVE = 57722
    MAP_DRIVE = 57723
    NEW_WINDOW = 57724
    OPEN_WITH = 57725
    CONTACT_PRESENCE = 57729
    PRIORITY = 57730
    GO_TO_TODAY = 57732
    FONT = 57733
    FONT_COLOR = 57734
    CONTACT2 = 57735
    FOLDER = 57736
    AUDIO = 57737
    PLACEHOLDER = 57738
    VIEW = 57739
    SET_LOCK_SCREEN = 57740
    SET_TILE = 57741
    CLOSED_CAPTION = 57744
    STOP_SLIDE_SHOW = 57745
    PERMISSIONS = 57746
    HIGHLIGHT = 57747
    DISABLE_UPDATES = 57748
    UN_FAVORITE = 57749
    UN_PIN = 57750
    OPEN_LOCAL = 57751
    MUTE = 57752
    ITALIC = 57753
    UNDERLINE = 57754
    BOLD = 57755
    MOVE_TO_FOLDER = 57756
    LIKE_DISLIKE = 57757
    DISLIKE = 57758
    LIKE = 57759
    ALIGN_RIGHT = 57760
    ALIGN_CENTER = 57761
    ALIGN_LEFT = 57762
    ZOOM = 57763
    ZOOM_OUT = 57764
    OPEN_FILE = 57765
    OTHER_USER = 57766
    ADMIN = 57767
    STREET = 57795
    MAP = 57796
    CLEAR_SELECTION = 57797
    FONT_DECREASE = 57798
    FONT_INCREASE = 57799
    FONT_SIZE = 57800
    CELL_PHONE = 57801
    RE_SHARE = 57802
    TAG = 57803
    REPEAT_ONE = 57804
    REPEAT_ALL = 57805
    OUTLINE_STAR = 57806
    SOLID_STAR = 57807
    CALCULATOR = 57808
    DIRECTIONS = 57809
    TARGET = 57810
    LIBRARY = 57811
    PHONE_BOOK = 57812
    MEMO = 57813
    MICROPHONE = 57814
    POST_UPDATE = 57815
    BACK_TO_WINDOW = 57816
    FULL_SCREEN = 57817
    NEW_FOLDER = 57818
    CALENDAR_REPLY = 57819
    UN_SYNC_FOLDER = 57821
    REPORT_HACKED = 57822
    SYNC_FOLDER = 57823
    BLOCK_CONTACT = 57824
    SWITCH_APPS = 57825
    ADD_FRIEND = 57826
    TOUCH_POINTER = 57827
    GO_TO_START = 57828
    ZERO_BARS = 57829
    ONE_BAR = 57830
    TWO_BARS = 57831
    THREE_BARS = 57832
    FOUR_BARS = 57833
    SCAN = 58004
    PREVIEW = 58005
    GLOBAL_NAVIGATION_BUTTON = 59136
    SHARE = 59181
    PRINT = 59209
    XBOX_ONE_CONSOLE = 59792

class TreeViewSelectionMode(enum.IntEnum):
    NONE = 0
    SINGLE = 1
    MULTIPLE = 2

class TwoPaneViewMode(enum.IntEnum):
    SINGLE_PANE = 0
    WIDE = 1
    TALL = 2

class TwoPaneViewPriority(enum.IntEnum):
    PANE1 = 0
    PANE2 = 1

class TwoPaneViewTallModeConfiguration(enum.IntEnum):
    SINGLE_PANE = 0
    TOP_BOTTOM = 1
    BOTTOM_TOP = 2

class TwoPaneViewWideModeConfiguration(enum.IntEnum):
    SINGLE_PANE = 0
    LEFT_RIGHT = 1
    RIGHT_LEFT = 2

class VirtualizationMode(enum.IntEnum):
    STANDARD = 0
    RECYCLING = 1

class WebViewExecutionMode(enum.IntEnum):
    SAME_THREAD = 0
    SEPARATE_THREAD = 1
    SEPARATE_PROCESS = 2

class WebViewPermissionState(enum.IntEnum):
    UNKNOWN = 0
    DEFER = 1
    ALLOW = 2
    DENY = 3

class WebViewPermissionType(enum.IntEnum):
    GEOLOCATION = 0
    UNLIMITED_INDEXED_DB_QUOTA = 1
    MEDIA = 2
    POINTER_LOCK = 3
    WEB_NOTIFICATIONS = 4
    SCREEN = 5
    IMMERSIVE_VIEW = 6

class ZoomMode(enum.IntEnum):
    DISABLED = 0
    ENABLED = 1

winrt.runtime._internals.mixin_mutable_sequence(ColumnDefinitionCollection)
winrt.runtime._internals.mixin_mutable_sequence(HubSectionCollection)
winrt.runtime._internals.mixin_mutable_sequence(ItemCollection)
winrt.runtime._internals.mixin_mutable_sequence(RowDefinitionCollection)
winrt.runtime._internals.mixin_mutable_sequence(SwipeItems)
winrt.runtime._internals.mixin_mutable_sequence(UIElementCollection)
BackClickEventHandler = typing.Callable[[winrt.system.Object, BackClickEventArgs], None]
CalendarViewDayItemChangingEventHandler = typing.Callable[[CalendarView, CalendarViewDayItemChangingEventArgs], None]
CleanUpVirtualizedItemEventHandler = typing.Callable[[winrt.system.Object, CleanUpVirtualizedItemEventArgs], None]
ContextMenuOpeningEventHandler = typing.Callable[[winrt.system.Object, ContextMenuEventArgs], None]
DragItemsStartingEventHandler = typing.Callable[[winrt.system.Object, DragItemsStartingEventArgs], None]
HubSectionHeaderClickEventHandler = typing.Callable[[winrt.system.Object, HubSectionHeaderClickEventArgs], None]
ItemClickEventHandler = typing.Callable[[winrt.system.Object, ItemClickEventArgs], None]
ListViewItemToKeyHandler = typing.Callable[[winrt.system.Object], str]
ListViewKeyToItemHandler = typing.Callable[[str], "windows_foundation.IAsyncOperation[winrt.system.Object]"]
NotifyEventHandler = typing.Callable[[winrt.system.Object, NotifyEventArgs], None]
SectionsInViewChangedEventHandler = typing.Callable[[winrt.system.Object, SectionsInViewChangedEventArgs], None]
SelectionChangedEventHandler = typing.Callable[[winrt.system.Object, SelectionChangedEventArgs], None]
SemanticZoomViewChangedEventHandler = typing.Callable[[winrt.system.Object, SemanticZoomViewChangedEventArgs], None]
TextChangedEventHandler = typing.Callable[[winrt.system.Object, TextChangedEventArgs], None]
TextControlPasteEventHandler = typing.Callable[[winrt.system.Object, TextControlPasteEventArgs], None]
WebViewNavigationFailedEventHandler = typing.Callable[[winrt.system.Object, WebViewNavigationFailedEventArgs], None]
