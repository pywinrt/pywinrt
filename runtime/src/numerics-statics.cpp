// The constants and the factory functions of the Windows.Foundation.Numerics
// structs.
//
// A constant is a value the struct names - Vector3.zero, Matrix4x4.identity -
// and a factory function builds one out of what it describes, as
// Matrix4x4.make_look_at() builds a transform out of three points. C++/WinRT
// declares both in <windowsnumerics.impl.h>, and both are reached through the
// type rather than through a value, so they sit on a metaclass of the type's
// own. numerics.cpp has the members that belong to a value.
//
// A factory function takes its arguments in a tuple because several of them
// are one Python name over C++ overloads of different lengths, so which one is
// meant is decided by how many arguments arrived.
//
// The mingw-w64 headers declare the same functions and only some of them
// resolve at link time, so the ones they leave out raise NotImplementedError
// on that toolchain rather than keeping the runtime from linking at all.

#include "numerics-statics.h"
#include "numerics-values.h"

#include <vector>

namespace py::interp::numerics
{
    namespace
    {
        /**
         * The entry of the struct a static member was reached through, which
         * is the type the call was made on: a constant and a factory function
         * live on the metaclass, so what they are given is the type itself.
         */
        type_entry* static_entry(PyObject* self) noexcept
        {
            if (!PyType_Check(self))
            {
                PyErr_SetString(PyExc_TypeError, "expected a type");
                return nullptr;
            }

            auto const type = reinterpret_cast<PyTypeObject*>(self);

            auto const entry = find_type_entry(type);
            if (!entry)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' is not a type a projection table built",
                    type->tp_name);
                return nullptr;
            }

            return entry;
        }

        /**
         * The error a member with several overloads raises when none of them
         * takes that many arguments.
         */
        PyObject* no_overload(char const* name, Py_ssize_t count) noexcept
        {
            PyErr_Format(
                PyExc_TypeError,
                "no overload of %s() takes %zd arguments",
                name,
                count);
            return nullptr;
        }

        // ----- the constants ----------------------------------------------

        /**
         * One of the values a struct names, which sits on the metaclass so
         * that it is read off the type rather than off an instance.
         */
        template<kind K, typename traits<K>::type (*Fn)()>
        PyObject* constant(PyObject* self, void* /*unused*/) noexcept
        {
            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            return write_struct<K, K>(*entry, Fn());
        }

        PyGetSetDef vector2_constants[]
            = {{"zero",
                constant<kind::vector2, &num::float2::zero>,
                nullptr,
                nullptr,
                nullptr},
               {"one",
                constant<kind::vector2, &num::float2::one>,
                nullptr,
                nullptr,
                nullptr},
               {"unit_x",
                constant<kind::vector2, &num::float2::unit_x>,
                nullptr,
                nullptr,
                nullptr},
               {"unit_y",
                constant<kind::vector2, &num::float2::unit_y>,
                nullptr,
                nullptr,
                nullptr},
               {}};

        PyGetSetDef vector3_constants[]
            = {{"zero",
                constant<kind::vector3, &num::float3::zero>,
                nullptr,
                nullptr,
                nullptr},
               {"one",
                constant<kind::vector3, &num::float3::one>,
                nullptr,
                nullptr,
                nullptr},
               {"unit_x",
                constant<kind::vector3, &num::float3::unit_x>,
                nullptr,
                nullptr,
                nullptr},
               {"unit_y",
                constant<kind::vector3, &num::float3::unit_y>,
                nullptr,
                nullptr,
                nullptr},
               {"unit_z",
                constant<kind::vector3, &num::float3::unit_z>,
                nullptr,
                nullptr,
                nullptr},
               {}};

        PyGetSetDef vector4_constants[]
            = {{"zero",
                constant<kind::vector4, &num::float4::zero>,
                nullptr,
                nullptr,
                nullptr},
               {"one",
                constant<kind::vector4, &num::float4::one>,
                nullptr,
                nullptr,
                nullptr},
               {"unit_x",
                constant<kind::vector4, &num::float4::unit_x>,
                nullptr,
                nullptr,
                nullptr},
               {"unit_y",
                constant<kind::vector4, &num::float4::unit_y>,
                nullptr,
                nullptr,
                nullptr},
               {"unit_z",
                constant<kind::vector4, &num::float4::unit_z>,
                nullptr,
                nullptr,
                nullptr},
               {"unit_w",
                constant<kind::vector4, &num::float4::unit_w>,
                nullptr,
                nullptr,
                nullptr},
               {}};

        PyGetSetDef matrix3x2_constants[]
            = {{"identity",
                constant<kind::matrix3x2, &num::float3x2::identity>,
                nullptr,
                nullptr,
                nullptr},
               {}};

        PyGetSetDef matrix4x4_constants[]
            = {{"identity",
                constant<kind::matrix4x4, &num::float4x4::identity>,
                nullptr,
                nullptr,
                nullptr},
               {}};

        PyGetSetDef quaternion_constants[]
            = {{"identity",
                constant<kind::quaternion, &num::quaternion::identity>,
                nullptr,
                nullptr,
                nullptr},
               {}};

        /**
         * The values the struct @p which names, or @c nullptr for a struct
         * that names none.
         */
        PyGetSetDef* constants_of(kind which) noexcept
        {
            switch (which)
            {
            case kind::vector2:
                return vector2_constants;
            case kind::vector3:
                return vector3_constants;
            case kind::vector4:
                return vector4_constants;
            case kind::matrix3x2:
                return matrix3x2_constants;
            case kind::matrix4x4:
                return matrix4x4_constants;
            case kind::quaternion:
                return quaternion_constants;
            case kind::plane:
            case kind::none:
                break;
            }

            return nullptr;
        }

        // ----- the factory functions --------------------------------------

        /**
         * A factory function with one signature, which is what most of a
         * Matrix4x4, the Plane and the Quaternion are made by.
         */
        template<kind K, auto Fn, typename... Args>
        PyObject* make_fixed(PyObject* self, PyObject* args, char const* name) noexcept
        {
            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            if (!check_count(args, static_cast<Py_ssize_t>(sizeof...(Args)), name))
            {
                return nullptr;
            }

            return call_static<K, Args...>(
                *entry,
                args,
                [](auto const&... arg)
                {
                    return Fn(arg...);
                });
        }

        PyObject* make_matrix3x2_translation(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_translation");
#else
            constexpr auto self_kind = kind::matrix3x2;

            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const make = [](auto const&... arg)
            {
                return num::make_float3x2_translation(arg...);
            };

            switch (PyTuple_GET_SIZE(args))
            {
            case 1:
                return call_static<self_kind, num::float2>(*entry, args, make);
            case 2:
                return call_static<self_kind, float, float>(*entry, args, make);
            }

            return no_overload("make_translation", PyTuple_GET_SIZE(args));
#endif
        }

        PyObject* make_matrix3x2_scale(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_scale");
#else
            constexpr auto self_kind = kind::matrix3x2;

            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const make = [](auto const&... arg)
            {
                return num::make_float3x2_scale(arg...);
            };

            switch (PyTuple_GET_SIZE(args))
            {
            case 2:
                return call_static<self_kind, float, float>(*entry, args, make);
            case 3:
                return call_static<self_kind, float, float, num::float2>(
                    *entry, args, make);
            }

            return no_overload("make_scale", PyTuple_GET_SIZE(args));
#endif
        }

        PyObject* make_matrix3x2_scale_from_vector(
            PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_scale_from_vector");
#else
            constexpr auto self_kind = kind::matrix3x2;

            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const make = [](auto const&... arg)
            {
                return num::make_float3x2_scale(arg...);
            };

            switch (PyTuple_GET_SIZE(args))
            {
            case 1:
                return call_static<self_kind, num::float2>(*entry, args, make);
            case 2:
                return call_static<self_kind, num::float2, num::float2>(
                    *entry, args, make);
            }

            return no_overload("make_scale_from_vector", PyTuple_GET_SIZE(args));
#endif
        }

        PyObject* make_matrix3x2_scale_from_scalar(
            PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_scale_from_scalar");
#else
            constexpr auto self_kind = kind::matrix3x2;

            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const make = [](auto const&... arg)
            {
                return num::make_float3x2_scale(arg...);
            };

            switch (PyTuple_GET_SIZE(args))
            {
            case 1:
                return call_static<self_kind, float>(*entry, args, make);
            case 2:
                return call_static<self_kind, float, num::float2>(*entry, args, make);
            }

            return no_overload("make_scale_from_scalar", PyTuple_GET_SIZE(args));
#endif
        }

        PyObject* make_matrix3x2_skew(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_skew");
#else
            constexpr auto self_kind = kind::matrix3x2;

            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const make = [](auto const&... arg)
            {
                return num::make_float3x2_skew(arg...);
            };

            switch (PyTuple_GET_SIZE(args))
            {
            case 2:
                return call_static<self_kind, float, float>(*entry, args, make);
            case 3:
                return call_static<self_kind, float, float, num::float2>(
                    *entry, args, make);
            }

            return no_overload("make_skew", PyTuple_GET_SIZE(args));
#endif
        }

        PyObject* make_matrix3x2_rotation(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_rotation");
#else
            constexpr auto self_kind = kind::matrix3x2;

            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const make = [](auto const&... arg)
            {
                return num::make_float3x2_rotation(arg...);
            };

            switch (PyTuple_GET_SIZE(args))
            {
            case 1:
                return call_static<self_kind, float>(*entry, args, make);
            case 2:
                return call_static<self_kind, float, num::float2>(*entry, args, make);
            }

            return no_overload("make_rotation", PyTuple_GET_SIZE(args));
#endif
        }

        PyObject* make_matrix4x4_billboard(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_billboard");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_billboard,
                num::float3,
                num::float3,
                num::float3,
                num::float3>(self, args, "make_billboard");
#endif
        }

        PyObject* make_matrix4x4_constrained_billboard(
            PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_constrained_billboard");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_constrained_billboard,
                num::float3,
                num::float3,
                num::float3,
                num::float3,
                num::float3>(self, args, "make_constrained_billboard");
#endif
        }

        PyObject* make_matrix4x4_translation(PyObject* self, PyObject* args) noexcept
        {
            constexpr auto self_kind = kind::matrix4x4;

            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const make = [](auto const&... arg)
            {
                return num::make_float4x4_translation(arg...);
            };

            switch (PyTuple_GET_SIZE(args))
            {
            case 1:
                return call_static<self_kind, num::float3>(*entry, args, make);
            case 3:
#if defined(__MINGW32__)
                return not_on_mingw("make_translation");
#else
                return call_static<self_kind, float, float, float>(*entry, args, make);
#endif
            }

            return no_overload("make_translation", PyTuple_GET_SIZE(args));
        }

        PyObject* make_matrix4x4_scale(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_scale");
#else
            constexpr auto self_kind = kind::matrix4x4;

            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const make = [](auto const&... arg)
            {
                return num::make_float4x4_scale(arg...);
            };

            switch (PyTuple_GET_SIZE(args))
            {
            case 3:
                return call_static<self_kind, float, float, float>(*entry, args, make);
            case 4:
                return call_static<self_kind, float, float, float, num::float3>(
                    *entry, args, make);
            }

            return no_overload("make_scale", PyTuple_GET_SIZE(args));
#endif
        }

        PyObject* make_matrix4x4_scale_from_vector(
            PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_scale_from_vector");
#else
            constexpr auto self_kind = kind::matrix4x4;

            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const make = [](auto const&... arg)
            {
                return num::make_float4x4_scale(arg...);
            };

            switch (PyTuple_GET_SIZE(args))
            {
            case 1:
                return call_static<self_kind, num::float3>(*entry, args, make);
            case 2:
                return call_static<self_kind, num::float3, num::float3>(
                    *entry, args, make);
            }

            return no_overload("make_scale_from_vector", PyTuple_GET_SIZE(args));
#endif
        }

        PyObject* make_matrix4x4_scale_from_scalar(
            PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_scale_from_scalar");
#else
            constexpr auto self_kind = kind::matrix4x4;

            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const make = [](auto const&... arg)
            {
                return num::make_float4x4_scale(arg...);
            };

            switch (PyTuple_GET_SIZE(args))
            {
            case 1:
                return call_static<self_kind, float>(*entry, args, make);
            case 2:
                return call_static<self_kind, float, num::float3>(*entry, args, make);
            }

            return no_overload("make_scale_from_scalar", PyTuple_GET_SIZE(args));
#endif
        }

#if !defined(__MINGW32__)
        /**
         * Matrix4x4.make_rotation_x(), .make_rotation_y() and
         * .make_rotation_z(), which differ only in which of the three
         * C++/WinRT functions they call.
         */
        template<
            num::float4x4 (*About)(float),
            num::float4x4 (*AboutCentered)(float, num::float3 const&)>
        PyObject* make_matrix4x4_rotation(
            PyObject* self, PyObject* args, char const* name) noexcept
        {
            auto const entry = static_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            switch (PyTuple_GET_SIZE(args))
            {
            case 1:
                return call_static<kind::matrix4x4, float>(
                    *entry,
                    args,
                    [](float radians)
                    {
                        return About(radians);
                    });
            case 2:
                return call_static<kind::matrix4x4, float, num::float3>(
                    *entry,
                    args,
                    [](float radians, num::float3 const& center)
                    {
                        return AboutCentered(radians, center);
                    });
            }

            return no_overload(name, PyTuple_GET_SIZE(args));
        }
#endif

        PyObject* make_matrix4x4_rotation_x(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_rotation_x");
#else
            return make_matrix4x4_rotation<
                num::make_float4x4_rotation_x,
                num::make_float4x4_rotation_x>(self, args, "make_rotation_x");
#endif
        }

        PyObject* make_matrix4x4_rotation_y(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_rotation_y");
#else
            return make_matrix4x4_rotation<
                num::make_float4x4_rotation_y,
                num::make_float4x4_rotation_y>(self, args, "make_rotation_y");
#endif
        }

        PyObject* make_matrix4x4_rotation_z(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_rotation_z");
#else
            return make_matrix4x4_rotation<
                num::make_float4x4_rotation_z,
                num::make_float4x4_rotation_z>(self, args, "make_rotation_z");
#endif
        }

        PyObject* make_matrix4x4_from_axis_angle(
            PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_from_axis_angle");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_from_axis_angle,
                num::float3,
                float>(self, args, "make_from_axis_angle");
#endif
        }

        PyObject* make_matrix4x4_perspective_field_of_view(
            PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_perspective_field_of_view");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_perspective_field_of_view,
                float,
                float,
                float,
                float>(self, args, "make_perspective_field_of_view");
#endif
        }

        PyObject* make_matrix4x4_perspective(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_perspective");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_perspective,
                float,
                float,
                float,
                float>(self, args, "make_perspective");
#endif
        }

        PyObject* make_matrix4x4_perspective_off_center(
            PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_perspective_off_center");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_perspective_off_center,
                float,
                float,
                float,
                float,
                float,
                float>(self, args, "make_perspective_off_center");
#endif
        }

        PyObject* make_matrix4x4_orthographic(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_orthographic");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_orthographic,
                float,
                float,
                float,
                float>(self, args, "make_orthographic");
#endif
        }

        PyObject* make_matrix4x4_orthographic_off_center(
            PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_orthographic_off_center");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_orthographic_off_center,
                float,
                float,
                float,
                float,
                float,
                float>(self, args, "make_orthographic_off_center");
#endif
        }

        PyObject* make_matrix4x4_look_at(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_look_at");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_look_at,
                num::float3,
                num::float3,
                num::float3>(self, args, "make_look_at");
#endif
        }

        PyObject* make_matrix4x4_world(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_world");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_world,
                num::float3,
                num::float3,
                num::float3>(self, args, "make_world");
#endif
        }

        PyObject* make_matrix4x4_from_quaternion(
            PyObject* self, PyObject* args) noexcept
        {
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_from_quaternion,
                num::quaternion>(self, args, "make_from_quaternion");
        }

        PyObject* make_matrix4x4_from_yaw_pitch_roll(
            PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_from_yaw_pitch_roll");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_from_yaw_pitch_roll,
                float,
                float,
                float>(self, args, "make_from_yaw_pitch_roll");
#endif
        }

        PyObject* make_matrix4x4_shadow(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_shadow");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_shadow,
                num::float3,
                num::plane>(self, args, "make_shadow");
#endif
        }

        PyObject* make_matrix4x4_reflection(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_reflection");
#else
            return make_fixed<
                kind::matrix4x4,
                num::make_float4x4_reflection,
                num::plane>(self, args, "make_reflection");
#endif
        }

        PyObject* make_plane_from_vertices(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("make_from_vertices");
#else
            return make_fixed<
                kind::plane,
                num::make_plane_from_vertices,
                num::float3,
                num::float3,
                num::float3>(self, args, "make_from_vertices");
#endif
        }

        PyObject* make_quaternion_from_axis_angle(
            PyObject* self, PyObject* args) noexcept
        {
            return make_fixed<
                kind::quaternion,
                num::make_quaternion_from_axis_angle,
                num::float3,
                float>(self, args, "make_from_axis_angle");
        }

        PyObject* make_quaternion_from_yaw_pitch_roll(
            PyObject* self, PyObject* args) noexcept
        {
            return make_fixed<
                kind::quaternion,
                num::make_quaternion_from_yaw_pitch_roll,
                float,
                float,
                float>(self, args, "make_from_yaw_pitch_roll");
        }

        PyObject* make_quaternion_from_rotation_matrix(
            PyObject* self, PyObject* args) noexcept
        {
            return make_fixed<
                kind::quaternion,
                num::make_quaternion_from_rotation_matrix,
                num::float4x4>(self, args, "make_from_rotation_matrix");
        }

        PyMethodDef matrix3x2_factories[]
            = {{"make_translation", make_matrix3x2_translation, METH_VARARGS, nullptr},
               {"make_scale", make_matrix3x2_scale, METH_VARARGS, nullptr},
               {"make_scale_from_vector",
                make_matrix3x2_scale_from_vector,
                METH_VARARGS,
                nullptr},
               {"make_scale_from_scalar",
                make_matrix3x2_scale_from_scalar,
                METH_VARARGS,
                nullptr},
               {"make_skew", make_matrix3x2_skew, METH_VARARGS, nullptr},
               {"make_rotation", make_matrix3x2_rotation, METH_VARARGS, nullptr},
               {}};

        PyMethodDef matrix4x4_factories[] = {
            {"make_billboard", make_matrix4x4_billboard, METH_VARARGS, nullptr},
            {"make_constrained_billboard",
             make_matrix4x4_constrained_billboard,
             METH_VARARGS,
             nullptr},
            {"make_translation", make_matrix4x4_translation, METH_VARARGS, nullptr},
            {"make_scale", make_matrix4x4_scale, METH_VARARGS, nullptr},
            {"make_scale_from_vector",
             make_matrix4x4_scale_from_vector,
             METH_VARARGS,
             nullptr},
            {"make_scale_from_scalar",
             make_matrix4x4_scale_from_scalar,
             METH_VARARGS,
             nullptr},
            {"make_rotation_x", make_matrix4x4_rotation_x, METH_VARARGS, nullptr},
            {"make_rotation_y", make_matrix4x4_rotation_y, METH_VARARGS, nullptr},
            {"make_rotation_z", make_matrix4x4_rotation_z, METH_VARARGS, nullptr},
            {"make_from_axis_angle",
             make_matrix4x4_from_axis_angle,
             METH_VARARGS,
             nullptr},
            {"make_perspective_field_of_view",
             make_matrix4x4_perspective_field_of_view,
             METH_VARARGS,
             nullptr},
            {"make_perspective", make_matrix4x4_perspective, METH_VARARGS, nullptr},
            {"make_perspective_off_center",
             make_matrix4x4_perspective_off_center,
             METH_VARARGS,
             nullptr},
            {"make_orthographic", make_matrix4x4_orthographic, METH_VARARGS, nullptr},
            {"make_orthographic_off_center",
             make_matrix4x4_orthographic_off_center,
             METH_VARARGS,
             nullptr},
            {"make_look_at", make_matrix4x4_look_at, METH_VARARGS, nullptr},
            {"make_world", make_matrix4x4_world, METH_VARARGS, nullptr},
            {"make_from_quaternion",
             make_matrix4x4_from_quaternion,
             METH_VARARGS,
             nullptr},
            {"make_from_yaw_pitch_roll",
             make_matrix4x4_from_yaw_pitch_roll,
             METH_VARARGS,
             nullptr},
            {"make_shadow", make_matrix4x4_shadow, METH_VARARGS, nullptr},
            {"make_reflection", make_matrix4x4_reflection, METH_VARARGS, nullptr},
            {}};

        PyMethodDef plane_factories[]
            = {{"make_from_vertices", make_plane_from_vertices, METH_VARARGS, nullptr},
               {}};

        PyMethodDef quaternion_factories[]
            = {{"make_from_axis_angle",
                make_quaternion_from_axis_angle,
                METH_VARARGS,
                nullptr},
               {"make_from_yaw_pitch_roll",
                make_quaternion_from_yaw_pitch_roll,
                METH_VARARGS,
                nullptr},
               {"make_from_rotation_matrix",
                make_quaternion_from_rotation_matrix,
                METH_VARARGS,
                nullptr},
               {}};

        /**
         * The factory functions of the struct @p which, or @c nullptr for a
         * struct that is only ever built from its fields.
         */
        PyMethodDef* factories_of(kind which) noexcept
        {
            switch (which)
            {
            case kind::matrix3x2:
                return matrix3x2_factories;
            case kind::matrix4x4:
                return matrix4x4_factories;
            case kind::plane:
                return plane_factories;
            case kind::quaternion:
                return quaternion_factories;
            case kind::vector2:
            case kind::vector3:
            case kind::vector4:
            case kind::none:
                break;
            }

            return nullptr;
        }
    } // namespace

    /**
     * Whether the struct @p which has a member that belongs to the type rather
     * than to a value: a constant, or a factory function.
     */
    bool has_statics(kind which) noexcept
    {
        if (constants_of(which))
        {
            return true;
        }

        return factories_of(which) != nullptr;
    }

    /**
     * Creates the metaclass that carries the constants and the factory
     * functions of the struct @p which.
     *
     * They are members of the type rather than of a value - Vector3.zero,
     * Matrix4x4.make_look_at(...) - which in Python means members of the
     * type's type, so they cannot be added once the type exists. That is why
     * this is called while structs.cpp is building the type rather than after
     * it has.
     */
    PyTypeObject* make_statics(projection& proj, type_entry& entry, kind which)
    {
        std::vector<PyType_Slot> slots;

        if (auto* const constants = constants_of(which))
        {
            slots.push_back({Py_tp_getset, reinterpret_cast<void*>(constants)});
        }

        if (auto* const factories = factories_of(which))
        {
            slots.push_back({Py_tp_methods, reinterpret_cast<void*>(factories)});
        }

        slots.push_back({});

        PyType_Spec spec{
            keep(proj, entry.tp_name + "_Static"),
            static_cast<int>(PyType_Type.tp_basicsize),
            static_cast<int>(PyType_Type.tp_itemsize),
            Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
            slots.data()};

        pyobj_handle bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(&PyType_Type))};
        if (!bases)
        {
            return nullptr;
        }

        return reinterpret_cast<PyTypeObject*>(
            PyType_FromSpecWithBases(&spec, bases.get()));
    }
} // namespace py::interp::numerics
