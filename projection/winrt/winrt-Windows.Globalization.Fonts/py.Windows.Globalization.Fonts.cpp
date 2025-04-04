// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.Globalization.Fonts.h"

namespace py::cpp::Windows::Globalization::Fonts
{
    // ----- LanguageFont class --------------------

    static PyObject* _new_LanguageFont(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Globalization::Fonts::LanguageFont>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Globalization::Fonts::LanguageFont>::type_name);
        return nullptr;
    }

    static void _dealloc_LanguageFont(py::wrapper::Windows::Globalization::Fonts::LanguageFont* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* LanguageFont_get_FontFamily(py::wrapper::Windows::Globalization::Fonts::LanguageFont* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFont", L"FontFamily");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.FontFamily();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFont_get_FontStretch(py::wrapper::Windows::Globalization::Fonts::LanguageFont* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFont", L"FontStretch");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.FontStretch();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFont_get_FontStyle(py::wrapper::Windows::Globalization::Fonts::LanguageFont* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFont", L"FontStyle");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.FontStyle();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFont_get_FontWeight(py::wrapper::Windows::Globalization::Fonts::LanguageFont* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFont", L"FontWeight");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.FontWeight();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFont_get_ScaleFactor(py::wrapper::Windows::Globalization::Fonts::LanguageFont* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFont", L"ScaleFactor");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ScaleFactor();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_LanguageFont(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Globalization::Fonts::LanguageFont>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_LanguageFont(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Globalization::Fonts::LanguageFont>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_LanguageFont[] = {
        { "_assign_array_", _assign_array_LanguageFont, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_LanguageFont), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_LanguageFont[] = {
        { "font_family", reinterpret_cast<getter>(LanguageFont_get_FontFamily), nullptr, nullptr, nullptr },
        { "font_stretch", reinterpret_cast<getter>(LanguageFont_get_FontStretch), nullptr, nullptr, nullptr },
        { "font_style", reinterpret_cast<getter>(LanguageFont_get_FontStyle), nullptr, nullptr, nullptr },
        { "font_weight", reinterpret_cast<getter>(LanguageFont_get_FontWeight), nullptr, nullptr, nullptr },
        { "scale_factor", reinterpret_cast<getter>(LanguageFont_get_ScaleFactor), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_LanguageFont[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_LanguageFont) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_LanguageFont) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_LanguageFont) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_LanguageFont) },
        { }};

    static PyType_Spec type_spec_LanguageFont = {
        "winrt._winrt_windows_globalization_fonts.LanguageFont",
        sizeof(py::wrapper::Windows::Globalization::Fonts::LanguageFont),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_LanguageFont};

    // ----- LanguageFontGroup class --------------------

    static PyObject* _new_LanguageFontGroup(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        if (kwds)
        {
            py::set_invalid_kwd_args_error();
            return nullptr;
        }

        auto arg_count = PyTuple_GET_SIZE(args);
        if (arg_count == 1)
        {
            try
            {
                auto param0 = py::convert_to<winrt::hstring>(args, 0);

                winrt::Windows::Globalization::Fonts::LanguageFontGroup instance{param0};
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

    static void _dealloc_LanguageFontGroup(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* LanguageFontGroup_get_DocumentAlternate1Font(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"DocumentAlternate1Font");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.DocumentAlternate1Font();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFontGroup_get_DocumentAlternate2Font(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"DocumentAlternate2Font");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.DocumentAlternate2Font();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFontGroup_get_DocumentHeadingFont(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"DocumentHeadingFont");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.DocumentHeadingFont();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFontGroup_get_FixedWidthTextFont(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"FixedWidthTextFont");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.FixedWidthTextFont();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFontGroup_get_ModernDocumentFont(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"ModernDocumentFont");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ModernDocumentFont();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFontGroup_get_TraditionalDocumentFont(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"TraditionalDocumentFont");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.TraditionalDocumentFont();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFontGroup_get_UICaptionFont(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"UICaptionFont");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.UICaptionFont();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFontGroup_get_UIHeadingFont(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"UIHeadingFont");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.UIHeadingFont();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFontGroup_get_UINotificationHeadingFont(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"UINotificationHeadingFont");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.UINotificationHeadingFont();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFontGroup_get_UITextFont(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"UITextFont");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.UITextFont();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* LanguageFontGroup_get_UITitleFont(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Globalization.Fonts.LanguageFontGroup", L"UITitleFont");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.UITitleFont();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_LanguageFontGroup(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Globalization::Fonts::LanguageFontGroup>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_LanguageFontGroup(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Globalization::Fonts::LanguageFontGroup>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_LanguageFontGroup[] = {
        { "_assign_array_", _assign_array_LanguageFontGroup, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_LanguageFontGroup), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_LanguageFontGroup[] = {
        { "document_alternate1_font", reinterpret_cast<getter>(LanguageFontGroup_get_DocumentAlternate1Font), nullptr, nullptr, nullptr },
        { "document_alternate2_font", reinterpret_cast<getter>(LanguageFontGroup_get_DocumentAlternate2Font), nullptr, nullptr, nullptr },
        { "document_heading_font", reinterpret_cast<getter>(LanguageFontGroup_get_DocumentHeadingFont), nullptr, nullptr, nullptr },
        { "fixed_width_text_font", reinterpret_cast<getter>(LanguageFontGroup_get_FixedWidthTextFont), nullptr, nullptr, nullptr },
        { "modern_document_font", reinterpret_cast<getter>(LanguageFontGroup_get_ModernDocumentFont), nullptr, nullptr, nullptr },
        { "traditional_document_font", reinterpret_cast<getter>(LanguageFontGroup_get_TraditionalDocumentFont), nullptr, nullptr, nullptr },
        { "ui_caption_font", reinterpret_cast<getter>(LanguageFontGroup_get_UICaptionFont), nullptr, nullptr, nullptr },
        { "ui_heading_font", reinterpret_cast<getter>(LanguageFontGroup_get_UIHeadingFont), nullptr, nullptr, nullptr },
        { "ui_notification_heading_font", reinterpret_cast<getter>(LanguageFontGroup_get_UINotificationHeadingFont), nullptr, nullptr, nullptr },
        { "ui_text_font", reinterpret_cast<getter>(LanguageFontGroup_get_UITextFont), nullptr, nullptr, nullptr },
        { "ui_title_font", reinterpret_cast<getter>(LanguageFontGroup_get_UITitleFont), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_LanguageFontGroup[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_LanguageFontGroup) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_LanguageFontGroup) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_LanguageFontGroup) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_LanguageFontGroup) },
        { }};

    static PyType_Spec type_spec_LanguageFontGroup = {
        "winrt._winrt_windows_globalization_fonts.LanguageFontGroup",
        sizeof(py::wrapper::Windows::Globalization::Fonts::LanguageFontGroup),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_LanguageFontGroup};

    // ----- Windows.Globalization.Fonts Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.Globalization.Fonts");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_globalization_fonts",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::Globalization::Fonts

PyMODINIT_FUNC PyInit__winrt_windows_globalization_fonts(void) noexcept
{
    using namespace py::cpp::Windows::Globalization::Fonts;

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

    py::pytype_handle LanguageFont_type{py::register_python_type(module.get(), &type_spec_LanguageFont, object_bases.get(), inspectable_meta_type)};
    if (!LanguageFont_type)
    {
        return nullptr;
    }

    py::pytype_handle LanguageFontGroup_type{py::register_python_type(module.get(), &type_spec_LanguageFontGroup, object_bases.get(), inspectable_meta_type)};
    if (!LanguageFontGroup_type)
    {
        return nullptr;
    }


    return module.detach();
}
