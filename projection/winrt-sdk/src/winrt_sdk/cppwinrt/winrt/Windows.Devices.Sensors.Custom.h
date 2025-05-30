// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Devices_Sensors_Custom_H
#define WINRT_Windows_Devices_Sensors_Custom_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/Windows.Devices.Sensors.h"
#include "winrt/impl/Windows.Foundation.2.h"
#include "winrt/impl/Windows.Foundation.Collections.2.h"
#include "winrt/impl/Windows.Devices.Sensors.Custom.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor<D>::GetCurrentReading() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensor, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->GetCurrentReading(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)this;
            check_hresult(_winrt_abi_type->GetCurrentReading(&value));
        }
        return winrt::Windows::Devices::Sensors::Custom::CustomSensorReading{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor<D>::MinimumReportInterval() const
    {
        uint32_t value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensor, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_MinimumReportInterval(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)this;
            check_hresult(_winrt_abi_type->get_MinimumReportInterval(&value));
        }
        return value;
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor<D>::ReportInterval(uint32_t value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensor, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_ReportInterval(value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)this;
            check_hresult(_winrt_abi_type->put_ReportInterval(value));
        }
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor<D>::ReportInterval() const
    {
        uint32_t value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensor, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_ReportInterval(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)this;
            check_hresult(_winrt_abi_type->get_ReportInterval(&value));
        }
        return value;
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor<D>::DeviceId() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensor, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_DeviceId(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)this;
            check_hresult(_winrt_abi_type->get_DeviceId(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor<D>::ReadingChanged(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Devices::Sensors::Custom::CustomSensor, winrt::Windows::Devices::Sensors::Custom::CustomSensorReadingChangedEventArgs> const& handler) const
    {
        winrt::event_token token{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensor, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->add_ReadingChanged(*(void**)(&handler), put_abi(token)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)this;
            check_hresult(_winrt_abi_type->add_ReadingChanged(*(void**)(&handler), put_abi(token)));
        }
        return token;
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor<D>::ReadingChanged(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Devices::Sensors::Custom::CustomSensor, winrt::Windows::Devices::Sensors::Custom::CustomSensorReadingChangedEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, ReadingChanged_revoker>(this, ReadingChanged(handler));
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor<D>::ReadingChanged(winrt::event_token const& token) const noexcept
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensor, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)&_winrt_casted_result;
            _winrt_abi_type->remove_ReadingChanged(impl::bind_in(token));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor>**)this;
            _winrt_abi_type->remove_ReadingChanged(impl::bind_in(token));
        }
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor2<D>::ReportLatency(uint32_t value) const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor2>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensor2, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor2>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->put_ReportLatency(value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor2>**)this;
            check_hresult(_winrt_abi_type->put_ReportLatency(value));
        }
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor2<D>::ReportLatency() const
    {
        uint32_t value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor2>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensor2, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor2>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_ReportLatency(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor2>**)this;
            check_hresult(_winrt_abi_type->get_ReportLatency(&value));
        }
        return value;
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensor2<D>::MaxBatchSize() const
    {
        uint32_t value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor2>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensor2, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor2>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_MaxBatchSize(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensor2>**)this;
            check_hresult(_winrt_abi_type->get_MaxBatchSize(&value));
        }
        return value;
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensorReading<D>::Timestamp() const
    {
        winrt::Windows::Foundation::DateTime value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_Timestamp(put_abi(value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading>**)this;
            check_hresult(_winrt_abi_type->get_Timestamp(put_abi(value)));
        }
        return value;
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensorReading<D>::Properties() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_Properties(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading>**)this;
            check_hresult(_winrt_abi_type->get_Properties(&value));
        }
        return winrt::Windows::Foundation::Collections::IMapView<hstring, winrt::Windows::Foundation::IInspectable>{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensorReading2<D>::PerformanceCount() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading2>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading2, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading2>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_PerformanceCount(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading2>**)this;
            check_hresult(_winrt_abi_type->get_PerformanceCount(&value));
        }
        return winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensorReadingChangedEventArgs<D>::Reading() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorReadingChangedEventArgs>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReadingChangedEventArgs, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReadingChangedEventArgs>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_Reading(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReadingChangedEventArgs>**)this;
            check_hresult(_winrt_abi_type->get_Reading(&value));
        }
        return winrt::Windows::Devices::Sensors::Custom::CustomSensorReading{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensorStatics<D>::GetDeviceSelector(winrt::guid const& interfaceId) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->GetDeviceSelector(impl::bind_in(interfaceId), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics>**)this;
            check_hresult(_winrt_abi_type->GetDeviceSelector(impl::bind_in(interfaceId), &result));
        }
        return hstring{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Devices_Sensors_Custom_ICustomSensorStatics<D>::FromIdAsync(param::hstring const& sensorId) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->FromIdAsync(*(void**)(&sensorId), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics>**)this;
            check_hresult(_winrt_abi_type->FromIdAsync(*(void**)(&sensorId), &result));
        }
        return winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Devices::Sensors::Custom::CustomSensor>{ result, take_ownership_from_abi };
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor> : produce_base<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor>
    {
        int32_t __stdcall GetCurrentReading(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Devices::Sensors::Custom::CustomSensorReading>(this->shim().GetCurrentReading());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_MinimumReportInterval(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().MinimumReportInterval());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_ReportInterval(uint32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().ReportInterval(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ReportInterval(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().ReportInterval());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_DeviceId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().DeviceId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall add_ReadingChanged(void* handler, winrt::event_token* token) noexcept final try
        {
            zero_abi<winrt::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<winrt::event_token>(this->shim().ReadingChanged(*reinterpret_cast<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Devices::Sensors::Custom::CustomSensor, winrt::Windows::Devices::Sensors::Custom::CustomSensorReadingChangedEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_ReadingChanged(winrt::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().ReadingChanged(*reinterpret_cast<winrt::event_token const*>(&token));
            return 0;
        }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor2> : produce_base<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensor2>
    {
        int32_t __stdcall put_ReportLatency(uint32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().ReportLatency(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ReportLatency(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().ReportLatency());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_MaxBatchSize(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().MaxBatchSize());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading> : produce_base<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading>
    {
        int32_t __stdcall get_Timestamp(int64_t* value) noexcept final try
        {
            zero_abi<winrt::Windows::Foundation::DateTime>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Foundation::DateTime>(this->shim().Timestamp());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Properties(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Foundation::Collections::IMapView<hstring, winrt::Windows::Foundation::IInspectable>>(this->shim().Properties());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading2> : produce_base<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading2>
    {
        int32_t __stdcall get_PerformanceCount(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan>>(this->shim().PerformanceCount());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorReadingChangedEventArgs> : produce_base<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorReadingChangedEventArgs>
    {
        int32_t __stdcall get_Reading(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Devices::Sensors::Custom::CustomSensorReading>(this->shim().Reading());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics> : produce_base<D, winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics>
    {
        int32_t __stdcall GetDeviceSelector(winrt::guid interfaceId, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<hstring>(this->shim().GetDeviceSelector(*reinterpret_cast<winrt::guid const*>(&interfaceId)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall FromIdAsync(void* sensorId, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Devices::Sensors::Custom::CustomSensor>>(this->shim().FromIdAsync(*reinterpret_cast<hstring const*>(&sensorId)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Windows::Devices::Sensors::Custom
{
    inline auto CustomSensor::GetDeviceSelector(winrt::guid const& interfaceId)
    {
        return impl::call_factory<CustomSensor, ICustomSensorStatics>([&](ICustomSensorStatics const& f) { return f.GetDeviceSelector(interfaceId); });
    }
    inline auto CustomSensor::FromIdAsync(param::hstring const& sensorId)
    {
        return impl::call_factory<CustomSensor, ICustomSensorStatics>([&](ICustomSensorStatics const& f) { return f.FromIdAsync(sensorId); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Windows::Devices::Sensors::Custom::ICustomSensor> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Devices::Sensors::Custom::ICustomSensor2> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReading2> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Devices::Sensors::Custom::ICustomSensorReadingChangedEventArgs> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Devices::Sensors::Custom::ICustomSensorStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Devices::Sensors::Custom::CustomSensor> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Devices::Sensors::Custom::CustomSensorReading> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Devices::Sensors::Custom::CustomSensorReadingChangedEventArgs> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
