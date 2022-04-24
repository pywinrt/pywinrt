#include "pybase.h"
#include <Shobjidl.h>
#include <winrt/base.h>

struct module_state
{
    PyTypeObject* Object_type;
};

/**
 * Adds a Python type to a Python module.
 *
 * @param module The module to add the type to.
 * @param type_name A valid Python identifier.
 * @param type_spec The Python type spec.
 * @param base_type The base type, a tuple of base types or nullptr to use the base
 * slot.
 * @returns A borrowed reference to the type object or nullptr on error.
 */
PyTypeObject* py::register_python_type(
    PyObject* module,
    const char* const type_name,
    PyType_Spec* type_spec,
    PyObject* base_type) noexcept
{
    py::pyobj_handle type_object{PyType_FromSpecWithBases(type_spec, base_type)};

    if (!type_object)
    {
        return nullptr;
    }

    // steals ref to type_object on success!
    if (PyModule_AddObject(module, type_name, type_object.get()) == -1)
    {
        return nullptr;
    }

    return reinterpret_cast<PyTypeObject*>(type_object.detach());
}

constexpr const char* const _type_name_Object = "Object";

PyDoc_STRVAR(Object_doc, "base class for wrapped WinRT object instances.");

static PyObject* Object_new(
    PyTypeObject* /* unused */, PyObject* /* unused */, PyObject* /* unused */) noexcept
{
    py::set_invalid_activation_error(_type_name_Object);
    return nullptr;
}

static void Object_dealloc(
    py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self)
{
    // auto hash_value =
    // std::hash<winrt::Windows::Foundation::IInspectable>{}(self->obj);
    // py::wrapped_instance(hash_value, nullptr);
    self->obj = nullptr;
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

std::unordered_map<std::size_t, PyObject*> instance_map{};

void py::wrapped_instance(std::size_t key, PyObject* obj)
{
    // TODO: re-enable instance wrapper caching

    // if obj is null, remove from instance_map
    // if (obj)
    //{
    //    auto insert = instance_map.try_emplace(key, obj);

    //    if (insert.second == false)
    //    {
    //        throw winrt::hresult_invalid_argument(L"wrapped WinRT object already
    //        cached");
    //    }
    //}
    // else
    //{
    //    // TODO: clean up the wrapped WinRT object. Currently leaking
    //    instance_map.extract(key);
    //}
}

PyObject* py::wrapped_instance(std::size_t key)
{
    // auto const it = instance_map.find(key);
    // if (it == instance_map.end())
    {
        return nullptr;
    }

    // return it->second;
}

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

static PyObject* uninit_apartment(PyObject* /*unused*/, PyObject* /*unused*/) noexcept
{
    winrt::uninit_apartment();
    Py_RETURN_NONE;
}

static PyObject* initialize_with_window(PyObject* /*unused*/, PyObject* args) noexcept
{
    PyObject* obj;
    Py_ssize_t hwnd;

    if (!PyArg_ParseTuple(args, "On", &obj, &hwnd))
    {
        return nullptr;
    }

    try
    {
        auto winrt_obj = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
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
    {"uninit_apartment", uninit_apartment, METH_NOARGS, "uninitialize the apartment"},
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

PyMODINIT_FUNC PyInit__winrt(void) noexcept
{
    static const auto kMTA = static_cast<long>(winrt::apartment_type::multi_threaded);
    static const auto kSTA = static_cast<long>(winrt::apartment_type::single_threaded);

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

PyTypeObject* py::winrt_type<py::Object>::get_python_type() noexcept
{
    PyObject* module = PyState_FindModule(&module_def);

    if (!module)
    {
        PyErr_SetString(PyExc_RuntimeError, "could not find _winrt module");
        return nullptr;
    }

    auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));
    assert(state);

    return state->Object_type;
}
