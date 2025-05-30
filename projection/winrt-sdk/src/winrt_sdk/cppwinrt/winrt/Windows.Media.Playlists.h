// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Media_Playlists_H
#define WINRT_Windows_Media_Playlists_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/Windows.Media.h"
#include "winrt/impl/Windows.Foundation.2.h"
#include "winrt/impl/Windows.Foundation.Collections.2.h"
#include "winrt/impl/Windows.Storage.2.h"
#include "winrt/impl/Windows.Media.Playlists.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_Media_Playlists_IPlaylist<D>::Files() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Media::Playlists::IPlaylist>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Media::Playlists::IPlaylist, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Media::Playlists::IPlaylist>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_Files(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Media::Playlists::IPlaylist>**)this;
            check_hresult(_winrt_abi_type->get_Files(&value));
        }
        return winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Storage::StorageFile>{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_Playlists_IPlaylist<D>::SaveAsync() const
    {
        void* operation{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Media::Playlists::IPlaylist>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Media::Playlists::IPlaylist, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Media::Playlists::IPlaylist>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->SaveAsync(&operation));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Media::Playlists::IPlaylist>**)this;
            check_hresult(_winrt_abi_type->SaveAsync(&operation));
        }
        return winrt::Windows::Foundation::IAsyncAction{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_Playlists_IPlaylist<D>::SaveAsAsync(winrt::Windows::Storage::IStorageFolder const& saveLocation, param::hstring const& desiredName, winrt::Windows::Storage::NameCollisionOption const& option) const
    {
        void* operation{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Media::Playlists::IPlaylist>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Media::Playlists::IPlaylist, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Media::Playlists::IPlaylist>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->SaveAsAsync(*(void**)(&saveLocation), *(void**)(&desiredName), static_cast<int32_t>(option), &operation));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Media::Playlists::IPlaylist>**)this;
            check_hresult(_winrt_abi_type->SaveAsAsync(*(void**)(&saveLocation), *(void**)(&desiredName), static_cast<int32_t>(option), &operation));
        }
        return winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_Playlists_IPlaylist<D>::SaveAsAsync(winrt::Windows::Storage::IStorageFolder const& saveLocation, param::hstring const& desiredName, winrt::Windows::Storage::NameCollisionOption const& option, winrt::Windows::Media::Playlists::PlaylistFormat const& playlistFormat) const
    {
        void* operation{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Media::Playlists::IPlaylist>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Media::Playlists::IPlaylist, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Media::Playlists::IPlaylist>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->SaveAsWithFormatAsync(*(void**)(&saveLocation), *(void**)(&desiredName), static_cast<int32_t>(option), static_cast<int32_t>(playlistFormat), &operation));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Media::Playlists::IPlaylist>**)this;
            check_hresult(_winrt_abi_type->SaveAsWithFormatAsync(*(void**)(&saveLocation), *(void**)(&desiredName), static_cast<int32_t>(option), static_cast<int32_t>(playlistFormat), &operation));
        }
        return winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Media_Playlists_IPlaylistStatics<D>::LoadAsync(winrt::Windows::Storage::IStorageFile const& file) const
    {
        void* operation{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Media::Playlists::IPlaylistStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Media::Playlists::IPlaylistStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Media::Playlists::IPlaylistStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->LoadAsync(*(void**)(&file), &operation));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Media::Playlists::IPlaylistStatics>**)this;
            check_hresult(_winrt_abi_type->LoadAsync(*(void**)(&file), &operation));
        }
        return winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Media::Playlists::Playlist>{ operation, take_ownership_from_abi };
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Media::Playlists::IPlaylist> : produce_base<D, winrt::Windows::Media::Playlists::IPlaylist>
    {
        int32_t __stdcall get_Files(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Storage::StorageFile>>(this->shim().Files());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall SaveAsync(void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<winrt::Windows::Foundation::IAsyncAction>(this->shim().SaveAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall SaveAsAsync(void* saveLocation, void* desiredName, int32_t option, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile>>(this->shim().SaveAsAsync(*reinterpret_cast<winrt::Windows::Storage::IStorageFolder const*>(&saveLocation), *reinterpret_cast<hstring const*>(&desiredName), *reinterpret_cast<winrt::Windows::Storage::NameCollisionOption const*>(&option)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall SaveAsWithFormatAsync(void* saveLocation, void* desiredName, int32_t option, int32_t playlistFormat, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile>>(this->shim().SaveAsAsync(*reinterpret_cast<winrt::Windows::Storage::IStorageFolder const*>(&saveLocation), *reinterpret_cast<hstring const*>(&desiredName), *reinterpret_cast<winrt::Windows::Storage::NameCollisionOption const*>(&option), *reinterpret_cast<winrt::Windows::Media::Playlists::PlaylistFormat const*>(&playlistFormat)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Media::Playlists::IPlaylistStatics> : produce_base<D, winrt::Windows::Media::Playlists::IPlaylistStatics>
    {
        int32_t __stdcall LoadAsync(void* file, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Media::Playlists::Playlist>>(this->shim().LoadAsync(*reinterpret_cast<winrt::Windows::Storage::IStorageFile const*>(&file)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Windows::Media::Playlists
{
    inline Playlist::Playlist() :
        Playlist(impl::call_factory_cast<Playlist(*)(winrt::Windows::Foundation::IActivationFactory const&), Playlist>([](winrt::Windows::Foundation::IActivationFactory const& f) { return f.template ActivateInstance<Playlist>(); }))
    {
    }
    inline auto Playlist::LoadAsync(winrt::Windows::Storage::IStorageFile const& file)
    {
        return impl::call_factory<Playlist, IPlaylistStatics>([&](IPlaylistStatics const& f) { return f.LoadAsync(file); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Windows::Media::Playlists::IPlaylist> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Media::Playlists::IPlaylistStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Media::Playlists::Playlist> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
