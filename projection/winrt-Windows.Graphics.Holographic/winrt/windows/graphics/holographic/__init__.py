# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from . import _winrt_windows_graphics_holographic

class HolographicDepthReprojectionMethod(enum.IntEnum):
    DEPTH_REPROJECTION = 0
    AUTO_PLANAR = 1

class HolographicFramePresentResult(enum.IntEnum):
    SUCCESS = 0
    DEVICE_REMOVED = 1

class HolographicFramePresentWaitBehavior(enum.IntEnum):
    WAIT_FOR_FRAME_TO_FINISH = 0
    DO_NOT_WAIT_FOR_FRAME_TO_FINISH = 1

class HolographicReprojectionMode(enum.IntEnum):
    POSITION_AND_ORIENTATION = 0
    ORIENTATION_ONLY = 1
    DISABLED = 2

class HolographicSpaceUserPresence(enum.IntEnum):
    ABSENT = 0
    PRESENT_PASSIVE = 1
    PRESENT_ACTIVE = 2

class HolographicViewConfigurationKind(enum.IntEnum):
    DISPLAY = 0
    PHOTO_VIDEO_CAMERA = 1

HolographicAdapterId = _winrt_windows_graphics_holographic.HolographicAdapterId
HolographicFrameId = _winrt_windows_graphics_holographic.HolographicFrameId
HolographicStereoTransform = _winrt_windows_graphics_holographic.HolographicStereoTransform
HolographicCamera = _winrt_windows_graphics_holographic.HolographicCamera
HolographicCameraPose = _winrt_windows_graphics_holographic.HolographicCameraPose
HolographicCameraRenderingParameters = _winrt_windows_graphics_holographic.HolographicCameraRenderingParameters
HolographicCameraViewportParameters = _winrt_windows_graphics_holographic.HolographicCameraViewportParameters
HolographicDisplay = _winrt_windows_graphics_holographic.HolographicDisplay
HolographicFrame = _winrt_windows_graphics_holographic.HolographicFrame
HolographicFramePrediction = _winrt_windows_graphics_holographic.HolographicFramePrediction
HolographicFramePresentationMonitor = _winrt_windows_graphics_holographic.HolographicFramePresentationMonitor
HolographicFramePresentationReport = _winrt_windows_graphics_holographic.HolographicFramePresentationReport
HolographicFrameRenderingReport = _winrt_windows_graphics_holographic.HolographicFrameRenderingReport
HolographicFrameScanoutMonitor = _winrt_windows_graphics_holographic.HolographicFrameScanoutMonitor
HolographicFrameScanoutReport = _winrt_windows_graphics_holographic.HolographicFrameScanoutReport
HolographicQuadLayer = _winrt_windows_graphics_holographic.HolographicQuadLayer
HolographicQuadLayerUpdateParameters = _winrt_windows_graphics_holographic.HolographicQuadLayerUpdateParameters
HolographicSpace = _winrt_windows_graphics_holographic.HolographicSpace
HolographicSpaceCameraAddedEventArgs = _winrt_windows_graphics_holographic.HolographicSpaceCameraAddedEventArgs
HolographicSpaceCameraRemovedEventArgs = _winrt_windows_graphics_holographic.HolographicSpaceCameraRemovedEventArgs
HolographicViewConfiguration = _winrt_windows_graphics_holographic.HolographicViewConfiguration