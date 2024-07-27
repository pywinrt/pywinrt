import unittest

import winrt.windows.foundation.numerics as wfn


# TODO: replace this when inverse method is implemented on Quaternion
def inverse(q: wfn.Quaternion) -> wfn.Quaternion:
    return wfn.Quaternion(-q.x, -q.y, -q.z, q.w) * (
        1 / (q.x**2 + q.y**2 + q.z**2 + q.w**2)
    )


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
            r"unsupported operand type\(s\) for \*: 'winrt._winrt_windows_foundation_numerics.Plane' and 'object'",
        ):
            wfn.Plane(wfn.Vector3(), 0) * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'winrt._winrt_windows_foundation_numerics.Vector2' and 'object'",
        ):
            wfn.Vector2() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'winrt._winrt_windows_foundation_numerics.Vector3' and 'object'",
        ):
            wfn.Vector3() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'winrt._winrt_windows_foundation_numerics.Vector4' and 'object'",
        ):
            wfn.Vector4() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'winrt._winrt_windows_foundation_numerics.Matrix3x2' and 'object'",
        ):
            wfn.Matrix3x2() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'int' and 'winrt._winrt_windows_foundation_numerics.Matrix3x2'",
        ):
            1 * wfn.Matrix3x2()

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'winrt._winrt_windows_foundation_numerics.Matrix4x4' and 'object'",
        ):
            wfn.Matrix4x4() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'int' and 'winrt._winrt_windows_foundation_numerics.Matrix4x4'",
        ):
            1 * wfn.Matrix4x4()

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'winrt._winrt_windows_foundation_numerics.Quaternion' and 'object'",
        ):
            wfn.Quaternion() * o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \*: 'int' and 'winrt._winrt_windows_foundation_numerics.Quaternion'",
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
            wfn.Quaternion(1, 2, 3, 4) * inverse(wfn.Quaternion(5, 6, 7, 8)),
        )

    def test_truediv_bad_type(self):
        o = object()
        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: 'winrt._winrt_windows_foundation_numerics.Plane' and 'object'",
        ):
            wfn.Plane(wfn.Vector3(), 0) / o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: 'winrt._winrt_windows_foundation_numerics.Vector2' and 'object'",
        ):
            wfn.Vector2() / o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: 'winrt._winrt_windows_foundation_numerics.Vector3' and 'object'",
        ):
            wfn.Vector3() / o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: 'winrt._winrt_windows_foundation_numerics.Vector4' and 'object'",
        ):
            wfn.Vector4() / o

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: 'winrt._winrt_windows_foundation_numerics.Matrix3x2' and 'int'",
        ):
            wfn.Matrix3x2() / 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: 'winrt._winrt_windows_foundation_numerics.Matrix4x4' and 'int'",
        ):
            wfn.Matrix4x4() / 1

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: 'winrt._winrt_windows_foundation_numerics.Quaternion' and 'int'",
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
            q, wfn.Quaternion(1, 2, 3, 4) * inverse(wfn.Quaternion(5, 6, 7, 8))
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
        self.assertEqual(
            wfn.Plane(wfn.Vector3(1, 2, 3), 4).dot(wfn.Vector4(5, 6, 7, 8)), 70
        )
        self.assertEqual(
            wfn.Plane(wfn.Vector3(1, 2, 3), 4).dot_coordinate(wfn.Vector3(5, 6, 7)), 42
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
        self.assertEqual(
            wfn.Matrix3x2(1, 2, 3, 4, 5, 6).lerp(
                wfn.Matrix3x2(7, 8, 9, 10, 11, 12), 0.5
            ),
            wfn.Matrix3x2(4, 5, 6, 7, 8, 9),
        )
        self.assertEqual(
            wfn.Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16).lerp(
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
