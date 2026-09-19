// Which of the Windows.Foundation.Numerics structs a type is, and what that
// gives it beyond its fields.
//
// Vector2, Vector3, Vector4, Matrix3x2, Matrix4x4, Plane and Quaternion are
// plain structs of floats in metadata: the fields are all that is written
// down, so structs.cpp builds the Python type from a table like any other. The
// arithmetic on them is not written down anywhere a projection can read. It is
// declared by C++/WinRT in <windowsnumerics.impl.h>, and it is keyed here on
// the name the table gives the type rather than on anything in the table.
//
// This is the part of that which belongs to a value, in numerics.cpp. The
// constants and the factory functions belong to the type and are in
// numerics-statics.h.

#pragma once

#include <Python.h>

#include "interp.h"

#include <cstdint>
#include <string_view>
#include <vector>

namespace py::interp::numerics
{
    /**
     * Which of the structs of Windows.Foundation.Numerics a type is, as far as
     * anything beyond its fields is concerned.
     */
    enum class kind : uint8_t
    {
        none,
        vector2,
        vector3,
        vector4,
        matrix3x2,
        matrix4x4,
        plane,
        quaternion,
    };

    kind classify(std::string_view winrt_name) noexcept;

    bool check_layout(kind which, uint32_t size, uint32_t align) noexcept;

    void add_slots(kind which, std::vector<PyType_Slot>& slots);

    void add_methods(kind which, std::vector<PyMethodDef>& methods);
} // namespace py::interp::numerics
