// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_ApplicationModel_DataTransfer_DragDrop_Core_0_H
#define WINRT_Windows_ApplicationModel_DataTransfer_DragDrop_Core_0_H
WINRT_EXPORT namespace winrt::Windows::ApplicationModel::DataTransfer
{
    struct DataPackage;
    enum class DataPackageOperation : uint32_t;
    struct DataPackageView;
}
WINRT_EXPORT namespace winrt::Windows::ApplicationModel::DataTransfer::DragDrop
{
    enum class DragDropModifiers : uint32_t;
}
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    struct EventRegistrationToken;
    struct IAsyncAction;
    struct Point;
    template <typename TSender, typename TResult> struct WINRT_IMPL_EMPTY_BASES TypedEventHandler;
}
WINRT_EXPORT namespace winrt::Windows::Graphics::Imaging
{
    struct SoftwareBitmap;
}
WINRT_EXPORT namespace winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core
{
    enum class CoreDragUIContentMode : uint32_t
    {
        Auto = 0,
        Deferred = 0x1,
    };
    struct ICoreDragDropManager;
    struct ICoreDragDropManagerStatics;
    struct ICoreDragInfo;
    struct ICoreDragInfo2;
    struct ICoreDragOperation;
    struct ICoreDragOperation2;
    struct ICoreDragUIOverride;
    struct ICoreDropOperationTarget;
    struct ICoreDropOperationTargetRequestedEventArgs;
    struct CoreDragDropManager;
    struct CoreDragInfo;
    struct CoreDragOperation;
    struct CoreDragUIOverride;
    struct CoreDropOperationTargetRequestedEventArgs;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManager>{ using type = interface_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManagerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo>{ using type = interface_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo2>{ using type = interface_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation>{ using type = interface_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation2>{ using type = interface_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragUIOverride>{ using type = interface_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTarget>{ using type = interface_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTargetRequestedEventArgs>{ using type = interface_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragDropManager>{ using type = class_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragInfo>{ using type = class_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragOperation>{ using type = class_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragUIOverride>{ using type = class_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDropOperationTargetRequestedEventArgs>{ using type = class_category; };
    template <> struct category<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragUIContentMode>{ using type = enum_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragDropManager> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.CoreDragDropManager";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragInfo> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.CoreDragInfo";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragOperation> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.CoreDragOperation";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragUIOverride> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.CoreDragUIOverride";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDropOperationTargetRequestedEventArgs> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.CoreDropOperationTargetRequestedEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragUIContentMode> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.CoreDragUIContentMode";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManager> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.ICoreDragDropManager";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManagerStatics> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.ICoreDragDropManagerStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.ICoreDragInfo";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo2> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.ICoreDragInfo2";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.ICoreDragOperation";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation2> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.ICoreDragOperation2";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragUIOverride> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.ICoreDragUIOverride";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTarget> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.ICoreDropOperationTarget";
    template <> inline constexpr auto& name_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTargetRequestedEventArgs> = L"Windows.ApplicationModel.DataTransfer.DragDrop.Core.ICoreDropOperationTargetRequestedEventArgs";
    template <> inline constexpr guid guid_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManager>{ 0x7D56D344,0x8464,0x4FAF,{ 0xAA,0x49,0x37,0xEA,0x6E,0x2D,0x7B,0xD1 } }; // 7D56D344-8464-4FAF-AA49-37EA6E2D7BD1
    template <> inline constexpr guid guid_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManagerStatics>{ 0x9542FDCA,0xDA12,0x4C1C,{ 0x8D,0x06,0x04,0x1D,0xB2,0x97,0x33,0xC3 } }; // 9542FDCA-DA12-4C1C-8D06-041DB29733C3
    template <> inline constexpr guid guid_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo>{ 0x48353A8B,0xCB50,0x464E,{ 0x95,0x75,0xCD,0x4E,0x3A,0x7A,0xB0,0x28 } }; // 48353A8B-CB50-464E-9575-CD4E3A7AB028
    template <> inline constexpr guid guid_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo2>{ 0xC54691E5,0xE6FB,0x4D74,{ 0xB4,0xB1,0x8A,0x3C,0x17,0xF2,0x5E,0x9E } }; // C54691E5-E6FB-4D74-B4B1-8A3C17F25E9E
    template <> inline constexpr guid guid_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation>{ 0xCC06DE4F,0x6DB0,0x4E62,{ 0xAB,0x1B,0xA7,0x4A,0x02,0xDC,0x6D,0x85 } }; // CC06DE4F-6DB0-4E62-AB1B-A74A02DC6D85
    template <> inline constexpr guid guid_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation2>{ 0x824B1E2C,0xD99A,0x4FC3,{ 0x85,0x07,0x6C,0x18,0x2F,0x33,0xB4,0x6A } }; // 824B1E2C-D99A-4FC3-8507-6C182F33B46A
    template <> inline constexpr guid guid_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragUIOverride>{ 0x89A85064,0x3389,0x4F4F,{ 0x88,0x97,0x7E,0x8A,0x3F,0xFB,0x3C,0x93 } }; // 89A85064-3389-4F4F-8897-7E8A3FFB3C93
    template <> inline constexpr guid guid_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTarget>{ 0xD9126196,0x4C5B,0x417D,{ 0xBB,0x37,0x76,0x38,0x1D,0xEF,0x8D,0xB4 } }; // D9126196-4C5B-417D-BB37-76381DEF8DB4
    template <> inline constexpr guid guid_v<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTargetRequestedEventArgs>{ 0x2ACA929A,0x5E28,0x4EA6,{ 0x82,0x9E,0x29,0x13,0x4E,0x66,0x5D,0x6D } }; // 2ACA929A-5E28-4EA6-829E-29134E665D6D
    template <> struct default_interface<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragDropManager>{ using type = winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManager; };
    template <> struct default_interface<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragInfo>{ using type = winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo; };
    template <> struct default_interface<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragOperation>{ using type = winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation; };
    template <> struct default_interface<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragUIOverride>{ using type = winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragUIOverride; };
    template <> struct default_interface<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDropOperationTargetRequestedEventArgs>{ using type = winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTargetRequestedEventArgs; };
    template <> struct abi<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManager>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall add_TargetRequested(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_TargetRequested(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall get_AreConcurrentOperationsEnabled(bool*) noexcept = 0;
            virtual int32_t __stdcall put_AreConcurrentOperationsEnabled(bool) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManagerStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetForCurrentView(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Data(void**) noexcept = 0;
            virtual int32_t __stdcall get_Modifiers(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Position(winrt::Windows::Foundation::Point*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo2>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_AllowedOperations(uint32_t*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Data(void**) noexcept = 0;
            virtual int32_t __stdcall SetPointerId(uint32_t) noexcept = 0;
            virtual int32_t __stdcall SetDragUIContentFromSoftwareBitmap(void*) noexcept = 0;
            virtual int32_t __stdcall SetDragUIContentFromSoftwareBitmapWithAnchorPoint(void*, winrt::Windows::Foundation::Point) noexcept = 0;
            virtual int32_t __stdcall get_DragUIContentMode(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall put_DragUIContentMode(uint32_t) noexcept = 0;
            virtual int32_t __stdcall StartAsync(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation2>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_AllowedOperations(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall put_AllowedOperations(uint32_t) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragUIOverride>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall SetContentFromSoftwareBitmap(void*) noexcept = 0;
            virtual int32_t __stdcall SetContentFromSoftwareBitmapWithAnchorPoint(void*, winrt::Windows::Foundation::Point) noexcept = 0;
            virtual int32_t __stdcall get_IsContentVisible(bool*) noexcept = 0;
            virtual int32_t __stdcall put_IsContentVisible(bool) noexcept = 0;
            virtual int32_t __stdcall get_Caption(void**) noexcept = 0;
            virtual int32_t __stdcall put_Caption(void*) noexcept = 0;
            virtual int32_t __stdcall get_IsCaptionVisible(bool*) noexcept = 0;
            virtual int32_t __stdcall put_IsCaptionVisible(bool) noexcept = 0;
            virtual int32_t __stdcall get_IsGlyphVisible(bool*) noexcept = 0;
            virtual int32_t __stdcall put_IsGlyphVisible(bool) noexcept = 0;
            virtual int32_t __stdcall Clear() noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTarget>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall EnterAsync(void*, void*, void**) noexcept = 0;
            virtual int32_t __stdcall OverAsync(void*, void*, void**) noexcept = 0;
            virtual int32_t __stdcall LeaveAsync(void*, void**) noexcept = 0;
            virtual int32_t __stdcall DropAsync(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTargetRequestedEventArgs>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall SetTarget(void*) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragDropManager
    {
        auto TargetRequested(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragDropManager, winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDropOperationTargetRequestedEventArgs> const& value) const;
        using TargetRequested_revoker = impl::event_revoker<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManager, &impl::abi_t<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManager>::remove_TargetRequested>;
        [[nodiscard]] auto TargetRequested(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragDropManager, winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDropOperationTargetRequestedEventArgs> const& value) const;
        auto TargetRequested(winrt::event_token const& value) const noexcept;
        [[nodiscard]] auto AreConcurrentOperationsEnabled() const;
        auto AreConcurrentOperationsEnabled(bool value) const;
    };
    template <> struct consume<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManager>
    {
        template <typename D> using type = consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragDropManager<D>;
    };
    template <typename D>
    struct consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragDropManagerStatics
    {
        auto GetForCurrentView() const;
    };
    template <> struct consume<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragDropManagerStatics>
    {
        template <typename D> using type = consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragDropManagerStatics<D>;
    };
    template <typename D>
    struct consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragInfo
    {
        [[nodiscard]] auto Data() const;
        [[nodiscard]] auto Modifiers() const;
        [[nodiscard]] auto Position() const;
    };
    template <> struct consume<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo>
    {
        template <typename D> using type = consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragInfo<D>;
    };
    template <typename D>
    struct consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragInfo2
    {
        [[nodiscard]] auto AllowedOperations() const;
    };
    template <> struct consume<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragInfo2>
    {
        template <typename D> using type = consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragInfo2<D>;
    };
    template <typename D>
    struct consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragOperation
    {
        [[nodiscard]] auto Data() const;
        auto SetPointerId(uint32_t pointerId) const;
        auto SetDragUIContentFromSoftwareBitmap(winrt::Windows::Graphics::Imaging::SoftwareBitmap const& softwareBitmap) const;
        auto SetDragUIContentFromSoftwareBitmap(winrt::Windows::Graphics::Imaging::SoftwareBitmap const& softwareBitmap, winrt::Windows::Foundation::Point const& anchorPoint) const;
        [[nodiscard]] auto DragUIContentMode() const;
        auto DragUIContentMode(winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragUIContentMode const& value) const;
        auto StartAsync() const;
    };
    template <> struct consume<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation>
    {
        template <typename D> using type = consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragOperation<D>;
    };
    template <typename D>
    struct consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragOperation2
    {
        [[nodiscard]] auto AllowedOperations() const;
        auto AllowedOperations(winrt::Windows::ApplicationModel::DataTransfer::DataPackageOperation const& value) const;
    };
    template <> struct consume<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragOperation2>
    {
        template <typename D> using type = consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragOperation2<D>;
    };
    template <typename D>
    struct consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragUIOverride
    {
        auto SetContentFromSoftwareBitmap(winrt::Windows::Graphics::Imaging::SoftwareBitmap const& softwareBitmap) const;
        auto SetContentFromSoftwareBitmap(winrt::Windows::Graphics::Imaging::SoftwareBitmap const& softwareBitmap, winrt::Windows::Foundation::Point const& anchorPoint) const;
        [[nodiscard]] auto IsContentVisible() const;
        auto IsContentVisible(bool value) const;
        [[nodiscard]] auto Caption() const;
        auto Caption(param::hstring const& value) const;
        [[nodiscard]] auto IsCaptionVisible() const;
        auto IsCaptionVisible(bool value) const;
        [[nodiscard]] auto IsGlyphVisible() const;
        auto IsGlyphVisible(bool value) const;
        auto Clear() const;
    };
    template <> struct consume<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDragUIOverride>
    {
        template <typename D> using type = consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDragUIOverride<D>;
    };
    template <typename D>
    struct consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDropOperationTarget
    {
        auto EnterAsync(winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragInfo const& dragInfo, winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragUIOverride const& dragUIOverride) const;
        auto OverAsync(winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragInfo const& dragInfo, winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragUIOverride const& dragUIOverride) const;
        auto LeaveAsync(winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragInfo const& dragInfo) const;
        auto DropAsync(winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::CoreDragInfo const& dragInfo) const;
    };
    template <> struct consume<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTarget>
    {
        template <typename D> using type = consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDropOperationTarget<D>;
    };
    template <typename D>
    struct consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDropOperationTargetRequestedEventArgs
    {
        auto SetTarget(winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTarget const& target) const;
    };
    template <> struct consume<winrt::Windows::ApplicationModel::DataTransfer::DragDrop::Core::ICoreDropOperationTargetRequestedEventArgs>
    {
        template <typename D> using type = consume_Windows_ApplicationModel_DataTransfer_DragDrop_Core_ICoreDropOperationTargetRequestedEventArgs<D>;
    };
}
#endif
