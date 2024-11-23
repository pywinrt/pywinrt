#include "pybase.h"
#include <py.Windows.System.h>

#include <winrt/base.h>
#include <winrt/Windows.System.h>
#include <DispatcherQueue.h>

#include <Python.h>

// https://learn.microsoft.com/en-us/windows/win32/api/dispatcherqueue

namespace py::cpp::Windows::System::Interop
{
    static PyObject* create_dispatcher_queue_controller(
        PyObject* /*unused*/, PyObject* args, PyObject* kw) noexcept
    {
        namespace abi = ABI::Windows::System;

        try
        {
            enum DISPATCHERQUEUE_THREAD_TYPE thread_type = DQTYPE_THREAD_CURRENT;
            enum DISPATCHERQUEUE_THREAD_APARTMENTTYPE apartment_type = DQTAT_COM_NONE;

            static const char* const keywords[]
                = {"thread_type", "apartment_type", nullptr};

            if (!PyArg_ParseTupleAndKeywords(
                    args,
                    kw,
                    "|ii",
                    const_cast<char**>(keywords),
                    &thread_type,
                    &apartment_type))
            {
                return nullptr;
            }

            DispatcherQueueOptions options{
                sizeof(DispatcherQueueOptions), thread_type, apartment_type};

            winrt::Windows::System::DispatcherQueueController controller{nullptr};
            winrt::check_hresult(CreateDispatcherQueueController(
                options,
                reinterpret_cast<abi::IDispatcherQueueController**>(
                    winrt::put_abi(controller))));

            return convert(controller);
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }
    }

    PyMethodDef module_methods[]
        = {{"create_dispatcher_queue_controller",
            reinterpret_cast<PyCFunction>(
                reinterpret_cast<void*>(create_dispatcher_queue_controller)),
            METH_VARARGS | METH_KEYWORDS,
            nullptr},
           {}};

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_system_interop",
           nullptr,
           0,
           module_methods,
           nullptr,
           nullptr,
           nullptr,
           nullptr};
} // namespace py::cpp::Windows::System::Interop

PyMODINIT_FUNC PyInit__winrt_windows_system_interop(void) noexcept
{
    using namespace py::cpp::Windows::System::Interop;

    if (py::import_winrt_runtime() == -1)
    {
        return nullptr;
    }

    return PyModule_Create(&module_def);
}
