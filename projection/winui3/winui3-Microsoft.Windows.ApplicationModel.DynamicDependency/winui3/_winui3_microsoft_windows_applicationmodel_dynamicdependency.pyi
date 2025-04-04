# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.applicationmodel as windows_applicationmodel

from winui3.microsoft.windows.applicationmodel.dynamicdependency import PackageDependencyLifetimeArtifactKind, PackageDependencyProcessorArchitectures

Self = typing.TypeVar('Self')

@typing.final
class PackageDependencyContextId:
    @_property
    def id(self) -> winrt.system.UInt64: ...
    def __new__(cls, id: winrt.system.UInt64 = 0) -> PackageDependencyContextId: ...
    def __replace__(self, /, **changes: typing.Any) -> PackageDependencyContextId: ...

@typing.final
class AddPackageDependencyOptions(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.Int32 Microsoft.Windows.ApplicationModel.DynamicDependency.AddPackageDependencyOptions::get_Rank()
    @_property
    def rank(self) -> winrt.system.Int32: ...
    # System.Void Microsoft.Windows.ApplicationModel.DynamicDependency.AddPackageDependencyOptions::put_Rank(System.Int32)
    @rank.setter
    def rank(self, value: winrt.system.Int32) -> None: ...
    # System.Boolean Microsoft.Windows.ApplicationModel.DynamicDependency.AddPackageDependencyOptions::get_PrependIfRankCollision()
    @_property
    def prepend_if_rank_collision(self) -> bool: ...
    # System.Void Microsoft.Windows.ApplicationModel.DynamicDependency.AddPackageDependencyOptions::put_PrependIfRankCollision(System.Boolean)
    @prepend_if_rank_collision.setter
    def prepend_if_rank_collision(self, value: bool) -> None: ...

@typing.final
class CreatePackageDependencyOptions(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.Boolean Microsoft.Windows.ApplicationModel.DynamicDependency.CreatePackageDependencyOptions::get_VerifyDependencyResolution()
    @_property
    def verify_dependency_resolution(self) -> bool: ...
    # System.Void Microsoft.Windows.ApplicationModel.DynamicDependency.CreatePackageDependencyOptions::put_VerifyDependencyResolution(System.Boolean)
    @verify_dependency_resolution.setter
    def verify_dependency_resolution(self, value: bool) -> None: ...
    # Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyLifetimeArtifactKind Microsoft.Windows.ApplicationModel.DynamicDependency.CreatePackageDependencyOptions::get_LifetimeArtifactKind()
    @_property
    def lifetime_artifact_kind(self) -> PackageDependencyLifetimeArtifactKind: ...
    # System.Void Microsoft.Windows.ApplicationModel.DynamicDependency.CreatePackageDependencyOptions::put_LifetimeArtifactKind(Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyLifetimeArtifactKind)
    @lifetime_artifact_kind.setter
    def lifetime_artifact_kind(self, value: PackageDependencyLifetimeArtifactKind) -> None: ...
    # System.String Microsoft.Windows.ApplicationModel.DynamicDependency.CreatePackageDependencyOptions::get_LifetimeArtifact()
    @_property
    def lifetime_artifact(self) -> str: ...
    # System.Void Microsoft.Windows.ApplicationModel.DynamicDependency.CreatePackageDependencyOptions::put_LifetimeArtifact(System.String)
    @lifetime_artifact.setter
    def lifetime_artifact(self, value: str) -> None: ...
    # Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyProcessorArchitectures Microsoft.Windows.ApplicationModel.DynamicDependency.CreatePackageDependencyOptions::get_Architectures()
    @_property
    def architectures(self) -> PackageDependencyProcessorArchitectures: ...
    # System.Void Microsoft.Windows.ApplicationModel.DynamicDependency.CreatePackageDependencyOptions::put_Architectures(Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyProcessorArchitectures)
    @architectures.setter
    def architectures(self, value: PackageDependencyProcessorArchitectures) -> None: ...

@typing.final
class PackageDependency_Static(winrt._winrt.IInspectable_Static):
    # Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::Create(System.String,Windows.ApplicationModel.PackageVersion)
    def create(cls, package_family_name: str, min_version: typing.Union[windows_applicationmodel.PackageVersion, typing.Tuple[winrt.system.UInt16, winrt.system.UInt16, winrt.system.UInt16, winrt.system.UInt16]], /) -> PackageDependency: ...
    # Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::Create(System.String,Windows.ApplicationModel.PackageVersion,Microsoft.Windows.ApplicationModel.DynamicDependency.CreatePackageDependencyOptions)
    def create2(cls, package_family_name: str, min_version: typing.Union[windows_applicationmodel.PackageVersion, typing.Tuple[winrt.system.UInt16, winrt.system.UInt16, winrt.system.UInt16, winrt.system.UInt16]], options: CreatePackageDependencyOptions, /) -> PackageDependency: ...
    # Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::CreateForSystem(System.String,Windows.ApplicationModel.PackageVersion,Microsoft.Windows.ApplicationModel.DynamicDependency.CreatePackageDependencyOptions)
    def create_for_system(cls, package_family_name: str, min_version: typing.Union[windows_applicationmodel.PackageVersion, typing.Tuple[winrt.system.UInt16, winrt.system.UInt16, winrt.system.UInt16, winrt.system.UInt16]], options: CreatePackageDependencyOptions, /) -> PackageDependency: ...
    # Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::GetFromId(System.String)
    def get_from_id(cls, id: str, /) -> PackageDependency: ...
    # Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::GetFromIdForSystem(System.String)
    def get_from_id_for_system(cls, id: str, /) -> PackageDependency: ...
    # System.UInt32 Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::get_GenerationId()
    @_property
    def generation_id(cls) -> winrt.system.UInt32: ...
    # System.UInt32 Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::get_PackageGraphRevisionId()
    @_property
    def package_graph_revision_id(cls) -> winrt.system.UInt32: ...

@typing.final
class PackageDependency(winrt.system.Object, metaclass=PackageDependency_Static):
    # Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyContext Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::Add()
    def add(self) -> PackageDependencyContext: ...
    # Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyContext Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::Add(Microsoft.Windows.ApplicationModel.DynamicDependency.AddPackageDependencyOptions)
    def add2(self, options: AddPackageDependencyOptions, /) -> PackageDependencyContext: ...
    # System.Void Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::Delete()
    def delete(self) -> None: ...
    # System.String Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependency::get_Id()
    @_property
    def id(self) -> str: ...

@typing.final
class PackageDependencyContext(winrt.system.Object):
    def __new__(cls: typing.Type[Self], context_id: typing.Union[PackageDependencyContextId, typing.Tuple[winrt.system.UInt64]]) -> Self: ...
    # System.Void Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyContext::Remove()
    def remove(self) -> None: ...
    # Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyContextId Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyContext::get_ContextId()
    @_property
    def context_id(self) -> PackageDependencyContextId: ...
    # System.String Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyContext::get_PackageDependencyId()
    @_property
    def package_dependency_id(self) -> str: ...
    # System.String Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyContext::get_PackageFullName()
    @_property
    def package_full_name(self) -> str: ...

@typing.final
class PackageDependencyRank_Static(winrt._winrt.IInspectable_Static):
    # System.Int32 Microsoft.Windows.ApplicationModel.DynamicDependency.PackageDependencyRank::get_Default()
    @_property
    def default(cls) -> winrt.system.Int32: ...

@typing.final
class PackageDependencyRank(winrt.system.Object, metaclass=PackageDependencyRank_Static):
    pass

