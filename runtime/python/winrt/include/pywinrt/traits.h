// Compile-time questions about a WinRT type: what it is called and which
// metadata category it belongs to.
//
// Everything here is evaluated by the compiler and emits no code of its own,
// and all of it answers from C++/WinRT's own metadata traits. Which Python
// type projects a WinRT type is not a compile-time question any more: the
// answer is in the table the runtime interprets, and a module asks for it by
// name through <pywinrt/abi.h>.

#pragma once

#include <pywinrt/prelude.h>

namespace py
{
    /**
     * Storage for the WinRT name of @p T as a NUL-terminated UTF-8 string.
     */
    template<typename T>
    inline constexpr auto type_name_storage_v
        = winrt::impl::concat(winrt::impl::to_utf8<T>(), '\0');

    /**
     * Gets the WinRT name of @p T as a NUL-terminated UTF-8 string suitable
     * for the "%s" conversion of PyErr_Format().
     *
     * This is used instead of typeid(T).name() so that the projection does not
     * need RTTI (the type descriptors it emits cost about 15% of the size of a
     * module). A WinRT name also reads better in an error message than an MSVC
     * mangled name.
     */
    template<typename T>
    constexpr char const* type_name() noexcept
    {
        return type_name_storage_v<T>.data();
    }

    template<typename T, typename = std::void_t<>>
    struct empty_instance
    {
        static T get()
        {
            return T{};
        }
    };

    template<typename T>
    struct empty_instance<T, std::void_t<decltype(T(nullptr))>>
    {
        static T get()
        {
            return T(nullptr);
        }
    };

    template<typename Category>
    struct pinterface_checker
    {
        static constexpr bool value = false;
    };

    template<template<typename... TArgs> typename T, typename... Args>
    struct pinterface_checker<T<Args...>>
    {
        static constexpr bool value = true;
    };

    template<typename T>
    constexpr bool is_basic_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::basic_category>;

    template<typename T>
    constexpr bool is_class_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::class_category>;

    template<typename T>
    constexpr bool is_delegate_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::delegate_category>;

    template<typename T>
    constexpr bool is_pdelegate_category_v
        = !std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T>
          && std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>;

    template<typename T>
    constexpr bool is_enum_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::enum_category>;

    template<typename T>
    struct is_struct_category
    {
        static constexpr bool value = false;
    };

    template<typename... Fields>
    struct is_struct_category<winrt::impl::struct_category<Fields...>>
    {
        static constexpr bool value = true;
    };

    template<typename T>
    constexpr bool is_struct_category_v
        = is_struct_category<typename winrt::impl::category<T>::type>::value;

    template<typename T>
    constexpr bool is_interface_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::interface_category>;

    template<typename T>
    constexpr bool is_pinterface_category_v
        = std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T>
          && pinterface_checker<typename winrt::impl::category<T>::type>::value;

    /**
     * A type that always evaluates to false.
     *
     * This is useful for static_assert() in template functions to catch
     * unsupported types.
     */
    template<typename T>
    struct false_type : std::false_type
    {
    };
} // namespace py
