// Compile-time questions about a WinRT type: what it is called, which metadata
// category it belongs to, and which Python type projects it.
//
// Everything here is evaluated by the compiler and emits no code of its own.
// py::py_type<T> and py::pinterface_python_type<T> are the two that the
// generated headers specialize; the rest answer from C++/WinRT's own metadata
// traits.

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
    struct pinterface_python_type
    {
        using abstract = void;
        using concrete = void;
    };

    template<typename T>
    struct delegate_python_type
    {
        using type = void;
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

    /**
     * Type registration for pure Python type info.
     *
     * @tparam T    A winrt type name like `winrt::Windows::Foundation::PropertyType`.
     */
    template<typename T>
    struct py_type
    {
        static constexpr std::string_view qualified_name = {};
        static constexpr const char* module_name = 0;
        static constexpr const char* type_name = 0;
    };

    template<typename T>
    inline constexpr bool is_key_value_pair_v
        = winrt::impl::is_key_value_pair<T>::value;

    template<typename T>
    struct key_value_pair_type;

    template<typename K, typename V>
    struct key_value_pair_type<
        winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>
    {
        using key_type = K;
        using value_type = V;
    };

    template<typename T>
    struct is_specialized_interface : std::false_type
    {
    };

    template<typename T>
    inline constexpr bool is_specialized_interface_v
        = is_specialized_interface<T>::value;

    template<typename TItem>
    struct is_specialized_interface<
        winrt::Windows::Foundation::Collections::IIterable<TItem>> : std::true_type
    {
    };

    template<typename TItem>
    struct is_specialized_interface<
        winrt::Windows::Foundation::Collections::IVectorView<TItem>> : std::true_type
    {
    };

    template<typename TItem>
    struct is_specialized_interface<
        winrt::Windows::Foundation::Collections::IVector<TItem>> : std::true_type
    {
    };

    template<typename TKey, typename TValue>
    struct is_specialized_interface<
        winrt::Windows::Foundation::Collections::IMapView<TKey, TValue>>
        : std::true_type
    {
    };

    template<typename TKey, typename TValue>
    struct is_specialized_interface<
        winrt::Windows::Foundation::Collections::IMap<TKey, TValue>> : std::true_type
    {
    };

    template<typename T>
    struct is_specialized_interface<winrt::Windows::Foundation::IReference<T>>
        : std::true_type
    {
    };
} // namespace py
