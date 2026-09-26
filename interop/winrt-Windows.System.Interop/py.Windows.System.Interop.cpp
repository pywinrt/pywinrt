#include "interop.h"

#include <DispatcherQueue.h>

// https://learn.microsoft.com/en-us/windows/win32/api/dispatcherqueue

namespace
{
    PyObject* create_dispatcher_queue_controller(
        PyObject* /*unused*/, PyObject* args) noexcept
    {
        int thread_type;
        int apartment_type;

        if (!PyArg_ParseTuple(args, "ii", &thread_type, &apartment_type))
        {
            return nullptr;
        }

        DispatcherQueueOptions options{
            sizeof(DispatcherQueueOptions),
            static_cast<DISPATCHERQUEUE_THREAD_TYPE>(thread_type),
            static_cast<DISPATCHERQUEUE_THREAD_APARTMENTTYPE>(apartment_type)};

        ABI::Windows::System::IDispatcherQueueController* controller{};

        auto const hr = CreateDispatcherQueueController(options, &controller);
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        return interop::new_interface_capsule(controller);
    }

    PyMethodDef module_methods[]{
        {"create_dispatcher_queue_controller",
         create_dispatcher_queue_controller,
         METH_VARARGS,
         nullptr},
        {}};

    PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_system_interop",
           nullptr,
           0,
           module_methods,
           nullptr,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_windows_system_interop(void) noexcept
{
    return PyModule_Create(&module_def);
}
