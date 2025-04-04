// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.Security.Authorization.AppCapabilityAccess.h"

namespace py::cpp::Windows::Security::Authorization::AppCapabilityAccess
{
    // ----- AppCapability class --------------------

    static PyObject* _new_AppCapability(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapability>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapability>::type_name);
        return nullptr;
    }

    static void _dealloc_AppCapability(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapability* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* AppCapability_CheckAccess(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapability* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"CheckAccess", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.CheckAccess();
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

    static PyObject* AppCapability_Create(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"Create", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::hstring>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapability::Create(param0);
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

    static PyObject* AppCapability_CreateWithProcessIdForUser(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 3)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"CreateWithProcessIdForUser", 3);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(3);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::System::User>(args, 0);
                auto param1 = py::convert_to<winrt::hstring>(args, 1);
                auto param2 = py::convert_to<uint32_t>(args, 2);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapability::CreateWithProcessIdForUser(param0, param1, param2);
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

    static PyObject* AppCapability_RequestAccessAsync(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapability* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"RequestAccessAsync", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.RequestAccessAsync();
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

    static PyObject* AppCapability_RequestAccessForCapabilitiesAsync(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"RequestAccessForCapabilitiesAsync", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Foundation::Collections::IIterable<winrt::hstring>>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapability::RequestAccessForCapabilitiesAsync(param0);
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

    static PyObject* AppCapability_RequestAccessForCapabilitiesForUserAsync(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 2)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"RequestAccessForCapabilitiesForUserAsync", 2);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(2);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::System::User>(args, 0);
                auto param1 = py::convert_to<winrt::Windows::Foundation::Collections::IIterable<winrt::hstring>>(args, 1);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapability::RequestAccessForCapabilitiesForUserAsync(param0, param1);
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

    static PyObject* AppCapability_get_CapabilityName(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapability* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"CapabilityName");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.CapabilityName();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* AppCapability_get_User(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapability* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"User");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.User();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* AppCapability_get_DisplayMessage(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapability* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"DisplayMessage");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.DisplayMessage();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int AppCapability_put_DisplayMessage(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapability* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"DisplayMessage");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::hstring>(arg);

            {
                auto _gil = release_gil();
                self->obj.DisplayMessage(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* AppCapability_add_AccessChanged(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapability* self, PyObject* arg) noexcept
    {
        try
        {
            static std::optional<bool> is_event_present{};

            if (!is_event_present.has_value())
            {
                is_event_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"AccessChanged");
            }

            if (!is_event_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
                return nullptr;
            }

            auto param0 = py::convert_to<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapability, winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapabilityAccessChangedEventArgs>>(arg);

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.AccessChanged(param0);
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* AppCapability_remove_AccessChanged(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapability* self, PyObject* arg) noexcept
    {
        try
        {
            static std::optional<bool> is_event_present{};

            if (!is_event_present.has_value())
            {
                is_event_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.Security.Authorization.AppCapabilityAccess.AppCapability", L"AccessChanged");
            }

            if (!is_event_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
                return nullptr;
            }

            auto param0 = py::convert_to<winrt::event_token>(arg);

            {
                auto _gil = release_gil();
                self->obj.AccessChanged(param0);
            }

            Py_RETURN_NONE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_AppCapability(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapability>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_AppCapability(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapability>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_AppCapability[] = {
        { "check_access", reinterpret_cast<PyCFunction>(AppCapability_CheckAccess), METH_VARARGS, nullptr },
        { "request_access_async", reinterpret_cast<PyCFunction>(AppCapability_RequestAccessAsync), METH_VARARGS, nullptr },
        { "add_access_changed", reinterpret_cast<PyCFunction>(AppCapability_add_AccessChanged), METH_O, nullptr },
        { "remove_access_changed", reinterpret_cast<PyCFunction>(AppCapability_remove_AccessChanged), METH_O, nullptr },
        { "_assign_array_", _assign_array_AppCapability, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_AppCapability), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_AppCapability[] = {
        { "capability_name", reinterpret_cast<getter>(AppCapability_get_CapabilityName), nullptr, nullptr, nullptr },
        { "user", reinterpret_cast<getter>(AppCapability_get_User), nullptr, nullptr, nullptr },
        { "display_message", reinterpret_cast<getter>(AppCapability_get_DisplayMessage), reinterpret_cast<setter>(AppCapability_put_DisplayMessage), nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_AppCapability[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_AppCapability) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_AppCapability) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_AppCapability) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_AppCapability) },
        { }};

    static PyType_Spec type_spec_AppCapability = {
        "winrt._winrt_windows_security_authorization_appcapabilityaccess.AppCapability",
        sizeof(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapability),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_AppCapability};

    static PyGetSetDef getset_AppCapability_Static[] = {
        { }};

    static PyMethodDef methods_AppCapability_Static[] = {
        { "create", reinterpret_cast<PyCFunction>(AppCapability_Create), METH_VARARGS, nullptr },
        { "create_with_process_id_for_user", reinterpret_cast<PyCFunction>(AppCapability_CreateWithProcessIdForUser), METH_VARARGS, nullptr },
        { "request_access_for_capabilities_async", reinterpret_cast<PyCFunction>(AppCapability_RequestAccessForCapabilitiesAsync), METH_VARARGS, nullptr },
        { "request_access_for_capabilities_for_user_async", reinterpret_cast<PyCFunction>(AppCapability_RequestAccessForCapabilitiesForUserAsync), METH_VARARGS, nullptr },
        { }};

    static PyType_Slot type_slots_AppCapability_Static[] = 
    {
        { Py_tp_base, reinterpret_cast<void*>(&PyType_Type) },
        { Py_tp_getset, reinterpret_cast<void*>(getset_AppCapability_Static) },
        { Py_tp_methods, reinterpret_cast<void*>(methods_AppCapability_Static) },
        { }
    };

    static PyType_Spec type_spec_AppCapability_Static = {
        "winrt._winrt_windows_security_authorization_appcapabilityaccess.AppCapability_Static",
        static_cast<int>(PyType_Type.tp_basicsize),
        static_cast<int>(PyType_Type.tp_itemsize),
        Py_TPFLAGS_DEFAULT,
        type_slots_AppCapability_Static};

    // ----- AppCapabilityAccessChangedEventArgs class --------------------

    static PyObject* _new_AppCapabilityAccessChangedEventArgs(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapabilityAccessChangedEventArgs>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapabilityAccessChangedEventArgs>::type_name);
        return nullptr;
    }

    static void _dealloc_AppCapabilityAccessChangedEventArgs(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapabilityAccessChangedEventArgs* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* _assign_array_AppCapabilityAccessChangedEventArgs(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapabilityAccessChangedEventArgs>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_AppCapabilityAccessChangedEventArgs(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Security::Authorization::AppCapabilityAccess::AppCapabilityAccessChangedEventArgs>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_AppCapabilityAccessChangedEventArgs[] = {
        { "_assign_array_", _assign_array_AppCapabilityAccessChangedEventArgs, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_AppCapabilityAccessChangedEventArgs), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_AppCapabilityAccessChangedEventArgs[] = {
        { }};

    static PyType_Slot _type_slots_AppCapabilityAccessChangedEventArgs[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_AppCapabilityAccessChangedEventArgs) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_AppCapabilityAccessChangedEventArgs) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_AppCapabilityAccessChangedEventArgs) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_AppCapabilityAccessChangedEventArgs) },
        { }};

    static PyType_Spec type_spec_AppCapabilityAccessChangedEventArgs = {
        "winrt._winrt_windows_security_authorization_appcapabilityaccess.AppCapabilityAccessChangedEventArgs",
        sizeof(py::wrapper::Windows::Security::Authorization::AppCapabilityAccess::AppCapabilityAccessChangedEventArgs),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_AppCapabilityAccessChangedEventArgs};

    // ----- Windows.Security.Authorization.AppCapabilityAccess Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.Security.Authorization.AppCapabilityAccess");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_security_authorization_appcapabilityaccess",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::Security::Authorization::AppCapabilityAccess

PyMODINIT_FUNC PyInit__winrt_windows_security_authorization_appcapabilityaccess(void) noexcept
{
    using namespace py::cpp::Windows::Security::Authorization::AppCapabilityAccess;

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

    py::pyobj_handle AppCapability_Static_bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(inspectable_meta_type))};
    if (!AppCapability_Static_bases)
    {
        return nullptr;
    }

    py::pyobj_handle type_AppCapability_Static{PyType_FromSpecWithBases(&type_spec_AppCapability_Static, AppCapability_Static_bases.get())};
    if (!type_AppCapability_Static)
    {
        return nullptr;
    }

    py::pytype_handle AppCapability_type{py::register_python_type(module.get(), &type_spec_AppCapability, object_bases.get(), reinterpret_cast<PyTypeObject*>(type_AppCapability_Static.get()))};
    if (!AppCapability_type)
    {
        return nullptr;
    }

    py::pytype_handle AppCapabilityAccessChangedEventArgs_type{py::register_python_type(module.get(), &type_spec_AppCapabilityAccessChangedEventArgs, object_bases.get(), inspectable_meta_type)};
    if (!AppCapabilityAccessChangedEventArgs_type)
    {
        return nullptr;
    }


    return module.detach();
}
