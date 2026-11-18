import math
import os
import unittest

import winrt.windows.foundation.numerics as wfn

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
