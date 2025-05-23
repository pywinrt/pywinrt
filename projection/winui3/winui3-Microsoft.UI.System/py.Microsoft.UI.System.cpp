// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Microsoft.UI.System.h"

namespace py::cpp::Microsoft::UI::System
{
    // ----- ThemeSettings class --------------------

    static PyObject* _new_ThemeSettings(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Microsoft::UI::System::ThemeSettings>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Microsoft::UI::System::ThemeSettings>::type_name);
        return nullptr;
    }

    static void _dealloc_ThemeSettings(py::wrapper::Microsoft::UI::System::ThemeSettings* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* ThemeSettings_CreateForWindowId(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Microsoft.UI.System.ThemeSettings", L"CreateForWindowId", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Microsoft::UI::WindowId>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Microsoft::UI::System::ThemeSettings::CreateForWindowId(param0);
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

    static PyObject* ThemeSettings_get_HighContrast(py::wrapper::Microsoft::UI::System::ThemeSettings* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.System.ThemeSettings", L"HighContrast");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.HighContrast();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ThemeSettings_get_HighContrastScheme(py::wrapper::Microsoft::UI::System::ThemeSettings* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.System.ThemeSettings", L"HighContrastScheme");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.HighContrastScheme();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ThemeSettings_add_Changed(py::wrapper::Microsoft::UI::System::ThemeSettings* self, PyObject* arg) noexcept
    {
        try
        {
            static std::optional<bool> is_event_present{};

            if (!is_event_present.has_value())
            {
                is_event_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Microsoft.UI.System.ThemeSettings", L"Changed");
            }

            if (!is_event_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
                return nullptr;
            }

            auto param0 = py::convert_to<winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::System::ThemeSettings, winrt::Windows::Foundation::IInspectable>>(arg);

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Changed(param0);
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ThemeSettings_remove_Changed(py::wrapper::Microsoft::UI::System::ThemeSettings* self, PyObject* arg) noexcept
    {
        try
        {
            static std::optional<bool> is_event_present{};

            if (!is_event_present.has_value())
            {
                is_event_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Microsoft.UI.System.ThemeSettings", L"Changed");
            }

            if (!is_event_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
                return nullptr;
            }

            auto param0 = py::convert_to<winrt::event_token>(arg);

            {
                auto _gil = release_gil();
                self->obj.Changed(param0);
            }

            Py_RETURN_NONE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_ThemeSettings(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Microsoft::UI::System::ThemeSettings>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_ThemeSettings(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Microsoft::UI::System::ThemeSettings>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_ThemeSettings[] = {
        { "add_changed", reinterpret_cast<PyCFunction>(ThemeSettings_add_Changed), METH_O, nullptr },
        { "remove_changed", reinterpret_cast<PyCFunction>(ThemeSettings_remove_Changed), METH_O, nullptr },
        { "_assign_array_", _assign_array_ThemeSettings, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_ThemeSettings), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_ThemeSettings[] = {
        { "high_contrast", reinterpret_cast<getter>(ThemeSettings_get_HighContrast), nullptr, nullptr, nullptr },
        { "high_contrast_scheme", reinterpret_cast<getter>(ThemeSettings_get_HighContrastScheme), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_ThemeSettings[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_ThemeSettings) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_ThemeSettings) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_ThemeSettings) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_ThemeSettings) },
        { }};

    static PyType_Spec type_spec_ThemeSettings = {
        "winui3._winui3_microsoft_ui_system.ThemeSettings",
        sizeof(py::wrapper::Microsoft::UI::System::ThemeSettings),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_ThemeSettings};

    static PyGetSetDef getset_ThemeSettings_Static[] = {
        { }};

    static PyMethodDef methods_ThemeSettings_Static[] = {
        { "create_for_window_id", reinterpret_cast<PyCFunction>(ThemeSettings_CreateForWindowId), METH_VARARGS, nullptr },
        { }};

    static PyType_Slot type_slots_ThemeSettings_Static[] = 
    {
        { Py_tp_base, reinterpret_cast<void*>(&PyType_Type) },
        { Py_tp_getset, reinterpret_cast<void*>(getset_ThemeSettings_Static) },
        { Py_tp_methods, reinterpret_cast<void*>(methods_ThemeSettings_Static) },
        { }
    };

    static PyType_Spec type_spec_ThemeSettings_Static = {
        "winui3._winui3_microsoft_ui_system.ThemeSettings_Static",
        static_cast<int>(PyType_Type.tp_basicsize),
        static_cast<int>(PyType_Type.tp_itemsize),
        Py_TPFLAGS_DEFAULT,
        type_slots_ThemeSettings_Static};

    // ----- Microsoft.UI.System Initialization --------------------

    PyDoc_STRVAR(module_doc, "Microsoft.UI.System");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winui3_microsoft_ui_system",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Microsoft::UI::System

PyMODINIT_FUNC PyInit__winui3_microsoft_ui_system(void) noexcept
{
    using namespace py::cpp::Microsoft::UI::System;

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

    py::pyobj_handle ThemeSettings_Static_bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(inspectable_meta_type))};
    if (!ThemeSettings_Static_bases)
    {
        return nullptr;
    }

    py::pyobj_handle type_ThemeSettings_Static{PyType_FromSpecWithBases(&type_spec_ThemeSettings_Static, ThemeSettings_Static_bases.get())};
    if (!type_ThemeSettings_Static)
    {
        return nullptr;
    }

    py::pytype_handle ThemeSettings_type{py::register_python_type(module.get(), &type_spec_ThemeSettings, object_bases.get(), reinterpret_cast<PyTypeObject*>(type_ThemeSettings_Static.get()))};
    if (!ThemeSettings_type)
    {
        return nullptr;
    }


    return module.detach();
}
