// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "pybase.h"
#include "py.Windows.ApplicationModel.Payments.Provider.h"


namespace py::cpp::Windows::ApplicationModel::Payments::Provider
{
    // ----- PaymentAppCanMakePaymentTriggerDetails class --------------------
    static constexpr const char* const type_name_PaymentAppCanMakePaymentTriggerDetails = "PaymentAppCanMakePaymentTriggerDetails";

    static PyObject* _new_PaymentAppCanMakePaymentTriggerDetails(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        py::set_invalid_activation_error(type_name_PaymentAppCanMakePaymentTriggerDetails);
        return nullptr;
    }

    static void _dealloc_PaymentAppCanMakePaymentTriggerDetails(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentAppCanMakePaymentTriggerDetails* self) noexcept
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

    static PyObject* PaymentAppCanMakePaymentTriggerDetails_ReportCanMakePaymentResult(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentAppCanMakePaymentTriggerDetails* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentAppCanMakePaymentTriggerDetails", L"ReportCanMakePaymentResult", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::ApplicationModel::Payments::PaymentCanMakePaymentResult>(args, 0);

                self->obj.ReportCanMakePaymentResult(param0);
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

    static PyObject* PaymentAppCanMakePaymentTriggerDetails_get_Request(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentAppCanMakePaymentTriggerDetails* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentAppCanMakePaymentTriggerDetails", L"Request"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.Request());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_PaymentAppCanMakePaymentTriggerDetails(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::Payments::Provider::PaymentAppCanMakePaymentTriggerDetails>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_PaymentAppCanMakePaymentTriggerDetails(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::Payments::Provider::PaymentAppCanMakePaymentTriggerDetails>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_PaymentAppCanMakePaymentTriggerDetails[] = {
        { "report_can_make_payment_result", reinterpret_cast<PyCFunction>(PaymentAppCanMakePaymentTriggerDetails_ReportCanMakePaymentResult), METH_VARARGS, nullptr },
        { "_assign_array_", _assign_array_PaymentAppCanMakePaymentTriggerDetails, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_PaymentAppCanMakePaymentTriggerDetails), METH_O | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_PaymentAppCanMakePaymentTriggerDetails[] = {
        { "request", reinterpret_cast<getter>(PaymentAppCanMakePaymentTriggerDetails_get_Request), nullptr, nullptr, nullptr },
        { }
    };

    static PyType_Slot _type_slots_PaymentAppCanMakePaymentTriggerDetails[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_PaymentAppCanMakePaymentTriggerDetails) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_PaymentAppCanMakePaymentTriggerDetails) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_PaymentAppCanMakePaymentTriggerDetails) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_PaymentAppCanMakePaymentTriggerDetails) },
        { },
    };

    static PyType_Spec type_spec_PaymentAppCanMakePaymentTriggerDetails =
    {
        "_winrt_windows_applicationmodel_payments_provider.PaymentAppCanMakePaymentTriggerDetails",
        sizeof(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentAppCanMakePaymentTriggerDetails),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_PaymentAppCanMakePaymentTriggerDetails
    };

    // ----- PaymentAppManager class --------------------
    static constexpr const char* const type_name_PaymentAppManager = "PaymentAppManager";

    static PyObject* _new_PaymentAppManager(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        py::set_invalid_activation_error(type_name_PaymentAppManager);
        return nullptr;
    }

    static void _dealloc_PaymentAppManager(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentAppManager* self) noexcept
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

    static PyObject* PaymentAppManager_RegisterAsync(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentAppManager* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentAppManager", L"RegisterAsync", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::Foundation::Collections::IIterable<winrt::hstring>>(args, 0);

                return py::convert(self->obj.RegisterAsync(param0));
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

    static PyObject* PaymentAppManager_UnregisterAsync(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentAppManager* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 0)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentAppManager", L"UnregisterAsync", 0))
            {
                py::set_arg_count_version_error(0);
                return nullptr;
            }

            try
            {
                return py::convert(self->obj.UnregisterAsync());
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

    static PyObject* PaymentAppManager_get_Current(PyObject* /*unused*/, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentAppManager", L"Current"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(winrt::Windows::ApplicationModel::Payments::Provider::PaymentAppManager::Current());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_PaymentAppManager(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::Payments::Provider::PaymentAppManager>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_PaymentAppManager(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::Payments::Provider::PaymentAppManager>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_PaymentAppManager[] = {
        { "register_async", reinterpret_cast<PyCFunction>(PaymentAppManager_RegisterAsync), METH_VARARGS, nullptr },
        { "unregister_async", reinterpret_cast<PyCFunction>(PaymentAppManager_UnregisterAsync), METH_VARARGS, nullptr },
        { "_assign_array_", _assign_array_PaymentAppManager, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_PaymentAppManager), METH_O | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_PaymentAppManager[] = {
        { }
    };

    static PyType_Slot _type_slots_PaymentAppManager[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_PaymentAppManager) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_PaymentAppManager) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_PaymentAppManager) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_PaymentAppManager) },
        { },
    };

    static PyType_Spec type_spec_PaymentAppManager =
    {
        "_winrt_windows_applicationmodel_payments_provider.PaymentAppManager",
        sizeof(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentAppManager),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_PaymentAppManager
    };

    static PyGetSetDef getset_PaymentAppManager_Meta[] = {
        { "current", reinterpret_cast<getter>(PaymentAppManager_get_Current), nullptr, nullptr, nullptr },
        { }
    };

    static PyType_Slot type_slots_PaymentAppManager_Meta[] = 
    {
        { Py_tp_base, reinterpret_cast<void*>(&PyType_Type) },
        { Py_tp_getset, reinterpret_cast<void*>(getset_PaymentAppManager_Meta) },
        { }
    };

    static PyType_Spec type_spec_PaymentAppManager_Meta =
    {
        "_winrt_windows_applicationmodel_payments_provider.PaymentAppManager_Meta",
        static_cast<int>(PyType_Type.tp_basicsize),
        static_cast<int>(PyType_Type.tp_itemsize),
        Py_TPFLAGS_DEFAULT,
        type_slots_PaymentAppManager_Meta
    };

    // ----- PaymentTransaction class --------------------
    static constexpr const char* const type_name_PaymentTransaction = "PaymentTransaction";

    static PyObject* _new_PaymentTransaction(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        py::set_invalid_activation_error(type_name_PaymentTransaction);
        return nullptr;
    }

    static void _dealloc_PaymentTransaction(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self) noexcept
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

    static PyObject* PaymentTransaction_AcceptAsync(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"AcceptAsync", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::ApplicationModel::Payments::PaymentToken>(args, 0);

                return py::convert(self->obj.AcceptAsync(param0));
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

    static PyObject* PaymentTransaction_FromIdAsync(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"FromIdAsync", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::hstring>(args, 0);

                return py::convert(winrt::Windows::ApplicationModel::Payments::Provider::PaymentTransaction::FromIdAsync(param0));
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

    static PyObject* PaymentTransaction_Reject(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 0)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"Reject", 0))
            {
                py::set_arg_count_version_error(0);
                return nullptr;
            }

            try
            {
                self->obj.Reject();
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

    static PyObject* PaymentTransaction_UpdateSelectedShippingOptionAsync(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"UpdateSelectedShippingOptionAsync", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::ApplicationModel::Payments::PaymentShippingOption>(args, 0);

                return py::convert(self->obj.UpdateSelectedShippingOptionAsync(param0));
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

    static PyObject* PaymentTransaction_UpdateShippingAddressAsync(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"UpdateShippingAddressAsync", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::ApplicationModel::Payments::PaymentAddress>(args, 0);

                return py::convert(self->obj.UpdateShippingAddressAsync(param0));
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

    static PyObject* PaymentTransaction_get_PayerPhoneNumber(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"PayerPhoneNumber"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.PayerPhoneNumber());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int PaymentTransaction_put_PayerPhoneNumber(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, PyObject* arg, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"PayerPhoneNumber"))
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

            self->obj.PayerPhoneNumber(param0);
            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* PaymentTransaction_get_PayerName(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"PayerName"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.PayerName());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int PaymentTransaction_put_PayerName(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, PyObject* arg, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"PayerName"))
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

            self->obj.PayerName(param0);
            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* PaymentTransaction_get_PayerEmail(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"PayerEmail"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.PayerEmail());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int PaymentTransaction_put_PayerEmail(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, PyObject* arg, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"PayerEmail"))
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

            self->obj.PayerEmail(param0);
            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* PaymentTransaction_get_PaymentRequest(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransaction", L"PaymentRequest"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.PaymentRequest());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_PaymentTransaction(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::Payments::Provider::PaymentTransaction>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_PaymentTransaction(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::Payments::Provider::PaymentTransaction>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_PaymentTransaction[] = {
        { "accept_async", reinterpret_cast<PyCFunction>(PaymentTransaction_AcceptAsync), METH_VARARGS, nullptr },
        { "from_id_async", reinterpret_cast<PyCFunction>(PaymentTransaction_FromIdAsync), METH_VARARGS | METH_STATIC, nullptr },
        { "reject", reinterpret_cast<PyCFunction>(PaymentTransaction_Reject), METH_VARARGS, nullptr },
        { "update_selected_shipping_option_async", reinterpret_cast<PyCFunction>(PaymentTransaction_UpdateSelectedShippingOptionAsync), METH_VARARGS, nullptr },
        { "update_shipping_address_async", reinterpret_cast<PyCFunction>(PaymentTransaction_UpdateShippingAddressAsync), METH_VARARGS, nullptr },
        { "_assign_array_", _assign_array_PaymentTransaction, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_PaymentTransaction), METH_O | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_PaymentTransaction[] = {
        { "payer_phone_number", reinterpret_cast<getter>(PaymentTransaction_get_PayerPhoneNumber), reinterpret_cast<setter>(PaymentTransaction_put_PayerPhoneNumber), nullptr, nullptr },
        { "payer_name", reinterpret_cast<getter>(PaymentTransaction_get_PayerName), reinterpret_cast<setter>(PaymentTransaction_put_PayerName), nullptr, nullptr },
        { "payer_email", reinterpret_cast<getter>(PaymentTransaction_get_PayerEmail), reinterpret_cast<setter>(PaymentTransaction_put_PayerEmail), nullptr, nullptr },
        { "payment_request", reinterpret_cast<getter>(PaymentTransaction_get_PaymentRequest), nullptr, nullptr, nullptr },
        { }
    };

    static PyType_Slot _type_slots_PaymentTransaction[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_PaymentTransaction) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_PaymentTransaction) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_PaymentTransaction) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_PaymentTransaction) },
        { },
    };

    static PyType_Spec type_spec_PaymentTransaction =
    {
        "_winrt_windows_applicationmodel_payments_provider.PaymentTransaction",
        sizeof(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransaction),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_PaymentTransaction
    };

    // ----- PaymentTransactionAcceptResult class --------------------
    static constexpr const char* const type_name_PaymentTransactionAcceptResult = "PaymentTransactionAcceptResult";

    static PyObject* _new_PaymentTransactionAcceptResult(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        py::set_invalid_activation_error(type_name_PaymentTransactionAcceptResult);
        return nullptr;
    }

    static void _dealloc_PaymentTransactionAcceptResult(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransactionAcceptResult* self) noexcept
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

    static PyObject* PaymentTransactionAcceptResult_get_Status(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransactionAcceptResult* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.Payments.Provider.PaymentTransactionAcceptResult", L"Status"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.Status());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_PaymentTransactionAcceptResult(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::Payments::Provider::PaymentTransactionAcceptResult>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_PaymentTransactionAcceptResult(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::Payments::Provider::PaymentTransactionAcceptResult>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_PaymentTransactionAcceptResult[] = {
        { "_assign_array_", _assign_array_PaymentTransactionAcceptResult, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_PaymentTransactionAcceptResult), METH_O | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_PaymentTransactionAcceptResult[] = {
        { "status", reinterpret_cast<getter>(PaymentTransactionAcceptResult_get_Status), nullptr, nullptr, nullptr },
        { }
    };

    static PyType_Slot _type_slots_PaymentTransactionAcceptResult[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_PaymentTransactionAcceptResult) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_PaymentTransactionAcceptResult) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_PaymentTransactionAcceptResult) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_PaymentTransactionAcceptResult) },
        { },
    };

    static PyType_Spec type_spec_PaymentTransactionAcceptResult =
    {
        "_winrt_windows_applicationmodel_payments_provider.PaymentTransactionAcceptResult",
        sizeof(py::wrapper::Windows::ApplicationModel::Payments::Provider::PaymentTransactionAcceptResult),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_PaymentTransactionAcceptResult
    };

    // ----- Windows.ApplicationModel.Payments.Provider Initialization --------------------
    PyDoc_STRVAR(module_doc, "Windows::ApplicationModel::Payments::Provider");


    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_applicationmodel_payments_provider",
           module_doc,
           0,
           nullptr,
           nullptr,
           nullptr,
           nullptr,
           nullptr};

} // py::cpp::Windows::ApplicationModel::Payments::Provider

PyMODINIT_FUNC PyInit__winrt_windows_applicationmodel_payments_provider(void) noexcept
{
    using namespace py::cpp::Windows::ApplicationModel::Payments::Provider;

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
    if (py::register_python_type(module.get(), type_name_PaymentAppCanMakePaymentTriggerDetails, &type_spec_PaymentAppCanMakePaymentTriggerDetails, nullptr, object_bases.get(), nullptr) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_PaymentAppCanMakePaymentTriggerDetails, &type_spec_PaymentAppCanMakePaymentTriggerDetails, object_bases.get(), nullptr) == -1)
    #endif
    {
        return nullptr;
    }

    py::pyobj_handle type_PaymentAppManager_Meta{PyType_FromSpec(&type_spec_PaymentAppManager_Meta)};
    if (!type_PaymentAppManager_Meta)
    {
        return nullptr;
    }

    #if PY_VERSION_HEX < 0x03090000
    if (py::register_python_type(module.get(), type_name_PaymentAppManager, &type_spec_PaymentAppManager, nullptr, object_bases.get(), reinterpret_cast<PyTypeObject*>(type_PaymentAppManager_Meta.get())) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_PaymentAppManager, &type_spec_PaymentAppManager, object_bases.get(), reinterpret_cast<PyTypeObject*>(type_PaymentAppManager_Meta.get())) == -1)
    #endif
    {
        return nullptr;
    }

    #if PY_VERSION_HEX < 0x03090000
    if (py::register_python_type(module.get(), type_name_PaymentTransaction, &type_spec_PaymentTransaction, nullptr, object_bases.get(), nullptr) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_PaymentTransaction, &type_spec_PaymentTransaction, object_bases.get(), nullptr) == -1)
    #endif
    {
        return nullptr;
    }

    #if PY_VERSION_HEX < 0x03090000
    if (py::register_python_type(module.get(), type_name_PaymentTransactionAcceptResult, &type_spec_PaymentTransactionAcceptResult, nullptr, object_bases.get(), nullptr) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_PaymentTransactionAcceptResult, &type_spec_PaymentTransactionAcceptResult, object_bases.get(), nullptr) == -1)
    #endif
    {
        return nullptr;
    }


    return module.detach();
}