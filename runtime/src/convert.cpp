// The conversions declared in convert.h that reach into the runtime's object
// model, defined here so that convert.h need not include the headers they use.

#include "_winrt.h"
#include "convert.h"
#include "errors.h"
#include "handles.h"
#include "implements.h"
#include "objects.h"
#include "prelude.h"
#include "wrappers.h"

#include <cstdint>

PyObject* py::converter<winrt::Windows::Foundation::IInspectable>::convert(
    winrt::Windows::Foundation::IInspectable const& value) noexcept
{
    if (!value)
    {
        Py_RETURN_NONE;
    }

    if (auto* const started = interp::python_object_of(winrt::get_abi(value)))
    {
        return started;
    }

    auto object_type = get_object_type();

    if (!object_type)
    {
        return nullptr;
    }

    return wrap<winrt::Windows::Foundation::IInspectable>(value, object_type);
}

winrt::Windows::Foundation::IInspectable py::converter<
    winrt::Windows::Foundation::IInspectable>::convert_to(PyObject* obj)
{
    throw_if_pyobj_null(obj);

    if (Py_IsNone(obj))
    {
        return nullptr;
    }

    auto object_type = get_object_type();

    if (!object_type)
    {
        throw python_exception();
    }

    // The layout, not isinstance(), which a Python implementation of an
    // interface passes without holding a WinRT object.
    if (!PyObject_TypeCheck(obj, object_type))
    {
        // Not a wrapper, but a Python class that derives from the public name
        // of a projected interface is an implementation of it, and the runtime
        // knows how to stand one up. Anything else fails there with a message
        // of its own.
        void* abi{};

        if (!unwrap_object(
                obj, winrt::guid_of<winrt::Windows::Foundation::IInspectable>(), &abi))
        {
            throw python_exception();
        }

        winrt::Windows::Foundation::IInspectable value;
        winrt::attach_abi(value, abi);

        return value;
    }

    auto const& held
        = reinterpret_cast<winrt_wrapper<winrt::Windows::Foundation::IUnknown>*>(obj)
              ->obj;

    // A delegate is no IInspectable, so the wrapper is asked, and it may be a
    // proxy, which is asked through its apartment.
    winrt::Windows::Foundation::IInspectable value;
    int32_t hr{};

    {
        auto _gil = release_gil();
        hr = held.as(
            winrt::guid_of<winrt::Windows::Foundation::IInspectable>(),
            winrt::put_abi(value));
    }

    winrt::check_hresult(hr);

    return value;
}
