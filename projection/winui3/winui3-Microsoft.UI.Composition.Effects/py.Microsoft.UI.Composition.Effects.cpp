// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Microsoft.UI.Composition.Effects.h"

namespace py::cpp::Microsoft::UI::Composition::Effects
{
    // ----- SceneLightingEffect class --------------------

    static PyObject* _new_SceneLightingEffect(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
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
                winrt::Microsoft::UI::Composition::Effects::SceneLightingEffect instance{};
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

    static void _dealloc_SceneLightingEffect(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* SceneLightingEffect_get_SpecularShine(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"SpecularShine");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.SpecularShine();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int SceneLightingEffect_put_SpecularShine(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"SpecularShine");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<float>(arg);

            {
                auto _gil = release_gil();
                self->obj.SpecularShine(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* SceneLightingEffect_get_SpecularAmount(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"SpecularAmount");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.SpecularAmount();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int SceneLightingEffect_put_SpecularAmount(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"SpecularAmount");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<float>(arg);

            {
                auto _gil = release_gil();
                self->obj.SpecularAmount(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* SceneLightingEffect_get_NormalMapSource(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"NormalMapSource");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.NormalMapSource();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int SceneLightingEffect_put_NormalMapSource(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"NormalMapSource");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Windows::Graphics::Effects::IGraphicsEffectSource>(arg);

            {
                auto _gil = release_gil();
                self->obj.NormalMapSource(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* SceneLightingEffect_get_DiffuseAmount(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"DiffuseAmount");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.DiffuseAmount();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int SceneLightingEffect_put_DiffuseAmount(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"DiffuseAmount");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<float>(arg);

            {
                auto _gil = release_gil();
                self->obj.DiffuseAmount(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* SceneLightingEffect_get_AmbientAmount(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"AmbientAmount");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.AmbientAmount();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int SceneLightingEffect_put_AmbientAmount(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"AmbientAmount");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<float>(arg);

            {
                auto _gil = release_gil();
                self->obj.AmbientAmount(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* SceneLightingEffect_get_ReflectanceModel(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"ReflectanceModel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ReflectanceModel();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int SceneLightingEffect_put_ReflectanceModel(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"ReflectanceModel");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Microsoft::UI::Composition::Effects::SceneLightingEffectReflectanceModel>(arg);

            {
                auto _gil = release_gil();
                self->obj.ReflectanceModel(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* SceneLightingEffect_get_Name(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"Name");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Name();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int SceneLightingEffect_put_Name(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Composition.Effects.SceneLightingEffect", L"Name");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::hstring>(arg);

            {
                auto _gil = release_gil();
                self->obj.Name(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* _assign_array_SceneLightingEffect(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Microsoft::UI::Composition::Effects::SceneLightingEffect>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_SceneLightingEffect(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Microsoft::UI::Composition::Effects::SceneLightingEffect>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_SceneLightingEffect[] = {
        { "_assign_array_", _assign_array_SceneLightingEffect, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_SceneLightingEffect), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_SceneLightingEffect[] = {
        { "specular_shine", reinterpret_cast<getter>(SceneLightingEffect_get_SpecularShine), reinterpret_cast<setter>(SceneLightingEffect_put_SpecularShine), nullptr, nullptr },
        { "specular_amount", reinterpret_cast<getter>(SceneLightingEffect_get_SpecularAmount), reinterpret_cast<setter>(SceneLightingEffect_put_SpecularAmount), nullptr, nullptr },
        { "normal_map_source", reinterpret_cast<getter>(SceneLightingEffect_get_NormalMapSource), reinterpret_cast<setter>(SceneLightingEffect_put_NormalMapSource), nullptr, nullptr },
        { "diffuse_amount", reinterpret_cast<getter>(SceneLightingEffect_get_DiffuseAmount), reinterpret_cast<setter>(SceneLightingEffect_put_DiffuseAmount), nullptr, nullptr },
        { "ambient_amount", reinterpret_cast<getter>(SceneLightingEffect_get_AmbientAmount), reinterpret_cast<setter>(SceneLightingEffect_put_AmbientAmount), nullptr, nullptr },
        { "reflectance_model", reinterpret_cast<getter>(SceneLightingEffect_get_ReflectanceModel), reinterpret_cast<setter>(SceneLightingEffect_put_ReflectanceModel), nullptr, nullptr },
        { "name", reinterpret_cast<getter>(SceneLightingEffect_get_Name), reinterpret_cast<setter>(SceneLightingEffect_put_Name), nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_SceneLightingEffect[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_SceneLightingEffect) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_SceneLightingEffect) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_SceneLightingEffect) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_SceneLightingEffect) },
        { }};

    static PyType_Spec type_spec_SceneLightingEffect = {
        "winui3._winui3_microsoft_ui_composition_effects.SceneLightingEffect",
        sizeof(py::wrapper::Microsoft::UI::Composition::Effects::SceneLightingEffect),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_SceneLightingEffect};

    // ----- Microsoft.UI.Composition.Effects Initialization --------------------

    PyDoc_STRVAR(module_doc, "Microsoft.UI.Composition.Effects");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winui3_microsoft_ui_composition_effects",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Microsoft::UI::Composition::Effects

PyMODINIT_FUNC PyInit__winui3_microsoft_ui_composition_effects(void) noexcept
{
    using namespace py::cpp::Microsoft::UI::Composition::Effects;

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

    py::pytype_handle SceneLightingEffect_type{py::register_python_type(module.get(), &type_spec_SceneLightingEffect, object_bases.get(), inspectable_meta_type)};
    if (!SceneLightingEffect_type)
    {
        return nullptr;
    }


    return module.detach();
}
