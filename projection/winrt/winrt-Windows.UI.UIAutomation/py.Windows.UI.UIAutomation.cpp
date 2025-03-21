// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.UI.UIAutomation.h"

namespace py::cpp::Windows::UI::UIAutomation
{
    // ----- AutomationConnection class --------------------

    static PyObject* _new_AutomationConnection(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::UI::UIAutomation::AutomationConnection>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::UI::UIAutomation::AutomationConnection>::type_name);
        return nullptr;
    }

    static void _dealloc_AutomationConnection(py::wrapper::Windows::UI::UIAutomation::AutomationConnection* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* AutomationConnection_get_AppUserModelId(py::wrapper::Windows::UI::UIAutomation::AutomationConnection* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.UI.UIAutomation.AutomationConnection", L"AppUserModelId");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.AppUserModelId();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* AutomationConnection_get_ExecutableFileName(py::wrapper::Windows::UI::UIAutomation::AutomationConnection* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.UI.UIAutomation.AutomationConnection", L"ExecutableFileName");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ExecutableFileName();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* AutomationConnection_get_IsRemoteSystem(py::wrapper::Windows::UI::UIAutomation::AutomationConnection* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.UI.UIAutomation.AutomationConnection", L"IsRemoteSystem");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.IsRemoteSystem();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_AutomationConnection(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::UI::UIAutomation::AutomationConnection>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_AutomationConnection(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::UI::UIAutomation::AutomationConnection>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_AutomationConnection[] = {
        { "_assign_array_", _assign_array_AutomationConnection, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_AutomationConnection), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_AutomationConnection[] = {
        { "app_user_model_id", reinterpret_cast<getter>(AutomationConnection_get_AppUserModelId), nullptr, nullptr, nullptr },
        { "executable_file_name", reinterpret_cast<getter>(AutomationConnection_get_ExecutableFileName), nullptr, nullptr, nullptr },
        { "is_remote_system", reinterpret_cast<getter>(AutomationConnection_get_IsRemoteSystem), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_AutomationConnection[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_AutomationConnection) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_AutomationConnection) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_AutomationConnection) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_AutomationConnection) },
        { }};

    static PyType_Spec type_spec_AutomationConnection = {
        "winrt._winrt_windows_ui_uiautomation.AutomationConnection",
        sizeof(py::wrapper::Windows::UI::UIAutomation::AutomationConnection),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_AutomationConnection};

    // ----- AutomationConnectionBoundObject class --------------------

    static PyObject* _new_AutomationConnectionBoundObject(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::UI::UIAutomation::AutomationConnectionBoundObject>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::UI::UIAutomation::AutomationConnectionBoundObject>::type_name);
        return nullptr;
    }

    static void _dealloc_AutomationConnectionBoundObject(py::wrapper::Windows::UI::UIAutomation::AutomationConnectionBoundObject* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* AutomationConnectionBoundObject_get_Connection(py::wrapper::Windows::UI::UIAutomation::AutomationConnectionBoundObject* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.UI.UIAutomation.AutomationConnectionBoundObject", L"Connection");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Connection();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_AutomationConnectionBoundObject(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::UI::UIAutomation::AutomationConnectionBoundObject>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_AutomationConnectionBoundObject(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::UI::UIAutomation::AutomationConnectionBoundObject>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_AutomationConnectionBoundObject[] = {
        { "_assign_array_", _assign_array_AutomationConnectionBoundObject, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_AutomationConnectionBoundObject), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_AutomationConnectionBoundObject[] = {
        { "connection", reinterpret_cast<getter>(AutomationConnectionBoundObject_get_Connection), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_AutomationConnectionBoundObject[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_AutomationConnectionBoundObject) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_AutomationConnectionBoundObject) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_AutomationConnectionBoundObject) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_AutomationConnectionBoundObject) },
        { }};

    static PyType_Spec type_spec_AutomationConnectionBoundObject = {
        "winrt._winrt_windows_ui_uiautomation.AutomationConnectionBoundObject",
        sizeof(py::wrapper::Windows::UI::UIAutomation::AutomationConnectionBoundObject),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_AutomationConnectionBoundObject};

    // ----- AutomationElement class --------------------

    static PyObject* _new_AutomationElement(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::UI::UIAutomation::AutomationElement>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::UI::UIAutomation::AutomationElement>::type_name);
        return nullptr;
    }

    static void _dealloc_AutomationElement(py::wrapper::Windows::UI::UIAutomation::AutomationElement* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* AutomationElement_get_AppUserModelId(py::wrapper::Windows::UI::UIAutomation::AutomationElement* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.UI.UIAutomation.AutomationElement", L"AppUserModelId");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.AppUserModelId();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* AutomationElement_get_ExecutableFileName(py::wrapper::Windows::UI::UIAutomation::AutomationElement* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.UI.UIAutomation.AutomationElement", L"ExecutableFileName");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ExecutableFileName();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* AutomationElement_get_IsRemoteSystem(py::wrapper::Windows::UI::UIAutomation::AutomationElement* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.UI.UIAutomation.AutomationElement", L"IsRemoteSystem");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.IsRemoteSystem();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_AutomationElement(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::UI::UIAutomation::AutomationElement>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_AutomationElement(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::UI::UIAutomation::AutomationElement>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_AutomationElement[] = {
        { "_assign_array_", _assign_array_AutomationElement, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_AutomationElement), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_AutomationElement[] = {
        { "app_user_model_id", reinterpret_cast<getter>(AutomationElement_get_AppUserModelId), nullptr, nullptr, nullptr },
        { "executable_file_name", reinterpret_cast<getter>(AutomationElement_get_ExecutableFileName), nullptr, nullptr, nullptr },
        { "is_remote_system", reinterpret_cast<getter>(AutomationElement_get_IsRemoteSystem), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_AutomationElement[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_AutomationElement) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_AutomationElement) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_AutomationElement) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_AutomationElement) },
        { }};

    static PyType_Spec type_spec_AutomationElement = {
        "winrt._winrt_windows_ui_uiautomation.AutomationElement",
        sizeof(py::wrapper::Windows::UI::UIAutomation::AutomationElement),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_AutomationElement};

    // ----- AutomationTextRange class --------------------

    static PyObject* _new_AutomationTextRange(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::UI::UIAutomation::AutomationTextRange>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::UI::UIAutomation::AutomationTextRange>::type_name);
        return nullptr;
    }

    static void _dealloc_AutomationTextRange(py::wrapper::Windows::UI::UIAutomation::AutomationTextRange* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* _assign_array_AutomationTextRange(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::UI::UIAutomation::AutomationTextRange>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_AutomationTextRange(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::UI::UIAutomation::AutomationTextRange>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_AutomationTextRange[] = {
        { "_assign_array_", _assign_array_AutomationTextRange, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_AutomationTextRange), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_AutomationTextRange[] = {
        { }};

    static PyType_Slot _type_slots_AutomationTextRange[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_AutomationTextRange) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_AutomationTextRange) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_AutomationTextRange) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_AutomationTextRange) },
        { }};

    static PyType_Spec type_spec_AutomationTextRange = {
        "winrt._winrt_windows_ui_uiautomation.AutomationTextRange",
        sizeof(py::wrapper::Windows::UI::UIAutomation::AutomationTextRange),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_AutomationTextRange};

    // ----- Windows.UI.UIAutomation Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.UI.UIAutomation");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_ui_uiautomation",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::UI::UIAutomation

PyMODINIT_FUNC PyInit__winrt_windows_ui_uiautomation(void) noexcept
{
    using namespace py::cpp::Windows::UI::UIAutomation;

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

    py::pytype_handle AutomationConnection_type{py::register_python_type(module.get(), &type_spec_AutomationConnection, object_bases.get(), inspectable_meta_type)};
    if (!AutomationConnection_type)
    {
        return nullptr;
    }

    py::pytype_handle AutomationConnectionBoundObject_type{py::register_python_type(module.get(), &type_spec_AutomationConnectionBoundObject, object_bases.get(), inspectable_meta_type)};
    if (!AutomationConnectionBoundObject_type)
    {
        return nullptr;
    }

    py::pytype_handle AutomationElement_type{py::register_python_type(module.get(), &type_spec_AutomationElement, object_bases.get(), inspectable_meta_type)};
    if (!AutomationElement_type)
    {
        return nullptr;
    }

    py::pytype_handle AutomationTextRange_type{py::register_python_type(module.get(), &type_spec_AutomationTextRange, object_bases.get(), inspectable_meta_type)};
    if (!AutomationTextRange_type)
    {
        return nullptr;
    }


    return module.detach();
}
