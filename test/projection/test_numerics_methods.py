import os
import unittest

import winrt.windows.foundation.numerics as wfn

ON_MINGW = "MINGW_PREFIX" in os.environ


class TestNumericsMethods(unittest.TestCase):
    def test_methods_take_a_tuple(self) -> None:
        # as every struct parameter does, which the stubs say, so these calls
        # type-check as written
        self.assertEqual(wfn.Vector3(1, 0, 0).dot((0, 1, 0)), 0)
        self.assertEqual(
            wfn.Matrix4x4.make_translation((1, 2, 3)).translation(),
            wfn.Vector3(1, 2, 3),
        )

        if not ON_MINGW:
            self.assertEqual(
                wfn.Plane.make_from_vertices((0, 0, 0), (1, 0, 0), (0, 1, 0)).normal,
                wfn.Vector3(0, 0, 1),
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
