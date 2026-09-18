// What a projected member is as an attribute of the type it belongs to.
//
// A method is a descriptor object with a vectorcall entry, so that calling one
// builds neither a bound method nor an argument tuple; a property is a getset
// whose closure is the member it stands for. Either way the descriptor carries
// what the call needs, which is why interp.cpp has nothing left to look up.
//
// The other half of the file is where those descriptors come from: one pass
// over a type's groups in the table turns each of them into a member_desc, its
// overloads and its arguments, in three allocations for the whole type.

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "interp.h"
#include "members.h"
#include "module_state.h"
#include "objects.h"
#include "types.h"

namespace py::interp
{
    namespace
    {
        /**
         * What a projected method is as an attribute of its type.
         *
         * It is a method descriptor with a vectorcall entry, which is what
         * keeps obj.method(...) from building a bound method object and an
         * argument tuple on every call. CPython hands such a descriptor the
         * instance as its first argument, so there is nothing to unpack: the
         * member is right here and the object is args[0].
         */
        struct method_object
        {
            PyObject_HEAD
            vectorcallfunc vectorcall;
            member_desc* member;
            PyObject* name;
            /// Borrowed: the type owns this descriptor, not the other way
            /// round.
            PyTypeObject* objclass;
        };

        PyObject* method_call(
            PyObject* callable,
            PyObject* const* args,
            size_t nargsf,
            PyObject* kwnames) noexcept
        {
            auto const self = reinterpret_cast<method_object*>(callable);

            if (kwnames && PyTuple_GET_SIZE(kwnames) != 0)
            {
                set_invalid_kwd_args_error();
                return nullptr;
            }

            auto const nargs = PyVectorcall_NARGS(nargsf);
            if (nargs < 1)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "descriptor '%U' of '%s' object needs an argument",
                    self->name,
                    self->objclass->tp_name);
                return nullptr;
            }

            auto const instance = args[0];
            if (!PyObject_TypeCheck(instance, self->objclass))
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "descriptor '%U' for '%s' objects doesn't apply to a '%s' object",
                    self->name,
                    self->objclass->tp_name,
                    Py_TYPE(instance)->tp_name);
                return nullptr;
            }

            auto const overload = select_overload(*self->member, nargs - 1);
            if (!overload)
            {
                return nullptr;
            }

            return call_member(
                *self->member,
                *overload,
                self->member->is_static ? nullptr : abi_of(instance),
                args + 1,
                nargs - 1);
        }

        PyObject* method_descr_get(
            PyObject* self, PyObject* obj, PyObject* /*type*/) noexcept
        {
            if (!obj)
            {
                return Py_NewRef(self);
            }

            return PyMethod_New(self, obj);
        }

        void method_dealloc(PyObject* self) noexcept
        {
            auto const tp = Py_TYPE(self);

            Py_CLEAR(reinterpret_cast<method_object*>(self)->name);
            tp->tp_free(self);
            Py_DECREF(tp);
        }

        PyObject* method_repr(PyObject* self) noexcept
        {
            auto const descr = reinterpret_cast<method_object*>(self);

            return PyUnicode_FromFormat(
                "<method '%U' of '%s' objects>", descr->name, descr->objclass->tp_name);
        }

        PyObject* method_get_name(PyObject* self, void* /*unused*/) noexcept
        {
            return Py_NewRef(reinterpret_cast<method_object*>(self)->name);
        }

        PyObject* method_get_qualname(PyObject* self, void* /*unused*/) noexcept
        {
            auto const descr = reinterpret_cast<method_object*>(self);

            return PyUnicode_FromFormat("%s.%U", descr->objclass->tp_name, descr->name);
        }

        PyObject* method_get_objclass(PyObject* self, void* /*unused*/) noexcept
        {
            return Py_NewRef(
                reinterpret_cast<PyObject*>(
                    reinterpret_cast<method_object*>(self)->objclass));
        }

        PyMemberDef method_members[]
            = {{"__vectorcalloffset__",
                T_PYSSIZET,
                offsetof(method_object, vectorcall),
                READONLY,
                nullptr},
               {}};

        PyGetSetDef method_getset[]
            = {{"__name__", method_get_name, nullptr, nullptr, nullptr},
               {"__qualname__", method_get_qualname, nullptr, nullptr, nullptr},
               {"__objclass__", method_get_objclass, nullptr, nullptr, nullptr},
               {}};

        PyType_Slot method_type_slots[]
            = {{Py_tp_dealloc, reinterpret_cast<void*>(method_dealloc)},
               {Py_tp_repr, reinterpret_cast<void*>(method_repr)},
               {Py_tp_call, reinterpret_cast<void*>(PyVectorcall_Call)},
               {Py_tp_descr_get, reinterpret_cast<void*>(method_descr_get)},
               {Py_tp_members, reinterpret_cast<void*>(method_members)},
               {Py_tp_getset, reinterpret_cast<void*>(method_getset)},
               {}};
    } // namespace

    /**
     * The type a projected method is bound as. Created by the _winrt module and
     * kept in its state.
     */
    PyType_Spec projected_method_type_spec
        = {"winrt._winrt.projected_method",
           sizeof(method_object),
           0,
           Py_TPFLAGS_DEFAULT | Py_TPFLAGS_METHOD_DESCRIPTOR
               | Py_TPFLAGS_HAVE_VECTORCALL | Py_TPFLAGS_IMMUTABLETYPE,
           method_type_slots};

    namespace
    {
        /**
         * Makes the attribute that stands for @p member on @p objclass.
         */
        pyobj_handle make_method(member_desc& member, PyTypeObject* objclass) noexcept
        {
            auto const s = py::cpp::_winrt::get_module_state();
            if (!s)
            {
                PyErr_SetString(PyExc_SystemError, "winrt-runtime is not loaded");
                return pyobj_handle{};
            }

            pyobj_handle self{
                s->projected_method_type->tp_alloc(s->projected_method_type, 0)};
            if (!self)
            {
                return self;
            }

            auto const descr = reinterpret_cast<method_object*>(self.get());
            descr->vectorcall = method_call;
            descr->member = &member;
            descr->name = Py_NewRef(member.py_name);
            descr->objclass = objclass;

            return self;
        }

        PyObject* property_get(PyObject* self, void* closure) noexcept
        {
            auto const member = static_cast<member_desc*>(closure);
            auto const overload = select_overload(*member, 0);
            if (!overload)
            {
                return nullptr;
            }

            return call_member(
                *member,
                *overload,
                member->is_static ? nullptr : abi_of(self),
                nullptr,
                0);
        }

        int property_set(PyObject* self, PyObject* value, void* closure) noexcept
        {
            auto const member = static_cast<member_desc*>(closure);

            if (!value)
            {
                PyErr_Format(
                    PyExc_AttributeError, "cannot delete '%U'", member->py_name);
                return -1;
            }

            auto const overload = select_overload(*member, 1);
            if (!overload)
            {
                return -1;
            }

            pyobj_handle result{call_member(
                *member,
                *overload,
                member->is_static ? nullptr : abi_of(self),
                &value,
                1)};

            return result ? 0 : -1;
        }
    } // namespace

    // ----- turning a table's groups into descriptors -----------------------

    namespace
    {
        bool same_guid(void const* left, void const* right) noexcept
        {
            if (!left || !right)
            {
                return false;
            }

            return left == right || std::memcmp(left, right, sizeof(winrt::guid)) == 0;
        }

        /**
         * Fills in one way of calling a member.
         */
        bool build_overload(
            projection& proj,
            type_entry& type,
            bool composable,
            table::member_view const& member,
            bool is_factory_call,
            overload_desc& overload,
            arg_desc* args)
        {
            overload.shape = get_forward_shape(member.forward_shape());

            if (!overload.shape)
            {
                return false;
            }

            overload.slot = member.slot();
            overload.winrt_name = member.winrt_name().data();
            overload.in_count = static_cast<uint16_t>(member.in_count());
            overload.out_count = static_cast<uint16_t>(member.out_count());
            overload.arg_count = static_cast<uint16_t>(member.param_count());
            overload.args = args;

            auto const declaring = member.declaring();

            if (is_factory_call)
            {
                // A static or a constructor is reached through an activation
                // factory rather than through an instance, and the interface
                // the table names is the one the factory is asked for. Plain
                // activation names none, which means IActivationFactory itself.
                if (declaring == table::no_ref)
                {
                    overload.iface = &winrt::guid_of<
                        winrt::Windows::Foundation::IActivationFactory>();
                    overload.iface_name = "Windows.Foundation.IActivationFactory";
                }
                else
                {
                    auto const record = proj.table->type(declaring);
                    overload.iface = record.guid();
                    overload.iface_name = keep(proj, qualified(record));
                }
            }
            else if (declaring != table::no_ref)
            {
                auto const record = proj.table->type(declaring);
                overload.iface_name = keep(proj, qualified(record));

                // A wrapper holds a pointer to one interface: the interface
                // itself, or a class's default interface. Anything else has to
                // be queried for. So does everything on a composable class,
                // because an instance of a class derived from it holds a
                // pointer to *its* default interface rather than to this one.
                if (composable || !same_guid(record.guid(), type.guid))
                {
                    overload.iface = record.guid();
                }
            }

            // Where each argument sits is what the shape says, and an array
            // parameter is two of them: the count and the pointer.
            uint32_t slot = 0;
            uint32_t cursor = 0;
            uint32_t widest = 1;
            auto measured = true;

            for (uint32_t i = 0; i < member.param_count(); i++)
            {
                auto const param = member.param(i);
                auto& arg = args[i];

                arg.code = param.code();
                arg.category = param.get_category();
                arg.is_return = param.is_return_value();
                arg.is_implicit = param.is_implicit();
                arg.by_reference = param.is_by_reference();
                arg.type = param.type();

                // Only a struct and a GUID are ever declared `ref`, and the
                // conversion knows how to write those two somewhere other than
                // the argument slot. Anything else would be written into the
                // slot and the pointer left dangling.
                if (arg.by_reference && arg.code != table::type_code::struct_
                    && arg.code != table::type_code::guid)
                {
                    PyErr_Format(
                        PyExc_ImportError,
                        "'%s' passes a '%s' by reference, which is not interpreted",
                        overload.winrt_name,
                        param.name().data());
                    return false;
                }

                if (slot >= overload.shape->count)
                {
                    PyErr_Format(
                        PyExc_ImportError,
                        "'%s' takes more arguments than its call shape has",
                        overload.winrt_name);
                    return false;
                }

                arg.offset = overload.shape->offsets[slot];
                slot += arg.category == table::param_category::in
                                || arg.category == table::param_category::out
                            ? 1
                            : 2;

                if (!needs_storage(arg))
                {
                    continue;
                }

                if (arg.code == table::type_code::struct_)
                {
                    // A struct from a package that has not been imported yet
                    // cannot be measured here, so the whole block is laid out
                    // again on the first call instead.
                    measured = false;
                    continue;
                }

                auto const layout = table::get_value_layout(arg.code);

                cursor = align_up(cursor, layout.align);
                arg.out_offset = static_cast<uint16_t>(cursor);
                cursor += layout.size;
                widest = std::max(widest, layout.align);
            }

            overload.prepared = measured;
            overload.out_size
                = measured ? static_cast<uint16_t>(align_up(cursor, widest)) : 0;

            // Plain activation hands back the object itself rather than its
            // default interface, so what comes back has to be queried for the
            // interface the wrapper holds.
            if (is_factory_call && declaring == table::no_ref && overload.arg_count == 1
                && args[0].code == table::type_code::object)
            {
                args[0].code = table::type_code::class_;
                args[0].type = type.index;
                args[0].info = &type;
                args[0].query_interface = true;
            }

            return true;
        }

        /**
         * Fills in one Python attribute.
         */
        bool build_member(
            projection& proj,
            type_entry& type,
            bool composable,
            table::group_view const& group,
            uint32_t first,
            uint32_t count,
            PyObject* name,
            member_desc& member,
            overload_desc* overloads,
            arg_desc*& args)
        {
            member.owner = &proj;
            member.declaring = &type;
            member.type_name = type.winrt_name;
            member.py_name = name;
            member.kind = group.kind();
            member.is_static = (group.flags() & table::group_flags::is_static) != 0;
            member.deprecated = (group.flags() & table::group_flags::deprecated) != 0;
            member.count = static_cast<uint16_t>(count);
            member.overloads = overloads;

            auto const is_factory_call
                = member.is_static || member.kind == table::group_kind::constructor;

            for (uint32_t i = 0; i < count; i++)
            {
                auto const record = group.member(first + i);

                if (!build_overload(
                        proj,
                        type,
                        composable,
                        record,
                        is_factory_call,
                        overloads[i],
                        args))
                {
                    return false;
                }

                args += record.param_count();
            }

            return true;
        }
    } // namespace

    /**
     * Builds the descriptors of every attribute of one type.
     */
    bool collect_members(
        projection& proj,
        type_entry& entry,
        table::type_view const& record,
        type_members& collected)
    {
        // Every descriptor of one type comes out of one block each, so that
        // a type costs three allocations however many members it has.
        uint32_t members = 0;
        uint32_t overloads = 0;
        uint32_t arguments = 0;

        for (uint32_t i = 0; i < record.group_count(); i++)
        {
            auto const group = record.group(i);

            members += group.kind() == table::group_kind::event ? 2 : 1;
            overloads += group.member_count();

            for (uint32_t j = 0; j < group.member_count(); j++)
            {
                arguments += group.member(j).param_count();
            }
        }

        if (members == 0)
        {
            return true;
        }

        proj.members.push_back(std::make_unique<member_desc[]>(members));
        proj.overloads.push_back(std::make_unique<overload_desc[]>(overloads));
        proj.args.push_back(std::make_unique<arg_desc[]>(arguments));

        auto* member = proj.members.back().get();
        auto* overload = proj.overloads.back().get();
        auto* argument = proj.args.back().get();

        auto const composable = (record.flags() & table::type_flags::composable) != 0;

        for (uint32_t i = 0; i < record.group_count(); i++)
        {
            auto const group = record.group(i);
            auto const is_static = (group.flags() & table::group_flags::is_static) != 0;

            if (group.kind() == table::group_kind::event)
            {
                // An event is two methods, add_x() and remove_x(), which is
                // how the projection has always spelled one.
                static constexpr char const* const prefixes[] = {"add_", "remove_"};

                for (uint32_t half = 0; half < 2 && half < group.member_count(); half++)
                {
                    auto const name = keep(
                        proj,
                        std::string{prefixes[half]} + std::string{group.py_name()});

                    pyobj_handle interned{PyUnicode_InternFromString(name)};
                    if (!interned)
                    {
                        return false;
                    }

                    proj.names.push_back(interned.get());

                    if (!build_member(
                            proj,
                            entry,
                            composable,
                            group,
                            half,
                            1,
                            interned.detach(),
                            *member,
                            overload,
                            argument))
                    {
                        return false;
                    }

                    collected.methods.emplace_back(member, is_static);
                    member++;
                    overload++;
                }

                continue;
            }

            pyobj_handle interned{PyUnicode_InternFromString(group.py_name().data())};
            if (!interned)
            {
                return false;
            }

            proj.names.push_back(interned.get());

            if (!build_member(
                    proj,
                    entry,
                    composable,
                    group,
                    0,
                    group.member_count(),
                    interned.detach(),
                    *member,
                    overload,
                    argument))
            {
                return false;
            }

            switch (group.kind())
            {
            case table::group_kind::constructor:
                entry.constructor = member;
                break;
            case table::group_kind::property:
            {
                PyGetSetDef def{};
                def.name = group.py_name().data();
                def.get = property_get;
                def.closure = member;

                for (uint32_t j = 0; j < group.member_count(); j++)
                {
                    if (group.member(j).kind() == table::member_kind::property_put)
                    {
                        def.set = property_set;
                    }
                }

                (is_static ? collected.static_getsets : collected.instance_getsets)
                    .push_back(def);

                break;
            }
            default:
                collected.methods.emplace_back(member, is_static);
                break;
            }

            member++;
            overload += group.member_count();
        }

        return true;
    }

    /**
     * Sets each method of a type as an attribute of the type, or of its
     * metaclass when it is static.
     */
    bool bind_methods(
        type_members& collected, PyTypeObject* instance_type, PyTypeObject* statics)
    {
        for (auto const& [member, is_static] : collected.methods)
        {
            auto* const target = is_static ? statics : instance_type;
            if (!target)
            {
                PyErr_Format(
                    PyExc_ImportError,
                    "'%s' has no type to bind '%U' to",
                    member->type_name,
                    member->py_name);
                return false;
            }

            pyobj_handle bound{make_method(*member, target)};
            if (!bound)
            {
                return false;
            }

            if (PyObject_SetAttr(
                    reinterpret_cast<PyObject*>(target), member->py_name, bound.get())
                == -1)
            {
                return false;
            }
        }

        return true;
    }
} // namespace py::interp
