import unittest

import winrt.windows.foundation.numerics as wfn


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
            r"unsupported operand type\(s\) for \+: 'winrt._winrt_windows_foundation_numerics.Plane' and 'int'",
        ):
            wfn.Plane(wfn.Vector3(), 0) + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: 'winrt._winrt_windows_foundation_numerics.Vector2' and 'int'",
        ):
            wfn.Vector2() + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: 'winrt._winrt_windows_foundation_numerics.Vector3' and 'int'",
        ):
            wfn.Vector3() + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: 'winrt._winrt_windows_foundation_numerics.Vector4' and 'int'",
        ):
            wfn.Vector4() + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: 'winrt._winrt_windows_foundation_numerics.Matrix3x2' and 'int'",
        ):
            wfn.Matrix3x2() + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: 'winrt._winrt_windows_foundation_numerics.Matrix4x4' and 'int'",
        ):
            wfn.Matrix4x4() + 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: 'winrt._winrt_windows_foundation_numerics.Quaternion' and 'int'",
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
            r"unsupported operand type\(s\) for -: 'winrt._winrt_windows_foundation_numerics.Plane' and 'int'",
        ):
            wfn.Plane(wfn.Vector3(), 0) - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: 'winrt._winrt_windows_foundation_numerics.Vector2' and 'int'",
        ):
            wfn.Vector2() - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: 'winrt._winrt_windows_foundation_numerics.Vector3' and 'int'",
        ):
            wfn.Vector3() - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: 'winrt._winrt_windows_foundation_numerics.Vector4' and 'int'",
        ):
            wfn.Vector4() - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: 'winrt._winrt_windows_foundation_numerics.Matrix3x2' and 'int'",
        ):
            wfn.Matrix3x2() - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: 'winrt._winrt_windows_foundation_numerics.Matrix4x4' and 'int'",
        ):
            wfn.Matrix4x4() - 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: 'winrt._winrt_windows_foundation_numerics.Quaternion' and 'int'",
        ):
            wfn.Quaternion() - 1
