// The objects that carry a value across the language boundary: the PyObject
// layouts that hold a WinRT object, struct or parameterized interface, the
// Python callable a delegate calls back into, the array interface the runtime
// allocates through, and py::IPywinrtObject, which finds the Python object
// behind a WinRT one.
//
// This is only the storage. Creating these objects (py::wrap) and converting
// their contents (py::converter) are in <pywinrt/convert.h>, and the WinRT side
// of a Python subclass of a composable type is in <pywinrt/compose.h>.

#pragma once

#include <pywinrt/handles.h>
#include <pywinrt/prelude.h>

namespace py
{
    struct delegate_callable
    {
        delegate_callable() noexcept = default;

        explicit delegate_callable(PyObject* callable) : _callable(callable)
        {
            Py_INCREF(_callable);
        }

        delegate_callable(delegate_callable&& other) noexcept
        {
            std::swap(_callable, other._callable);
        }

        ~delegate_callable()
        {
            auto gil = ensure_gil();
            Py_CLEAR(_callable);
        }

        PyObject* callable() const noexcept
        {
            return _callable;
        }

      private:
        PyObject* _callable{};
    };

    /**
     * Python PyObject struct for wrapping WinRT structs.
     */
    template<typename T>
    struct winrt_struct_wrapper
    {
        PyObject_HEAD
        T obj{};
    };

    /**
     * Python PyObject struct for wrapping WinRT objects and interfaces.
     *
     * @tparam T   The WinRT type to wrap (e.g. `winrt::Windows::...`).
     */
    template<typename T>
    struct winrt_wrapper
    {
        PyObject_HEAD
        /** The winrt object instance. */
        T obj{};
        static_assert(std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>);
    };

    /**
     * Python PyObject struct for wrapping WinRT parameterized interfaces.
     *
     * @tparam T   The abstract pinterface wrapper type to wrap (e.g.
     * `py::proj::Windows::...`).
     */
    template<typename T>
    struct winrt_pinterface_wrapper
        : winrt_wrapper<winrt::Windows::Foundation::IUnknown>
    {
        /** The PyWinRT member implementation for the concrete generic type. */
        std::unique_ptr<T> impl{};
    };

    /**
     * Generic WinRT array type (System.Array) implementation.
     */
    struct Array
    {
        /**
         * Allocates a new array.
         * @param [in]  size    The number of elements in the array.
         * @returns @c true on success, otherwise sets Python error and returns
         * @c false.
         */
        virtual bool Alloc(uint32_t size) noexcept = 0;

        /**
         * Gets the WinRT name of the element type of the array.
         */
        virtual std::wstring_view WinrtElementTypeName() noexcept = 0;

        /**
         * Gets the Py_buffer format string for this array type.
         *
         * The format string must be compatible with the struct module.
         */
        virtual std::string_view Format() noexcept = 0;

        /**
         * Whether an element holds references - a string, an object, or a
         * struct with a field that does - which copying its bytes does not
         * duplicate.
         */
        virtual bool HoldsReferences() noexcept = 0;

        /**
         * Gets the number of elements in the array.
         */
        virtual uint32_t Size() noexcept = 0;

        /**
         * Gets the size of a single element in bytes for this array type.
         */
        virtual size_t ValueSize() noexcept = 0;

        /**
         * Gets a pointer to the array buffer.
         */
        virtual void* Data() noexcept = 0;

        /**
         * Gets the item a @p index and converts it to a Python object.
         * @param [in]  index   The index of the item in the array.
         * @returns A new reference to a Python object or sets Python error and returns
         * @c nullptr on failure.
         */
        virtual PyObject* At(uint32_t index) noexcept = 0;

        /**
         * Converts @p item to a WinRT object and stores it in the array.
         * @param [in]  index   The index of the item in the array.
         * @param [in]  item    The Python object to convert and store.
         * @returns @c true on success, otherwise sets Python error and returns
         * @c false.
         */
        virtual bool Set(Py_ssize_t index, PyObject* item) noexcept = 0;

        // needed to avoid leaks with derived types when used with std::unique_ptr
        virtual ~Array() = default;
    };

    struct
#if WINRT_IMPL_HAS_DECLSPEC_UUID
        __declspec(uuid("9c3654bc-4adc-463a-b392-d6bc9289c925"))
#endif
        IPywinrtObject : ::IUnknown
    {
        virtual int32_t __stdcall GetPyObject(PyObject*&) = 0;
        virtual int32_t __stdcall GetComposableInner(
            winrt::Windows::Foundation::IInspectable&)
            = 0;
    };

    /**
     * Get the inner object if the object is a subclass of a composable type,
     * otherwise return the object itself.
     *
     * This is only used for calling overridable methods. When we have a Python
     * subclass of a composable type and we call super().overridable_method(),
     * we would get infinite recursion if we didn't get the inner object and use
     * that to call the method.
     */
    static inline winrt::Windows::Foundation::IInspectable get_inner_or_self(
        winrt::Windows::Foundation::IInspectable const& self) noexcept
    {
        if (auto pyobj = self.try_as<IPywinrtObject>())
        {
            winrt::Windows::Foundation::IInspectable inner{};
            if (auto ret = pyobj->GetComposableInner(inner))
            {
                winrt::throw_hresult(ret);
            }

            return inner;
        }

        return self;
    }
} // namespace py


#if !WINRT_IMPL_HAS_DECLSPEC_UUID
__CRT_UUID_DECL(
    py::IPywinrtObject,
    // clang-format off
    0x9c3654bc, 0x4adc, 0x463a, 0xb3, 0x92, 0xd6, 0xbc, 0x92, 0x89, 0xc9, 0x25
);
#endif
