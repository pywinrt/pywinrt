from uuid import UUID
import unittest

import winrt.testcomponent as tc


class TestTestComponent(unittest.TestCase):
    def test_struct_subclass(self):
        # subclassing a struct type is not allowed
        with self.assertRaisesRegex(TypeError, "not an acceptable base type"):

            class s(tc.Blittable):
                pass

    def test_class_subclass(self):
        # subclassing a class type is not allowed
        with self.assertRaisesRegex(TypeError, "not an acceptable base type"):

            class s(tc.TestRunner):
                pass

    def test_interface_subclass(self):
        # subclassing a interface type is not allowed
        with self.assertRaisesRegex(TypeError, "not an acceptable base type"):

            class s(tc.ITests):
                pass

    def test_blittable(self):
        b = tc.Blittable()

        self.assertEqual(b.a, 0)
        self.assertEqual(b.b, 0)
        self.assertEqual(b.c, 0)
        self.assertEqual(b.d, 0)
        self.assertEqual(b.e, 0)
        self.assertEqual(b.f, 0)
        self.assertEqual(b.g, 0)
        self.assertEqual(b.h, 0)
        self.assertEqual(b.i, 0)
        self.assertEqual(b.j, UUID("00000000-0000-0000-0000-000000000000"))

        b.a = 1
        b.b = 2
        b.c = 3
        b.d = 4
        b.e = 5
        b.f = 6
        b.g = 7
        b.h = 8
        b.i = 9
        b.j = UUID("10000000-1000-1000-1000-100000000000")

        self.assertEqual(b.a, 1)
        self.assertEqual(b.b, 2)
        self.assertEqual(b.c, 3)
        self.assertEqual(b.d, 4)
        self.assertEqual(b.e, 5)
        self.assertEqual(b.f, 6)
        self.assertEqual(b.g, 7)
        self.assertEqual(b.h, 8)
        self.assertEqual(b.i, 9)
        self.assertEqual(b.j, UUID("10000000-1000-1000-1000-100000000000"))

        b = tc.Blittable(
            1, 2, 3, 4, 5, 6, 7, 8, 9, UUID("10000000-1000-1000-1000-100000000000")
        )

        self.assertEqual(b.a, 1)
        self.assertEqual(b.b, 2)
        self.assertEqual(b.c, 3)
        self.assertEqual(b.d, 4)
        self.assertEqual(b.e, 5)
        self.assertEqual(b.f, 6)
        self.assertEqual(b.g, 7)
        self.assertEqual(b.h, 8)
        self.assertEqual(b.i, 9)
        self.assertEqual(b.j, UUID("10000000-1000-1000-1000-100000000000"))

    def test_non_blittable(self):
        nb = tc.NonBlittable()

        self.assertIs(nb.a, False)
        self.assertEqual(nb.b, "\x00")
        self.assertEqual(nb.c, "")
        self.assertIsNone(nb.d)

        nb.a = True
        nb.b = "b"
        nb.c = "c"
        nb.d = 4

        self.assertIs(nb.a, True)
        self.assertEqual(nb.b, "b")
        self.assertEqual(nb.c, "c")
        self.assertEqual(nb.d, 4)

        nb = tc.NonBlittable(True, "b", "c", 4)
        self.assertIs(nb.a, True)
        self.assertEqual(nb.b, "b")
        self.assertEqual(nb.c, "c")
        self.assertEqual(nb.d, 4)

    def test_nested(self):
        n = tc.Nested()

        self.assertEqual(n.blittable.a, 0)
        self.assertEqual(n.blittable.b, 0)
        self.assertEqual(n.blittable.c, 0)
        self.assertEqual(n.blittable.d, 0)
        self.assertEqual(n.blittable.e, 0)
        self.assertEqual(n.blittable.f, 0)
        self.assertEqual(n.blittable.g, 0)
        self.assertEqual(n.blittable.h, 0)
        self.assertEqual(n.blittable.i, 0)
        self.assertEqual(n.blittable.j, UUID("00000000-0000-0000-0000-000000000000"))

        self.assertIs(n.non_blittable.a, False)
        self.assertEqual(n.non_blittable.b, "\x00")
        self.assertEqual(n.non_blittable.c, "")
        self.assertIsNone(n.non_blittable.d)

        n.blittable.a = 1
        n.blittable.b = 2
        n.blittable.c = 3
        n.blittable.d = 4
        n.blittable.e = 5
        n.blittable.f = 6
        n.blittable.g = 7
        n.blittable.h = 8
        n.blittable.i = 9
        n.blittable.j = UUID("10000000-1000-1000-1000-100000000000")

        n.non_blittable.a = True
        n.non_blittable.b = "b"
        n.non_blittable.c = "c"
        n.non_blittable.d = 4

        # FIXME: nested struct assignment is not working
        # self.assertEqual(n.blittable.a, 1)
        # self.assertEqual(n.blittable.b, 2)
        # self.assertEqual(n.blittable.c, 3)
        # self.assertEqual(n.blittable.d, 4)
        # self.assertEqual(n.blittable.e, 5)
        # self.assertEqual(n.blittable.f, 6)
        # self.assertEqual(n.blittable.g, 7)
        # self.assertEqual(n.blittable.h, 8)
        # self.assertEqual(n.blittable.i, 9)
        # self.assertEqual(n.blittable.j, UUID('10000000-1000-1000-1000-100000000000'))

        # self.assertIs(n.non_blittable.a, True)
        # self.assertEqual(n.non_blittable.b, "b")
        # self.assertEqual(n.non_blittable.c, "c")
        # self.assertEqual(n.non_blittable.d, 4)

        n = tc.Nested(
            tc.Blittable(
                1, 2, 3, 4, 5, 6, 7, 8, 9, UUID("10000000-1000-1000-1000-100000000000")
            ),
            tc.NonBlittable(True, "b", "c", 4),
        )

        self.assertEqual(n.blittable.a, 1)
        self.assertEqual(n.blittable.b, 2)
        self.assertEqual(n.blittable.c, 3)
        self.assertEqual(n.blittable.d, 4)
        self.assertEqual(n.blittable.e, 5)
        self.assertEqual(n.blittable.f, 6)
        self.assertEqual(n.blittable.g, 7)
        self.assertEqual(n.blittable.h, 8)
        self.assertEqual(n.blittable.i, 9)
        self.assertEqual(n.blittable.j, UUID("10000000-1000-1000-1000-100000000000"))

        self.assertIs(n.non_blittable.a, True)
        self.assertEqual(n.non_blittable.b, "b")
        self.assertEqual(n.non_blittable.c, "c")
        self.assertEqual(n.non_blittable.d, 4)
