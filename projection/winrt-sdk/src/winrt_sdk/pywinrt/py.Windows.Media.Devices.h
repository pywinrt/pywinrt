// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Devices.Enumeration.h")
#include "py.Windows.Devices.Enumeration.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Media.Capture.h")
#include "py.Windows.Media.Capture.h"
#endif

#if __has_include("py.Windows.Media.Devices.Core.h")
#include "py.Windows.Media.Devices.Core.h"
#endif

#if __has_include("py.Windows.Media.MediaProperties.h")
#include "py.Windows.Media.MediaProperties.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Capture.h>
#include <winrt/Windows.Media.Devices.Core.h>
#include <winrt/Windows.Media.MediaProperties.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Windows.Media.Devices.h>

namespace py::proj::Windows::Media::Devices
{}

namespace py::impl::Windows::Media::Devices
{
    struct CallControlEventHandler
    {
        static winrt::Windows::Media::Devices::CallControlEventHandler get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)](auto param0)
            {
                winrt::handle_type<py::gil_state_traits> gil_state{ PyGILState_Ensure() };

                py::pyobj_handle py_param0{ py::convert(param0) };

                py::pyobj_handle args{ PyTuple_Pack(1, py_param0.get()) };

                if (!args) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }

                py::pyobj_handle return_value{ PyObject_CallObject(delegate.callable(), args.get()) };

                if (!return_value)
                {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }
            };
        };
    };

    struct DialRequestedEventHandler
    {
        static winrt::Windows::Media::Devices::DialRequestedEventHandler get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)](auto param0, auto param1)
            {
                winrt::handle_type<py::gil_state_traits> gil_state{ PyGILState_Ensure() };

                py::pyobj_handle py_param0{ py::convert(param0) };
                py::pyobj_handle py_param1{ py::convert(param1) };

                py::pyobj_handle args{ PyTuple_Pack(2, py_param0.get(), py_param1.get()) };

                if (!args) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }

                py::pyobj_handle return_value{ PyObject_CallObject(delegate.callable(), args.get()) };

                if (!return_value)
                {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }
            };
        };
    };

    struct KeypadPressedEventHandler
    {
        static winrt::Windows::Media::Devices::KeypadPressedEventHandler get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)](auto param0, auto param1)
            {
                winrt::handle_type<py::gil_state_traits> gil_state{ PyGILState_Ensure() };

                py::pyobj_handle py_param0{ py::convert(param0) };
                py::pyobj_handle py_param1{ py::convert(param1) };

                py::pyobj_handle args{ PyTuple_Pack(2, py_param0.get(), py_param1.get()) };

                if (!args) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }

                py::pyobj_handle return_value{ PyObject_CallObject(delegate.callable(), args.get()) };

                if (!return_value)
                {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }
            };
        };
    };

    struct RedialRequestedEventHandler
    {
        static winrt::Windows::Media::Devices::RedialRequestedEventHandler get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)](auto param0, auto param1)
            {
                winrt::handle_type<py::gil_state_traits> gil_state{ PyGILState_Ensure() };

                py::pyobj_handle py_param0{ py::convert(param0) };
                py::pyobj_handle py_param1{ py::convert(param1) };

                py::pyobj_handle args{ PyTuple_Pack(2, py_param0.get(), py_param1.get()) };

                if (!args) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }

                py::pyobj_handle return_value{ PyObject_CallObject(delegate.callable(), args.get()) };

                if (!return_value)
                {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }
            };
        };
    };
}

namespace py::wrapper::Windows::Media::Devices
{
    using AdvancedPhotoCaptureSettings = py::winrt_wrapper<winrt::Windows::Media::Devices::AdvancedPhotoCaptureSettings>;
    using AdvancedPhotoControl = py::winrt_wrapper<winrt::Windows::Media::Devices::AdvancedPhotoControl>;
    using AudioDeviceController = py::winrt_wrapper<winrt::Windows::Media::Devices::AudioDeviceController>;
    using AudioDeviceModule = py::winrt_wrapper<winrt::Windows::Media::Devices::AudioDeviceModule>;
    using AudioDeviceModuleNotificationEventArgs = py::winrt_wrapper<winrt::Windows::Media::Devices::AudioDeviceModuleNotificationEventArgs>;
    using AudioDeviceModulesManager = py::winrt_wrapper<winrt::Windows::Media::Devices::AudioDeviceModulesManager>;
    using CallControl = py::winrt_wrapper<winrt::Windows::Media::Devices::CallControl>;
    using CameraOcclusionInfo = py::winrt_wrapper<winrt::Windows::Media::Devices::CameraOcclusionInfo>;
    using CameraOcclusionState = py::winrt_wrapper<winrt::Windows::Media::Devices::CameraOcclusionState>;
    using CameraOcclusionStateChangedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Devices::CameraOcclusionStateChangedEventArgs>;
    using DefaultAudioCaptureDeviceChangedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Devices::DefaultAudioCaptureDeviceChangedEventArgs>;
    using DefaultAudioRenderDeviceChangedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Devices::DefaultAudioRenderDeviceChangedEventArgs>;
    using DialRequestedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Devices::DialRequestedEventArgs>;
    using DigitalWindowBounds = py::winrt_wrapper<winrt::Windows::Media::Devices::DigitalWindowBounds>;
    using DigitalWindowCapability = py::winrt_wrapper<winrt::Windows::Media::Devices::DigitalWindowCapability>;
    using DigitalWindowControl = py::winrt_wrapper<winrt::Windows::Media::Devices::DigitalWindowControl>;
    using ExposureCompensationControl = py::winrt_wrapper<winrt::Windows::Media::Devices::ExposureCompensationControl>;
    using ExposureControl = py::winrt_wrapper<winrt::Windows::Media::Devices::ExposureControl>;
    using ExposurePriorityVideoControl = py::winrt_wrapper<winrt::Windows::Media::Devices::ExposurePriorityVideoControl>;
    using FlashControl = py::winrt_wrapper<winrt::Windows::Media::Devices::FlashControl>;
    using FocusControl = py::winrt_wrapper<winrt::Windows::Media::Devices::FocusControl>;
    using FocusSettings = py::winrt_wrapper<winrt::Windows::Media::Devices::FocusSettings>;
    using HdrVideoControl = py::winrt_wrapper<winrt::Windows::Media::Devices::HdrVideoControl>;
    using InfraredTorchControl = py::winrt_wrapper<winrt::Windows::Media::Devices::InfraredTorchControl>;
    using IsoSpeedControl = py::winrt_wrapper<winrt::Windows::Media::Devices::IsoSpeedControl>;
    using KeypadPressedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Devices::KeypadPressedEventArgs>;
    using LowLagPhotoControl = py::winrt_wrapper<winrt::Windows::Media::Devices::LowLagPhotoControl>;
    using LowLagPhotoSequenceControl = py::winrt_wrapper<winrt::Windows::Media::Devices::LowLagPhotoSequenceControl>;
    using MediaDevice = py::winrt_wrapper<winrt::Windows::Media::Devices::MediaDevice>;
    using MediaDeviceControl = py::winrt_wrapper<winrt::Windows::Media::Devices::MediaDeviceControl>;
    using MediaDeviceControlCapabilities = py::winrt_wrapper<winrt::Windows::Media::Devices::MediaDeviceControlCapabilities>;
    using ModuleCommandResult = py::winrt_wrapper<winrt::Windows::Media::Devices::ModuleCommandResult>;
    using OpticalImageStabilizationControl = py::winrt_wrapper<winrt::Windows::Media::Devices::OpticalImageStabilizationControl>;
    using PanelBasedOptimizationControl = py::winrt_wrapper<winrt::Windows::Media::Devices::PanelBasedOptimizationControl>;
    using PhotoConfirmationControl = py::winrt_wrapper<winrt::Windows::Media::Devices::PhotoConfirmationControl>;
    using RedialRequestedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Devices::RedialRequestedEventArgs>;
    using RegionOfInterest = py::winrt_wrapper<winrt::Windows::Media::Devices::RegionOfInterest>;
    using RegionsOfInterestControl = py::winrt_wrapper<winrt::Windows::Media::Devices::RegionsOfInterestControl>;
    using SceneModeControl = py::winrt_wrapper<winrt::Windows::Media::Devices::SceneModeControl>;
    using TorchControl = py::winrt_wrapper<winrt::Windows::Media::Devices::TorchControl>;
    using VideoDeviceController = py::winrt_wrapper<winrt::Windows::Media::Devices::VideoDeviceController>;
    using VideoDeviceControllerGetDevicePropertyResult = py::winrt_wrapper<winrt::Windows::Media::Devices::VideoDeviceControllerGetDevicePropertyResult>;
    using VideoTemporalDenoisingControl = py::winrt_wrapper<winrt::Windows::Media::Devices::VideoTemporalDenoisingControl>;
    using WhiteBalanceControl = py::winrt_wrapper<winrt::Windows::Media::Devices::WhiteBalanceControl>;
    using ZoomControl = py::winrt_wrapper<winrt::Windows::Media::Devices::ZoomControl>;
    using ZoomSettings = py::winrt_wrapper<winrt::Windows::Media::Devices::ZoomSettings>;
    using IDefaultAudioDeviceChangedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Devices::IDefaultAudioDeviceChangedEventArgs>;
    using IMediaDeviceController = py::winrt_wrapper<winrt::Windows::Media::Devices::IMediaDeviceController>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::AdvancedPhotoMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::AudioDeviceRole> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::AutoFocusRange> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::CameraOcclusionKind> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::CameraStreamState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::CaptureSceneMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::CaptureUse> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::ColorTemperaturePreset> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::DigitalWindowMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::FocusMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::FocusPreset> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::HdrVideoMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::InfraredTorchMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::IsoSpeedPreset> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::ManualFocusDistance> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::MediaCaptureFocusState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::MediaCaptureOptimization> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::MediaCapturePauseBehavior> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::OpticalImageStabilizationMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::RegionOfInterestType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::SendCommandStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::TelephonyKey> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::VideoDeviceControllerGetDevicePropertyStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::VideoDeviceControllerSetDevicePropertyStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::VideoTemporalDenoisingMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Devices::ZoomTransitionMode> = "i";


    template<>
    struct py_type<winrt::Windows::Media::Devices::AdvancedPhotoMode>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "AdvancedPhotoMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::AudioDeviceRole>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "AudioDeviceRole";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::AutoFocusRange>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "AutoFocusRange";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::CameraOcclusionKind>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "CameraOcclusionKind";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::CameraStreamState>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "CameraStreamState";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::CaptureSceneMode>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "CaptureSceneMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::CaptureUse>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "CaptureUse";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::ColorTemperaturePreset>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "ColorTemperaturePreset";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::DigitalWindowMode>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "DigitalWindowMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::FocusMode>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "FocusMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::FocusPreset>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "FocusPreset";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::HdrVideoMode>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "HdrVideoMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::InfraredTorchMode>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "InfraredTorchMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::IsoSpeedPreset>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "IsoSpeedPreset";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::ManualFocusDistance>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "ManualFocusDistance";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::MediaCaptureFocusState>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "MediaCaptureFocusState";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::MediaCaptureOptimization>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "MediaCaptureOptimization";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::MediaCapturePauseBehavior>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "MediaCapturePauseBehavior";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::OpticalImageStabilizationMode>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "OpticalImageStabilizationMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::RegionOfInterestType>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "RegionOfInterestType";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::SendCommandStatus>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "SendCommandStatus";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::TelephonyKey>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "TelephonyKey";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::VideoDeviceControllerGetDevicePropertyStatus>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "VideoDeviceControllerGetDevicePropertyStatus";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::VideoDeviceControllerSetDevicePropertyStatus>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "VideoDeviceControllerSetDevicePropertyStatus";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::VideoTemporalDenoisingMode>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "VideoTemporalDenoisingMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::ZoomTransitionMode>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "ZoomTransitionMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::AdvancedPhotoCaptureSettings>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "AdvancedPhotoCaptureSettings";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::AdvancedPhotoControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "AdvancedPhotoControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::AudioDeviceController>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "AudioDeviceController";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::AudioDeviceModule>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "AudioDeviceModule";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::AudioDeviceModuleNotificationEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "AudioDeviceModuleNotificationEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::AudioDeviceModulesManager>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "AudioDeviceModulesManager";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::CallControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "CallControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::CameraOcclusionInfo>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "CameraOcclusionInfo";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::CameraOcclusionState>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "CameraOcclusionState";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::CameraOcclusionStateChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "CameraOcclusionStateChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::DefaultAudioCaptureDeviceChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "DefaultAudioCaptureDeviceChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::DefaultAudioRenderDeviceChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "DefaultAudioRenderDeviceChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::DialRequestedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "DialRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::DigitalWindowBounds>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "DigitalWindowBounds";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::DigitalWindowCapability>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "DigitalWindowCapability";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::DigitalWindowControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "DigitalWindowControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::ExposureCompensationControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "ExposureCompensationControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::ExposureControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "ExposureControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::ExposurePriorityVideoControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "ExposurePriorityVideoControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::FlashControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "FlashControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::FocusControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "FocusControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::FocusSettings>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "FocusSettings";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::HdrVideoControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "HdrVideoControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::InfraredTorchControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "InfraredTorchControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::IsoSpeedControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "IsoSpeedControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::KeypadPressedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "KeypadPressedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::LowLagPhotoControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "LowLagPhotoControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::LowLagPhotoSequenceControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "LowLagPhotoSequenceControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::MediaDevice>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "MediaDevice";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::MediaDeviceControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "MediaDeviceControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::MediaDeviceControlCapabilities>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "MediaDeviceControlCapabilities";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::ModuleCommandResult>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "ModuleCommandResult";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::OpticalImageStabilizationControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "OpticalImageStabilizationControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::PanelBasedOptimizationControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "PanelBasedOptimizationControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::PhotoConfirmationControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "PhotoConfirmationControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::RedialRequestedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "RedialRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::RegionOfInterest>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "RegionOfInterest";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::RegionsOfInterestControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "RegionsOfInterestControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::SceneModeControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "SceneModeControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::TorchControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "TorchControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::VideoDeviceController>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "VideoDeviceController";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::VideoDeviceControllerGetDevicePropertyResult>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "VideoDeviceControllerGetDevicePropertyResult";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::VideoTemporalDenoisingControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "VideoTemporalDenoisingControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::WhiteBalanceControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "WhiteBalanceControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::ZoomControl>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "ZoomControl";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::ZoomSettings>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "ZoomSettings";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::IDefaultAudioDeviceChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "IDefaultAudioDeviceChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Devices::IMediaDeviceController>
    {
        static constexpr const char* module_name = "winrt.windows.media.devices";
        static constexpr const char* type_name = "IMediaDeviceController";
    };
    template <>
    struct delegate_python_type<winrt::Windows::Media::Devices::CallControlEventHandler>
    {
        using type = py::impl::Windows::Media::Devices::CallControlEventHandler;
    };

    template <>
    struct delegate_python_type<winrt::Windows::Media::Devices::DialRequestedEventHandler>
    {
        using type = py::impl::Windows::Media::Devices::DialRequestedEventHandler;
    };

    template <>
    struct delegate_python_type<winrt::Windows::Media::Devices::KeypadPressedEventHandler>
    {
        using type = py::impl::Windows::Media::Devices::KeypadPressedEventHandler;
    };

    template <>
    struct delegate_python_type<winrt::Windows::Media::Devices::RedialRequestedEventHandler>
    {
        using type = py::impl::Windows::Media::Devices::RedialRequestedEventHandler;
    };

}