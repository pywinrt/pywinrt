#pragma once

namespace pywinrt
{
    namespace stdfs = std::filesystem;

    inline void write_base_files(stdfs::path const& folder)
    {
        writer w;

        write_license(w);
        w.write("#pragma once\n");
        w.write("#define PYWINRT_VERSION \"%\"\n", PYWINRT_VERSION_STRING);

        w.flush_to_file(folder / "pywinrt_version.h");
    }

    inline void write_namespace_h(
        stdfs::path const& folder,
        std::string_view const& ns,
        std::set<std::string> const& needed_namespaces,
        cache::namespace_members const& members)
    {
        writer w;
        w.current_namespace = ns;

        for (auto&& needed_ns : needed_namespaces)
        {
            w.needed_namespaces.insert(needed_ns);
        }

        auto filename = w.write_temp("py.%.h", ns);

        auto segments = get_dotted_name_segments(ns);

        w.write("\nnamespace py::proj::%\n{", bind_list("::", segments));
        {
            writer::indent_guard g{w};
            settings.filter.bind_each<write_pinterface_decl>(members.interfaces)(w);
        }
        w.write("}\n");

        w.write("\nnamespace py::impl::%\n{", bind_list("::", segments));
        {
            writer::indent_guard g{w};
            settings.filter.bind_each<write_delegate_callable_wrapper>(
                members.delegates)(w);
            settings.filter.bind_each<write_pinterface_impl>(members.interfaces)(w);
        }
        w.write("}\n");

        w.write("\nnamespace py::wrapper::%\n{\n", bind_list("::", segments));
        {
            writer::indent_guard g{w};
            settings.filter.bind_each<write_python_wrapper_alias>(members.classes)(w);
            settings.filter.bind_each<write_python_wrapper_alias>(members.interfaces)(
                w);
            settings.filter.bind_each<write_python_wrapper_alias>(members.structs)(w);
        }
        w.write("}\n");

        w.write("\nnamespace py\n{\n");
        {
            writer::indent_guard g{w};

            settings.filter.bind_each<write_enum_buffer_format_decl>(members.enums)(w);
            settings.filter.bind_each<write_struct_buffer_format_decl>(members.structs)(
                w);
            settings.filter.bind_each<write_py_type_specialization_struct>(
                members.enums)(w);
            settings.filter.bind_each<write_py_type_specialization_struct>(
                members.classes)(w);
            settings.filter.bind_each<write_py_type_specialization_struct>(
                members.interfaces)(w);
            settings.filter.bind_each<write_py_type_specialization_struct>(
                members.structs)(w);
            settings.filter.bind_each<write_pinterface_type_mapper>(members.interfaces)(
                w);
            settings.filter.bind_each<write_delegate_type_mapper>(members.delegates)(w);
        }
        w.write("}\n");

        w.swap();

        write_license(w);
        w.write("#pragma once\n\n");
        w.write("#include \"pybase.h\"\n");
        w.write(
            "static_assert(winrt::check_version(PYWINRT_VERSION, \"%\"), \"Mismatched Py/WinRT headers.\");\n",
            PYWINRT_VERSION_STRING);

        w.write_each<write_py_include>(w.needed_namespaces);
        w.write("\n");
        w.write_each<write_winrt_include>(w.needed_namespaces);

        {
            auto format = R"(
#include <winrt/%.h>
)";
            w.write(format, ns);
        }

        w.flush_to_file(folder / filename);
    }

    inline auto write_namespace_cpp(
        stdfs::path const& folder,
        std::string_view const& ns,
        cache::namespace_members const& members)
    {
        writer w;
        w.current_namespace = ns;
        auto filename = w.write_temp("py.%.cpp", ns);

        write_license(w);
        w.write("#include \"py.%.h\"\n", ns);

        if (ns == "Windows.Foundation")
        {
            w.write(R"(

static int64_t custom_get(winrt::event_token const& instance)
{
    return instance.value;
}

static void custom_set(winrt::event_token& instance, int64_t value)
{
    instance.value = value;
}

static int32_t custom_get(winrt::hresult const& instance)
{
    return instance;
}

static void custom_set(winrt::hresult& instance, int32_t value)
{
    instance = value;
})");
        }

        auto segments = get_dotted_name_segments(ns);
        w.write("\n\nnamespace py::cpp::%\n{", bind_list("::", segments));
        {
            writer::indent_guard g{w};

            settings.filter.bind_each<write_inspectable_type>(members.classes)(w);
            settings.filter.bind_each<write_inspectable_type>(members.interfaces)(w);
            settings.filter.bind_each<write_struct>(members.structs)(w);
            write_namespace_initialization(w, ns, members);
        }
        w.write("} // py::cpp::%\n", bind_list("::", segments));

        write_namespace_module_init_function(w, ns, members);

        w.flush_to_file(folder / filename);
        return std::move(w.needed_namespaces);
    }

    inline void write_namespace_dunder_init_py(
        stdfs::path const& folder,
        std::string_view const& ns,
        cache::namespace_members const& members)
    {
        writer w;
        w.current_namespace = ns;

        write_license(w, "#");

        auto enum_types = filter_types(settings.filter, members.enums);
        if (!enum_types.empty())
        {
            w.write("import enum\n");
        }

        auto delegate_aliases = w.write_temp(
            "%",
            bind(settings.filter.bind_each<write_python_delegate_type_alias>(
                members.delegates)));

        if (!delegate_aliases.empty())
        {
            w.write("import typing\n");
            w.write("import uuid as _uuid\n");
        }

        if (!enum_types.empty() || !delegate_aliases.empty())
        {
            w.write("\n");
        }

        w.write("import winrt.system\n");
        w.write("from winrt import %\n", bind<write_ns_module_name>(ns));

        // Search for any type names that got quoted in the delegates and
        // collect them to determine if any imports are needed.
        std::regex winrt_type(R"(\b[a-z]+(?:_[a-z]+)+\.[a-zA-Z]+)");
        std::string search_str{delegate_aliases};
        std::set<std::string> delegate_imports{};

        for (std::smatch sm; std::regex_search(search_str, sm, winrt_type);
             search_str = sm.suffix())
        {
            auto match = sm.str();
            auto last_dot = match.find_last_of('.');
            delegate_imports.insert(match.substr(0, last_dot));
        }

        // Write any import needed for the quoted type in the delegates
        if (!delegate_imports.empty())
        {
            w.write("\nif typing.TYPE_CHECKING:\n");
            {
                writer::indent_guard g{w};

                for (auto&& import : delegate_imports)
                {
                    std::string ns;
                    std::replace_copy(
                        import.begin(), import.end(), std::back_inserter(ns), '_', '.');

                    w.write("import winrt.% as %\n", ns, import);
                }
            }

            w.write("\n");
        }

        w.write("\n");
        w.write("__all__ = [\n");
        {
            writer::indent_guard g{w};
            settings.filter.bind_each<write_dunder_all_type_name_entry>(members.enums)(
                w);
            settings.filter.bind_each<write_dunder_all_type_name_entry>(
                members.structs)(w);
            settings.filter.bind_each<write_dunder_all_type_name_entry>(
                members.classes)(w);
            settings.filter.bind_each<write_dunder_all_type_name_entry>(
                members.interfaces)(w);
            settings.filter.bind_each<write_dunder_all_type_name_entry>(
                members.delegates)(w);
        }
        w.write("]\n");

        write_python_type_vars(w, members.delegates);

        settings.filter.bind_each<write_python_enum>(members.enums)(w);
        w.write("\n");
        settings.filter.bind_each<write_python_import_type>(members.structs)(w);
        settings.filter.bind_each<write_python_import_type>(members.classes)(w);
        settings.filter.bind_each<write_python_import_type>(members.interfaces)(w);
        w.write("%", delegate_aliases);

        w.flush_to_file(folder / "__init__.py");
    }

    /**
     * Writes the .pyi file for a namespace.
     *
     * @param folder The folder to write the file to.
     * @param needed_namespaces The set of namespaces that are needed by this namespace.
     * @param ns The namespace to write the file for.
     * @param members The members of the namespace.
     */
    inline void write_namespace_pyi(
        stdfs::path const& folder,
        std::set<std::string> const& needed_namespaces,
        std::string_view const& ns,
        cache::namespace_members const& members)
    {
        writer w;
        w.current_namespace = ns;

        write_license(w, "#");

        w.write("import datetime\n");
        w.write("import sys\n");
        w.write("import types\n");
        w.write("import typing\n");
        w.write("import uuid as _uuid\n");
        w.write("from builtins import property as _property\n");
        w.write("\n");
        w.write("import winrt._winrt\n");
        w.write("import winrt.system\n");

        w.write_each<write_python_import_namespace>(needed_namespaces);
        w.write("\n");

        auto enum_types = filter_types(settings.filter, members.enums);
        if (!enum_types.empty())
        {
            w.write(
                "from winrt.% import %\n",
                bind<write_python_subpackage>(ns),
                bind_list<write_type_name>(", ", enum_types));
        }

        auto delegate_types = filter_types(settings.filter, members.delegates);
        if (!delegate_types.empty())
        {
            w.write(
                "from winrt.% import %\n",
                bind<write_python_subpackage>(ns),
                bind_list<write_type_name>(", ", delegate_types));
        }

        if (!enum_types.empty() || !delegate_types.empty())
        {
            w.write("\n");
        }

        w.write("Self = typing.TypeVar('Self')\n");
        write_python_type_vars(w, members.interfaces);
        w.write("\n");

        settings.filter.bind_each<write_python_typing_for_struct>(members.structs)(w);
        settings.filter.bind_each<write_python_typing_for_object>(members.classes)(w);
        settings.filter.bind_each<write_python_typing_for_object>(members.interfaces)(
            w);

        auto file_name = w.write_temp("%.pyi", bind<write_ns_module_name>(ns));
        w.flush_to_file(folder / file_name);
    }

    /**
     * Writes a `pywinrt-version.txt` file.
     *
     * @param folder The folder to write the file to.
     */
    inline void write_pywinrt_version_txt(stdfs::path const& folder)
    {
        writer w{};

        w.write(PYWINRT_VERSION_STRING);

        w.flush_to_file(folder / "pywinrt-version.txt");
    }

    /**
     * Writes a `requirements.txt` file.
     *
     * @param folder The folder to write the file to.
     */
    inline void write_requirements_txt(stdfs::path const& folder)
    {
        writer w{};

        write_license(w, "#");

        w.write("winrt-runtime==%\n", PYWINRT_VERSION_STRING);

        w.flush_to_file(folder / "requirements.txt");
    }

    /**
     * Writes a `all-requirements.txt` file for the given namespaces.
     *
     * No file is written if the set of namespaces is empty.
     *
     * @param folder The folder to write the file to.
     * @param needed_namespaces The set of namespaces that are needed by the caller.
     */
    inline void write_all_requirements_txt(
        stdfs::path const& folder, std::set<std::string> const& needed_namespaces)
    {
        if (needed_namespaces.empty())
        {
            return;
        }

        writer w{};

        write_license(w, "#");

        for (auto&& ns : needed_namespaces)
        {
            w.write("winrt-%[all]==%\n", ns, PYWINRT_VERSION_STRING);
        }

        w.flush_to_file(folder / "all-requirements.txt");
    }
} // namespace pywinrt
