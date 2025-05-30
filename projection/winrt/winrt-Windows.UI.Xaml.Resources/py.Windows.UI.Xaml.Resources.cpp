// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.UI.Xaml.Resources.h"

namespace py::cpp::Windows::UI::Xaml::Resources
{
    // ----- CustomXamlResourceLoader class --------------------

    struct PyWinrtCustomXamlResourceLoader;
    using BasePyWinrtCustomXamlResourceLoader = winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoaderT<PyWinrtCustomXamlResourceLoader, py::IPywinrtObject>;

    struct PyWinrtCustomXamlResourceLoader : py::py_obj_ref, BasePyWinrtCustomXamlResourceLoader
    {
        PyWinrtCustomXamlResourceLoader(PyObject* py_obj) : py::py_obj_ref(py_obj), BasePyWinrtCustomXamlResourceLoader() {}

        int32_t __stdcall GetPyObject(PyObject*& obj) override
        {
            obj = py::py_obj_ref::get_py_obj();
            return 0;
        }

        int32_t __stdcall GetComposableInner(winrt::Windows::Foundation::IInspectable& inner) override
        {
            inner = m_inner;
            return winrt::impl::error_ok;
        }

        static void toggle_reference(PyWinrtCustomXamlResourceLoader* instance, bool is_last_reference)
        {
            py::py_obj_ref::toggle_reference(instance, is_last_reference);
        }

        int32_t query_interface_tearoff(winrt::guid const& id, void** result) const noexcept override
        {
            return py::py_obj_ref::query_interface_tearoff(id, result);
        }

        std::vector<winrt::guid> get_iids_tearoff() const noexcept override
        {
            return py::py_obj_ref::get_iids_tearoff();
        }

        winrt::Windows::Foundation::IInspectable GetResource(winrt::hstring const& param0, winrt::hstring const& param1, winrt::hstring const& param2, winrt::hstring const& param3)
        {
            auto gil = py::ensure_gil();

            try
            {
                py::pyobj_handle self{get_py_obj()};

                py::pyobj_handle method{PyObject_GetAttrString(self.get(), "_get_resource")};
                if (!method)
                {
                    throw python_exception();
                }

                py::pyobj_handle py_param0{py::convert(param0)};
                if (!py_param0)
                {
                    throw python_exception();
                }

                py::pyobj_handle py_param1{py::convert(param1)};
                if (!py_param1)
                {
                    throw python_exception();
                }

                py::pyobj_handle py_param2{py::convert(param2)};
                if (!py_param2)
                {
                    throw python_exception();
                }

                py::pyobj_handle py_param3{py::convert(param3)};
                if (!py_param3)
                {
                    throw python_exception();
                }

                py::pyobj_handle args{PyTuple_Pack(4, py_param0.get(), py_param1.get(), py_param2.get(), py_param3.get())};
                if (!args)
                {
                    throw python_exception();
                }

                py::pyobj_handle return_value{PyObject_CallObject(method.get(), args.get())};
                if (!return_value)
                {
                    throw python_exception();
                }

                return py::convert_to<winrt::Windows::Foundation::IInspectable>(return_value.get());
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }
    };

    static PyObject* _new_CustomXamlResourceLoader(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        if (kwds)
        {
            py::set_invalid_kwd_args_error();
            return nullptr;
        }

        auto arg_count = PyTuple_GET_SIZE(args);

        auto self_type = get_python_type_for<winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoader>();
        if (!self_type)
        {
            return nullptr;
        }

        if (arg_count == 0)
        {
            try
            {
                if (type != self_type)
                {
                    py::pyobj_handle self{type->tp_alloc(type, 0)};
                    if (!self)
                    {
                        return nullptr;
                    }

                    std::construct_at(&reinterpret_cast<py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>*>(self.get())->obj, nullptr);

                    auto obj_impl = winrt::make_self<PyWinrtCustomXamlResourceLoader>(self.get());

                    auto obj = py::make_py_obj<PyWinrtCustomXamlResourceLoader>(obj_impl, type, self.get());
                    if (!obj)
                    {
                        return nullptr;
                    }

                    reinterpret_cast<py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>*>(self.get())->obj = std::move(obj);

                    return self.detach();
                }

                winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoader instance{};
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

    static void _dealloc_CustomXamlResourceLoader(py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* CustomXamlResourceLoader_GetResource(py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 4)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.UI.Xaml.Resources.CustomXamlResourceLoader", L"GetResource", 4);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(4);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::hstring>(args, 0);
                auto param1 = py::convert_to<winrt::hstring>(args, 1);
                auto param2 = py::convert_to<winrt::hstring>(args, 2);
                auto param3 = py::convert_to<winrt::hstring>(args, 3);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return py::get_inner_or_self(self->obj).try_as<winrt::Windows::UI::Xaml::Resources::ICustomXamlResourceLoaderOverrides>().GetResource(param0, param1, param2, param3);
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

    static PyObject* CustomXamlResourceLoader_get_Current(PyObject* /*unused*/, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.UI.Xaml.Resources.CustomXamlResourceLoader", L"Current");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoader::Current();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int CustomXamlResourceLoader_put_Current(PyObject* /*unused*/, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.UI.Xaml.Resources.CustomXamlResourceLoader", L"Current");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoader>(arg);

            {
                auto _gil = release_gil();
                winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoader::Current(param0);
            }

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
        auto array = std::make_unique<py::ComArray<winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoader>>();
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
            return py::convert(return_value.as<winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoader>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_CustomXamlResourceLoader[] = {
        { "_get_resource", reinterpret_cast<PyCFunction>(CustomXamlResourceLoader_GetResource), METH_VARARGS, nullptr },
        { "_assign_array_", _assign_array_CustomXamlResourceLoader, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_CustomXamlResourceLoader), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_CustomXamlResourceLoader[] = {
        { }};

    static PyType_Slot _type_slots_CustomXamlResourceLoader[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_CustomXamlResourceLoader) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_CustomXamlResourceLoader) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_CustomXamlResourceLoader) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_CustomXamlResourceLoader) },
        { }};

    static PyType_Spec type_spec_CustomXamlResourceLoader = {
        "winrt._winrt_windows_ui_xaml_resources.CustomXamlResourceLoader",
        sizeof(py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>),
        0,
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
        _type_slots_CustomXamlResourceLoader};

    static PyGetSetDef getset_CustomXamlResourceLoader_Static[] = {
        { "current", reinterpret_cast<getter>(CustomXamlResourceLoader_get_Current), reinterpret_cast<setter>(CustomXamlResourceLoader_put_Current), nullptr, nullptr },
        { }};

    static PyMethodDef methods_CustomXamlResourceLoader_Static[] = {
        { }};

    static PyType_Slot type_slots_CustomXamlResourceLoader_Static[] = 
    {
        { Py_tp_base, reinterpret_cast<void*>(&PyType_Type) },
        { Py_tp_getset, reinterpret_cast<void*>(getset_CustomXamlResourceLoader_Static) },
        { Py_tp_methods, reinterpret_cast<void*>(methods_CustomXamlResourceLoader_Static) },
        { }
    };

    static PyType_Spec type_spec_CustomXamlResourceLoader_Static = {
        "winrt._winrt_windows_ui_xaml_resources.CustomXamlResourceLoader_Static",
        static_cast<int>(PyType_Type.tp_basicsize),
        static_cast<int>(PyType_Type.tp_itemsize),
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
        type_slots_CustomXamlResourceLoader_Static};

    // ----- Windows.UI.Xaml.Resources Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.UI.Xaml.Resources");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_ui_xaml_resources",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::UI::Xaml::Resources

PyMODINIT_FUNC PyInit__winrt_windows_ui_xaml_resources(void) noexcept
{
    using namespace py::cpp::Windows::UI::Xaml::Resources;

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

    py::pyobj_handle CustomXamlResourceLoader_Static_bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(inspectable_meta_type))};
    if (!CustomXamlResourceLoader_Static_bases)
    {
        return nullptr;
    }

    py::pyobj_handle type_CustomXamlResourceLoader_Static{PyType_FromSpecWithBases(&type_spec_CustomXamlResourceLoader_Static, CustomXamlResourceLoader_Static_bases.get())};
    if (!type_CustomXamlResourceLoader_Static)
    {
        return nullptr;
    }

    py::pytype_handle CustomXamlResourceLoader_type{py::register_python_type(module.get(), &type_spec_CustomXamlResourceLoader, object_bases.get(), reinterpret_cast<PyTypeObject*>(type_CustomXamlResourceLoader_Static.get()))};
    if (!CustomXamlResourceLoader_type)
    {
        return nullptr;
    }


    return module.detach();
}
