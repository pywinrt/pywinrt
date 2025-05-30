// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_Windows_Media_Capture_H
#define WINRT_Microsoft_Windows_Media_Capture_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/impl/Microsoft.UI.2.h"
#include "winrt/impl/Windows.Foundation.2.h"
#include "winrt/impl/Windows.Storage.2.h"
#include "winrt/impl/Microsoft.Windows.Media.Capture.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUI<D>::PhotoSettings() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_PhotoSettings(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI>**)this;
            check_hresult(_winrt_abi_type->get_PhotoSettings(&value));
        }
        return winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIPhotoCaptureSettings{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUI<D>::VideoSettings() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_VideoSettings(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI>**)this;
            check_hresult(_winrt_abi_type->get_VideoSettings(&value));
        }
        return winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIVideoCaptureSettings{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUI<D>::CaptureFileAsync(winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIMode const& mode) const
    {
        void* operation{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->CaptureFileAsync(static_cast<int32_t>(mode), &operation));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI>**)this;
            check_hresult(_winrt_abi_type->CaptureFileAsync(static_cast<int32_t>(mode), &operation));
        }
        return winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile>{ operation, take_ownership_from_abi };
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIFactory<D>::CreateInstance(winrt::Microsoft::UI::WindowId const& windowId) const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIFactory>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIFactory, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIFactory>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->CreateInstance(impl::bind_in(windowId), &value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIFactory>**)this;
            check_hresult(_winrt_abi_type->CreateInstance(impl::bind_in(windowId), &value));
        }
        return winrt::Microsoft::Windows::Media::Capture::CameraCaptureUI{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIPhotoCaptureSettings<D>::AllowCropping() const
    {
        bool value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_AllowCropping(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->get_AllowCropping(&value));
        }
        return value;
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIPhotoCaptureSettings<D>::AllowCropping(bool value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_AllowCropping(value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->put_AllowCropping(value));
        }
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIPhotoCaptureSettings<D>::CroppedAspectRatio() const
    {
        winrt::Windows::Foundation::Size value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_CroppedAspectRatio(put_abi(value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->get_CroppedAspectRatio(put_abi(value)));
        }
        return value;
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIPhotoCaptureSettings<D>::CroppedAspectRatio(winrt::Windows::Foundation::Size const& value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_CroppedAspectRatio(impl::bind_in(value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->put_CroppedAspectRatio(impl::bind_in(value)));
        }
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIPhotoCaptureSettings<D>::CroppedSizeInPixels() const
    {
        winrt::Windows::Foundation::Size value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_CroppedSizeInPixels(put_abi(value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->get_CroppedSizeInPixels(put_abi(value)));
        }
        return value;
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIPhotoCaptureSettings<D>::CroppedSizeInPixels(winrt::Windows::Foundation::Size const& value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_CroppedSizeInPixels(impl::bind_in(value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->put_CroppedSizeInPixels(impl::bind_in(value)));
        }
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIPhotoCaptureSettings<D>::Format() const
    {
        winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIPhotoFormat value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_Format(reinterpret_cast<int32_t*>(&value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->get_Format(reinterpret_cast<int32_t*>(&value)));
        }
        return value;
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIPhotoCaptureSettings<D>::Format(winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIPhotoFormat const& value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_Format(static_cast<int32_t>(value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->put_Format(static_cast<int32_t>(value)));
        }
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIPhotoCaptureSettings<D>::MaxResolution() const
    {
        winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIMaxPhotoResolution value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_MaxResolution(reinterpret_cast<int32_t*>(&value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->get_MaxResolution(reinterpret_cast<int32_t*>(&value)));
        }
        return value;
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIPhotoCaptureSettings<D>::MaxResolution(winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIMaxPhotoResolution const& value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_MaxResolution(static_cast<int32_t>(value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->put_MaxResolution(static_cast<int32_t>(value)));
        }
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIVideoCaptureSettings<D>::AllowTrimming() const
    {
        bool value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_AllowTrimming(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->get_AllowTrimming(&value));
        }
        return value;
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIVideoCaptureSettings<D>::AllowTrimming(bool value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_AllowTrimming(value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->put_AllowTrimming(value));
        }
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIVideoCaptureSettings<D>::Format() const
    {
        winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIVideoFormat value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_Format(reinterpret_cast<int32_t*>(&value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->get_Format(reinterpret_cast<int32_t*>(&value)));
        }
        return value;
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIVideoCaptureSettings<D>::Format(winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIVideoFormat const& value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_Format(static_cast<int32_t>(value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->put_Format(static_cast<int32_t>(value)));
        }
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIVideoCaptureSettings<D>::MaxDurationInSeconds() const
    {
        float value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_MaxDurationInSeconds(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->get_MaxDurationInSeconds(&value));
        }
        return value;
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIVideoCaptureSettings<D>::MaxDurationInSeconds(float value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_MaxDurationInSeconds(value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->put_MaxDurationInSeconds(value));
        }
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIVideoCaptureSettings<D>::MaxResolution() const
    {
        winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIMaxVideoResolution value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_MaxResolution(reinterpret_cast<int32_t*>(&value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->get_MaxResolution(reinterpret_cast<int32_t*>(&value)));
        }
        return value;
    }
    template <typename D> auto consume_Microsoft_Windows_Media_Capture_ICameraCaptureUIVideoCaptureSettings<D>::MaxResolution(winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIMaxVideoResolution const& value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_MaxResolution(static_cast<int32_t>(value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>**)this;
            check_hresult(_winrt_abi_type->put_MaxResolution(static_cast<int32_t>(value)));
        }
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI> : produce_base<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI>
    {
        int32_t __stdcall get_PhotoSettings(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIPhotoCaptureSettings>(this->shim().PhotoSettings());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_VideoSettings(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIVideoCaptureSettings>(this->shim().VideoSettings());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall CaptureFileAsync(int32_t mode, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile>>(this->shim().CaptureFileAsync(*reinterpret_cast<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIMode const*>(&mode)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIFactory> : produce_base<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIFactory>
    {
        int32_t __stdcall CreateInstance(struct struct_Microsoft_UI_WindowId windowId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUI>(this->shim().CreateInstance(*reinterpret_cast<winrt::Microsoft::UI::WindowId const*>(&windowId)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings> : produce_base<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings>
    {
        int32_t __stdcall get_AllowCropping(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().AllowCropping());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_AllowCropping(bool value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().AllowCropping(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_CroppedAspectRatio(winrt::Windows::Foundation::Size* value) noexcept final try
        {
            zero_abi<winrt::Windows::Foundation::Size>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Foundation::Size>(this->shim().CroppedAspectRatio());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_CroppedAspectRatio(winrt::Windows::Foundation::Size value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().CroppedAspectRatio(*reinterpret_cast<winrt::Windows::Foundation::Size const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_CroppedSizeInPixels(winrt::Windows::Foundation::Size* value) noexcept final try
        {
            zero_abi<winrt::Windows::Foundation::Size>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Foundation::Size>(this->shim().CroppedSizeInPixels());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_CroppedSizeInPixels(winrt::Windows::Foundation::Size value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().CroppedSizeInPixels(*reinterpret_cast<winrt::Windows::Foundation::Size const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Format(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIPhotoFormat>(this->shim().Format());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Format(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Format(*reinterpret_cast<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIPhotoFormat const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_MaxResolution(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIMaxPhotoResolution>(this->shim().MaxResolution());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_MaxResolution(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().MaxResolution(*reinterpret_cast<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIMaxPhotoResolution const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings> : produce_base<D, winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings>
    {
        int32_t __stdcall get_AllowTrimming(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().AllowTrimming());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_AllowTrimming(bool value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().AllowTrimming(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Format(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIVideoFormat>(this->shim().Format());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Format(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Format(*reinterpret_cast<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIVideoFormat const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_MaxDurationInSeconds(float* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<float>(this->shim().MaxDurationInSeconds());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_MaxDurationInSeconds(float value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().MaxDurationInSeconds(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_MaxResolution(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIMaxVideoResolution>(this->shim().MaxResolution());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_MaxResolution(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().MaxResolution(*reinterpret_cast<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIMaxVideoResolution const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Microsoft::Windows::Media::Capture
{
    inline CameraCaptureUI::CameraCaptureUI(winrt::Microsoft::UI::WindowId const& windowId) :
        CameraCaptureUI(impl::call_factory<CameraCaptureUI, ICameraCaptureUIFactory>([&](ICameraCaptureUIFactory const& f) { return f.CreateInstance(windowId); }))
    {
    }
    inline CameraCaptureUIPhotoCaptureSettings::CameraCaptureUIPhotoCaptureSettings() :
        CameraCaptureUIPhotoCaptureSettings(impl::call_factory_cast<CameraCaptureUIPhotoCaptureSettings(*)(winrt::Windows::Foundation::IActivationFactory const&), CameraCaptureUIPhotoCaptureSettings>([](winrt::Windows::Foundation::IActivationFactory const& f) { return f.template ActivateInstance<CameraCaptureUIPhotoCaptureSettings>(); }))
    {
    }
    inline CameraCaptureUIVideoCaptureSettings::CameraCaptureUIVideoCaptureSettings() :
        CameraCaptureUIVideoCaptureSettings(impl::call_factory_cast<CameraCaptureUIVideoCaptureSettings(*)(winrt::Windows::Foundation::IActivationFactory const&), CameraCaptureUIVideoCaptureSettings>([](winrt::Windows::Foundation::IActivationFactory const& f) { return f.template ActivateInstance<CameraCaptureUIVideoCaptureSettings>(); }))
    {
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUI> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIFactory> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIPhotoCaptureSettings> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Microsoft::Windows::Media::Capture::ICameraCaptureUIVideoCaptureSettings> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUI> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIPhotoCaptureSettings> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Microsoft::Windows::Media::Capture::CameraCaptureUIVideoCaptureSettings> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
