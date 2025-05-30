// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_ApplicationModel_Appointments_1_H
#define WINRT_Windows_ApplicationModel_Appointments_1_H
#include "winrt/impl/Windows.ApplicationModel.Appointments.0.h"
WINRT_EXPORT namespace winrt::Windows::ApplicationModel::Appointments
{
    struct WINRT_IMPL_EMPTY_BASES IAppointment :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointment>
    {
        IAppointment(std::nullptr_t = nullptr) noexcept {}
        IAppointment(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointment2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointment2>,
        impl::require<winrt::Windows::ApplicationModel::Appointments::IAppointment2, winrt::Windows::ApplicationModel::Appointments::IAppointment>
    {
        IAppointment2(std::nullptr_t = nullptr) noexcept {}
        IAppointment2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointment3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointment3>,
        impl::require<winrt::Windows::ApplicationModel::Appointments::IAppointment3, winrt::Windows::ApplicationModel::Appointments::IAppointment, winrt::Windows::ApplicationModel::Appointments::IAppointment2>
    {
        IAppointment3(std::nullptr_t = nullptr) noexcept {}
        IAppointment3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentCalendar :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentCalendar>
    {
        IAppointmentCalendar(std::nullptr_t = nullptr) noexcept {}
        IAppointmentCalendar(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentCalendar2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentCalendar2>,
        impl::require<winrt::Windows::ApplicationModel::Appointments::IAppointmentCalendar2, winrt::Windows::ApplicationModel::Appointments::IAppointmentCalendar>
    {
        IAppointmentCalendar2(std::nullptr_t = nullptr) noexcept {}
        IAppointmentCalendar2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
        using impl::consume_t<IAppointmentCalendar2, IAppointmentCalendar2>::DisplayColor;
        using impl::consume_t<IAppointmentCalendar2, winrt::Windows::ApplicationModel::Appointments::IAppointmentCalendar>::DisplayColor;
        using impl::consume_t<IAppointmentCalendar2, IAppointmentCalendar2>::IsHidden;
        using impl::consume_t<IAppointmentCalendar2, winrt::Windows::ApplicationModel::Appointments::IAppointmentCalendar>::IsHidden;
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentCalendar3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentCalendar3>
    {
        IAppointmentCalendar3(std::nullptr_t = nullptr) noexcept {}
        IAppointmentCalendar3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentCalendarSyncManager :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentCalendarSyncManager>
    {
        IAppointmentCalendarSyncManager(std::nullptr_t = nullptr) noexcept {}
        IAppointmentCalendarSyncManager(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentCalendarSyncManager2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentCalendarSyncManager2>
    {
        IAppointmentCalendarSyncManager2(std::nullptr_t = nullptr) noexcept {}
        IAppointmentCalendarSyncManager2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentConflictResult :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentConflictResult>
    {
        IAppointmentConflictResult(std::nullptr_t = nullptr) noexcept {}
        IAppointmentConflictResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentException :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentException>
    {
        IAppointmentException(std::nullptr_t = nullptr) noexcept {}
        IAppointmentException(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentInvitee :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentInvitee>,
        impl::require<winrt::Windows::ApplicationModel::Appointments::IAppointmentInvitee, winrt::Windows::ApplicationModel::Appointments::IAppointmentParticipant>
    {
        IAppointmentInvitee(std::nullptr_t = nullptr) noexcept {}
        IAppointmentInvitee(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentManagerForUser :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentManagerForUser>
    {
        IAppointmentManagerForUser(std::nullptr_t = nullptr) noexcept {}
        IAppointmentManagerForUser(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentManagerStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentManagerStatics>
    {
        IAppointmentManagerStatics(std::nullptr_t = nullptr) noexcept {}
        IAppointmentManagerStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentManagerStatics2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentManagerStatics2>
    {
        IAppointmentManagerStatics2(std::nullptr_t = nullptr) noexcept {}
        IAppointmentManagerStatics2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentManagerStatics3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentManagerStatics3>
    {
        IAppointmentManagerStatics3(std::nullptr_t = nullptr) noexcept {}
        IAppointmentManagerStatics3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentParticipant :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentParticipant>
    {
        IAppointmentParticipant(std::nullptr_t = nullptr) noexcept {}
        IAppointmentParticipant(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentPropertiesStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentPropertiesStatics>
    {
        IAppointmentPropertiesStatics(std::nullptr_t = nullptr) noexcept {}
        IAppointmentPropertiesStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentPropertiesStatics2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentPropertiesStatics2>,
        impl::require<winrt::Windows::ApplicationModel::Appointments::IAppointmentPropertiesStatics2, winrt::Windows::ApplicationModel::Appointments::IAppointmentPropertiesStatics>
    {
        IAppointmentPropertiesStatics2(std::nullptr_t = nullptr) noexcept {}
        IAppointmentPropertiesStatics2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentRecurrence :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentRecurrence>
    {
        IAppointmentRecurrence(std::nullptr_t = nullptr) noexcept {}
        IAppointmentRecurrence(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentRecurrence2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentRecurrence2>,
        impl::require<winrt::Windows::ApplicationModel::Appointments::IAppointmentRecurrence2, winrt::Windows::ApplicationModel::Appointments::IAppointmentRecurrence>
    {
        IAppointmentRecurrence2(std::nullptr_t = nullptr) noexcept {}
        IAppointmentRecurrence2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentRecurrence3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentRecurrence3>,
        impl::require<winrt::Windows::ApplicationModel::Appointments::IAppointmentRecurrence3, winrt::Windows::ApplicationModel::Appointments::IAppointmentRecurrence, winrt::Windows::ApplicationModel::Appointments::IAppointmentRecurrence2>
    {
        IAppointmentRecurrence3(std::nullptr_t = nullptr) noexcept {}
        IAppointmentRecurrence3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStore :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStore>
    {
        IAppointmentStore(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStore(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStore2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStore2>,
        impl::require<winrt::Windows::ApplicationModel::Appointments::IAppointmentStore2, winrt::Windows::ApplicationModel::Appointments::IAppointmentStore>
    {
        IAppointmentStore2(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStore2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
        using impl::consume_t<IAppointmentStore2, IAppointmentStore2>::CreateAppointmentCalendarAsync;
        using impl::consume_t<IAppointmentStore2, winrt::Windows::ApplicationModel::Appointments::IAppointmentStore>::CreateAppointmentCalendarAsync;
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStore3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStore3>
    {
        IAppointmentStore3(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStore3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStoreChange :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStoreChange>
    {
        IAppointmentStoreChange(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStoreChange(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStoreChange2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStoreChange2>,
        impl::require<winrt::Windows::ApplicationModel::Appointments::IAppointmentStoreChange2, winrt::Windows::ApplicationModel::Appointments::IAppointmentStoreChange>
    {
        IAppointmentStoreChange2(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStoreChange2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStoreChangeReader :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStoreChangeReader>
    {
        IAppointmentStoreChangeReader(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStoreChangeReader(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStoreChangeTracker :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStoreChangeTracker>
    {
        IAppointmentStoreChangeTracker(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStoreChangeTracker(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStoreChangeTracker2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStoreChangeTracker2>
    {
        IAppointmentStoreChangeTracker2(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStoreChangeTracker2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStoreChangedDeferral :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStoreChangedDeferral>
    {
        IAppointmentStoreChangedDeferral(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStoreChangedDeferral(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStoreChangedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStoreChangedEventArgs>
    {
        IAppointmentStoreChangedEventArgs(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStoreChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IAppointmentStoreNotificationTriggerDetails :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAppointmentStoreNotificationTriggerDetails>
    {
        IAppointmentStoreNotificationTriggerDetails(std::nullptr_t = nullptr) noexcept {}
        IAppointmentStoreNotificationTriggerDetails(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IFindAppointmentsOptions :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IFindAppointmentsOptions>
    {
        IFindAppointmentsOptions(std::nullptr_t = nullptr) noexcept {}
        IFindAppointmentsOptions(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
}
#endif
