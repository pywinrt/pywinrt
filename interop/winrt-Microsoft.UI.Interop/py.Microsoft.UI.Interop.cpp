#include "interop.h"

#include <Windows.h>

#include <winrt/Microsoft.UI.Interop.h>

// https://learn.microsoft.com/en-us/windows/windows-app-sdk/api/win32/winrt-microsoft.ui.interop/
//
// The App SDK ships these functions for C++/WinRT and for the ABI headers,
// and not the ABI Microsoft.UI.h that the latter need, so this module is
// written against C++/WinRT.

namespace
{
    /// Raises what winrt._winrt.hresult_error() makes of the C++/WinRT
    /// exception being handled, which carries the error info of the call that
    /// failed. Must only be called from a catch block.
    /// @returns nullptr, for the caller to return.
    PyObject* set_hresult_error() noexcept
    {
        try
        {
            throw;
        }
        catch (winrt::hresult_error const& e)
        {
            auto info = e.try_as<winrt::impl::IErrorInfo>();

            return interop::set_hresult_error(
                e.code(), reinterpret_cast<IErrorInfo*>(info.detach()));
        }
        catch (std::bad_alloc const&)
        {
            return PyErr_NoMemory();
        }
        catch (...)
        {
            return interop::set_hresult_error(E_FAIL, nullptr);
        }
    }

    PyObject* get_window_id_from_window(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto const ptr = PyLong_AsVoidPtr(arg);
        if (!ptr && PyErr_Occurred())
        {
            return nullptr;
        }

        try
        {
            auto const id
                = winrt::Microsoft::UI::GetWindowIdFromWindow(static_cast<HWND>(ptr));

            return PyLong_FromUnsignedLongLong(id.Value);
        }
        catch (...)
        {
            return set_hresult_error();
        }
    }

    PyObject* get_window_from_window_id(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto const value = PyLong_AsUnsignedLongLong(arg);
        if (value == static_cast<unsigned long long>(-1) && PyErr_Occurred())
        {
            return nullptr;
        }

        try
        {
            auto const handle = winrt::Microsoft::UI::GetWindowFromWindowId(
                winrt::Microsoft::UI::WindowId{value});

            return PyLong_FromVoidPtr(static_cast<void*>(handle));
        }
        catch (...)
        {
            return set_hresult_error();
        }
    }

    PyObject* get_display_id_from_monitor(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto const ptr = PyLong_AsVoidPtr(arg);
        if (!ptr && PyErr_Occurred())
        {
            return nullptr;
        }

        try
        {
            auto const id = winrt::Microsoft::UI::GetDisplayIdFromMonitor(
                static_cast<HMONITOR>(ptr));

            return PyLong_FromUnsignedLongLong(id.Value);
        }
        catch (...)
        {
            return set_hresult_error();
        }
    }

    PyObject* get_monitor_from_display_id(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto const value = PyLong_AsUnsignedLongLong(arg);
        if (value == static_cast<unsigned long long>(-1) && PyErr_Occurred())
        {
            return nullptr;
        }

        try
        {
            auto const handle = winrt::Microsoft::UI::GetMonitorFromDisplayId(
                winrt::Microsoft::UI::DisplayId{value});

            return PyLong_FromVoidPtr(static_cast<void*>(handle));
        }
        catch (...)
        {
            return set_hresult_error();
        }
    }

    PyObject* get_icon_id_from_icon(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto const ptr = PyLong_AsVoidPtr(arg);
        if (!ptr && PyErr_Occurred())
        {
            return nullptr;
        }

        try
        {
            auto const id
                = winrt::Microsoft::UI::GetIconIdFromIcon(static_cast<HICON>(ptr));

            return PyLong_FromUnsignedLongLong(id.Value);
        }
        catch (...)
        {
            return set_hresult_error();
        }
    }

    PyObject* get_icon_from_icon_id(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto const value = PyLong_AsUnsignedLongLong(arg);
        if (value == static_cast<unsigned long long>(-1) && PyErr_Occurred())
        {
            return nullptr;
        }

        try
        {
            auto const handle = winrt::Microsoft::UI::GetIconFromIconId(
                winrt::Microsoft::UI::IconId{value});

            return PyLong_FromVoidPtr(static_cast<void*>(handle));
        }
        catch (...)
        {
            return set_hresult_error();
        }
    }

    PyMethodDef module_methods[]{
        {"get_window_id_from_window", get_window_id_from_window, METH_O, nullptr},
        {"get_window_from_window_id", get_window_from_window_id, METH_O, nullptr},
        {"get_display_id_from_monitor", get_display_id_from_monitor, METH_O, nullptr},
        {"get_monitor_from_display_id", get_monitor_from_display_id, METH_O, nullptr},
        {"get_icon_id_from_icon", get_icon_id_from_icon, METH_O, nullptr},
        {"get_icon_from_icon_id", get_icon_from_icon_id, METH_O, nullptr},
        {}};

    PyDoc_STRVAR(module_doc, "UI Interop APIs.");

    PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_microsoft_ui_interop",
           module_doc,
           0,
           module_methods,
           interop::module_slots,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_microsoft_ui_interop(void) noexcept
{
    return PyModuleDef_Init(&module_def);
}
