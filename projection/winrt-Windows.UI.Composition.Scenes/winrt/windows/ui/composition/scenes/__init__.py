# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from . import _winrt_windows_ui_composition_scenes

class SceneAlphaMode(enum.IntEnum):
    OPAQUE = 0
    ALPHA_TEST = 1
    BLEND = 2

class SceneAttributeSemantic(enum.IntEnum):
    INDEX = 0
    VERTEX = 1
    NORMAL = 2
    TEX_COORD0 = 3
    TEX_COORD1 = 4
    COLOR = 5
    TANGENT = 6

class SceneComponentType(enum.IntEnum):
    MESH_RENDERER_COMPONENT = 0

class SceneWrappingMode(enum.IntEnum):
    CLAMP_TO_EDGE = 0
    MIRRORED_REPEAT = 1
    REPEAT = 2

SceneBoundingBox = _winrt_windows_ui_composition_scenes.SceneBoundingBox
SceneComponent = _winrt_windows_ui_composition_scenes.SceneComponent
SceneComponentCollection = _winrt_windows_ui_composition_scenes.SceneComponentCollection
winrt.system._mixin_mutable_sequence(SceneComponentCollection)
SceneMaterial = _winrt_windows_ui_composition_scenes.SceneMaterial
SceneMaterialInput = _winrt_windows_ui_composition_scenes.SceneMaterialInput
SceneMesh = _winrt_windows_ui_composition_scenes.SceneMesh
SceneMeshMaterialAttributeMap = _winrt_windows_ui_composition_scenes.SceneMeshMaterialAttributeMap
winrt.system._mixin_mutable_mapping(SceneMeshMaterialAttributeMap)
SceneMeshRendererComponent = _winrt_windows_ui_composition_scenes.SceneMeshRendererComponent
SceneMetallicRoughnessMaterial = _winrt_windows_ui_composition_scenes.SceneMetallicRoughnessMaterial
SceneModelTransform = _winrt_windows_ui_composition_scenes.SceneModelTransform
SceneNode = _winrt_windows_ui_composition_scenes.SceneNode
SceneNodeCollection = _winrt_windows_ui_composition_scenes.SceneNodeCollection
winrt.system._mixin_mutable_sequence(SceneNodeCollection)
SceneObject = _winrt_windows_ui_composition_scenes.SceneObject
ScenePbrMaterial = _winrt_windows_ui_composition_scenes.ScenePbrMaterial
SceneRendererComponent = _winrt_windows_ui_composition_scenes.SceneRendererComponent
SceneSurfaceMaterialInput = _winrt_windows_ui_composition_scenes.SceneSurfaceMaterialInput
SceneVisual = _winrt_windows_ui_composition_scenes.SceneVisual