// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.Phone.Devices.Notification.h"

namespace py::cpp::Windows::Phone::Devices::Notification
{
    // ----- VibrationDevice class --------------------

    static PyObject* _new_VibrationDevice(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Phone::Devices::Notification::VibrationDevice>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Phone::Devices::Notification::VibrationDevice>::type_name);
        return nullptr;
    }

    static void _dealloc_VibrationDevice(py::wrapper::Windows::Phone::Devices::Notification::VibrationDevice* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* VibrationDevice_Cancel(py::wrapper::Windows::Phone::Devices::Notification::VibrationDevice* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Phone.Devices.Notification.VibrationDevice", L"Cancel", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                {
                    auto _gil = release_gil();
                    self->obj.Cancel();
                }

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

    static PyObject* VibrationDevice_GetDefault(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Phone.Devices.Notification.VibrationDevice", L"GetDefault", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Windows::Phone::Devices::Notification::VibrationDevice::GetDefault();
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

    static PyObject* VibrationDevice_Vibrate(py::wrapper::Windows::Phone::Devices::Notification::VibrationDevice* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Phone.Devices.Notification.VibrationDevice", L"Vibrate", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Foundation::TimeSpan>(args, 0);

                {
                    auto _gil = release_gil();
                    self->obj.Vibrate(param0);
                }

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

    static PyObject* _assign_array_VibrationDevice(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Phone::Devices::Notification::VibrationDevice>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_VibrationDevice(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Phone::Devices::Notification::VibrationDevice>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_VibrationDevice[] = {
        { "cancel", reinterpret_cast<PyCFunction>(VibrationDevice_Cancel), METH_VARARGS, nullptr },
        { "vibrate", reinterpret_cast<PyCFunction>(VibrationDevice_Vibrate), METH_VARARGS, nullptr },
        { "_assign_array_", _assign_array_VibrationDevice, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_VibrationDevice), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_VibrationDevice[] = {
        { }};

    static PyType_Slot _type_slots_VibrationDevice[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_VibrationDevice) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_VibrationDevice) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_VibrationDevice) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_VibrationDevice) },
        { }};

    static PyType_Spec type_spec_VibrationDevice = {
        "winrt._winrt_windows_phone_devices_notification.VibrationDevice",
        sizeof(py::wrapper::Windows::Phone::Devices::Notification::VibrationDevice),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_VibrationDevice};

    static PyGetSetDef getset_VibrationDevice_Static[] = {
        { }};

    static PyMethodDef methods_VibrationDevice_Static[] = {
        { "get_default", reinterpret_cast<PyCFunction>(VibrationDevice_GetDefault), METH_VARARGS, nullptr },
        { }};

    static PyType_Slot type_slots_VibrationDevice_Static[] = 
    {
        { Py_tp_base, reinterpret_cast<void*>(&PyType_Type) },
        { Py_tp_getset, reinterpret_cast<void*>(getset_VibrationDevice_Static) },
        { Py_tp_methods, reinterpret_cast<void*>(methods_VibrationDevice_Static) },
        { }
    };

    static PyType_Spec type_spec_VibrationDevice_Static = {
        "winrt._winrt_windows_phone_devices_notification.VibrationDevice_Static",
        static_cast<int>(PyType_Type.tp_basicsize),
        static_cast<int>(PyType_Type.tp_itemsize),
        Py_TPFLAGS_DEFAULT,
        type_slots_VibrationDevice_Static};

    // ----- Windows.Phone.Devices.Notification Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.Phone.Devices.Notification");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_phone_devices_notification",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::Phone::Devices::Notification

PyMODINIT_FUNC PyInit__winrt_windows_phone_devices_notification(void) noexcept
{
    using namespace py::cpp::Windows::Phone::Devices::Notification;

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

    py::pyobj_handle VibrationDevice_Static_bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(inspectable_meta_type))};
    if (!VibrationDevice_Static_bases)
    {
        return nullptr;
    }

    py::pyobj_handle type_VibrationDevice_Static{PyType_FromSpecWithBases(&type_spec_VibrationDevice_Static, VibrationDevice_Static_bases.get())};
    if (!type_VibrationDevice_Static)
    {
        return nullptr;
    }

    py::pytype_handle VibrationDevice_type{py::register_python_type(module.get(), &type_spec_VibrationDevice, object_bases.get(), reinterpret_cast<PyTypeObject*>(type_VibrationDevice_Static.get()))};
    if (!VibrationDevice_type)
    {
        return nullptr;
    }


    return module.detach();
}
