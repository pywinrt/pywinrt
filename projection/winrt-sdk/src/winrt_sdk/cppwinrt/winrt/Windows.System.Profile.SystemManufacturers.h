// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_System_Profile_SystemManufacturers_H
#define WINRT_Windows_System_Profile_SystemManufacturers_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/Windows.System.Profile.h"
#include "winrt/impl/Windows.Foundation.2.h"
#include "winrt/impl/Windows.System.Profile.SystemManufacturers.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_IOemSupportInfo<D>::SupportLink() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_SupportLink(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo>**)this;
            check_hresult(_winrt_abi_type->get_SupportLink(&value));
        }
        return winrt::Windows::Foundation::Uri{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_IOemSupportInfo<D>::SupportAppLink() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_SupportAppLink(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo>**)this;
            check_hresult(_winrt_abi_type->get_SupportAppLink(&value));
        }
        return winrt::Windows::Foundation::Uri{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_IOemSupportInfo<D>::SupportProvider() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_SupportProvider(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo>**)this;
            check_hresult(_winrt_abi_type->get_SupportProvider(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISmbiosInformationStatics<D>::SerialNumber() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISmbiosInformationStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISmbiosInformationStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISmbiosInformationStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_SerialNumber(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISmbiosInformationStatics>**)this;
            check_hresult(_winrt_abi_type->get_SerialNumber(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISystemSupportDeviceInfo<D>::OperatingSystem() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_OperatingSystem(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)this;
            check_hresult(_winrt_abi_type->get_OperatingSystem(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISystemSupportDeviceInfo<D>::FriendlyName() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_FriendlyName(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)this;
            check_hresult(_winrt_abi_type->get_FriendlyName(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISystemSupportDeviceInfo<D>::SystemManufacturer() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_SystemManufacturer(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)this;
            check_hresult(_winrt_abi_type->get_SystemManufacturer(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISystemSupportDeviceInfo<D>::SystemProductName() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_SystemProductName(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)this;
            check_hresult(_winrt_abi_type->get_SystemProductName(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISystemSupportDeviceInfo<D>::SystemSku() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_SystemSku(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)this;
            check_hresult(_winrt_abi_type->get_SystemSku(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISystemSupportDeviceInfo<D>::SystemHardwareVersion() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_SystemHardwareVersion(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)this;
            check_hresult(_winrt_abi_type->get_SystemHardwareVersion(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISystemSupportDeviceInfo<D>::SystemFirmwareVersion() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_SystemFirmwareVersion(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>**)this;
            check_hresult(_winrt_abi_type->get_SystemFirmwareVersion(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISystemSupportInfoStatics<D>::LocalSystemEdition() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_LocalSystemEdition(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics>**)this;
            check_hresult(_winrt_abi_type->get_LocalSystemEdition(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISystemSupportInfoStatics<D>::OemSupportInfo() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_OemSupportInfo(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics>**)this;
            check_hresult(_winrt_abi_type->get_OemSupportInfo(&value));
        }
        return winrt::Windows::System::Profile::SystemManufacturers::OemSupportInfo{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_System_Profile_SystemManufacturers_ISystemSupportInfoStatics2<D>::LocalDeviceInfo() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics2>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics2, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics2>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_LocalDeviceInfo(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics2>**)this;
            check_hresult(_winrt_abi_type->get_LocalDeviceInfo(&value));
        }
        return winrt::Windows::System::Profile::SystemManufacturers::SystemSupportDeviceInfo{ value, take_ownership_from_abi };
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo> : produce_base<D, winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo>
    {
        int32_t __stdcall get_SupportLink(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Foundation::Uri>(this->shim().SupportLink());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SupportAppLink(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Foundation::Uri>(this->shim().SupportAppLink());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SupportProvider(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().SupportProvider());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::System::Profile::SystemManufacturers::ISmbiosInformationStatics> : produce_base<D, winrt::Windows::System::Profile::SystemManufacturers::ISmbiosInformationStatics>
    {
        int32_t __stdcall get_SerialNumber(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().SerialNumber());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo> : produce_base<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo>
    {
        int32_t __stdcall get_OperatingSystem(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().OperatingSystem());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_FriendlyName(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().FriendlyName());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SystemManufacturer(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().SystemManufacturer());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SystemProductName(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().SystemProductName());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SystemSku(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().SystemSku());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SystemHardwareVersion(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().SystemHardwareVersion());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SystemFirmwareVersion(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().SystemFirmwareVersion());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics> : produce_base<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics>
    {
        int32_t __stdcall get_LocalSystemEdition(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().LocalSystemEdition());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_OemSupportInfo(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::System::Profile::SystemManufacturers::OemSupportInfo>(this->shim().OemSupportInfo());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics2> : produce_base<D, winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics2>
    {
        int32_t __stdcall get_LocalDeviceInfo(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::System::Profile::SystemManufacturers::SystemSupportDeviceInfo>(this->shim().LocalDeviceInfo());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Windows::System::Profile::SystemManufacturers
{
    inline auto SmbiosInformation::SerialNumber()
    {
        return impl::call_factory_cast<hstring(*)(ISmbiosInformationStatics const&), SmbiosInformation, ISmbiosInformationStatics>([](ISmbiosInformationStatics const& f) { return f.SerialNumber(); });
    }
    inline auto SystemSupportInfo::LocalSystemEdition()
    {
        return impl::call_factory_cast<hstring(*)(ISystemSupportInfoStatics const&), SystemSupportInfo, ISystemSupportInfoStatics>([](ISystemSupportInfoStatics const& f) { return f.LocalSystemEdition(); });
    }
    inline auto SystemSupportInfo::OemSupportInfo()
    {
        return impl::call_factory_cast<winrt::Windows::System::Profile::SystemManufacturers::OemSupportInfo(*)(ISystemSupportInfoStatics const&), SystemSupportInfo, ISystemSupportInfoStatics>([](ISystemSupportInfoStatics const& f) { return f.OemSupportInfo(); });
    }
    inline auto SystemSupportInfo::LocalDeviceInfo()
    {
        return impl::call_factory_cast<winrt::Windows::System::Profile::SystemManufacturers::SystemSupportDeviceInfo(*)(ISystemSupportInfoStatics2 const&), SystemSupportInfo, ISystemSupportInfoStatics2>([](ISystemSupportInfoStatics2 const& f) { return f.LocalDeviceInfo(); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Windows::System::Profile::SystemManufacturers::IOemSupportInfo> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::System::Profile::SystemManufacturers::ISmbiosInformationStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportDeviceInfo> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::System::Profile::SystemManufacturers::ISystemSupportInfoStatics2> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::System::Profile::SystemManufacturers::OemSupportInfo> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::System::Profile::SystemManufacturers::SmbiosInformation> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::System::Profile::SystemManufacturers::SystemSupportDeviceInfo> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::System::Profile::SystemManufacturers::SystemSupportInfo> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
