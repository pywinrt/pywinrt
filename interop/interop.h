// What an interop module needs to exchange WinRT objects and errors with
// winrt-runtime.
//
// An interop module shares no C ABI with the runtime and includes none of its
// headers. A WinRT object crosses as an interface pointer capsule, which
// winrt._winrt.as_interface() makes from a projected object and
// winrt._winrt.wrap_interface() makes a projected object from, and a failed
// call raises the exception that winrt._winrt.hresult_error() builds. The
// capsule's name and those three functions are the whole contract, so this
// header can change freely.
//
// scripts/generate-pyproject.py copies this file into each interop package,
// so that its source distribution builds on its own.

#pragma once

#include <Python.h>

#include <windows.h>

#include <oleauto.h>
#include <roapi.h>
#include <winstring.h>

#include <cstddef>

namespace interop
{
    /// The name every interface pointer capsule carries, in the runtime and in
    /// every interop module alike. One holds a reference to a COM interface,
    /// which its destructor releases.
    inline constexpr char interface_capsule_name[] = "winrt.interface";

    /// The destructor of an interface pointer capsule this module made.
    inline void release_interface(PyObject* capsule) noexcept
    {
        auto const abi = PyCapsule_GetPointer(capsule, interface_capsule_name);
        if (!abi)
        {
            PyErr_WriteUnraisable(capsule);
            return;
        }

        static_cast<IUnknown*>(abi)->Release();
    }

    /// Hands @p abi, and the reference that comes with it, to a new interface
    /// pointer capsule. The reference is released if that fails.
    inline PyObject* new_interface_capsule(IUnknown* abi) noexcept
    {
        auto const capsule
            = PyCapsule_New(abi, interface_capsule_name, release_interface);
        if (!capsule)
        {
            abi->Release();
        }

        return capsule;
    }

    /// Raises what winrt._winrt.hresult_error() makes of @p hr and @p info,
    /// the error info of the call that failed with it, which may be
    /// @c nullptr. The reference to @p info moves in.
    /// @returns nullptr, for the caller to return.
    inline PyObject* set_hresult_error(HRESULT hr, IErrorInfo* info) noexcept
    {
        auto const capsule = info ? new_interface_capsule(info) : Py_NewRef(Py_None);
        if (!capsule)
        {
            return nullptr;
        }

        auto const runtime = PyImport_ImportModule("winrt._winrt");
        if (!runtime)
        {
            Py_DECREF(capsule);
            return nullptr;
        }

        auto const error = PyObject_CallMethod(
            runtime, "hresult_error", "lO", static_cast<long>(hr), capsule);
        Py_DECREF(runtime);
        Py_DECREF(capsule);
        if (!error)
        {
            return nullptr;
        }

        PyErr_SetObject(reinterpret_cast<PyObject*>(Py_TYPE(error)), error);
        Py_DECREF(error);

        return nullptr;
    }

    /// Raises what winrt._winrt.hresult_error() makes of @p hr and of the error
    /// info that the call that just failed with it left on this thread.
    /// @returns nullptr, for the caller to return.
    inline PyObject* set_hresult_error(HRESULT hr) noexcept
    {
        // The error info belongs to the thread and the next failure replaces
        // it, so it is taken before anything else can fail.
        IErrorInfo* info{};
        if (GetErrorInfo(0, &info) != S_OK)
        {
            info = nullptr;
        }

        return set_hresult_error(hr, info);
    }

    /// The @p I interface of what the interface pointer capsule @p capsule
    /// holds, which the caller owns a reference to.
    /// @returns false with a Python exception set when @p capsule is not one
    /// or what it holds does not implement @p I.
    template<typename I>
    bool query_capsule(PyObject* capsule, I** result) noexcept
    {
        auto const abi = PyCapsule_GetPointer(capsule, interface_capsule_name);
        if (!abi)
        {
            return false;
        }

        auto const hr = static_cast<IUnknown*>(abi)->QueryInterface(
            __uuidof(I), reinterpret_cast<void**>(result));
        if (FAILED(hr))
        {
            set_hresult_error(hr);
            return false;
        }

        return true;
    }

    /// The activation factory of the runtime class @p class_name, as its @p I
    /// interface.
    template<typename I, std::size_t N>
    HRESULT get_activation_factory(wchar_t const (&class_name)[N], I** factory) noexcept
    {
        HSTRING_HEADER header;
        HSTRING name;

        auto const hr = WindowsCreateStringReference(class_name, N - 1, &header, &name);
        if (FAILED(hr))
        {
            return hr;
        }

        return RoGetActivationFactory(
            name, __uuidof(I), reinterpret_cast<void**>(factory));
    }

    /// The slots of an interop module that has nothing to set up when it is
    /// executed. It keeps no state, but it exchanges objects and errors with
    /// winrt-runtime, which refuses any interpreter but the main one, so it
    /// says the same.
    inline PyModuleDef_Slot module_slots[]{
#ifdef Py_mod_multiple_interpreters
        {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
#endif
        {}};
} // namespace interop
