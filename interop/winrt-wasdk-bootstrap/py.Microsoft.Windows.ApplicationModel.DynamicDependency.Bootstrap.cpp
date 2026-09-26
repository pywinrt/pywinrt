#include "interop.h"

#include <windows.h>

#include <WindowsAppSDK-VersionInfo.h>
#include <MddBootstrap.h>

#include <memory>
#include <string_view>

// https://learn.microsoft.com/en-us/windows/apps/api-reference/bootstrapper-cpp-api/microsoft.windows.applicationmodel.dynamicdependency.bootstrap/microsoft.windows.applicationmodel.dynamicdependency.bootstrap#functions-in-themicrosoftwindowsapplicationmodeldynamicdependencybootstrap-namespace
namespace
{
    using namespace ::Microsoft::Windows::ApplicationModel::DynamicDependency::
        Bootstrap;

    struct ShutdownObject
    {
        PyObject_HEAD;
        unique_mddbootstrapshutdown shutdown;
    };

    static PyObject* initialize(PyObject* module, PyObject* args) noexcept
    {
        uint32_t major_minor_version;
        PyObject* version_tag_obj;
        uint64_t min_version;
        InitializeOptions options;

        if (!PyArg_ParseTuple(
                args,
                "IOKI:initialize",
                &major_minor_version,
                &version_tag_obj,
                &min_version,
                &options))
        {
            return nullptr;
        }

        wchar_t* version_tag{};

        if (!Py_IsNone(version_tag_obj))
        {
            version_tag = PyUnicode_AsWideCharString(version_tag_obj, nullptr);
            if (!version_tag)
            {
                return nullptr;
            }
        }

        std::unique_ptr<wchar_t, decltype(&PyMem_Free)> version_tag_owner{
            version_tag, &PyMem_Free};

        auto const shutdown_type = PyObject_GetAttrString(module, "Shutdown");
        if (!shutdown_type)
        {
            return nullptr;
        }

        auto const shutdown = PyObject_CallNoArgs(shutdown_type);
        Py_DECREF(shutdown_type);
        if (!shutdown)
        {
            return nullptr;
        }

        // FIXME: validate type of shutdown before casting to ShutdownObject

        auto const hr
            = InitializeNoThrow(major_minor_version, version_tag, min_version, options);
        if (FAILED(hr))
        {
            Py_DECREF(shutdown);
            return interop::set_hresult_error(hr);
        }

        reinterpret_cast<ShutdownObject*>(shutdown)->shutdown.reset(
            reinterpret_cast<details::mddbootstrapshutdown_t*>(1));

        return shutdown;
    }

    static PyMethodDef module_methods[]{
        {"initialize", initialize, METH_VARARGS, nullptr}, {}};

    PyDoc_STRVAR(module_doc, "APIs for boostrapping Windows App SDK framework.");

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_microsoft_windows_applicationmodel_dynamicdependency_bootstrap",
           module_doc,
           0,
           module_methods,
           nullptr,
           nullptr,
           nullptr,
           nullptr};

    static PyObject* shutdown_new(
        PyTypeObject* subtype, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        auto self = reinterpret_cast<ShutdownObject*>(subtype->tp_alloc(subtype, 0));

        if (!self)
        {
            return nullptr;
        }

        // call C++ constructors on memory allocated from CPython heap
        std::construct_at(&self->shutdown);

        return reinterpret_cast<PyObject*>(self);
    }

    static void shutdown_dealloc(ShutdownObject* self) noexcept
    {
        PyTypeObject* tp = Py_TYPE(self);

        if (self->shutdown)
        {
#if PY_VERSION_HEX < 0x030C0000
            PyObject *error_type, *error_value, *error_traceback;

            PyErr_Fetch(&error_type, &error_value, &error_traceback);
#else
            auto const error = PyErr_GetRaisedException();
#endif

            if (PyErr_WarnEx(
                    PyExc_RuntimeWarning,
                    "Shutdown object was not called before being garbage collected.",
                    1)
                == -1)
            {
                PyErr_WriteUnraisable(reinterpret_cast<PyObject*>(self));
            }

#if PY_VERSION_HEX < 0x030C0000
            PyErr_Restore(error_type, error_value, error_traceback);
#else
            if (error)
            {
                PyErr_SetRaisedException(error);
            }
#endif
        }

        std::destroy_at(&self->shutdown);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* shutdown_call(PyObject* self, PyObject* /*unused*/) noexcept
    {
        reinterpret_cast<ShutdownObject*>(self)->shutdown.reset();

        Py_RETURN_NONE;
    }

    static PyObject* shutdown_identity(PyObject* self, PyObject* /*unused*/) noexcept
    {
        Py_INCREF(self);
        return self;
    }

    static PyMethodDef shutdown_methods[]{
        {"__enter__",
         reinterpret_cast<PyCFunction>(shutdown_identity),
         METH_NOARGS,
         nullptr},
        {"__exit__",
         reinterpret_cast<PyCFunction>(shutdown_call),
         METH_VARARGS,
         nullptr},
        {}};

    static PyType_Slot shutdown_slots[]{
        {Py_tp_new, reinterpret_cast<void*>(shutdown_new)},
        {Py_tp_dealloc, reinterpret_cast<void*>(shutdown_dealloc)},
        {Py_tp_call, reinterpret_cast<void*>(shutdown_call)},
        {Py_tp_methods, reinterpret_cast<void*>(shutdown_methods)},
        {}};

    static PyType_Spec shutdown_type_spec{
        "winrt._winrt_microsoft_windows_applicationmodel_dynamicdependency_bootstrap.Shutdown",
        sizeof(ShutdownObject),
        0,
        Py_TPFLAGS_DEFAULT,
        shutdown_slots};
} // namespace

PyMODINIT_FUNC
PyInit__winrt_microsoft_windows_applicationmodel_dynamicdependency_bootstrap(
    void) noexcept
{
    std::unique_ptr<PyObject, decltype(&Py_DecRef)> module{
        PyModule_Create(&module_def), &Py_DecRef};
    if (!module)
    {
        return nullptr;
    }

    if (PyModule_AddStringConstant(
            module.get(),
            "RELEASE_VERSION",
            std::string_view(WINDOWSAPPSDK_RELEASE_VERSION_TAG).empty()
                ? Py_STRINGIFY(WINDOWSAPPSDK_RELEASE_MAJOR) "." Py_STRINGIFY(
                      WINDOWSAPPSDK_RELEASE_MINOR)
                : Py_STRINGIFY(WINDOWSAPPSDK_RELEASE_MAJOR) "." Py_STRINGIFY(
                      WINDOWSAPPSDK_RELEASE_MINOR) "-" WINDOWSAPPSDK_RELEASE_VERSION_TAG)
        == -1)
    {
        return nullptr;
    }

    if (PyModule_AddStringConstant(
            module.get(),
            "RUNTIME_VERSION",
            WINDOWSAPPSDK_RUNTIME_VERSION_DOTQUADSTRING)
        == -1)
    {
        return nullptr;
    }

    auto const shutdown_type
        = PyType_FromModuleAndSpec(module.get(), &shutdown_type_spec, nullptr);
    if (!shutdown_type)
        return nullptr;

    auto const added = PyModule_AddType(
        module.get(), reinterpret_cast<PyTypeObject*>(shutdown_type));
    Py_DECREF(shutdown_type);
    if (added == -1)
        return nullptr;

    return module.release();
}
