// The C++ type, the Python name and the shape of each of the
// Windows.Foundation.Numerics structs, which everything else about them is
// keyed on.
//
// This is apart from numerics-values.h so that numerics-buffer.cpp, which
// needs the shapes and none of the conversions, can have it alone.

#pragma once

#include <Python.h>

#include <pywinrt/base.h>

#include "numerics.h"

#include <array>

namespace py::interp::numerics
{
    namespace
    {
        namespace num = winrt::Windows::Foundation::Numerics;

        /**
         * The C++ value and the Python name of one of the structs, and its
         * shape as a block of floats, which is what its buffer exports: a
         * matrix is its rows one after another and a Plane is its normal
         * followed by its distance.
         */
        template<kind K>
        struct traits;

        template<>
        struct traits<kind::vector2>
        {
            using type = num::float2;
            static constexpr char const* py_name = "Vector2";
            static constexpr std::array<Py_ssize_t, 1> shape{2};
        };

        template<>
        struct traits<kind::vector3>
        {
            using type = num::float3;
            static constexpr char const* py_name = "Vector3";
            static constexpr std::array<Py_ssize_t, 1> shape{3};
        };

        template<>
        struct traits<kind::vector4>
        {
            using type = num::float4;
            static constexpr char const* py_name = "Vector4";
            static constexpr std::array<Py_ssize_t, 1> shape{4};
        };

        template<>
        struct traits<kind::matrix3x2>
        {
            using type = num::float3x2;
            static constexpr char const* py_name = "Matrix3x2";
            static constexpr std::array<Py_ssize_t, 2> shape{3, 2};
        };

        template<>
        struct traits<kind::matrix4x4>
        {
            using type = num::float4x4;
            static constexpr char const* py_name = "Matrix4x4";
            static constexpr std::array<Py_ssize_t, 2> shape{4, 4};
        };

        template<>
        struct traits<kind::plane>
        {
            using type = num::plane;
            static constexpr char const* py_name = "Plane";
            static constexpr std::array<Py_ssize_t, 1> shape{4};
        };

        template<>
        struct traits<kind::quaternion>
        {
            using type = num::quaternion;
            static constexpr char const* py_name = "Quaternion";
            static constexpr std::array<Py_ssize_t, 1> shape{4};
        };
    } // namespace
} // namespace py::interp::numerics
