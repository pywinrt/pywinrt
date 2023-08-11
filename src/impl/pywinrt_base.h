#pragma once

#include <windows.h>
#include <shlwapi.h>
#include <xmllite.h>
#include <stdexcept>
#include <assert.h>
#include <array>
#include <bitset>
#include <fstream>
#include <future>
#include <list>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <set>
#include <filesystem>

namespace pywinrt
{
    using namespace std::literals;

    [[noreturn]] inline void throw_invalid(std::string const& message)
    {
        throw std::invalid_argument(message);
    }

    template<typename... T>
    [[noreturn]] inline void throw_invalid(std::string message, T const&... args)
    {
        (message.append(args), ...);
        throw std::invalid_argument(message);
    }

    template<typename T>
    auto c_str(std::basic_string_view<T> const& view) noexcept
    {
        if (*(view.data() + view.size()))
        {
            std::terminate();
        }

        return view.data();
    }

    inline bool starts_with(
        std::string_view const& value, std::string_view const& match) noexcept
    {
        return 0 == value.compare(0, match.size(), match);
    }

    template<typename... T>
    struct visit_overload : T...
    {
        using T::operator()...;
    };

    template<typename V, typename... C>
    auto call(V&& variant, C&&... call)
    {
        return std::visit(
            visit_overload<C...>{std::forward<C>(call)...}, std::forward<V>(variant));
    }
} // namespace pywinrt
