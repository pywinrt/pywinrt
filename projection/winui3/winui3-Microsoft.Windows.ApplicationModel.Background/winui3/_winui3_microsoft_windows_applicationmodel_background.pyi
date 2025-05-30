# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.applicationmodel.background as windows_applicationmodel_background

Self = typing.TypeVar('Self')

@typing.final
class BackgroundTaskBuilder(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.Void Microsoft.Windows.ApplicationModel.Background.BackgroundTaskBuilder::AddCondition(Windows.ApplicationModel.Background.IBackgroundCondition)
    def add_condition(self, condition: windows_applicationmodel_background.IBackgroundCondition, /) -> None: ...
    # Windows.ApplicationModel.Background.BackgroundTaskRegistration Microsoft.Windows.ApplicationModel.Background.BackgroundTaskBuilder::Register()
    def register(self) -> windows_applicationmodel_background.BackgroundTaskRegistration: ...
    # Windows.ApplicationModel.Background.BackgroundTaskRegistration Microsoft.Windows.ApplicationModel.Background.BackgroundTaskBuilder::Register(System.String)
    def register2(self, name: str, /) -> windows_applicationmodel_background.BackgroundTaskRegistration: ...
    # System.Void Microsoft.Windows.ApplicationModel.Background.BackgroundTaskBuilder::SetTaskEntryPointClsid(System.Guid)
    def set_task_entry_point_clsid(self, cls_id: _uuid.UUID, /) -> None: ...
    # System.Void Microsoft.Windows.ApplicationModel.Background.BackgroundTaskBuilder::SetTrigger(Windows.ApplicationModel.Background.IBackgroundTrigger)
    def set_trigger(self, trigger: windows_applicationmodel_background.IBackgroundTrigger, /) -> None: ...
    # Windows.ApplicationModel.Background.BackgroundTaskRegistrationGroup Microsoft.Windows.ApplicationModel.Background.BackgroundTaskBuilder::get_TaskGroup()
    @_property
    def task_group(self) -> windows_applicationmodel_background.BackgroundTaskRegistrationGroup: ...
    # System.Void Microsoft.Windows.ApplicationModel.Background.BackgroundTaskBuilder::put_TaskGroup(Windows.ApplicationModel.Background.BackgroundTaskRegistrationGroup)
    @task_group.setter
    def task_group(self, value: windows_applicationmodel_background.BackgroundTaskRegistrationGroup) -> None: ...
    # System.String Microsoft.Windows.ApplicationModel.Background.BackgroundTaskBuilder::get_Name()
    @_property
    def name(self) -> str: ...
    # System.Void Microsoft.Windows.ApplicationModel.Background.BackgroundTaskBuilder::put_Name(System.String)
    @name.setter
    def name(self, value: str) -> None: ...

