// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Microsoft.UI.Xaml.Resources.h"


namespace py::cpp::Microsoft::UI::Xaml::Resources
{
    // ----- CustomXamlResourceLoader class --------------------

    static PyObject* _new_CustomXamlResourceLoader(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        if (kwds != nullptr)
        {
            py::set_invalid_kwd_args_error();
            return nullptr;
        }

        auto arg_count = PyTuple_Size(args);
        if (arg_count == 0)
        {
            try
            {
                winrt::Microsoft::UI::Xaml::Resources::CustomXamlResourceLoader instance{  };
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

    static void _dealloc_CustomXamlResourceLoader(py::wrapper::Microsoft::UI::Xaml::Resources::CustomXamlResourceLoader* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* CustomXamlResourceLoader_GetResource(py::wrapper::Microsoft::UI::Xaml::Resources::CustomXamlResourceLoader* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 4)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Microsoft.UI.Xaml.Resources.CustomXamlResourceLoader", L"GetResource", 4))
            {
                py::set_arg_count_version_error(4);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::hstring>(args, 0);
                auto param1 = py::convert_to<winrt::hstring>(args, 1);
                auto param2 = py::convert_to<winrt::hstring>(args, 2);
                auto param3 = py::convert_to<winrt::hstring>(args, 3);

                return py::convert(self->obj.GetResource(param0, param1, param2, param3));
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

    static PyObject* CustomXamlResourceLoader_get_Current(PyObject* /*unused*/, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Xaml.Resources.CustomXamlResourceLoader", L"Current"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(winrt::Microsoft::UI::Xaml::Resources::CustomXamlResourceLoader::Current());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int CustomXamlResourceLoader_put_Current(PyObject* /*unused*/, PyObject* arg, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.UI.Xaml.Resources.CustomXamlResourceLoader", L"Current"))
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
            auto param0 = py::convert_to<winrt::Microsoft::UI::Xaml::Resources::CustomXamlResourceLoader>(arg);

            winrt::Microsoft::UI::Xaml::Resources::CustomXamlResourceLoader::Current(param0);
            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* _assign_array_CustomXamlResourceLoader(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Microsoft::UI::Xaml::Resources::CustomXamlResourceLoader>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_CustomXamlResourceLoader(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Microsoft::UI::Xaml::Resources::CustomXamlResourceLoader>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_CustomXamlResourceLoader[] = {
        { "get_resource", reinterpret_cast<PyCFunction>(CustomXamlResourceLoader_GetResource), METH_VARARGS, nullptr },
        { "_assign_array_", _assign_array_CustomXamlResourceLoader, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_CustomXamlResourceLoader), METH_O | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_CustomXamlResourceLoader[] = {
        { }
    };

    static PyType_Slot _type_slots_CustomXamlResourceLoader[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_CustomXamlResourceLoader) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_CustomXamlResourceLoader) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_CustomXamlResourceLoader) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_CustomXamlResourceLoader) },
        { },
    };

    static PyType_Spec type_spec_CustomXamlResourceLoader =
    {
        "winrt._winrt_microsoft_ui_xaml_resources.CustomXamlResourceLoader",
        sizeof(py::wrapper::Microsoft::UI::Xaml::Resources::CustomXamlResourceLoader),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_CustomXamlResourceLoader
    };

    static PyGetSetDef getset_CustomXamlResourceLoader_Static[] = {
        { "current", reinterpret_cast<getter>(CustomXamlResourceLoader_get_Current), reinterpret_cast<setter>(CustomXamlResourceLoader_put_Current), nullptr, nullptr },
        { }
    };

    static PyMethodDef methods_CustomXamlResourceLoader_Static[] = {
        { }
    };

    static PyType_Slot type_slots_CustomXamlResourceLoader_Static[] = 
    {
        { Py_tp_base, reinterpret_cast<void*>(&PyType_Type) },
        { Py_tp_getset, reinterpret_cast<void*>(getset_CustomXamlResourceLoader_Static) },
        { Py_tp_methods, reinterpret_cast<void*>(methods_CustomXamlResourceLoader_Static) },
        { }
    };

    static PyType_Spec type_spec_CustomXamlResourceLoader_Static =
    {
        "winrt._winrt_microsoft_ui_xaml_resources.CustomXamlResourceLoader_Static",
        static_cast<int>(PyType_Type.tp_basicsize),
        static_cast<int>(PyType_Type.tp_itemsize),
        Py_TPFLAGS_DEFAULT,
        type_slots_CustomXamlResourceLoader_Static
    };

    // ----- Microsoft.UI.Xaml.Resources Initialization --------------------
    PyDoc_STRVAR(module_doc, "Microsoft::UI::Xaml::Resources");


    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_microsoft_ui_xaml_resources",
           module_doc,
           0,
           nullptr,
           nullptr,
           nullptr,
           nullptr,
           nullptr};

} // py::cpp::Microsoft::UI::Xaml::Resources

PyMODINIT_FUNC PyInit__winrt_microsoft_ui_xaml_resources(void) noexcept
{
    using namespace py::cpp::Microsoft::UI::Xaml::Resources;

    if (py::import_winrt_runtime() == -1)
    {
        return nullptr;
    }

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

    py::pyobj_handle type_CustomXamlResourceLoader_Static{PyType_FromSpec(&type_spec_CustomXamlResourceLoader_Static)};
    if (!type_CustomXamlResourceLoader_Static)
    {
        return nullptr;
    }

    if (py::register_python_type(module.get(), &type_spec_CustomXamlResourceLoader, object_bases.get(), reinterpret_cast<PyTypeObject*>(type_CustomXamlResourceLoader_Static.get())) == -1)
    {
        return nullptr;
    }


    return module.detach();
}