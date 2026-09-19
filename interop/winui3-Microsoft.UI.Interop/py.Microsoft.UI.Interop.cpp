#include <Windows.h>

#include <Python.h>
#include <pythoncapi_compat.h>
#include <pywinrt/base.h>

#include <winrt/Microsoft.UI.Interop.h>

// https://learn.microsoft.com/en-us/windows/windows-app-sdk/api/win32/winrt-microsoft.ui.interop/
namespace py::cpp::Microsoft::UI::Interop
{
    static PyObject* Interop_GetWindowIdFromWindow(
        PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto ptr = PyLong_AsVoidPtr(arg);
        if (!ptr && PyErr_Occurred())
        {
            return nullptr;
        }

        auto hwnd = static_cast<HWND>(ptr);

        try
        {
            auto type = py::get_python_type("winui3.microsoft.ui.WindowId");
            if (!type)
            {
                return nullptr;
            }

            auto windowId = winrt::Microsoft::UI::GetWindowIdFromWindow(hwnd);

            return py::struct_to_python(type, &windowId);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* Interop_GetWindowFromWindowId(
        PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto type = py::get_python_type("winui3.microsoft.ui.WindowId");
            if (!type)
            {
                return nullptr;
            }

            winrt::Microsoft::UI::WindowId windowId{};
            if (!py::struct_from_python(type, arg, &windowId))
            {
                return nullptr;
            }

            auto hwnd = winrt::Microsoft::UI::GetWindowFromWindowId(windowId);
            return PyLong_FromVoidPtr(static_cast<void*>(hwnd));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* Interop_GetDisplayIdFromMonitor(
        PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto ptr = PyLong_AsVoidPtr(arg);
        if (!ptr && PyErr_Occurred())
        {
            return nullptr;
        }

        auto hmonitor = static_cast<HMONITOR>(ptr);

        try
        {
            auto type = py::get_python_type("winui3.microsoft.ui.DisplayId");
            if (!type)
            {
                return nullptr;
            }

            auto displayId = winrt::Microsoft::UI::GetDisplayIdFromMonitor(hmonitor);

            return py::struct_to_python(type, &displayId);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* Interop_GetMonitorFromDisplayId(
        PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto type = py::get_python_type("winui3.microsoft.ui.DisplayId");
            if (!type)
            {
                return nullptr;
            }

            winrt::Microsoft::UI::DisplayId displayId{};
            if (!py::struct_from_python(type, arg, &displayId))
            {
                return nullptr;
            }

            auto hmonitor = winrt::Microsoft::UI::GetMonitorFromDisplayId(displayId);
            return PyLong_FromVoidPtr(static_cast<void*>(hmonitor));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* Interop_GetIconIdFromIcon(
        PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto ptr = PyLong_AsVoidPtr(arg);
        if (!ptr && PyErr_Occurred())
        {
            return nullptr;
        }

        auto hicon = static_cast<HICON>(ptr);

        try
        {
            auto type = py::get_python_type("winui3.microsoft.ui.IconId");
            if (!type)
            {
                return nullptr;
            }

            auto iconId = winrt::Microsoft::UI::GetIconIdFromIcon(hicon);

            return py::struct_to_python(type, &iconId);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* Interop_GetIconFromIconId(
        PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto type = py::get_python_type("winui3.microsoft.ui.IconId");
            if (!type)
            {
                return nullptr;
            }

            winrt::Microsoft::UI::IconId iconId{};
            if (!py::struct_from_python(type, arg, &iconId))
            {
                return nullptr;
            }

            auto hicon = winrt::Microsoft::UI::GetIconFromIconId(iconId);
            return PyLong_FromVoidPtr(static_cast<void*>(hicon));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef module_methods[]{
        {"get_window_id_from_window",
         &Interop_GetWindowIdFromWindow,
         METH_O,
         PyDoc_STR(
             "Gets the WindowId that corresponds to the specified hwnd, if the provided HWND is valid.")},
        {"get_window_from_window_id",
         &Interop_GetWindowFromWindowId,
         METH_O,
         PyDoc_STR(
             "Gets the window handle that corresponds to the specified windowId, if the provided windowId is valid and the system has an HWND that represents the window.")},
        {"get_display_id_from_monitor",
         &Interop_GetDisplayIdFromMonitor,
         METH_O,
         PyDoc_STR(
             "Gets the DisplayId that corresponds to the specified hmonitor, if the provided HMONITOR is valid.")},
        {"get_monitor_from_display_id",
         &Interop_GetMonitorFromDisplayId,
         METH_O,
         PyDoc_STR(
             "Gets the display monitor handle that corresponds to the specified displayId, if the provided displayId is valid and the system has an HMONITOR that represents the display monitor.")},
        {"get_icon_id_from_icon",
         &Interop_GetIconIdFromIcon,
         METH_O,
         PyDoc_STR(
             "Gets the IconId that corresponds to the specified hicon, if the provided HICON is valid.")},
        {"get_icon_from_icon_id",
         &Interop_GetIconFromIconId,
         METH_O,
         PyDoc_STR(
             "Gets the icon handle that corresponds to the specified iconId, if the provided iconId is valid and the system has an HICON that represents the icon.")},
        {}};

    PyDoc_STRVAR(module_doc, "UI Interop APIs.");

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winui3_microsoft_ui_interop",
           module_doc,
           0,
           module_methods,
           nullptr,
           nullptr,
           nullptr,
           nullptr};

} // namespace py::cpp::Microsoft::UI::Interop

PyMODINIT_FUNC PyInit__winui3_microsoft_ui_interop(void) noexcept
{
    using namespace py::cpp::Microsoft::UI::Interop;

    if (py::import_winrt_runtime() == -1)
    {
        return nullptr;
    }

    py::pyobj_handle module{PyModule_Create(&module_def)};

    return module.detach();
}
