// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "pybase.h"
#include "py.Windows.ApplicationModel.DataTransfer.DragDrop.h"


namespace py::cpp::Windows::ApplicationModel::DataTransfer::DragDrop
{
    // ----- Windows.ApplicationModel.DataTransfer.DragDrop Initialization --------------------
    PyDoc_STRVAR(module_doc, "Windows::ApplicationModel::DataTransfer::DragDrop");


    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_applicationmodel_datatransfer_dragdrop",
           module_doc,
           0,
           nullptr,
           nullptr,
           nullptr,
           nullptr,
           nullptr};

} // py::cpp::Windows::ApplicationModel::DataTransfer::DragDrop

PyMODINIT_FUNC PyInit__winrt_windows_applicationmodel_datatransfer_dragdrop(void) noexcept
{
    using namespace py::cpp::Windows::ApplicationModel::DataTransfer::DragDrop;

    py::pyobj_handle module{PyModule_Create(&module_def)};

    if (!module)
    {
        return nullptr;
    }

    auto object_type = py::get_object_type();
    if (!object_type)
    {
        return nullptr;
    }

    py::pyobj_handle object_bases{PyTuple_Pack(1, object_type)};

    if (!object_bases)
    {
        return nullptr;
    }


    return module.detach();
}