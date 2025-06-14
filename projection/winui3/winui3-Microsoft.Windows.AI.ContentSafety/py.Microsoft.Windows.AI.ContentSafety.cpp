// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Microsoft.Windows.AI.ContentSafety.h"

namespace py::cpp::Microsoft::Windows::AI::ContentSafety
{
    // ----- ContentFilterOptions class --------------------

    static PyObject* _new_ContentFilterOptions(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
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
                winrt::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions instance{};
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

    static void _dealloc_ContentFilterOptions(py::wrapper::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* ContentFilterOptions_get_ResponseMaxAllowedSeverityLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ContentFilterOptions", L"ResponseMaxAllowedSeverityLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ResponseMaxAllowedSeverityLevel();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ContentFilterOptions_put_ResponseMaxAllowedSeverityLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ContentFilterOptions", L"ResponseMaxAllowedSeverityLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity>(arg);

            {
                auto _gil = release_gil();
                self->obj.ResponseMaxAllowedSeverityLevel(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* ContentFilterOptions_get_PromptMaxAllowedSeverityLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ContentFilterOptions", L"PromptMaxAllowedSeverityLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.PromptMaxAllowedSeverityLevel();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ContentFilterOptions_put_PromptMaxAllowedSeverityLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ContentFilterOptions", L"PromptMaxAllowedSeverityLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity>(arg);

            {
                auto _gil = release_gil();
                self->obj.PromptMaxAllowedSeverityLevel(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* ContentFilterOptions_get_ImageMaxAllowedSeverityLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ContentFilterOptions", L"ImageMaxAllowedSeverityLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ImageMaxAllowedSeverityLevel();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ContentFilterOptions_put_ImageMaxAllowedSeverityLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ContentFilterOptions", L"ImageMaxAllowedSeverityLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity>(arg);

            {
                auto _gil = release_gil();
                self->obj.ImageMaxAllowedSeverityLevel(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* _assign_array_ContentFilterOptions(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_ContentFilterOptions(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_ContentFilterOptions[] = {
        { "_assign_array_", _assign_array_ContentFilterOptions, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_ContentFilterOptions), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_ContentFilterOptions[] = {
        { "response_max_allowed_severity_level", reinterpret_cast<getter>(ContentFilterOptions_get_ResponseMaxAllowedSeverityLevel), reinterpret_cast<setter>(ContentFilterOptions_put_ResponseMaxAllowedSeverityLevel), nullptr, nullptr },
        { "prompt_max_allowed_severity_level", reinterpret_cast<getter>(ContentFilterOptions_get_PromptMaxAllowedSeverityLevel), reinterpret_cast<setter>(ContentFilterOptions_put_PromptMaxAllowedSeverityLevel), nullptr, nullptr },
        { "image_max_allowed_severity_level", reinterpret_cast<getter>(ContentFilterOptions_get_ImageMaxAllowedSeverityLevel), reinterpret_cast<setter>(ContentFilterOptions_put_ImageMaxAllowedSeverityLevel), nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_ContentFilterOptions[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_ContentFilterOptions) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_ContentFilterOptions) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_ContentFilterOptions) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_ContentFilterOptions) },
        { }};

    static PyType_Spec type_spec_ContentFilterOptions = {
        "winui3._winui3_microsoft_windows_ai_contentsafety.ContentFilterOptions",
        sizeof(py::wrapper::Microsoft::Windows::AI::ContentSafety::ContentFilterOptions),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_ContentFilterOptions};

    // ----- ImageContentFilterSeverity class --------------------

    static PyObject* _new_ImageContentFilterSeverity(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
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
                auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::SeverityLevel>(args, 0);

                winrt::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity instance{param0};
                return py::wrap(instance, type);
            }
            catch (...)
            {
                py::to_PyErr();
                return nullptr;
            }
        }
        else if (arg_count == 0)
        {
            try
            {
                winrt::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity instance{};
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

    static void _dealloc_ImageContentFilterSeverity(py::wrapper::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* ImageContentFilterSeverity_get_ViolentContentLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ImageContentFilterSeverity", L"ViolentContentLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ViolentContentLevel();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ImageContentFilterSeverity_put_ViolentContentLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ImageContentFilterSeverity", L"ViolentContentLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::SeverityLevel>(arg);

            {
                auto _gil = release_gil();
                self->obj.ViolentContentLevel(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* ImageContentFilterSeverity_get_RacyContentLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ImageContentFilterSeverity", L"RacyContentLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.RacyContentLevel();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ImageContentFilterSeverity_put_RacyContentLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ImageContentFilterSeverity", L"RacyContentLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::SeverityLevel>(arg);

            {
                auto _gil = release_gil();
                self->obj.RacyContentLevel(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* ImageContentFilterSeverity_get_GoryContentLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ImageContentFilterSeverity", L"GoryContentLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.GoryContentLevel();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ImageContentFilterSeverity_put_GoryContentLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ImageContentFilterSeverity", L"GoryContentLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::SeverityLevel>(arg);

            {
                auto _gil = release_gil();
                self->obj.GoryContentLevel(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* ImageContentFilterSeverity_get_AdultContentLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ImageContentFilterSeverity", L"AdultContentLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.AdultContentLevel();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int ImageContentFilterSeverity_put_AdultContentLevel(py::wrapper::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.ImageContentFilterSeverity", L"AdultContentLevel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::SeverityLevel>(arg);

            {
                auto _gil = release_gil();
                self->obj.AdultContentLevel(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* _assign_array_ImageContentFilterSeverity(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_ImageContentFilterSeverity(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_ImageContentFilterSeverity[] = {
        { "_assign_array_", _assign_array_ImageContentFilterSeverity, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_ImageContentFilterSeverity), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_ImageContentFilterSeverity[] = {
        { "violent_content_level", reinterpret_cast<getter>(ImageContentFilterSeverity_get_ViolentContentLevel), reinterpret_cast<setter>(ImageContentFilterSeverity_put_ViolentContentLevel), nullptr, nullptr },
        { "racy_content_level", reinterpret_cast<getter>(ImageContentFilterSeverity_get_RacyContentLevel), reinterpret_cast<setter>(ImageContentFilterSeverity_put_RacyContentLevel), nullptr, nullptr },
        { "gory_content_level", reinterpret_cast<getter>(ImageContentFilterSeverity_get_GoryContentLevel), reinterpret_cast<setter>(ImageContentFilterSeverity_put_GoryContentLevel), nullptr, nullptr },
        { "adult_content_level", reinterpret_cast<getter>(ImageContentFilterSeverity_get_AdultContentLevel), reinterpret_cast<setter>(ImageContentFilterSeverity_put_AdultContentLevel), nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_ImageContentFilterSeverity[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_ImageContentFilterSeverity) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_ImageContentFilterSeverity) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_ImageContentFilterSeverity) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_ImageContentFilterSeverity) },
        { }};

    static PyType_Spec type_spec_ImageContentFilterSeverity = {
        "winui3._winui3_microsoft_windows_ai_contentsafety.ImageContentFilterSeverity",
        sizeof(py::wrapper::Microsoft::Windows::AI::ContentSafety::ImageContentFilterSeverity),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_ImageContentFilterSeverity};

    // ----- TextContentFilterSeverity class --------------------

    static PyObject* _new_TextContentFilterSeverity(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
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
                auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::SeverityLevel>(args, 0);

                winrt::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity instance{param0};
                return py::wrap(instance, type);
            }
            catch (...)
            {
                py::to_PyErr();
                return nullptr;
            }
        }
        else if (arg_count == 0)
        {
            try
            {
                winrt::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity instance{};
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

    static void _dealloc_TextContentFilterSeverity(py::wrapper::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* TextContentFilterSeverity_get_Violent(py::wrapper::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.TextContentFilterSeverity", L"Violent");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Violent();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int TextContentFilterSeverity_put_Violent(py::wrapper::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.TextContentFilterSeverity", L"Violent");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::SeverityLevel>(arg);

            {
                auto _gil = release_gil();
                self->obj.Violent(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* TextContentFilterSeverity_get_Sexual(py::wrapper::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.TextContentFilterSeverity", L"Sexual");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Sexual();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int TextContentFilterSeverity_put_Sexual(py::wrapper::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.TextContentFilterSeverity", L"Sexual");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::SeverityLevel>(arg);

            {
                auto _gil = release_gil();
                self->obj.Sexual(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* TextContentFilterSeverity_get_SelfHarm(py::wrapper::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.TextContentFilterSeverity", L"SelfHarm");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.SelfHarm();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int TextContentFilterSeverity_put_SelfHarm(py::wrapper::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.TextContentFilterSeverity", L"SelfHarm");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::SeverityLevel>(arg);

            {
                auto _gil = release_gil();
                self->obj.SelfHarm(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* TextContentFilterSeverity_get_Hate(py::wrapper::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.TextContentFilterSeverity", L"Hate");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Hate();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int TextContentFilterSeverity_put_Hate(py::wrapper::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Windows.AI.ContentSafety.TextContentFilterSeverity", L"Hate");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::Windows::AI::ContentSafety::SeverityLevel>(arg);

            {
                auto _gil = release_gil();
                self->obj.Hate(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* _assign_array_TextContentFilterSeverity(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_TextContentFilterSeverity(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_TextContentFilterSeverity[] = {
        { "_assign_array_", _assign_array_TextContentFilterSeverity, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_TextContentFilterSeverity), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_TextContentFilterSeverity[] = {
        { "violent", reinterpret_cast<getter>(TextContentFilterSeverity_get_Violent), reinterpret_cast<setter>(TextContentFilterSeverity_put_Violent), nullptr, nullptr },
        { "sexual", reinterpret_cast<getter>(TextContentFilterSeverity_get_Sexual), reinterpret_cast<setter>(TextContentFilterSeverity_put_Sexual), nullptr, nullptr },
        { "self_harm", reinterpret_cast<getter>(TextContentFilterSeverity_get_SelfHarm), reinterpret_cast<setter>(TextContentFilterSeverity_put_SelfHarm), nullptr, nullptr },
        { "hate", reinterpret_cast<getter>(TextContentFilterSeverity_get_Hate), reinterpret_cast<setter>(TextContentFilterSeverity_put_Hate), nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_TextContentFilterSeverity[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_TextContentFilterSeverity) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_TextContentFilterSeverity) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_TextContentFilterSeverity) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_TextContentFilterSeverity) },
        { }};

    static PyType_Spec type_spec_TextContentFilterSeverity = {
        "winui3._winui3_microsoft_windows_ai_contentsafety.TextContentFilterSeverity",
        sizeof(py::wrapper::Microsoft::Windows::AI::ContentSafety::TextContentFilterSeverity),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_TextContentFilterSeverity};

    // ----- Microsoft.Windows.AI.ContentSafety Initialization --------------------

    PyDoc_STRVAR(module_doc, "Microsoft.Windows.AI.ContentSafety");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winui3_microsoft_windows_ai_contentsafety",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Microsoft::Windows::AI::ContentSafety

PyMODINIT_FUNC PyInit__winui3_microsoft_windows_ai_contentsafety(void) noexcept
{
    using namespace py::cpp::Microsoft::Windows::AI::ContentSafety;

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

    py::pytype_handle ContentFilterOptions_type{py::register_python_type(module.get(), &type_spec_ContentFilterOptions, object_bases.get(), inspectable_meta_type)};
    if (!ContentFilterOptions_type)
    {
        return nullptr;
    }

    py::pytype_handle ImageContentFilterSeverity_type{py::register_python_type(module.get(), &type_spec_ImageContentFilterSeverity, object_bases.get(), inspectable_meta_type)};
    if (!ImageContentFilterSeverity_type)
    {
        return nullptr;
    }

    py::pytype_handle TextContentFilterSeverity_type{py::register_python_type(module.get(), &type_spec_TextContentFilterSeverity, object_bases.get(), inspectable_meta_type)};
    if (!TextContentFilterSeverity_type)
    {
        return nullptr;
    }


    return module.detach();
}
