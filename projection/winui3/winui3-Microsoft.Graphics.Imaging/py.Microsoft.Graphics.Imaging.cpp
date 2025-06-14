// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#include "py.Microsoft.Graphics.Imaging.h"

namespace py::cpp::Microsoft::Graphics::Imaging
{
    // ----- ImageBuffer class --------------------

    static PyObject* _new_ImageBuffer(PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        static_assert(py::py_type<winrt::Microsoft::Graphics::Imaging::ImageBuffer>::type_name);
        py::set_invalid_activation_error(py::py_type<winrt::Microsoft::Graphics::Imaging::ImageBuffer>::type_name);
        return nullptr;
    }

    static void _dealloc_ImageBuffer(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self) noexcept
    {
        auto tp = Py_TYPE(self);
        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* ImageBuffer_Close(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Microsoft.Graphics.Imaging.ImageBuffer", L"Close", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                {
                    auto _gil = release_gil();
                    self->obj.Close();
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

    static PyObject* ImageBuffer_CopyToByteArray(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Microsoft.Graphics.Imaging.ImageBuffer", L"CopyToByteArray", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                auto param0 = py::convert_to<py::pybuf_view<uint8_t, true>>(args, 0);

                {
                    auto _gil = release_gil();
                    self->obj.CopyToByteArray(param0);
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

    static PyObject* ImageBuffer_CopyToSoftwareBitmap(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 0)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Microsoft.Graphics.Imaging.ImageBuffer", L"CopyToSoftwareBitmap", 0);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(0);
                    return nullptr;
                }

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return self->obj.CopyToSoftwareBitmap();
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

    static PyObject* ImageBuffer_CreateForBuffer(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 5)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Microsoft.Graphics.Imaging.ImageBuffer", L"CreateForBuffer", 5);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(5);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Storage::Streams::IBuffer>(args, 0);
                auto param1 = py::convert_to<winrt::Microsoft::Graphics::Imaging::ImageBufferPixelFormat>(args, 1);
                auto param2 = py::convert_to<int32_t>(args, 2);
                auto param3 = py::convert_to<int32_t>(args, 3);
                auto param4 = py::convert_to<int32_t>(args, 4);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Microsoft::Graphics::Imaging::ImageBuffer::CreateForBuffer(param0, param1, param2, param3, param4);
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

    static PyObject* ImageBuffer_CreateForSoftwareBitmap(PyObject* /*unused*/, PyObject* args) noexcept
    {
        auto arg_count = PyTuple_GET_SIZE(args);

        if (arg_count == 1)
        {
            try
            {
                static std::optional<bool> is_overload_present{};

                if (!is_overload_present.has_value())
                {
                    is_overload_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(L"Microsoft.Graphics.Imaging.ImageBuffer", L"CreateForSoftwareBitmap", 1);
                }

                if (!is_overload_present.value())
                {
                    py::set_arg_count_version_error(1);
                    return nullptr;
                }

                auto param0 = py::convert_to<winrt::Windows::Graphics::Imaging::SoftwareBitmap>(args, 0);

                return py::convert([&]()
                {
                    auto _gil = release_gil();
                    return winrt::Microsoft::Graphics::Imaging::ImageBuffer::CreateForSoftwareBitmap(param0);
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

    static PyObject* ImageBuffer_get_Buffer(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Graphics.Imaging.ImageBuffer", L"Buffer");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.Buffer();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ImageBuffer_get_PixelFormat(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Graphics.Imaging.ImageBuffer", L"PixelFormat");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.PixelFormat();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ImageBuffer_get_PixelHeight(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Graphics.Imaging.ImageBuffer", L"PixelHeight");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.PixelHeight();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ImageBuffer_get_PixelWidth(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Graphics.Imaging.ImageBuffer", L"PixelWidth");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.PixelWidth();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* ImageBuffer_get_RowStride(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self, void* /*unused*/) noexcept
    {
        try
        {
            static std::optional<bool> is_property_present{};

            if (!is_property_present.has_value())
            {
                is_property_present = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Microsoft.Graphics.Imaging.ImageBuffer", L"RowStride");
            }

            if (!is_property_present.value())
            {
                PyErr_SetString(PyExc_AttributeError, "property is not available in this version of Windows");
                return nullptr;
            }

            return py::convert([&]()
            {
                auto _gil = release_gil();
                return self->obj.RowStride();
            }());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _assign_array_ImageBuffer(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        auto array = std::make_unique<py::ComArray<winrt::Microsoft::Graphics::Imaging::ImageBuffer>>();
        if (!py::cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }
        Py_RETURN_NONE;
    }

    static PyObject* _from_ImageBuffer(PyObject* /*unused*/, PyObject* arg) noexcept
    {
        try
        {
            auto return_value = py::convert_to<winrt::Windows::Foundation::IInspectable>(arg);
            return py::convert(return_value.as<winrt::Microsoft::Graphics::Imaging::ImageBuffer>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* _enter_ImageBuffer(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self, PyObject* /*unused*/) noexcept
    {
        return Py_NewRef(self);
    }

    static PyObject* _exit_ImageBuffer(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer* self, PyObject* /*unused*/) noexcept
    {
        try
        {
            {
                auto _gil = py::release_gil();
                self->obj.Close();
            }

            Py_RETURN_FALSE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef _methods_ImageBuffer[] = {
        { "close", reinterpret_cast<PyCFunction>(ImageBuffer_Close), METH_VARARGS, nullptr },
        { "copy_to_byte_array", reinterpret_cast<PyCFunction>(ImageBuffer_CopyToByteArray), METH_VARARGS, nullptr },
        { "copy_to_software_bitmap", reinterpret_cast<PyCFunction>(ImageBuffer_CopyToSoftwareBitmap), METH_VARARGS, nullptr },
        { "_assign_array_", _assign_array_ImageBuffer, METH_O | METH_STATIC, nullptr },
        { "_from", reinterpret_cast<PyCFunction>(_from_ImageBuffer), METH_O | METH_STATIC, nullptr },
        { "__enter__", reinterpret_cast<PyCFunction>(_enter_ImageBuffer), METH_NOARGS, nullptr },
        { "__exit__", reinterpret_cast<PyCFunction>(_exit_ImageBuffer), METH_VARARGS, nullptr },
        { }};

    static PyGetSetDef _getset_ImageBuffer[] = {
        { "buffer", reinterpret_cast<getter>(ImageBuffer_get_Buffer), nullptr, nullptr, nullptr },
        { "pixel_format", reinterpret_cast<getter>(ImageBuffer_get_PixelFormat), nullptr, nullptr, nullptr },
        { "pixel_height", reinterpret_cast<getter>(ImageBuffer_get_PixelHeight), nullptr, nullptr, nullptr },
        { "pixel_width", reinterpret_cast<getter>(ImageBuffer_get_PixelWidth), nullptr, nullptr, nullptr },
        { "row_stride", reinterpret_cast<getter>(ImageBuffer_get_RowStride), nullptr, nullptr, nullptr },
        { }};

    static PyType_Slot _type_slots_ImageBuffer[] = {
        { Py_tp_new, reinterpret_cast<void*>(_new_ImageBuffer) },
        { Py_tp_dealloc, reinterpret_cast<void*>(_dealloc_ImageBuffer) },
        { Py_tp_methods, reinterpret_cast<void*>(_methods_ImageBuffer) },
        { Py_tp_getset, reinterpret_cast<void*>(_getset_ImageBuffer) },
        { }};

    static PyType_Spec type_spec_ImageBuffer = {
        "winui3._winui3_microsoft_graphics_imaging.ImageBuffer",
        sizeof(py::wrapper::Microsoft::Graphics::Imaging::ImageBuffer),
        0,
        Py_TPFLAGS_DEFAULT,
        _type_slots_ImageBuffer};

    static PyGetSetDef getset_ImageBuffer_Static[] = {
        { }};

    static PyMethodDef methods_ImageBuffer_Static[] = {
        { "create_for_buffer", reinterpret_cast<PyCFunction>(ImageBuffer_CreateForBuffer), METH_VARARGS, nullptr },
        { "create_for_software_bitmap", reinterpret_cast<PyCFunction>(ImageBuffer_CreateForSoftwareBitmap), METH_VARARGS, nullptr },
        { }};

    static PyType_Slot type_slots_ImageBuffer_Static[] = 
    {
        { Py_tp_base, reinterpret_cast<void*>(&PyType_Type) },
        { Py_tp_getset, reinterpret_cast<void*>(getset_ImageBuffer_Static) },
        { Py_tp_methods, reinterpret_cast<void*>(methods_ImageBuffer_Static) },
        { }
    };

    static PyType_Spec type_spec_ImageBuffer_Static = {
        "winui3._winui3_microsoft_graphics_imaging.ImageBuffer_Static",
        static_cast<int>(PyType_Type.tp_basicsize),
        static_cast<int>(PyType_Type.tp_itemsize),
        Py_TPFLAGS_DEFAULT,
        type_slots_ImageBuffer_Static};

    // ----- Microsoft.Graphics.Imaging Initialization --------------------

    PyDoc_STRVAR(module_doc, "Microsoft.Graphics.Imaging");

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_winui3_microsoft_graphics_imaging",
        module_doc,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr};
} // py::cpp::Microsoft::Graphics::Imaging

PyMODINIT_FUNC PyInit__winui3_microsoft_graphics_imaging(void) noexcept
{
    using namespace py::cpp::Microsoft::Graphics::Imaging;

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

    py::pyobj_handle ImageBuffer_Static_bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(inspectable_meta_type))};
    if (!ImageBuffer_Static_bases)
    {
        return nullptr;
    }

    py::pyobj_handle type_ImageBuffer_Static{PyType_FromSpecWithBases(&type_spec_ImageBuffer_Static, ImageBuffer_Static_bases.get())};
    if (!type_ImageBuffer_Static)
    {
        return nullptr;
    }

    py::pytype_handle ImageBuffer_type{py::register_python_type(module.get(), &type_spec_ImageBuffer, object_bases.get(), reinterpret_cast<PyTypeObject*>(type_ImageBuffer_Static.get()))};
    if (!ImageBuffer_type)
    {
        return nullptr;
    }


    return module.detach();
}
