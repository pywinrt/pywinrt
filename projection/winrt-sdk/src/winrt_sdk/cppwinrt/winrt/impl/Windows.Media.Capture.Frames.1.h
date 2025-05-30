// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Media_Capture_Frames_1_H
#define WINRT_Windows_Media_Capture_Frames_1_H
#include "winrt/impl/Windows.Foundation.0.h"
#include "winrt/impl/Windows.Media.Capture.Frames.0.h"
WINRT_EXPORT namespace winrt::Windows::Media::Capture::Frames
{
    struct WINRT_IMPL_EMPTY_BASES IAudioMediaFrame :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IAudioMediaFrame>
    {
        IAudioMediaFrame(std::nullptr_t = nullptr) noexcept {}
        IAudioMediaFrame(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IBufferMediaFrame :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IBufferMediaFrame>
    {
        IBufferMediaFrame(std::nullptr_t = nullptr) noexcept {}
        IBufferMediaFrame(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IDepthMediaFrame :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IDepthMediaFrame>
    {
        IDepthMediaFrame(std::nullptr_t = nullptr) noexcept {}
        IDepthMediaFrame(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IDepthMediaFrame2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IDepthMediaFrame2>
    {
        IDepthMediaFrame2(std::nullptr_t = nullptr) noexcept {}
        IDepthMediaFrame2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IDepthMediaFrameFormat :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IDepthMediaFrameFormat>
    {
        IDepthMediaFrameFormat(std::nullptr_t = nullptr) noexcept {}
        IDepthMediaFrameFormat(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IInfraredMediaFrame :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IInfraredMediaFrame>
    {
        IInfraredMediaFrame(std::nullptr_t = nullptr) noexcept {}
        IInfraredMediaFrame(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameArrivedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameArrivedEventArgs>
    {
        IMediaFrameArrivedEventArgs(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameArrivedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameFormat :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameFormat>
    {
        IMediaFrameFormat(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameFormat(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameFormat2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameFormat2>
    {
        IMediaFrameFormat2(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameFormat2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameReader :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameReader>,
        impl::require<winrt::Windows::Media::Capture::Frames::IMediaFrameReader, winrt::Windows::Foundation::IClosable>
    {
        IMediaFrameReader(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameReader(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameReader2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameReader2>
    {
        IMediaFrameReader2(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameReader2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameReference :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameReference>,
        impl::require<winrt::Windows::Media::Capture::Frames::IMediaFrameReference, winrt::Windows::Foundation::IClosable>
    {
        IMediaFrameReference(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameReference(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameReference2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameReference2>
    {
        IMediaFrameReference2(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameReference2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSource :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSource>
    {
        IMediaFrameSource(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSource(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSourceController :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSourceController>
    {
        IMediaFrameSourceController(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSourceController(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSourceController2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSourceController2>
    {
        IMediaFrameSourceController2(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSourceController2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSourceController3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSourceController3>
    {
        IMediaFrameSourceController3(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSourceController3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSourceGetPropertyResult :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSourceGetPropertyResult>
    {
        IMediaFrameSourceGetPropertyResult(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSourceGetPropertyResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSourceGroup :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSourceGroup>
    {
        IMediaFrameSourceGroup(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSourceGroup(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSourceGroupStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSourceGroupStatics>
    {
        IMediaFrameSourceGroupStatics(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSourceGroupStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSourceInfo :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSourceInfo>
    {
        IMediaFrameSourceInfo(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSourceInfo(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSourceInfo2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSourceInfo2>
    {
        IMediaFrameSourceInfo2(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSourceInfo2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSourceInfo3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSourceInfo3>
    {
        IMediaFrameSourceInfo3(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSourceInfo3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMediaFrameSourceInfo4 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMediaFrameSourceInfo4>
    {
        IMediaFrameSourceInfo4(std::nullptr_t = nullptr) noexcept {}
        IMediaFrameSourceInfo4(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMultiSourceMediaFrameArrivedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMultiSourceMediaFrameArrivedEventArgs>
    {
        IMultiSourceMediaFrameArrivedEventArgs(std::nullptr_t = nullptr) noexcept {}
        IMultiSourceMediaFrameArrivedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMultiSourceMediaFrameReader :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMultiSourceMediaFrameReader>,
        impl::require<winrt::Windows::Media::Capture::Frames::IMultiSourceMediaFrameReader, winrt::Windows::Foundation::IClosable>
    {
        IMultiSourceMediaFrameReader(std::nullptr_t = nullptr) noexcept {}
        IMultiSourceMediaFrameReader(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMultiSourceMediaFrameReader2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMultiSourceMediaFrameReader2>
    {
        IMultiSourceMediaFrameReader2(std::nullptr_t = nullptr) noexcept {}
        IMultiSourceMediaFrameReader2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IMultiSourceMediaFrameReference :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IMultiSourceMediaFrameReference>,
        impl::require<winrt::Windows::Media::Capture::Frames::IMultiSourceMediaFrameReference, winrt::Windows::Foundation::IClosable>
    {
        IMultiSourceMediaFrameReference(std::nullptr_t = nullptr) noexcept {}
        IMultiSourceMediaFrameReference(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IVideoMediaFrame :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IVideoMediaFrame>
    {
        IVideoMediaFrame(std::nullptr_t = nullptr) noexcept {}
        IVideoMediaFrame(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IVideoMediaFrameFormat :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IVideoMediaFrameFormat>
    {
        IVideoMediaFrameFormat(std::nullptr_t = nullptr) noexcept {}
        IVideoMediaFrameFormat(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
}
#endif
