// The objects that carry a value across the language boundary: the PyObject
// layouts that hold a WinRT object, struct or parameterized interface, the
// array interface the runtime allocates through, and py::IPywinrtObject, which
// finds the Python object behind a WinRT one.
//
// This is only the storage. Creating these objects (py::wrap) and converting
// their contents (py::converter) are in <pywinrt/convert.h>, and the WinRT side
// of a Python subclass of a composable type is in the runtime's compose.cpp.

#pragma once

#include <pywinrt/prelude.h>

namespace py
{
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

        /**
         * Copies @p count elements starting at @p start into a new array of
         * the same element type.
         * @param [in]  start   The index of the first element to copy.
         * @param [in]  count   The number of elements to copy, which the
         * caller has checked fit in the array. If they no longer do when they
         * are copied - a lent array taken back meanwhile - the new array is
         * empty.
         * @returns The new array or sets Python error and returns @c nullptr
         * on failure.
         */
        virtual std::unique_ptr<Array> Slice(uint32_t start, uint32_t count) noexcept
            = 0;

        /**
         * Counts an export of the elements through the buffer protocol, which
         * lasts until RemoveExport().
         *
         * An element that holds a reference cannot be replaced while there is
         * one, as a bytearray cannot be resized: whoever has the buffer - a
         * WinRT call it was passed to, which runs without the GIL, or a
         * memoryview - reads the reference, and replacing the element would
         * release it.
         */
        void AddExport() noexcept
        {
            guard lock{*this};
            export_count_++;
        }

        /**
         * Counts the end of an export that AddExport() counted.
         */
        void RemoveExport() noexcept
        {
            guard lock{*this};
            export_count_--;
        }

        // needed to avoid leaks with derived types when used with std::unique_ptr
        virtual ~Array() = default;

      protected:
        /**
         * Whether the elements are exported through the buffer protocol. The
         * caller holds the lock.
         */
        bool Exported() const noexcept
        {
            return export_count_ != 0;
        }

        /**
         * Sets the error for an element that holds a reference and was not
         * replaced because the elements are exported.
         */
        static void SetExportedError() noexcept
        {
            PyErr_SetString(
                PyExc_BufferError,
                "Existing exports of data: an element that holds a reference "
                "cannot be replaced");
        }

        /**
         * Holds the lock over an array's elements for a scope.
         *
         * Free-threading is what this is for: assigning an element that holds
         * a reference releases the one it replaces, so a thread still reading
         * that one would read freed memory. Under the GIL only one thread
         * touches an array at a time, and the guard compiles away.
         *
         * The lock covers copying an element's bytes and duplicating what
         * they refer to, and never a conversion, which can run Python: a
         * PyMutex is not recursive, and Python run under it could come back to
         * the same array. So an element is copied out under the lock and
         * converted after it, and a value is converted first and swapped in
         * under it, with the element it replaces released afterwards. Nor is
         * a Python error set under it, since making the exception can run a
         * collection and with it a finalizer.
         *
         * The buffer protocol is not locked. An array whose elements hold
         * references exports them read-only, so the worst a write racing an
         * assignment can do is tear a value that has no references in it, as
         * with a bytearray; and such an element is not replaced while it is
         * exported (AddExport()).
         */
        class guard
        {
          public:
            guard(guard const&) = delete;
            guard& operator=(guard const&) = delete;

#ifdef Py_GIL_DISABLED
            explicit guard(Array& array) noexcept : mutex_(&array.mutex_)
            {
                PyMutex_Lock(mutex_);
            }

            ~guard()
            {
                PyMutex_Unlock(mutex_);
            }

          private:
            PyMutex* mutex_;
#else
            explicit guard(Array&) noexcept
            {
            }
#endif
        };

      private:
#ifdef Py_GIL_DISABLED
        PyMutex mutex_{};
#endif
        uint32_t export_count_{};
    };

    struct
#if WINRT_IMPL_HAS_DECLSPEC_UUID
        __declspec(uuid("9c3654bc-4adc-463a-b392-d6bc9289c925"))
#endif
        IPywinrtObject : ::IUnknown
    {
        virtual int32_t __stdcall GetPyObject(PyObject*&) = 0;
        virtual int32_t __stdcall GetComposableInner(
            winrt::Windows::Foundation::IInspectable&) = 0;
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
