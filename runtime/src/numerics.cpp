// The arithmetic and the methods of the Windows.Foundation.Numerics structs,
// which no winmd has.
//
// Vector2, Vector3, Vector4, Matrix3x2, Matrix4x4, Plane and Quaternion are
// plain structs of floats in metadata: the fields are all that is written
// down, so structs.cpp builds the Python type from a table like any other.
// What one can do with a value of one is not written down anywhere a
// projection can read - C++/WinRT declares it in <windowsnumerics.impl.h> -
// so every member here calls a function of that header, and which members a
// type gets is keyed on the name the table gives it. The constants and the
// factory functions, which belong to the type rather than to a value, are in
// numerics-statics.cpp.
//
// The functions of <windowsnumerics.impl.h> are overloaded on the C++ type of
// the argument. Python has one name per member, so an overload set is chosen
// by the Python type of the argument, which is what transform() and its
// neighbours do.
//
// The mingw-w64 headers declare the same functions and only some of them
// resolve at link time, so the ones they leave out raise NotImplementedError
// on that toolchain rather than keeping the runtime from linking at all.

#include "numerics-values.h"

#include <optional>
#include <string_view>
#include <vector>

namespace py::interp::numerics
{
    namespace
    {
        /**
         * The entry of the struct @p self is an instance of.
         */
        type_entry* instance_entry(PyObject* self) noexcept
        {
            auto const entry = find_type_entry(Py_TYPE(self));
            if (!entry)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' is not a type a projection table built",
                    Py_TYPE(self)->tp_name);
                return nullptr;
            }

            return entry;
        }

        /**
         * The entry of the struct whose number slot is being called, found
         * from whichever operand is of it.
         *
         * CPython hands a number slot the operands in the order they were
         * written and tries the right one's slot when the left one's answers
         * NotImplemented, so either of the two may be the value the slot
         * belongs to and the other may be anything at all.
         */
        template<kind K>
        type_entry* operand_entry(PyObject* left, PyObject* right) noexcept
        {
            for (auto* const operand : {left, right})
            {
                auto const entry = find_type_entry(Py_TYPE(operand));
                if (!entry)
                {
                    continue;
                }

                if (entry->numerics_kind != K)
                {
                    continue;
                }

                return entry;
            }

            return nullptr;
        }

        /**
         * The value of @p self, which is an instance of the struct @p entry
         * describes.
         */
        template<kind K>
        typename traits<K>::type instance_value(
            type_entry& entry, PyObject* self) noexcept
        {
            typename traits<K>::type value;
            std::memcpy(
                &value,
                reinterpret_cast<uint8_t const*>(self) + entry.blob_offset,
                sizeof(value));

            return value;
        }

        /**
         * The value of @p obj if it is an instance of the struct @p Wanted,
         * and nothing if it is of any other type.
         *
         * This is the test an overload that C++/WinRT spells by parameter type
         * is chosen by. It is an exact type check rather than a conversion,
         * because two overloads of one Python name would otherwise both accept
         * the same tuple.
         */
        template<kind Known, kind Wanted>
        std::optional<typename traits<Wanted>::type> match_struct(
            type_entry& known, PyObject* obj) noexcept
        {
            auto const entry = entry_for<Known, Wanted>(known);
            if (!entry)
            {
                return std::nullopt;
            }

            if (!Py_IS_TYPE(obj, entry->py_type))
            {
                return std::nullopt;
            }

            typename traits<Wanted>::type value;
            std::memcpy(
                &value,
                reinterpret_cast<uint8_t const*>(obj) + entry->blob_offset,
                sizeof(value));

            return value;
        }

        /**
         * Calls @p fn on the value of @p self and the arguments in @p args.
         */
        template<kind K, typename... Args, typename Fn>
        PyObject* call_on(PyObject* self, PyObject* const* args, Fn fn) noexcept
        {
            auto const entry = instance_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const value = instance_value<K>(*entry, self);

            return apply<K, Args...>(
                *entry,
                args,
                [&](auto const&... rest)
                {
                    return fn(value, rest...);
                });
        }

        /**
         * The arguments of a member that takes a fixed number of them, out of
         * the tuple they arrived in.
         */
        template<size_t N>
        bool unpack_args(
            PyObject* args, char const* name, std::array<PyObject*, N>& items) noexcept
        {
            if (!check_count(args, static_cast<Py_ssize_t>(N), name))
            {
                return false;
            }

            for (size_t i = 0; i < N; i++)
            {
                items[i] = PyTuple_GET_ITEM(args, static_cast<Py_ssize_t>(i));
            }

            return true;
        }

        /**
         * The error a member with several overloads raises when the argument
         * is of a type none of them takes.
         */
        PyObject* no_overload_for(char const* name, PyObject* arg) noexcept
        {
            PyErr_Format(
                PyExc_TypeError,
                "no overload of %s() takes a '%s'",
                name,
                Py_TYPE(arg)->tp_name);
            return nullptr;
        }

        // ----- the number slots -------------------------------------------

        /**
         * NotImplemented, which is what makes CPython raise the message that
         * names both operands, or @c nullptr when an operand could not be read
         * for some reason other than its type.
         */
        PyObject* not_implemented() noexcept
        {
            if (PyErr_Occurred())
            {
                return nullptr;
            }

            Py_RETURN_NOTIMPLEMENTED;
        }

        /**
         * Reads one operand of a number slot, clearing the error when it is of
         * a type the operator does not take so that the slot can hand back
         * NotImplemented.
         */
        template<kind K>
        bool read_operand(
            type_entry& entry, PyObject* obj, typename traits<K>::type& out) noexcept
        {
            if (read_struct<K, K>(entry, obj, out))
            {
                return true;
            }

            if (PyErr_ExceptionMatches(PyExc_TypeError))
            {
                PyErr_Clear();
            }

            return false;
        }

        template<kind K>
        PyObject* slot_add(PyObject* left, PyObject* right) noexcept
        {
            auto const entry = operand_entry<K>(left, right);
            if (!entry)
            {
                Py_RETURN_NOTIMPLEMENTED;
            }

            typename traits<K>::type a;
            if (!read_operand<K>(*entry, left, a))
            {
                return not_implemented();
            }

            typename traits<K>::type b;
            if (!read_operand<K>(*entry, right, b))
            {
                return not_implemented();
            }

            return write_struct<K, K>(*entry, a + b);
        }

        template<kind K>
        PyObject* slot_subtract(PyObject* left, PyObject* right) noexcept
        {
            auto const entry = operand_entry<K>(left, right);
            if (!entry)
            {
                Py_RETURN_NOTIMPLEMENTED;
            }

            typename traits<K>::type a;
            if (!read_operand<K>(*entry, left, a))
            {
                return not_implemented();
            }

            typename traits<K>::type b;
            if (!read_operand<K>(*entry, right, b))
            {
                return not_implemented();
            }

            return write_struct<K, K>(*entry, a - b);
        }

        /**
         * The value of @p obj if it is a number, and nothing otherwise. What
         * counts is what float() of it would be, so an int scales a vector.
         */
        std::optional<float> as_number(PyObject* obj) noexcept
        {
            pyobj_handle number{PyNumber_Float(obj)};
            if (!number)
            {
                PyErr_Clear();
                return std::nullopt;
            }

            return static_cast<float>(PyFloat_AS_DOUBLE(number.get()));
        }

        /**
         * Multiplication, of two values of the struct or of one and a number.
         *
         * @tparam ScalarLeft Whether a number on the left scales the value,
         * which the vectors offer and the matrices and the quaternion do not.
         */
        template<kind K, bool ScalarLeft>
        PyObject* slot_multiply(PyObject* left, PyObject* right) noexcept
        {
            auto const entry = operand_entry<K>(left, right);
            if (!entry)
            {
                Py_RETURN_NOTIMPLEMENTED;
            }

            if constexpr (ScalarLeft)
            {
                if (auto const scale = as_number(left))
                {
                    typename traits<K>::type value;
                    if (!read_operand<K>(*entry, right, value))
                    {
                        return not_implemented();
                    }

                    return write_struct<K, K>(*entry, *scale * value);
                }
            }

            typename traits<K>::type a;
            if (!read_operand<K>(*entry, left, a))
            {
                return not_implemented();
            }

            if (auto const scale = as_number(right))
            {
                return write_struct<K, K>(*entry, a * *scale);
            }

            typename traits<K>::type b;
            if (!read_operand<K>(*entry, right, b))
            {
                return not_implemented();
            }

            return write_struct<K, K>(*entry, a * b);
        }

        /**
         * Division, of two values of the struct or of one by a number.
         *
         * @tparam ByScalar Whether a number on the right divides the value,
         * which the vectors offer and the quaternion does not.
         */
        template<kind K, bool ByScalar>
        PyObject* slot_true_divide(PyObject* left, PyObject* right) noexcept
        {
            auto const entry = operand_entry<K>(left, right);
            if (!entry)
            {
                Py_RETURN_NOTIMPLEMENTED;
            }

            typename traits<K>::type a;
            if (!read_operand<K>(*entry, left, a))
            {
                return not_implemented();
            }

            if constexpr (ByScalar)
            {
                if (auto const divisor = as_number(right))
                {
                    return write_struct<K, K>(*entry, a / *divisor);
                }
            }

            typename traits<K>::type b;
            if (!read_operand<K>(*entry, right, b))
            {
                return not_implemented();
            }

            return write_struct<K, K>(*entry, a / b);
        }

        template<kind K>
        PyObject* slot_negative(PyObject* operand) noexcept
        {
            auto const entry = instance_entry(operand);
            if (!entry)
            {
                return nullptr;
            }

            return write_struct<K, K>(*entry, -instance_value<K>(*entry, operand));
        }

        /**
         * abs(), which is how long the value is.
         */
        template<kind K>
        PyObject* slot_absolute(PyObject* operand) noexcept
        {
            auto const entry = instance_entry(operand);
            if (!entry)
            {
                return nullptr;
            }

            return convert(num::length(instance_value<K>(*entry, operand)));
        }

        /**
         * The slots of a vector, which are every operator the structs have:
         * both orders of a scalar multiplication, division by a vector or by a
         * number, and abs().
         */
        template<kind K>
        void push_vector_slots(std::vector<PyType_Slot>& slots)
        {
            slots.push_back({Py_nb_add, reinterpret_cast<void*>(slot_add<K>)});
            slots.push_back(
                {Py_nb_subtract, reinterpret_cast<void*>(slot_subtract<K>)});
            slots.push_back(
                {Py_nb_multiply, reinterpret_cast<void*>(slot_multiply<K, true>)});
            slots.push_back(
                {Py_nb_true_divide,
                 reinterpret_cast<void*>(slot_true_divide<K, true>)});
            slots.push_back(
                {Py_nb_negative, reinterpret_cast<void*>(slot_negative<K>)});
            slots.push_back(
                {Py_nb_absolute, reinterpret_cast<void*>(slot_absolute<K>)});
        }

        /**
         * The slots of a matrix, which has no division and no length, and
         * whose scalar multiplication takes the number on the right only.
         */
        template<kind K>
        void push_matrix_slots(std::vector<PyType_Slot>& slots)
        {
            slots.push_back({Py_nb_add, reinterpret_cast<void*>(slot_add<K>)});
            slots.push_back(
                {Py_nb_subtract, reinterpret_cast<void*>(slot_subtract<K>)});
            slots.push_back(
                {Py_nb_multiply, reinterpret_cast<void*>(slot_multiply<K, false>)});
            slots.push_back(
                {Py_nb_negative, reinterpret_cast<void*>(slot_negative<K>)});
        }

        /**
         * The slots of a quaternion, which divides by another quaternion and
         * by no number, and whose scalar multiplication takes the number on
         * the right only.
         */
        void push_quaternion_slots(std::vector<PyType_Slot>& slots)
        {
            constexpr auto k = kind::quaternion;

            slots.push_back({Py_nb_add, reinterpret_cast<void*>(slot_add<k>)});
            slots.push_back(
                {Py_nb_subtract, reinterpret_cast<void*>(slot_subtract<k>)});
            slots.push_back(
                {Py_nb_multiply, reinterpret_cast<void*>(slot_multiply<k, false>)});
            slots.push_back(
                {Py_nb_true_divide,
                 reinterpret_cast<void*>(slot_true_divide<k, false>)});
            slots.push_back(
                {Py_nb_negative, reinterpret_cast<void*>(slot_negative<k>)});
            slots.push_back(
                {Py_nb_absolute, reinterpret_cast<void*>(slot_absolute<k>)});
        }

        // ----- the methods ------------------------------------------------

        template<kind K>
        PyObject* method_length(PyObject* self, PyObject* /*unused*/) noexcept
        {
            return call_on<K>(
                self,
                nullptr,
                [](auto const& value)
                {
                    return num::length(value);
                });
        }

        template<kind K>
        PyObject* method_length_squared(PyObject* self, PyObject* /*unused*/) noexcept
        {
            return call_on<K>(
                self,
                nullptr,
                [](auto const& value)
                {
                    return num::length_squared(value);
                });
        }

        template<kind K>
        PyObject* method_distance(PyObject* self, PyObject* arg) noexcept
        {
            return call_on<K, typename traits<K>::type>(
                self,
                &arg,
                [](auto const& a, auto const& b)
                {
                    return num::distance(a, b);
                });
        }

        template<kind K>
        PyObject* method_distance_squared(PyObject* self, PyObject* arg) noexcept
        {
            return call_on<K, typename traits<K>::type>(
                self,
                &arg,
                [](auto const& a, auto const& b)
                {
                    return num::distance_squared(a, b);
                });
        }

        template<kind K>
        PyObject* method_dot(PyObject* self, PyObject* arg) noexcept
        {
            return call_on<K, typename traits<K>::type>(
                self,
                &arg,
                [](auto const& a, auto const& b)
                {
                    return num::dot(a, b);
                });
        }

        template<kind K>
        PyObject* method_normalize(PyObject* self, PyObject* /*unused*/) noexcept
        {
            return call_on<K>(
                self,
                nullptr,
                [](auto const& value)
                {
                    return num::normalize(value);
                });
        }

        template<kind K>
        PyObject* method_reflect(PyObject* self, PyObject* arg) noexcept
        {
            return call_on<K, typename traits<K>::type>(
                self,
                &arg,
                [](auto const& vector, auto const& normal)
                {
                    return num::reflect(vector, normal);
                });
        }

        template<kind K>
        PyObject* method_min(PyObject* self, PyObject* arg) noexcept
        {
            return call_on<K, typename traits<K>::type>(
                self,
                &arg,
                [](auto const& a, auto const& b)
                {
                    return (num::min)(a, b);
                });
        }

        template<kind K>
        PyObject* method_max(PyObject* self, PyObject* arg) noexcept
        {
            return call_on<K, typename traits<K>::type>(
                self,
                &arg,
                [](auto const& a, auto const& b)
                {
                    return (num::max)(a, b);
                });
        }

        template<kind K>
        PyObject* method_clamp(PyObject* self, PyObject* args) noexcept
        {
            using value_type = typename traits<K>::type;

            std::array<PyObject*, 2> items{};
            if (!unpack_args(args, "clamp", items))
            {
                return nullptr;
            }

            return call_on<K, value_type, value_type>(
                self,
                items.data(),
                [](auto const& value, auto const& low, auto const& high)
                {
                    return num::clamp(value, low, high);
                });
        }

        template<kind K>
        PyObject* method_lerp(PyObject* self, PyObject* args) noexcept
        {
            std::array<PyObject*, 2> items{};
            if (!unpack_args(args, "lerp", items))
            {
                return nullptr;
            }

            return call_on<K, typename traits<K>::type, float>(
                self,
                items.data(),
                [](auto const& a, auto const& b, float amount)
                {
                    return num::lerp(a, b, amount);
                });
        }

        /**
         * lerp() between two matrices, which mingw-w64 leaves undefined.
         */
        template<kind K>
        PyObject* method_matrix_lerp(PyObject* self, PyObject* args) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)args;
            return not_on_mingw("lerp");
#else
            return method_lerp<K>(self, args);
#endif
        }

        PyObject* method_cross(PyObject* self, PyObject* arg) noexcept
        {
            return call_on<kind::vector3, num::float3>(
                self,
                &arg,
                [](auto const& a, auto const& b)
                {
                    return num::cross(a, b);
                });
        }

        /**
         * Vector2.transform(), which takes either matrix or a rotation.
         */
        PyObject* method_vector2_transform(PyObject* self, PyObject* arg) noexcept
        {
            constexpr auto self_kind = kind::vector2;

            auto const entry = instance_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const value = instance_value<self_kind>(*entry, self);

            if (auto const rotation
                = match_struct<self_kind, kind::quaternion>(*entry, arg))
            {
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform(value, *rotation));
            }

            if (auto const matrix
                = match_struct<self_kind, kind::matrix3x2>(*entry, arg))
            {
#if defined(__MINGW32__)
                return not_on_mingw("transform");
#else
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform(value, *matrix));
#endif
            }

            if (auto const matrix
                = match_struct<self_kind, kind::matrix4x4>(*entry, arg))
            {
#if defined(__MINGW32__)
                return not_on_mingw("transform");
#else
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform(value, *matrix));
#endif
            }

            return no_overload_for("transform", arg);
        }

        /**
         * Vector2.transform_normal(), which mingw-w64 leaves undefined for
         * both of the matrices it takes.
         */
        PyObject* method_vector2_transform_normal(
            PyObject* self, PyObject* arg) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)arg;
            return not_on_mingw("transform_normal");
#else
            constexpr auto self_kind = kind::vector2;

            auto const entry = instance_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const value = instance_value<self_kind>(*entry, self);

            if (auto const matrix
                = match_struct<self_kind, kind::matrix3x2>(*entry, arg))
            {
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform_normal(value, *matrix));
            }

            if (auto const matrix
                = match_struct<self_kind, kind::matrix4x4>(*entry, arg))
            {
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform_normal(value, *matrix));
            }

            return no_overload_for("transform_normal", arg);
#endif
        }

        /**
         * Vector2.transform4() and Vector3.transform4(), which widen the
         * transformed value to a Vector4.
         */
        template<kind K>
        PyObject* method_transform4(PyObject* self, PyObject* arg) noexcept
        {
            auto const entry = instance_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const value = instance_value<K>(*entry, self);

            if (auto const rotation = match_struct<K, kind::quaternion>(*entry, arg))
            {
                return write_struct<K, kind::vector4>(
                    *entry, num::transform4(value, *rotation));
            }

            if (auto const matrix = match_struct<K, kind::matrix4x4>(*entry, arg))
            {
#if defined(__MINGW32__)
                return not_on_mingw("transform4");
#else
                return write_struct<K, kind::vector4>(
                    *entry, num::transform4(value, *matrix));
#endif
            }

            return no_overload_for("transform4", arg);
        }

        /**
         * Vector3.transform(), whose two overloads mingw-w64 both leave
         * undefined.
         */
        PyObject* method_vector3_transform(PyObject* self, PyObject* arg) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)arg;
            return not_on_mingw("transform");
#else
            constexpr auto self_kind = kind::vector3;

            auto const entry = instance_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const value = instance_value<self_kind>(*entry, self);

            if (auto const matrix
                = match_struct<self_kind, kind::matrix4x4>(*entry, arg))
            {
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform(value, *matrix));
            }

            if (auto const rotation
                = match_struct<self_kind, kind::quaternion>(*entry, arg))
            {
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform(value, *rotation));
            }

            return no_overload_for("transform", arg);
#endif
        }

        /**
         * Vector3.transform_normal(), which mingw-w64 leaves undefined.
         */
        PyObject* method_vector3_transform_normal(
            PyObject* self, PyObject* arg) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)arg;
            return not_on_mingw("transform_normal");
#else
            return call_on<kind::vector3, num::float4x4>(
                self,
                &arg,
                [](auto const& normal, auto const& matrix)
                {
                    return num::transform_normal(normal, matrix);
                });
#endif
        }

        /**
         * Vector4.transform(), which takes a matrix or a rotation.
         */
        PyObject* method_vector4_transform(PyObject* self, PyObject* arg) noexcept
        {
            constexpr auto self_kind = kind::vector4;

            auto const entry = instance_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const value = instance_value<self_kind>(*entry, self);

            if (auto const rotation
                = match_struct<self_kind, kind::quaternion>(*entry, arg))
            {
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform(value, *rotation));
            }

            if (auto const matrix
                = match_struct<self_kind, kind::matrix4x4>(*entry, arg))
            {
#if defined(__MINGW32__)
                return not_on_mingw("transform");
#else
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform(value, *matrix));
#endif
            }

            return no_overload_for("transform", arg);
        }

        template<kind K>
        PyObject* method_is_identity(PyObject* self, PyObject* /*unused*/) noexcept
        {
            return call_on<K>(
                self,
                nullptr,
                [](auto const& value)
                {
                    return num::is_identity(value);
                });
        }

        template<kind K>
        PyObject* method_determinant(PyObject* self, PyObject* /*unused*/) noexcept
        {
            return call_on<K>(
                self,
                nullptr,
                [](auto const& value)
                {
                    return num::determinant(value);
                });
        }

        /**
         * translation(): the offset a matrix moves a point by, which is a
         * Vector2 of a Matrix3x2 and a Vector3 of a Matrix4x4.
         */
        template<kind K>
        PyObject* method_translation(PyObject* self, PyObject* /*unused*/) noexcept
        {
            return call_on<K>(
                self,
                nullptr,
                [](auto const& value)
                {
                    return num::translation(value);
                });
        }

        /**
         * invert(): the matrix that undoes this one, or a ValueError when
         * there is none.
         */
        template<kind K>
        PyObject* method_invert(PyObject* self, PyObject* /*unused*/) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            return not_on_mingw("invert");
#else
            auto const entry = instance_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const value = instance_value<K>(*entry, self);

            typename traits<K>::type result{};
            if (!num::invert(value, &result))
            {
                PyErr_SetString(PyExc_ValueError, "the matrix is not invertible");
                return nullptr;
            }

            return write_struct<K, K>(*entry, result);
#endif
        }

        /**
         * decompose(): the scale, the rotation and the translation a transform
         * is made of, or a ValueError when it is not made of them.
         */
        PyObject* method_decompose(PyObject* self, PyObject* /*unused*/) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            return not_on_mingw("decompose");
#else
            constexpr auto self_kind = kind::matrix4x4;

            auto const entry = instance_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const value = instance_value<self_kind>(*entry, self);

            num::float3 scale{};
            num::quaternion rotation{};
            num::float3 translation{};
            if (!num::decompose(value, &scale, &rotation, &translation))
            {
                PyErr_SetString(PyExc_ValueError, "the matrix is not decomposable");
                return nullptr;
            }

            pyobj_handle first{write_struct<self_kind, kind::vector3>(*entry, scale)};
            if (!first)
            {
                return nullptr;
            }

            pyobj_handle second{
                write_struct<self_kind, kind::quaternion>(*entry, rotation)};
            if (!second)
            {
                return nullptr;
            }

            pyobj_handle third{
                write_struct<self_kind, kind::vector3>(*entry, translation)};
            if (!third)
            {
                return nullptr;
            }

            return PyTuple_Pack(3, first.get(), second.get(), third.get());
#endif
        }

        /**
         * Matrix4x4.transform(): the matrix turned by a rotation.
         */
        PyObject* method_matrix4x4_transform(PyObject* self, PyObject* arg) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)arg;
            return not_on_mingw("transform");
#else
            return call_on<kind::matrix4x4, num::quaternion>(
                self,
                &arg,
                [](auto const& value, auto const& rotation)
                {
                    return num::transform(value, rotation);
                });
#endif
        }

        PyObject* method_transpose(PyObject* self, PyObject* /*unused*/) noexcept
        {
            return call_on<kind::matrix4x4>(
                self,
                nullptr,
                [](auto const& value)
                {
                    return num::transpose(value);
                });
        }

        /**
         * Plane.transform(), which takes a matrix or a rotation.
         */
        PyObject* method_plane_transform(PyObject* self, PyObject* arg) noexcept
        {
            constexpr auto self_kind = kind::plane;

            auto const entry = instance_entry(self);
            if (!entry)
            {
                return nullptr;
            }

            auto const value = instance_value<self_kind>(*entry, self);

            if (auto const rotation
                = match_struct<self_kind, kind::quaternion>(*entry, arg))
            {
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform(value, *rotation));
            }

            if (auto const matrix
                = match_struct<self_kind, kind::matrix4x4>(*entry, arg))
            {
#if defined(__MINGW32__)
                return not_on_mingw("transform");
#else
                return write_struct<self_kind, self_kind>(
                    *entry, num::transform(value, *matrix));
#endif
            }

            return no_overload_for("transform", arg);
        }

        /**
         * Plane.dot(), which is the plane against a Vector4 rather than
         * against another plane.
         */
        PyObject* method_plane_dot(PyObject* self, PyObject* arg) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)arg;
            return not_on_mingw("dot");
#else
            return call_on<kind::plane, num::float4>(
                self,
                &arg,
                [](auto const& plane, auto const& value)
                {
                    return num::dot(plane, value);
                });
#endif
        }

        PyObject* method_dot_coordinate(PyObject* self, PyObject* arg) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)arg;
            return not_on_mingw("dot_coordinate");
#else
            return call_on<kind::plane, num::float3>(
                self,
                &arg,
                [](auto const& plane, auto const& value)
                {
                    return num::dot_coordinate(plane, value);
                });
#endif
        }

        PyObject* method_dot_normal(PyObject* self, PyObject* arg) noexcept
        {
#if defined(__MINGW32__)
            (void)self;
            (void)arg;
            return not_on_mingw("dot_normal");
#else
            return call_on<kind::plane, num::float3>(
                self,
                &arg,
                [](auto const& plane, auto const& value)
                {
                    return num::dot_normal(plane, value);
                });
#endif
        }

        PyObject* method_conjugate(PyObject* self, PyObject* /*unused*/) noexcept
        {
            return call_on<kind::quaternion>(
                self,
                nullptr,
                [](auto const& value)
                {
                    return num::conjugate(value);
                });
        }

        PyObject* method_inverse(PyObject* self, PyObject* /*unused*/) noexcept
        {
            return call_on<kind::quaternion>(
                self,
                nullptr,
                [](auto const& value)
                {
                    return num::inverse(value);
                });
        }

        PyObject* method_slerp(PyObject* self, PyObject* args) noexcept
        {
            std::array<PyObject*, 2> items{};
            if (!unpack_args(args, "slerp", items))
            {
                return nullptr;
            }

            return call_on<kind::quaternion, num::quaternion, float>(
                self,
                items.data(),
                [](auto const& a, auto const& b, float amount)
                {
                    return num::slerp(a, b, amount);
                });
        }

        PyObject* method_concatenate(PyObject* self, PyObject* arg) noexcept
        {
            return call_on<kind::quaternion, num::quaternion>(
                self,
                &arg,
                [](auto const& a, auto const& b)
                {
                    return num::concatenate(a, b);
                });
        }

        // ----- what each struct offers ------------------------------------

        PyMethodDef vector2_methods[]
            = {{"length", method_length<kind::vector2>, METH_NOARGS, nullptr},
               {"length_squared",
                method_length_squared<kind::vector2>,
                METH_NOARGS,
                nullptr},
               {"distance", method_distance<kind::vector2>, METH_O, nullptr},
               {"distance_squared",
                method_distance_squared<kind::vector2>,
                METH_O,
                nullptr},
               {"dot", method_dot<kind::vector2>, METH_O, nullptr},
               {"normalize", method_normalize<kind::vector2>, METH_NOARGS, nullptr},
               {"reflect", method_reflect<kind::vector2>, METH_O, nullptr},
               {"min", method_min<kind::vector2>, METH_O, nullptr},
               {"max", method_max<kind::vector2>, METH_O, nullptr},
               {"clamp", method_clamp<kind::vector2>, METH_VARARGS, nullptr},
               {"lerp", method_lerp<kind::vector2>, METH_VARARGS, nullptr},
               {"transform", method_vector2_transform, METH_O, nullptr},
               {"transform_normal", method_vector2_transform_normal, METH_O, nullptr},
               {"transform4", method_transform4<kind::vector2>, METH_O, nullptr},
               {}};

        PyMethodDef vector3_methods[]
            = {{"length", method_length<kind::vector3>, METH_NOARGS, nullptr},
               {"length_squared",
                method_length_squared<kind::vector3>,
                METH_NOARGS,
                nullptr},
               {"distance", method_distance<kind::vector3>, METH_O, nullptr},
               {"distance_squared",
                method_distance_squared<kind::vector3>,
                METH_O,
                nullptr},
               {"dot", method_dot<kind::vector3>, METH_O, nullptr},
               {"cross", method_cross, METH_O, nullptr},
               {"normalize", method_normalize<kind::vector3>, METH_NOARGS, nullptr},
               {"reflect", method_reflect<kind::vector3>, METH_O, nullptr},
               {"min", method_min<kind::vector3>, METH_O, nullptr},
               {"max", method_max<kind::vector3>, METH_O, nullptr},
               {"clamp", method_clamp<kind::vector3>, METH_VARARGS, nullptr},
               {"lerp", method_lerp<kind::vector3>, METH_VARARGS, nullptr},
               {"transform", method_vector3_transform, METH_O, nullptr},
               {"transform_normal", method_vector3_transform_normal, METH_O, nullptr},
               {"transform4", method_transform4<kind::vector3>, METH_O, nullptr},
               {}};

        PyMethodDef vector4_methods[]
            = {{"length", method_length<kind::vector4>, METH_NOARGS, nullptr},
               {"length_squared",
                method_length_squared<kind::vector4>,
                METH_NOARGS,
                nullptr},
               {"distance", method_distance<kind::vector4>, METH_O, nullptr},
               {"distance_squared",
                method_distance_squared<kind::vector4>,
                METH_O,
                nullptr},
               {"dot", method_dot<kind::vector4>, METH_O, nullptr},
               {"normalize", method_normalize<kind::vector4>, METH_NOARGS, nullptr},
               {"min", method_min<kind::vector4>, METH_O, nullptr},
               {"max", method_max<kind::vector4>, METH_O, nullptr},
               {"clamp", method_clamp<kind::vector4>, METH_VARARGS, nullptr},
               {"lerp", method_lerp<kind::vector4>, METH_VARARGS, nullptr},
               {"transform", method_vector4_transform, METH_O, nullptr},
               {}};

        PyMethodDef matrix3x2_methods[] = {
            {"is_identity", method_is_identity<kind::matrix3x2>, METH_NOARGS, nullptr},
            {"determinant", method_determinant<kind::matrix3x2>, METH_NOARGS, nullptr},
            {"translation", method_translation<kind::matrix3x2>, METH_NOARGS, nullptr},
            {"invert", method_invert<kind::matrix3x2>, METH_NOARGS, nullptr},
            {"lerp", method_matrix_lerp<kind::matrix3x2>, METH_VARARGS, nullptr},
            {}};

        PyMethodDef matrix4x4_methods[] = {
            {"is_identity", method_is_identity<kind::matrix4x4>, METH_NOARGS, nullptr},
            {"determinant", method_determinant<kind::matrix4x4>, METH_NOARGS, nullptr},
            {"translation", method_translation<kind::matrix4x4>, METH_NOARGS, nullptr},
            {"invert", method_invert<kind::matrix4x4>, METH_NOARGS, nullptr},
            {"decompose", method_decompose, METH_NOARGS, nullptr},
            {"transform", method_matrix4x4_transform, METH_O, nullptr},
            {"transpose", method_transpose, METH_NOARGS, nullptr},
            {"lerp", method_matrix_lerp<kind::matrix4x4>, METH_VARARGS, nullptr},
            {}};

        PyMethodDef plane_methods[]
            = {{"normalize", method_normalize<kind::plane>, METH_NOARGS, nullptr},
               {"transform", method_plane_transform, METH_O, nullptr},
               {"dot", method_plane_dot, METH_O, nullptr},
               {"dot_coordinate", method_dot_coordinate, METH_O, nullptr},
               {"dot_normal", method_dot_normal, METH_O, nullptr},
               {}};

        PyMethodDef quaternion_methods[] = {
            {"is_identity", method_is_identity<kind::quaternion>, METH_NOARGS, nullptr},
            {"length", method_length<kind::quaternion>, METH_NOARGS, nullptr},
            {"length_squared",
             method_length_squared<kind::quaternion>,
             METH_NOARGS,
             nullptr},
            {"dot", method_dot<kind::quaternion>, METH_O, nullptr},
            {"normalize", method_normalize<kind::quaternion>, METH_NOARGS, nullptr},
            {"conjugate", method_conjugate, METH_NOARGS, nullptr},
            {"inverse", method_inverse, METH_NOARGS, nullptr},
            {"slerp", method_slerp, METH_VARARGS, nullptr},
            {"lerp", method_lerp<kind::quaternion>, METH_VARARGS, nullptr},
            {"concatenate", method_concatenate, METH_O, nullptr},
            {}};

        /**
         * The methods the struct @p which has beyond the ones every struct
         * has, or @c nullptr for a struct with none.
         */
        PyMethodDef* methods_of(kind which) noexcept
        {
            switch (which)
            {
            case kind::vector2:
                return vector2_methods;
            case kind::vector3:
                return vector3_methods;
            case kind::vector4:
                return vector4_methods;
            case kind::matrix3x2:
                return matrix3x2_methods;
            case kind::matrix4x4:
                return matrix4x4_methods;
            case kind::plane:
                return plane_methods;
            case kind::quaternion:
                return quaternion_methods;
            case kind::none:
                break;
            }

            return nullptr;
        }
    } // namespace

    /**
     * Which of the structs of Windows.Foundation.Numerics @p winrt_name is, or
     * kind::none for every other type in that namespace and in every other.
     */
    kind classify(std::string_view winrt_name) noexcept
    {
        constexpr std::string_view prefix{"Windows.Foundation.Numerics."};

        if (!winrt_name.starts_with(prefix))
        {
            return kind::none;
        }

        auto const name = winrt_name.substr(prefix.size());

        if (name == traits<kind::vector2>::py_name)
        {
            return kind::vector2;
        }

        if (name == traits<kind::vector3>::py_name)
        {
            return kind::vector3;
        }

        if (name == traits<kind::vector4>::py_name)
        {
            return kind::vector4;
        }

        if (name == traits<kind::matrix3x2>::py_name)
        {
            return kind::matrix3x2;
        }

        if (name == traits<kind::matrix4x4>::py_name)
        {
            return kind::matrix4x4;
        }

        if (name == traits<kind::plane>::py_name)
        {
            return kind::plane;
        }

        if (name == traits<kind::quaternion>::py_name)
        {
            return kind::quaternion;
        }

        return kind::none;
    }

    /**
     * Whether the layout the table reader worked out for one of the structs is
     * the one C++/WinRT gives it.
     *
     * A value is read out of a wrapper and written back as the C++ type, which
     * only says the same thing as long as the two agree. Both are a run of
     * floats, so they always have, and checking it at import is what keeps a
     * change in either of them from being read as silence.
     */
    bool check_layout(kind which, uint32_t size, uint32_t align) noexcept
    {
        switch (which)
        {
        case kind::vector2:
            return size == sizeof(num::float2) && align == alignof(num::float2);
        case kind::vector3:
            return size == sizeof(num::float3) && align == alignof(num::float3);
        case kind::vector4:
            return size == sizeof(num::float4) && align == alignof(num::float4);
        case kind::matrix3x2:
            return size == sizeof(num::float3x2) && align == alignof(num::float3x2);
        case kind::matrix4x4:
            return size == sizeof(num::float4x4) && align == alignof(num::float4x4);
        case kind::plane:
            return size == sizeof(num::plane) && align == alignof(num::plane);
        case kind::quaternion:
            return size == sizeof(num::quaternion) && align == alignof(num::quaternion);
        case kind::none:
            break;
        }

        return true;
    }

    /**
     * Adds the number slots of the struct @p which, which are the arithmetic
     * C++/WinRT declares operators for.
     *
     * The Plane gets none: it compares and does nothing else, so + on one
     * raises the message CPython writes for a type with no such slot, which is
     * what the projection has always done.
     */
    void add_slots(kind which, std::vector<PyType_Slot>& slots)
    {
        switch (which)
        {
        case kind::vector2:
            push_vector_slots<kind::vector2>(slots);
            break;
        case kind::vector3:
            push_vector_slots<kind::vector3>(slots);
            break;
        case kind::vector4:
            push_vector_slots<kind::vector4>(slots);
            break;
        case kind::matrix3x2:
            push_matrix_slots<kind::matrix3x2>(slots);
            break;
        case kind::matrix4x4:
            push_matrix_slots<kind::matrix4x4>(slots);
            break;
        case kind::quaternion:
            push_quaternion_slots(slots);
            break;
        case kind::plane:
        case kind::none:
            break;
        }
    }

    /**
     * Adds the methods the struct @p which has beyond the ones every struct
     * has.
     */
    void add_methods(kind which, std::vector<PyMethodDef>& methods)
    {
        auto* const extra = methods_of(which);
        if (!extra)
        {
            return;
        }

        for (auto* def = extra; def->ml_name; def++)
        {
            methods.push_back(*def);
        }
    }
} // namespace py::interp::numerics
