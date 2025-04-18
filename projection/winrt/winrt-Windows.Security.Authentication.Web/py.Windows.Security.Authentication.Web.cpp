// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.Security.Authentication.Web.h"

namespace py::cpp::Windows::Security::Authentication::Web
{
    // ----- WebAuthenticationBroker class --------------------

    static PyObject* _new_WebAuthenticationBroker(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker>::type_name);
        return nullptr;
    }

    static PyObject* WebAuthenticationBroker_AuthenticateAndContinue(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authentication.Web.WebAuthenticationBroker", L"AuthenticateAndContinue", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Foundation::Uri>(args, 0);

                {
                    auto _gil = release_gil();
                    winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker::AuthenticateAndContinue(param0);
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

    static PyObject* WebAuthenticationBroker_AuthenticateSilentlyAsync(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authentication.Web.WebAuthenticationBroker", L"AuthenticateSilentlyAsync", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Foundation::Uri>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker::AuthenticateSilentlyAsync(param0);
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

    static PyObject* WebAuthenticationBroker_AuthenticateSilentlyWithOptionsAsync(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 2)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authentication.Web.WebAuthenticationBroker", L"AuthenticateSilentlyAsync", 2);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(2);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Foundation::Uri>(args, 0);
                auto param1 = py::convert_to<winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions>(args, 1);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker::AuthenticateSilentlyAsync(param0, param1);
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

    static PyObject* WebAuthenticationBroker_AuthenticateWithCallbackUriAndContinue(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 2)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authentication.Web.WebAuthenticationBroker", L"AuthenticateAndContinue", 2);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(2);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Foundation::Uri>(args, 0);
                auto param1 = py::convert_to<winrt::Windows::Foundation::Uri>(args, 1);

                {
                    auto _gil = release_gil();
                    winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker::AuthenticateAndContinue(param0, param1);
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

    static PyObject* WebAuthenticationBroker_AuthenticateWithCallbackUriAsync(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 3)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authentication.Web.WebAuthenticationBroker", L"AuthenticateAsync", 3);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(3);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions>(args, 0);
                auto param1 = py::convert_to<winrt::Windows::Foundation::Uri>(args, 1);
                auto param2 = py::convert_to<winrt::Windows::Foundation::Uri>(args, 2);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker::AuthenticateAsync(param0, param1, param2);
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

    static PyObject* WebAuthenticationBroker_AuthenticateWithCallbackUriContinuationDataAndOptionsAndContinue(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 4)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authentication.Web.WebAuthenticationBroker", L"AuthenticateAndContinue", 4);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(4);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Foundation::Uri>(args, 0);
                auto param1 = py::convert_to<winrt::Windows::Foundation::Uri>(args, 1);
                auto param2 = py::convert_to<winrt::Windows::Foundation::Collections::ValueSet>(args, 2);
                auto param3 = py::convert_to<winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions>(args, 3);

                {
                    auto _gil = release_gil();
                    winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker::AuthenticateAndContinue(param0, param1, param2, param3);
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

    static PyObject* WebAuthenticationBroker_AuthenticateWithoutCallbackUriAsync(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 2)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authentication.Web.WebAuthenticationBroker", L"AuthenticateAsync", 2);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(2);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions>(args, 0);
                auto param1 = py::convert_to<winrt::Windows::Foundation::Uri>(args, 1);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker::AuthenticateAsync(param0, param1);
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

    static PyObject* WebAuthenticationBroker_GetCurrentApplicationCallbackUri(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.Authentication.Web.WebAuthenticationBroker", L"GetCurrentApplicationCallbackUri", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker::GetCurrentApplicationCallbackUri();
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

    static PyMethodDef _methods_WebAuthenticationBroker[] = {
        { }};

    static PyGetSetDef _getset_WebAuthenticationBroker[] = {
        { }};

    static PyType_Slot _type_slots_WebAuthenticationBroker[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_WebAuthenticationBroker) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_WebAuthenticationBroker) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_WebAuthenticationBroker) },
        { }};

    static PyType_Spec type_spec_WebAuthenticationBroker = {
        "winrt._winrt_windows_security_authentication_web.WebAuthenticationBroker",
        0,
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_WebAuthenticationBroker};

    static PyGetSetDef getset_WebAuthenticationBroker_Static[] = {
        { }};

    static PyMethodDef methods_WebAuthenticationBroker_Static[] = {
        { "authenticate_and_continue", reinterpret_cast<PyCFunction>(WebAuthenticationBroker_AuthenticateAndContinue), METH_VARARGS, nullptr },
        { "authenticate_silently_async", reinterpret_cast<PyCFunction>(WebAuthenticationBroker_AuthenticateSilentlyAsync), METH_VARARGS, nullptr },
        { "authenticate_silently_with_options_async", reinterpret_cast<PyCFunction>(WebAuthenticationBroker_AuthenticateSilentlyWithOptionsAsync), METH_VARARGS, nullptr },
        { "authenticate_with_callback_uri_and_continue", reinterpret_cast<PyCFunction>(WebAuthenticationBroker_AuthenticateWithCallbackUriAndContinue), METH_VARARGS, nullptr },
        { "authenticate_with_callback_uri_async", reinterpret_cast<PyCFunction>(WebAuthenticationBroker_AuthenticateWithCallbackUriAsync), METH_VARARGS, nullptr },
        { "authenticate_with_callback_uri_continuation_data_and_options_and_continue", reinterpret_cast<PyCFunction>(WebAuthenticationBroker_AuthenticateWithCallbackUriContinuationDataAndOptionsAndContinue), METH_VARARGS, nullptr },
        { "authenticate_without_callback_uri_async", reinterpret_cast<PyCFunction>(WebAuthenticationBroker_AuthenticateWithoutCallbackUriAsync), METH_VARARGS, nullptr },
        { "get_current_application_callback_uri", reinterpret_cast<PyCFunction>(WebAuthenticationBroker_GetCurrentApplicationCallbackUri), METH_VARARGS, nullptr },
        { }};

    static PyType_Slot type_slots_WebAuthenticationBroker_Static[] = 
    {
        { Py_tp_base, reinterpret_cast<void*>(&PyType_Type) },
        { Py_tp_getset, reinterpret_cast<void*>(getset_WebAuthenticationBroker_Static) },
        { Py_tp_methods, reinterpret_cast<void*>(methods_WebAuthenticationBroker_Static) },
        { }
    };

    static PyType_Spec type_spec_WebAuthenticationBroker_Static = {
        "winrt._winrt_windows_security_authentication_web.WebAuthenticationBroker_Static",
        static_cast<int>(PyType_Type.tp_basicsize),
        static_cast<int>(PyType_Type.tp_itemsize),
        Py_TPFLAGS_DEFAULT,
        type_slots_WebAuthenticationBroker_Static};

    // ----- WebAuthenticationResult class --------------------

    static PyObject* _new_WebAuthenticationResult(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>::type_name);
        return nullptr;
    }

    static void _dealloc_WebAuthenticationResult(py::wrapper::Windows::Security::Authentication::Web::WebAuthenticationResult* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* WebAuthenticationResult_get_ResponseData(py::wrapper::Windows::Security::Authentication::Web::WebAuthenticationResult* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Security.Authentication.Web.WebAuthenticationResult", L"ResponseData");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ResponseData();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* WebAuthenticationResult_get_ResponseErrorDetail(py::wrapper::Windows::Security::Authentication::Web::WebAuthenticationResult* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Security.Authentication.Web.WebAuthenticationResult", L"ResponseErrorDetail");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ResponseErrorDetail();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* WebAuthenticationResult_get_ResponseStatus(py::wrapper::Windows::Security::Authentication::Web::WebAuthenticationResult* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Security.Authentication.Web.WebAuthenticationResult", L"ResponseStatus");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ResponseStatus();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_WebAuthenticationResult(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_WebAuthenticationResult(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_WebAuthenticationResult[] = {
        { "_assign_array_", _assign_array_WebAuthenticationResult, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_WebAuthenticationResult), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_WebAuthenticationResult[] = {
        { "response_data", reinterpret_cast<getter>(WebAuthenticationResult_get_ResponseData), nullptr, nullptr, nullptr },
        { "response_error_detail", reinterpret_cast<getter>(WebAuthenticationResult_get_ResponseErrorDetail), nullptr, nullptr, nullptr },
        { "response_status", reinterpret_cast<getter>(WebAuthenticationResult_get_ResponseStatus), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_WebAuthenticationResult[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_WebAuthenticationResult) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_WebAuthenticationResult) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_WebAuthenticationResult) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_WebAuthenticationResult) },
        { }};

    static PyType_Spec type_spec_WebAuthenticationResult = {
        "winrt._winrt_windows_security_authentication_web.WebAuthenticationResult",
        sizeof(py::wrapper::Windows::Security::Authentication::Web::WebAuthenticationResult),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_WebAuthenticationResult};

    // ----- Windows.Security.Authentication.Web Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.Security.Authentication.Web");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_security_authentication_web",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::Security::Authentication::Web

PyMODINIT_FUNC PyInit__winrt_windows_security_authentication_web(void) noexcept
{
    using namespace py::cpp::Windows::Security::Authentication::Web;

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

    py::pyobj_handle WebAuthenticationBroker_Static_bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(inspectable_meta_type))};
    if (!WebAuthenticationBroker_Static_bases)
    {
        return nullptr;
    }

    py::pyobj_handle type_WebAuthenticationBroker_Static{PyType_FromSpecWithBases(&type_spec_WebAuthenticationBroker_Static, WebAuthenticationBroker_Static_bases.get())};
    if (!type_WebAuthenticationBroker_Static)
    {
        return nullptr;
    }

    py::pytype_handle WebAuthenticationBroker_type{py::register_python_type(module.get(), &type_spec_WebAuthenticationBroker, object_bases.get(), reinterpret_cast<PyTypeObject*>(type_WebAuthenticationBroker_Static.get()))};
    if (!WebAuthenticationBroker_type)
    {
        return nullptr;
    }

    py::pytype_handle WebAuthenticationResult_type{py::register_python_type(module.get(), &type_spec_WebAuthenticationResult, object_bases.get(), inspectable_meta_type)};
    if (!WebAuthenticationResult_type)
    {
        return nullptr;
    }


    return module.detach();
}
