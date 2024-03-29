#pragma once

namespace pywinrt
{
    struct settings_type
    {
        std::set<std::string> input;
        std::set<std::string> reference;

        std::optional<std::filesystem::path> header_path;
        std::filesystem::path output_folder;
        bool verbose{};

        std::set<std::string> include;
        std::set<std::string> exclude;
        winmd::reader::filter filter;
    };

    extern settings_type settings;
} // namespace pywinrt
