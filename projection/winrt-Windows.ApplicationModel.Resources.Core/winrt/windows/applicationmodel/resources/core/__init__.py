# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from . import _winrt_windows_applicationmodel_resources_core

class ResourceCandidateKind(enum.IntEnum):
    STRING = 0
    FILE = 1
    EMBEDDED_DATA = 2

class ResourceQualifierPersistence(enum.IntEnum):
    NONE = 0
    LOCAL_MACHINE = 1

ResourceLayoutInfo = _winrt_windows_applicationmodel_resources_core.ResourceLayoutInfo
NamedResource = _winrt_windows_applicationmodel_resources_core.NamedResource
ResourceCandidate = _winrt_windows_applicationmodel_resources_core.ResourceCandidate
ResourceCandidateVectorView = _winrt_windows_applicationmodel_resources_core.ResourceCandidateVectorView
winrt.system._mixin_sequence(ResourceCandidateVectorView)
ResourceContext = _winrt_windows_applicationmodel_resources_core.ResourceContext
ResourceContextLanguagesVectorView = _winrt_windows_applicationmodel_resources_core.ResourceContextLanguagesVectorView
winrt.system._mixin_sequence(ResourceContextLanguagesVectorView)
ResourceManager = _winrt_windows_applicationmodel_resources_core.ResourceManager
ResourceMap = _winrt_windows_applicationmodel_resources_core.ResourceMap
winrt.system._mixin_mapping(ResourceMap)
ResourceMapIterator = _winrt_windows_applicationmodel_resources_core.ResourceMapIterator
ResourceMapMapView = _winrt_windows_applicationmodel_resources_core.ResourceMapMapView
winrt.system._mixin_mapping(ResourceMapMapView)
ResourceMapMapViewIterator = _winrt_windows_applicationmodel_resources_core.ResourceMapMapViewIterator
ResourceQualifier = _winrt_windows_applicationmodel_resources_core.ResourceQualifier
ResourceQualifierMapView = _winrt_windows_applicationmodel_resources_core.ResourceQualifierMapView
winrt.system._mixin_mapping(ResourceQualifierMapView)
ResourceQualifierObservableMap = _winrt_windows_applicationmodel_resources_core.ResourceQualifierObservableMap
winrt.system._mixin_mutable_mapping(ResourceQualifierObservableMap)
ResourceQualifierVectorView = _winrt_windows_applicationmodel_resources_core.ResourceQualifierVectorView
winrt.system._mixin_sequence(ResourceQualifierVectorView)