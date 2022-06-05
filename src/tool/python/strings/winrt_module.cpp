#include "pybase.h"
#include <Shobjidl.h>
#include <winrt/base.h>

namespace py::cpp::_winrt
{
    struct module_state
    {
        PyTypeObject* Object_type;
    };

    constexpr const char* const _type_name_Object = "Object";

    PyDoc_STRVAR(Object_doc, "base class for wrapped WinRT object instances.");

    static PyObject* Object_new(
        PyTypeObject* /* unused */,
        PyObject* /* unused */,
        PyObject* /* unused */) noexcept
    {
        py::set_invalid_activation_error(_type_name_Object);
        return nullptr;
    }

    static void Object_dealloc(
        py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self)
    {
        auto tp = Py_TYPE(self);
        self->obj = nullptr;
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyType_Slot Object_type_slots[] = {
        {Py_tp_new, Object_new},
        {Py_tp_dealloc, Object_dealloc},
        {Py_tp_doc, const_cast<char*>(Object_doc)},
        {},
    };

    static PyType_Spec Object_type_spec
        = {"_winrt.Object",
           sizeof(py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>),
           0,
           Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
           Object_type_slots};

    static PyObject* init_apartment(PyObject* /*unused*/, PyObject* type_obj) noexcept
    {
        auto type = PyLong_AsLong(type_obj);

        if (type == -1 && PyErr_Occurred())
        {
            return nullptr;
        }

        try
        {
            winrt::init_apartment(static_cast<winrt::apartment_type>(type));
            Py_RETURN_NONE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* uninit_apartment(
        PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        winrt::uninit_apartment();
        Py_RETURN_NONE;
    }

    static PyObject* initialize_with_window(
        PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* obj;
        Py_ssize_t hwnd;

        if (!PyArg_ParseTuple(args, "On", &obj, &hwnd))
        {
            return nullptr;
        }

        try
        {
            auto winrt_obj
                = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            auto result = winrt_obj.as<IInitializeWithWindow>()->Initialize(
                reinterpret_cast<HWND>(hwnd));

            if (result != S_OK)
            {
                winrt::throw_hresult(result);
            }
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }

        Py_RETURN_NONE;
    }

    PyDoc_STRVAR(module_doc, "_winrt");

    static PyMethodDef module_methods[]{
        {"init_apartment", init_apartment, METH_O, "initialize the apartment"},
        {"uninit_apartment",
         uninit_apartment,
         METH_NOARGS,
         "uninitialize the apartment"},
        {"initialize_with_window",
         initialize_with_window,
         METH_VARARGS,
         "interop function to invoke IInitializeWithWindow::Initialize on an object"},
        {}};

    static int module_traverse(PyObject* module, visitproc visit, void* arg) noexcept
    {
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));
        assert(state);

        Py_VISIT(state->Object_type);

        return 0;
    }

    static int module_clear(PyObject* module) noexcept
    {
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));
        assert(state);

        Py_CLEAR(state->Object_type);

        return 0;
    }

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt",
           module_doc,
           sizeof(module_state),
           module_methods,
           nullptr,
           module_traverse,
           module_clear,
           nullptr};

    static PyObject* module_init() noexcept
    {
        static const auto kMTA
            = static_cast<long>(winrt::apartment_type::multi_threaded);
        static const auto kSTA
            = static_cast<long>(winrt::apartment_type::single_threaded);

        py::pyobj_handle module{PyModule_Create(&module_def)};

        if (!module)
        {
            return nullptr;
        }

        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module.get()));
        assert(state);

        state->Object_type = py::register_python_type(
            module.get(), _type_name_Object, &Object_type_spec, nullptr);

        if (!state->Object_type)
        {
            return nullptr;
        }

        Py_INCREF(state->Object_type);

        if (PyModule_AddIntConstant(module.get(), "MTA", kMTA) == -1)
        {
            return nullptr;
        }

        if (PyModule_AddIntConstant(module.get(), "STA", kSTA) == -1)
        {
            return nullptr;
        }

        return module.detach();
    }
} // namespace py::cpp::_winrt

PyMODINIT_FUNC PyInit__winrt(void) noexcept
{
    return py::cpp::_winrt::module_init();
}

PyTypeObject* py::winrt_type<py::Object>::get_python_type() noexcept
{
    // borrowed ref
    PyObject* module = PyState_FindModule(&py::cpp::_winrt::module_def);

    if (!module)
    {
        PyErr_SetString(PyExc_RuntimeError, "could not find _winrt module");
        return nullptr;
    }

    auto state
        = reinterpret_cast<py::cpp::_winrt::module_state*>(PyModule_GetState(module));
    assert(state);

    return state->Object_type;
}
