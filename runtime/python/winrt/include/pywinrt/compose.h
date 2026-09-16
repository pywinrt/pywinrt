// The WinRT side of a Python subclass of a composable type.
//
// A composable WinRT class can be derived from, and the derived object has to
// answer WinRT calls itself: py::py_obj_ref holds the Python object and the
// interface tearoffs it implements, py::ImplementsInterface is one such tearoff,
// and py::make_py_obj walks the Python type's bases when the object is
// constructed to collect them.
//
// The reference between the two objects is a toggle rather than a plain count:
// while WinRT holds a reference the Python object must not be collected, and
// when it does not, the pair has to be collectable as a cycle.

#pragma once

#include <pywinrt/convert.h>
#include <pywinrt/handles.h>
#include <pywinrt/prelude.h>
#include <pywinrt/wrappers.h>

namespace py
{
    struct ImplementsInterface;
    static inline void copy_to_abi(ImplementsInterface* obj, void*& abi) noexcept;

    /**
     * Base class for WinRT type wrapping Python subclass of composable type.
     */
    struct py_obj_ref
    {
      private:
        PyObject* py_obj;
        std::map<winrt::guid, std::unique_ptr<ImplementsInterface>> interfaces;
        std::vector<winrt::guid> interface_guids{};

      public:
        void add_interface(
            winrt::guid const& guid, std::unique_ptr<ImplementsInterface> iface)
        {
            interfaces.emplace(guid, std::move(iface));
            interface_guids.push_back(guid);
        }

      protected:
        py_obj_ref() = delete;

        py_obj_ref(PyObject* py_obj) : py_obj{py_obj}
        {
        }

        PyObject* get_py_obj() noexcept
        {
            return Py_NewRef(py_obj);
        }

        static void toggle_reference(py_obj_ref* obj, bool is_last_reference) noexcept
        {
            auto state = PyGILState_Ensure();

            if (is_last_reference)
            {
                // We hold the only WinRT reference - allow the Python object
                // to be GC'd
                PyObject_GC_Track(obj->py_obj);
                // This might be the last reference to the Python object, so
                // obj may be destroyed after this call and no longer valid!
                Py_DECREF(obj->py_obj);
            }
            else
            {
                // external WinRT code has a reference - don't allow Python
                // object to be GC'd
                Py_INCREF(obj->py_obj);
                PyObject_GC_UnTrack(obj->py_obj);
            }

            PyGILState_Release(state);
        }

        int32_t query_interface_tearoff(
            winrt::guid const& guid, void** obj) const noexcept
        {

            auto match = interfaces.find(guid);
            if (match != interfaces.end())
            {
                copy_to_abi(match->second.get(), *obj);
                return 0;
            }

            return winrt::impl::error_no_interface;
        }

        std::vector<winrt::guid> get_iids_tearoff() const noexcept
        {
            return interface_guids;
        }
    };

    struct ImplementsInterface
    {
      private:
        PyObject* py_obj;
        winrt::impl::inspectable_abi* runtime_class;
        void* interface_abi;

      public:
        ImplementsInterface() = delete;

        ImplementsInterface(
            PyObject* py_obj,
            winrt::impl::inspectable_abi* runtime_class,
            void* interface_abi)
            : py_obj{py_obj}, runtime_class(runtime_class), interface_abi(interface_abi)
        {
        }

        void* get_abi() const noexcept
        {
            return interface_abi;
        }

        int32_t __stdcall QueryInterface(winrt::guid const& id, void** object) noexcept
        {
            return runtime_class->QueryInterface(id, object);
        }

        uint32_t __stdcall AddRef() noexcept
        {
            return runtime_class->AddRef();
        }

        uint32_t __stdcall Release() noexcept
        {
            return runtime_class->Release();
        }

        int32_t __stdcall GetIids(
            winrt::impl::count_type* count, winrt::impl::guid_type** array) noexcept
        {
            return runtime_class->GetIids(
                reinterpret_cast<uint32_t*>(count),
                reinterpret_cast<winrt::guid**>(array));
        }

        int32_t __stdcall abi_GetRuntimeClassName(void** name) noexcept
        {
            return runtime_class->GetRuntimeClassName(name);
        }

        int32_t __stdcall abi_GetTrustLevel(
            winrt::Windows::Foundation::TrustLevel* trustLevel) noexcept
        {
            return runtime_class->GetTrustLevel(trustLevel);
        }

      protected:
        PyObject* get_py_obj() noexcept
        {
            return Py_NewRef(py_obj);
        }
    };

    static inline void copy_to_abi(ImplementsInterface* obj, void*& abi) noexcept
    {
        abi = obj->get_abi();

        if (abi)
        {
            obj->AddRef();
        }
    }

    template<typename D, typename I>
    struct ImplementsInterfaceT : ImplementsInterface, winrt::impl::producers<D, I>
    {
        struct abi_guard
        {
            PyGILState_STATE state;

            abi_guard(D& /*unused*/)
            {
                state = PyGILState_Ensure();
            }

            ~abi_guard()
            {
                PyGILState_Release(state);
            }
        };

        ImplementsInterfaceT() = delete;

        ImplementsInterfaceT(
            PyObject* py_obj, winrt::impl::inspectable_abi* runtime_class)
            : ImplementsInterface(py_obj, runtime_class, winrt::to_abi(this))
        {
        }
    };

    template<typename T>
    static inline T::composable make_py_obj(
        winrt::com_ptr<T> const& obj_impl, PyTypeObject* py_type, PyObject* py_obj)
    {
        auto obj = obj_impl.template as<typename T::composable>();

        pyobj_handle bases_iter{PyObject_GetIter(py_type->tp_bases)};
        if (!bases_iter)
        {
            return nullptr;
        }

        for (;;)
        {
            pyobj_handle base{PyIter_Next(bases_iter.get())};
            if (!base)
            {
                if (PyErr_Occurred())
                {
                    return nullptr;
                }

                break;
            }

            pyobj_handle make{};
            auto ret = PyObject_GetOptionalAttrString(base.get(), "_make_", make.put());
            if (ret == -1)
            {
                return nullptr;
            }
            if (ret == 0)
            {
                continue;
            }

            pyobj_handle iface_obj{PyObject_CallFunction(
                make.get(), "On", py_obj, (*(winrt::impl::inspectable_abi**)&obj))};
            if (!iface_obj)
            {
                return nullptr;
            }

            std::unique_ptr<ImplementsInterface> iface{
                reinterpret_cast<ImplementsInterface*>(
                    PyLong_AsVoidPtr(iface_obj.get()))};
            if (!iface && PyErr_Occurred())
            {
                return nullptr;
            }

            pyobj_handle guid_obj{PyObject_CallMethod(base.get(), "_guid_", nullptr)};
            if (!guid_obj)
            {
                return nullptr;
            }

            auto guid = py::convert_to<winrt::guid>(guid_obj.get());

            obj_impl->add_interface(guid, std::move(iface));
        }

        return std::move(obj);
    }
} // namespace py
