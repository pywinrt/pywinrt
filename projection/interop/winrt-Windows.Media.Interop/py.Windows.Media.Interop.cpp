#include <systemmediatransportcontrolsinterop.h>

#include "pybase.h"
#include "py.Windows.Media.h"
#include <winrt/Windows.Media.h>

// https://learn.microsoft.com/en-us/windows/win32/api/systemmediatransportcontrolsinterop/

namespace py::cpp::Windows::Media::Interop
{
    static PyObject* get_for_window(PyObject* /*unused*/, PyObject* hwnd_obj) noexcept
    {
        try
        {
            auto hwnd = PyLong_AsVoidPtr(hwnd_obj);
            if (!hwnd && PyErr_Occurred())
            {
                return nullptr;
            }

            auto activation_factory = winrt::get_activation_factory<
                winrt::Windows::Media::SystemMediaTransportControls>();
            auto interop_factory
                = activation_factory.as<ISystemMediaTransportControlsInterop>();
            winrt::Windows::Media::SystemMediaTransportControls item = {nullptr};
            winrt::check_hresult(interop_factory->GetForWindow(
                reinterpret_cast<HWND>(hwnd),
                winrt::guid_of<winrt::Windows::Media::SystemMediaTransportControls>(),
                winrt::put_abi(item)));

            return py::convert(item);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef module_methods[]{
        {"get_for_window", get_for_window, METH_O, nullptr}, {}};

    PyDoc_STRVAR(
        module_doc, "APIs for desktop interop with the Windows.Media namespace.");

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_media_interop",
           module_doc,
           0,
           module_methods,
           nullptr,
           nullptr,
           nullptr,
           nullptr};
} // namespace py::cpp::Windows::Media::Interop

PyMODINIT_FUNC PyInit__winrt_windows_media_interop(void) noexcept
{
    using namespace py::cpp::Windows::Media::Interop;

    if (py::import_winrt_runtime() == -1)
    {
        return nullptr;
    }

    return PyModule_Create(&module_def);
}
