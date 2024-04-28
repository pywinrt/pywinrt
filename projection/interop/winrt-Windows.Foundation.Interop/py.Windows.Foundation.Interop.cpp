#include <Python.h>

namespace py::cpp::Windows::Foundation::Interop
{
    PyDoc_STRVAR(module_doc, "APIs for interop with the Windows.Foundation namespace.");

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_foundation_interop",
           module_doc,
           0,
           nullptr,
           nullptr,
           nullptr,
           nullptr,
           nullptr};
} // namespace py::cpp::Windows::Foundation::Interop

PyMODINIT_FUNC PyInit__winrt_windows_foundation_interop(void) noexcept
{
    using namespace py::cpp::Windows::Foundation::Interop;
    return PyModule_Create(&module_def);
}
