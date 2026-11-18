import array
import math
import os
import sys
import typing
import unittest

import winrt.windows.foundation.numerics as wfn

ON_MINGW = "MINGW_PREFIX" in os.environ


class TestNumericsOperators(unittest.TestCase):
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

        # a tuple of the fields is no matrix for an operator, so the other side
        # answers, which for * and a tuple is repetition
        with self.assertRaisesRegex(TypeError, "can't multiply sequence"):
            wfn.Matrix4x4() * tuple(range(16))  # type: ignore

        with self.assertRaisesRegex(TypeError, "can't multiply sequence"):
            tuple(range(16)) * wfn.Matrix4x4()  # type: ignore

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for @: '[\w+\.]*Matrix4x4' and 'tuple'",
        ):
            wfn.Matrix4x4.identity @ tuple(range(16))  # type: ignore

    @unittest.skipIf(sys.version_info < (3, 12), "__buffer__ is new in 3.12")
    def test_matrix_buffer_operand_is_not_implemented(self) -> None:
        class Rows:
            """
            A block of floats in a matrix's shape that answers the operators
            itself, as a NumPy array does.
            """

            def __buffer__(self, flags: int, /) -> "memoryview[float]":
                return (
                    memoryview(array.array("f", range(16))).cast("B").cast("f", (4, 4))
                )

            def __radd__(self, other: object) -> str:
                return "radd"

            def __rmul__(self, other: object) -> str:
                return "rmul"

            def __mul__(self, other: object) -> str:
                return "mul"

            def __rmatmul__(self, other: object) -> str:
                return "rmatmul"

        rows: typing.Any = Rows()

        self.assertEqual(wfn.Matrix4x4() + rows, "radd")
        self.assertEqual(wfn.Matrix4x4() * rows, "rmul")
        self.assertEqual(rows * wfn.Matrix4x4(), "mul")
        self.assertEqual(wfn.Matrix4x4.identity @ rows, "rmatmul")

    def test_tuple_is_not_an_operand(self) -> None:
        # A tuple of the fields is taken where a method takes the struct, but
        # beside an operator it reads as a sequence, so the other side answers.
        v = wfn.Vector3(1, 0, 0)
        t: typing.Any = (0, 1, 0)

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Vector3' and 'tuple'",
        ):
            _ = v + t

        with self.assertRaisesRegex(TypeError, "can only concatenate tuple"):
            _ = t + v

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for -: '[\w+\.]*Vector3' and 'tuple'",
        ):
            _ = v - t

        with self.assertRaisesRegex(TypeError, "can't multiply sequence"):
            _ = v * t

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for /: '[\w+\.]*Vector3' and 'tuple'",
        ):
            _ = v / t

        # A buffer of floats is taken where a method takes the struct as well,
        # but not beside an operator, and array.array has no answer of its own.
        a: typing.Any = array.array("f", [1, 1, 1])

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for \+: '[\w+\.]*Vector3' and 'array.array'",
        ):
            _ = v + a

        q: typing.Any = (0, 0, 0, 1)

        with self.assertRaisesRegex(TypeError, "can't multiply sequence"):
            _ = wfn.Quaternion.identity * q

        with self.assertRaisesRegex(
            TypeError,
            r"unsupported operand type\(s\) for @: '[\w+\.]*Quaternion' and 'tuple'",
        ):
            _ = wfn.Quaternion.identity @ q

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
