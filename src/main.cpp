#include <winmd_reader.h>

#include "cmd_reader.h"
#include "task_group.h"
#include "text_writer.h"

#include "helpers.h"
#include "settings.h"
#include "type_writers.h"
#include "code_writers.h"
#include "file_writers.h"

namespace pywinrt
{
    settings_type settings;

    struct usage_exception
    {
    };

    static constexpr cmd::option options[]{
        {"input",
         0,
         cmd::option::no_max,
         "<spec>",
         "Windows metadata to include in projection"},
        {"reference",
         0,
         cmd::option::no_max,
         "<spec>",
         "Windows metadata to reference from projection"},
        {"output", 0, 1, "<path>", "Location of generated projection"},
        {"include",
         0,
         cmd::option::no_max,
         "<prefix>",
         "One or more prefixes to include in projection"},
        {"exclude",
         0,
         cmd::option::no_max,
         "<prefix>",
         "One or more prefixes to exclude from projection"},
        {"header-path", 0, 1, "<path>", "Install headers in custom path"},
        {"verbose", 0, 0, {}, "Show detailed progress information"},
        {"help", 0, cmd::option::no_max, {}, "Show detailed help"},
    };

    static void print_usage(writer& w)
    {
        static auto printColumns
            = [](writer& w, std::string_view const& col1, std::string_view const& col2)
        {
            w.write_printf("  %-20s%s\n", col1.data(), col2.data());
        };

        static auto printOption = [](writer& w, cmd::option const& opt)
        {
            if (opt.desc.empty())
            {
                return;
            }
            printColumns(w, w.write_temp("-% %", opt.name, opt.arg), opt.desc);
        };

        auto format = R"(
Py/WinRT v%
Copyright (c) Microsoft Corporation. All rights reserved.

  pywinrt.exe [options...]

Options:

%  ^@<path>             Response file containing command line options

Where <spec> is one or more of:

  path                Path to winmd file or recursively scanned folder
  local               Local ^%WinDir^%\System32\WinMetadata folder
  sdk[+]              Current version of Windows SDK [with extensions]
  10.0.12345.0[+]     Specific version of Windows SDK [with extensions]
)";
        w.write(format, PYWINRT_VERSION_STRING, bind_each(printOption, options));
    }

    void process_args(int const argc, char** argv)
    {
        cmd::reader args{argc, argv, options};

        if (!args || args.exists("help"))
        {
            throw usage_exception{};
        }

        settings.verbose = args.exists("verbose");
        settings.input = args.files("input", database::is_database);
        settings.reference = args.files("reference", database::is_database);

        for (auto&& include : args.values("include"))
        {
            settings.include.insert(include);
        }

        for (auto&& exclude : args.values("exclude"))
        {
            settings.exclude.insert(exclude);
        }

        settings.output_folder = absolute(args.value("output", "output"));
        create_directories(settings.output_folder);

        if (args.exists("header-path"))
        {
            settings.header_path = absolute(args.value("header-path"));
            create_directories(settings.header_path.value());
        }
    }

    auto get_files_to_cache()
    {
        std::vector<std::string> files;
        files.insert(files.end(), settings.input.begin(), settings.input.end());
        files.insert(files.end(), settings.reference.begin(), settings.reference.end());
        return files;
    }

    bool has_projected_types(cache::namespace_members const& members)
    {
        return !members.interfaces.empty() || !members.classes.empty()
               || !members.enums.empty() || !members.structs.empty()
               || !members.delegates.empty();
    }

    static void build_filters(cache const& c)
    {
        std::set<std::string> include;

        if (!settings.include.empty())
        {
            // if user gave includes, use them
            include = settings.include;
        }
        else if (!settings.reference.empty())
        {
            // if user did not give includes but gave references, get all types
            // from input
            for (auto file : settings.input)
            {
                auto db = std::find_if(
                    c.databases().begin(),
                    c.databases().end(),
                    [&](auto&& db)
                    {
                        return db.path() == file;
                    });

                for (auto&& type : db->TypeDef)
                {
                    if (!type.Flags().WindowsRuntime())
                    {
                        continue;
                    }

                    std::string full_name{type.TypeNamespace()};
                    full_name += '.';
                    full_name += type.TypeName();
                    include.insert(full_name);
                }
            }
        }
        else
        {
            // if user gave neither includes nor references, then we don't need
            // an include filter
            include = {};
        }

        settings.filter = {include, settings.exclude};
    }

    int run(int const argc, char** argv)
    {
        int result{};
        writer w;

        try
        {
            auto start = get_start_time();
            process_args(argc, argv);
            cache c{get_files_to_cache()};
            build_filters(c);

            if (settings.verbose)
            {
                for (auto&& file : settings.input)
                {
                    w.write("input: %\n", file);
                }

                for (auto&& file : settings.reference)
                {
                    w.write("reference: %\n", file);
                }

                w.write("output: %\n", settings.output_folder.string());
            }

            w.flush_to_console();

            task_group group;

            // if we are building the base projection (not user components),
            // then emit some extra files
            if (settings.reference.empty())
            {
                group.add(
                    []
                    {
                        write_base_files(
                            settings.header_path.value_or(settings.output_folder));
                    });
            }

            std::vector<std::string> generated_namespaces{};

            for (auto&& [ns, members] : c.namespaces())
            {
                if (!has_projected_types(members) || !settings.filter.includes(members))
                {
                    continue;
                }

                generated_namespaces.emplace_back(ns);

                auto ns_package_name = w.write_temp("winrt-%", ns);
                auto ns_package_dir = settings.output_folder / ns_package_name;
                auto ns_winrt_dir = ns_package_dir / "winrt";
                auto ns_dir = ns_winrt_dir;

                for (auto&& ns_segment : get_dotted_name_segments(ns))
                {
                    auto segment
                        = w.write_temp("%", bind<write_python_identifier>(ns_segment));
                    ns_dir /= segment;
                }

                create_directories(ns_dir);

                group.add(
                    [ns_package_dir, ns_winrt_dir, ns_dir, ns = ns, members = members]
                    {
                        auto header_dir = settings.header_path.value_or(ns_package_dir);

                        auto namespaces
                            = write_namespace_cpp(ns_package_dir, ns, members);
                        write_namespace_h(header_dir, ns, namespaces, members);
                        write_namespace_dunder_init_py(ns_dir, ns, members);
                        write_namespace_pyi(ns_winrt_dir, namespaces, ns, members);
                        write_requirements_txt(ns_package_dir);
                        write_all_requirements_txt(ns_package_dir, namespaces);
                    });
            }

            group.get();

            if (settings.verbose)
            {
                w.write("time: %ms\n", get_elapsed_time(start));
            }
        }
        catch (usage_exception const&)
        {
            print_usage(w);
        }
        catch (std::exception const& e)
        {
            w.write(" error: %\n", e.what());
            result = 1;
        }

        w.flush_to_console();
        return result;
    }
} // namespace pywinrt

int main(int const argc, char** argv)
{
    return pywinrt::run(argc, argv);
}
