// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "pybase.h"
#include "py.Windows.ApplicationModel.Wallet.System.h"


namespace py::cpp::Windows::ApplicationModel::Wallet::System
{
    // ----- WalletItemSystemStore class --------------------
    static constexpr const char* const type_name_WalletItemSystemStore = "WalletItemSystemStore";

    static PyObject* _new_WalletItemSystemStore(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        py::set_invalid_activation_error(type_name_WalletItemSystemStore);
        return nullptr;
    }

    static void _dealloc_WalletItemSystemStore(py::wrapper::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore* self) noexcept
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

    static PyObject* WalletItemSystemStore_DeleteAsync(py::wrapper::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Wallet.System.WalletItemSystemStore", L"DeleteAsync", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::ApplicationModel::Wallet::WalletItem>(args, 0);

                return py::convert(self->obj.DeleteAsync(param0));
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

    static PyObject* WalletItemSystemStore_GetAppStatusForItem(py::wrapper::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Wallet.System.WalletItemSystemStore", L"GetAppStatusForItem", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::ApplicationModel::Wallet::WalletItem>(args, 0);

                return py::convert(self->obj.GetAppStatusForItem(param0));
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

    static PyObject* WalletItemSystemStore_GetItemsAsync(py::wrapper::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 0)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Wallet.System.WalletItemSystemStore", L"GetItemsAsync", 0))
            {
                py::set_arg_count_version_error(0);
                return nullptr;
            }

            try
            {
                return py::convert(self->obj.GetItemsAsync());
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

    static PyObject* WalletItemSystemStore_ImportItemAsync(py::wrapper::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Wallet.System.WalletItemSystemStore", L"ImportItemAsync", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::Storage::Streams::IRandomAccessStreamReference>(args, 0);

                return py::convert(self->obj.ImportItemAsync(param0));
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

    static PyObject* WalletItemSystemStore_LaunchAppForItemAsync(py::wrapper::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Wallet.System.WalletItemSystemStore", L"LaunchAppForItemAsync", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::ApplicationModel::Wallet::WalletItem>(args, 0);

                return py::convert(self->obj.LaunchAppForItemAsync(param0));
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

    static PyObject* WalletItemSystemStore_add_ItemsChanged(py::wrapper::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore* self, PyObject* arg) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.ApplicationModel.Wallet.System.WalletItemSystemStore", L"ItemsChanged"))
        {
            PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            auto param0 = py::convert_to<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore, winrt::Windows::Foundation::IInspectable>>(arg);

            return py::convert(self->obj.ItemsChanged(param0));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* WalletItemSystemStore_remove_ItemsChanged(py::wrapper::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore* self, PyObject* arg) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.ApplicationModel.Wallet.System.WalletItemSystemStore", L"ItemsChanged"))
        {
            PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            auto param0 = py::convert_to<winrt::event_token>(arg);

            self->obj.ItemsChanged(param0);
            Py_RETURN_NONE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_WalletItemSystemStore(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_WalletItemSystemStore(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_WalletItemSystemStore[] = {
        { "delete_async", reinterpret_cast<PyCFunction>(WalletItemSystemStore_DeleteAsync), METH_VARARGS, nullptr },
        { "get_app_status_for_item", reinterpret_cast<PyCFunction>(WalletItemSystemStore_GetAppStatusForItem), METH_VARARGS, nullptr },
        { "get_items_async", reinterpret_cast<PyCFunction>(WalletItemSystemStore_GetItemsAsync), METH_VARARGS, nullptr },
        { "import_item_async", reinterpret_cast<PyCFunction>(WalletItemSystemStore_ImportItemAsync), METH_VARARGS, nullptr },
        { "launch_app_for_item_async", reinterpret_cast<PyCFunction>(WalletItemSystemStore_LaunchAppForItemAsync), METH_VARARGS, nullptr },
        { "add_items_changed", reinterpret_cast<PyCFunction>(WalletItemSystemStore_add_ItemsChanged), METH_O, nullptr },
        { "remove_items_changed", reinterpret_cast<PyCFunction>(WalletItemSystemStore_remove_ItemsChanged), METH_O, nullptr },
        { "_assign_array_", _assign_array_WalletItemSystemStore, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_WalletItemSystemStore), METH_O | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_WalletItemSystemStore[] = {
        { }
    };

    static PyType_Slot _type_slots_WalletItemSystemStore[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_WalletItemSystemStore) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_WalletItemSystemStore) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_WalletItemSystemStore) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_WalletItemSystemStore) },
        { },
    };

    static PyType_Spec type_spec_WalletItemSystemStore =
    {
        "_winrt_windows_applicationmodel_wallet_system.WalletItemSystemStore",
        sizeof(py::wrapper::Windows::ApplicationModel::Wallet::System::WalletItemSystemStore),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_WalletItemSystemStore
    };

    // ----- WalletManagerSystem class --------------------
    static constexpr const char* const type_name_WalletManagerSystem = "WalletManagerSystem";

    static PyObject* _new_WalletManagerSystem(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        py::set_invalid_activation_error(type_name_WalletManagerSystem);
        return nullptr;
    }

    static PyObject* WalletManagerSystem_RequestStoreAsync(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 0)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.Wallet.System.WalletManagerSystem", L"RequestStoreAsync", 0))
            {
                py::set_arg_count_version_error(0);
                return nullptr;
            }

            try
            {
                return py::convert(winrt::Windows::ApplicationModel::Wallet::System::WalletManagerSystem::RequestStoreAsync());
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

    static PyMethodDef _methods_WalletManagerSystem[] = {
        { "request_store_async", reinterpret_cast<PyCFunction>(WalletManagerSystem_RequestStoreAsync), METH_VARARGS | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_WalletManagerSystem[] = {
        { }
    };

    static PyType_Slot _type_slots_WalletManagerSystem[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_WalletManagerSystem) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_WalletManagerSystem) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_WalletManagerSystem) },
        { },
    };

    static PyType_Spec type_spec_WalletManagerSystem =
    {
        "_winrt_windows_applicationmodel_wallet_system.WalletManagerSystem",
        0,
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_WalletManagerSystem
    };

    // ----- Windows.ApplicationModel.Wallet.System Initialization --------------------
    PyDoc_STRVAR(module_doc, "Windows::ApplicationModel::Wallet::System");


    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_applicationmodel_wallet_system",
           module_doc,
           0,
           nullptr,
           nullptr,
           nullptr,
           nullptr,
           nullptr};

} // py::cpp::Windows::ApplicationModel::Wallet::System

PyMODINIT_FUNC PyInit__winrt_windows_applicationmodel_wallet_system(void) noexcept
{
    using namespace py::cpp::Windows::ApplicationModel::Wallet::System;

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
    if (py::register_python_type(module.get(), type_name_WalletItemSystemStore, &type_spec_WalletItemSystemStore, nullptr, object_bases.get(), nullptr) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_WalletItemSystemStore, &type_spec_WalletItemSystemStore, object_bases.get(), nullptr) == -1)
    #endif
    {
        return nullptr;
    }

    #if PY_VERSION_HEX < 0x03090000
    if (py::register_python_type(module.get(), type_name_WalletManagerSystem, &type_spec_WalletManagerSystem, nullptr, object_bases.get(), nullptr) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_WalletManagerSystem, &type_spec_WalletManagerSystem, object_bases.get(), nullptr) == -1)
    #endif
    {
        return nullptr;
    }


    return module.detach();
}