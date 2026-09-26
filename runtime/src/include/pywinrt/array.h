// WinRT arrays. py::ComArray<T> is the concrete py::Array behind the runtime's
// winrt.system.Array object, and the converters here are what pass one to and
// from a projected method.
//
// A winrt::array_view cannot be handed to Python as it is - nothing says how
// long the data it points at stays alive - so converting one out always copies.

#pragma once

#include <pywinrt/abi.h>
#include <pywinrt/convert.h>
#include <pywinrt/errors.h>
#include <pywinrt/prelude.h>
#include <pywinrt/traits.h>
#include <pywinrt/wrappers.h>

namespace py
{
    template<typename T>
    struct ComArray;

    template<typename T>
    struct converter<winrt::array_view<T>>
    {
        static PyObject* convert(winrt::array_view<T> const& instance) noexcept
        {
            // there is no way to know the scope of the lifetime of the
            // underlying data of the array_view so we need to make a copy
            // before sending to Python to ensure we don't introduce use after
            // free memory bugs

            using copy_t = std::remove_const_t<T>;
            auto copy = std::make_unique<ComArray<copy_t>>();
            copy->array = winrt::com_array<copy_t>(instance.begin(), instance.end());

            return cpp::_winrt::Array_New(std::move(copy));
        }

        static winrt::array_view<T> convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            PyErr_Format(
                PyExc_NotImplementedError,
                "py::converter<%s>::convert_to() is not implemented",
                type_name<T>());
            throw python_exception();
        }
    };

    template<typename T>
    struct converter<winrt::com_array<T>>
    {
        static PyObject* convert(winrt::com_array<T> const& instance) noexcept
        {
            auto array = std::make_unique<ComArray<T>>();
            // HACK: casting to rvalue reference to steal data
            // std::move doesn't work because of const&
            array->array = const_cast<winrt::com_array<T>&&>(instance);
            return py::cpp::_winrt::Array_New(std::move(array));
        }

        static auto convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            Py_ssize_t list_size = PySequence_Size(obj);

            if (list_size == -1)
            {
                throw python_exception();
            }

            winrt::com_array<T> items(
                static_cast<uint32_t>(list_size), empty_instance<T>::get());

            for (Py_ssize_t index = 0; index < list_size; index++)
            {
                pyobj_handle item{PySequence_GetItem(obj, index)};

                if (!item)
                {
                    throw python_exception();
                }

                items[static_cast<uint32_t>(index)]
                    = converter<T>::convert_to(item.get());
            }

            return items;
        }
    };

    template<typename T>
    struct ComArray : Array
    {
        winrt::com_array<T> array;

        bool Alloc(uint32_t size) noexcept override
        {
            try
            {
                array = winrt::com_array<T>(size, empty_instance<T>::get());
                return true;
            }
            catch (...)
            {
                py::to_PyErr();
                return false;
            }
        }

        virtual std::wstring_view WinrtElementTypeName() noexcept override
        {
            return winrt::name_of<T>();
        }

        std::string_view Format() noexcept override
        {
            static_assert(buffer_format<T>);
            return buffer_format<T>;
        }

        uint32_t Size() noexcept override
        {
            return array.size();
        }

        size_t ValueSize() noexcept override
        {
            return sizeof(typename winrt::com_array<T>::value_type);
        }

        void* Data() noexcept override
        {
            return array.data();
        }

        PyObject* At(uint32_t index) noexcept override
        {
            try
            {
                return convert<T>(array.at(index));
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }
        }

        bool Set(Py_ssize_t index, PyObject* item) noexcept override
        {
            if (index < 0)
            {
                index = array.size() + index;
            }

            if (index < 0 || static_cast<uint32_t>(index) >= array.size())
            {
                PyErr_SetString(PyExc_IndexError, "index out of range");
                return false;
            }

            try
            {
                array[static_cast<winrt::array_view<T>::size_type>(index)]
                    = convert_to<T>(item);
                return true;
            }
            catch (...)
            {
                py::to_PyErr();
                return false;
            }
        }
    };
} // namespace py
