# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation
import winrt.windows.foundation.collections
import winrt.windows.phone.personalinformation
import winrt.windows.storage.streams

Self = typing.TypeVar('Self')

class ContactPartnerProvisioningManager_Static(type):
    def associate_network_account_async(cls, store: typing.Optional[winrt.windows.phone.personalinformation.ContactStore], network_name: str, network_account_id: str, /) -> winrt.windows.foundation.IAsyncAction: ...
    def associate_social_network_account_async(cls, store: typing.Optional[winrt.windows.phone.personalinformation.ContactStore], network_name: str, network_account_id: str, /) -> winrt.windows.foundation.IAsyncAction: ...
    def import_vcard_to_system_async(cls, stream: typing.Optional[winrt.windows.storage.streams.IInputStream], /) -> winrt.windows.foundation.IAsyncAction: ...

class ContactPartnerProvisioningManager(winrt.system.Object, metaclass=ContactPartnerProvisioningManager_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ContactPartnerProvisioningManager: ...

class MessagePartnerProvisioningManager_Static(type):
    def import_mms_to_system_async(cls, incoming: bool, read: bool, subject: str, sender: str, recipients: winrt.windows.foundation.collections.IVectorView[str], delivery_time: datetime.datetime, attachments: winrt.windows.foundation.collections.IVectorView[winrt.windows.foundation.collections.IMapView[str, winrt.system.Object]], /) -> winrt.windows.foundation.IAsyncAction: ...
    def import_sms_to_system_async(cls, incoming: bool, read: bool, body: str, sender: str, recipients: winrt.windows.foundation.collections.IVectorView[str], delivery_time: datetime.datetime, /) -> winrt.windows.foundation.IAsyncAction: ...

class MessagePartnerProvisioningManager(winrt.system.Object, metaclass=MessagePartnerProvisioningManager_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> MessagePartnerProvisioningManager: ...
