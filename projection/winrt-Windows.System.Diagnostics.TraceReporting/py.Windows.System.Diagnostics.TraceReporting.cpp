// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "pybase.h"
#include "py.Windows.System.Diagnostics.TraceReporting.h"


namespace py::cpp::Windows::System::Diagnostics::TraceReporting
{
    // ----- PlatformDiagnosticActions class --------------------
    static constexpr const char* const type_name_PlatformDiagnosticActions = "PlatformDiagnosticActions";

    static PyObject* _new_PlatformDiagnosticActions(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        py::set_invalid_activation_error(type_name_PlatformDiagnosticActions);
        return nullptr;
    }

    static PyObject* PlatformDiagnosticActions_DownloadLatestSettingsForNamespace(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 5)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticActions", L"DownloadLatestSettingsForNamespace", 5))
            {
                py::set_arg_count_version_error(5);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::hstring>(args, 0);
                auto param1 = py::convert_to<winrt::hstring>(args, 1);
                auto param2 = py::convert_to<bool>(args, 2);
                auto param3 = py::convert_to<bool>(args, 3);
                auto param4 = py::convert_to<bool>(args, 4);

                return py::convert(winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticActions::DownloadLatestSettingsForNamespace(param0, param1, param2, param3, param4));
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

    static PyObject* PlatformDiagnosticActions_ForceUpload(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 3)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticActions", L"ForceUpload", 3))
            {
                py::set_arg_count_version_error(3);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticEventBufferLatencies>(args, 0);
                auto param1 = py::convert_to<bool>(args, 1);
                auto param2 = py::convert_to<bool>(args, 2);

                return py::convert(winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticActions::ForceUpload(param0, param1, param2));
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

    static PyObject* PlatformDiagnosticActions_GetActiveScenarioList(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 0)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticActions", L"GetActiveScenarioList", 0))
            {
                py::set_arg_count_version_error(0);
                return nullptr;
            }

            try
            {
                return py::convert(winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticActions::GetActiveScenarioList());
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

    static PyObject* PlatformDiagnosticActions_GetActiveTraceRuntime(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticActions", L"GetActiveTraceRuntime", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceSlotType>(args, 0);

                return py::convert(winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticActions::GetActiveTraceRuntime(param0));
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

    static PyObject* PlatformDiagnosticActions_GetKnownTraceList(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticActions", L"GetKnownTraceList", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceSlotType>(args, 0);

                return py::convert(winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticActions::GetKnownTraceList(param0));
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

    static PyObject* PlatformDiagnosticActions_IsScenarioEnabled(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 1)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticActions", L"IsScenarioEnabled", 1))
            {
                py::set_arg_count_version_error(1);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::guid>(args, 0);

                return py::convert(winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticActions::IsScenarioEnabled(param0));
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

    static PyObject* PlatformDiagnosticActions_IsTraceRunning(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 3)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticActions", L"IsTraceRunning", 3))
            {
                py::set_arg_count_version_error(3);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceSlotType>(args, 0);
                auto param1 = py::convert_to<winrt::guid>(args, 1);
                auto param2 = py::convert_to<uint64_t>(args, 2);

                return py::convert(winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticActions::IsTraceRunning(param0, param1, param2));
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

    static PyObject* PlatformDiagnosticActions_TryEscalateScenario(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_Size(args);

        if (arg_count == 6)
        {
            if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticActions", L"TryEscalateScenario", 6))
            {
                py::set_arg_count_version_error(6);
                return nullptr;
            }

            try
            {
                auto param0 = py::convert_to<winrt::guid>(args, 0);
                auto param1 = py::convert_to<winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticEscalationType>(args, 1);
                auto param2 = py::convert_to<winrt::hstring>(args, 2);
                auto param3 = py::convert_to<bool>(args, 3);
                auto param4 = py::convert_to<bool>(args, 4);
                auto param5 = py::convert_to<winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::hstring>>(args, 5);

                return py::convert(winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticActions::TryEscalateScenario(param0, param1, param2, param3, param4, param5));
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

    static PyMethodDef _methods_PlatformDiagnosticActions[] = {
        { "download_latest_settings_for_namespace", reinterpret_cast<PyCFunction>(PlatformDiagnosticActions_DownloadLatestSettingsForNamespace), METH_VARARGS | METH_STATIC, nullptr },
        { "force_upload", reinterpret_cast<PyCFunction>(PlatformDiagnosticActions_ForceUpload), METH_VARARGS | METH_STATIC, nullptr },
        { "get_active_scenario_list", reinterpret_cast<PyCFunction>(PlatformDiagnosticActions_GetActiveScenarioList), METH_VARARGS | METH_STATIC, nullptr },
        { "get_active_trace_runtime", reinterpret_cast<PyCFunction>(PlatformDiagnosticActions_GetActiveTraceRuntime), METH_VARARGS | METH_STATIC, nullptr },
        { "get_known_trace_list", reinterpret_cast<PyCFunction>(PlatformDiagnosticActions_GetKnownTraceList), METH_VARARGS | METH_STATIC, nullptr },
        { "is_scenario_enabled", reinterpret_cast<PyCFunction>(PlatformDiagnosticActions_IsScenarioEnabled), METH_VARARGS | METH_STATIC, nullptr },
        { "is_trace_running", reinterpret_cast<PyCFunction>(PlatformDiagnosticActions_IsTraceRunning), METH_VARARGS | METH_STATIC, nullptr },
        { "try_escalate_scenario", reinterpret_cast<PyCFunction>(PlatformDiagnosticActions_TryEscalateScenario), METH_VARARGS | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_PlatformDiagnosticActions[] = {
        { }
    };

    static PyType_Slot _type_slots_PlatformDiagnosticActions[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_PlatformDiagnosticActions) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_PlatformDiagnosticActions) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_PlatformDiagnosticActions) },
        { },
    };

    static PyType_Spec type_spec_PlatformDiagnosticActions =
    {
        "_winrt_windows_system_diagnostics_tracereporting.PlatformDiagnosticActions",
        0,
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_PlatformDiagnosticActions
    };

    // ----- PlatformDiagnosticTraceInfo class --------------------
    static constexpr const char* const type_name_PlatformDiagnosticTraceInfo = "PlatformDiagnosticTraceInfo";

    static PyObject* _new_PlatformDiagnosticTraceInfo(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        py::set_invalid_activation_error(type_name_PlatformDiagnosticTraceInfo);
        return nullptr;
    }

    static void _dealloc_PlatformDiagnosticTraceInfo(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceInfo* self) noexcept
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

    static PyObject* PlatformDiagnosticTraceInfo_get_IsAutoLogger(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceInfo* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticTraceInfo", L"IsAutoLogger"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.IsAutoLogger());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* PlatformDiagnosticTraceInfo_get_IsExclusive(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceInfo* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticTraceInfo", L"IsExclusive"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.IsExclusive());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* PlatformDiagnosticTraceInfo_get_MaxTraceDurationFileTime(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceInfo* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticTraceInfo", L"MaxTraceDurationFileTime"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.MaxTraceDurationFileTime());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* PlatformDiagnosticTraceInfo_get_Priority(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceInfo* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticTraceInfo", L"Priority"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.Priority());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* PlatformDiagnosticTraceInfo_get_ProfileHash(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceInfo* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticTraceInfo", L"ProfileHash"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.ProfileHash());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* PlatformDiagnosticTraceInfo_get_ScenarioId(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceInfo* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticTraceInfo", L"ScenarioId"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.ScenarioId());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_PlatformDiagnosticTraceInfo(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceInfo>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_PlatformDiagnosticTraceInfo(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceInfo>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_PlatformDiagnosticTraceInfo[] = {
        { "_assign_array_", _assign_array_PlatformDiagnosticTraceInfo, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_PlatformDiagnosticTraceInfo), METH_O | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_PlatformDiagnosticTraceInfo[] = {
        { "is_auto_logger", reinterpret_cast<getter>(PlatformDiagnosticTraceInfo_get_IsAutoLogger), nullptr, nullptr, nullptr },
        { "is_exclusive", reinterpret_cast<getter>(PlatformDiagnosticTraceInfo_get_IsExclusive), nullptr, nullptr, nullptr },
        { "max_trace_duration_file_time", reinterpret_cast<getter>(PlatformDiagnosticTraceInfo_get_MaxTraceDurationFileTime), nullptr, nullptr, nullptr },
        { "priority", reinterpret_cast<getter>(PlatformDiagnosticTraceInfo_get_Priority), nullptr, nullptr, nullptr },
        { "profile_hash", reinterpret_cast<getter>(PlatformDiagnosticTraceInfo_get_ProfileHash), nullptr, nullptr, nullptr },
        { "scenario_id", reinterpret_cast<getter>(PlatformDiagnosticTraceInfo_get_ScenarioId), nullptr, nullptr, nullptr },
        { }
    };

    static PyType_Slot _type_slots_PlatformDiagnosticTraceInfo[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_PlatformDiagnosticTraceInfo) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_PlatformDiagnosticTraceInfo) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_PlatformDiagnosticTraceInfo) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_PlatformDiagnosticTraceInfo) },
        { },
    };

    static PyType_Spec type_spec_PlatformDiagnosticTraceInfo =
    {
        "_winrt_windows_system_diagnostics_tracereporting.PlatformDiagnosticTraceInfo",
        sizeof(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceInfo),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_PlatformDiagnosticTraceInfo
    };

    // ----- PlatformDiagnosticTraceRuntimeInfo class --------------------
    static constexpr const char* const type_name_PlatformDiagnosticTraceRuntimeInfo = "PlatformDiagnosticTraceRuntimeInfo";

    static PyObject* _new_PlatformDiagnosticTraceRuntimeInfo(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        py::set_invalid_activation_error(type_name_PlatformDiagnosticTraceRuntimeInfo);
        return nullptr;
    }

    static void _dealloc_PlatformDiagnosticTraceRuntimeInfo(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceRuntimeInfo* self) noexcept
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

    static PyObject* PlatformDiagnosticTraceRuntimeInfo_get_EtwRuntimeFileTime(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceRuntimeInfo* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticTraceRuntimeInfo", L"EtwRuntimeFileTime"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.EtwRuntimeFileTime());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* PlatformDiagnosticTraceRuntimeInfo_get_RuntimeFileTime(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceRuntimeInfo* self, void* /*unused*/) noexcept
    {
        if (!winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.System.Diagnostics.TraceReporting.PlatformDiagnosticTraceRuntimeInfo", L"RuntimeFileTime"))
        {
            PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
            return nullptr;
        }

        try
        {
            return py::convert(self->obj.RuntimeFileTime());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_PlatformDiagnosticTraceRuntimeInfo(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceRuntimeInfo>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_PlatformDiagnosticTraceRuntimeInfo(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceRuntimeInfo>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_PlatformDiagnosticTraceRuntimeInfo[] = {
        { "_assign_array_", _assign_array_PlatformDiagnosticTraceRuntimeInfo, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_PlatformDiagnosticTraceRuntimeInfo), METH_O | METH_STATIC, nullptr },
        { }
    };

    static PyGetSetDef _getset_PlatformDiagnosticTraceRuntimeInfo[] = {
        { "etw_runtime_file_time", reinterpret_cast<getter>(PlatformDiagnosticTraceRuntimeInfo_get_EtwRuntimeFileTime), nullptr, nullptr, nullptr },
        { "runtime_file_time", reinterpret_cast<getter>(PlatformDiagnosticTraceRuntimeInfo_get_RuntimeFileTime), nullptr, nullptr, nullptr },
        { }
    };

    static PyType_Slot _type_slots_PlatformDiagnosticTraceRuntimeInfo[] = 
    {
        { Py_tp_new, reinterpret_cast<void*>(_new_PlatformDiagnosticTraceRuntimeInfo) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_PlatformDiagnosticTraceRuntimeInfo) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_PlatformDiagnosticTraceRuntimeInfo) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_PlatformDiagnosticTraceRuntimeInfo) },
        { },
    };

    static PyType_Spec type_spec_PlatformDiagnosticTraceRuntimeInfo =
    {
        "_winrt_windows_system_diagnostics_tracereporting.PlatformDiagnosticTraceRuntimeInfo",
        sizeof(py::wrapper::Windows::System::Diagnostics::TraceReporting::PlatformDiagnosticTraceRuntimeInfo),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_PlatformDiagnosticTraceRuntimeInfo
    };

    // ----- Windows.System.Diagnostics.TraceReporting Initialization --------------------
    PyDoc_STRVAR(module_doc, "Windows::System::Diagnostics::TraceReporting");


    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_system_diagnostics_tracereporting",
           module_doc,
           0,
           nullptr,
           nullptr,
           nullptr,
           nullptr,
           nullptr};

} // py::cpp::Windows::System::Diagnostics::TraceReporting

PyMODINIT_FUNC PyInit__winrt_windows_system_diagnostics_tracereporting(void) noexcept
{
    using namespace py::cpp::Windows::System::Diagnostics::TraceReporting;

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
    if (py::register_python_type(module.get(), type_name_PlatformDiagnosticActions, &type_spec_PlatformDiagnosticActions, nullptr, object_bases.get(), nullptr) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_PlatformDiagnosticActions, &type_spec_PlatformDiagnosticActions, object_bases.get(), nullptr) == -1)
    #endif
    {
        return nullptr;
    }

    #if PY_VERSION_HEX < 0x03090000
    if (py::register_python_type(module.get(), type_name_PlatformDiagnosticTraceInfo, &type_spec_PlatformDiagnosticTraceInfo, nullptr, object_bases.get(), nullptr) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_PlatformDiagnosticTraceInfo, &type_spec_PlatformDiagnosticTraceInfo, object_bases.get(), nullptr) == -1)
    #endif
    {
        return nullptr;
    }

    #if PY_VERSION_HEX < 0x03090000
    if (py::register_python_type(module.get(), type_name_PlatformDiagnosticTraceRuntimeInfo, &type_spec_PlatformDiagnosticTraceRuntimeInfo, nullptr, object_bases.get(), nullptr) == -1)
    #else
    if (py::register_python_type(module.get(), type_name_PlatformDiagnosticTraceRuntimeInfo, &type_spec_PlatformDiagnosticTraceRuntimeInfo, object_bases.get(), nullptr) == -1)
    #endif
    {
        return nullptr;
    }


    return module.detach();
}