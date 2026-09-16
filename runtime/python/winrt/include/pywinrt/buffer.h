// The Python buffer protocol: the PEP 3118 format string for each element type,
// the check that an incoming buffer matches the one WinRT expects, and
// py::pybuf_view, which presents a buffer to WinRT as a winrt::array_view
// without copying it.

#pragma once

#include <pywinrt/abi.h>
#include <pywinrt/convert.h>
#include <pywinrt/errors.h>
#include <pywinrt/prelude.h>
#include <pywinrt/traits.h>

namespace py
{
    template<typename T, typename = void>
    struct buffer
    {
        static bool is_compatible(Py_buffer const& /*unused*/) noexcept
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::buffer<%s>::is_compatible() is not implemented",
                type_name<T>());
            return false;
        } // namespace py
    };

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
        buffer_format<T, std::enable_if_t<is_class_category_v<T>>>
        = "P";

    template<typename T>
    inline constexpr const char*
        buffer_format<T, std::enable_if_t<is_interface_category_v<T>>>
        = "P";

    template<typename T>
    constexpr const char*
        buffer_format<T, std::enable_if_t<is_pinterface_category_v<T>>>
        = "P";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::DateTime>
        = "q";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::TimeSpan>
        = "q";

    template<typename T>
    struct buffer<T>
    {
        static bool is_compatible(Py_buffer const& view) noexcept
        {
            static_assert(buffer_format<T>);
            return is_buffer_compatible(view, sizeof(T), buffer_format<T>);
        }
    };

    /**
     * A wrapper around the Python buffer protocol that implements winrt::array_view.
     */
    template<typename T, bool writeable>
    struct pybuf_view : winrt::array_view<T>
    {
        using typename winrt::array_view<T>::value_type;
        using typename winrt::array_view<T>::pointer;
        using typename winrt::array_view<T>::size_type;

        pybuf_view(pybuf_view const&) = delete;
        pybuf_view& operator=(pybuf_view const&) = delete;

        pybuf_view() = delete;

        pybuf_view(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            // this is assuming pybuf_view is always treated as read-only
            if (PyObject_GetBuffer(
                    obj,
                    &view,
                    PyBUF_C_CONTIGUOUS | PyBUF_FORMAT
                        | (writeable ? PyBUF_WRITABLE : 0))
                == -1)
            {
                throw python_exception();
            }

            // TODO: if view.format == "P", we should try to verify the pointer type

            if (!buffer<T>::is_compatible(view))
            {
                PyBuffer_Release(&view);

                throw python_exception();
            }

            this->m_data = reinterpret_cast<pointer>(view.buf);
            this->m_size = static_cast<size_type>(view.shape[0]);
        }

        ~pybuf_view()
        {
            PyBuffer_Release(&view);
        }

      private:
        Py_buffer view;
    };

    template<typename T, bool writeable>
    struct converter<pybuf_view<T, writeable>>
    {
        static PyObject* convert(pybuf_view<T, writeable> const& /*unused*/) noexcept
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::converter<%s>::convert() is not implemented for py::pybuf_view",
                type_name<T>());
            return nullptr;
        }

        static pybuf_view<T, writeable> convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            return pybuf_view<T, writeable>{obj};
        }
    };
} // namespace py
