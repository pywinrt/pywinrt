// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Graphics_Effects_0_H
#define WINRT_Windows_Graphics_Effects_0_H
WINRT_EXPORT namespace winrt::Windows::Graphics::Effects
{
    struct IGraphicsEffect;
    struct IGraphicsEffectSource;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::Graphics::Effects::IGraphicsEffect>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Graphics::Effects::IGraphicsEffectSource>{ using type = interface_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::Graphics::Effects::IGraphicsEffect> = L"Windows.Graphics.Effects.IGraphicsEffect";
    template <> inline constexpr auto& name_v<winrt::Windows::Graphics::Effects::IGraphicsEffectSource> = L"Windows.Graphics.Effects.IGraphicsEffectSource";
    template <> inline constexpr guid guid_v<winrt::Windows::Graphics::Effects::IGraphicsEffect>{ 0xCB51C0CE,0x8FE6,0x4636,{ 0xB2,0x02,0x86,0x1F,0xAA,0x07,0xD8,0xF3 } }; // CB51C0CE-8FE6-4636-B202-861FAA07D8F3
    template <> inline constexpr guid guid_v<winrt::Windows::Graphics::Effects::IGraphicsEffectSource>{ 0x2D8F9DDC,0x4339,0x4EB9,{ 0x92,0x16,0xF9,0xDE,0xB7,0x56,0x58,0xA2 } }; // 2D8F9DDC-4339-4EB9-9216-F9DEB75658A2
    template <> struct abi<winrt::Windows::Graphics::Effects::IGraphicsEffect>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Name(void**) noexcept = 0;
            virtual int32_t __stdcall put_Name(void*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Graphics::Effects::IGraphicsEffectSource>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
        };
    };
    template <typename D>
    struct consume_Windows_Graphics_Effects_IGraphicsEffect
    {
        [[nodiscard]] auto Name() const;
        auto Name(param::hstring const& name) const;
    };
    template <> struct consume<winrt::Windows::Graphics::Effects::IGraphicsEffect>
    {
        template <typename D> using type = consume_Windows_Graphics_Effects_IGraphicsEffect<D>;
    };
    template <typename D>
    struct consume_Windows_Graphics_Effects_IGraphicsEffectSource
    {
    };
    template <> struct consume<winrt::Windows::Graphics::Effects::IGraphicsEffectSource>
    {
        template <typename D> using type = consume_Windows_Graphics_Effects_IGraphicsEffectSource<D>;
    };
}
#endif
