// The vocabulary the members of the Windows.Foundation.Numerics structs are
// written in: which struct a value belongs to, and how one is read out of a
// Python object and written back into one.
//
// numerics.cpp and numerics-statics.cpp are both written in terms of this, so
// what is here is what a member of one of the structs needs whether it belongs
// to a value or to the type. Most of it is templates, because the C++ type of
// the value is what says which of the seven structs is meant, and the rest is
// what those templates call. Both translation units get their own copy, which
// is what the anonymous namespace says.

#pragma once

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "numerics.h"
#include "structs.h"
#include "types.h"

#include <array>
#include <cstring>
#include <tuple>
#include <type_traits>
#include <utility>

namespace py::interp::numerics
{
    namespace
    {
        namespace num = winrt::Windows::Foundation::Numerics;

        /**
         * The C++ value and the Python name of one of the structs.
         */
        template<kind K>
        struct traits;

        template<>
        struct traits<kind::vector2>
        {
            using type = num::float2;
            static constexpr char const* py_name = "Vector2";
        };

        template<>
        struct traits<kind::vector3>
        {
            using type = num::float3;
            static constexpr char const* py_name = "Vector3";
        };

        template<>
        struct traits<kind::vector4>
        {
            using type = num::float4;
            static constexpr char const* py_name = "Vector4";
        };

        template<>
        struct traits<kind::matrix3x2>
        {
            using type = num::float3x2;
            static constexpr char const* py_name = "Matrix3x2";
        };

        template<>
        struct traits<kind::matrix4x4>
        {
            using type = num::float4x4;
            static constexpr char const* py_name = "Matrix4x4";
        };

        template<>
        struct traits<kind::plane>
        {
            using type = num::plane;
            static constexpr char const* py_name = "Plane";
        };

        template<>
        struct traits<kind::quaternion>
        {
            using type = num::quaternion;
            static constexpr char const* py_name = "Quaternion";
        };

        /**
         * Which struct a C++ value belongs to, which is how the argument and
         * result types of a C++/WinRT function say which Python type to read
         * or to build.
         */
        template<typename T>
        struct value_kind;

        template<>
        struct value_kind<num::float2> : std::integral_constant<kind, kind::vector2>
        {
        };

        template<>
        struct value_kind<num::float3> : std::integral_constant<kind, kind::vector3>
        {
        };

        template<>
        struct value_kind<num::float4> : std::integral_constant<kind, kind::vector4>
        {
        };

        template<>
        struct value_kind<num::float3x2> : std::integral_constant<kind, kind::matrix3x2>
        {
        };

        template<>
        struct value_kind<num::float4x4> : std::integral_constant<kind, kind::matrix4x4>
        {
        };

        template<>
        struct value_kind<num::plane> : std::integral_constant<kind, kind::plane>
        {
        };

        template<>
        struct value_kind<num::quaternion>
            : std::integral_constant<kind, kind::quaternion>
        {
        };

        // ----- finding the types ------------------------------------------

        /**
         * The entry of the struct @p Wanted, given the entry of @p Known,
         * which came from the same table.
         *
         * All seven structs are declared in one namespace, so a sibling is
         * found by name in the table @p known came from. The lookup is made
         * when the call is rather than when the type was built, because the
         * types refer to each other in both directions - a Vector2 is
         * transformed by a Matrix3x2 and a Matrix3x2 has a Vector2 translation
         * - so neither of the two can be the one that is built first.
         */
        template<kind Known, kind Wanted>
        type_entry* entry_for(type_entry& known) noexcept
        {
            if constexpr (Known == Wanted)
            {
                return &known;
            }
            else
            {
                auto& proj = *known.owner;

                auto const found = proj.by_py_name.find(traits<Wanted>::py_name);
                if (found == proj.by_py_name.end())
                {
                    return nullptr;
                }

                return ensure_entry(proj, found->second);
            }
        }

        /**
         * The error raised when a table of Windows.Foundation.Numerics is
         * missing one of the structs the others are written in terms of.
         */
        void missing_type(char const* py_name) noexcept
        {
            if (PyErr_Occurred())
            {
                return;
            }

            PyErr_Format(PyExc_TypeError, "'%s' is not in this projection", py_name);
        }

        // ----- reading and writing values ---------------------------------

        /**
         * Reads @p obj as a number.
         */
        bool read_number(PyObject* obj, float& out) noexcept
        {
            try
            {
                out = convert_to<float>(obj);
                return true;
            }
            catch (...)
            {
                to_PyErr();
                return false;
            }
        }

        /**
         * Reads @p obj as a value of the struct @p Wanted, which it is an
         * instance of or a tuple of the fields of.
         */
        template<kind Known, kind Wanted>
        bool read_struct(
            type_entry& known,
            PyObject* obj,
            typename traits<Wanted>::type& out) noexcept
        {
            auto const entry = entry_for<Known, Wanted>(known);
            if (!entry)
            {
                missing_type(traits<Wanted>::py_name);
                return false;
            }

            try
            {
                struct_from_python(*entry, obj, &out);
                return true;
            }
            catch (...)
            {
                to_PyErr();
                return false;
            }
        }

        /**
         * Wraps @p value as the Python type of the struct @p Wanted.
         */
        template<kind Known, kind Wanted>
        PyObject* write_struct(
            type_entry& known, typename traits<Wanted>::type const& value) noexcept
        {
            auto const entry = entry_for<Known, Wanted>(known);
            if (!entry)
            {
                missing_type(traits<Wanted>::py_name);
                return nullptr;
            }

            return struct_to_python(*entry, &value);
        }

        /**
         * Reads one argument of a member, whose C++ type says what it is.
         */
        template<kind Known, typename T>
        bool read_arg(type_entry& known, PyObject* obj, T& out) noexcept
        {
            if constexpr (std::is_same_v<T, float>)
            {
                return read_number(obj, out);
            }
            else
            {
                return read_struct<Known, value_kind<T>::value>(known, obj, out);
            }
        }

        /**
         * Wraps what a member handed back, which is a number, a truth value or
         * a value of one of the structs.
         */
        template<kind Known, typename R>
        PyObject* write_result(type_entry& known, R const& result) noexcept
        {
            if constexpr (std::is_same_v<R, float>)
            {
                return convert(result);
            }
            else if constexpr (std::is_same_v<R, bool>)
            {
                return convert(result);
            }
            else
            {
                return write_struct<Known, value_kind<R>::value>(known, result);
            }
        }

        // ----- calling one of the C++/WinRT functions ----------------------

        template<kind Known, typename Fn, typename... Args, size_t... I>
        PyObject* apply_args(
            type_entry& known,
            PyObject* const* args,
            Fn fn,
            std::tuple<Args...>& values,
            std::index_sequence<I...>) noexcept
        {
            auto ok = true;
            (..., (ok = ok && read_arg<Known>(known, args[I], std::get<I>(values))));

            if (!ok)
            {
                return nullptr;
            }

            return std::apply(
                [&](auto const&... value)
                {
                    return write_result<Known>(known, fn(value...));
                },
                values);
        }

        /**
         * Reads the arguments a member declares out of @p args, calls @p fn
         * with them and wraps what it handed back.
         */
        template<kind Known, typename... Args, typename Fn>
        PyObject* apply(type_entry& known, PyObject* const* args, Fn fn) noexcept
        {
            std::tuple<Args...> values{};

            return apply_args<Known>(
                known, args, fn, values, std::index_sequence_for<Args...>{});
        }

        /**
         * Calls @p fn with the arguments in the tuple @p args, which a static
         * member receives them in and whose length the caller has checked.
         */
        template<kind K, typename... Args, typename Fn>
        PyObject* call_static(type_entry& entry, PyObject* args, Fn fn) noexcept
        {
            std::array<PyObject*, sizeof...(Args)> items{};

            for (size_t i = 0; i < items.size(); i++)
            {
                items[i] = PyTuple_GET_ITEM(args, static_cast<Py_ssize_t>(i));
            }

            return apply<K, Args...>(entry, items.data(), fn);
        }

        /**
         * Whether @p args holds exactly @p count arguments, and the error a
         * member raises when it does not.
         */
        bool check_count(PyObject* args, Py_ssize_t count, char const* name) noexcept
        {
            if (PyTuple_GET_SIZE(args) == count)
            {
                return true;
            }

            PyErr_Format(
                PyExc_TypeError,
                "%s() takes %zd arguments, not %zd",
                name,
                count,
                PyTuple_GET_SIZE(args));
            return false;
        }

#if defined(__MINGW32__)
        /**
         * The error a member raises on a toolchain whose C++/WinRT headers
         * declare the function it calls and provide no definition of it.
         *
         * mingw-w64 ships <windowsnumerics.impl.h> with the whole of the
         * declarations and only part of the implementation, so what this
         * answers for are the members that would otherwise keep the runtime
         * from linking at all.
         */
        PyObject* not_on_mingw(char const* name) noexcept
        {
            PyErr_Format(
                PyExc_NotImplementedError, "%s() is not implemented on MinGW", name);
            return nullptr;
        }
#endif
    } // namespace
} // namespace py::interp::numerics
