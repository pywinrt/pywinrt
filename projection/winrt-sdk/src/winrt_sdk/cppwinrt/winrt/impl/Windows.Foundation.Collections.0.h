// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Foundation_Collections_0_H
#define WINRT_Windows_Foundation_Collections_0_H
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    struct EventRegistrationToken;
}
WINRT_EXPORT namespace winrt::Windows::Foundation::Collections
{
    enum class CollectionChange : int32_t
    {
        Reset = 0,
        ItemInserted = 1,
        ItemRemoved = 2,
        ItemChanged = 3,
    };
    template <typename T> struct WINRT_IMPL_EMPTY_BASES IIterable;
    template <typename T> struct WINRT_IMPL_EMPTY_BASES IIterator;
    template <typename K, typename V> struct WINRT_IMPL_EMPTY_BASES IKeyValuePair;
    template <typename K> struct WINRT_IMPL_EMPTY_BASES IMapChangedEventArgs;
    template <typename K, typename V> struct WINRT_IMPL_EMPTY_BASES IMapView;
    template <typename K, typename V> struct WINRT_IMPL_EMPTY_BASES IMap;
    template <typename K, typename V> struct WINRT_IMPL_EMPTY_BASES IObservableMap;
    template <typename T> struct WINRT_IMPL_EMPTY_BASES IObservableVector;
    struct IPropertySet;
    struct IVectorChangedEventArgs;
    template <typename T> struct WINRT_IMPL_EMPTY_BASES IVectorView;
    template <typename T> struct WINRT_IMPL_EMPTY_BASES IVector;
    struct PropertySet;
    struct StringMap;
    struct ValueSet;
    template <typename K, typename V> struct WINRT_IMPL_EMPTY_BASES MapChangedEventHandler;
    template <typename T> struct WINRT_IMPL_EMPTY_BASES VectorChangedEventHandler;
}
namespace winrt::impl
{
    template <typename T> struct category<winrt::Windows::Foundation::Collections::IIterable<T>>{ using type = generic_category<T>; };
    template <typename T> struct category<winrt::Windows::Foundation::Collections::IIterator<T>>{ using type = generic_category<T>; };
    template <typename K, typename V> struct category<winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>{ using type = generic_category<K, V>; };
    template <typename K> struct category<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>{ using type = generic_category<K>; };
    template <typename K, typename V> struct category<winrt::Windows::Foundation::Collections::IMapView<K, V>>{ using type = generic_category<K, V>; };
    template <typename K, typename V> struct category<winrt::Windows::Foundation::Collections::IMap<K, V>>{ using type = generic_category<K, V>; };
    template <typename K, typename V> struct category<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>{ using type = generic_category<K, V>; };
    template <typename T> struct category<winrt::Windows::Foundation::Collections::IObservableVector<T>>{ using type = generic_category<T>; };
    template <> struct category<winrt::Windows::Foundation::Collections::IPropertySet>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>{ using type = interface_category; };
    template <typename T> struct category<winrt::Windows::Foundation::Collections::IVectorView<T>>{ using type = generic_category<T>; };
    template <typename T> struct category<winrt::Windows::Foundation::Collections::IVector<T>>{ using type = generic_category<T>; };
    template <> struct category<winrt::Windows::Foundation::Collections::PropertySet>{ using type = class_category; };
    template <> struct category<winrt::Windows::Foundation::Collections::StringMap>{ using type = class_category; };
    template <> struct category<winrt::Windows::Foundation::Collections::ValueSet>{ using type = class_category; };
    template <> struct category<winrt::Windows::Foundation::Collections::CollectionChange>{ using type = enum_category; };
    template <typename K, typename V> struct category<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>{ using type = generic_category<K, V>; };
    template <typename T> struct category<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>{ using type = generic_category<T>; };
    template <> inline constexpr auto& name_v<winrt::Windows::Foundation::Collections::PropertySet> = L"Windows.Foundation.Collections.PropertySet";
    template <> inline constexpr auto& name_v<winrt::Windows::Foundation::Collections::StringMap> = L"Windows.Foundation.Collections.StringMap";
    template <> inline constexpr auto& name_v<winrt::Windows::Foundation::Collections::ValueSet> = L"Windows.Foundation.Collections.ValueSet";
    template <> inline constexpr auto& name_v<winrt::Windows::Foundation::Collections::CollectionChange> = L"Windows.Foundation.Collections.CollectionChange";
    template <typename T> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::IIterable<T>> = zcombine(L"Windows.Foundation.Collections.IIterable`1<", name_v<T>, L">");
    template <typename T> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::IIterator<T>> = zcombine(L"Windows.Foundation.Collections.IIterator`1<", name_v<T>, L">");
    template <typename K, typename V> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>> = zcombine(L"Windows.Foundation.Collections.IKeyValuePair`2<", name_v<K>, L", ", name_v<V>, L">");
    template <typename K> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>> = zcombine(L"Windows.Foundation.Collections.IMapChangedEventArgs`1<", name_v<K>, L">");
    template <typename K, typename V> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::IMapView<K, V>> = zcombine(L"Windows.Foundation.Collections.IMapView`2<", name_v<K>, L", ", name_v<V>, L">");
    template <typename K, typename V> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::IMap<K, V>> = zcombine(L"Windows.Foundation.Collections.IMap`2<", name_v<K>, L", ", name_v<V>, L">");
    template <typename K, typename V> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::IObservableMap<K, V>> = zcombine(L"Windows.Foundation.Collections.IObservableMap`2<", name_v<K>, L", ", name_v<V>, L">");
    template <typename T> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::IObservableVector<T>> = zcombine(L"Windows.Foundation.Collections.IObservableVector`1<", name_v<T>, L">");
    template <> inline constexpr auto& name_v<winrt::Windows::Foundation::Collections::IPropertySet> = L"Windows.Foundation.Collections.IPropertySet";
    template <> inline constexpr auto& name_v<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs> = L"Windows.Foundation.Collections.IVectorChangedEventArgs";
    template <typename T> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::IVectorView<T>> = zcombine(L"Windows.Foundation.Collections.IVectorView`1<", name_v<T>, L">");
    template <typename T> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::IVector<T>> = zcombine(L"Windows.Foundation.Collections.IVector`1<", name_v<T>, L">");
    template <typename K, typename V> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>> = zcombine(L"Windows.Foundation.Collections.MapChangedEventHandler`2<", name_v<K>, L", ", name_v<V>, L">");
    template <typename T> inline constexpr auto name_v<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>> = zcombine(L"Windows.Foundation.Collections.VectorChangedEventHandler`1<", name_v<T>, L">");
    template <typename T> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IIterable<T>>{ pinterface_guid<winrt::Windows::Foundation::Collections::IIterable<T>>::value };
    template <typename T> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::IIterable<T>>{ 0xFAA585EA,0x6214,0x4217,{ 0xAF,0xDA,0x7F,0x46,0xDE,0x58,0x69,0xB3 } }; // FAA585EA-6214-4217-AFDA-7F46DE5869B3
    template <typename T> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IIterator<T>>{ pinterface_guid<winrt::Windows::Foundation::Collections::IIterator<T>>::value };
    template <typename T> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::IIterator<T>>{ 0x6A79E863,0x4300,0x459A,{ 0x99,0x66,0xCB,0xB6,0x60,0x96,0x3E,0xE1 } }; // 6A79E863-4300-459A-9966-CBB660963EE1
    template <typename K, typename V> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>{ pinterface_guid<winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>::value };
    template <typename K, typename V> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>{ 0x02B51929,0xC1C4,0x4A7E,{ 0x89,0x40,0x03,0x12,0xB5,0xC1,0x85,0x00 } }; // 02B51929-C1C4-4A7E-8940-0312B5C18500
    template <typename K> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>{ pinterface_guid<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>::value };
    template <typename K> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>{ 0x9939F4DF,0x050A,0x4C0F,{ 0xAA,0x60,0x77,0x07,0x5F,0x9C,0x47,0x77 } }; // 9939F4DF-050A-4C0F-AA60-77075F9C4777
    template <typename K, typename V> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IMapView<K, V>>{ pinterface_guid<winrt::Windows::Foundation::Collections::IMapView<K, V>>::value };
    template <typename K, typename V> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::IMapView<K, V>>{ 0xE480CE40,0xA338,0x4ADA,{ 0xAD,0xCF,0x27,0x22,0x72,0xE4,0x8C,0xB9 } }; // E480CE40-A338-4ADA-ADCF-272272E48CB9
    template <typename K, typename V> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IMap<K, V>>{ pinterface_guid<winrt::Windows::Foundation::Collections::IMap<K, V>>::value };
    template <typename K, typename V> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::IMap<K, V>>{ 0x3C2925FE,0x8519,0x45C1,{ 0xAA,0x79,0x19,0x7B,0x67,0x18,0xC1,0xC1 } }; // 3C2925FE-8519-45C1-AA79-197B6718C1C1
    template <typename K, typename V> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>{ pinterface_guid<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>::value };
    template <typename K, typename V> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>{ 0x65DF2BF5,0xBF39,0x41B5,{ 0xAE,0xBC,0x5A,0x9D,0x86,0x5E,0x47,0x2B } }; // 65DF2BF5-BF39-41B5-AEBC-5A9D865E472B
    template <typename T> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IObservableVector<T>>{ pinterface_guid<winrt::Windows::Foundation::Collections::IObservableVector<T>>::value };
    template <typename T> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::IObservableVector<T>>{ 0x5917EB53,0x50B4,0x4A0D,{ 0xB3,0x09,0x65,0x86,0x2B,0x3F,0x1D,0xBC } }; // 5917EB53-50B4-4A0D-B309-65862B3F1DBC
    template <> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IPropertySet>{ 0x8A43ED9F,0xF4E6,0x4421,{ 0xAC,0xF9,0x1D,0xAB,0x29,0x86,0x82,0x0C } }; // 8A43ED9F-F4E6-4421-ACF9-1DAB2986820C
    template <> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>{ 0x575933DF,0x34FE,0x4480,{ 0xAF,0x15,0x07,0x69,0x1F,0x3D,0x5D,0x9B } }; // 575933DF-34FE-4480-AF15-07691F3D5D9B
    template <typename T> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IVectorView<T>>{ pinterface_guid<winrt::Windows::Foundation::Collections::IVectorView<T>>::value };
    template <typename T> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::IVectorView<T>>{ 0xBBE1FA4C,0xB0E3,0x4583,{ 0xBA,0xEF,0x1F,0x1B,0x2E,0x48,0x3E,0x56 } }; // BBE1FA4C-B0E3-4583-BAEF-1F1B2E483E56
    template <typename T> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::IVector<T>>{ pinterface_guid<winrt::Windows::Foundation::Collections::IVector<T>>::value };
    template <typename T> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::IVector<T>>{ 0x913337E9,0x11A1,0x4345,{ 0xA3,0xA2,0x4E,0x7F,0x95,0x6E,0x22,0x2D } }; // 913337E9-11A1-4345-A3A2-4E7F956E222D
    template <typename K, typename V> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>{ pinterface_guid<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>::value };
    template <typename K, typename V> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>{ 0x179517F3,0x94EE,0x41F8,{ 0xBD,0xDC,0x76,0x8A,0x89,0x55,0x44,0xF3 } }; // 179517F3-94EE-41F8-BDDC-768A895544F3
    template <typename T> inline constexpr guid guid_v<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>{ pinterface_guid<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>::value };
    template <typename T> inline constexpr guid generic_guid_v<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>{ 0x0C051752,0x9FBF,0x4C70,{ 0xAA,0x0C,0x0E,0x4C,0x82,0xD9,0xA7,0x61 } }; // 0C051752-9FBF-4C70-AA0C-0E4C82D9A761
    template <> struct default_interface<winrt::Windows::Foundation::Collections::PropertySet>{ using type = winrt::Windows::Foundation::Collections::IPropertySet; };
    template <> struct default_interface<winrt::Windows::Foundation::Collections::StringMap>{ using type = winrt::Windows::Foundation::Collections::IMap<hstring, hstring>; };
    template <> struct default_interface<winrt::Windows::Foundation::Collections::ValueSet>{ using type = winrt::Windows::Foundation::Collections::IPropertySet; };
    template <typename T> struct abi<winrt::Windows::Foundation::Collections::IIterable<T>>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall First(void**) noexcept = 0;
        };
    };
    template <typename T> struct abi<winrt::Windows::Foundation::Collections::IIterator<T>>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Current(arg_out<T>) noexcept = 0;
            virtual int32_t __stdcall get_HasCurrent(bool*) noexcept = 0;
            virtual int32_t __stdcall MoveNext(bool*) noexcept = 0;
            virtual int32_t __stdcall GetMany(uint32_t, arg_out<T>, uint32_t*) noexcept = 0;
        };
    };
    template <typename K, typename V> struct abi<winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Key(arg_out<K>) noexcept = 0;
            virtual int32_t __stdcall get_Value(arg_out<V>) noexcept = 0;
        };
    };
    template <typename K> struct abi<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_CollectionChange(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Key(arg_out<K>) noexcept = 0;
        };
    };
    template <typename K, typename V> struct abi<winrt::Windows::Foundation::Collections::IMapView<K, V>>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall Lookup(arg_in<K>, arg_out<V>) noexcept = 0;
            virtual int32_t __stdcall get_Size(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall HasKey(arg_in<K>, bool*) noexcept = 0;
            virtual int32_t __stdcall Split(void**, void**) noexcept = 0;
        };
    };
    template <typename K, typename V> struct abi<winrt::Windows::Foundation::Collections::IMap<K, V>>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall Lookup(arg_in<K>, arg_out<V>) noexcept = 0;
            virtual int32_t __stdcall get_Size(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall HasKey(arg_in<K>, bool*) noexcept = 0;
            virtual int32_t __stdcall GetView(void**) noexcept = 0;
            virtual int32_t __stdcall Insert(arg_in<K>, arg_in<V>, bool*) noexcept = 0;
            virtual int32_t __stdcall Remove(arg_in<K>) noexcept = 0;
            virtual int32_t __stdcall Clear() noexcept = 0;
        };
    };
    template <typename K, typename V> struct abi<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall add_MapChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_MapChanged(winrt::event_token) noexcept = 0;
        };
    };
    template <typename T> struct abi<winrt::Windows::Foundation::Collections::IObservableVector<T>>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall add_VectorChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_VectorChanged(winrt::event_token) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Foundation::Collections::IPropertySet>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
        };
    };
    template <> struct abi<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_CollectionChange(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Index(uint32_t*) noexcept = 0;
        };
    };
    template <typename T> struct abi<winrt::Windows::Foundation::Collections::IVectorView<T>>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetAt(uint32_t, arg_out<T>) noexcept = 0;
            virtual int32_t __stdcall get_Size(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall IndexOf(arg_in<T>, uint32_t*, bool*) noexcept = 0;
            virtual int32_t __stdcall GetMany(uint32_t, uint32_t, arg_out<T>, uint32_t*) noexcept = 0;
        };
    };
    template <typename T> struct abi<winrt::Windows::Foundation::Collections::IVector<T>>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetAt(uint32_t, arg_out<T>) noexcept = 0;
            virtual int32_t __stdcall get_Size(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall GetView(void**) noexcept = 0;
            virtual int32_t __stdcall IndexOf(arg_in<T>, uint32_t*, bool*) noexcept = 0;
            virtual int32_t __stdcall SetAt(uint32_t, arg_in<T>) noexcept = 0;
            virtual int32_t __stdcall InsertAt(uint32_t, arg_in<T>) noexcept = 0;
            virtual int32_t __stdcall RemoveAt(uint32_t) noexcept = 0;
            virtual int32_t __stdcall Append(arg_in<T>) noexcept = 0;
            virtual int32_t __stdcall RemoveAtEnd() noexcept = 0;
            virtual int32_t __stdcall Clear() noexcept = 0;
            virtual int32_t __stdcall GetMany(uint32_t, uint32_t, arg_out<T>, uint32_t*) noexcept = 0;
            virtual int32_t __stdcall ReplaceAll(uint32_t, arg_out<T>) noexcept = 0;
        };
    };
    template <typename K, typename V> struct abi<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>
    {
        struct WINRT_IMPL_NOVTABLE type : unknown_abi
        {
            virtual int32_t __stdcall Invoke(void*, void*) noexcept = 0;
        };
    };
    template <typename T> struct abi<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>
    {
        struct WINRT_IMPL_NOVTABLE type : unknown_abi
        {
            virtual int32_t __stdcall Invoke(void*, void*) noexcept = 0;
        };
    };
    template <typename D, typename T>
    struct consume_Windows_Foundation_Collections_IIterable
    {
        auto First() const;

        auto begin() const;
        auto end() const;
    };
    template <typename T> struct consume<winrt::Windows::Foundation::Collections::IIterable<T>>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IIterable<D, T>;
    };
    template <typename D, typename T>
    struct consume_Windows_Foundation_Collections_IIterator
    {
        [[nodiscard]] auto Current() const;
        [[nodiscard]] auto HasCurrent() const;
        auto MoveNext() const;
        auto GetMany(array_view<T> items) const;

        auto& operator++()
        {
            if (!MoveNext())
            {
                static_cast<D&>(*this) = nullptr;
            }

            return static_cast<D&>(*this);
        }

        T operator*() const
        {
            return Current();
        }

        void operator++(int)
        {
            ++(*this);
        }
    };
    template <typename T> struct consume<winrt::Windows::Foundation::Collections::IIterator<T>>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IIterator<D, T>;
    };
    template <typename D, typename K, typename V>
    struct consume_Windows_Foundation_Collections_IKeyValuePair
    {
        [[nodiscard]] auto Key() const;
        [[nodiscard]] auto Value() const;

        bool operator==(Windows::Foundation::Collections::IKeyValuePair<K, V> const& other) const
        {
            return Key() == other.Key() && Value() == other.Value();
        }

        bool operator!=(Windows::Foundation::Collections::IKeyValuePair<K, V> const& other) const
        {
            return !(*this == other);
        }
    };
    template <typename K, typename V> struct consume<winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IKeyValuePair<D, K, V>;
    };
    template <typename D, typename K>
    struct consume_Windows_Foundation_Collections_IMapChangedEventArgs
    {
        [[nodiscard]] auto CollectionChange() const;
        [[nodiscard]] auto Key() const;
    };
    template <typename K> struct consume<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IMapChangedEventArgs<D, K>;
    };
    template <typename D, typename K, typename V>
    struct consume_Windows_Foundation_Collections_IMapView
    {
        auto Lookup(impl::param_type<K> const& key) const;
        [[nodiscard]] auto Size() const;
        auto HasKey(impl::param_type<K> const& key) const;
        auto Split(winrt::Windows::Foundation::Collections::IMapView<K, V>& first, winrt::Windows::Foundation::Collections::IMapView<K, V>& second) const;

        auto TryLookup(param_type<K> const& key) const
        {
            if constexpr (std::is_base_of_v<Windows::Foundation::IUnknown, V>)
            {
                V result{ nullptr };
                impl::check_hresult_allow_bounds(WINRT_IMPL_SHIM(Windows::Foundation::Collections::IMapView<K, V>)->Lookup(get_abi(key), put_abi(result)));
                return result;
            }
            else
            {
                std::optional<V> result;
                V value{ empty_value<V>() };

                if (0 == impl::check_hresult_allow_bounds(WINRT_IMPL_SHIM(Windows::Foundation::Collections::IMapView<K, V>)->Lookup(get_abi(key), put_abi(value))))
                {
                    result = std::move(value);
                }

                return result;
            }
        }
    };
    template <typename K, typename V> struct consume<winrt::Windows::Foundation::Collections::IMapView<K, V>>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IMapView<D, K, V>;
    };
    template <typename D, typename K, typename V>
    struct consume_Windows_Foundation_Collections_IMap
    {
        auto Lookup(impl::param_type<K> const& key) const;
        [[nodiscard]] auto Size() const;
        auto HasKey(impl::param_type<K> const& key) const;
        auto GetView() const;
        auto Insert(impl::param_type<K> const& key, impl::param_type<V> const& value) const;
        auto Remove(impl::param_type<K> const& key) const;
        auto Clear() const;

        auto TryLookup(param_type<K> const& key) const
        {
            if constexpr (std::is_base_of_v<Windows::Foundation::IUnknown, V>)
            {
                V result{ nullptr };
                impl::check_hresult_allow_bounds(WINRT_IMPL_SHIM(Windows::Foundation::Collections::IMap<K, V>)->Lookup(get_abi(key), put_abi(result)));
                return result;
            }
            else
            {
                std::optional<V> result;
                V value{ empty_value<V>() };

                if (0 == impl::check_hresult_allow_bounds(WINRT_IMPL_SHIM(Windows::Foundation::Collections::IMap<K, V>)->Lookup(get_abi(key), put_abi(value))))
                {
                    result = std::move(value);
                }

                return result;
            }
        }

        auto TryRemove(param_type<K> const& key) const
        {
            return 0 == impl::check_hresult_allow_bounds(WINRT_IMPL_SHIM(Windows::Foundation::Collections::IMap<K, V>)->Remove(get_abi(key)));
        }
    };
    template <typename K, typename V> struct consume<winrt::Windows::Foundation::Collections::IMap<K, V>>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IMap<D, K, V>;
    };
    template <typename D, typename K, typename V>
    struct consume_Windows_Foundation_Collections_IObservableMap
    {
        auto MapChanged(winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V> const& vhnd) const;
        using MapChanged_revoker = impl::event_revoker<winrt::Windows::Foundation::Collections::IObservableMap<K, V>, &impl::abi_t<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>::remove_MapChanged>;
        [[nodiscard]] auto MapChanged(auto_revoke_t, winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V> const& vhnd) const;
        auto MapChanged(winrt::event_token const& token) const noexcept;
    };
    template <typename K, typename V> struct consume<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IObservableMap<D, K, V>;
    };
    template <typename D, typename T>
    struct consume_Windows_Foundation_Collections_IObservableVector
    {
        auto VectorChanged(winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T> const& vhnd) const;
        using VectorChanged_revoker = impl::event_revoker<winrt::Windows::Foundation::Collections::IObservableVector<T>, &impl::abi_t<winrt::Windows::Foundation::Collections::IObservableVector<T>>::remove_VectorChanged>;
        [[nodiscard]] auto VectorChanged(auto_revoke_t, winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T> const& vhnd) const;
        auto VectorChanged(winrt::event_token const& token) const noexcept;
    };
    template <typename T> struct consume<winrt::Windows::Foundation::Collections::IObservableVector<T>>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IObservableVector<D, T>;
    };
    template <typename D>
    struct consume_Windows_Foundation_Collections_IPropertySet
    {
    };
    template <> struct consume<winrt::Windows::Foundation::Collections::IPropertySet>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IPropertySet<D>;
    };
    template <typename D>
    struct consume_Windows_Foundation_Collections_IVectorChangedEventArgs
    {
        [[nodiscard]] auto CollectionChange() const;
        [[nodiscard]] auto Index() const;
    };
    template <> struct consume<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IVectorChangedEventArgs<D>;
    };
    template <typename D, typename T>
    struct consume_Windows_Foundation_Collections_IVectorView
    {
        auto GetAt(uint32_t index) const;
        [[nodiscard]] auto Size() const;
        auto IndexOf(impl::param_type<T> const& value, uint32_t& index) const;
        auto GetMany(uint32_t startIndex, array_view<T> items) const;
    };
    template <typename T> struct consume<winrt::Windows::Foundation::Collections::IVectorView<T>>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IVectorView<D, T>;
    };
    template <typename D, typename T>
    struct consume_Windows_Foundation_Collections_IVector
    {
        auto GetAt(uint32_t index) const;
        [[nodiscard]] auto Size() const;
        auto GetView() const;
        auto IndexOf(impl::param_type<T> const& value, uint32_t& index) const;
        auto SetAt(uint32_t index, impl::param_type<T> const& value) const;
        auto InsertAt(uint32_t index, impl::param_type<T> const& value) const;
        auto RemoveAt(uint32_t index) const;
        auto Append(impl::param_type<T> const& value) const;
        auto RemoveAtEnd() const;
        auto Clear() const;
        auto GetMany(uint32_t startIndex, array_view<T> items) const;
        auto ReplaceAll(array_view<T const> items) const;
    };
    template <typename T> struct consume<winrt::Windows::Foundation::Collections::IVector<T>>
    {
        template <typename D> using type = consume_Windows_Foundation_Collections_IVector<D, T>;
    };
}
#endif
