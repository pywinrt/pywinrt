import math
import os
import unittest

import winrt.windows.foundation.numerics as wfn

ON_MINGW = "MINGW_PREFIX" in os.environ


class TestNumerics(unittest.TestCase):
    def test_struct_ctor_pos(self):
        r = wfn.Rational(2, 4)

        self.assertEqual(r.numerator, 2)
        self.assertEqual(r.denominator, 4)

    def test_struct_ctor_kwd(self):
        r = wfn.Rational(denominator=2, numerator=4)

        self.assertEqual(r.numerator, 4)
        self.assertEqual(r.denominator, 2)

    def test_struct_ctor_mix(self):
        r = wfn.Rational(3, denominator=6)

        self.assertEqual(r.numerator, 3)
        self.assertEqual(r.denominator, 6)

    def test_vec3(self):
        v = wfn.Vector3(1.0, 2.0, 3.0)

        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)

    def test_plane(self):
        v = wfn.Vector3(1.0, 2.0, 3.0)
        p = wfn.Plane(v, 4.0)
        n = p.normal

        self.assertEqual(n.x, 1.0)
        self.assertEqual(n.y, 2.0)
        self.assertEqual(n.z, 3.0)
        self.assertEqual(p.d, 4.0)

    @unittest.skipIf(ON_MINGW, "Not implemented on MinGW")
    def test_make_matrix3x2(self):
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
    def test_make_matrix4x4(self):
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

        m = wfn.Matrix4x4.make_orthographic(1, 2, 3, 4, 0.1, 100)
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
    def test_make_plane(self):
        p = wfn.Plane.make_from_vertices(
            wfn.Vector3(1, 0, 0), wfn.Vector3(0, 1, 0), wfn.Vector3(0, 0, 1)
        )
        self.assertAlmostEqual(p.normal.x, 0.577350, places=5)
        self.assertAlmostEqual(p.normal.y, 0.577350, places=5)
        self.assertAlmostEqual(p.normal.z, 0.577350, places=5)
        self.assertAlmostEqual(p.d, -0.577350, places=5)

    def test_make_quaternion(self):
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

    def test_zero_one(self):
        self.assertEqual(wfn.Vector2.zero, wfn.Vector2(0, 0))
        self.assertEqual(wfn.Vector2.one, wfn.Vector2(1, 1))

        self.assertEqual(wfn.Vector3.zero, wfn.Vector3(0, 0, 0))
        self.assertEqual(wfn.Vector3.one, wfn.Vector3(1, 1, 1))

        self.assertEqual(wfn.Vector4.zero, wfn.Vector4(0, 0, 0, 0))
        self.assertEqual(wfn.Vector4.one, wfn.Vector4(1, 1, 1, 1))

    def test_unit(self):
        self.assertEqual(wfn.Vector2.unit_x, wfn.Vector2(1, 0))
        self.assertEqual(wfn.Vector2.unit_y, wfn.Vector2(0, 1))

        self.assertEqual(wfn.Vector3.unit_x, wfn.Vector3(1, 0, 0))
        self.assertEqual(wfn.Vector3.unit_y, wfn.Vector3(0, 1, 0))
        self.assertEqual(wfn.Vector3.unit_z, wfn.Vector3(0, 0, 1))

        self.assertEqual(wfn.Vector4.unit_x, wfn.Vector4(1, 0, 0, 0))
        self.assertEqual(wfn.Vector4.unit_y, wfn.Vector4(0, 1, 0, 0))
        self.assertEqual(wfn.Vector4.unit_z, wfn.Vector4(0, 0, 1, 0))
        self.assertEqual(wfn.Vector4.unit_w, wfn.Vector4(0, 0, 0, 1))

    def test_identity(self):
        self.assertEqual(wfn.Matrix3x2.identity, wfn.Matrix3x2(1, 0, 0, 1, 0, 0))
        self.assertEqual(
            wfn.Matrix4x4.identity,
            wfn.Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1),
        )
        self.assertEqual(wfn.Quaternion.identity, wfn.Quaternion(0, 0, 0, 1))

    def test_add(self):
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

    def test_add_bad_type(self):
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Plane' and 'int'",
        ):
            wfn.Plane(wfn.Vector3(), 0) + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Vector2' and 'int'",
        ):
            wfn.Vector2() + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Vector3' and 'int'",
        ):
            wfn.Vector3() + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Vector4' and 'int'",
        ):
            wfn.Vector4() + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Matrix3x2' and 'int'",
        ):
            wfn.Matrix3x2() + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Matrix4x4' and 'int'",
        ):
            wfn.Matrix4x4() + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Quaternion' and 'int'",
        ):
            wfn.Quaternion() + 1

    def test_sub(self):
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

    def test_sub_bad_type(self):
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Plane' and 'int'",
        ):
            wfn.Plane(wfn.Vector3(), 0) - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Vector2' and 'int'",
        ):
            wfn.Vector2() - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Vector3' and 'int'",
        ):
            wfn.Vector3() - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Vector4' and 'int'",
        ):
            wfn.Vector4() - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Matrix3x2' and 'int'",
        ):
            wfn.Matrix3x2() - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Matrix4x4' and 'int'",
        ):
            wfn.Matrix4x4() - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Quaternion' and 'int'",
        ):
            wfn.Quaternion() - 1

    def test_mul(self):
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
            wfn.Matrix3x2(1, 2, 3, 4, 5, 6) * wfn.Matrix3x2(7, 8, 9, 10, 11, 12),
            wfn.Matrix3x2(25, 28, 57, 64, 100, 112),
        )
        self.assertEqual(
            wfn.Matrix3x2(1, 2, 3, 4, 5, 6) * 2,
            wfn.Matrix3x2(2, 4, 6, 8, 10, 12),
        )

        self.assertEqual(
            wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
            * wfn.Matrix4x4(
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
            wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16) * 2,
            wfn.Matrix4x4(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32),
        )

        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4) * wfn.Quaternion(5, 6, 7, 8),
            wfn.Quaternion(24, 48, 48, -6),
        )
        self.assertEqual(wfn.Quaternion(1, 2, 3, 4) * 2, wfn.Quaternion(2, 4, 6, 8))

    def test_mul_bad_type(self):
        o = object()
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Plane' and 'object'",
        ):
            wfn.Plane(wfn.Vector3(), 0) * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Vector2' and 'object'",
        ):
            wfn.Vector2() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Vector3' and 'object'",
        ):
            wfn.Vector3() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Vector4' and 'object'",
        ):
            wfn.Vector4() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Matrix3x2' and 'object'",
        ):
            wfn.Matrix3x2() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'int' and '[\w+\.]*Matrix3x2'",
        ):
            1 * wfn.Matrix3x2()

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Matrix4x4' and 'object'",
        ):
            wfn.Matrix4x4() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'int' and '[\w+\.]*Matrix4x4'",
        ):
            1 * wfn.Matrix4x4()

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: '[\w+\.]*Quaternion' and 'object'",
        ):
            wfn.Quaternion() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'int' and '[\w+\.]*Quaternion'",
        ):
            1 * wfn.Quaternion()

    def test_truediv(self):
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

    def test_truediv_bad_type(self):
        o = object()
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Plane' and 'object'",
        ):
            wfn.Plane(wfn.Vector3(), 0) / o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Vector2' and 'object'",
        ):
            wfn.Vector2() / o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Vector3' and 'object'",
        ):
            wfn.Vector3() / o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Vector4' and 'object'",
        ):
            wfn.Vector4() / o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Matrix3x2' and 'int'",
        ):
            wfn.Matrix3x2() / 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Matrix4x4' and 'int'",
        ):
            wfn.Matrix4x4() / 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Quaternion' and 'int'",
        ):
            wfn.Quaternion() / 1

    def test_neg(self):
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

    def test_iadd(self):
        v = wfn.Vector2(1, 2)
        v += wfn.Vector2(3, 4)
        self.assertEqual(v, wfn.Vector2(4, 6))

        v = wfn.Vector3(1, 2, 3)
        v += wfn.Vector3(4, 5, 6)
        self.assertEqual(v, wfn.Vector3(5, 7, 9))

        v = wfn.Vector4(1, 2, 3, 4)
        v += wfn.Vector4(5, 6, 7, 8)
        self.assertEqual(v, wfn.Vector4(6, 8, 10, 12))

        m = wfn.Matrix3x2(1, 2, 3, 4, 5, 6)
        m += wfn.Matrix3x2(7, 8, 9, 10, 11, 12)
        self.assertEqual(m, wfn.Matrix3x2(8, 10, 12, 14, 16, 18))

        m = wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        m += wfn.Matrix4x4(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
        )
        self.assertEqual(
            m,
            wfn.Matrix4x4(
                18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48
            ),
        )

        q = wfn.Quaternion(1, 2, 3, 4)
        q += wfn.Quaternion(5, 6, 7, 8)
        self.assertEqual(q, wfn.Quaternion(6, 8, 10, 12))

    def test_isub(self):
        v = wfn.Vector2(1, 2)
        v -= wfn.Vector2(3, 4)
        self.assertEqual(v, wfn.Vector2(-2, -2))

        v = wfn.Vector3(1, 2, 3)
        v -= wfn.Vector3(4, 5, 6)
        self.assertEqual(v, wfn.Vector3(-3, -3, -3))

        v = wfn.Vector4(1, 2, 3, 4)
        v -= wfn.Vector4(5, 6, 7, 8)
        self.assertEqual(v, wfn.Vector4(-4, -4, -4, -4))

        m = wfn.Matrix3x2(1, 2, 3, 4, 5, 6)
        m -= wfn.Matrix3x2(7, 8, 9, 10, 11, 12)
        self.assertEqual(m, wfn.Matrix3x2(-6, -6, -6, -6, -6, -6))

        m = wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        m -= wfn.Matrix4x4(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
        )
        self.assertEqual(
            m,
            wfn.Matrix4x4(*[-16] * 16),
        )

        q = wfn.Quaternion(1, 2, 3, 4)
        q -= wfn.Quaternion(5, 6, 7, 8)
        self.assertEqual(q, wfn.Quaternion(-4, -4, -4, -4))

    def test_imul(self):
        v = wfn.Vector2(1, 2)
        v *= wfn.Vector2(3, 4)
        self.assertEqual(v, wfn.Vector2(3, 8))

        v = wfn.Vector2(1, 2)
        v *= 3
        self.assertEqual(v, wfn.Vector2(3, 6))

        v = wfn.Vector3(1, 2, 3)
        v *= wfn.Vector3(4, 5, 6)
        self.assertEqual(v, wfn.Vector3(4, 10, 18))

        v = wfn.Vector3(1, 2, 3)
        v *= 4
        self.assertEqual(v, wfn.Vector3(4, 8, 12))

        v = wfn.Vector4(1, 2, 3, 4)
        v *= wfn.Vector4(5, 6, 7, 8)
        self.assertEqual(v, wfn.Vector4(5, 12, 21, 32))

        v = wfn.Vector4(1, 2, 3, 4)
        v *= 5
        self.assertEqual(v, wfn.Vector4(5, 10, 15, 20))

        m = wfn.Matrix3x2(1, 2, 3, 4, 5, 6)
        m *= wfn.Matrix3x2(7, 8, 9, 10, 11, 12)
        self.assertEqual(m, wfn.Matrix3x2(25, 28, 57, 64, 100, 112))

        m = wfn.Matrix3x2(1, 2, 3, 4, 5, 6)
        m *= 2
        self.assertEqual(m, wfn.Matrix3x2(2, 4, 6, 8, 10, 12))

        m = wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        m *= wfn.Matrix4x4(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
        )
        self.assertEqual(
            m,
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

        m = wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        m *= 2
        self.assertEqual(
            m, wfn.Matrix4x4(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32)
        )

        q = wfn.Quaternion(1, 2, 3, 4)
        q *= wfn.Quaternion(5, 6, 7, 8)
        self.assertEqual(q, wfn.Quaternion(24, 48, 48, -6))

        q = wfn.Quaternion(1, 2, 3, 4)
        q *= 2
        self.assertEqual(q, wfn.Quaternion(2, 4, 6, 8))

    def test_itruediv(self):
        v = wfn.Vector2(1, 2)
        v /= wfn.Vector2(3, 4)
        self.assertEqual(v, wfn.Vector2(1 / 3, 2 / 4))

        v = wfn.Vector2(1, 2)
        v /= 3
        self.assertEqual(v, wfn.Vector2(1 / 3, 2 / 3))

        v = wfn.Vector3(1, 2, 3)
        v /= wfn.Vector3(4, 5, 6)
        self.assertEqual(v, wfn.Vector3(1 / 4, 2 / 5, 3 / 6))

        v = wfn.Vector3(1, 2, 3)
        v /= 4
        self.assertEqual(v, wfn.Vector3(1 / 4, 2 / 4, 3 / 4))

        v = wfn.Vector4(1, 2, 3, 4)
        v /= wfn.Vector4(5, 6, 7, 8)
        self.assertEqual(v, wfn.Vector4(1 / 5, 2 / 6, 3 / 7, 4 / 8))

        v = wfn.Vector4(1, 2, 3, 4)
        v /= 5
        self.assertEqual(v, wfn.Vector4(1 / 5, 2 / 5, 3 / 5, 4 / 5))

        q = wfn.Quaternion(1, 2, 3, 4)
        q /= wfn.Quaternion(5, 6, 7, 8)
        self.assertEqual(
            q, wfn.Quaternion(1, 2, 3, 4) * wfn.Quaternion(5, 6, 7, 8).inverse()
        )

    def test_is_identity(self):
        self.assertTrue(wfn.Matrix3x2(1, 0, 0, 1, 0, 0).is_identity())
        self.assertTrue(
            wfn.Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1).is_identity()
        )
        self.assertTrue(wfn.Quaternion(0, 0, 0, 1).is_identity())

    def test_length(self):
        self.assertEqual(wfn.Vector2(3, 4).length(), 5)
        self.assertEqual(abs(wfn.Vector2(3, 4)), 5)
        self.assertAlmostEqual(wfn.Vector3(1, 2, 3).length(), 14**0.5, places=5)
        self.assertAlmostEqual(abs(wfn.Vector3(1, 2, 3)), 14**0.5, places=5)
        self.assertAlmostEqual(wfn.Vector4(1, 2, 3, 4).length(), 30**0.5, places=5)
        self.assertAlmostEqual(abs(wfn.Vector4(1, 2, 3, 4)), 30**0.5, places=5)
        self.assertAlmostEqual(wfn.Quaternion(1, 2, 3, 4).length(), 30**0.5, places=5)
        self.assertAlmostEqual(abs(wfn.Quaternion(1, 2, 3, 4)), 30**0.5, places=5)

    def test_length_squared(self):
        self.assertEqual(wfn.Vector2(3, 4).length_squared(), 25)
        self.assertEqual(wfn.Vector3(1, 2, 3).length_squared(), 14)
        self.assertEqual(wfn.Vector4(1, 2, 3, 4).length_squared(), 30)
        self.assertEqual(wfn.Quaternion(1, 2, 3, 4).length_squared(), 30)

    def test_distance(self):
        self.assertEqual(wfn.Vector2(1, 2).distance(wfn.Vector2(4, 6)), 5)
        self.assertAlmostEqual(
            wfn.Vector3(1, 2, 3).distance(wfn.Vector3(4, 6, 9)), 61**0.5, places=5
        )
        self.assertAlmostEqual(
            wfn.Vector4(1, 2, 3, 4).distance(wfn.Vector4(4, 6, 9, 12)),
            125**0.5,
            places=5,
        )

    def test_distance_squared(self):
        self.assertEqual(wfn.Vector2(1, 2).distance_squared(wfn.Vector2(4, 6)), 25)
        self.assertEqual(
            wfn.Vector3(1, 2, 3).distance_squared(wfn.Vector3(4, 6, 9)), 61
        )
        self.assertEqual(
            wfn.Vector4(1, 2, 3, 4).distance_squared(wfn.Vector4(4, 6, 9, 12)), 125
        )

    def test_dot(self):
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

    def test_cross(self):
        self.assertEqual(
            wfn.Vector3(1, 2, 3).cross(wfn.Vector3(4, 5, 6)), wfn.Vector3(-3, 6, -3)
        )

    def test_normalize(self):
        self.assertEqual(wfn.Vector2(3, 4).normalize(), wfn.Vector2(3 / 5, 4 / 5))

        n = wfn.Vector3(1, 2, 3).normalize()
        self.assertAlmostEqual(n.x, 1 / 14**0.5, places=5)
        self.assertAlmostEqual(n.y, 2 / 14**0.5, places=5)
        self.assertAlmostEqual(n.z, 3 / 14**0.5, places=5)

        n = wfn.Vector4(1, 2, 3, 4).normalize()
        self.assertAlmostEqual(n.x, 1 / 30**0.5, places=5)
        self.assertAlmostEqual(n.y, 2 / 30**0.5, places=5)
        self.assertAlmostEqual(n.z, 3 / 30**0.5, places=5)
        self.assertAlmostEqual(n.w, 4 / 30**0.5, places=5)

        n = wfn.Plane(wfn.Vector3(1, 2, 3), 4).normalize()
        self.assertAlmostEqual(n.normal.x, 1 / 14**0.5, places=5)
        self.assertAlmostEqual(n.normal.y, 2 / 14**0.5, places=5)
        self.assertAlmostEqual(n.normal.z, 3 / 14**0.5, places=5)
        self.assertAlmostEqual(n.d, 4 / 14**0.5, places=5)

        n = wfn.Quaternion(1, 2, 3, 4).normalize()
        self.assertAlmostEqual(n.x, 1 / 30**0.5, places=5)
        self.assertAlmostEqual(n.y, 2 / 30**0.5, places=5)
        self.assertAlmostEqual(n.z, 3 / 30**0.5, places=5)
        self.assertAlmostEqual(n.w, 4 / 30**0.5, places=5)

    def test_reflect(self):
        self.assertEqual(
            wfn.Vector2(1, 2).reflect(wfn.Vector2(3, 4)), wfn.Vector2(-65, -86)
        )
        self.assertEqual(
            wfn.Vector3(1, 2, 3).reflect(wfn.Vector3(4, 5, 6)),
            wfn.Vector3(-255, -318, -381),
        )

    def test_min(self):
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

    def test_max(self):
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

    def test_clamp(self):
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

    def test_lerp(self):
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
    def test_transform(self):
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

    def test_deteminant(self):
        self.assertEqual(wfn.Matrix3x2(1, 2, 3, 4, 5, 6).determinant(), -2)
        self.assertEqual(
            wfn.Matrix4x4(
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
            ).determinant(),
            0,
        )

    def test_translation(self):
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
    def test_invert(self):
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
    def test_decompose(self):
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

    def test_transpose(self):
        self.assertEqual(
            wfn.Matrix4x4(
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
            ).transpose(),
            wfn.Matrix4x4(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16),
        )

    def test_conjugate(self):
        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4).conjugate(), wfn.Quaternion(-1, -2, -3, 4)
        )

    def test_inverse(self):
        q = wfn.Quaternion(1, 2, 3, 4).inverse()
        self.assertAlmostEqual(q.x, -0.033333, places=5)
        self.assertAlmostEqual(q.y, -0.066667, places=5)
        self.assertAlmostEqual(q.z, -0.1, places=5)
        self.assertAlmostEqual(q.w, 0.133333, places=5)

    def test_concatenate(self):
        self.assertEqual(
            wfn.Quaternion(1, 2, 3, 4).concatenate(wfn.Quaternion(5, 6, 7, 8)),
            wfn.Quaternion(32, 32, 56, -6),
        )
