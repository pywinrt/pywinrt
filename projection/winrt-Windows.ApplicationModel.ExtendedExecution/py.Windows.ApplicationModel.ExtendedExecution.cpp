// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "pybase.h"
#include "py.Windows.ApplicationModel.ExtendedExecution.h"


namespace py::cpp::Windows::ApplicationModel::ExtendedExecution
{
    // ----- ExtendedExecutionRevokedEventArgs class --------------------
    static constexpr const char* const type_name_ExtendedExecutionRevokedEventArgs = "ExtendedExecutionRevokedEventArgs";

    static PyObject* _new_ExtendedExecutionRevokedEventArgs(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        py::set_invalid_activation_error(type_name_ExtendedExecutionRevokedEventArgs);
        return nullptr;
    }

    static void _dealloc_ExtendedExecutionRevokedEventArgs(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionRevokedEventArgs* self) noexcept
    {
        auto tp = Py_TYPE(self);

        if (PyType_IS_GC(tp))
        {
            PyObject_GC_UnTrack(self);
        }

        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* ExtendedExecutionRevokedEventArgs_get_Reason(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionRevokedEventArgs* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionRevokedEventArgs", L"Reason"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.Reason());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_ExtendedExecutionRevokedEventArgs(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionRevokedEventArgs>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_ExtendedExecutionRevokedEventArgs(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionRevokedEventArgs>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_ExtendedExecutionRevokedEventArgs[] = {
        { "_assign_array_", _assign_array_ExtendedExecutionRevokedEventArgs, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_ExtendedExecutionRevokedEventArgs), METH_O | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_ExtendedExecutionRevokedEventArgs[] = {
        { "reason", reinterpret_cast<getter>(ExtendedExecutionRevokedEventArgs_get_Reason), nullptr, nullptr, nullptr },
        { }
    };

    static PyType_Slot _type_slots_ExtendedExecutionRevokedEventArgs[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_ExtendedExecutionRevokedEventArgs) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_ExtendedExecutionRevokedEventArgs) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_ExtendedExecutionRevokedEventArgs) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_ExtendedExecutionRevokedEventArgs) },
        { },
    };

    static PyType_Spec type_spec_ExtendedExecutionRevokedEventArgs =
    {
        "_winrt_windows_applicationmodel_extendedexecution.ExtendedExecutionRevokedEventArgs",
        sizeof(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionRevokedEventArgs),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_ExtendedExecutionRevokedEventArgs
    };

    // ----- ExtendedExecutionSession class --------------------
    static constexpr const char* const type_name_ExtendedExecutionSession = "ExtendedExecutionSession";

    static PyObject* _new_ExtendedExecutionSession(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        if (kwds != nullptr)
        {
            py::set_invalid_kwd_args_error();
            return nullptr;
        }

        auto arg_count = PyTuple_Size(args);
        if (arg_count == 0)
        {
            try
            {
                winrt::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession instance{  };
                return py::wrap(instance, type);
            }
            catch (...)
            {
                py::to_PyErr();
                return nullptr;
            }
        }
        else
        {
            py::set_invalid_arg_count_error(arg_count);
            return nullptr;
        }
    }

    static void _dealloc_ExtendedExecutionSession(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self) noexcept
    {
        auto tp = Py_TYPE(self);

        if (PyType_IS_GC(tp))
        {
            PyObject_GC_UnTrack(self);
        }

        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* ExtendedExecutionSession_Close(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 0)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession", L"Close", 0))
            {
                py::set_arg_count_version_error(0);
                return nullptr;
            }

            try
            {
                self->obj.Close();
                Py_RETURN_NONE;
            }
            catch (...)
            {
                py::to_PyErr();
                return nullptr;
            }
        }
        else
        {
            py::set_invalid_arg_count_error(arg_count);
            return nullptr;
        }
    }

    static PyObject* ExtendedExecutionSession_RequestExtensionAsync(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 0)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession", L"RequestExtensionAsync", 0))
            {
                py::set_arg_count_version_error(0);
                return nullptr;
            }

            try
            {
                return py::convert(self->obj.RequestExtensionAsync());
            }
            catch (...)
            {
                py::to_PyErr();
                return nullptr;
            }
        }
        else
        {
            py::set_invalid_arg_count_error(arg_count);
            return nullptr;
        }
    }

    static PyObject* ExtendedExecutionSession_get_Reason(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession", L"Reason"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.Reason());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ExtendedExecutionSession_put_Reason(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self, PyObject* arg, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession", L"Reason"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return -1;
        }

        if (arg == nullptr)
        {
            PyErr_SetString(PyExc_AttributeError, "can't delete attribute");
            return -1;
        }

        try
        {
            auto param0 = py::convert_to<winrt::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionReason>(arg);

            self->obj.Reason(param0);
            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* ExtendedExecutionSession_get_PercentProgress(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession", L"PercentProgress"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.PercentProgress());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ExtendedExecutionSession_put_PercentProgress(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self, PyObject* arg, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession", L"PercentProgress"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return -1;
        }

        if (arg == nullptr)
        {
            PyErr_SetString(PyExc_AttributeError, "can't delete attribute");
            return -1;
        }

        try
        {
            auto param0 = py::convert_to<uint32_t>(arg);

            self->obj.PercentProgress(param0);
            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* ExtendedExecutionSession_get_Description(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession", L"Description"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.Description());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ExtendedExecutionSession_put_Description(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self, PyObject* arg, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession", L"Description"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return -1;
        }

        if (arg == nullptr)
        {
            PyErr_SetString(PyExc_AttributeError, "can't delete attribute");
            return -1;
        }

        try
        {
            auto param0 = py::convert_to<winrt::hstring>(arg);

            self->obj.Description(param0);
            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* ExtendedExecutionSession_add_Revoked(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self, PyObject* arg) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession", L"Revoked"))
        {
            PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            auto param0 = py::convert_to<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Foundation::IInspectable, winrt::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionRevokedEventArgs>>(arg);

            return py::convert(self->obj.Revoked(param0));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ExtendedExecutionSession_remove_Revoked(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self, PyObject* arg) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession", L"Revoked"))
        {
            PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            auto param0 = py::convert_to<winrt::event_token>(arg);

            self->obj.Revoked(param0);
            Py_RETURN_NONE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_ExtendedExecutionSession(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_ExtendedExecutionSession(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _enter_ExtendedExecutionSession(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self) noexcept
    {
        Py_INCREF(self);
        return reinterpret_cast<PyObject*>(self);
    }

    static PyObject* _exit_ExtendedExecutionSession(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession* self) noexcept
    {
        try
        {
            self->obj.Close();
            Py_RETURN_FALSE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_ExtendedExecutionSession[] = {
        { "close", reinterpret_cast<PyCFunction>(ExtendedExecutionSession_Close), METH_VARARGS, nullptr },
        { "request_extension_async", reinterpret_cast<PyCFunction>(ExtendedExecutionSession_RequestExtensionAsync), METH_VARARGS, nullptr },
        { "add_revoked", reinterpret_cast<PyCFunction>(ExtendedExecutionSession_add_Revoked), METH_O, nullptr },
        { "remove_revoked", reinterpret_cast<PyCFunction>(ExtendedExecutionSession_remove_Revoked), METH_O, nullptr },
        { "_assign_array_", _assign_array_ExtendedExecutionSession, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_ExtendedExecutionSession), METH_O | METH_STATIC, nullptr },
        { "__enter__", reinterpret_cast<PyCFunction>(_enter_ExtendedExecutionSession), METH_NOARGS, nullptr },
        { "__exit__",  reinterpret_cast<PyCFunction>(_exit_ExtendedExecutionSession), METH_VARARGS, nullptr },
        { }
    };

    static PyGetSetDef _getset_ExtendedExecutionSession[] = {
        { "reason", reinterpret_cast<getter>(ExtendedExecutionSession_get_Reason), reinterpret_cast<setter>(ExtendedExecutionSession_put_Reason), nullptr, nullptr },
        { "percent_progress", reinterpret_cast<getter>(ExtendedExecutionSession_get_PercentProgress), reinterpret_cast<setter>(ExtendedExecutionSession_put_PercentProgress), nullptr, nullptr },
        { "description", reinterpret_cast<getter>(ExtendedExecutionSession_get_Description), reinterpret_cast<setter>(ExtendedExecutionSession_put_Description), nullptr, nullptr },
        { }
    };

    static PyType_Slot _type_slots_ExtendedExecutionSession[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_ExtendedExecutionSession) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_ExtendedExecutionSession) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_ExtendedExecutionSession) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_ExtendedExecutionSession) },
        { },
    };

    static PyType_Spec type_spec_ExtendedExecutionSession =
    {
        "_winrt_windows_applicationmodel_extendedexecution.ExtendedExecutionSession",
        sizeof(py::wrapper::Windows::ApplicationModel::ExtendedExecution::ExtendedExecutionSession),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_ExtendedExecutionSession
    };

    // ----- Windows.ApplicationModel.ExtendedExecution Initialization --------------------
    PyDoc_STRVAR(module_doc, "Windows::ApplicationModel::ExtendedExecution");


    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_applicationmodel_extendedexecution",
           module_doc,
           0,
           nullptr,
           nullptr,
           nullptr,
           nullptr,
           nullptr};

} // py::cpp::Windows::ApplicationModel::ExtendedExecution

PyMODINIT_FUNC PyInit__winrt_windows_applicationmodel_extendedexecution(void) noexcept
{
    using namespace py::cpp::Windows::ApplicationModel::ExtendedExecution;

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

    #if PY_VERSION_HEX < 0x03090000
    if (py::register_python_type(module.get(), type_name_ExtendedExecutionRevokedEventArgs, &type_spec_ExtendedExecutionRevokedEventArgs, nullptr, object_bases.get(), nullptr) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_ExtendedExecutionRevokedEventArgs, &type_spec_ExtendedExecutionRevokedEventArgs, object_bases.get(), nullptr) == -1)
    #endif
    {
        return nullptr;
    }

    #if PY_VERSION_HEX < 0x03090000
    if (py::register_python_type(module.get(), type_name_ExtendedExecutionSession, &type_spec_ExtendedExecutionSession, nullptr, object_bases.get(), nullptr) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_ExtendedExecutionSession, &type_spec_ExtendedExecutionSession, object_bases.get(), nullptr) == -1)
    #endif
    {
        return nullptr;
    }


    return module.detach();
}