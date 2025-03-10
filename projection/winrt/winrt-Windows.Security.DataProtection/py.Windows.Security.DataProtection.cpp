// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.Security.DataProtection.h"

namespace py::cpp::Windows::Security::DataProtection
{
    // ----- UserDataAvailabilityStateChangedEventArgs class --------------------

    static PyObject* _new_UserDataAvailabilityStateChangedEventArgs(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Security::DataProtection::UserDataAvailabilityStateChangedEventArgs>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Security::DataProtection::UserDataAvailabilityStateChangedEventArgs>::type_name);
        return nullptr;
    }

    static void _dealloc_UserDataAvailabilityStateChangedEventArgs(py::wrapper::Windows::Security::DataProtection::UserDataAvailabilityStateChangedEventArgs* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* UserDataAvailabilityStateChangedEventArgs_GetDeferral(py::wrapper::Windows::Security::DataProtection::UserDataAvailabilityStateChangedEventArgs* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.DataProtection.UserDataAvailabilityStateChangedEventArgs", L"GetDeferral", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.GetDeferral();
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

    static PyObject* _assign_array_UserDataAvailabilityStateChangedEventArgs(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Security::DataProtection::UserDataAvailabilityStateChangedEventArgs>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_UserDataAvailabilityStateChangedEventArgs(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Security::DataProtection::UserDataAvailabilityStateChangedEventArgs>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_UserDataAvailabilityStateChangedEventArgs[] = {
        { "get_deferral", reinterpret_cast<PyCFunction>(UserDataAvailabilityStateChangedEventArgs_GetDeferral), METH_VARARGS, nullptr },
        { "_assign_array_", _assign_array_UserDataAvailabilityStateChangedEventArgs, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_UserDataAvailabilityStateChangedEventArgs), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_UserDataAvailabilityStateChangedEventArgs[] = {
        { }};

    static PyType_Slot _type_slots_UserDataAvailabilityStateChangedEventArgs[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_UserDataAvailabilityStateChangedEventArgs) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_UserDataAvailabilityStateChangedEventArgs) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_UserDataAvailabilityStateChangedEventArgs) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_UserDataAvailabilityStateChangedEventArgs) },
        { }};

    static PyType_Spec type_spec_UserDataAvailabilityStateChangedEventArgs = {
        "winrt._winrt_windows_security_dataprotection.UserDataAvailabilityStateChangedEventArgs",
        sizeof(py::wrapper::Windows::Security::DataProtection::UserDataAvailabilityStateChangedEventArgs),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_UserDataAvailabilityStateChangedEventArgs};

    // ----- UserDataBufferUnprotectResult class --------------------

    static PyObject* _new_UserDataBufferUnprotectResult(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Security::DataProtection::UserDataBufferUnprotectResult>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Security::DataProtection::UserDataBufferUnprotectResult>::type_name);
        return nullptr;
    }

    static void _dealloc_UserDataBufferUnprotectResult(py::wrapper::Windows::Security::DataProtection::UserDataBufferUnprotectResult* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* UserDataBufferUnprotectResult_get_Status(py::wrapper::Windows::Security::DataProtection::UserDataBufferUnprotectResult* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Security.DataProtection.UserDataBufferUnprotectResult", L"Status");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Status();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* UserDataBufferUnprotectResult_get_UnprotectedBuffer(py::wrapper::Windows::Security::DataProtection::UserDataBufferUnprotectResult* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Security.DataProtection.UserDataBufferUnprotectResult", L"UnprotectedBuffer");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.UnprotectedBuffer();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_UserDataBufferUnprotectResult(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Security::DataProtection::UserDataBufferUnprotectResult>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_UserDataBufferUnprotectResult(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Security::DataProtection::UserDataBufferUnprotectResult>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_UserDataBufferUnprotectResult[] = {
        { "_assign_array_", _assign_array_UserDataBufferUnprotectResult, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_UserDataBufferUnprotectResult), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_UserDataBufferUnprotectResult[] = {
        { "status", reinterpret_cast<getter>(UserDataBufferUnprotectResult_get_Status), nullptr, nullptr, nullptr },
        { "unprotected_buffer", reinterpret_cast<getter>(UserDataBufferUnprotectResult_get_UnprotectedBuffer), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_UserDataBufferUnprotectResult[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_UserDataBufferUnprotectResult) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_UserDataBufferUnprotectResult) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_UserDataBufferUnprotectResult) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_UserDataBufferUnprotectResult) },
        { }};

    static PyType_Spec type_spec_UserDataBufferUnprotectResult = {
        "winrt._winrt_windows_security_dataprotection.UserDataBufferUnprotectResult",
        sizeof(py::wrapper::Windows::Security::DataProtection::UserDataBufferUnprotectResult),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_UserDataBufferUnprotectResult};

    // ----- UserDataProtectionManager class --------------------

    static PyObject* _new_UserDataProtectionManager(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Security::DataProtection::UserDataProtectionManager>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Security::DataProtection::UserDataProtectionManager>::type_name);
        return nullptr;
    }

    static void _dealloc_UserDataProtectionManager(py::wrapper::Windows::Security::DataProtection::UserDataProtectionManager* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* UserDataProtectionManager_GetStorageItemProtectionInfoAsync(py::wrapper::Windows::Security::DataProtection::UserDataProtectionManager* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.DataProtection.UserDataProtectionManager", L"GetStorageItemProtectionInfoAsync", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Storage::IStorageItem>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.GetStorageItemProtectionInfoAsync(param0);
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

    static PyObject* UserDataProtectionManager_IsContinuedDataAvailabilityExpected(py::wrapper::Windows::Security::DataProtection::UserDataProtectionManager* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.DataProtection.UserDataProtectionManager", L"IsContinuedDataAvailabilityExpected", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Security::DataProtection::UserDataAvailability>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.IsContinuedDataAvailabilityExpected(param0);
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

    static PyObject* UserDataProtectionManager_ProtectBufferAsync(py::wrapper::Windows::Security::DataProtection::UserDataProtectionManager* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 2)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.DataProtection.UserDataProtectionManager", L"ProtectBufferAsync", 2);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(2);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Storage::Streams::IBuffer>(args, 0);
                auto param1 = py::convert_to<winrt::Windows::Security::DataProtection::UserDataAvailability>(args, 1);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.ProtectBufferAsync(param0, param1);
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

    static PyObject* UserDataProtectionManager_ProtectStorageItemAsync(py::wrapper::Windows::Security::DataProtection::UserDataProtectionManager* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 2)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.DataProtection.UserDataProtectionManager", L"ProtectStorageItemAsync", 2);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(2);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Storage::IStorageItem>(args, 0);
                auto param1 = py::convert_to<winrt::Windows::Security::DataProtection::UserDataAvailability>(args, 1);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.ProtectStorageItemAsync(param0, param1);
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

    static PyObject* UserDataProtectionManager_TryGetDefault(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.DataProtection.UserDataProtectionManager", L"TryGetDefault", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::DataProtection::UserDataProtectionManager::TryGetDefault();
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

    static PyObject* UserDataProtectionManager_TryGetForUser(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.DataProtection.UserDataProtectionManager", L"TryGetForUser", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::System::User>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Security::DataProtection::UserDataProtectionManager::TryGetForUser(param0);
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

    static PyObject* UserDataProtectionManager_UnprotectBufferAsync(py::wrapper::Windows::Security::DataProtection::UserDataProtectionManager* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Security.DataProtection.UserDataProtectionManager", L"UnprotectBufferAsync", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Storage::Streams::IBuffer>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.UnprotectBufferAsync(param0);
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

    static PyObject* UserDataProtectionManager_add_DataAvailabilityStateChanged(py::wrapper::Windows::Security::DataProtection::UserDataProtectionManager* self, PyObject* arg) noexcept
    {
        try
        {
            static std::optional<bool> is_event_present{};

            if (!is_event_present.has_value())
            {
                is_event_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.Security.DataProtection.UserDataProtectionManager", L"DataAvailabilityStateChanged");
            }

            if (!is_event_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
                return nullptr;
            }

            auto param0 = py::convert_to<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Security::DataProtection::UserDataProtectionManager, winrt::Windows::Security::DataProtection::UserDataAvailabilityStateChangedEventArgs>>(arg);

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.DataAvailabilityStateChanged(param0);
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* UserDataProtectionManager_remove_DataAvailabilityStateChanged(py::wrapper::Windows::Security::DataProtection::UserDataProtectionManager* self, PyObject* arg) noexcept
    {
        try
        {
            static std::optional<bool> is_event_present{};

            if (!is_event_present.has_value())
            {
                is_event_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.Security.DataProtection.UserDataProtectionManager", L"DataAvailabilityStateChanged");
            }

            if (!is_event_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
                return nullptr;
            }

            auto param0 = py::convert_to<winrt::event_token>(arg);

            {
                auto _gil = release_gil();
                self->obj.DataAvailabilityStateChanged(param0);
            }

            Py_RETURN_NONE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_UserDataProtectionManager(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Security::DataProtection::UserDataProtectionManager>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_UserDataProtectionManager(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Security::DataProtection::UserDataProtectionManager>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_UserDataProtectionManager[] = {
        { "get_storage_item_protection_info_async", reinterpret_cast<PyCFunction>(UserDataProtectionManager_GetStorageItemProtectionInfoAsync), METH_VARARGS, nullptr },
        { "is_continued_data_availability_expected", reinterpret_cast<PyCFunction>(UserDataProtectionManager_IsContinuedDataAvailabilityExpected), METH_VARARGS, nullptr },
        { "protect_buffer_async", reinterpret_cast<PyCFunction>(UserDataProtectionManager_ProtectBufferAsync), METH_VARARGS, nullptr },
        { "protect_storage_item_async", reinterpret_cast<PyCFunction>(UserDataProtectionManager_ProtectStorageItemAsync), METH_VARARGS, nullptr },
        { "unprotect_buffer_async", reinterpret_cast<PyCFunction>(UserDataProtectionManager_UnprotectBufferAsync), METH_VARARGS, nullptr },
        { "add_data_availability_state_changed", reinterpret_cast<PyCFunction>(UserDataProtectionManager_add_DataAvailabilityStateChanged), METH_O, nullptr },
        { "remove_data_availability_state_changed", reinterpret_cast<PyCFunction>(UserDataProtectionManager_remove_DataAvailabilityStateChanged), METH_O, nullptr },
        { "_assign_array_", _assign_array_UserDataProtectionManager, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_UserDataProtectionManager), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_UserDataProtectionManager[] = {
        { }};

    static PyType_Slot _type_slots_UserDataProtectionManager[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_UserDataProtectionManager) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_UserDataProtectionManager) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_UserDataProtectionManager) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_UserDataProtectionManager) },
        { }};

    static PyType_Spec type_spec_UserDataProtectionManager = {
        "winrt._winrt_windows_security_dataprotection.UserDataProtectionManager",
        sizeof(py::wrapper::Windows::Security::DataProtection::UserDataProtectionManager),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_UserDataProtectionManager};

    static PyGetSetDef getset_UserDataProtectionManager_Static[] = {
        { }};

    static PyMethodDef methods_UserDataProtectionManager_Static[] = {
        { "try_get_default", reinterpret_cast<PyCFunction>(UserDataProtectionManager_TryGetDefault), METH_VARARGS, nullptr },
        { "try_get_for_user", reinterpret_cast<PyCFunction>(UserDataProtectionManager_TryGetForUser), METH_VARARGS, nullptr },
        { }};

    static PyType_Slot type_slots_UserDataProtectionManager_Static[] = 
    {
        { Py_tp_base, reinterpret_cast<void*>(&PyType_Type) },
        { Py_tp_getset, reinterpret_cast<void*>(getset_UserDataProtectionManager_Static) },
        { Py_tp_methods, reinterpret_cast<void*>(methods_UserDataProtectionManager_Static) },
        { }
    };

    static PyType_Spec type_spec_UserDataProtectionManager_Static = {
        "winrt._winrt_windows_security_dataprotection.UserDataProtectionManager_Static",
        static_cast<int>(PyType_Type.tp_basicsize),
        static_cast<int>(PyType_Type.tp_itemsize),
        Py_TPFLAGS_DEFAULT,
        type_slots_UserDataProtectionManager_Static};

    // ----- UserDataStorageItemProtectionInfo class --------------------

    static PyObject* _new_UserDataStorageItemProtectionInfo(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Security::DataProtection::UserDataStorageItemProtectionInfo>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Security::DataProtection::UserDataStorageItemProtectionInfo>::type_name);
        return nullptr;
    }

    static void _dealloc_UserDataStorageItemProtectionInfo(py::wrapper::Windows::Security::DataProtection::UserDataStorageItemProtectionInfo* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* UserDataStorageItemProtectionInfo_get_Availability(py::wrapper::Windows::Security::DataProtection::UserDataStorageItemProtectionInfo* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Security.DataProtection.UserDataStorageItemProtectionInfo", L"Availability");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Availability();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_UserDataStorageItemProtectionInfo(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Security::DataProtection::UserDataStorageItemProtectionInfo>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_UserDataStorageItemProtectionInfo(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Security::DataProtection::UserDataStorageItemProtectionInfo>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_UserDataStorageItemProtectionInfo[] = {
        { "_assign_array_", _assign_array_UserDataStorageItemProtectionInfo, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_UserDataStorageItemProtectionInfo), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_UserDataStorageItemProtectionInfo[] = {
        { "availability", reinterpret_cast<getter>(UserDataStorageItemProtectionInfo_get_Availability), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_UserDataStorageItemProtectionInfo[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_UserDataStorageItemProtectionInfo) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_UserDataStorageItemProtectionInfo) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_UserDataStorageItemProtectionInfo) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_UserDataStorageItemProtectionInfo) },
        { }};

    static PyType_Spec type_spec_UserDataStorageItemProtectionInfo = {
        "winrt._winrt_windows_security_dataprotection.UserDataStorageItemProtectionInfo",
        sizeof(py::wrapper::Windows::Security::DataProtection::UserDataStorageItemProtectionInfo),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_UserDataStorageItemProtectionInfo};

    // ----- Windows.Security.DataProtection Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.Security.DataProtection");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_security_dataprotection",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::Security::DataProtection

PyMODINIT_FUNC PyInit__winrt_windows_security_dataprotection(void) noexcept
{
    using namespace py::cpp::Windows::Security::DataProtection;

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

    py::pytype_handle UserDataAvailabilityStateChangedEventArgs_type{py::register_python_type(module.get(), &type_spec_UserDataAvailabilityStateChangedEventArgs, object_bases.get(), inspectable_meta_type)};
    if (!UserDataAvailabilityStateChangedEventArgs_type)
    {
        return nullptr;
    }

    py::pytype_handle UserDataBufferUnprotectResult_type{py::register_python_type(module.get(), &type_spec_UserDataBufferUnprotectResult, object_bases.get(), inspectable_meta_type)};
    if (!UserDataBufferUnprotectResult_type)
    {
        return nullptr;
    }

    py::pyobj_handle UserDataProtectionManager_Static_bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(inspectable_meta_type))};
    if (!UserDataProtectionManager_Static_bases)
    {
        return nullptr;
    }

    py::pyobj_handle type_UserDataProtectionManager_Static{PyType_FromSpecWithBases(&type_spec_UserDataProtectionManager_Static, UserDataProtectionManager_Static_bases.get())};
    if (!type_UserDataProtectionManager_Static)
    {
        return nullptr;
    }

    py::pytype_handle UserDataProtectionManager_type{py::register_python_type(module.get(), &type_spec_UserDataProtectionManager, object_bases.get(), reinterpret_cast<PyTypeObject*>(type_UserDataProtectionManager_Static.get()))};
    if (!UserDataProtectionManager_type)
    {
        return nullptr;
    }

    py::pytype_handle UserDataStorageItemProtectionInfo_type{py::register_python_type(module.get(), &type_spec_UserDataStorageItemProtectionInfo, object_bases.get(), inspectable_meta_type)};
    if (!UserDataStorageItemProtectionInfo_type)
    {
        return nullptr;
    }


    return module.detach();
}
