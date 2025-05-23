# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections
import winrt.windows.media.audio as windows_media_audio
import winrt.windows.storage.streams as windows_storage_streams

from winrt.windows.applicationmodel.conversationalagent import ActivationSignalDetectionConfigurationCreationStatus, ActivationSignalDetectionConfigurationRemovalResult, ActivationSignalDetectionConfigurationSetModelDataResult, ActivationSignalDetectionConfigurationStateChangeResult, ActivationSignalDetectionTrainingDataFormat, ActivationSignalDetectorKind, ActivationSignalDetectorPowerState, ConversationalAgentActivationKind, ConversationalAgentActivationResult, ConversationalAgentSessionUpdateResponse, ConversationalAgentState, ConversationalAgentSystemStateChangeType, ConversationalAgentVoiceActivationPrerequisiteKind, DetectionConfigurationAvailabilityChangeKind, DetectionConfigurationTrainingStatus, SignalDetectorResourceKind

Self = typing.TypeVar('Self')

@typing.final
class ActivationSignalDetectionConfiguration(winrt.system.Object, windows_foundation.IClosable):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ...
    # Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationTrainingStatus Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::ApplyTrainingData(Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionTrainingDataFormat,Windows.Storage.Streams.IInputStream)
    def apply_training_data(self, training_data_format: ActivationSignalDetectionTrainingDataFormat, training_data: windows_storage_streams.IInputStream, /) -> DetectionConfigurationTrainingStatus: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationTrainingStatus> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::ApplyTrainingDataAsync(Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionTrainingDataFormat,Windows.Storage.Streams.IInputStream)
    def apply_training_data_async(self, training_data_format: ActivationSignalDetectionTrainingDataFormat, training_data: windows_storage_streams.IInputStream, /) -> windows_foundation.IAsyncOperation[DetectionConfigurationTrainingStatus]: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::ClearModelData()
    def clear_model_data(self) -> None: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::ClearModelDataAsync()
    def clear_model_data_async(self) -> windows_foundation.IAsyncAction: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::ClearTrainingData()
    def clear_training_data(self) -> None: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::ClearTrainingDataAsync()
    def clear_training_data_async(self) -> windows_foundation.IAsyncAction: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::Close()
    def close(self) -> None: ...
    # Windows.Storage.Streams.IInputStream Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::GetModelData()
    def get_model_data(self) -> windows_storage_streams.IInputStream: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Storage.Streams.IInputStream> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::GetModelDataAsync()
    def get_model_data_async(self) -> windows_foundation.IAsyncOperation[windows_storage_streams.IInputStream]: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::GetModelDataType()
    def get_model_data_type(self) -> str: ...
    # Windows.Foundation.IAsyncOperation`1<System.String> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::GetModelDataTypeAsync()
    def get_model_data_type_async(self) -> windows_foundation.IAsyncOperation[str]: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::SetEnabled(System.Boolean)
    def set_enabled(self, value: bool, /) -> None: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::SetEnabledAsync(System.Boolean)
    def set_enabled_async(self, value: bool, /) -> windows_foundation.IAsyncAction: ...
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationStateChangeResult Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::SetEnabledWithResult(System.Boolean)
    def set_enabled_with_result(self, value: bool, /) -> ActivationSignalDetectionConfigurationStateChangeResult: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationStateChangeResult> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::SetEnabledWithResultAsync(System.Boolean)
    def set_enabled_with_result_async(self, value: bool, /) -> windows_foundation.IAsyncOperation[ActivationSignalDetectionConfigurationStateChangeResult]: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::SetModelData(System.String,Windows.Storage.Streams.IInputStream)
    def set_model_data(self, data_type: str, data: windows_storage_streams.IInputStream, /) -> None: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::SetModelDataAsync(System.String,Windows.Storage.Streams.IInputStream)
    def set_model_data_async(self, data_type: str, data: windows_storage_streams.IInputStream, /) -> windows_foundation.IAsyncAction: ...
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationSetModelDataResult Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::SetModelDataWithResult(System.String,Windows.Storage.Streams.IInputStream)
    def set_model_data_with_result(self, data_type: str, data: windows_storage_streams.IInputStream, /) -> ActivationSignalDetectionConfigurationSetModelDataResult: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationSetModelDataResult> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::SetModelDataWithResultAsync(System.String,Windows.Storage.Streams.IInputStream)
    def set_model_data_with_result_async(self, data_type: str, data: windows_storage_streams.IInputStream, /) -> windows_foundation.IAsyncOperation[ActivationSignalDetectionConfigurationSetModelDataResult]: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::add_AvailabilityChanged(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration,Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationAvailabilityChangedEventArgs>)
    def add_availability_changed(self, handler: windows_foundation.TypedEventHandler[ActivationSignalDetectionConfiguration, DetectionConfigurationAvailabilityChangedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::remove_AvailabilityChanged(Windows.Foundation.EventRegistrationToken)
    def remove_availability_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationAvailabilityInfo Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::get_AvailabilityInfo()
    @_property
    def availability_info(self) -> DetectionConfigurationAvailabilityInfo: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::get_DisplayName()
    @_property
    def display_name(self) -> str: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::get_IsActive()
    @_property
    def is_active(self) -> bool: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::get_ModelId()
    @_property
    def model_id(self) -> str: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::get_SignalId()
    @_property
    def signal_id(self) -> str: ...
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionTrainingDataFormat Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::get_TrainingDataFormat()
    @_property
    def training_data_format(self) -> ActivationSignalDetectionTrainingDataFormat: ...
    # System.UInt32 Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::get_TrainingStepsCompleted()
    @_property
    def training_steps_completed(self) -> winrt.system.UInt32: ...
    # System.UInt32 Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::get_TrainingStepsRemaining()
    @_property
    def training_steps_remaining(self) -> winrt.system.UInt32: ...
    # System.UInt32 Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration::get_TrainingStepCompletionMaxAllowedTime()
    @_property
    def training_step_completion_max_allowed_time(self) -> winrt.system.UInt32: ...

@typing.final
class ActivationSignalDetectionConfigurationCreationResult(winrt.system.Object):
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationCreationResult::get_Configuration()
    @_property
    def configuration(self) -> ActivationSignalDetectionConfiguration: ...
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationCreationStatus Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationCreationResult::get_Status()
    @_property
    def status(self) -> ActivationSignalDetectionConfigurationCreationStatus: ...

@typing.final
class ActivationSignalDetector(winrt.system.Object):
    # System.Void Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::CreateConfiguration(System.String,System.String,System.String)
    def create_configuration(self, signal_id: str, model_id: str, display_name: str, /) -> None: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::CreateConfigurationAsync(System.String,System.String,System.String)
    def create_configuration_async(self, signal_id: str, model_id: str, display_name: str, /) -> windows_foundation.IAsyncAction: ...
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationCreationResult Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::CreateConfigurationWithResult(System.String,System.String,System.String)
    def create_configuration_with_result(self, signal_id: str, model_id: str, display_name: str, /) -> ActivationSignalDetectionConfigurationCreationResult: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationCreationResult> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::CreateConfigurationWithResultAsync(System.String,System.String,System.String)
    def create_configuration_with_result_async(self, signal_id: str, model_id: str, display_name: str, /) -> windows_foundation.IAsyncOperation[ActivationSignalDetectionConfigurationCreationResult]: ...
    # Windows.Foundation.Collections.IVector`1<System.String> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::GetAvailableModelIdsForSignalId(System.String)
    def get_available_model_ids_for_signal_id(self, signal_id: str, /) -> typing.MutableSequence[str]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVector`1<System.String>> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::GetAvailableModelIdsForSignalIdAsync(System.String)
    def get_available_model_ids_for_signal_id_async(self, signal_id: str, /) -> windows_foundation.IAsyncOperation[typing.MutableSequence[str]]: ...
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::GetConfiguration(System.String,System.String)
    def get_configuration(self, signal_id: str, model_id: str, /) -> ActivationSignalDetectionConfiguration: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::GetConfigurationAsync(System.String,System.String)
    def get_configuration_async(self, signal_id: str, model_id: str, /) -> windows_foundation.IAsyncOperation[ActivationSignalDetectionConfiguration]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::GetConfigurations()
    def get_configurations(self) -> typing.Sequence[ActivationSignalDetectionConfiguration]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfiguration>> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::GetConfigurationsAsync()
    def get_configurations_async(self) -> windows_foundation.IAsyncOperation[typing.Sequence[ActivationSignalDetectionConfiguration]]: ...
    # Windows.Foundation.Collections.IVectorView`1<System.String> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::GetSupportedModelIdsForSignalId(System.String)
    def get_supported_model_ids_for_signal_id(self, signal_id: str, /) -> typing.Sequence[str]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<System.String>> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::GetSupportedModelIdsForSignalIdAsync(System.String)
    def get_supported_model_ids_for_signal_id_async(self, signal_id: str, /) -> windows_foundation.IAsyncOperation[typing.Sequence[str]]: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::RemoveConfiguration(System.String,System.String)
    def remove_configuration(self, signal_id: str, model_id: str, /) -> None: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::RemoveConfigurationAsync(System.String,System.String)
    def remove_configuration_async(self, signal_id: str, model_id: str, /) -> windows_foundation.IAsyncAction: ...
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationRemovalResult Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::RemoveConfigurationWithResult(System.String,System.String)
    def remove_configuration_with_result(self, signal_id: str, model_id: str, /) -> ActivationSignalDetectionConfigurationRemovalResult: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionConfigurationRemovalResult> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::RemoveConfigurationWithResultAsync(System.String,System.String)
    def remove_configuration_with_result_async(self, signal_id: str, model_id: str, /) -> windows_foundation.IAsyncOperation[ActivationSignalDetectionConfigurationRemovalResult]: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::get_CanCreateConfigurations()
    @_property
    def can_create_configurations(self) -> bool: ...
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectorKind Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::get_Kind()
    @_property
    def kind(self) -> ActivationSignalDetectorKind: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::get_ProviderId()
    @_property
    def provider_id(self) -> str: ...
    # Windows.Foundation.Collections.IVectorView`1<System.String> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::get_SupportedModelDataTypes()
    @_property
    def supported_model_data_types(self) -> typing.Sequence[str]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectorPowerState> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::get_SupportedPowerStates()
    @_property
    def supported_power_states(self) -> typing.Sequence[ActivationSignalDetectorPowerState]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectionTrainingDataFormat> Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::get_SupportedTrainingDataFormats()
    @_property
    def supported_training_data_formats(self) -> typing.Sequence[ActivationSignalDetectionTrainingDataFormat]: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector::get_DetectorId()
    @_property
    def detector_id(self) -> str: ...

@typing.final
class ConversationalAgentDetectorManager_Static(winrt._winrt.IInspectable_Static):
    # Windows.ApplicationModel.ConversationalAgent.ConversationalAgentDetectorManager Windows.ApplicationModel.ConversationalAgent.ConversationalAgentDetectorManager::get_Default()
    @_property
    def default(cls) -> ConversationalAgentDetectorManager: ...

@typing.final
class ConversationalAgentDetectorManager(winrt.system.Object, metaclass=ConversationalAgentDetectorManager_Static):
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector Windows.ApplicationModel.ConversationalAgent.ConversationalAgentDetectorManager::GetActivationSignalDetectorFromId(System.String)
    def get_activation_signal_detector_from_id(self, detector_id: str, /) -> ActivationSignalDetector: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentDetectorManager::GetActivationSignalDetectorFromIdAsync(System.String)
    def get_activation_signal_detector_from_id_async(self, detector_id: str, /) -> windows_foundation.IAsyncOperation[ActivationSignalDetector]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentDetectorManager::GetActivationSignalDetectors(Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectorKind)
    def get_activation_signal_detectors(self, kind: ActivationSignalDetectorKind, /) -> typing.Sequence[ActivationSignalDetector]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector>> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentDetectorManager::GetActivationSignalDetectorsAsync(Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectorKind)
    def get_activation_signal_detectors_async(self, kind: ActivationSignalDetectorKind, /) -> windows_foundation.IAsyncOperation[typing.Sequence[ActivationSignalDetector]]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentDetectorManager::GetAllActivationSignalDetectors()
    def get_all_activation_signal_detectors(self) -> typing.Sequence[ActivationSignalDetector]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetector>> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentDetectorManager::GetAllActivationSignalDetectorsAsync()
    def get_all_activation_signal_detectors_async(self) -> windows_foundation.IAsyncOperation[typing.Sequence[ActivationSignalDetector]]: ...

@typing.final
class ConversationalAgentSession_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetCurrentSessionAsync()
    def get_current_session_async(cls) -> windows_foundation.IAsyncOperation[ConversationalAgentSession]: ...
    # Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetCurrentSessionSync()
    def get_current_session_sync(cls) -> ConversationalAgentSession: ...

@typing.final
class ConversationalAgentSession(winrt.system.Object, windows_foundation.IClosable, metaclass=ConversationalAgentSession_Static):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::Close()
    def close(self) -> None: ...
    # Windows.Media.Audio.AudioDeviceInputNode Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::CreateAudioDeviceInputNode(Windows.Media.Audio.AudioGraph)
    def create_audio_device_input_node(self, graph: windows_media_audio.AudioGraph, /) -> windows_media_audio.AudioDeviceInputNode: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Media.Audio.AudioDeviceInputNode> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::CreateAudioDeviceInputNodeAsync(Windows.Media.Audio.AudioGraph)
    def create_audio_device_input_node_async(self, graph: windows_media_audio.AudioGraph, /) -> windows_foundation.IAsyncOperation[windows_media_audio.AudioDeviceInputNode]: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetAudioCaptureDeviceId()
    def get_audio_capture_device_id(self) -> str: ...
    # Windows.Foundation.IAsyncOperation`1<System.String> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetAudioCaptureDeviceIdAsync()
    def get_audio_capture_device_id_async(self) -> windows_foundation.IAsyncOperation[str]: ...
    # System.Object Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetAudioClient()
    def get_audio_client(self) -> winrt.system.Object: ...
    # Windows.Foundation.IAsyncOperation`1<System.Object> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetAudioClientAsync()
    def get_audio_client_async(self) -> windows_foundation.IAsyncOperation[winrt.system.Object]: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetAudioRenderDeviceId()
    def get_audio_render_device_id(self) -> str: ...
    # Windows.Foundation.IAsyncOperation`1<System.String> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetAudioRenderDeviceIdAsync()
    def get_audio_render_device_id_async(self) -> windows_foundation.IAsyncOperation[str]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.ConversationalAgentVoiceActivationPrerequisiteKind> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetMissingPrerequisites()
    def get_missing_prerequisites(self) -> typing.Sequence[ConversationalAgentVoiceActivationPrerequisiteKind]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.ConversationalAgentVoiceActivationPrerequisiteKind>> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetMissingPrerequisitesAsync()
    def get_missing_prerequisites_async(self) -> windows_foundation.IAsyncOperation[typing.Sequence[ConversationalAgentVoiceActivationPrerequisiteKind]]: ...
    # System.UInt32 Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetSignalModelId()
    def get_signal_model_id(self) -> winrt.system.UInt32: ...
    # Windows.Foundation.IAsyncOperation`1<System.UInt32> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetSignalModelIdAsync()
    def get_signal_model_id_async(self) -> windows_foundation.IAsyncOperation[winrt.system.UInt32]: ...
    # Windows.Foundation.Collections.IVectorView`1<System.UInt32> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetSupportedSignalModelIds()
    def get_supported_signal_model_ids(self) -> typing.Sequence[winrt.system.UInt32]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<System.UInt32>> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::GetSupportedSignalModelIdsAsync()
    def get_supported_signal_model_ids_async(self) -> windows_foundation.IAsyncOperation[typing.Sequence[winrt.system.UInt32]]: ...
    # Windows.ApplicationModel.ConversationalAgent.ConversationalAgentActivationResult Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::RequestActivation(Windows.ApplicationModel.ConversationalAgent.ConversationalAgentActivationKind)
    def request_activation(self, activation_kind: ConversationalAgentActivationKind, /) -> ConversationalAgentActivationResult: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ConversationalAgentActivationResult> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::RequestActivationAsync(Windows.ApplicationModel.ConversationalAgent.ConversationalAgentActivationKind)
    def request_activation_async(self, activation_kind: ConversationalAgentActivationKind, /) -> windows_foundation.IAsyncOperation[ConversationalAgentActivationResult]: ...
    # Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSessionUpdateResponse Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::RequestAgentStateChange(Windows.ApplicationModel.ConversationalAgent.ConversationalAgentState)
    def request_agent_state_change(self, state: ConversationalAgentState, /) -> ConversationalAgentSessionUpdateResponse: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSessionUpdateResponse> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::RequestAgentStateChangeAsync(Windows.ApplicationModel.ConversationalAgent.ConversationalAgentState)
    def request_agent_state_change_async(self, state: ConversationalAgentState, /) -> windows_foundation.IAsyncOperation[ConversationalAgentSessionUpdateResponse]: ...
    # Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSessionUpdateResponse Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::RequestForegroundActivation()
    def request_foreground_activation(self) -> ConversationalAgentSessionUpdateResponse: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSessionUpdateResponse> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::RequestForegroundActivationAsync()
    def request_foreground_activation_async(self) -> windows_foundation.IAsyncOperation[ConversationalAgentSessionUpdateResponse]: ...
    # Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSessionUpdateResponse Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::RequestInterruptible(System.Boolean)
    def request_interruptible(self, interruptible: bool, /) -> ConversationalAgentSessionUpdateResponse: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSessionUpdateResponse> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::RequestInterruptibleAsync(System.Boolean)
    def request_interruptible_async(self, interruptible: bool, /) -> windows_foundation.IAsyncOperation[ConversationalAgentSessionUpdateResponse]: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::SetSignalModelId(System.UInt32)
    def set_signal_model_id(self, signal_model_id: winrt.system.UInt32, /) -> bool: ...
    # Windows.Foundation.IAsyncOperation`1<System.Boolean> Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::SetSignalModelIdAsync(System.UInt32)
    def set_signal_model_id_async(self, signal_model_id: winrt.system.UInt32, /) -> windows_foundation.IAsyncOperation[bool]: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::SetSupportLockScreenActivation(System.Boolean)
    def set_support_lock_screen_activation(self, lock_screen_activation_supported: bool, /) -> None: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::SetSupportLockScreenActivationAsync(System.Boolean)
    def set_support_lock_screen_activation_async(self, lock_screen_activation_supported: bool, /) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::add_SessionInterrupted(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession,Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSessionInterruptedEventArgs>)
    def add_session_interrupted(self, handler: windows_foundation.TypedEventHandler[ConversationalAgentSession, ConversationalAgentSessionInterruptedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::remove_SessionInterrupted(Windows.Foundation.EventRegistrationToken)
    def remove_session_interrupted(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::add_SignalDetected(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession,Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignalDetectedEventArgs>)
    def add_signal_detected(self, handler: windows_foundation.TypedEventHandler[ConversationalAgentSession, ConversationalAgentSignalDetectedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::remove_SignalDetected(Windows.Foundation.EventRegistrationToken)
    def remove_signal_detected(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::add_SystemStateChanged(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession,Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSystemStateChangedEventArgs>)
    def add_system_state_changed(self, handler: windows_foundation.TypedEventHandler[ConversationalAgentSession, ConversationalAgentSystemStateChangedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::remove_SystemStateChanged(Windows.Foundation.EventRegistrationToken)
    def remove_system_state_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.ApplicationModel.ConversationalAgent.ConversationalAgentState Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::get_AgentState()
    @_property
    def agent_state(self) -> ConversationalAgentState: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::get_IsIndicatorLightAvailable()
    @_property
    def is_indicator_light_available(self) -> bool: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::get_IsInterrupted()
    @_property
    def is_interrupted(self) -> bool: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::get_IsInterruptible()
    @_property
    def is_interruptible(self) -> bool: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::get_IsScreenAvailable()
    @_property
    def is_screen_available(self) -> bool: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::get_IsUserAuthenticated()
    @_property
    def is_user_authenticated(self) -> bool: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::get_IsVoiceActivationAvailable()
    @_property
    def is_voice_activation_available(self) -> bool: ...
    # Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSession::get_Signal()
    @_property
    def signal(self) -> ConversationalAgentSignal: ...

@typing.final
class ConversationalAgentSessionInterruptedEventArgs(winrt.system.Object):
    pass

@typing.final
class ConversationalAgentSignal(winrt.system.Object):
    # Windows.Foundation.TimeSpan Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::get_SignalStart()
    @_property
    def signal_start(self) -> datetime.timedelta: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::put_SignalStart(Windows.Foundation.TimeSpan)
    @signal_start.setter
    def signal_start(self, value: datetime.timedelta) -> None: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::get_SignalName()
    @_property
    def signal_name(self) -> str: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::put_SignalName(System.String)
    @signal_name.setter
    def signal_name(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::get_SignalId()
    @_property
    def signal_id(self) -> str: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::put_SignalId(System.String)
    @signal_id.setter
    def signal_id(self, value: str) -> None: ...
    # Windows.Foundation.TimeSpan Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::get_SignalEnd()
    @_property
    def signal_end(self) -> datetime.timedelta: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::put_SignalEnd(Windows.Foundation.TimeSpan)
    @signal_end.setter
    def signal_end(self, value: datetime.timedelta) -> None: ...
    # System.Object Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::get_SignalContext()
    @_property
    def signal_context(self) -> winrt.system.Object: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::put_SignalContext(System.Object)
    @signal_context.setter
    def signal_context(self, value: winrt.system.Object) -> None: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::get_IsSignalVerificationRequired()
    @_property
    def is_signal_verification_required(self) -> bool: ...
    # System.Void Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::put_IsSignalVerificationRequired(System.Boolean)
    @is_signal_verification_required.setter
    def is_signal_verification_required(self, value: bool) -> None: ...
    # System.String Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::get_DetectorId()
    @_property
    def detector_id(self) -> str: ...
    # Windows.ApplicationModel.ConversationalAgent.ActivationSignalDetectorKind Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSignal::get_DetectorKind()
    @_property
    def detector_kind(self) -> ActivationSignalDetectorKind: ...

@typing.final
class ConversationalAgentSignalDetectedEventArgs(winrt.system.Object):
    pass

@typing.final
class ConversationalAgentSystemStateChangedEventArgs(winrt.system.Object):
    # Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSystemStateChangeType Windows.ApplicationModel.ConversationalAgent.ConversationalAgentSystemStateChangedEventArgs::get_SystemStateChangeType()
    @_property
    def system_state_change_type(self) -> ConversationalAgentSystemStateChangeType: ...

@typing.final
class DetectionConfigurationAvailabilityChangedEventArgs(winrt.system.Object):
    # Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationAvailabilityChangeKind Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationAvailabilityChangedEventArgs::get_Kind()
    @_property
    def kind(self) -> DetectionConfigurationAvailabilityChangeKind: ...

@typing.final
class DetectionConfigurationAvailabilityInfo(winrt.system.Object):
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationAvailabilityInfo::get_HasLockScreenPermission()
    @_property
    def has_lock_screen_permission(self) -> bool: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationAvailabilityInfo::get_HasPermission()
    @_property
    def has_permission(self) -> bool: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationAvailabilityInfo::get_HasSystemResourceAccess()
    @_property
    def has_system_resource_access(self) -> bool: ...
    # System.Boolean Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationAvailabilityInfo::get_IsEnabled()
    @_property
    def is_enabled(self) -> bool: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.ConversationalAgent.SignalDetectorResourceKind> Windows.ApplicationModel.ConversationalAgent.DetectionConfigurationAvailabilityInfo::get_UnavailableSystemResources()
    @_property
    def unavailable_system_resources(self) -> typing.Sequence[SignalDetectorResourceKind]: ...

