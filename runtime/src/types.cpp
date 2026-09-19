// Loading one namespace's projection table and making its Python types.
//
// A projection package ships a table and no compiled code, so the types its
// __init__.py used to import from an extension module are created here instead,
// straight into the package's own module. This file is the part that is the
// same whatever kind of type it is: mapping the table, keeping track of what
// has been built, and finding a type by name. What a class, an interface, a
// struct or an enum actually is belongs to objects.cpp, structs.cpp and
// enums.cpp, and the members bound to them to members.cpp.
//
// A table is mapped once per process and never unmapped, because the names and
// descriptors built from it point into it and a type outlives the import that
// made it. Everything built from those bytes is a Python object, so it belongs
// to the interpreter that built it and lives in that interpreter's module
// state.

#include <Windows.h>

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "delegates.h"
#include "enums.h"
#include "interp.h"
#include "module_state.h"
#include "objects.h"
#include "structs.h"
#include "types.h"

#include <unordered_map>

namespace py::interp
{
    namespace
    {
        /**
         * The tables that have been mapped, by the path they were mapped from.
         *
         * A table is mapped once per process and never unmapped: the names and
         * descriptors built from it point into it, and a type outlives the
         * import that made it. The map is process wide rather than per
         * interpreter for the same reason - two interpreters that import the
         * same package read the same bytes - while everything built from those
         * bytes belongs to the interpreter that built it.
         */
        struct table_store
        {
            winrt::slim_mutex lock;
            std::unordered_map<std::string, std::unique_ptr<table::file>> by_path;
        };

        table_store& get_table_store() noexcept
        {
            static table_store store;
            return store;
        }

        /**
         * Maps @p path and reads its header.
         *
         * @throws table::format_error if it is not a table this runtime reads.
         */
        table::file const& open_table(std::string const& path)
        {
            auto& store = get_table_store();

            {
                winrt::slim_lock_guard const guard{store.lock};

                auto const it = store.by_path.find(path);
                if (it != store.by_path.end())
                {
                    return *it->second;
                }
            }

            auto const wide_path = winrt::to_hstring(path);

            winrt::file_handle file{CreateFileW(
                wide_path.c_str(),
                GENERIC_READ,
                FILE_SHARE_READ,
                nullptr,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr)};
            if (!file)
            {
                winrt::throw_last_error();
            }

            LARGE_INTEGER size{};

            if (!GetFileSizeEx(file.get(), &size))
            {
                winrt::throw_last_error();
            }

            winrt::handle mapping{
                CreateFileMappingW(file.get(), nullptr, PAGE_READONLY, 0, 0, nullptr)};
            if (!mapping)
            {
                winrt::throw_last_error();
            }

            auto const view = MapViewOfFile(mapping.get(), FILE_MAP_READ, 0, 0, 0);
            if (!view)
            {
                winrt::throw_last_error();
            }

            // The view is deliberately never unmapped, which is also why the
            // handles above may close here.
            auto opened = std::make_unique<table::file>(
                view, static_cast<size_t>(size.QuadPart));

            winrt::slim_lock_guard const guard{store.lock};

            auto const [it, added] = store.by_path.try_emplace(path, std::move(opened));

            return *it->second;
        }

        py::cpp::_winrt::module_state* state() noexcept
        {
            return py::cpp::_winrt::get_module_state();
        }

        using py::cpp::_winrt::state_guard;
    } // namespace

    /**
     * The next offset at or after @p value that @p align allows.
     *
     * Three things are laid out from this one rule, because the table writes
     * none of them down: where a call's outputs sit in the block it receives
     * them in, where a struct's fields sit in its value, and where that value
     * sits in the Python object that wraps it.
     */
    uint32_t align_up(uint32_t value, uint32_t align) noexcept
    {
        return (value + align - 1) / align * align;
    }

    /**
     * Keeps a name the table does not spell out, so that a descriptor can
     * point at it for as long as it lives.
     */
    char const* keep(projection& proj, std::string value)
    {
        return proj.strings.emplace_back(std::move(value)).c_str();
    }

    /// The WinRT name of a type with its namespace in front.
    std::string qualified(table::type_view const& type)
    {
        std::string name{type.winrt_namespace()};
        name += '.';
        name += type.name();

        return name;
    }

    /**
     * Copies a getset array into storage that outlives the type built from
     * it, which is what CPython requires of one.
     */
    PyGetSetDef* keep_getsets(projection& proj, std::vector<PyGetSetDef>& defs)
    {
        proj.getsets.push_back(std::make_unique<PyGetSetDef[]>(defs.size() + 1));

        auto* const array = proj.getsets.back().get();
        std::copy(defs.begin(), defs.end(), array);

        return array;
    }

    /**
     * Keeps a copy of the method array a type is created with, which CPython
     * requires to outlive the type the same way a getset array does.
     *
     * The array is already terminated when it arrives, because a caller may
     * have appended to one that was.
     */
    PyMethodDef* keep_methods(projection& proj, std::vector<PyMethodDef>& defs)
    {
        proj.method_defs.push_back(std::make_unique<PyMethodDef[]>(defs.size()));

        auto* const array = proj.method_defs.back().get();
        std::copy(defs.begin(), defs.end(), array);

        return array;
    }

    /**
     * Records which table record @p type was built from, so that a value of it
     * can be converted by a package that only names it.
     */
    bool remember(type_entry& entry, PyTypeObject* type)
    {
        auto const s = state();
        if (!s)
        {
            PyErr_SetString(PyExc_SystemError, "winrt-runtime is not loaded");
            return false;
        }

        {
            state_guard guard{s->cache_lock};
            s->type_entries[type] = &entry;
        }

        return true;
    }

    /**
     * The type entry of a wrapper type this runtime built, or @c nullptr. An
     * interface is here twice, as the wrapper and as the abstract type.
     */
    type_entry* get_type_entry(PyTypeObject* type) noexcept
    {
        auto const s = state();
        if (!s)
        {
            return nullptr;
        }

        state_guard guard{s->cache_lock};
        auto const it = s->type_entries.find(type);

        return it == s->type_entries.end() ? nullptr : it->second;
    }

    // ----- building the types of a namespace -------------------------------

    /**
     * The Python type of a type reference, whichever package defines it.
     */
    PyTypeObject* ensure_referenced_type(projection& proj, uint32_t index) noexcept
    {
        type_entry* cache{};
        auto const info = resolve(proj, index, cache);

        return info ? info->py_type : nullptr;
    }

    /**
     * Builds everything one record of this namespace's table stands for, and
     * everything it derives from, on the first call.
     *
     * For all but a delegate that is a Python type. A delegate has none - the
     * module binds a typing alias to its name - so what it gets is the vtable
     * a Python callable is handed to WinRT behind, which is why this rather
     * than ensure_type() is what the call path asks for.
     */
    type_entry* ensure_entry(projection& proj, uint32_t index) noexcept
    {
        auto& entry = proj.types[index];
        if (entry.py_type || entry.reverse)
        {
            return &entry;
        }

        auto const record = proj.table->type(index);

        entry.owner = &proj;
        entry.index = index;
        entry.category = record.get_category();

        try
        {
            entry.winrt_name = keep(proj, qualified(record));
            entry.tp_name = proj.module_name + "." + std::string{record.py_name()};

            switch (entry.category)
            {
            case table::category::enum_:
                if (!make_enum_type(proj, entry, record))
                {
                    return nullptr;
                }

                break;
            case table::category::delegate:
                if (!make_delegate_type(proj, entry, record))
                {
                    return nullptr;
                }

                break;
            case table::category::struct_:
                // A struct the projection spells as one integer - an HRESULT,
                // an event token - is a subclass of int and has no fields to
                // lay out.
                if (record.flags() & table::type_flags::integer)
                {
                    if (!make_integer_type(proj, entry))
                    {
                        return nullptr;
                    }

                    break;
                }

                if (!make_struct_type(proj, entry, record))
                {
                    return nullptr;
                }

                break;
            case table::category::interface_:
                if (!make_interface_type(proj, entry, record))
                {
                    return nullptr;
                }

                break;
            case table::category::class_:
                if (!make_class_type(proj, entry, record))
                {
                    return nullptr;
                }

                break;
            }
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }

        return &entry;
    }

    /**
     * The entry of a type that the table names and says nothing else about.
     *
     * An interface that is exclusive to a class is written down as a name and
     * an IID, because the class redeclares every member of it, so there is no
     * Python type to build and nothing here builds one. What such an entry is
     * for is the vtable a WinRT caller enters the interface through, which
     * compose.cpp assembles from the class's members.
     */
    type_entry* ensure_named_entry(projection& proj, uint32_t index) noexcept
    {
        auto& entry = proj.types[index];
        if (entry.winrt_name)
        {
            return &entry;
        }

        auto const record = proj.table->type(index);

        entry.owner = &proj;
        entry.index = index;
        entry.category = record.get_category();
        entry.guid = record.guid();

        try
        {
            entry.winrt_name = keep(proj, qualified(record));
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }

        return &entry;
    }

    /**
     * The Python type of one record of this namespace's table, built on the
     * first call.
     */
    PyTypeObject* ensure_type(projection& proj, uint32_t index) noexcept
    {
        auto const entry = ensure_entry(proj, index);

        return entry ? entry->py_type : nullptr;
    }

    /**
     * Builds whatever @p qualified_name names from the table of the package
     * that defines it, if that table is registered.
     *
     * This is what makes a namespace that refers to itself through another one
     * work: the table is registered before a single type is built, so a type
     * can be reached whatever state its module's __init__.py is in.
     *
     * @returns @c nullptr without a Python error set if no table claims the
     * name, or @c nullptr with one if building it failed.
     */
    type_entry* find_registered_entry(std::string_view qualified_name) noexcept
    {
        auto const s = state();
        if (!s)
        {
            return nullptr;
        }

        auto const dot = qualified_name.find_last_of('.');
        if (dot == std::string_view::npos)
        {
            return nullptr;
        }

        // A loaded projection is never let go of before the state itself is,
        // and what a table says never changes, so the lock is over finding it
        // and nothing else. Building the type is not done holding a lock: it
        // imports the modules of every type it names, which comes back here.
        projection* proj{};

        {
            state_guard guard{s->cache_lock};
            auto const found
                = s->projections.find(std::string{qualified_name.substr(0, dot)});

            if (found == s->projections.end())
            {
                return nullptr;
            }

            proj = found->second.get();
        }

        auto const index = proj->by_py_name.find(qualified_name.substr(dot + 1));
        if (index == proj->by_py_name.end())
        {
            return nullptr;
        }

        return ensure_entry(*proj, index->second);
    }

    /**
     * The Python type @p qualified_name names, from the table of the package
     * that defines it.
     *
     * @returns A borrowed reference, or @c nullptr as
     * find_registered_entry() does.
     */
    PyTypeObject* find_registered_type(std::string_view qualified_name) noexcept
    {
        auto const entry = find_registered_entry(qualified_name);

        return entry ? entry->py_type : nullptr;
    }

    /**
     * The entry for @p qualified_name, importing the package that defines it
     * if nothing has yet.
     *
     * A type reaches the same place through py::get_python_type(), which
     * imports and then caches the Python type it finds. A delegate has no
     * Python type to cache, so it comes here instead.
     *
     * @returns @c nullptr with a Python error set if the name cannot be
     * resolved.
     */
    type_entry* find_defining_entry(std::string_view qualified_name) noexcept
    {
        if (auto const entry = find_registered_entry(qualified_name))
        {
            return entry;
        }

        if (PyErr_Occurred())
        {
            return nullptr;
        }

        auto const dot = qualified_name.find_last_of('.');
        if (dot == std::string_view::npos)
        {
            PyErr_Format(
                PyExc_ValueError,
                "invalid qualified name: %s",
                std::string{qualified_name}.c_str());
            return nullptr;
        }

        pyobj_handle module{
            PyImport_ImportModule(std::string{qualified_name.substr(0, dot)}.c_str())};
        if (!module)
        {
            return nullptr;
        }

        if (auto const entry = find_registered_entry(qualified_name))
        {
            return entry;
        }

        if (!PyErr_Occurred())
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' is not something a projection table describes",
                std::string{qualified_name}.c_str());
        }

        return nullptr;
    }

    /**
     * Builds the Python types of one namespace into a module.
     *
     * @param args A (module, path) tuple: the module to create the types in and
     * the file system path of its _table.pywinrt.
     */
    PyObject* load_projection(PyObject* /*self*/, PyObject* args) noexcept
    {
        PyObject* module{};
        char const* path{};

        if (!PyArg_ParseTuple(args, "Os", &module, &path))
        {
            return nullptr;
        }

        auto const s = state();
        if (!s)
        {
            PyErr_SetString(PyExc_SystemError, "winrt-runtime is not loaded");
            return nullptr;
        }

        auto const module_name = PyModule_GetName(module);
        if (!module_name)
        {
            return nullptr;
        }

        {
            state_guard guard{s->cache_lock};

            if (s->projections.find(module_name) != s->projections.end())
            {
                // A reload re-runs __init__.py, and the types it made the
                // first time are still the ones in it.
                Py_RETURN_NONE;
            }
        }

        table::file const* file{};

        try
        {
            file = &open_table(path);
        }
        catch (table::format_error const& e)
        {
            PyErr_Format(
                PyExc_ImportError,
                "'%s' is not a projection table this winrt-runtime reads: %s",
                module_name,
                e.what());
            return nullptr;
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }

        if (file->forward_shape_limit() > forward_shape_count()
            || file->reverse_shape_limit() > reverse_shape_count())
        {
            PyErr_Format(
                PyExc_ImportError,
                "'%s' was generated against a larger census of call shapes than this "
                "winrt-runtime was built with; upgrade winrt-runtime",
                module_name);
            return nullptr;
        }

        projection* proj{};

        try
        {
            auto owned = std::make_unique<projection>();

            owned->table = file;
            owned->module_name = module_name;
            owned->module = module;
            owned->types.resize(file->type_count());

            for (uint32_t i = 0; i < file->type_count(); i++)
            {
                auto const record = file->type(i);
                if (record.flags() & table::type_flags::python_type)
                {
                    owned->by_py_name.emplace(record.py_name(), i);
                }
            }

            proj = owned.get();

            // Only the thread executing the module's __init__.py gets here,
            // because the import system holds that module's lock, so the
            // window above cannot be lost to another thread.
            {
                state_guard guard{s->cache_lock};
                s->projections.emplace(module_name, std::move(owned));
            }
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }

        // Registering the table before a single type is built is what lets a
        // namespace that refers to itself through another one resolve: the
        // other package imports this one back and finds the table rather than a
        // half-initialised module.
        for (uint32_t i = 0; i < file->type_count(); i++)
        {
            auto const record = file->type(i);
            if (!(record.flags() & table::type_flags::python_type))
            {
                continue;
            }

            switch (record.get_category())
            {
            case table::category::enum_:
            case table::category::struct_:
            case table::category::interface_:
            case table::category::class_:
                if (!ensure_type(*proj, i))
                {
                    return nullptr;
                }

                break;
            default:
                break;
            }
        }

        Py_RETURN_NONE;
    }

    /**
     * Lets go of the Python types this projection built, which is what
     * breaks the cycle between a type and the descriptors bound to it. The
     * descriptors themselves stay until the module state is freed, because
     * a type that outlives this call still has them bound, and so do the
     * names they answer to.
     */
    void projection::release_types() noexcept
    {
        for (auto& entry : types)
        {
            Py_CLEAR(entry.py_type);
            Py_CLEAR(entry.statics);
            Py_CLEAR(entry.implements);
        }
    }

    projection::~projection()
    {
        for (auto& name : names)
        {
            Py_XDECREF(name);
        }
    }
} // namespace py::interp
