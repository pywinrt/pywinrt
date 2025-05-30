// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Storage_BulkAccess_0_H
#define WINRT_Windows_Storage_BulkAccess_0_H
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    struct EventRegistrationToken;
    template <typename TSender, typename TResult> struct WINRT_IMPL_EMPTY_BASES TypedEventHandler;
}
WINRT_EXPORT namespace winrt::Windows::Storage::FileProperties
{
    struct BasicProperties;
    struct DocumentProperties;
    struct ImageProperties;
    struct MusicProperties;
    struct StorageItemThumbnail;
    enum class ThumbnailMode : int32_t;
    enum class ThumbnailOptions : uint32_t;
    struct VideoProperties;
}
WINRT_EXPORT namespace winrt::Windows::Storage::Search
{
    struct IStorageQueryResultBase;
}
WINRT_EXPORT namespace winrt::Windows::Storage::BulkAccess
{
    struct IFileInformationFactory;
    struct IFileInformationFactoryFactory;
    struct IStorageItemInformation;
    struct FileInformation;
    struct FileInformationFactory;
    struct FolderInformation;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::Storage::BulkAccess::IFileInformationFactory>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Storage::BulkAccess::IFileInformationFactoryFactory>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Storage::BulkAccess::IStorageItemInformation>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Storage::BulkAccess::FileInformation>{ using type = class_category; };
    template <> struct category<winrt::Windows::Storage::BulkAccess::FileInformationFactory>{ using type = class_category; };
    template <> struct category<winrt::Windows::Storage::BulkAccess::FolderInformation>{ using type = class_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::Storage::BulkAccess::FileInformation> = L"Windows.Storage.BulkAccess.FileInformation";
    template <> inline constexpr auto& name_v<winrt::Windows::Storage::BulkAccess::FileInformationFactory> = L"Windows.Storage.BulkAccess.FileInformationFactory";
    template <> inline constexpr auto& name_v<winrt::Windows::Storage::BulkAccess::FolderInformation> = L"Windows.Storage.BulkAccess.FolderInformation";
    template <> inline constexpr auto& name_v<winrt::Windows::Storage::BulkAccess::IFileInformationFactory> = L"Windows.Storage.BulkAccess.IFileInformationFactory";
    template <> inline constexpr auto& name_v<winrt::Windows::Storage::BulkAccess::IFileInformationFactoryFactory> = L"Windows.Storage.BulkAccess.IFileInformationFactoryFactory";
    template <> inline constexpr auto& name_v<winrt::Windows::Storage::BulkAccess::IStorageItemInformation> = L"Windows.Storage.BulkAccess.IStorageItemInformation";
    template <> inline constexpr guid guid_v<winrt::Windows::Storage::BulkAccess::IFileInformationFactory>{ 0x401D88BE,0x960F,0x4D6D,{ 0xA7,0xD0,0x1A,0x38,0x61,0xE7,0x6C,0x83 } }; // 401D88BE-960F-4D6D-A7D0-1A3861E76C83
    template <> inline constexpr guid guid_v<winrt::Windows::Storage::BulkAccess::IFileInformationFactoryFactory>{ 0x84EA0E7D,0xE4A2,0x4F00,{ 0x8A,0xFA,0xAF,0x5E,0x0F,0x82,0x6B,0xD5 } }; // 84EA0E7D-E4A2-4F00-8AFA-AF5E0F826BD5
    template <> inline constexpr guid guid_v<winrt::Windows::Storage::BulkAccess::IStorageItemInformation>{ 0x87A5CB8B,0x8972,0x4F40,{ 0x8D,0xE0,0xD8,0x6F,0xB1,0x79,0xD8,0xFA } }; // 87A5CB8B-8972-4F40-8DE0-D86FB179D8FA
    template <> struct default_interface<winrt::Windows::Storage::BulkAccess::FileInformation>{ using type = winrt::Windows::Storage::BulkAccess::IStorageItemInformation; };
    template <> struct default_interface<winrt::Windows::Storage::BulkAccess::FileInformationFactory>{ using type = winrt::Windows::Storage::BulkAccess::IFileInformationFactory; };
    template <> struct default_interface<winrt::Windows::Storage::BulkAccess::FolderInformation>{ using type = winrt::Windows::Storage::BulkAccess::IStorageItemInformation; };
    template <> struct abi<winrt::Windows::Storage::BulkAccess::IFileInformationFactory>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetItemsAsync(uint32_t, uint32_t, void**) noexcept = 0;
            virtual int32_t __stdcall GetItemsAsyncDefaultStartAndCount(void**) noexcept = 0;
            virtual int32_t __stdcall GetFilesAsync(uint32_t, uint32_t, void**) noexcept = 0;
            virtual int32_t __stdcall GetFilesAsyncDefaultStartAndCount(void**) noexcept = 0;
            virtual int32_t __stdcall GetFoldersAsync(uint32_t, uint32_t, void**) noexcept = 0;
            virtual int32_t __stdcall GetFoldersAsyncDefaultStartAndCount(void**) noexcept = 0;
            virtual int32_t __stdcall GetVirtualizedItemsVector(void**) noexcept = 0;
            virtual int32_t __stdcall GetVirtualizedFilesVector(void**) noexcept = 0;
            virtual int32_t __stdcall GetVirtualizedFoldersVector(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Storage::BulkAccess::IFileInformationFactoryFactory>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall CreateWithMode(void*, int32_t, void**) noexcept = 0;
            virtual int32_t __stdcall CreateWithModeAndSize(void*, int32_t, uint32_t, void**) noexcept = 0;
            virtual int32_t __stdcall CreateWithModeAndSizeAndOptions(void*, int32_t, uint32_t, uint32_t, void**) noexcept = 0;
            virtual int32_t __stdcall CreateWithModeAndSizeAndOptionsAndFlags(void*, int32_t, uint32_t, uint32_t, bool, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Storage::BulkAccess::IStorageItemInformation>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_MusicProperties(void**) noexcept = 0;
            virtual int32_t __stdcall get_VideoProperties(void**) noexcept = 0;
            virtual int32_t __stdcall get_ImageProperties(void**) noexcept = 0;
            virtual int32_t __stdcall get_DocumentProperties(void**) noexcept = 0;
            virtual int32_t __stdcall get_BasicProperties(void**) noexcept = 0;
            virtual int32_t __stdcall get_Thumbnail(void**) noexcept = 0;
            virtual int32_t __stdcall add_ThumbnailUpdated(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_ThumbnailUpdated(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall add_PropertiesUpdated(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_PropertiesUpdated(winrt::event_token) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_Storage_BulkAccess_IFileInformationFactory
    {
        auto GetItemsAsync(uint32_t startIndex, uint32_t maxItemsToRetrieve) const;
        auto GetItemsAsync() const;
        auto GetFilesAsync(uint32_t startIndex, uint32_t maxItemsToRetrieve) const;
        auto GetFilesAsync() const;
        auto GetFoldersAsync(uint32_t startIndex, uint32_t maxItemsToRetrieve) const;
        auto GetFoldersAsync() const;
        auto GetVirtualizedItemsVector() const;
        auto GetVirtualizedFilesVector() const;
        auto GetVirtualizedFoldersVector() const;
    };
    template <> struct consume<winrt::Windows::Storage::BulkAccess::IFileInformationFactory>
    {
        template <typename D> using type = consume_Windows_Storage_BulkAccess_IFileInformationFactory<D>;
    };
    template <typename D>
    struct consume_Windows_Storage_BulkAccess_IFileInformationFactoryFactory
    {
        auto CreateWithMode(winrt::Windows::Storage::Search::IStorageQueryResultBase const& queryResult, winrt::Windows::Storage::FileProperties::ThumbnailMode const& mode) const;
        auto CreateWithModeAndSize(winrt::Windows::Storage::Search::IStorageQueryResultBase const& queryResult, winrt::Windows::Storage::FileProperties::ThumbnailMode const& mode, uint32_t requestedThumbnailSize) const;
        auto CreateWithModeAndSizeAndOptions(winrt::Windows::Storage::Search::IStorageQueryResultBase const& queryResult, winrt::Windows::Storage::FileProperties::ThumbnailMode const& mode, uint32_t requestedThumbnailSize, winrt::Windows::Storage::FileProperties::ThumbnailOptions const& thumbnailOptions) const;
        auto CreateWithModeAndSizeAndOptionsAndFlags(winrt::Windows::Storage::Search::IStorageQueryResultBase const& queryResult, winrt::Windows::Storage::FileProperties::ThumbnailMode const& mode, uint32_t requestedThumbnailSize, winrt::Windows::Storage::FileProperties::ThumbnailOptions const& thumbnailOptions, bool delayLoad) const;
    };
    template <> struct consume<winrt::Windows::Storage::BulkAccess::IFileInformationFactoryFactory>
    {
        template <typename D> using type = consume_Windows_Storage_BulkAccess_IFileInformationFactoryFactory<D>;
    };
    template <typename D>
    struct consume_Windows_Storage_BulkAccess_IStorageItemInformation
    {
        [[nodiscard]] auto MusicProperties() const;
        [[nodiscard]] auto VideoProperties() const;
        [[nodiscard]] auto ImageProperties() const;
        [[nodiscard]] auto DocumentProperties() const;
        [[nodiscard]] auto BasicProperties() const;
        [[nodiscard]] auto Thumbnail() const;
        auto ThumbnailUpdated(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Storage::BulkAccess::IStorageItemInformation, winrt::Windows::Foundation::IInspectable> const& changedHandler) const;
        using ThumbnailUpdated_revoker = impl::event_revoker<winrt::Windows::Storage::BulkAccess::IStorageItemInformation, &impl::abi_t<winrt::Windows::Storage::BulkAccess::IStorageItemInformation>::remove_ThumbnailUpdated>;
        [[nodiscard]] auto ThumbnailUpdated(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Storage::BulkAccess::IStorageItemInformation, winrt::Windows::Foundation::IInspectable> const& changedHandler) const;
        auto ThumbnailUpdated(winrt::event_token const& eventCookie) const noexcept;
        auto PropertiesUpdated(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Storage::BulkAccess::IStorageItemInformation, winrt::Windows::Foundation::IInspectable> const& changedHandler) const;
        using PropertiesUpdated_revoker = impl::event_revoker<winrt::Windows::Storage::BulkAccess::IStorageItemInformation, &impl::abi_t<winrt::Windows::Storage::BulkAccess::IStorageItemInformation>::remove_PropertiesUpdated>;
        [[nodiscard]] auto PropertiesUpdated(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Storage::BulkAccess::IStorageItemInformation, winrt::Windows::Foundation::IInspectable> const& changedHandler) const;
        auto PropertiesUpdated(winrt::event_token const& eventCookie) const noexcept;
    };
    template <> struct consume<winrt::Windows::Storage::BulkAccess::IStorageItemInformation>
    {
        template <typename D> using type = consume_Windows_Storage_BulkAccess_IStorageItemInformation<D>;
    };
}
#endif
