# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.applicationmodel.contacts as windows_applicationmodel_contacts
import winrt.windows.applicationmodel.datatransfer as windows_applicationmodel_datatransfer
import winrt.windows.foundation.collections as windows_foundation_collections
import winrt.windows.storage.streams as windows_storage_streams

Self = typing.TypeVar('Self')

@typing.final
class QuickLink(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.String Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink::get_Title()
    @_property
    def title(self) -> str: ...
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink::put_Title(System.String)
    @title.setter
    def title(self, value: str) -> None: ...
    # Windows.Storage.Streams.RandomAccessStreamReference Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink::get_Thumbnail()
    @_property
    def thumbnail(self) -> windows_storage_streams.RandomAccessStreamReference: ...
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink::put_Thumbnail(Windows.Storage.Streams.RandomAccessStreamReference)
    @thumbnail.setter
    def thumbnail(self, value: windows_storage_streams.RandomAccessStreamReference) -> None: ...
    # System.String Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink::get_Id()
    @_property
    def id(self) -> str: ...
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink::put_Id(System.String)
    @id.setter
    def id(self, value: str) -> None: ...
    # Windows.Foundation.Collections.IVector`1<System.String> Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink::get_SupportedDataFormats()
    @_property
    def supported_data_formats(self) -> typing.MutableSequence[str]: ...
    # Windows.Foundation.Collections.IVector`1<System.String> Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink::get_SupportedFileTypes()
    @_property
    def supported_file_types(self) -> typing.MutableSequence[str]: ...

@typing.final
class ShareOperation(winrt.system.Object):
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::DismissUI()
    def dismiss_ui(self) -> None: ...
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::RemoveThisQuickLink()
    def remove_this_quick_link(self) -> None: ...
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::ReportCompleted()
    def report_completed(self) -> None: ...
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::ReportCompleted(Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink)
    def report_completed_with_quick_link(self, quicklink: QuickLink, /) -> None: ...
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::ReportDataRetrieved()
    def report_data_retrieved(self) -> None: ...
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::ReportError(System.String)
    def report_error(self, value: str, /) -> None: ...
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::ReportStarted()
    def report_started(self) -> None: ...
    # System.Void Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::ReportSubmittedBackgroundTask()
    def report_submitted_background_task(self) -> None: ...
    # Windows.ApplicationModel.DataTransfer.DataPackageView Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::get_Data()
    @_property
    def data(self) -> windows_applicationmodel_datatransfer.DataPackageView: ...
    # System.String Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::get_QuickLinkId()
    @_property
    def quick_link_id(self) -> str: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.Contacts.Contact> Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation::get_Contacts()
    @_property
    def contacts(self) -> typing.Sequence[windows_applicationmodel_contacts.Contact]: ...

