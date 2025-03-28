// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.ApplicationModel.ExtendedExecution.Foreground.h"

namespace py::cpp::Windows::ApplicationModel::ExtendedExecution::Foreground
{
    // ----- ExtendedExecutionForegroundRevokedEventArgs class --------------------

    static PyObject* _new_ExtendedExecutionForegroundRevokedEventArgs(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundRevokedEventArgs>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundRevokedEventArgs>::type_name);
        return nullptr;
    }

    static void _dealloc_ExtendedExecutionForegroundRevokedEventArgs(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundRevokedEventArgs* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* ExtendedExecutionForegroundRevokedEventArgs_get_Reason(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundRevokedEventArgs* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.Foreground.ExtendedExecutionForegroundRevokedEventArgs", L"Reason");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Reason();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_ExtendedExecutionForegroundRevokedEventArgs(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundRevokedEventArgs>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_ExtendedExecutionForegroundRevokedEventArgs(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundRevokedEventArgs>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_ExtendedExecutionForegroundRevokedEventArgs[] = {
        { "_assign_array_", _assign_array_ExtendedExecutionForegroundRevokedEventArgs, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_ExtendedExecutionForegroundRevokedEventArgs), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_ExtendedExecutionForegroundRevokedEventArgs[] = {
        { "reason", reinterpret_cast<getter>(ExtendedExecutionForegroundRevokedEventArgs_get_Reason), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_ExtendedExecutionForegroundRevokedEventArgs[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_ExtendedExecutionForegroundRevokedEventArgs) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_ExtendedExecutionForegroundRevokedEventArgs) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_ExtendedExecutionForegroundRevokedEventArgs) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_ExtendedExecutionForegroundRevokedEventArgs) },
        { }};

    static PyType_Spec type_spec_ExtendedExecutionForegroundRevokedEventArgs = {
        "winrt._winrt_windows_applicationmodel_extendedexecution_foreground.ExtendedExecutionForegroundRevokedEventArgs",
        sizeof(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundRevokedEventArgs),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_ExtendedExecutionForegroundRevokedEventArgs};

    // ----- ExtendedExecutionForegroundSession class --------------------

    static PyObject* _new_ExtendedExecutionForegroundSession(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        if (kwds)
        {
            py::set_invalid_kwd_args_error();
            return nullptr;
        }

        auto arg_count = PyTuple_GET_SIZE(args);
        if (arg_count == 0)
        {
            try
            {
                winrt::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession instance{};
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

    static void _dealloc_ExtendedExecutionForegroundSession(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* ExtendedExecutionForegroundSession_Close(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.ExtendedExecution.Foreground.ExtendedExecutionForegroundSession", L"Close", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                {
                    auto _gil = release_gil();
                    self->obj.Close();
                }

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

    static PyObject* ExtendedExecutionForegroundSession_RequestExtensionAsync(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.ExtendedExecution.Foreground.ExtendedExecutionForegroundSession", L"RequestExtensionAsync", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.RequestExtensionAsync();
                }());
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

    static PyObject* ExtendedExecutionForegroundSession_get_Reason(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.Foreground.ExtendedExecutionForegroundSession", L"Reason");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Reason();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ExtendedExecutionForegroundSession_put_Reason(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self, PyObject* arg, void* /*unused*/) noexcept
    {
        if (!arg)
        {
            PyErr_SetString(PyExc_AttributeError, "can't delete attribute");
            return -1;
        }

        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.Foreground.ExtendedExecutionForegroundSession", L"Reason");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundReason>(arg);

            {
                auto _gil = release_gil();
                self->obj.Reason(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* ExtendedExecutionForegroundSession_get_Description(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.Foreground.ExtendedExecutionForegroundSession", L"Description");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Description();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ExtendedExecutionForegroundSession_put_Description(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self, PyObject* arg, void* /*unused*/) noexcept
    {
        if (!arg)
        {
            PyErr_SetString(PyExc_AttributeError, "can't delete attribute");
            return -1;
        }

        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.ExtendedExecution.Foreground.ExtendedExecutionForegroundSession", L"Description");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::hstring>(arg);

            {
                auto _gil = release_gil();
                self->obj.Description(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* ExtendedExecutionForegroundSession_add_Revoked(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self, PyObject* arg) noexcept
    {
        try
        {
            static std::optional<bool> is_event_present{};

            if (!is_event_present.has_value())
            {
                is_event_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.ApplicationModel.ExtendedExecution.Foreground.ExtendedExecutionForegroundSession", L"Revoked");
            }

            if (!is_event_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
                return nullptr;
            }

            auto param0 = py::convert_to<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Foundation::IInspectable, winrt::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundRevokedEventArgs>>(arg);

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Revoked(param0);
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ExtendedExecutionForegroundSession_remove_Revoked(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self, PyObject* arg) noexcept
    {
        try
        {
            static std::optional<bool> is_event_present{};

            if (!is_event_present.has_value())
            {
                is_event_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.ApplicationModel.ExtendedExecution.Foreground.ExtendedExecutionForegroundSession", L"Revoked");
            }

            if (!is_event_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
                return nullptr;
            }

            auto param0 = py::convert_to<winrt::event_token>(arg);

            {
                auto _gil = release_gil();
                self->obj.Revoked(param0);
            }

            Py_RETURN_NONE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_ExtendedExecutionForegroundSession(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_ExtendedExecutionForegroundSession(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _enter_ExtendedExecutionForegroundSession(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self, PyObject* /*unused*/) noexcept
    {
        return Py_NewRef(self);
    }

    static PyObject* _exit_ExtendedExecutionForegroundSession(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession* self, PyObject* /*unused*/) noexcept
    {
        try
        {
            {
                auto _gil = py::release_gil();
                self->obj.Close();
            }

            Py_RETURN_FALSE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_ExtendedExecutionForegroundSession[] = {
        { "close", reinterpret_cast<PyCFunction>(ExtendedExecutionForegroundSession_Close), METH_VARARGS, nullptr },
        { "request_extension_async", reinterpret_cast<PyCFunction>(ExtendedExecutionForegroundSession_RequestExtensionAsync), METH_VARARGS, nullptr },
        { "add_revoked", reinterpret_cast<PyCFunction>(ExtendedExecutionForegroundSession_add_Revoked), METH_O, nullptr },
        { "remove_revoked", reinterpret_cast<PyCFunction>(ExtendedExecutionForegroundSession_remove_Revoked), METH_O, nullptr },
        { "_assign_array_", _assign_array_ExtendedExecutionForegroundSession, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_ExtendedExecutionForegroundSession), METH_O | METH_STATIC, nullptr },
        { "__enter__", reinterpret_cast<PyCFunction>(_enter_ExtendedExecutionForegroundSession), METH_NOARGS, nullptr },
        { "__exit__", reinterpret_cast<PyCFunction>(_exit_ExtendedExecutionForegroundSession), METH_VARARGS, nullptr },
        { }};

    static PyGetSetDef _getset_ExtendedExecutionForegroundSession[] = {
        { "reason", reinterpret_cast<getter>(ExtendedExecutionForegroundSession_get_Reason), reinterpret_cast<setter>(ExtendedExecutionForegroundSession_put_Reason), nullptr, nullptr },
        { "description", reinterpret_cast<getter>(ExtendedExecutionForegroundSession_get_Description), reinterpret_cast<setter>(ExtendedExecutionForegroundSession_put_Description), nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_ExtendedExecutionForegroundSession[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_ExtendedExecutionForegroundSession) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_ExtendedExecutionForegroundSession) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_ExtendedExecutionForegroundSession) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_ExtendedExecutionForegroundSession) },
        { }};

    static PyType_Spec type_spec_ExtendedExecutionForegroundSession = {
        "winrt._winrt_windows_applicationmodel_extendedexecution_foreground.ExtendedExecutionForegroundSession",
        sizeof(py::wrapper::Windows::ApplicationModel::ExtendedExecution::Foreground::ExtendedExecutionForegroundSession),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_ExtendedExecutionForegroundSession};

    // ----- Windows.ApplicationModel.ExtendedExecution.Foreground Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.ApplicationModel.ExtendedExecution.Foreground");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_applicationmodel_extendedexecution_foreground",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::ApplicationModel::ExtendedExecution::Foreground

PyMODINIT_FUNC PyInit__winrt_windows_applicationmodel_extendedexecution_foreground(void) noexcept
{
    using namespace py::cpp::Windows::ApplicationModel::ExtendedExecution::Foreground;

    if (py::import_winrt_runtime() == -1)
    {
        return nullptr;
    }

    py::pyobj_handle module{PyModule_Create(&module_def)};

    if (!module)
    {
        return nullptr;
    }

    auto inspectable_meta_type = py::get_inspectable_meta_type();
    if (!inspectable_meta_type)
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

    py::pytype_handle ExtendedExecutionForegroundRevokedEventArgs_type{py::register_python_type(module.get(), &type_spec_ExtendedExecutionForegroundRevokedEventArgs, object_bases.get(), inspectable_meta_type)};
    if (!ExtendedExecutionForegroundRevokedEventArgs_type)
    {
        return nullptr;
    }

    py::pytype_handle ExtendedExecutionForegroundSession_type{py::register_python_type(module.get(), &type_spec_ExtendedExecutionForegroundSession, object_bases.get(), inspectable_meta_type)};
    if (!ExtendedExecutionForegroundSession_type)
    {
        return nullptr;
    }


    return module.detach();
}
