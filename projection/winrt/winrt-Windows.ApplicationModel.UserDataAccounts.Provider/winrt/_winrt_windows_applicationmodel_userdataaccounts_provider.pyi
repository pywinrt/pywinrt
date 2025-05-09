# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property
from abc import abstractmethod

import winrt._winrt
import winrt.system
import winrt.windows.applicationmodel.userdataaccounts as windows_applicationmodel_userdataaccounts
import winrt.windows.foundation.collections as windows_foundation_collections

from winrt.windows.applicationmodel.userdataaccounts.provider import UserDataAccountProviderOperationKind, UserDataAccountProviderPartnerAccountKind

Self = typing.TypeVar('Self')

@typing.final
class UserDataAccountPartnerAccountInfo(winrt.system.Object):
    # Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderPartnerAccountKind Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountPartnerAccountInfo::get_AccountKind()
    @_property
    def account_kind(self) -> UserDataAccountProviderPartnerAccountKind: ...
    # System.String Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountPartnerAccountInfo::get_DisplayName()
    @_property
    def display_name(self) -> str: ...
    # System.UInt32 Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountPartnerAccountInfo::get_Priority()
    @_property
    def priority(self) -> winrt.system.UInt32: ...

@typing.final
class UserDataAccountProviderAddAccountOperation(winrt.system.Object, IUserDataAccountProviderOperation):
    # System.Void Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderAddAccountOperation::ReportCompleted(System.String)
    def report_completed(self, user_data_account_id: str, /) -> None: ...
    # Windows.ApplicationModel.UserDataAccounts.UserDataAccountContentKinds Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderAddAccountOperation::get_ContentKinds()
    @_property
    def content_kinds(self) -> windows_applicationmodel_userdataaccounts.UserDataAccountContentKinds: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountPartnerAccountInfo> Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderAddAccountOperation::get_PartnerAccountInfos()
    @_property
    def partner_account_infos(self) -> typing.Sequence[UserDataAccountPartnerAccountInfo]: ...
    # Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderOperationKind Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderAddAccountOperation::get_Kind()
    @_property
    def kind(self) -> UserDataAccountProviderOperationKind: ...

@typing.final
class UserDataAccountProviderResolveErrorsOperation(winrt.system.Object, IUserDataAccountProviderOperation):
    # System.Void Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderResolveErrorsOperation::ReportCompleted()
    def report_completed(self) -> None: ...
    # Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderOperationKind Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderResolveErrorsOperation::get_Kind()
    @_property
    def kind(self) -> UserDataAccountProviderOperationKind: ...
    # System.String Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderResolveErrorsOperation::get_UserDataAccountId()
    @_property
    def user_data_account_id(self) -> str: ...

@typing.final
class UserDataAccountProviderSettingsOperation(winrt.system.Object, IUserDataAccountProviderOperation):
    # System.Void Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderSettingsOperation::ReportCompleted()
    def report_completed(self) -> None: ...
    # Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderOperationKind Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderSettingsOperation::get_Kind()
    @_property
    def kind(self) -> UserDataAccountProviderOperationKind: ...
    # System.String Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderSettingsOperation::get_UserDataAccountId()
    @_property
    def user_data_account_id(self) -> str: ...

@typing.final
class _IUserDataAccountProviderOperation: ...

class IUserDataAccountProviderOperation(winrt._winrt.IInspectable):
    # Windows.ApplicationModel.UserDataAccounts.Provider.UserDataAccountProviderOperationKind Windows.ApplicationModel.UserDataAccounts.Provider.IUserDataAccountProviderOperation::get_Kind()
    @_property
    @abstractmethod
    def kind(self) -> UserDataAccountProviderOperationKind: ...

