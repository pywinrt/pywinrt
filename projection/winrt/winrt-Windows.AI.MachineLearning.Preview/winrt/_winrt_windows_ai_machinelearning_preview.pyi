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
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections
import winrt.windows.graphics.imaging as windows_graphics_imaging
import winrt.windows.storage as windows_storage
import winrt.windows.storage.streams as windows_storage_streams

from winrt.windows.ai.machinelearning.preview import FeatureElementKindPreview, LearningModelDeviceKindPreview, LearningModelFeatureKindPreview

Self = typing.TypeVar('Self')

@typing.final
class ImageVariableDescriptorPreview(winrt.system.Object, ILearningModelVariableDescriptorPreview):
    # Windows.Graphics.Imaging.BitmapPixelFormat Windows.AI.MachineLearning.Preview.ImageVariableDescriptorPreview::get_BitmapPixelFormat()
    @_property
    def bitmap_pixel_format(self) -> windows_graphics_imaging.BitmapPixelFormat: ...
    # System.UInt32 Windows.AI.MachineLearning.Preview.ImageVariableDescriptorPreview::get_Height()
    @_property
    def height(self) -> winrt.system.UInt32: ...
    # System.UInt32 Windows.AI.MachineLearning.Preview.ImageVariableDescriptorPreview::get_Width()
    @_property
    def width(self) -> winrt.system.UInt32: ...
    # System.String Windows.AI.MachineLearning.Preview.ImageVariableDescriptorPreview::get_Description()
    @_property
    def description(self) -> str: ...
    # System.Boolean Windows.AI.MachineLearning.Preview.ImageVariableDescriptorPreview::get_IsRequired()
    @_property
    def is_required(self) -> bool: ...
    # Windows.AI.MachineLearning.Preview.LearningModelFeatureKindPreview Windows.AI.MachineLearning.Preview.ImageVariableDescriptorPreview::get_ModelFeatureKind()
    @_property
    def model_feature_kind(self) -> LearningModelFeatureKindPreview: ...
    # System.String Windows.AI.MachineLearning.Preview.ImageVariableDescriptorPreview::get_Name()
    @_property
    def name(self) -> str: ...

@typing.final
class InferencingOptionsPreview(winrt.system.Object):
    # System.Boolean Windows.AI.MachineLearning.Preview.InferencingOptionsPreview::get_ReclaimMemoryAfterEvaluation()
    @_property
    def reclaim_memory_after_evaluation(self) -> bool: ...
    # System.Void Windows.AI.MachineLearning.Preview.InferencingOptionsPreview::put_ReclaimMemoryAfterEvaluation(System.Boolean)
    @reclaim_memory_after_evaluation.setter
    def reclaim_memory_after_evaluation(self, value: bool) -> None: ...
    # Windows.AI.MachineLearning.Preview.LearningModelDeviceKindPreview Windows.AI.MachineLearning.Preview.InferencingOptionsPreview::get_PreferredDeviceKind()
    @_property
    def preferred_device_kind(self) -> LearningModelDeviceKindPreview: ...
    # System.Void Windows.AI.MachineLearning.Preview.InferencingOptionsPreview::put_PreferredDeviceKind(Windows.AI.MachineLearning.Preview.LearningModelDeviceKindPreview)
    @preferred_device_kind.setter
    def preferred_device_kind(self, value: LearningModelDeviceKindPreview) -> None: ...
    # System.Boolean Windows.AI.MachineLearning.Preview.InferencingOptionsPreview::get_MinimizeMemoryAllocation()
    @_property
    def minimize_memory_allocation(self) -> bool: ...
    # System.Void Windows.AI.MachineLearning.Preview.InferencingOptionsPreview::put_MinimizeMemoryAllocation(System.Boolean)
    @minimize_memory_allocation.setter
    def minimize_memory_allocation(self, value: bool) -> None: ...
    # System.Int32 Windows.AI.MachineLearning.Preview.InferencingOptionsPreview::get_MaxBatchSize()
    @_property
    def max_batch_size(self) -> winrt.system.Int32: ...
    # System.Void Windows.AI.MachineLearning.Preview.InferencingOptionsPreview::put_MaxBatchSize(System.Int32)
    @max_batch_size.setter
    def max_batch_size(self, value: winrt.system.Int32) -> None: ...
    # System.Boolean Windows.AI.MachineLearning.Preview.InferencingOptionsPreview::get_IsTracingEnabled()
    @_property
    def is_tracing_enabled(self) -> bool: ...
    # System.Void Windows.AI.MachineLearning.Preview.InferencingOptionsPreview::put_IsTracingEnabled(System.Boolean)
    @is_tracing_enabled.setter
    def is_tracing_enabled(self, value: bool) -> None: ...

@typing.final
class LearningModelBindingPreview(winrt.system.Object, winrt._winrt.Mapping[str, winrt.system.Object]):
    def __len__(self) -> int: ...
    def __iter__(self) -> typing.Iterator[str]: ...
    def __contains__(self, key: object) -> bool: ...
    def __getitem__(self, key: str) -> winrt.system.Object: ...
    def __new__(cls: typing.Type[Self], model: LearningModelPreview) -> Self: ...
    # System.Void Windows.AI.MachineLearning.Preview.LearningModelBindingPreview::Bind(System.String,System.Object)
    # @deprecated("Use ILearningModelBinding instead of ILearningModelBindingPreview. For more info, see MSDN.")
    def bind(self, name: str, value: winrt.system.Object, /) -> None: ...
    # System.Void Windows.AI.MachineLearning.Preview.LearningModelBindingPreview::Bind(System.String,System.Object,Windows.Foundation.Collections.IPropertySet)
    # @deprecated("Use ILearningModelBinding instead of ILearningModelBindingPreview. For more info, see MSDN.")
    def bind_with_properties(self, name: str, value: winrt.system.Object, metadata: windows_foundation_collections.IPropertySet, /) -> None: ...
    # System.Void Windows.AI.MachineLearning.Preview.LearningModelBindingPreview::Clear()
    # @deprecated("Use ILearningModelBinding instead of ILearningModelBindingPreview. For more info, see MSDN.")
    def clear(self) -> None: ...
    # Windows.Foundation.Collections.IIterator`1<Windows.Foundation.Collections.IKeyValuePair`2<System.String,System.Object>> Windows.AI.MachineLearning.Preview.LearningModelBindingPreview::First()
    def first(self) -> windows_foundation_collections.IIterator[windows_foundation_collections.IKeyValuePair[str, winrt.system.Object]]: ...
    # System.Boolean Windows.AI.MachineLearning.Preview.LearningModelBindingPreview::HasKey(System.String)
    def has_key(self, key: str, /) -> bool: ...
    # System.Object Windows.AI.MachineLearning.Preview.LearningModelBindingPreview::Lookup(System.String)
    def lookup(self, key: str, /) -> winrt.system.Object: ...
    # System.Void Windows.AI.MachineLearning.Preview.LearningModelBindingPreview::Split(Windows.Foundation.Collections.IMapView`2<System.String,System.Object>&,Windows.Foundation.Collections.IMapView`2<System.String,System.Object>&)
    def split(self) -> typing.Tuple[typing.Mapping[str, winrt.system.Object], typing.Mapping[str, winrt.system.Object]]: ...
    # System.UInt32 Windows.AI.MachineLearning.Preview.LearningModelBindingPreview::get_Size()
    @_property
    def size(self) -> winrt.system.UInt32: ...

@typing.final
class LearningModelDescriptionPreview(winrt.system.Object):
    # System.String Windows.AI.MachineLearning.Preview.LearningModelDescriptionPreview::get_Author()
    @_property
    def author(self) -> str: ...
    # System.String Windows.AI.MachineLearning.Preview.LearningModelDescriptionPreview::get_Description()
    @_property
    def description(self) -> str: ...
    # System.String Windows.AI.MachineLearning.Preview.LearningModelDescriptionPreview::get_Domain()
    @_property
    def domain(self) -> str: ...
    # Windows.Foundation.Collections.IIterable`1<Windows.AI.MachineLearning.Preview.ILearningModelVariableDescriptorPreview> Windows.AI.MachineLearning.Preview.LearningModelDescriptionPreview::get_InputFeatures()
    @_property
    def input_features(self) -> typing.Iterable[ILearningModelVariableDescriptorPreview]: ...
    # Windows.Foundation.Collections.IMapView`2<System.String,System.String> Windows.AI.MachineLearning.Preview.LearningModelDescriptionPreview::get_Metadata()
    @_property
    def metadata(self) -> typing.Mapping[str, str]: ...
    # System.String Windows.AI.MachineLearning.Preview.LearningModelDescriptionPreview::get_Name()
    @_property
    def name(self) -> str: ...
    # Windows.Foundation.Collections.IIterable`1<Windows.AI.MachineLearning.Preview.ILearningModelVariableDescriptorPreview> Windows.AI.MachineLearning.Preview.LearningModelDescriptionPreview::get_OutputFeatures()
    @_property
    def output_features(self) -> typing.Iterable[ILearningModelVariableDescriptorPreview]: ...
    # System.Int64 Windows.AI.MachineLearning.Preview.LearningModelDescriptionPreview::get_Version()
    @_property
    def version(self) -> winrt.system.Int64: ...

@typing.final
class LearningModelEvaluationResultPreview(winrt.system.Object):
    # System.String Windows.AI.MachineLearning.Preview.LearningModelEvaluationResultPreview::get_CorrelationId()
    @_property
    def correlation_id(self) -> str: ...
    # Windows.Foundation.Collections.IMapView`2<System.String,System.Object> Windows.AI.MachineLearning.Preview.LearningModelEvaluationResultPreview::get_Outputs()
    @_property
    def outputs(self) -> typing.Mapping[str, winrt.system.Object]: ...

@typing.final
class LearningModelPreview_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.AI.MachineLearning.Preview.LearningModelPreview> Windows.AI.MachineLearning.Preview.LearningModelPreview::LoadModelFromStorageFileAsync(Windows.Storage.IStorageFile)
    # @deprecated("Use ILearningModelStatics instead of ILearningModelPreviewStatics. For more info, see MSDN.")
    def load_model_from_storage_file_async(cls, model_file: windows_storage.IStorageFile, /) -> windows_foundation.IAsyncOperation[LearningModelPreview]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.AI.MachineLearning.Preview.LearningModelPreview> Windows.AI.MachineLearning.Preview.LearningModelPreview::LoadModelFromStreamAsync(Windows.Storage.Streams.IRandomAccessStreamReference)
    # @deprecated("Use ILearningModelStatics instead of ILearningModelPreviewStatics. For more info, see MSDN.")
    def load_model_from_stream_async(cls, model_stream: windows_storage_streams.IRandomAccessStreamReference, /) -> windows_foundation.IAsyncOperation[LearningModelPreview]: ...

@typing.final
class LearningModelPreview(winrt.system.Object, metaclass=LearningModelPreview_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.AI.MachineLearning.Preview.LearningModelEvaluationResultPreview> Windows.AI.MachineLearning.Preview.LearningModelPreview::EvaluateAsync(Windows.AI.MachineLearning.Preview.LearningModelBindingPreview,System.String)
    # @deprecated("Use ILearningModel instead of ILearningModelPreview. For more info, see MSDN.")
    def evaluate_async(self, binding: LearningModelBindingPreview, correlation_id: str, /) -> windows_foundation.IAsyncOperation[LearningModelEvaluationResultPreview]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.AI.MachineLearning.Preview.LearningModelEvaluationResultPreview> Windows.AI.MachineLearning.Preview.LearningModelPreview::EvaluateFeaturesAsync(Windows.Foundation.Collections.IMap`2<System.String,System.Object>,System.String)
    # @deprecated("Use ILearningModel instead of ILearningModelPreview. For more info, see MSDN.")
    def evaluate_features_async(self, features: typing.MutableMapping[str, winrt.system.Object], correlation_id: str, /) -> windows_foundation.IAsyncOperation[LearningModelEvaluationResultPreview]: ...
    # Windows.AI.MachineLearning.Preview.InferencingOptionsPreview Windows.AI.MachineLearning.Preview.LearningModelPreview::get_InferencingOptions()
    @_property
    def inferencing_options(self) -> InferencingOptionsPreview: ...
    # System.Void Windows.AI.MachineLearning.Preview.LearningModelPreview::put_InferencingOptions(Windows.AI.MachineLearning.Preview.InferencingOptionsPreview)
    @inferencing_options.setter
    def inferencing_options(self, value: InferencingOptionsPreview) -> None: ...
    # Windows.AI.MachineLearning.Preview.LearningModelDescriptionPreview Windows.AI.MachineLearning.Preview.LearningModelPreview::get_Description()
    @_property
    def description(self) -> LearningModelDescriptionPreview: ...

@typing.final
class LearningModelVariableDescriptorPreview(winrt.system.Object, ILearningModelVariableDescriptorPreview):
    # System.String Windows.AI.MachineLearning.Preview.LearningModelVariableDescriptorPreview::get_Description()
    @_property
    def description(self) -> str: ...
    # System.Boolean Windows.AI.MachineLearning.Preview.LearningModelVariableDescriptorPreview::get_IsRequired()
    @_property
    def is_required(self) -> bool: ...
    # Windows.AI.MachineLearning.Preview.LearningModelFeatureKindPreview Windows.AI.MachineLearning.Preview.LearningModelVariableDescriptorPreview::get_ModelFeatureKind()
    @_property
    def model_feature_kind(self) -> LearningModelFeatureKindPreview: ...
    # System.String Windows.AI.MachineLearning.Preview.LearningModelVariableDescriptorPreview::get_Name()
    @_property
    def name(self) -> str: ...

@typing.final
class MapVariableDescriptorPreview(winrt.system.Object, ILearningModelVariableDescriptorPreview):
    # System.String Windows.AI.MachineLearning.Preview.MapVariableDescriptorPreview::get_Description()
    @_property
    def description(self) -> str: ...
    # System.Boolean Windows.AI.MachineLearning.Preview.MapVariableDescriptorPreview::get_IsRequired()
    @_property
    def is_required(self) -> bool: ...
    # Windows.AI.MachineLearning.Preview.LearningModelFeatureKindPreview Windows.AI.MachineLearning.Preview.MapVariableDescriptorPreview::get_ModelFeatureKind()
    @_property
    def model_feature_kind(self) -> LearningModelFeatureKindPreview: ...
    # System.String Windows.AI.MachineLearning.Preview.MapVariableDescriptorPreview::get_Name()
    @_property
    def name(self) -> str: ...
    # Windows.AI.MachineLearning.Preview.ILearningModelVariableDescriptorPreview Windows.AI.MachineLearning.Preview.MapVariableDescriptorPreview::get_Fields()
    @_property
    def fields(self) -> ILearningModelVariableDescriptorPreview: ...
    # Windows.AI.MachineLearning.Preview.FeatureElementKindPreview Windows.AI.MachineLearning.Preview.MapVariableDescriptorPreview::get_KeyKind()
    @_property
    def key_kind(self) -> FeatureElementKindPreview: ...
    # Windows.Foundation.Collections.IIterable`1<System.Int64> Windows.AI.MachineLearning.Preview.MapVariableDescriptorPreview::get_ValidIntegerKeys()
    @_property
    def valid_integer_keys(self) -> typing.Iterable[winrt.system.Int64]: ...
    # Windows.Foundation.Collections.IIterable`1<System.String> Windows.AI.MachineLearning.Preview.MapVariableDescriptorPreview::get_ValidStringKeys()
    @_property
    def valid_string_keys(self) -> typing.Iterable[str]: ...

@typing.final
class SequenceVariableDescriptorPreview(winrt.system.Object, ILearningModelVariableDescriptorPreview):
    # System.String Windows.AI.MachineLearning.Preview.SequenceVariableDescriptorPreview::get_Description()
    @_property
    def description(self) -> str: ...
    # System.Boolean Windows.AI.MachineLearning.Preview.SequenceVariableDescriptorPreview::get_IsRequired()
    @_property
    def is_required(self) -> bool: ...
    # Windows.AI.MachineLearning.Preview.LearningModelFeatureKindPreview Windows.AI.MachineLearning.Preview.SequenceVariableDescriptorPreview::get_ModelFeatureKind()
    @_property
    def model_feature_kind(self) -> LearningModelFeatureKindPreview: ...
    # System.String Windows.AI.MachineLearning.Preview.SequenceVariableDescriptorPreview::get_Name()
    @_property
    def name(self) -> str: ...
    # Windows.AI.MachineLearning.Preview.ILearningModelVariableDescriptorPreview Windows.AI.MachineLearning.Preview.SequenceVariableDescriptorPreview::get_ElementType()
    @_property
    def element_type(self) -> ILearningModelVariableDescriptorPreview: ...

@typing.final
class TensorVariableDescriptorPreview(winrt.system.Object, ILearningModelVariableDescriptorPreview):
    # System.String Windows.AI.MachineLearning.Preview.TensorVariableDescriptorPreview::get_Description()
    @_property
    def description(self) -> str: ...
    # System.Boolean Windows.AI.MachineLearning.Preview.TensorVariableDescriptorPreview::get_IsRequired()
    @_property
    def is_required(self) -> bool: ...
    # Windows.AI.MachineLearning.Preview.LearningModelFeatureKindPreview Windows.AI.MachineLearning.Preview.TensorVariableDescriptorPreview::get_ModelFeatureKind()
    @_property
    def model_feature_kind(self) -> LearningModelFeatureKindPreview: ...
    # System.String Windows.AI.MachineLearning.Preview.TensorVariableDescriptorPreview::get_Name()
    @_property
    def name(self) -> str: ...
    # Windows.AI.MachineLearning.Preview.FeatureElementKindPreview Windows.AI.MachineLearning.Preview.TensorVariableDescriptorPreview::get_DataType()
    @_property
    def data_type(self) -> FeatureElementKindPreview: ...
    # Windows.Foundation.Collections.IIterable`1<System.Int64> Windows.AI.MachineLearning.Preview.TensorVariableDescriptorPreview::get_Shape()
    @_property
    def shape(self) -> typing.Iterable[winrt.system.Int64]: ...

@typing.final
class _ILearningModelVariableDescriptorPreview: ...

class ILearningModelVariableDescriptorPreview(winrt._winrt.IInspectable):
    # System.String Windows.AI.MachineLearning.Preview.ILearningModelVariableDescriptorPreview::get_Description()
    @_property
    @abstractmethod
    def description(self) -> str: ...
    # System.Boolean Windows.AI.MachineLearning.Preview.ILearningModelVariableDescriptorPreview::get_IsRequired()
    @_property
    @abstractmethod
    def is_required(self) -> bool: ...
    # Windows.AI.MachineLearning.Preview.LearningModelFeatureKindPreview Windows.AI.MachineLearning.Preview.ILearningModelVariableDescriptorPreview::get_ModelFeatureKind()
    @_property
    @abstractmethod
    def model_feature_kind(self) -> LearningModelFeatureKindPreview: ...
    # System.String Windows.AI.MachineLearning.Preview.ILearningModelVariableDescriptorPreview::get_Name()
    @_property
    @abstractmethod
    def name(self) -> str: ...

