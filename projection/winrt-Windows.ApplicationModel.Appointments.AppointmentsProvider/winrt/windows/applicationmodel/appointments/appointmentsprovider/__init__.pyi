# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing

import winrt.system
import winrt.windows.applicationmodel.appointments
import winrt.windows.foundation

Self = typing.TypeVar('Self')

class AddAppointmentOperation(winrt.system.Object):
    appointment_information: typing.Optional[winrt.windows.applicationmodel.appointments.Appointment]
    source_package_family_name: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> AddAppointmentOperation: ...
    def dismiss_u_i(self) -> None: ...
    def report_canceled(self) -> None: ...
    def report_completed(self, item_id: str, /) -> None: ...
    def report_error(self, value: str, /) -> None: ...

class AppointmentsProviderLaunchActionVerbs(winrt.system.Object):
    add_appointment: typing.ClassVar[str]
    remove_appointment: typing.ClassVar[str]
    replace_appointment: typing.ClassVar[str]
    show_time_frame: typing.ClassVar[str]
    show_appointment_details: typing.ClassVar[str]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> AppointmentsProviderLaunchActionVerbs: ...

class RemoveAppointmentOperation(winrt.system.Object):
    appointment_id: str
    instance_start_date: typing.Optional[typing.Optional[datetime.datetime]]
    source_package_family_name: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> RemoveAppointmentOperation: ...
    def dismiss_u_i(self) -> None: ...
    def report_canceled(self) -> None: ...
    def report_completed(self) -> None: ...
    def report_error(self, value: str, /) -> None: ...

class ReplaceAppointmentOperation(winrt.system.Object):
    appointment_id: str
    appointment_information: typing.Optional[winrt.windows.applicationmodel.appointments.Appointment]
    instance_start_date: typing.Optional[typing.Optional[datetime.datetime]]
    source_package_family_name: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ReplaceAppointmentOperation: ...
    def dismiss_u_i(self) -> None: ...
    def report_canceled(self) -> None: ...
    def report_completed(self, item_id: str, /) -> None: ...
    def report_error(self, value: str, /) -> None: ...
