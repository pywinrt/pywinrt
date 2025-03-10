// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Media.h>
#include <winrt/Windows.Media.Audio.h>
#include <winrt/Windows.Media.Casting.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.MediaProperties.h>
#include <winrt/Windows.Media.Protection.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Media.Playback.h>

namespace py::proj::Windows::Media::Playback
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::AutoLoadedDisplayPropertyKind> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::FailedMediaStreamKind> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::MediaBreakInsertionMethod> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::MediaCommandEnablingRule> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::MediaPlaybackItemChangedReason> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::MediaPlaybackItemErrorCode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::MediaPlaybackSessionVideoConstrictionReason> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::MediaPlaybackState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::MediaPlayerAudioCategory> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::MediaPlayerAudioDeviceType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::MediaPlayerError> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::MediaPlayerState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::SphericalVideoProjectionMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::StereoscopicVideoRenderMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Media::Playback::TimedMetadataTrackPresentationMode> = "i";


    template<>
    struct py_type<winrt::Windows::Media::Playback::AutoLoadedDisplayPropertyKind>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.AutoLoadedDisplayPropertyKind";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "AutoLoadedDisplayPropertyKind";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::FailedMediaStreamKind>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.FailedMediaStreamKind";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "FailedMediaStreamKind";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaBreakInsertionMethod>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaBreakInsertionMethod";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaBreakInsertionMethod";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaCommandEnablingRule>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaCommandEnablingRule";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaCommandEnablingRule";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackItemChangedReason>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackItemChangedReason";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackItemChangedReason";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackItemErrorCode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackItemErrorCode";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackItemErrorCode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackSessionVideoConstrictionReason>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackSessionVideoConstrictionReason";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackSessionVideoConstrictionReason";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackState>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackState";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackState";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlayerAudioCategory>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlayerAudioCategory";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlayerAudioCategory";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlayerAudioDeviceType>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlayerAudioDeviceType";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlayerAudioDeviceType";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlayerError>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlayerError";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlayerError";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlayerState>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlayerState";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlayerState";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::SphericalVideoProjectionMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.SphericalVideoProjectionMode";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "SphericalVideoProjectionMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::StereoscopicVideoRenderMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.StereoscopicVideoRenderMode";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "StereoscopicVideoRenderMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::TimedMetadataTrackPresentationMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.TimedMetadataTrackPresentationMode";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "TimedMetadataTrackPresentationMode";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::BackgroundMediaPlayer>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.BackgroundMediaPlayer";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "BackgroundMediaPlayer";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::CurrentMediaPlaybackItemChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.CurrentMediaPlaybackItemChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "CurrentMediaPlaybackItemChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaBreak>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaBreak";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaBreak";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaBreakEndedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaBreakEndedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaBreakEndedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaBreakManager>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaBreakManager";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaBreakManager";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaBreakSchedule>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaBreakSchedule";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaBreakSchedule";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaBreakSeekedOverEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaBreakSeekedOverEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaBreakSeekedOverEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaBreakSkippedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaBreakSkippedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaBreakSkippedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaBreakStartedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaBreakStartedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaBreakStartedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaItemDisplayProperties>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaItemDisplayProperties";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaItemDisplayProperties";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackAudioTrackList>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackAudioTrackList";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackAudioTrackList";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManager>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManager";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManager";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerAutoRepeatModeReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerAutoRepeatModeReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerAutoRepeatModeReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerCommandBehavior>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerCommandBehavior";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerCommandBehavior";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerFastForwardReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerFastForwardReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerFastForwardReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerNextReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerNextReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerNextReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerPauseReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerPauseReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerPauseReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerPlayReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerPlayReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerPlayReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerPositionReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerPositionReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerPositionReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerPreviousReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerPreviousReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerPreviousReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerRateReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerRateReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerRateReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerRewindReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerRewindReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerRewindReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerShuffleReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackCommandManagerShuffleReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackCommandManagerShuffleReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackItem>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackItem";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackItem";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackItemError>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackItemError";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackItemError";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackItemFailedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackItemFailedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackItemFailedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackItemOpenedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackItemOpenedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackItemOpenedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackList>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackList";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackList";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackSession>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackSession";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackSession";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackSessionBufferingStartedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackSessionBufferingStartedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackSessionBufferingStartedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackSessionOutputDegradationPolicyState>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackSessionOutputDegradationPolicyState";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackSessionOutputDegradationPolicyState";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackSphericalVideoProjection>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackSphericalVideoProjection";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackSphericalVideoProjection";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackTimedMetadataTrackList>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackTimedMetadataTrackList";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackTimedMetadataTrackList";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlaybackVideoTrackList>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlaybackVideoTrackList";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlaybackVideoTrackList";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlayer>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlayer";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlayer";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlayerDataReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlayerDataReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlayerDataReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlayerFailedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlayerFailedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlayerFailedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlayerRateChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlayerRateChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlayerRateChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::MediaPlayerSurface>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.MediaPlayerSurface";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "MediaPlayerSurface";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::PlaybackMediaMarker>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.PlaybackMediaMarker";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "PlaybackMediaMarker";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::PlaybackMediaMarkerReachedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.PlaybackMediaMarkerReachedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "PlaybackMediaMarkerReachedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::PlaybackMediaMarkerSequence>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.PlaybackMediaMarkerSequence";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "PlaybackMediaMarkerSequence";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::TimedMetadataPresentationModeChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback.TimedMetadataPresentationModeChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "TimedMetadataPresentationModeChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::IMediaEnginePlaybackSource>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback._IMediaEnginePlaybackSource";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "_IMediaEnginePlaybackSource";
    };

    template<>
    struct py_type<winrt::Windows::Media::Playback::IMediaPlaybackSource>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.media.playback._IMediaPlaybackSource";
        static constexpr const char* module_name = "winrt.windows.media.playback";
        static constexpr const char* type_name = "_IMediaPlaybackSource";
    };
}

#if __has_include("py.Windows.Devices.Enumeration.h")
#include "py.Windows.Devices.Enumeration.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Foundation.Numerics.h")
#include "py.Windows.Foundation.Numerics.h"
#endif

#if __has_include("py.Windows.Graphics.DirectX.Direct3D11.h")
#include "py.Windows.Graphics.DirectX.Direct3D11.h"
#endif

#if __has_include("py.Windows.Media.h")
#include "py.Windows.Media.h"
#endif

#if __has_include("py.Windows.Media.Audio.h")
#include "py.Windows.Media.Audio.h"
#endif

#if __has_include("py.Windows.Media.Casting.h")
#include "py.Windows.Media.Casting.h"
#endif

#if __has_include("py.Windows.Media.Core.h")
#include "py.Windows.Media.Core.h"
#endif

#if __has_include("py.Windows.Media.MediaProperties.h")
#include "py.Windows.Media.MediaProperties.h"
#endif

#if __has_include("py.Windows.Media.Protection.h")
#include "py.Windows.Media.Protection.h"
#endif

#if __has_include("py.Windows.Storage.h")
#include "py.Windows.Storage.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

#if __has_include("py.Windows.UI.Composition.h")
#include "py.Windows.UI.Composition.h"
#endif

namespace py::impl::Windows::Media::Playback
{
}

namespace py::wrapper::Windows::Media::Playback
{
    using BackgroundMediaPlayer = py::winrt_wrapper<winrt::Windows::Media::Playback::BackgroundMediaPlayer>;
    using CurrentMediaPlaybackItemChangedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::CurrentMediaPlaybackItemChangedEventArgs>;
    using MediaBreak = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaBreak>;
    using MediaBreakEndedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaBreakEndedEventArgs>;
    using MediaBreakManager = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaBreakManager>;
    using MediaBreakSchedule = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaBreakSchedule>;
    using MediaBreakSeekedOverEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaBreakSeekedOverEventArgs>;
    using MediaBreakSkippedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaBreakSkippedEventArgs>;
    using MediaBreakStartedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaBreakStartedEventArgs>;
    using MediaItemDisplayProperties = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaItemDisplayProperties>;
    using MediaPlaybackAudioTrackList = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackAudioTrackList>;
    using MediaPlaybackCommandManager = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManager>;
    using MediaPlaybackCommandManagerAutoRepeatModeReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerAutoRepeatModeReceivedEventArgs>;
    using MediaPlaybackCommandManagerCommandBehavior = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerCommandBehavior>;
    using MediaPlaybackCommandManagerFastForwardReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerFastForwardReceivedEventArgs>;
    using MediaPlaybackCommandManagerNextReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerNextReceivedEventArgs>;
    using MediaPlaybackCommandManagerPauseReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerPauseReceivedEventArgs>;
    using MediaPlaybackCommandManagerPlayReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerPlayReceivedEventArgs>;
    using MediaPlaybackCommandManagerPositionReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerPositionReceivedEventArgs>;
    using MediaPlaybackCommandManagerPreviousReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerPreviousReceivedEventArgs>;
    using MediaPlaybackCommandManagerRateReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerRateReceivedEventArgs>;
    using MediaPlaybackCommandManagerRewindReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerRewindReceivedEventArgs>;
    using MediaPlaybackCommandManagerShuffleReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackCommandManagerShuffleReceivedEventArgs>;
    using MediaPlaybackItem = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackItem>;
    using MediaPlaybackItemError = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackItemError>;
    using MediaPlaybackItemFailedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackItemFailedEventArgs>;
    using MediaPlaybackItemOpenedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackItemOpenedEventArgs>;
    using MediaPlaybackList = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackList>;
    using MediaPlaybackSession = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackSession>;
    using MediaPlaybackSessionBufferingStartedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackSessionBufferingStartedEventArgs>;
    using MediaPlaybackSessionOutputDegradationPolicyState = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackSessionOutputDegradationPolicyState>;
    using MediaPlaybackSphericalVideoProjection = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackSphericalVideoProjection>;
    using MediaPlaybackTimedMetadataTrackList = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackTimedMetadataTrackList>;
    using MediaPlaybackVideoTrackList = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlaybackVideoTrackList>;
    using MediaPlayer = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlayer>;
    using MediaPlayerDataReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlayerDataReceivedEventArgs>;
    using MediaPlayerFailedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlayerFailedEventArgs>;
    using MediaPlayerRateChangedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlayerRateChangedEventArgs>;
    using MediaPlayerSurface = py::winrt_wrapper<winrt::Windows::Media::Playback::MediaPlayerSurface>;
    using PlaybackMediaMarker = py::winrt_wrapper<winrt::Windows::Media::Playback::PlaybackMediaMarker>;
    using PlaybackMediaMarkerReachedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::PlaybackMediaMarkerReachedEventArgs>;
    using PlaybackMediaMarkerSequence = py::winrt_wrapper<winrt::Windows::Media::Playback::PlaybackMediaMarkerSequence>;
    using TimedMetadataPresentationModeChangedEventArgs = py::winrt_wrapper<winrt::Windows::Media::Playback::TimedMetadataPresentationModeChangedEventArgs>;
    using IMediaEnginePlaybackSource = py::winrt_wrapper<winrt::Windows::Media::Playback::IMediaEnginePlaybackSource>;
    using IMediaPlaybackSource = py::winrt_wrapper<winrt::Windows::Media::Playback::IMediaPlaybackSource>;
}

namespace py
{
}
