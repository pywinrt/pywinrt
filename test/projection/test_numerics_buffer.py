import array
import os
import struct
import typing
import unittest

import winrt.windows.foundation.numerics as wfn
from winrt.system import Array

ON_MINGW = "MINGW_PREFIX" in os.environ


class TestNumericsBuffer(unittest.TestCase):
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

        with self.assertRaisesRegex(
            TypeError, r"as m\[row, column\], not by 3 indices"
        ):
            m[0, 1, 2]  # type: ignore

        with self.assertRaisesRegex(TypeError, r"as m\[row, column\], not by 1 index"):
            m[(0,)]  # type: ignore

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
        # A buffer stands for a struct where a method takes one, the way a
        # tuple does, which the stubs leave out, so the buffers are typed as
        # Any.
        floats: typing.Any = array.array("f", [1, 1, 1])
        doubles: typing.Any = array.array("d", [1, 1, 1])

        v = wfn.Vector3(1, 2, 3)
        self.assertEqual(v.dot(floats), 6)
        self.assertEqual(v.dot(doubles), 6)

        # a matrix from a block in its shape, and not from flat floats, which
        # could be its rows or its columns
        rows: typing.Any = (
            memoryview(array.array("f", range(16))).cast("B").cast("f", (4, 4))
        )
        flat: typing.Any = array.array("f", range(16))

        if not ON_MINGW:
            self.assertEqual(wfn.Matrix4x4().lerp(rows, 1), wfn.Matrix4x4(*range(16)))

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
            r"a buffer read as '[\w+\.]*Vector3' must hold floats or doubles of "
            r"shape \(3,\), not 'f' of shape \(2,\)",
        ):
            v.dot(array.array("f", [1, 1]))  # type: ignore

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

        if not ON_MINGW:
            with self.assertRaisesRegex(
                TypeError,
                r"an Array of Windows.Foundation.Numerics.Vector2 is not a "
                r"'[\w+\.]*Matrix3x2': memoryview\(\) of it reads its floats as one",
            ):
                wfn.Matrix3x2().lerp(Array(wfn.Vector2, 3), 1)  # type: ignore

            self.assertEqual(
                wfn.Matrix3x2().lerp(memoryview(Array(wfn.Vector2, 3)), 1),  # type: ignore
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
