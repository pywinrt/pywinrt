import array
import math
import os
import struct
import typing
import unittest

import winrt.windows.foundation.numerics as wfn
from winrt.system import Array

ON_MINGW = "MINGW_PREFIX" in os.environ


class TestNumerics(unittest.TestCase):
    def test_struct_ctor_pos(self) -> None:
        r = wfn.Rational(2, 4)

        self.assertEqual(r.numerator, 2)
        self.assertEqual(r.denominator, 4)

    def test_struct_ctor_kwd(self) -> None:
        r = wfn.Rational(denominator=2, numerator=4)

        self.assertEqual(r.numerator, 4)
        self.assertEqual(r.denominator, 2)

    def test_struct_ctor_mix(self) -> None:
        r = wfn.Rational(3, denominator=6)

        self.assertEqual(r.numerator, 3)
        self.assertEqual(r.denominator, 6)

    def test_vec3(self) -> None:
        v = wfn.Vector3(1.0, 2.0, 3.0)

        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)

    def test_hash(self) -> None:
        self.assertEqual(hash(wfn.Vector2(1, 2)), hash(wfn.Vector2(1, 2)))
        self.assertEqual(len({wfn.Vector2(1, 2), wfn.Vector2(1, 2)}), 1)

    def test_plane(self) -> None:
        v = wfn.Vector3(1.0, 2.0, 3.0)
        p = wfn.Plane(v, 4.0)
        n = p.normal

        self.assertEqual(n.x, 1.0)
        self.assertEqual(n.y, 2.0)
        self.assertEqual(n.z, 3.0)
        self.assertEqual(p.d, 4.0)

    @unittest.skipIf(ON_MINGW, "Not implemented on MinGW")
    def test_make_matrix3x2(self) -> None:
        self.assertEqual(
            wfn.Matrix3x2.make_translation(wfn.Vector2(1, 2)),
            wfn.Matrix3x2(1, 0, 0, 1, 1, 2),
        )
        self.assertEqual(
            wfn.Matrix3x2.make_translation(1, 2), wfn.Matrix3x2(1, 0, 0, 1, 1, 2)
        )
        self.assertEqual(
            wfn.Matrix3x2.make_scale(1, 2), wfn.Matrix3x2(1, 0, 0, 2, 0, 0)
        )
        self.assertEqual(
            wfn.Matrix3x2.make_scale(1, 2, wfn.Vector2(3, 4)),
            wfn.Matrix3x2(1, 0, 0, 2, 0, -4),
        )
        self.assertEqual(
            wfn.Matrix3x2.make_scale_from_vector(wfn.Vector2(1, 2)),
            wfn.Matrix3x2(1, 0, 0, 2, 0, 0),
        )
        self.assertEqual(
            wfn.Matrix3x2.make_scale_from_vector(wfn.Vector2(1, 2), wfn.Vector2(3, 4)),
            wfn.Matrix3x2(1, 0, 0, 2, 0, -4),
        )
        self.assertEqual(
            wfn.Matrix3x2.make_scale_from_scalar(3), wfn.Matrix3x2(3, 0, 0, 3, 0, 0)
        )
        self.assertEqual(
            wfn.Matrix3x2.make_scale_from_scalar(3, wfn.Vector2(4, 5)),
            wfn.Matrix3x2(3, 0, 0, 3, -8, -10),
        )

        m = wfn.Matrix3x2.make_skew(1, 2)
        self.assertEqual(m.m11, 1)
        self.assertAlmostEqual(m.m12, -2.185040, places=5)
        self.assertAlmostEqual(m.m21, 1.557408, places=5)
        self.assertEqual(m.m22, 1)
        self.assertEqual(m.m31, 0)
        self.assertEqual(m.m32, 0)

        m = wfn.Matrix3x2.make_skew(1, 2, wfn.Vector2(3, 4))
        self.assertEqual(m.m11, 1)
        self.assertAlmostEqual(m.m12, -2.185040, places=5)
        self.assertAlmostEqual(m.m21, 1.557408, places=5)
        self.assertEqual(m.m22, 1)
        self.assertAlmostEqual(m.m31, -6.229631, places=5)
        self.assertAlmostEqual(m.m32, 6.555120, places=5)

        m = wfn.Matrix3x2.make_rotation(math.pi / 2)
        self.assertEqual(m.m11, 0)
        self.assertEqual(m.m12, 1)
        self.assertEqual(m.m21, -1)
        self.assertEqual(m.m22, 0)
        self.assertEqual(m.m31, 0)
        self.assertEqual(m.m32, 0)

        m = wfn.Matrix3x2.make_rotation(math.pi / 2, wfn.Vector2(1, 2))
        self.assertEqual(m.m11, 0)
        self.assertEqual(m.m12, 1)
        self.assertEqual(m.m21, -1)
        self.assertEqual(m.m22, 0)
        self.assertEqual(m.m31, 3)
        self.assertEqual(m.m32, 1)

    @unittest.skipIf(ON_MINGW, "Not implemented on MinGW")
    def test_make_matrix4x4(self) -> None:
        m = wfn.Matrix4x4.make_billboard(
            wfn.Vector3(1, 2, 3),
            wfn.Vector3(4, 5, 6),
            wfn.Vector3(7, 8, 9),
            wfn.Vector3(10, 11, 12),
        )
        self.assertAlmostEqual(m.m11, 0.408248, places=5)
        self.assertAlmostEqual(m.m12, -0.816497, places=5)
        self.assertAlmostEqual(m.m13, 0.408248, places=5)
        self.assertEqual(m.m14, 0)
        self.assertAlmostEqual(m.m21, -0.707107, places=5)
        self.assertEqual(m.m22, 0)
        self.assertAlmostEqual(m.m23, 0.707107, places=5)
        self.assertEqual(m.m24, 0)
        self.assertAlmostEqual(m.m31, -0.577350, places=5)
        self.assertAlmostEqual(m.m32, -0.577350, places=5)
        self.assertAlmostEqual(m.m33, -0.577350, places=5)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 1)
        self.assertEqual(m.m42, 2)
        self.assertEqual(m.m43, 3)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_constrained_billboard(
            wfn.Vector3(1, 2, 3),
            wfn.Vector3(4, 5, 6),
            wfn.Vector3(7, 8, 9),
            wfn.Vector3(10, 11, 12),
            wfn.Vector3(13, 14, 15),
        )
        self.assertEqual(m.m11, 0)
        self.assertAlmostEqual(m.m12, 0.747409, places=5)
        self.assertAlmostEqual(m.m13, -0.664364, places=5)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, 7)
        self.assertEqual(m.m22, 8)
        self.assertEqual(m.m23, 9)
        self.assertEqual(m.m24, 0)
        self.assertAlmostEqual(m.m31, 0.864536, places=5)
        self.assertAlmostEqual(m.m32, -0.333890, places=5)
        self.assertAlmostEqual(m.m33, -0.375626, places=5)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 1)
        self.assertEqual(m.m42, 2)
        self.assertEqual(m.m43, 3)
        self.assertEqual(m.m44, 1)

        self.assertEqual(
            wfn.Matrix4x4.make_translation(wfn.Vector3(1, 2, 3)),
            wfn.Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 2, 3, 1),
        )
        self.assertEqual(
            wfn.Matrix4x4.make_translation(1, 2, 3),
            wfn.Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 2, 3, 1),
        )
        self.assertEqual(
            wfn.Matrix4x4.make_scale(1, 2, 3),
            wfn.Matrix4x4(1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1),
        )
        self.assertEqual(
            wfn.Matrix4x4.make_scale(1, 2, 3, wfn.Vector3(4, 5, 6)),
            wfn.Matrix4x4(1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, -5, -12, 1),
        )
        self.assertEqual(
            wfn.Matrix4x4.make_scale_from_vector(wfn.Vector3(1, 2, 3)),
            wfn.Matrix4x4(1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1),
        )
        self.assertEqual(
            wfn.Matrix4x4.make_scale_from_vector(
                wfn.Vector3(1, 2, 3), wfn.Vector3(4, 5, 6)
            ),
            wfn.Matrix4x4(1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, -5, -12, 1),
        )
        self.assertEqual(
            wfn.Matrix4x4.make_scale_from_scalar(3),
            wfn.Matrix4x4(3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1),
        )
        self.assertEqual(
            wfn.Matrix4x4.make_scale_from_scalar(3, wfn.Vector3(4, 5, 6)),
            wfn.Matrix4x4(3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, -8, -10, -12, 1),
        )

        m = wfn.Matrix4x4.make_rotation_x(math.pi / 2)
        self.assertEqual(m.m11, 1)
        self.assertEqual(m.m12, 0)
        self.assertEqual(m.m13, 0)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, 0)
        self.assertAlmostEqual(m.m22, 0)
        self.assertEqual(m.m23, 1)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 0)
        self.assertEqual(m.m32, -1)
        self.assertAlmostEqual(m.m33, 0)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 0)
        self.assertEqual(m.m43, 0)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_rotation_x(math.pi / 2, wfn.Vector3(1, 2, 3))
        self.assertEqual(m.m11, 1)
        self.assertEqual(m.m12, 0)
        self.assertEqual(m.m13, 0)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, 0)
        self.assertAlmostEqual(m.m22, 0)
        self.assertEqual(m.m23, 1)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 0)
        self.assertEqual(m.m32, -1)
        self.assertAlmostEqual(m.m33, 0)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 5)
        self.assertEqual(m.m43, 1)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_rotation_y(math.pi / 2)
        self.assertAlmostEqual(m.m11, 0)
        self.assertEqual(m.m12, 0)
        self.assertEqual(m.m13, -1)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, 0)
        self.assertEqual(m.m22, 1)
        self.assertEqual(m.m23, 0)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 1)
        self.assertEqual(m.m32, 0)
        self.assertAlmostEqual(m.m33, 0)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 0)
        self.assertEqual(m.m43, 0)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_rotation_y(math.pi / 2, wfn.Vector3(1, 2, 3))
        self.assertAlmostEqual(m.m11, 0)
        self.assertEqual(m.m12, 0)
        self.assertEqual(m.m13, -1)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, 0)
        self.assertEqual(m.m22, 1)
        self.assertEqual(m.m23, 0)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 1)
        self.assertEqual(m.m32, 0)
        self.assertAlmostEqual(m.m33, 0)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, -2)
        self.assertEqual(m.m42, 0)
        self.assertEqual(m.m43, 4)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_rotation_z(math.pi / 2)
        self.assertAlmostEqual(m.m11, 0)
        self.assertEqual(m.m12, 1)
        self.assertEqual(m.m13, 0)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, -1)
        self.assertAlmostEqual(m.m22, 0)
        self.assertEqual(m.m23, 0)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 0)
        self.assertEqual(m.m32, 0)
        self.assertEqual(m.m33, 1)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 0)
        self.assertEqual(m.m43, 0)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_rotation_z(math.pi / 2, wfn.Vector3(1, 2, 3))
        self.assertAlmostEqual(m.m11, 0)
        self.assertEqual(m.m12, 1)
        self.assertEqual(m.m13, 0)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, -1)
        self.assertAlmostEqual(m.m22, 0)
        self.assertEqual(m.m23, 0)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 0)
        self.assertEqual(m.m32, 0)
        self.assertEqual(m.m33, 1)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 3)
        self.assertEqual(m.m42, 1)
        self.assertEqual(m.m43, 0)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_from_axis_angle(wfn.Vector3(1, 2, 3), math.pi / 2)
        self.assertEqual(m.m11, 1)
        self.assertEqual(m.m12, 5)
        self.assertAlmostEqual(m.m13, 1, places=5)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, -1)
        self.assertEqual(m.m22, 4)
        self.assertAlmostEqual(m.m23, 7, places=5)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 5)
        self.assertAlmostEqual(m.m32, 5, places=5)
        self.assertEqual(m.m33, 9)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 0)
        self.assertEqual(m.m43, 0)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_perspective_field_of_view(math.pi / 2, 1, 0.1, 100)
        self.assertEqual(m.m11, 1)
        self.assertEqual(m.m12, 0)
        self.assertEqual(m.m13, 0)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, 0)
        self.assertEqual(m.m22, 1)
        self.assertEqual(m.m23, 0)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 0)
        self.assertEqual(m.m32, 0)
        self.assertAlmostEqual(m.m33, -1.001001, places=5)
        self.assertEqual(m.m34, -1)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 0)
        self.assertAlmostEqual(m.m43, -0.100100, places=5)
        self.assertEqual(m.m44, 0)

        m = wfn.Matrix4x4.make_perspective(1, 2, 0.1, 100)
        self.assertAlmostEqual(m.m11, 0.2)
        self.assertEqual(m.m12, 0)
        self.assertEqual(m.m13, 0)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, 0)
        self.assertAlmostEqual(m.m22, 0.1)
        self.assertEqual(m.m23, 0)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 0)
        self.assertEqual(m.m32, 0)
        self.assertAlmostEqual(m.m33, -1.001001, places=5)
        self.assertEqual(m.m34, -1)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 0)
        self.assertAlmostEqual(m.m43, -0.100100, places=5)
        self.assertEqual(m.m44, 0)

        m = wfn.Matrix4x4.make_perspective_off_center(1, 2, 3, 4, 0.1, 100)
        self.assertAlmostEqual(m.m11, 0.2)
        self.assertEqual(m.m12, 0)
        self.assertEqual(m.m13, 0)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, 0)
        self.assertAlmostEqual(m.m22, 0.2)
        self.assertEqual(m.m23, 0)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 3)
        self.assertEqual(m.m32, 7)
        self.assertAlmostEqual(m.m33, -1.001001, places=5)
        self.assertEqual(m.m34, -1)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 0)
        self.assertAlmostEqual(m.m43, -0.100100, places=5)
        self.assertEqual(m.m44, 0)

        m = wfn.Matrix4x4.make_orthographic(1, 2, 3, 4)
        self.assertEqual(m.m11, 2)
        self.assertEqual(m.m12, 0)
        self.assertEqual(m.m13, 0)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, 0)
        self.assertEqual(m.m22, 1)
        self.assertEqual(m.m23, 0)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 0)
        self.assertEqual(m.m32, 0)
        self.assertEqual(m.m33, -1)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 0)
        self.assertEqual(m.m43, -3)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_orthographic_off_center(1, 2, 3, 4, 5, 6)
        self.assertEqual(m.m11, 2)
        self.assertEqual(m.m12, 0)
        self.assertEqual(m.m13, 0)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, 0)
        self.assertEqual(m.m22, 2)
        self.assertEqual(m.m23, 0)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 0)
        self.assertEqual(m.m32, 0)
        self.assertEqual(m.m33, -1)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, -3)
        self.assertEqual(m.m42, -7)
        self.assertEqual(m.m43, -5)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_look_at(
            wfn.Vector3(1, 2, 3), wfn.Vector3(4, 5, 6), wfn.Vector3(7, 8, 9)
        )
        self.assertAlmostEqual(m.m11, 0.408248, places=5)
        self.assertAlmostEqual(m.m12, -0.707107, places=5)
        self.assertAlmostEqual(m.m13, -0.577350, places=5)
        self.assertEqual(m.m14, 0)
        self.assertAlmostEqual(m.m21, -0.816497, places=5)
        self.assertEqual(m.m22, 0)
        self.assertAlmostEqual(m.m23, -0.577350, places=5)
        self.assertEqual(m.m24, 0)
        self.assertAlmostEqual(m.m31, 0.408248, places=5)
        self.assertAlmostEqual(m.m32, 0.707107, places=5)
        self.assertAlmostEqual(m.m33, -0.577350, places=5)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 0)
        self.assertAlmostEqual(m.m42, -1.414214, places=5)
        self.assertAlmostEqual(m.m43, 3.4641016, places=5)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_world(
            wfn.Vector3(1, 2, 3), wfn.Vector3(4, 5, 6), wfn.Vector3(7, 8, 9)
        )
        self.assertAlmostEqual(m.m11, -0.408248, places=5)
        self.assertAlmostEqual(m.m12, 0.816497, places=5)
        self.assertAlmostEqual(m.m13, -0.408248, places=5)
        self.assertEqual(m.m14, 0)
        self.assertAlmostEqual(m.m21, 0.790911, places=5)
        self.assertAlmostEqual(m.m22, 0.093048, places=5)
        self.assertAlmostEqual(m.m23, -0.604815, places=5)
        self.assertEqual(m.m24, 0)
        self.assertAlmostEqual(m.m31, -0.455842, places=5)
        self.assertAlmostEqual(m.m32, -0.569802, places=5)
        self.assertAlmostEqual(m.m33, -0.683763, places=5)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 1)
        self.assertEqual(m.m42, 2)
        self.assertEqual(m.m43, 3)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_from_quaternion(wfn.Quaternion(1, 2, 3, 4))
        self.assertEqual(m.m11, -25)
        self.assertEqual(m.m12, 28)
        self.assertEqual(m.m13, -10)
        self.assertEqual(m.m14, 0)
        self.assertEqual(m.m21, -20)
        self.assertEqual(m.m22, -19)
        self.assertEqual(m.m23, 20)
        self.assertEqual(m.m24, 0)
        self.assertEqual(m.m31, 22)
        self.assertEqual(m.m32, 4)
        self.assertEqual(m.m33, -9)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 0)
        self.assertEqual(m.m43, 0)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_from_yaw_pitch_roll(1, 2, 3)
        self.assertAlmostEqual(m.m11, -0.426918, places=5)
        self.assertAlmostEqual(m.m12, -0.058727, places=5)
        self.assertAlmostEqual(m.m13, 0.902382, places=5)
        self.assertEqual(m.m14, 0)
        self.assertAlmostEqual(m.m21, -0.833737, places=5)
        self.assertAlmostEqual(m.m22, 0.411982, places=5)
        self.assertAlmostEqual(m.m23, -0.367630, places=5)
        self.assertEqual(m.m24, 0)
        self.assertAlmostEqual(m.m31, -0.350175, places=5)
        self.assertAlmostEqual(m.m32, -0.909297, places=5)
        self.assertAlmostEqual(m.m33, -0.224845, places=5)
        self.assertEqual(m.m34, 0)
        self.assertEqual(m.m41, 0)
        self.assertEqual(m.m42, 0)
        self.assertEqual(m.m43, 0)
        self.assertEqual(m.m44, 1)

        m = wfn.Matrix4x4.make_shadow(
            wfn.Vector3(1, 2, 3), wfn.Plane(wfn.Vector3(4, 5, 6), 7)
        )
        self.assertAlmostEqual(m.m11, 3.190896, places=5)
        self.assertAlmostEqual(m.m12, -0.911684, places=5)
        self.assertAlmostEqual(m.m13, -1.367527, places=5)
        self.assertEqual(m.m14, 0)
        self.assertAlmostEqual(m.m21, -0.569802, places=5)
        self.assertAlmostEqual(m.m22, 2.507133, places=5)
        self.assertAlmostEqual(m.m23, -1.7094087, places=5)
        self.assertEqual(m.m24, 0)
        self.assertAlmostEqual(m.m31, -0.683763, places=5)
        self.assertAlmostEqual(m.m32, -1.367527, places=5)
        self.assertAlmostEqual(m.m33, 1.595448, places=5)
        self.assertEqual(m.m34, 0)
        self.assertAlmostEqual(m.m41, -0.797724, places=5)
        self.assertAlmostEqual(m.m42, -1.595448, places=5)
        self.assertAlmostEqual(m.m43, -2.393172, places=5)
        self.assertAlmostEqual(m.m44, 3.646738, places=5)

        m = wfn.Matrix4x4.make_reflection(wfn.Plane(wfn.Vector3(1, 2, 3), 4))
        self.assertAlmostEqual(m.m11, 0.857142, places=5)
        self.assertAlmostEqual(m.m12, -0.285714, places=5)
        self.assertAlmostEqual(m.m13, -0.4285714, places=5)
        self.assertEqual(m.m14, 0)
        self.assertAlmostEqual(m.m21, -0.285714, places=5)
        self.assertAlmostEqual(m.m22, 0.428571, places=5)
        self.assertAlmostEqual(m.m23, -0.857142, places=5)
        self.assertEqual(m.m24, 0)
        self.assertAlmostEqual(m.m31, -0.428571, places=5)
        self.assertAlmostEqual(m.m32, -0.857142, places=5)
        self.assertAlmostEqual(m.m33, -0.285714, places=5)
        self.assertEqual(m.m34, 0)
        self.assertAlmostEqual(m.m41, -0.571428, places=5)
        self.assertAlmostEqual(m.m42, -1.142857, places=5)
        self.assertAlmostEqual(m.m43, -1.714285, places=5)
        self.assertEqual(m.m44, 1)

    @unittest.skipIf(ON_MINGW, "Not implemented on MinGW")
    def test_make_plane(self) -> None:
        p = wfn.Plane.make_from_vertices(
            wfn.Vector3(1, 0, 0), wfn.Vector3(0, 1, 0), wfn.Vector3(0, 0, 1)
        )
        self.assertAlmostEqual(p.normal.x, 0.577350, places=5)
        self.assertAlmostEqual(p.normal.y, 0.577350, places=5)
        self.assertAlmostEqual(p.normal.z, 0.577350, places=5)
        self.assertAlmostEqual(p.d, -0.577350, places=5)

    def test_make_quaternion(self) -> None:
        q = wfn.Quaternion.make_from_axis_angle(wfn.Vector3(1, 2, 3), math.pi / 2)
        self.assertAlmostEqual(q.x, 0.707107, places=5)
        self.assertAlmostEqual(q.y, 1.414214, places=5)
        self.assertAlmostEqual(q.z, 2.121320, places=5)
        self.assertAlmostEqual(q.w, 0.707107, places=5)

        q = wfn.Quaternion.make_from_yaw_pitch_roll(1, 2, 3)
        self.assertAlmostEqual(q.x, 0.310622, places=5)
        self.assertAlmostEqual(q.y, -0.718287, places=5)
        self.assertAlmostEqual(q.z, 0.444435, places=5)
        self.assertAlmostEqual(q.w, 0.435952, places=5)

        q = wfn.Quaternion.make_from_rotation_matrix(
            wfn.Matrix4x4(1, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1)
        )
        self.assertAlmostEqual(q.x, 0.707107, places=5)
        self.assertEqual(q.y, 0)
        self.assertEqual(q.z, 0)
        self.assertAlmostEqual(q.w, 0.707107, places=5)

    def test_zero_one(self) -> None:
        self.assertEqual(wfn.Vector2.zero, wfn.Vector2(0, 0))
        self.assertEqual(wfn.Vector2.one, wfn.Vector2(1, 1))

        self.assertEqual(wfn.Vector3.zero, wfn.Vector3(0, 0, 0))
        self.assertEqual(wfn.Vector3.one, wfn.Vector3(1, 1, 1))

        self.assertEqual(wfn.Vector4.zero, wfn.Vector4(0, 0, 0, 0))
        self.assertEqual(wfn.Vector4.one, wfn.Vector4(1, 1, 1, 1))

    def test_unit(self) -> None:
        self.assertEqual(wfn.Vector2.unit_x, wfn.Vector2(1, 0))
        self.assertEqual(wfn.Vector2.unit_y, wfn.Vector2(0, 1))

        self.assertEqual(wfn.Vector3.unit_x, wfn.Vector3(1, 0, 0))
        self.assertEqual(wfn.Vector3.unit_y, wfn.Vector3(0, 1, 0))
        self.assertEqual(wfn.Vector3.unit_z, wfn.Vector3(0, 0, 1))

        self.assertEqual(wfn.Vector4.unit_x, wfn.Vector4(1, 0, 0, 0))
        self.assertEqual(wfn.Vector4.unit_y, wfn.Vector4(0, 1, 0, 0))
        self.assertEqual(wfn.Vector4.unit_z, wfn.Vector4(0, 0, 1, 0))
        self.assertEqual(wfn.Vector4.unit_w, wfn.Vector4(0, 0, 0, 1))

    def test_identity(self) -> None:
        self.assertEqual(wfn.Matrix3x2.identity, wfn.Matrix3x2(1, 0, 0, 1, 0, 0))
        self.assertEqual(
            wfn.Matrix4x4.identity,
            wfn.Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1),
        )
        self.assertEqual(wfn.Quaternion.identity, wfn.Quaternion(0, 0, 0, 1))

    def test_add(self) -> None:
        self.assertEqual(wfn.Vector2(1, 2) + wfn.Vector2(3, 4), wfn.Vector2(4, 6))
        self.assertEqual(
            wfn.Vector3(1, 2, 3) + wfn.Vector3(4, 5, 6), wfn.Vector3(5, 7, 9)
        )
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4) + wfn.Vector4(5, 6, 7, 8), wfn.Vector4(6, 8, 10, 12)
        )
        self.assertEqual(
            wfn.Matrix3x2(1, 2, 3, 4, 5, 6) + wfn.Matrix3x2(7, 8, 9, 10, 11, 12),
            wfn.Matrix3x2(8, 10, 12, 14, 16, 18),
        )
        self.assertEqual(
            wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
            + wfn.Matrix4x4(
                17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
            ),
            wfn.Matrix4x4(
                18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48
            ),
        )
        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4) + wfn.Quaternion(5, 6, 7, 8),
            wfn.Quaternion(6, 8, 10, 12),
        )

    def test_add_bad_type(self) -> None:
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Plane' and 'int'",
        ):
            wfn.Plane(wfn.Vector3(), 0) + 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Vector2' and 'int'",
        ):
            wfn.Vector2() + 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Vector3' and 'int'",
        ):
            wfn.Vector3() + 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Vector4' and 'int'",
        ):
            wfn.Vector4() + 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Matrix3x2' and 'int'",
        ):
            wfn.Matrix3x2() + 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Matrix4x4' and 'int'",
        ):
            wfn.Matrix4x4() + 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Quaternion' and 'int'",
        ):
            wfn.Quaternion() + 1  # type: ignore

    def test_sub(self) -> None:
        self.assertEqual(wfn.Vector2(1, 2) - wfn.Vector2(3, 4), wfn.Vector2(-2, -2))
        self.assertEqual(
            wfn.Vector3(1, 2, 3) - wfn.Vector3(4, 5, 6), wfn.Vector3(-3, -3, -3)
        )
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4) - wfn.Vector4(5, 6, 7, 8),
            wfn.Vector4(-4, -4, -4, -4),
        )
        self.assertEqual(
            wfn.Matrix3x2(1, 2, 3, 4, 5, 6) - wfn.Matrix3x2(7, 8, 9, 10, 11, 12),
            wfn.Matrix3x2(-6, -6, -6, -6, -6, -6),
        )
        self.assertEqual(
            wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
            - wfn.Matrix4x4(
                17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
            ),
            wfn.Matrix4x4(*[-16] * 16),
        )
        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4) - wfn.Quaternion(5, 6, 7, 8),
            wfn.Quaternion(-4, -4, -4, -4),
        )

    def test_sub_bad_type(self) -> None:
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Plane' and 'int'",
        ):
            wfn.Plane(wfn.Vector3(), 0) - 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Vector2' and 'int'",
        ):
            wfn.Vector2() - 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Vector3' and 'int'",
        ):
            wfn.Vector3() - 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Vector4' and 'int'",
        ):
            wfn.Vector4() - 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Matrix3x2' and 'int'",
        ):
            wfn.Matrix3x2() - 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Matrix4x4' and 'int'",
        ):
            wfn.Matrix4x4() - 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Quaternion' and 'int'",
        ):
            wfn.Quaternion() - 1  # type: ignore

    def test_mul(self) -> None:
        self.assertEqual(wfn.Vector2(1, 2) * wfn.Vector2(3, 4), wfn.Vector2(3, 8))
        self.assertEqual(wfn.Vector2(1, 2) * 3, wfn.Vector2(3, 6))
        self.assertEqual(3 * wfn.Vector2(1, 2), wfn.Vector2(3, 6))

        self.assertEqual(
            wfn.Vector3(1, 2, 3) * wfn.Vector3(4, 5, 6), wfn.Vector3(4, 10, 18)
        )
        self.assertEqual(wfn.Vector3(1, 2, 3) * 4, wfn.Vector3(4, 8, 12))
        self.assertEqual(4 * wfn.Vector3(1, 2, 3), wfn.Vector3(4, 8, 12))

        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4) * wfn.Vector4(5, 6, 7, 8),
            wfn.Vector4(5, 12, 21, 32),
        )
        self.assertEqual(wfn.Vector4(1, 2, 3, 4) * 5, wfn.Vector4(5, 10, 15, 20))
        self.assertEqual(5 * wfn.Vector4(1, 2, 3, 4), wfn.Vector4(5, 10, 15, 20))

        self.assertEqual(
            wfn.Matrix3x2(1, 2, 3, 4, 5, 6) * 2,
            wfn.Matrix3x2(2, 4, 6, 8, 10, 12),
        )
        self.assertEqual(
            2 * wfn.Matrix3x2(1, 2, 3, 4, 5, 6),
            wfn.Matrix3x2(2, 4, 6, 8, 10, 12),
        )

        self.assertEqual(
            wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16) * 2,
            wfn.Matrix4x4(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32),
        )
        self.assertEqual(
            2 * wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16),
            wfn.Matrix4x4(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32),
        )

        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4) * wfn.Quaternion(5, 6, 7, 8),
            wfn.Quaternion(24, 48, 48, -6),
        )
        self.assertEqual(wfn.Quaternion(1, 2, 3, 4) * 2, wfn.Quaternion(2, 4, 6, 8))
        self.assertEqual(2 * wfn.Quaternion(1, 2, 3, 4), wfn.Quaternion(2, 4, 6, 8))

    def test_mul_bad_type(self) -> None:
        o = object()
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Plane' and 'object'",
        ):
            wfn.Plane(wfn.Vector3(), 0) * o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Vector2' and 'object'",
        ):
            wfn.Vector2() * o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Vector3' and 'object'",
        ):
            wfn.Vector3() * o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Vector4' and 'object'",
        ):
            wfn.Vector4() * o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Matrix3x2' and 'object'",
        ):
            wfn.Matrix3x2() * o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Matrix4x4' and 'object'",
        ):
            wfn.Matrix4x4() * o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Quaternion' and 'object'",
        ):
            wfn.Quaternion() * o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"\* between two 'Matrix3x2' values is not their product: use @",
        ):
            wfn.Matrix3x2() * wfn.Matrix3x2()  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"\* between two 'Matrix4x4' values is not their product: use @",
        ):
            wfn.Matrix4x4() * wfn.Matrix4x4()  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"\* between two 'Matrix4x4' values is not their product: use @",
        ):
            wfn.Matrix4x4() * tuple(range(16))  # type: ignore

    def test_truediv(self) -> None:
        self.assertEqual(
            wfn.Vector2(1, 2) / wfn.Vector2(3, 4), wfn.Vector2(1 / 3, 2 / 4)
        )
        self.assertEqual(wfn.Vector2(1, 2) / 3, wfn.Vector2(1 / 3, 2 / 3))

        self.assertEqual(
            wfn.Vector3(1, 2, 3) / wfn.Vector3(4, 5, 6),
            wfn.Vector3(1 / 4, 2 / 5, 3 / 6),
        )
        self.assertEqual(wfn.Vector3(1, 2, 3) / 4, wfn.Vector3(1 / 4, 2 / 4, 3 / 4))

        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4) / wfn.Vector4(5, 6, 7, 8),
            wfn.Vector4(1 / 5, 2 / 6, 3 / 7, 4 / 8),
        )
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4) / 5, wfn.Vector4(1 / 5, 2 / 5, 3 / 5, 4 / 5)
        )

        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4) / wfn.Quaternion(5, 6, 7, 8),
            wfn.Quaternion(1, 2, 3, 4) * wfn.Quaternion(5, 6, 7, 8).inverse(),
        )

    def test_truediv_bad_type(self) -> None:
        o = object()
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Plane' and 'object'",
        ):
            wfn.Plane(wfn.Vector3(), 0) / o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Vector2' and 'object'",
        ):
            wfn.Vector2() / o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Vector3' and 'object'",
        ):
            wfn.Vector3() / o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Vector4' and 'object'",
        ):
            wfn.Vector4() / o  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Matrix3x2' and 'int'",
        ):
            wfn.Matrix3x2() / 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Matrix4x4' and 'int'",
        ):
            wfn.Matrix4x4() / 1  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Quaternion' and 'int'",
        ):
            wfn.Quaternion() / 1  # type: ignore

    def test_neg(self) -> None:
        self.assertEqual(-wfn.Vector2(1, 2), wfn.Vector2(-1, -2))
        self.assertEqual(-wfn.Vector3(1, 2, 3), wfn.Vector3(-1, -2, -3))
        self.assertEqual(-wfn.Vector4(1, 2, 3, 4), wfn.Vector4(-1, -2, -3, -4))
        self.assertEqual(
            -wfn.Matrix3x2(1, 2, 3, 4, 5, 6), wfn.Matrix3x2(-1, -2, -3, -4, -5, -6)
        )
        self.assertEqual(
            -wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16),
            wfn.Matrix4x4(
                -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16
            ),
        )
        self.assertEqual(-wfn.Quaternion(1, 2, 3, 4), wfn.Quaternion(-1, -2, -3, -4))

    def test_iadd(self) -> None:
        v2 = wfn.Vector2(1, 2)
        v2 += wfn.Vector2(3, 4)
        self.assertEqual(v2, wfn.Vector2(4, 6))

        v3 = wfn.Vector3(1, 2, 3)
        v3 += wfn.Vector3(4, 5, 6)
        self.assertEqual(v3, wfn.Vector3(5, 7, 9))

        v4 = wfn.Vector4(1, 2, 3, 4)
        v4 += wfn.Vector4(5, 6, 7, 8)
        self.assertEqual(v4, wfn.Vector4(6, 8, 10, 12))

        m32 = wfn.Matrix3x2(1, 2, 3, 4, 5, 6)
        m32 += wfn.Matrix3x2(7, 8, 9, 10, 11, 12)
        self.assertEqual(m32, wfn.Matrix3x2(8, 10, 12, 14, 16, 18))

        m44 = wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        m44 += wfn.Matrix4x4(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
        )
        self.assertEqual(
            m44,
            wfn.Matrix4x4(
                18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48
            ),
        )

        q = wfn.Quaternion(1, 2, 3, 4)
        q += wfn.Quaternion(5, 6, 7, 8)
        self.assertEqual(q, wfn.Quaternion(6, 8, 10, 12))

    def test_isub(self) -> None:
        v2 = wfn.Vector2(1, 2)
        v2 -= wfn.Vector2(3, 4)
        self.assertEqual(v2, wfn.Vector2(-2, -2))

        v3 = wfn.Vector3(1, 2, 3)
        v3 -= wfn.Vector3(4, 5, 6)
        self.assertEqual(v3, wfn.Vector3(-3, -3, -3))

        v4 = wfn.Vector4(1, 2, 3, 4)
        v4 -= wfn.Vector4(5, 6, 7, 8)
        self.assertEqual(v4, wfn.Vector4(-4, -4, -4, -4))

        m32 = wfn.Matrix3x2(1, 2, 3, 4, 5, 6)
        m32 -= wfn.Matrix3x2(7, 8, 9, 10, 11, 12)
        self.assertEqual(m32, wfn.Matrix3x2(-6, -6, -6, -6, -6, -6))

        m44 = wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        m44 -= wfn.Matrix4x4(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
        )
        self.assertEqual(
            m44,
            wfn.Matrix4x4(*[-16] * 16),
        )

        q = wfn.Quaternion(1, 2, 3, 4)
        q -= wfn.Quaternion(5, 6, 7, 8)
        self.assertEqual(q, wfn.Quaternion(-4, -4, -4, -4))

    def test_imul(self) -> None:
        v2 = wfn.Vector2(1, 2)
        v2 *= wfn.Vector2(3, 4)
        self.assertEqual(v2, wfn.Vector2(3, 8))

        v2 = wfn.Vector2(1, 2)
        v2 *= 3
        self.assertEqual(v2, wfn.Vector2(3, 6))

        v3 = wfn.Vector3(1, 2, 3)
        v3 *= wfn.Vector3(4, 5, 6)
        self.assertEqual(v3, wfn.Vector3(4, 10, 18))

        v3 = wfn.Vector3(1, 2, 3)
        v3 *= 4
        self.assertEqual(v3, wfn.Vector3(4, 8, 12))

        v4 = wfn.Vector4(1, 2, 3, 4)
        v4 *= wfn.Vector4(5, 6, 7, 8)
        self.assertEqual(v4, wfn.Vector4(5, 12, 21, 32))

        v4 = wfn.Vector4(1, 2, 3, 4)
        v4 *= 5
        self.assertEqual(v4, wfn.Vector4(5, 10, 15, 20))

        m32 = wfn.Matrix3x2(1, 2, 3, 4, 5, 6)
        m32 *= 2
        self.assertEqual(m32, wfn.Matrix3x2(2, 4, 6, 8, 10, 12))

        m44 = wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        m44 *= 2
        self.assertEqual(
            m44,
            wfn.Matrix4x4(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32),
        )

        q = wfn.Quaternion(1, 2, 3, 4)
        q *= wfn.Quaternion(5, 6, 7, 8)
        self.assertEqual(q, wfn.Quaternion(24, 48, 48, -6))

        q = wfn.Quaternion(1, 2, 3, 4)
        q *= 2
        self.assertEqual(q, wfn.Quaternion(2, 4, 6, 8))

    def test_matmul(self) -> None:
        self.assertEqual(
            wfn.Matrix3x2(1, 2, 3, 4, 5, 6) @ wfn.Matrix3x2(7, 8, 9, 10, 11, 12),
            wfn.Matrix3x2(25, 28, 57, 64, 100, 112),
        )

        self.assertEqual(
            wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
            @ wfn.Matrix4x4(
                17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
            ),
            wfn.Matrix4x4(
                250,
                260,
                270,
                280,
                618,
                644,
                670,
                696,
                986,
                1028,
                1070,
                1112,
                1354,
                1412,
                1470,
                1528,
            ),
        )

        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4) @ wfn.Quaternion(5, 6, 7, 8),
            wfn.Quaternion(24, 48, 48, -6),
        )

    @unittest.skipIf(ON_MINGW, "Not implemented on MinGW")
    def test_matmul_transform(self) -> None:
        self.assertEqual(
            wfn.Vector2(1, 2) @ wfn.Matrix3x2.make_translation(10, 20),
            wfn.Vector2(11, 22),
        )

        translation = wfn.Matrix4x4.make_translation(10, 20, 30)
        self.assertEqual(
            wfn.Vector3(1, 2, 3) @ translation,
            wfn.Vector3(1, 2, 3).transform(translation),
        )
        self.assertEqual(wfn.Vector3(1, 2, 3) @ translation, wfn.Vector3(11, 22, 33))
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 1) @ translation, wfn.Vector4(11, 22, 33, 1)
        )

        # a Vector2 through a 3-D transform is (x, y, 0, 1)
        self.assertEqual(wfn.Vector2(1, 2) @ translation, wfn.Vector2(11, 22))

        rotation = wfn.Quaternion.make_from_axis_angle(wfn.Vector3(0, 0, 1), math.pi)
        self.assertEqual(
            wfn.Vector3(1, 2, 3) @ rotation, wfn.Vector3(1, 2, 3).transform(rotation)
        )

    def test_matmul_rotation(self) -> None:
        rotation = wfn.Quaternion.make_from_axis_angle(wfn.Vector3(0, 0, 1), math.pi)

        self.assertEqual(
            wfn.Vector2(1, 2) @ rotation, wfn.Vector2(1, 2).transform(rotation)
        )
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4) @ rotation,
            wfn.Vector4(1, 2, 3, 4).transform(rotation),
        )

        v = wfn.Vector2(1, 2)
        v @= wfn.Quaternion.identity
        self.assertEqual(v, wfn.Vector2(1, 2))

    def test_imatmul(self) -> None:
        m = wfn.Matrix3x2(1, 2, 3, 4, 5, 6)
        m @= wfn.Matrix3x2(7, 8, 9, 10, 11, 12)
        self.assertEqual(m, wfn.Matrix3x2(25, 28, 57, 64, 100, 112))

        q = wfn.Quaternion(1, 2, 3, 4)
        q @= wfn.Quaternion(5, 6, 7, 8)
        self.assertEqual(q, wfn.Quaternion(24, 48, 48, -6))

    def test_matmul_bad_type(self) -> None:
        # A vector on the right would be a column vector, which System.Numerics
        # does not multiply by.
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for @: '[\w+\.]*Matrix4x4' and "
            r"'[\w+\.]*Vector3'",
        ):
            wfn.Matrix4x4.identity @ wfn.Vector3(1, 2, 3)  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for @: '[\w+\.]*Vector3' and "
            r"'[\w+\.]*Vector3'",
        ):
            wfn.Vector3() @ wfn.Vector3()  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for @: '[\w+\.]*Vector4' and "
            r"'[\w+\.]*Matrix3x2'",
        ):
            wfn.Vector4() @ wfn.Matrix3x2.identity  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for @: '[\w+\.]*Vector3' and 'tuple'",
        ):
            wfn.Vector3() @ tuple(range(16))  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for @: '[\w+\.]*Matrix4x4' and 'int'",
        ):
            wfn.Matrix4x4() @ 2  # type: ignore

    def test_buffer(self) -> None:
        v = wfn.Vector3(1, 2, 3)
        view = memoryview(v)
        self.assertEqual(view.format, "f")
        self.assertEqual(view.shape, (3,))
        self.assertEqual(view.strides, (4,))
        self.assertTrue(view.readonly)
        self.assertTrue(view.c_contiguous)
        self.assertEqual(view.tolist(), [1, 2, 3])
        self.assertEqual(bytes(v), struct.pack("3f", 1, 2, 3))
        self.assertEqual(struct.unpack("3f", v), (1, 2, 3))

        self.assertEqual(memoryview(wfn.Vector2(1, 2)).shape, (2,))
        self.assertEqual(memoryview(wfn.Vector4(1, 2, 3, 4)).shape, (4,))
        self.assertEqual(memoryview(wfn.Quaternion(1, 2, 3, 4)).tolist(), [1, 2, 3, 4])

        # a plane is its normal followed by its distance
        plane = wfn.Plane(wfn.Vector3(1, 2, 3), 4)
        self.assertEqual(memoryview(plane).tolist(), [1, 2, 3, 4])

        # a matrix is its rows one after another
        m = wfn.Matrix3x2(1, 2, 3, 4, 5, 6)
        view = memoryview(m)
        self.assertEqual(view.shape, (3, 2))
        self.assertEqual(view.strides, (8, 4))
        self.assertEqual(view.tolist(), [[1, 2], [3, 4], [5, 6]])

        view = memoryview(wfn.Matrix4x4(*range(16)))
        self.assertEqual(view.shape, (4, 4))
        self.assertEqual(view.strides, (16, 4))
        self.assertEqual(view.tolist()[3], [12, 13, 14, 15])

    def test_buffer_read_only(self) -> None:
        view = memoryview(wfn.Vector3())

        with self.assertRaisesRegex(TypeError, "read-only"):
            view[0] = 1

        with self.assertRaises(TypeError):
            struct.pack_into("f", wfn.Vector3(), 0, 1.0)

    def test_sequence(self) -> None:
        v = wfn.Vector3(1, 2, 3)
        self.assertEqual(len(v), 3)
        self.assertEqual(v[0], 1)
        self.assertEqual(v[-1], 3)
        self.assertEqual(list(v), [1, 2, 3])
        self.assertEqual(max(v), 3)
        self.assertEqual(sum(v), 6)
        self.assertIn(2, v)

        x, y, z = v
        self.assertEqual((x, y, z), (1, 2, 3))

        self.assertEqual(list(wfn.Vector2(1, 2)), [1, 2])
        self.assertEqual(list(wfn.Vector4(1, 2, 3, 4)), [1, 2, 3, 4])
        self.assertEqual(list(wfn.Quaternion(1, 2, 3, 4)), [1, 2, 3, 4])

        with self.assertRaisesRegex(IndexError, "'Vector3' index out of range"):
            v[3]

        with self.assertRaisesRegex(IndexError, "'Vector3' index out of range"):
            v[-4]

        with self.assertRaises(TypeError):
            v[0:2]  # type: ignore

    def test_matrix_subscript(self) -> None:
        m = wfn.Matrix4x4(*range(1, 17))
        self.assertEqual(m[0, 0], m.m11)
        self.assertEqual(m[0, 1], m.m12)
        self.assertEqual(m[3, 0], m.m41)
        self.assertEqual(m[-1, -1], m.m44)

        m32 = wfn.Matrix3x2(1, 2, 3, 4, 5, 6)
        self.assertEqual(m32[2, 1], m32.m32)
        self.assertEqual(m32[1, 0], m32.m21)

        with self.assertRaisesRegex(TypeError, r"as m\[row, column\], not by 'int'"):
            m[0]  # type: ignore

        with self.assertRaisesRegex(TypeError, r"as m\[row, column\], not by 'slice'"):
            m[0:2]  # type: ignore

        with self.assertRaisesRegex(TypeError, r"as m\[row, column\], not by 'tuple'"):
            m[0, 1, 2]  # type: ignore

        with self.assertRaisesRegex(IndexError, "row index out of range"):
            m[4, 0]

        with self.assertRaisesRegex(IndexError, "column index out of range"):
            m32[0, 2]

        with self.assertRaisesRegex(TypeError, "column indices must be integers"):
            m[0, 1.0]  # type: ignore

        # a matrix is not a sequence: its buffer and unpack() are the flat forms
        with self.assertRaises(TypeError):
            len(m)  # type: ignore

        with self.assertRaises(TypeError):
            iter(m)  # type: ignore

    def test_buffer_as_value(self) -> None:
        # A buffer stands for a struct the way a tuple does, which the stubs
        # leave out, so the buffers are typed as Any.
        floats: typing.Any = array.array("f", [1, 1, 1])
        doubles: typing.Any = array.array("d", [1, 1, 1])

        v = wfn.Vector3(1, 2, 3)
        self.assertEqual(v + floats, wfn.Vector3(2, 3, 4))
        self.assertEqual(v + doubles, wfn.Vector3(2, 3, 4))

        # a matrix from a block in its shape, and not from flat floats, which
        # could be its rows or its columns
        rows: typing.Any = (
            memoryview(array.array("f", range(16))).cast("B").cast("f", (4, 4))
        )
        flat: typing.Any = array.array("f", range(16))
        self.assertEqual(wfn.Matrix4x4() + rows, wfn.Matrix4x4(*range(16)))

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Matrix4x4' and "
            r"'array.array'",
        ):
            wfn.Matrix4x4() + flat

        with self.assertRaisesRegex(
            TypeError,
            r"must hold floats or doubles of shape \(4, 4\), not 'f' of shape "
            r"\(16,\)",
        ):
            wfn.Quaternion.make_from_rotation_matrix(flat)

        identity: typing.Any = memoryview(wfn.Matrix4x4.identity)
        self.assertEqual(
            wfn.Quaternion.make_from_rotation_matrix(identity), wfn.Quaternion.identity
        )

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Vector3' and "
            r"'array.array'",
        ):
            v + array.array("f", [1, 1])  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"a buffer read as '[\w+\.]*Vector3' must hold floats or doubles of "
            r"shape \(3,\), not 'i' of shape \(3,\)",
        ):
            v.dot(array.array("i", [1, 1, 1]))  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"must hold floats or doubles of shape \(4, 4\), not 'f' of shape "
            r"\(4,\)",
        ):
            wfn.Quaternion.make_from_rotation_matrix(array.array("f", range(4)))  # type: ignore

        # a value of another of the structs is not one of these, though its
        # floats fit
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Vector4' and "
            r"'[\w+\.]*Quaternion'",
        ):
            wfn.Vector4() + wfn.Quaternion()  # type: ignore

        # nor is an Array of them one value, though its floats are in the shape
        # of one; memoryview() of it is plain floats, which say what they are
        identity_rows = Array(
            wfn.Vector4,
            [
                wfn.Vector4.unit_x,
                wfn.Vector4.unit_y,
                wfn.Vector4.unit_z,
                wfn.Vector4.unit_w,
            ],
        )

        with self.assertRaisesRegex(
            TypeError,
            r"an Array of Windows.Foundation.Numerics.Vector4 is not a "
            r"'[\w+\.]*Matrix4x4': memoryview\(\) of it reads its floats as one",
        ):
            wfn.Quaternion.make_from_rotation_matrix(identity_rows)  # type: ignore

        self.assertEqual(
            wfn.Quaternion.make_from_rotation_matrix(memoryview(identity_rows)),  # type: ignore
            wfn.Quaternion.identity,
        )

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Matrix3x2' and "
            r"'[\w+\.]*Array'",
        ):
            wfn.Matrix3x2() + Array(wfn.Vector2, 3)  # type: ignore

        self.assertEqual(
            wfn.Matrix3x2() + memoryview(Array(wfn.Vector2, 3)),  # type: ignore
            wfn.Matrix3x2(),
        )

    def test_array_buffer(self) -> None:
        a = Array(wfn.Vector3, [wfn.Vector3(1, 2, 3), wfn.Vector3(4, 5, 6)])
        view = memoryview(a)
        self.assertEqual(view.format, "f")
        self.assertEqual(view.itemsize, 4)
        self.assertEqual(view.shape, (2, 3))
        self.assertEqual(view.strides, (12, 4))
        self.assertEqual(view.tolist(), [[1, 2, 3], [4, 5, 6]])

        # the array is writable through its buffer
        view[1, 2] = 7
        self.assertEqual(a[1], wfn.Vector3(4, 5, 7))
        view.release()

        m = Array(wfn.Matrix4x4, [wfn.Matrix4x4(*range(16))])
        view = memoryview(m)
        self.assertEqual(view.shape, (1, 4, 4))
        self.assertEqual(view.strides, (64, 16, 4))
        self.assertEqual(
            view.tolist(), [[list(range(row, row + 4)) for row in range(0, 16, 4)]]
        )
        view.release()

        self.assertEqual(memoryview(Array(wfn.Matrix3x2, 2)).shape, (2, 3, 2))
        self.assertEqual(memoryview(Array(wfn.Plane, 2)).shape, (2, 4))

    def test_array_from_buffer(self) -> None:
        flat = array.array("f", range(6))
        a = Array(wfn.Vector3, memoryview(flat).cast("B").cast("f", (2, 3)))
        self.assertEqual(list(a), [wfn.Vector3(0, 1, 2), wfn.Vector3(3, 4, 5)])

        self.assertEqual(Array(wfn.Vector3, memoryview(a)), a)
        # an Array of the same struct is copied
        self.assertEqual(Array(wfn.Vector3, a), a)

        m = Array(
            wfn.Matrix4x4,
            memoryview(array.array("f", range(32))).cast("B").cast("f", (2, 4, 4)),
        )
        self.assertEqual(len(m), 2)
        self.assertEqual(m[1], wfn.Matrix4x4(*range(16, 32)))
        self.assertEqual(Array(wfn.Matrix4x4, memoryview(m)), m)

        # flat floats are not an array of them, which has a shape
        with self.assertRaisesRegex(
            TypeError,
            r"a buffer for an Array of Windows.Foundation.Numerics.Vector3 must "
            r"hold float32 values of shape \(n, 3\), not 'f' of shape \(6,\)",
        ):
            Array(wfn.Vector3, flat)

        with self.assertRaisesRegex(
            TypeError,
            r"of shape \(n, 4, 4\), not 'f' of shape \(32,\)",
        ):
            Array(wfn.Matrix4x4, array.array("f", range(32)))

        with self.assertRaisesRegex(
            TypeError, r"of shape \(n, 3\), not 'f' of shape \(4,\)"
        ):
            Array(wfn.Vector3, array.array("f", range(4)))

        with self.assertRaisesRegex(
            TypeError, r"float32 values of shape \(n, 3\), not 'd' of shape \(6,\)"
        ):
            Array(wfn.Vector3, array.array("d", range(6)))

        # one value is not an array of them
        with self.assertRaisesRegex(
            TypeError,
            r"a '[\w+\.]*Vector3' is not an Array of "
            r"Windows.Foundation.Numerics.Vector3: pass \[value\] for an Array of "
            r"one",
        ):
            Array(wfn.Vector3, wfn.Vector3(1, 2, 3))

        self.assertEqual(
            list(Array(wfn.Vector3, [wfn.Vector3(1, 2, 3)])), [wfn.Vector3(1, 2, 3)]
        )

        # nor is a value of another of the structs, or an Array of them,
        # though the floats fit; memoryview() of the Array reads them as these
        with self.assertRaisesRegex(
            TypeError,
            r"a '[\w+\.]*Quaternion' is not an Array of "
            r"Windows.Foundation.Numerics.Vector4$",
        ):
            Array(wfn.Vector4, wfn.Quaternion(1, 2, 3, 4))

        q = Array(wfn.Quaternion, [wfn.Quaternion(1, 2, 3, 4), wfn.Quaternion.identity])

        with self.assertRaisesRegex(
            TypeError,
            r"an Array of Windows.Foundation.Numerics.Quaternion is not an Array of "
            r"Windows.Foundation.Numerics.Vector4: memoryview\(\) of it reads its "
            r"floats as one",
        ):
            Array(wfn.Vector4, q)

        self.assertEqual(
            list(Array(wfn.Vector4, memoryview(q))),
            [wfn.Vector4(1, 2, 3, 4), wfn.Vector4(0, 0, 0, 1)],
        )

    def test_itruediv(self) -> None:
        v2 = wfn.Vector2(1, 2)
        v2 /= wfn.Vector2(3, 4)
        self.assertEqual(v2, wfn.Vector2(1 / 3, 2 / 4))

        v2 = wfn.Vector2(1, 2)
        v2 /= 3
        self.assertEqual(v2, wfn.Vector2(1 / 3, 2 / 3))

        v3 = wfn.Vector3(1, 2, 3)
        v3 /= wfn.Vector3(4, 5, 6)
        self.assertEqual(v3, wfn.Vector3(1 / 4, 2 / 5, 3 / 6))

        v3 = wfn.Vector3(1, 2, 3)
        v3 /= 4
        self.assertEqual(v3, wfn.Vector3(1 / 4, 2 / 4, 3 / 4))

        v4 = wfn.Vector4(1, 2, 3, 4)
        v4 /= wfn.Vector4(5, 6, 7, 8)
        self.assertEqual(v4, wfn.Vector4(1 / 5, 2 / 6, 3 / 7, 4 / 8))

        v4 = wfn.Vector4(1, 2, 3, 4)
        v4 /= 5
        self.assertEqual(v4, wfn.Vector4(1 / 5, 2 / 5, 3 / 5, 4 / 5))

        q = wfn.Quaternion(1, 2, 3, 4)
        q /= wfn.Quaternion(5, 6, 7, 8)
        self.assertEqual(
            q, wfn.Quaternion(1, 2, 3, 4) * wfn.Quaternion(5, 6, 7, 8).inverse()
        )

    def test_is_identity(self) -> None:
        self.assertTrue(wfn.Matrix3x2(1, 0, 0, 1, 0, 0).is_identity())
        self.assertTrue(
            wfn.Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1).is_identity()
        )
        self.assertTrue(wfn.Quaternion(0, 0, 0, 1).is_identity())

    def test_length(self) -> None:
        self.assertEqual(wfn.Vector2(3, 4).length(), 5)
        self.assertEqual(abs(wfn.Vector2(3, 4)), 5)
        self.assertAlmostEqual(wfn.Vector3(1, 2, 3).length(), 14**0.5, places=5)
        self.assertAlmostEqual(abs(wfn.Vector3(1, 2, 3)), 14**0.5, places=5)
        self.assertAlmostEqual(wfn.Vector4(1, 2, 3, 4).length(), 30**0.5, places=5)
        self.assertAlmostEqual(abs(wfn.Vector4(1, 2, 3, 4)), 30**0.5, places=5)
        self.assertAlmostEqual(wfn.Quaternion(1, 2, 3, 4).length(), 30**0.5, places=5)
        self.assertAlmostEqual(abs(wfn.Quaternion(1, 2, 3, 4)), 30**0.5, places=5)

    def test_length_squared(self) -> None:
        self.assertEqual(wfn.Vector2(3, 4).length_squared(), 25)
        self.assertEqual(wfn.Vector3(1, 2, 3).length_squared(), 14)
        self.assertEqual(wfn.Vector4(1, 2, 3, 4).length_squared(), 30)
        self.assertEqual(wfn.Quaternion(1, 2, 3, 4).length_squared(), 30)

    def test_distance(self) -> None:
        self.assertEqual(wfn.Vector2(1, 2).distance(wfn.Vector2(4, 6)), 5)
        self.assertAlmostEqual(
            wfn.Vector3(1, 2, 3).distance(wfn.Vector3(4, 6, 9)), 61**0.5, places=5
        )
        self.assertAlmostEqual(
            wfn.Vector4(1, 2, 3, 4).distance(wfn.Vector4(4, 6, 9, 12)),
            125**0.5,
            places=5,
        )

    def test_distance_squared(self) -> None:
        self.assertEqual(wfn.Vector2(1, 2).distance_squared(wfn.Vector2(4, 6)), 25)
        self.assertEqual(
            wfn.Vector3(1, 2, 3).distance_squared(wfn.Vector3(4, 6, 9)), 61
        )
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4).distance_squared(wfn.Vector4(4, 6, 9, 12)), 125
        )

    def test_dot(self) -> None:
        self.assertEqual(wfn.Vector2(1, 2).dot(wfn.Vector2(3, 4)), 11)
        self.assertEqual(wfn.Vector3(1, 2, 3).dot(wfn.Vector3(4, 5, 6)), 32)
        self.assertEqual(wfn.Vector4(1, 2, 3, 4).dot(wfn.Vector4(5, 6, 7, 8)), 70)

        if not ON_MINGW:
            self.assertEqual(
                wfn.Plane(wfn.Vector3(1, 2, 3), 4).dot(wfn.Vector4(5, 6, 7, 8)), 70
            )
            self.assertEqual(
                wfn.Plane(wfn.Vector3(1, 2, 3), 4).dot_coordinate(wfn.Vector3(5, 6, 7)),
                42,
            )
            self.assertEqual(
                wfn.Plane(wfn.Vector3(1, 2, 3), 4).dot_normal(wfn.Vector3(5, 6, 7)), 38
            )

        self.assertEqual(wfn.Quaternion(1, 2, 3, 4).dot(wfn.Quaternion(5, 6, 7, 8)), 70)

    def test_cross(self) -> None:
        self.assertEqual(
            wfn.Vector3(1, 2, 3).cross(wfn.Vector3(4, 5, 6)), wfn.Vector3(-3, 6, -3)
        )

    def test_normalize(self) -> None:
        self.assertEqual(wfn.Vector2(3, 4).normalize(), wfn.Vector2(3 / 5, 4 / 5))

        v3 = wfn.Vector3(1, 2, 3).normalize()
        self.assertAlmostEqual(v3.x, 1 / 14**0.5, places=5)
        self.assertAlmostEqual(v3.y, 2 / 14**0.5, places=5)
        self.assertAlmostEqual(v3.z, 3 / 14**0.5, places=5)

        v4 = wfn.Vector4(1, 2, 3, 4).normalize()
        self.assertAlmostEqual(v4.x, 1 / 30**0.5, places=5)
        self.assertAlmostEqual(v4.y, 2 / 30**0.5, places=5)
        self.assertAlmostEqual(v4.z, 3 / 30**0.5, places=5)
        self.assertAlmostEqual(v4.w, 4 / 30**0.5, places=5)

        p = wfn.Plane(wfn.Vector3(1, 2, 3), 4).normalize()
        self.assertAlmostEqual(p.normal.x, 1 / 14**0.5, places=5)
        self.assertAlmostEqual(p.normal.y, 2 / 14**0.5, places=5)
        self.assertAlmostEqual(p.normal.z, 3 / 14**0.5, places=5)
        self.assertAlmostEqual(p.d, 4 / 14**0.5, places=5)

        q = wfn.Quaternion(1, 2, 3, 4).normalize()
        self.assertAlmostEqual(q.x, 1 / 30**0.5, places=5)
        self.assertAlmostEqual(q.y, 2 / 30**0.5, places=5)
        self.assertAlmostEqual(q.z, 3 / 30**0.5, places=5)
        self.assertAlmostEqual(q.w, 4 / 30**0.5, places=5)

    def test_reflect(self) -> None:
        self.assertEqual(
            wfn.Vector2(1, 2).reflect(wfn.Vector2(3, 4)), wfn.Vector2(-65, -86)
        )
        self.assertEqual(
            wfn.Vector3(1, 2, 3).reflect(wfn.Vector3(4, 5, 6)),
            wfn.Vector3(-255, -318, -381),
        )

    def test_min(self) -> None:
        self.assertEqual(wfn.Vector2(1, 2).min(wfn.Vector2(3, 4)), wfn.Vector2(1, 2))
        self.assertEqual(wfn.Vector2(3, 4).min(wfn.Vector2(1, 2)), wfn.Vector2(1, 2))
        self.assertEqual(
            wfn.Vector3(1, 2, 3).min(wfn.Vector3(4, 5, 6)), wfn.Vector3(1, 2, 3)
        )
        self.assertEqual(
            wfn.Vector3(4, 5, 6).min(wfn.Vector3(1, 2, 3)), wfn.Vector3(1, 2, 3)
        )
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4).min(wfn.Vector4(5, 6, 7, 8)),
            wfn.Vector4(1, 2, 3, 4),
        )
        self.assertEqual(
            wfn.Vector4(5, 6, 7, 8).min(wfn.Vector4(1, 2, 3, 4)),
            wfn.Vector4(1, 2, 3, 4),
        )

    def test_max(self) -> None:
        self.assertEqual(wfn.Vector2(1, 2).max(wfn.Vector2(3, 4)), wfn.Vector2(3, 4))
        self.assertEqual(wfn.Vector2(3, 4).max(wfn.Vector2(1, 2)), wfn.Vector2(3, 4))
        self.assertEqual(
            wfn.Vector3(1, 2, 3).max(wfn.Vector3(4, 5, 6)), wfn.Vector3(4, 5, 6)
        )
        self.assertEqual(
            wfn.Vector3(4, 5, 6).max(wfn.Vector3(1, 2, 3)), wfn.Vector3(4, 5, 6)
        )
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4).max(wfn.Vector4(5, 6, 7, 8)),
            wfn.Vector4(5, 6, 7, 8),
        )
        self.assertEqual(
            wfn.Vector4(5, 6, 7, 8).max(wfn.Vector4(1, 2, 3, 4)),
            wfn.Vector4(5, 6, 7, 8),
        )

    def test_clamp(self) -> None:
        self.assertEqual(
            wfn.Vector2(1, 2).clamp(wfn.Vector2(3, 4), wfn.Vector2(5, 6)),
            wfn.Vector2(3, 4),
        )
        self.assertEqual(
            wfn.Vector2(3, 4).clamp(wfn.Vector2(1, 2), wfn.Vector2(5, 6)),
            wfn.Vector2(3, 4),
        )
        self.assertEqual(
            wfn.Vector2(5, 6).clamp(wfn.Vector2(1, 2), wfn.Vector2(3, 4)),
            wfn.Vector2(3, 4),
        )

        self.assertEqual(
            wfn.Vector3(1, 2, 3).clamp(wfn.Vector3(4, 5, 6), wfn.Vector3(7, 8, 9)),
            wfn.Vector3(4, 5, 6),
        )
        self.assertEqual(
            wfn.Vector3(4, 5, 6).clamp(wfn.Vector3(1, 2, 3), wfn.Vector3(7, 8, 9)),
            wfn.Vector3(4, 5, 6),
        )
        self.assertEqual(
            wfn.Vector3(7, 8, 9).clamp(wfn.Vector3(1, 2, 3), wfn.Vector3(4, 5, 6)),
            wfn.Vector3(4, 5, 6),
        )

        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4).clamp(
                wfn.Vector4(5, 6, 7, 8), wfn.Vector4(9, 10, 11, 12)
            ),
            wfn.Vector4(5, 6, 7, 8),
        )
        self.assertEqual(
            wfn.Vector4(5, 6, 7, 8).clamp(
                wfn.Vector4(1, 2, 3, 4), wfn.Vector4(9, 10, 11, 12)
            ),
            wfn.Vector4(5, 6, 7, 8),
        )
        self.assertEqual(
            wfn.Vector4(9, 10, 11, 12).clamp(
                wfn.Vector4(1, 2, 3, 4), wfn.Vector4(5, 6, 7, 8)
            ),
            wfn.Vector4(5, 6, 7, 8),
        )

    def test_lerp(self) -> None:
        self.assertEqual(
            wfn.Vector2(1, 2).lerp(wfn.Vector2(3, 4), 0.5), wfn.Vector2(2, 3)
        )
        self.assertEqual(
            wfn.Vector3(1, 2, 3).lerp(wfn.Vector3(4, 5, 6), 0.5),
            wfn.Vector3(2.5, 3.5, 4.5),
        )
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4).lerp(wfn.Vector4(5, 6, 7, 8), 0.5),
            wfn.Vector4(3, 4, 5, 6),
        )

        if not ON_MINGW:
            self.assertEqual(
                wfn.Matrix3x2(1, 2, 3, 4, 5, 6).lerp(
                    wfn.Matrix3x2(7, 8, 9, 10, 11, 12), 0.5
                ),
                wfn.Matrix3x2(4, 5, 6, 7, 8, 9),
            )
            self.assertEqual(
                wfn.Matrix4x4(
                    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
                ).lerp(
                    wfn.Matrix4x4(
                        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
                    ),
                    0.5,
                ),
                wfn.Matrix4x4(
                    9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24
                ),
            )

        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4).slerp(wfn.Quaternion(5, 6, 7, 8), 0.5),
            wfn.Quaternion(3, 4, 5, 6),
        )

        q = wfn.Quaternion(1, 2, 3, 4).lerp(wfn.Quaternion(5, 6, 7, 8), 0.5)
        self.assertAlmostEqual(q.x, 0.323498, places=5)
        self.assertAlmostEqual(q.y, 0.431331, places=5)
        self.assertAlmostEqual(q.z, 0.539164, places=5)
        self.assertAlmostEqual(q.w, 0.646997, places=5)

    @unittest.skipIf(ON_MINGW, "Not implemented")
    def test_transform(self) -> None:
        self.assertEqual(
            wfn.Vector2(1, 2).transform(wfn.Matrix3x2(3, 4, 5, 6, 7, 8)),
            wfn.Vector2(20, 24),
        )
        self.assertEqual(
            wfn.Vector2(1, 2).transform(
                wfn.Matrix4x4(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18)
            ),
            wfn.Vector2(32, 36),
        )
        self.assertEqual(
            wfn.Vector2(1, 2).transform_normal(wfn.Matrix3x2(3, 4, 5, 6, 7, 8)),
            wfn.Vector2(13, 16),
        )
        self.assertEqual(
            wfn.Vector2(1, 2).transform_normal(
                wfn.Matrix4x4(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18),
            ),
            wfn.Vector2(17, 20),
        )
        self.assertEqual(
            wfn.Vector2(1, 2).transform(wfn.Quaternion(3, 4, 5, 6)),
            wfn.Vector2(-153, -50),
        )
        self.assertEqual(
            wfn.Vector2(1, 2).transform4(
                wfn.Matrix4x4(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18)
            ),
            wfn.Vector4(32, 36, 40, 44),
        )
        self.assertEqual(
            wfn.Vector2(1, 2).transform4(wfn.Quaternion(3, 4, 5, 6)),
            wfn.Vector4(-153, -50, 134, 1),
        )

        self.assertEqual(
            wfn.Vector3(1, 2, 3).transform(
                wfn.Matrix4x4(4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19)
            ),
            wfn.Vector3(72, 79, 86),
        )
        self.assertEqual(
            wfn.Vector3(1, 2, 3).transform_normal(
                wfn.Matrix4x4(4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19)
            ),
            wfn.Vector3(56, 62, 68),
        )
        self.assertEqual(
            wfn.Vector3(1, 2, 3).transform(wfn.Quaternion(4, 5, 6, 7)),
            wfn.Vector3(145, -70, -33),
        )
        self.assertEqual(
            wfn.Vector3(1, 2, 3).transform4(
                wfn.Matrix4x4(4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19)
            ),
            wfn.Vector4(72, 79, 86, 93),
        )
        self.assertEqual(
            wfn.Vector3(1, 2, 3).transform4(wfn.Quaternion(4, 5, 6, 7)),
            wfn.Vector4(145, -70, -33, 1),
        )

        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4).transform(
                wfn.Matrix4x4(5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20)
            ),
            wfn.Vector4(130, 140, 150, 160),
        )
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4).transform(wfn.Quaternion(5, 6, 7, 8)),
            wfn.Vector4(225, -110, -61, 4),
        )

        self.assertEqual(
            wfn.Matrix4x4(
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
            ).transform(wfn.Quaternion(17, 18, 19, 20)),
            wfn.Matrix4x4(
                2433,
                -1214,
                -1021,
                4,
                1829,
                -906,
                -761,
                8,
                1225,
                -598,
                -501,
                12,
                621,
                -290,
                -241,
                16,
            ),
        )

        self.assertEqual(
            wfn.Plane(wfn.Vector3(1, 0, 0), 1).transform(
                wfn.Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
            ),
            wfn.Plane(wfn.Vector3(1, 0, 0), 1),
        )
        self.assertEqual(
            wfn.Plane(wfn.Vector3(1, 2, 3), 4).transform(wfn.Quaternion(5, 6, 7, 8)),
            wfn.Plane(wfn.Vector3(225, -110, -61), 4),
        )

    def test_deteminant(self) -> None:
        self.assertEqual(wfn.Matrix3x2(1, 2, 3, 4, 5, 6).determinant(), -2)
        self.assertEqual(
            wfn.Matrix4x4(
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
            ).determinant(),
            0,
        )

    def test_translation(self) -> None:
        self.assertEqual(
            wfn.Matrix3x2(1, 2, 3, 4, 5, 6).translation(), wfn.Vector2(5, 6)
        )
        self.assertEqual(
            wfn.Matrix4x4(
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
            ).translation(),
            wfn.Vector3(13, 14, 15),
        )

    @unittest.skipIf(ON_MINGW, "Not implemented")
    def test_invert(self) -> None:
        self.assertEqual(
            wfn.Matrix3x2(1, 2, 3, 4, 5, 6).invert(),
            wfn.Matrix3x2(-2, 1, 1.5, -0.5, 1, -2),
        )

        with self.assertRaises(ValueError):
            wfn.Matrix3x2(0, 0, 0, 0, 0, 0).invert()

        self.assertEqual(
            wfn.Matrix4x4(1, 2, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1).invert(),
            wfn.Matrix4x4(1, -2, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1),
        )

        with self.assertRaises(ValueError):
            wfn.Matrix4x4(
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
            ).invert()

    @unittest.skipIf(ON_MINGW, "Not implemented")
    def test_decompose(self) -> None:
        scale, rotation, translation = wfn.Matrix4x4(
            1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 4, 5, 6, 1
        ).decompose()
        self.assertEqual(scale, wfn.Vector3(1, 2, 3))
        self.assertEqual(rotation, wfn.Quaternion(0, 0, 0, 1))
        self.assertEqual(translation, wfn.Vector3(4, 5, 6))

        with self.assertRaises(ValueError):
            wfn.Matrix4x4(
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
            ).decompose()

    def test_transpose(self) -> None:
        self.assertEqual(
            wfn.Matrix4x4(
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
            ).transpose(),
            wfn.Matrix4x4(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16),
        )

    def test_conjugate(self) -> None:
        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4).conjugate(), wfn.Quaternion(-1, -2, -3, 4)
        )

    def test_inverse(self) -> None:
        q = wfn.Quaternion(1, 2, 3, 4).inverse()
        self.assertAlmostEqual(q.x, -0.033333, places=5)
        self.assertAlmostEqual(q.y, -0.066667, places=5)
        self.assertAlmostEqual(q.z, -0.1, places=5)
        self.assertAlmostEqual(q.w, 0.133333, places=5)

    def test_concatenate(self) -> None:
        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4).concatenate(wfn.Quaternion(5, 6, 7, 8)),
            wfn.Quaternion(32, 32, 56, -6),
        )
