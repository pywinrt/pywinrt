// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.ApplicationModel.DataTransfer.ShareTarget.h"

namespace py::cpp::Windows::ApplicationModel::DataTransfer::ShareTarget
{
    // ----- QuickLink class --------------------

    static PyObject* _new_QuickLink(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
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
                winrt::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink instance{};
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

    static void _dealloc_QuickLink(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* QuickLink_get_Title(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink", L"Title");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Title();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int QuickLink_put_Title(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink", L"Title");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::hstring>(arg);

            {
                auto _gil = release_gil();
                self->obj.Title(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* QuickLink_get_Thumbnail(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink", L"Thumbnail");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Thumbnail();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int QuickLink_put_Thumbnail(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink", L"Thumbnail");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Windows::Storage::Streams::RandomAccessStreamReference>(arg);

            {
                auto _gil = release_gil();
                self->obj.Thumbnail(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* QuickLink_get_Id(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink", L"Id");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Id();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int QuickLink_put_Id(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink", L"Id");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::hstring>(arg);

            {
                auto _gil = release_gil();
                self->obj.Id(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* QuickLink_get_SupportedDataFormats(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink", L"SupportedDataFormats");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.SupportedDataFormats();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* QuickLink_get_SupportedFileTypes(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.QuickLink", L"SupportedFileTypes");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.SupportedFileTypes();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_QuickLink(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_QuickLink(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_QuickLink[] = {
        { "_assign_array_", _assign_array_QuickLink, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_QuickLink), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_QuickLink[] = {
        { "title", reinterpret_cast<getter>(QuickLink_get_Title), reinterpret_cast<setter>(QuickLink_put_Title), nullptr, nullptr },
        { "thumbnail", reinterpret_cast<getter>(QuickLink_get_Thumbnail), reinterpret_cast<setter>(QuickLink_put_Thumbnail), nullptr, nullptr },
        { "id", reinterpret_cast<getter>(QuickLink_get_Id), reinterpret_cast<setter>(QuickLink_put_Id), nullptr, nullptr },
        { "supported_data_formats", reinterpret_cast<getter>(QuickLink_get_SupportedDataFormats), nullptr, nullptr, nullptr },
        { "supported_file_types", reinterpret_cast<getter>(QuickLink_get_SupportedFileTypes), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_QuickLink[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_QuickLink) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_QuickLink) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_QuickLink) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_QuickLink) },
        { }};

    static PyType_Spec type_spec_QuickLink = {
        "winrt._winrt_windows_applicationmodel_datatransfer_sharetarget.QuickLink",
        sizeof(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_QuickLink};

    // ----- ShareOperation class --------------------

    static PyObject* _new_ShareOperation(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation>::type_name);
        return nullptr;
    }

    static void _dealloc_ShareOperation(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* ShareOperation_DismissUI(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"DismissUI", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                {
                    auto _gil = release_gil();
                    self->obj.DismissUI();
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

    static PyObject* ShareOperation_RemoveThisQuickLink(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"RemoveThisQuickLink", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                {
                    auto _gil = release_gil();
                    self->obj.RemoveThisQuickLink();
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

    static PyObject* ShareOperation_ReportCompleted(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"ReportCompleted", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                {
                    auto _gil = release_gil();
                    self->obj.ReportCompleted();
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

    static PyObject* ShareOperation_ReportCompletedWithQuickLink(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"ReportCompleted", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::ApplicationModel::DataTransfer::ShareTarget::QuickLink>(args, 0);

                {
                    auto _gil = release_gil();
                    self->obj.ReportCompleted(param0);
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

    static PyObject* ShareOperation_ReportDataRetrieved(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"ReportDataRetrieved", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                {
                    auto _gil = release_gil();
                    self->obj.ReportDataRetrieved();
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

    static PyObject* ShareOperation_ReportError(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"ReportError", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::hstring>(args, 0);

                {
                    auto _gil = release_gil();
                    self->obj.ReportError(param0);
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

    static PyObject* ShareOperation_ReportStarted(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"ReportStarted", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                {
                    auto _gil = release_gil();
                    self->obj.ReportStarted();
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

    static PyObject* ShareOperation_ReportSubmittedBackgroundTask(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"ReportSubmittedBackgroundTask", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                {
                    auto _gil = release_gil();
                    self->obj.ReportSubmittedBackgroundTask();
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

    static PyObject* ShareOperation_get_Data(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"Data");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Data();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ShareOperation_get_QuickLinkId(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"QuickLinkId");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.QuickLinkId();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ShareOperation_get_Contacts(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.ApplicationModel.DataTransfer.ShareTarget.ShareOperation", L"Contacts");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Contacts();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_ShareOperation(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_ShareOperation(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_ShareOperation[] = {
        { "dismiss_ui", reinterpret_cast<PyCFunction>(ShareOperation_DismissUI), METH_VARARGS, nullptr },
        { "remove_this_quick_link", reinterpret_cast<PyCFunction>(ShareOperation_RemoveThisQuickLink), METH_VARARGS, nullptr },
        { "report_completed", reinterpret_cast<PyCFunction>(ShareOperation_ReportCompleted), METH_VARARGS, nullptr },
        { "report_completed_with_quick_link", reinterpret_cast<PyCFunction>(ShareOperation_ReportCompletedWithQuickLink), METH_VARARGS, nullptr },
        { "report_data_retrieved", reinterpret_cast<PyCFunction>(ShareOperation_ReportDataRetrieved), METH_VARARGS, nullptr },
        { "report_error", reinterpret_cast<PyCFunction>(ShareOperation_ReportError), METH_VARARGS, nullptr },
        { "report_started", reinterpret_cast<PyCFunction>(ShareOperation_ReportStarted), METH_VARARGS, nullptr },
        { "report_submitted_background_task", reinterpret_cast<PyCFunction>(ShareOperation_ReportSubmittedBackgroundTask), METH_VARARGS, nullptr },
        { "_assign_array_", _assign_array_ShareOperation, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_ShareOperation), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_ShareOperation[] = {
        { "data", reinterpret_cast<getter>(ShareOperation_get_Data), nullptr, nullptr, nullptr },
        { "quick_link_id", reinterpret_cast<getter>(ShareOperation_get_QuickLinkId), nullptr, nullptr, nullptr },
        { "contacts", reinterpret_cast<getter>(ShareOperation_get_Contacts), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_ShareOperation[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_ShareOperation) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_ShareOperation) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_ShareOperation) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_ShareOperation) },
        { }};

    static PyType_Spec type_spec_ShareOperation = {
        "winrt._winrt_windows_applicationmodel_datatransfer_sharetarget.ShareOperation",
        sizeof(py::wrapper::Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_ShareOperation};

    // ----- Windows.ApplicationModel.DataTransfer.ShareTarget Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.ApplicationModel.DataTransfer.ShareTarget");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_applicationmodel_datatransfer_sharetarget",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::ApplicationModel::DataTransfer::ShareTarget

PyMODINIT_FUNC PyInit__winrt_windows_applicationmodel_datatransfer_sharetarget(void) noexcept
{
    using namespace py::cpp::Windows::ApplicationModel::DataTransfer::ShareTarget;

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

    py::pytype_handle QuickLink_type{py::register_python_type(module.get(), &type_spec_QuickLink, object_bases.get(), inspectable_meta_type)};
    if (!QuickLink_type)
    {
        return nullptr;
    }

    py::pytype_handle ShareOperation_type{py::register_python_type(module.get(), &type_spec_ShareOperation, object_bases.get(), inspectable_meta_type)};
    if (!ShareOperation_type)
    {
        return nullptr;
    }


    return module.detach();
}
