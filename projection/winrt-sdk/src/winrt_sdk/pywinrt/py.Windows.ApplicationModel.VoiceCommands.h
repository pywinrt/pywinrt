// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.ApplicationModel.AppService.h")
#include "py.Windows.ApplicationModel.AppService.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Globalization.h")
#include "py.Windows.Globalization.h"
#endif

#if __has_include("py.Windows.Media.SpeechRecognition.h")
#include "py.Windows.Media.SpeechRecognition.h"
#endif

#if __has_include("py.Windows.Storage.h")
#include "py.Windows.Storage.h"
#endif

#include <winrt/Windows.ApplicationModel.AppService.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.Media.SpeechRecognition.h>
#include <winrt/Windows.Storage.h>

#include <winrt/Windows.ApplicationModel.VoiceCommands.h>

namespace py::proj::Windows::ApplicationModel::VoiceCommands
{}

namespace py::impl::Windows::ApplicationModel::VoiceCommands
{}

namespace py::wrapper::Windows::ApplicationModel::VoiceCommands
{
    using VoiceCommand = py::winrt_wrapper<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommand>;
    using VoiceCommandCompletedEventArgs = py::winrt_wrapper<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandCompletedEventArgs>;
    using VoiceCommandConfirmationResult = py::winrt_wrapper<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandConfirmationResult>;
    using VoiceCommandContentTile = py::winrt_wrapper<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandContentTile>;
    using VoiceCommandDefinition = py::winrt_wrapper<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandDefinition>;
    using VoiceCommandDefinitionManager = py::winrt_wrapper<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandDefinitionManager>;
    using VoiceCommandDisambiguationResult = py::winrt_wrapper<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandDisambiguationResult>;
    using VoiceCommandResponse = py::winrt_wrapper<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandResponse>;
    using VoiceCommandServiceConnection = py::winrt_wrapper<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandServiceConnection>;
    using VoiceCommandUserMessage = py::winrt_wrapper<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandUserMessage>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandCompletionReason> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandContentTileType> = "i";


    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandCompletionReason>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandCompletionReason";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandContentTileType>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandContentTileType";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommand>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommand";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandCompletedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandCompletedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandConfirmationResult>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandConfirmationResult";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandContentTile>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandContentTile";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandDefinition>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandDefinition";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandDefinitionManager>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandDefinitionManager";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandDisambiguationResult>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandDisambiguationResult";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandResponse>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandResponse";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandServiceConnection>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandServiceConnection";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::VoiceCommands::VoiceCommandUserMessage>
    {
        static constexpr const char* module_name = "winrt.windows.applicationmodel.voicecommands";
        static constexpr const char* type_name = "VoiceCommandUserMessage";
    };
}