# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.applicationmodel.socialinfo as windows_applicationmodel_socialinfo
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections

Self = typing.TypeVar('Self')

@typing.final
class SocialDashboardItemUpdater(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.SocialInfo.Provider.SocialDashboardItemUpdater::CommitAsync()
    # @deprecated("ISocialDashboardItemUpdater is deprecated and might not work on all platforms. For more info, see MSDN.")
    def commit_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.DateTime Windows.ApplicationModel.SocialInfo.Provider.SocialDashboardItemUpdater::get_Timestamp()
    @_property
    def timestamp(self) -> datetime.datetime: ...
    # System.Void Windows.ApplicationModel.SocialInfo.Provider.SocialDashboardItemUpdater::put_Timestamp(Windows.Foundation.DateTime)
    @timestamp.setter
    def timestamp(self, value: datetime.datetime) -> None: ...
    # Windows.ApplicationModel.SocialInfo.SocialItemThumbnail Windows.ApplicationModel.SocialInfo.Provider.SocialDashboardItemUpdater::get_Thumbnail()
    @_property
    def thumbnail(self) -> windows_applicationmodel_socialinfo.SocialItemThumbnail: ...
    # System.Void Windows.ApplicationModel.SocialInfo.Provider.SocialDashboardItemUpdater::put_Thumbnail(Windows.ApplicationModel.SocialInfo.SocialItemThumbnail)
    @thumbnail.setter
    def thumbnail(self, value: windows_applicationmodel_socialinfo.SocialItemThumbnail) -> None: ...
    # Windows.Foundation.Uri Windows.ApplicationModel.SocialInfo.Provider.SocialDashboardItemUpdater::get_TargetUri()
    @_property
    def target_uri(self) -> windows_foundation.Uri: ...
    # System.Void Windows.ApplicationModel.SocialInfo.Provider.SocialDashboardItemUpdater::put_TargetUri(Windows.Foundation.Uri)
    @target_uri.setter
    def target_uri(self, value: windows_foundation.Uri) -> None: ...
    # Windows.ApplicationModel.SocialInfo.SocialFeedContent Windows.ApplicationModel.SocialInfo.Provider.SocialDashboardItemUpdater::get_Content()
    @_property
    def content(self) -> windows_applicationmodel_socialinfo.SocialFeedContent: ...
    # System.String Windows.ApplicationModel.SocialInfo.Provider.SocialDashboardItemUpdater::get_OwnerRemoteId()
    @_property
    def owner_remote_id(self) -> str: ...

@typing.final
class SocialFeedUpdater(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.SocialInfo.Provider.SocialFeedUpdater::CommitAsync()
    # @deprecated("ISocialFeedUpdater is deprecated and might not work on all platforms. For more info, see MSDN.")
    def commit_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.Collections.IVector`1<Windows.ApplicationModel.SocialInfo.SocialFeedItem> Windows.ApplicationModel.SocialInfo.Provider.SocialFeedUpdater::get_Items()
    @_property
    def items(self) -> typing.MutableSequence[windows_applicationmodel_socialinfo.SocialFeedItem]: ...
    # Windows.ApplicationModel.SocialInfo.SocialFeedKind Windows.ApplicationModel.SocialInfo.Provider.SocialFeedUpdater::get_Kind()
    @_property
    def kind(self) -> windows_applicationmodel_socialinfo.SocialFeedKind: ...
    # System.String Windows.ApplicationModel.SocialInfo.Provider.SocialFeedUpdater::get_OwnerRemoteId()
    @_property
    def owner_remote_id(self) -> str: ...

@typing.final
class SocialInfoProviderManager_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.SocialInfo.Provider.SocialDashboardItemUpdater> Windows.ApplicationModel.SocialInfo.Provider.SocialInfoProviderManager::CreateDashboardItemUpdaterAsync(System.String)
    # @deprecated("ISocialInfoProviderManagerStatics is deprecated and might not work on all platforms. For more info, see MSDN.")
    def create_dashboard_item_updater_async(cls, owner_remote_id: str, /) -> windows_foundation.IAsyncOperation[SocialDashboardItemUpdater]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.SocialInfo.Provider.SocialFeedUpdater> Windows.ApplicationModel.SocialInfo.Provider.SocialInfoProviderManager::CreateSocialFeedUpdaterAsync(Windows.ApplicationModel.SocialInfo.SocialFeedKind,Windows.ApplicationModel.SocialInfo.SocialFeedUpdateMode,System.String)
    # @deprecated("ISocialInfoProviderManagerStatics is deprecated and might not work on all platforms. For more info, see MSDN.")
    def create_social_feed_updater_async(cls, kind: windows_applicationmodel_socialinfo.SocialFeedKind, mode: windows_applicationmodel_socialinfo.SocialFeedUpdateMode, owner_remote_id: str, /) -> windows_foundation.IAsyncOperation[SocialFeedUpdater]: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.SocialInfo.Provider.SocialInfoProviderManager::DeprovisionAsync()
    # @deprecated("ISocialInfoProviderManagerStatics is deprecated and might not work on all platforms. For more info, see MSDN.")
    def deprovision_async(cls) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncOperation`1<System.Boolean> Windows.ApplicationModel.SocialInfo.Provider.SocialInfoProviderManager::ProvisionAsync()
    # @deprecated("ISocialInfoProviderManagerStatics is deprecated and might not work on all platforms. For more info, see MSDN.")
    def provision_async(cls) -> windows_foundation.IAsyncOperation[bool]: ...
    # System.Void Windows.ApplicationModel.SocialInfo.Provider.SocialInfoProviderManager::ReportNewContentAvailable(System.String,Windows.ApplicationModel.SocialInfo.SocialFeedKind)
    # @deprecated("ISocialInfoProviderManagerStatics is deprecated and might not work on all platforms. For more info, see MSDN.")
    def report_new_content_available(cls, contact_remote_id: str, kind: windows_applicationmodel_socialinfo.SocialFeedKind, /) -> None: ...
    # System.Void Windows.ApplicationModel.SocialInfo.Provider.SocialInfoProviderManager::UpdateBadgeCountValue(System.String,System.Int32)
    # @deprecated("ISocialInfoProviderManagerStatics is deprecated and might not work on all platforms. For more info, see MSDN.")
    def update_badge_count_value(cls, item_remote_id: str, new_count: winrt.system.Int32, /) -> None: ...

@typing.final
class SocialInfoProviderManager(winrt.system.Object, metaclass=SocialInfoProviderManager_Static):
    pass

