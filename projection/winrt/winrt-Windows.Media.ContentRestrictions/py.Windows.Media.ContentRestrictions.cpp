// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Windows.Media.ContentRestrictions.h"

namespace py::cpp::Windows::Media::ContentRestrictions
{
    // ----- ContentRestrictionsBrowsePolicy class --------------------

    static PyObject* _new_ContentRestrictionsBrowsePolicy(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy>::type_name);
        return nullptr;
    }

    static void _dealloc_ContentRestrictionsBrowsePolicy(py::wrapper::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* ContentRestrictionsBrowsePolicy_get_GeographicRegion(py::wrapper::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.ContentRestrictionsBrowsePolicy", L"GeographicRegion");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.GeographicRegion();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ContentRestrictionsBrowsePolicy_get_MaxBrowsableAgeRating(py::wrapper::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.ContentRestrictionsBrowsePolicy", L"MaxBrowsableAgeRating");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.MaxBrowsableAgeRating();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ContentRestrictionsBrowsePolicy_get_PreferredAgeRating(py::wrapper::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.ContentRestrictionsBrowsePolicy", L"PreferredAgeRating");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.PreferredAgeRating();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_ContentRestrictionsBrowsePolicy(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_ContentRestrictionsBrowsePolicy(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_ContentRestrictionsBrowsePolicy[] = {
        { "_assign_array_", _assign_array_ContentRestrictionsBrowsePolicy, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_ContentRestrictionsBrowsePolicy), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_ContentRestrictionsBrowsePolicy[] = {
        { "geographic_region", reinterpret_cast<getter>(ContentRestrictionsBrowsePolicy_get_GeographicRegion), nullptr, nullptr, nullptr },
        { "max_browsable_age_rating", reinterpret_cast<getter>(ContentRestrictionsBrowsePolicy_get_MaxBrowsableAgeRating), nullptr, nullptr, nullptr },
        { "preferred_age_rating", reinterpret_cast<getter>(ContentRestrictionsBrowsePolicy_get_PreferredAgeRating), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_ContentRestrictionsBrowsePolicy[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_ContentRestrictionsBrowsePolicy) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_ContentRestrictionsBrowsePolicy) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_ContentRestrictionsBrowsePolicy) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_ContentRestrictionsBrowsePolicy) },
        { }};

    static PyType_Spec type_spec_ContentRestrictionsBrowsePolicy = {
        "winrt._winrt_windows_media_contentrestrictions.ContentRestrictionsBrowsePolicy",
        sizeof(py::wrapper::Windows::Media::ContentRestrictions::ContentRestrictionsBrowsePolicy),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_ContentRestrictionsBrowsePolicy};

    // ----- RatedContentDescription class --------------------

    static PyObject* _new_RatedContentDescription(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
    {
        if (kwds)
        {
            py::set_invalid_kwd_args_error();
            return nullptr;
        }

        auto arg_count = PyTuple_GET_SIZE(args);
        if (arg_count == 3)
        {
            try
            {
                auto param0 = py::convert_to<winrt::hstring>(args, 0);
                auto param1 = py::convert_to<winrt::hstring>(args, 1);
                auto param2 = py::convert_to<winrt::Windows::Media::ContentRestrictions::RatedContentCategory>(args, 2);

                winrt::Windows::Media::ContentRestrictions::RatedContentDescription instance{param0, param1, param2};
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

    static void _dealloc_RatedContentDescription(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* RatedContentDescription_get_Title(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.RatedContentDescription", L"Title");
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

    static int RatedContentDescription_put_Title(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.RatedContentDescription", L"Title");
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

    static PyObject* RatedContentDescription_get_Ratings(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.RatedContentDescription", L"Ratings");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Ratings();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int RatedContentDescription_put_Ratings(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.RatedContentDescription", L"Ratings");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Windows::Foundation::Collections::IVector<winrt::hstring>>(arg);

            {
                auto _gil = release_gil();
                self->obj.Ratings(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* RatedContentDescription_get_Image(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.RatedContentDescription", L"Image");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Image();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int RatedContentDescription_put_Image(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.RatedContentDescription", L"Image");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Windows::Storage::Streams::IRandomAccessStreamReference>(arg);

            {
                auto _gil = release_gil();
                self->obj.Image(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* RatedContentDescription_get_Id(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.RatedContentDescription", L"Id");
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

    static int RatedContentDescription_put_Id(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.RatedContentDescription", L"Id");
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

    static PyObject* RatedContentDescription_get_Category(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.RatedContentDescription", L"Category");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Category();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static int RatedContentDescription_put_Category(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription* self, PyObject* arg, void* /*unused*/) noexcept
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
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Media.ContentRestrictions.RatedContentDescription", L"Category");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return -1;
            }

            auto param0 = py::convert_to<winrt::Windows::Media::ContentRestrictions::RatedContentCategory>(arg);

            {
                auto _gil = release_gil();
                self->obj.Category(param0);
            }

            return 0;
        }
        catch (...)
        {
            py::to_PyErr();
            return -1;
        }
    }

    static PyObject* _assign_array_RatedContentDescription(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Media::ContentRestrictions::RatedContentDescription>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_RatedContentDescription(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Media::ContentRestrictions::RatedContentDescription>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_RatedContentDescription[] = {
        { "_assign_array_", _assign_array_RatedContentDescription, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_RatedContentDescription), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_RatedContentDescription[] = {
        { "title", reinterpret_cast<getter>(RatedContentDescription_get_Title), reinterpret_cast<setter>(RatedContentDescription_put_Title), nullptr, nullptr },
        { "ratings", reinterpret_cast<getter>(RatedContentDescription_get_Ratings), reinterpret_cast<setter>(RatedContentDescription_put_Ratings), nullptr, nullptr },
        { "image", reinterpret_cast<getter>(RatedContentDescription_get_Image), reinterpret_cast<setter>(RatedContentDescription_put_Image), nullptr, nullptr },
        { "id", reinterpret_cast<getter>(RatedContentDescription_get_Id), reinterpret_cast<setter>(RatedContentDescription_put_Id), nullptr, nullptr },
        { "category", reinterpret_cast<getter>(RatedContentDescription_get_Category), reinterpret_cast<setter>(RatedContentDescription_put_Category), nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_RatedContentDescription[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_RatedContentDescription) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_RatedContentDescription) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_RatedContentDescription) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_RatedContentDescription) },
        { }};

    static PyType_Spec type_spec_RatedContentDescription = {
        "winrt._winrt_windows_media_contentrestrictions.RatedContentDescription",
        sizeof(py::wrapper::Windows::Media::ContentRestrictions::RatedContentDescription),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_RatedContentDescription};

    // ----- RatedContentRestrictions class --------------------

    static PyObject* _new_RatedContentRestrictions(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
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
                auto param0 = py::convert_to<uint32_t>(args, 0);

                winrt::Windows::Media::ContentRestrictions::RatedContentRestrictions instance{param0};
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
                winrt::Windows::Media::ContentRestrictions::RatedContentRestrictions instance{};
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

    static void _dealloc_RatedContentRestrictions(py::wrapper::Windows::Media::ContentRestrictions::RatedContentRestrictions* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* RatedContentRestrictions_GetBrowsePolicyAsync(py::wrapper::Windows::Media::ContentRestrictions::RatedContentRestrictions* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Media.ContentRestrictions.RatedContentRestrictions", L"GetBrowsePolicyAsync", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.GetBrowsePolicyAsync();
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

    static PyObject* RatedContentRestrictions_GetRestrictionLevelAsync(py::wrapper::Windows::Media::ContentRestrictions::RatedContentRestrictions* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Media.ContentRestrictions.RatedContentRestrictions", L"GetRestrictionLevelAsync", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Media::ContentRestrictions::RatedContentDescription>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.GetRestrictionLevelAsync(param0);
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

    static PyObject* RatedContentRestrictions_RequestContentAccessAsync(py::wrapper::Windows::Media::ContentRestrictions::RatedContentRestrictions* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Windows.Media.ContentRestrictions.RatedContentRestrictions", L"RequestContentAccessAsync", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Media::ContentRestrictions::RatedContentDescription>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.RequestContentAccessAsync(param0);
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

    static PyObject* RatedContentRestrictions_add_RestrictionsChanged(py::wrapper::Windows::Media::ContentRestrictions::RatedContentRestrictions* self, PyObject* arg) noexcept
    {
        try
        {
            static std::optional<bool> is_event_present{};

            if (!is_event_present.has_value())
            {
                is_event_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.Media.ContentRestrictions.RatedContentRestrictions", L"RestrictionsChanged");
            }

            if (!is_event_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
                return nullptr;
            }

            auto param0 = py::convert_to<winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable>>(arg);

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.RestrictionsChanged(param0);
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* RatedContentRestrictions_remove_RestrictionsChanged(py::wrapper::Windows::Media::ContentRestrictions::RatedContentRestrictions* self, PyObject* arg) noexcept
    {
        try
        {
            static std::optional<bool> is_event_present{};

            if (!is_event_present.has_value())
            {
                is_event_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(L"Windows.Media.ContentRestrictions.RatedContentRestrictions", L"RestrictionsChanged");
            }

            if (!is_event_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "event is not available in this version of Windows");
                return nullptr;
            }

            auto param0 = py::convert_to<winrt::event_token>(arg);

            {
                auto _gil = release_gil();
                self->obj.RestrictionsChanged(param0);
            }

            Py_RETURN_NONE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_RatedContentRestrictions(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Windows::Media::ContentRestrictions::RatedContentRestrictions>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_RatedContentRestrictions(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Windows::Media::ContentRestrictions::RatedContentRestrictions>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_RatedContentRestrictions[] = {
        { "get_browse_policy_async", reinterpret_cast<PyCFunction>(RatedContentRestrictions_GetBrowsePolicyAsync), METH_VARARGS, nullptr },
        { "get_restriction_level_async", reinterpret_cast<PyCFunction>(RatedContentRestrictions_GetRestrictionLevelAsync), METH_VARARGS, nullptr },
        { "request_content_access_async", reinterpret_cast<PyCFunction>(RatedContentRestrictions_RequestContentAccessAsync), METH_VARARGS, nullptr },
        { "add_restrictions_changed", reinterpret_cast<PyCFunction>(RatedContentRestrictions_add_RestrictionsChanged), METH_O, nullptr },
        { "remove_restrictions_changed", reinterpret_cast<PyCFunction>(RatedContentRestrictions_remove_RestrictionsChanged), METH_O, nullptr },
        { "_assign_array_", _assign_array_RatedContentRestrictions, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_RatedContentRestrictions), METH_O | METH_STATIC, nullptr },
        { }};

    static PyGetSetDef _getset_RatedContentRestrictions[] = {
        { }};

    static PyType_Slot _type_slots_RatedContentRestrictions[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_RatedContentRestrictions) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_RatedContentRestrictions) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_RatedContentRestrictions) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_RatedContentRestrictions) },
        { }};

    static PyType_Spec type_spec_RatedContentRestrictions = {
        "winrt._winrt_windows_media_contentrestrictions.RatedContentRestrictions",
        sizeof(py::wrapper::Windows::Media::ContentRestrictions::RatedContentRestrictions),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_RatedContentRestrictions};

    // ----- Windows.Media.ContentRestrictions Initialization --------------------

    PyDoc_STRVAR(module_doc, "Windows.Media.ContentRestrictions");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winrt_windows_media_contentrestrictions",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Windows::Media::ContentRestrictions

PyMODINIT_FUNC PyInit__winrt_windows_media_contentrestrictions(void) noexcept
{
    using namespace py::cpp::Windows::Media::ContentRestrictions;

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

    py::pytype_handle ContentRestrictionsBrowsePolicy_type{py::register_python_type(module.get(), &type_spec_ContentRestrictionsBrowsePolicy, object_bases.get(), inspectable_meta_type)};
    if (!ContentRestrictionsBrowsePolicy_type)
    {
        return nullptr;
    }

    py::pytype_handle RatedContentDescription_type{py::register_python_type(module.get(), &type_spec_RatedContentDescription, object_bases.get(), inspectable_meta_type)};
    if (!RatedContentDescription_type)
    {
        return nullptr;
    }

    py::pytype_handle RatedContentRestrictions_type{py::register_python_type(module.get(), &type_spec_RatedContentRestrictions, object_bases.get(), inspectable_meta_type)};
    if (!RatedContentRestrictions_type)
    {
        return nullptr;
    }


    return module.detach();
}
