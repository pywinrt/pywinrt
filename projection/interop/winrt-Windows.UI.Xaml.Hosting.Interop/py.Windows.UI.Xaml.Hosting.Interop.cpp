#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>
#include "pybase.h"
#include <winrt/base.h>

// https://learn.microsoft.com/en-us/windows/win32/api/windows.ui.xaml.hosting.desktopwindowxamlsource/

namespace py::cpp::Windows::UI::Xaml::Hosting::Interop
{
    struct DesktopWindowXamlSourceNativeObject
        : py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>
    {
        winrt::com_ptr<IDesktopWindowXamlSourceNative> native{};
        winrt::com_ptr<IDesktopWindowXamlSourceNative2> native2{};
    };

    static void DesktopWindowXamlSourceNative_dealloc(
        DesktopWindowXamlSourceNativeObject* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        std::destroy_at(&self->native);
        std::destroy_at(&self->native2);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

#if PY_VERSION_HEX < 0x030A0000
    static PyObject* DesktopWindowXamlSourceNative_new(
        PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        py::set_invalid_activation_error("DesktopWindowXamlSourceNative");
        return nullptr;
    }
#endif

    static DesktopWindowXamlSourceNativeObject* DesktopWindowXamlSourceNative_from(
        PyTypeObject* cls, PyObject* arg) noexcept
    {
        try
        {
            auto inspectable
                = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            auto native = inspectable.as<IDesktopWindowXamlSourceNative>();
            auto native2 = inspectable.as<IDesktopWindowXamlSourceNative2>();

            auto instance = reinterpret_cast<DesktopWindowXamlSourceNativeObject*>(
                cls->tp_alloc(cls, 0));
            if (!instance)
            {
                return nullptr;
            }

            std::construct_at(&instance->obj, std::move(inspectable));
            std::construct_at(&instance->native, std::move(native));
            std::construct_at(&instance->native2, std::move(native2));

            return instance;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* DesktopWindowXamlSourceNative_attach_to_window(
        DesktopWindowXamlSourceNativeObject* self, PyObject* arg) noexcept
    {
        try
        {
            auto hwnd = static_cast<HWND>(PyLong_AsVoidPtr(arg));
            winrt::check_hresult(self->native->AttachToWindow(hwnd));
            Py_RETURN_NONE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* DesktopWindowXamlSourceNative_pretranslate_message(
        DesktopWindowXamlSourceNativeObject* self, PyObject* arg) noexcept
    {
        try
        {
            Py_buffer buffer;

            if (PyObject_GetBuffer(arg, &buffer, PyBUF_C_CONTIGUOUS | PyBUF_FORMAT)
                == -1)
            {
                throw py::python_exception();
            }

            using py_buffer_ptr
                = std::unique_ptr<Py_buffer, decltype(&PyBuffer_Release)>;
            py_buffer_ptr{&buffer, &PyBuffer_Release};

            if (!py::is_buffer_compatible(
                    buffer,
                    sizeof(MSG),
                    "T{P:hwnd:I:message:P:wParam:P:lParam:L:time:T{l:x:l:y:}:point:L:lPrivate:}"))
            {
                throw py::python_exception();
            }

            BOOL handled;
            winrt::check_hresult(self->native2->PreTranslateMessage(
                reinterpret_cast<MSG*>(buffer.buf), &handled));
            return PyBool_FromLong(handled);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef DesktopWindowXamlSourceNative_methods[]{
        {"_from",
         reinterpret_cast<PyCFunction>(DesktopWindowXamlSourceNative_from),
         METH_CLASS | METH_O,
         nullptr},
        {"attach_to_window",
         reinterpret_cast<PyCFunction>(DesktopWindowXamlSourceNative_attach_to_window),
         METH_O,
         nullptr},
        {"pretranslate_message",
         reinterpret_cast<PyCFunction>(
             DesktopWindowXamlSourceNative_pretranslate_message),
         METH_O,
         nullptr},
        {}};

    static PyObject* DesktopWindowXamlSourceNative_get_window_handle(
        DesktopWindowXamlSourceNativeObject* self, void* /*unused*/) noexcept
    {
        try
        {
            HWND hwnd;
            winrt::check_hresult(self->native->get_WindowHandle(&hwnd));
            return PyLong_FromVoidPtr(hwnd);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyGetSetDef DesktopWindowXamlSourceNative_getset[]{
        {"window_handle",
         reinterpret_cast<getter>(DesktopWindowXamlSourceNative_get_window_handle),
         nullptr,
         nullptr,
         nullptr},
        {}};

    static PyType_Slot DesktopWindowXamlSourceNative_slots[]{
        {Py_tp_dealloc, reinterpret_cast<void*>(DesktopWindowXamlSourceNative_dealloc)},
#if PY_VERSION_HEX < 0x030A0000
        {Py_tp_new, reinterpret_cast<void*>(DesktopWindowXamlSourceNative_new)},
#endif
        {Py_tp_methods, DesktopWindowXamlSourceNative_methods},
        {Py_tp_getset, DesktopWindowXamlSourceNative_getset},
        {}};

    static PyType_Spec DesktopWindowXamlSourceNative_type_spec{
        "winrt._winrt_windows_ui_xaml_hosting_interop.DesktopWindowXamlSourceNative",
        sizeof(DesktopWindowXamlSourceNativeObject),
        0,
        Py_TPFLAGS_DEFAULT
#if PY_VERSION_HEX >= 0x030A0000
            | Py_TPFLAGS_DISALLOW_INSTANTIATION
#endif
        ,
        DesktopWindowXamlSourceNative_slots};

    PyDoc_STRVAR(
        module_doc,
        "APIs for desktop interop with the Windows.UI.Xaml.Hosting namespace.");

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_ui_xaml_hosting_interop",
           module_doc,
           0,
           nullptr,
           nullptr,
           nullptr,
           nullptr,
           nullptr};
} // namespace py::cpp::Windows::UI::Xaml::Hosting::Interop

PyMODINIT_FUNC PyInit__winrt_windows_ui_xaml_hosting_interop(void) noexcept
{
    using namespace py::cpp::Windows::UI::Xaml::Hosting::Interop;

    if (py::import_winrt_runtime() == -1)
    {
        return nullptr;
    }

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

    py::pyobj_handle bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(object_type))};
    if (!bases)
    {
        return nullptr;
    }

    py::pytype_handle x_type{py::register_python_type(
        module.get(), &DesktopWindowXamlSourceNative_type_spec, bases.get(), nullptr)};
    if (!x_type)
    {
        return nullptr;
    }

    return module.detach();
}
