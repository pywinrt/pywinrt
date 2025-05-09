// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.Devices.Pwm.Provider.h"

namespace py::cpp::Windows::Devices::Pwm::Provider
{
    // ----- IPwmControllerProvider interface --------------------

    #if PY_VERSION_HEX < 0x030A0000
    static PyObject* _new_IPwmControllerProvider(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Devices::Pwm::Provider::IPwmControllerProvider>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Devices::Pwm::Provider::IPwmControllerProvider>::type_name);
        return nullptr;
    }
    #endif

    static void _dealloc_IPwmControllerProvider(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* IPwmControllerProvider_AcquirePin(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Devices.Pwm.Provider.IPwmControllerProvider", L"AcquirePin", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<int32_t>(args, 0);

                {
                    auto _gil = release_gil();
                    self->obj.AcquirePin(param0);
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

    static PyObject* IPwmControllerProvider_DisablePin(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Devices.Pwm.Provider.IPwmControllerProvider", L"DisablePin", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<int32_t>(args, 0);

                {
                    auto _gil = release_gil();
                    self->obj.DisablePin(param0);
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

    static PyObject* IPwmControllerProvider_EnablePin(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Devices.Pwm.Provider.IPwmControllerProvider", L"EnablePin", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<int32_t>(args, 0);

                {
                    auto _gil = release_gil();
                    self->obj.EnablePin(param0);
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

    static PyObject* IPwmControllerProvider_ReleasePin(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Devices.Pwm.Provider.IPwmControllerProvider", L"ReleasePin", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<int32_t>(args, 0);

                {
                    auto _gil = release_gil();
                    self->obj.ReleasePin(param0);
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

    static PyObject* IPwmControllerProvider_SetDesiredFrequency(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Devices.Pwm.Provider.IPwmControllerProvider", L"SetDesiredFrequency", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<double>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.SetDesiredFrequency(param0);
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

    static PyObject* IPwmControllerProvider_SetPulseParameters(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 3)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Devices.Pwm.Provider.IPwmControllerProvider", L"SetPulseParameters", 3);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(3);
                    return nullptr;
                }

                auto param0 = py::convert_to<int32_t>(args, 0);
                auto param1 = py::convert_to<double>(args, 1);
                auto param2 = py::convert_to<bool>(args, 2);

                {
                    auto _gil = release_gil();
                    self->obj.SetPulseParameters(param0, param1, param2);
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

    static PyObject* IPwmControllerProvider_get_ActualFrequency(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Devices.Pwm.Provider.IPwmControllerProvider", L"ActualFrequency");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.ActualFrequency();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* IPwmControllerProvider_get_MaxFrequency(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Devices.Pwm.Provider.IPwmControllerProvider", L"MaxFrequency");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.MaxFrequency();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* IPwmControllerProvider_get_MinFrequency(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Devices.Pwm.Provider.IPwmControllerProvider", L"MinFrequency");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.MinFrequency();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* IPwmControllerProvider_get_PinCount(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Devices.Pwm.Provider.IPwmControllerProvider", L"PinCount");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.PinCount();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_IPwmControllerProvider[] = {
        { "acquire_pin", reinterpret_cast<PyCFunction>(IPwmControllerProvider_AcquirePin), METH_VARARGS, nullptr },
        { "disable_pin", reinterpret_cast<PyCFunction>(IPwmControllerProvider_DisablePin), METH_VARARGS, nullptr },
        { "enable_pin", reinterpret_cast<PyCFunction>(IPwmControllerProvider_EnablePin), METH_VARARGS, nullptr },
        { "release_pin", reinterpret_cast<PyCFunction>(IPwmControllerProvider_ReleasePin), METH_VARARGS, nullptr },
        { "set_desired_frequency", reinterpret_cast<PyCFunction>(IPwmControllerProvider_SetDesiredFrequency), METH_VARARGS, nullptr },
        { "set_pulse_parameters", reinterpret_cast<PyCFunction>(IPwmControllerProvider_SetPulseParameters), METH_VARARGS, nullptr },
        { }};

    static PyGetSetDef _getset_IPwmControllerProvider[] = {
        { "actual_frequency", reinterpret_cast<getter>(IPwmControllerProvider_get_ActualFrequency), nullptr, nullptr, nullptr },
        { "max_frequency", reinterpret_cast<getter>(IPwmControllerProvider_get_MaxFrequency), nullptr, nullptr, nullptr },
        { "min_frequency", reinterpret_cast<getter>(IPwmControllerProvider_get_MinFrequency), nullptr, nullptr, nullptr },
        { "pin_count", reinterpret_cast<getter>(IPwmControllerProvider_get_PinCount), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_IPwmControllerProvider[] = {
        #if PY_VERSION_HEX < 0x030A0000
        { Py_tp_new, reinterpret_cast<void*>(_new_IPwmControllerProvider) },
        #endif
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_IPwmControllerProvider) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_IPwmControllerProvider) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_IPwmControllerProvider) },
        { }};

    static PyType_Spec type_spec_IPwmControllerProvider = {
        "winrt._winrt_windows_devices_pwm_provider._IPwmControllerProvider",
        sizeof(py::wrapper::Windows::Devices::Pwm::Provider::IPwmControllerProvider),
        0,
        Py_TPFLAGS_DEFAULT
        #if PY_VERSION_HEX >= 0x030A0000
        | Py_TPFLAGS_DISALLOW_INSTANTIATION
        #endif
        ,
        _type_slots_IPwmControllerProvider};

    struct ImplementsIPwmControllerProvider : py::ImplementsInterfaceT<ImplementsIPwmControllerProvider, winrt::Windows::Devices::Pwm::Provider::IPwmControllerProvider>
    {
        ImplementsIPwmControllerProvider() = delete;
        ImplementsIPwmControllerProvider(PyObject* py_obj, winrt::impl::inspectable_abi* runtime_class) : py::ImplementsInterfaceT<ImplementsIPwmControllerProvider, winrt::Windows::Devices::Pwm::Provider::IPwmControllerProvider>(py_obj, runtime_class)
        {
        }

        auto AcquirePin(int32_t param0)
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle method{PyObject_GetAttrString(self.get(), "acquire_pin")};
                if (!method)
                {
                    throw python_exception();
                }

                py::pyobj_handle py_param0{py::convert(param0)};
                if (!py_param0)
                {
                    throw python_exception();
                }

                py::pyobj_handle return_value{PyObject_CallOneArg(method.get(), py_param0.get())};
                if (!return_value)
                {
                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }

        auto DisablePin(int32_t param0)
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle method{PyObject_GetAttrString(self.get(), "disable_pin")};
                if (!method)
                {
                    throw python_exception();
                }

                py::pyobj_handle py_param0{py::convert(param0)};
                if (!py_param0)
                {
                    throw python_exception();
                }

                py::pyobj_handle return_value{PyObject_CallOneArg(method.get(), py_param0.get())};
                if (!return_value)
                {
                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }

        auto EnablePin(int32_t param0)
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle method{PyObject_GetAttrString(self.get(), "enable_pin")};
                if (!method)
                {
                    throw python_exception();
                }

                py::pyobj_handle py_param0{py::convert(param0)};
                if (!py_param0)
                {
                    throw python_exception();
                }

                py::pyobj_handle return_value{PyObject_CallOneArg(method.get(), py_param0.get())};
                if (!return_value)
                {
                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }

        auto ReleasePin(int32_t param0)
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle method{PyObject_GetAttrString(self.get(), "release_pin")};
                if (!method)
                {
                    throw python_exception();
                }

                py::pyobj_handle py_param0{py::convert(param0)};
                if (!py_param0)
                {
                    throw python_exception();
                }

                py::pyobj_handle return_value{PyObject_CallOneArg(method.get(), py_param0.get())};
                if (!return_value)
                {
                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }

        auto SetDesiredFrequency(double param0)
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle method{PyObject_GetAttrString(self.get(), "set_desired_frequency")};
                if (!method)
                {
                    throw python_exception();
                }

                py::pyobj_handle py_param0{py::convert(param0)};
                if (!py_param0)
                {
                    throw python_exception();
                }

                py::pyobj_handle return_value{PyObject_CallOneArg(method.get(), py_param0.get())};
                if (!return_value)
                {
                    throw python_exception();
                }

                return py::convert_to<double>(return_value.get());
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }

        auto SetPulseParameters(int32_t param0, double param1, bool param2)
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle method{PyObject_GetAttrString(self.get(), "set_pulse_parameters")};
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

                py::pyobj_handle args{PyTuple_Pack(3, py_param0.get(), py_param1.get(), py_param2.get())};
                if (!args)
                {
                    throw python_exception();
                }

                py::pyobj_handle return_value{PyObject_CallObject(method.get(), args.get())};
                if (!return_value)
                {
                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }

        auto ActualFrequency()
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle value{PyObject_GetAttrString(self.get(), "actual_frequency")};
                if (!value)
                {
                    throw python_exception();
                }

                return py::convert_to<double>(value.get());
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }

        auto MaxFrequency()
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle value{PyObject_GetAttrString(self.get(), "max_frequency")};
                if (!value)
                {
                    throw python_exception();
                }

                return py::convert_to<double>(value.get());
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }

        auto MinFrequency()
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle value{PyObject_GetAttrString(self.get(), "min_frequency")};
                if (!value)
                {
                    throw python_exception();
                }

                return py::convert_to<double>(value.get());
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }

        auto PinCount()
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle value{PyObject_GetAttrString(self.get(), "pin_count")};
                if (!value)
                {
                    throw python_exception();
                }

                return py::convert_to<int32_t>(value.get());
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }
    };

    static PyObject* _assign_array_IPwmControllerProvider(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Devices::Pwm::Provider::IPwmControllerProvider>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_IPwmControllerProvider(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Devices::Pwm::Provider::IPwmControllerProvider>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _guid_ImplementsIPwmControllerProvider(PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        try
        {
            return py::convert(winrt::guid_of<winrt::Windows::Devices::Pwm::Provider::IPwmControllerProvider>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _make_ImplementsIPwmControllerProvider(PyObject* /*unused*/, PyObject* args) noexcept
    {
        try
        {
            PyObject* py_obj;
            winrt::impl::inspectable_abi* runtime_class;

            if (!PyArg_ParseTuple(args, "On", &py_obj, &runtime_class))
            {
                return nullptr;
            }

            auto iface{std::make_unique<ImplementsIPwmControllerProvider>(py_obj, runtime_class)};

            return PyLong_FromVoidPtr(iface.release());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef methods_ImplementsIPwmControllerProvider[] = {
        { "_assign_array_", _assign_array_IPwmControllerProvider, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_IPwmControllerProvider), METH_O | METH_STATIC, nullptr },
        { "_guid_", reinterpret_cast<PyCFunction>(_guid_ImplementsIPwmControllerProvider), METH_NOARGS | METH_STATIC, nullptr },
        { "_make_", reinterpret_cast<PyCFunction>(_make_ImplementsIPwmControllerProvider), METH_VARARGS | METH_STATIC, nullptr },
        { }};

    static PyType_Slot type_slots_ImplementsIPwmControllerProvider[] = {
        { Py_tp_methods, reinterpret_cast<void*>(methods_ImplementsIPwmControllerProvider) },
        { }};

    static PyType_Spec type_spec_ImplementsIPwmControllerProvider = {
        "winrt._winrt_windows_devices_pwm_provider.IPwmControllerProvider",
        0,
        0,
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE
        #if PY_VERSION_HEX >= 0x030A0000
        | Py_TPFLAGS_DISALLOW_INSTANTIATION
        #endif
        ,
        type_slots_ImplementsIPwmControllerProvider};

    // ----- IPwmProvider interface --------------------

    #if PY_VERSION_HEX < 0x030A0000
    static PyObject* _new_IPwmProvider(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Devices::Pwm::Provider::IPwmProvider>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Devices::Pwm::Provider::IPwmProvider>::type_name);
        return nullptr;
    }
    #endif

    static void _dealloc_IPwmProvider(py::wrapper::Windows::Devices::Pwm::Provider::IPwmProvider* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* IPwmProvider_GetControllers(py::wrapper::Windows::Devices::Pwm::Provider::IPwmProvider* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Devices.Pwm.Provider.IPwmProvider", L"GetControllers", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.GetControllers();
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

    static PyMethodDef _methods_IPwmProvider[] = {
        { "get_controllers", reinterpret_cast<PyCFunction>(IPwmProvider_GetControllers), METH_VARARGS, nullptr },
        { }};

    static PyGetSetDef _getset_IPwmProvider[] = {
        { }};

    static PyType_Slot _type_slots_IPwmProvider[] = {
        #if PY_VERSION_HEX < 0x030A0000
        { Py_tp_new, reinterpret_cast<void*>(_new_IPwmProvider) },
        #endif
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_IPwmProvider) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_IPwmProvider) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_IPwmProvider) },
        { }};

    static PyType_Spec type_spec_IPwmProvider = {
        "winrt._winrt_windows_devices_pwm_provider._IPwmProvider",
        sizeof(py::wrapper::Windows::Devices::Pwm::Provider::IPwmProvider),
        0,
        Py_TPFLAGS_DEFAULT
        #if PY_VERSION_HEX >= 0x030A0000
        | Py_TPFLAGS_DISALLOW_INSTANTIATION
        #endif
        ,
        _type_slots_IPwmProvider};

    struct ImplementsIPwmProvider : py::ImplementsInterfaceT<ImplementsIPwmProvider, winrt::Windows::Devices::Pwm::Provider::IPwmProvider>
    {
        ImplementsIPwmProvider() = delete;
        ImplementsIPwmProvider(PyObject* py_obj, winrt::impl::inspectable_abi* runtime_class) : py::ImplementsInterfaceT<ImplementsIPwmProvider, winrt::Windows::Devices::Pwm::Provider::IPwmProvider>(py_obj, runtime_class)
        {
        }

        auto GetControllers()
        {
            try
            {
                py::pyobj_handle self{this->get_py_obj()};

                py::pyobj_handle method{PyObject_GetAttrString(self.get(), "get_controllers")};
                if (!method)
                {
                    throw python_exception();
                }

                py::pyobj_handle return_value{PyObject_CallNoArgs(method.get())};
                if (!return_value)
                {
                    throw python_exception();
                }

                return py::convert_to<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Devices::Pwm::Provider::IPwmControllerProvider>>(return_value.get());
            }
            catch (python_exception)
            {
                py::write_unraisable_and_throw();
            }
        }
    };

    static PyObject* _assign_array_IPwmProvider(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Devices::Pwm::Provider::IPwmProvider>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_IPwmProvider(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Devices::Pwm::Provider::IPwmProvider>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _guid_ImplementsIPwmProvider(PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        try
        {
            return py::convert(winrt::guid_of<winrt::Windows::Devices::Pwm::Provider::IPwmProvider>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _make_ImplementsIPwmProvider(PyObject* /*unused*/, PyObject* args) noexcept
    {
        try
        {
            PyObject* py_obj;
            winrt::impl::inspectable_abi* runtime_class;

            if (!PyArg_ParseTuple(args, "On", &py_obj, &runtime_class))
            {
                return nullptr;
            }

            auto iface{std::make_unique<ImplementsIPwmProvider>(py_obj, runtime_class)};

            return PyLong_FromVoidPtr(iface.release());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef methods_ImplementsIPwmProvider[] = {
        { "_assign_array_", _assign_array_IPwmProvider, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_IPwmProvider), METH_O | METH_STATIC, nullptr },
        { "_guid_", reinterpret_cast<PyCFunction>(_guid_ImplementsIPwmProvider), METH_NOARGS | METH_STATIC, nullptr },
        { "_make_", reinterpret_cast<PyCFunction>(_make_ImplementsIPwmProvider), METH_VARARGS | METH_STATIC, nullptr },
        { }};

    static PyType_Slot type_slots_ImplementsIPwmProvider[] = {
        { Py_tp_methods, reinterpret_cast<void*>(methods_ImplementsIPwmProvider) },
        { }};

    static PyType_Spec type_spec_ImplementsIPwmProvider = {
        "winrt._winrt_windows_devices_pwm_provider.IPwmProvider",
        0,
        0,
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE
        #if PY_VERSION_HEX >= 0x030A0000
        | Py_TPFLAGS_DISALLOW_INSTANTIATION
        #endif
        ,
        type_slots_ImplementsIPwmProvider};

    // ----- Windows.Devices.Pwm.Provider Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.Devices.Pwm.Provider");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_devices_pwm_provider",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::Devices::Pwm::Provider

PyMODINIT_FUNC PyInit__winrt_windows_devices_pwm_provider(void) noexcept
{
    using namespace py::cpp::Windows::Devices::Pwm::Provider;

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

    py::pytype_handle IPwmControllerProvider_type{py::register_python_type(module.get(), &type_spec_IPwmControllerProvider, object_bases.get(), nullptr)};
    if (!IPwmControllerProvider_type)
    {
        return nullptr;
    }

    py::pytype_handle ImplementsIPwmControllerProvider_type{py::register_python_type(module.get(), &type_spec_ImplementsIPwmControllerProvider, nullptr, inspectable_meta_type)};
    if (!ImplementsIPwmControllerProvider_type)
    {
        return nullptr;
    }

    if (PyModule_AddType(module.get(), ImplementsIPwmControllerProvider_type.get()) == -1)
    {
        return nullptr;
    }

    py::pytype_handle IPwmProvider_type{py::register_python_type(module.get(), &type_spec_IPwmProvider, object_bases.get(), nullptr)};
    if (!IPwmProvider_type)
    {
        return nullptr;
    }

    py::pytype_handle ImplementsIPwmProvider_type{py::register_python_type(module.get(), &type_spec_ImplementsIPwmProvider, nullptr, inspectable_meta_type)};
    if (!ImplementsIPwmProvider_type)
    {
        return nullptr;
    }

    if (PyModule_AddType(module.get(), ImplementsIPwmProvider_type.get()) == -1)
    {
        return nullptr;
    }


    return module.detach();
}
