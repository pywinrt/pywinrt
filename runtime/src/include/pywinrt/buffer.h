// The Python buffer protocol: the PEP 3118 format string for each element type
// of a py::ComArray<T>.

#pragma once

#include <pywinrt/prelude.h>
#include <pywinrt/traits.h>

namespace py
{
    // PEP 3118 struct formats

    template<typename T, typename = void>
    inline constexpr const char* buffer_format = 0;

    template<>
    inline constexpr const char* buffer_format<bool> = "?";

    template<>
    inline constexpr const char* buffer_format<int8_t> = "b";

    template<>
    inline constexpr const char* buffer_format<uint8_t> = "B";

    template<>
    inline constexpr const char* buffer_format<int16_t> = "h";

    template<>
    inline constexpr const char* buffer_format<uint16_t> = "H";

    template<>
    inline constexpr const char* buffer_format<int32_t> = "i";

    template<>
    inline constexpr const char* buffer_format<uint32_t> = "I";

    template<>
    inline constexpr const char* buffer_format<int64_t> = "q";

    template<>
    inline constexpr const char* buffer_format<uint64_t> = "Q";

    template<>
    inline constexpr const char* buffer_format<float> = "f";

    template<>
    inline constexpr const char* buffer_format<double> = "d";

    template<>
    inline constexpr const char* buffer_format<char16_t> = "u";

    template<>
    inline constexpr const char* buffer_format<winrt::guid> = "T{I2H8B}";

    template<>
    inline constexpr const char* buffer_format<winrt::hstring> = "P";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::IInspectable>
        = "P";

    template<typename T>
    inline constexpr const char*
        buffer_format<T, std::enable_if_t<is_class_category_v<T>>> = "P";

    template<typename T>
    inline constexpr const char*
        buffer_format<T, std::enable_if_t<is_interface_category_v<T>>> = "P";

    template<typename T>
    constexpr const char*
        buffer_format<T, std::enable_if_t<is_pinterface_category_v<T>>> = "P";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::DateTime>
        = "q";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::TimeSpan>
        = "q";
} // namespace py
