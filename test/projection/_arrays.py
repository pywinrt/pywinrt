"""
What the tests of winrt.system.Array share: the width of a pointer, and
values of the test component's structs to fill an array with.
"""

import sys
import uuid

import test_winrt.testcomponent as tc

is_64bits = sys.maxsize > 2**32
pointer_size = 8 if is_64bits else 4


def blittable(seed: int) -> tc.Blittable:
    return tc.Blittable(
        seed,
        seed + 1,
        seed + 2,
        seed + 3,
        seed + 4,
        seed + 5,
        seed + 6,
        seed + 7.5,
        seed + 8.5,
        uuid.UUID(int=seed),
    )


def non_blittable(seed: int) -> tc.NonBlittable:
    return tc.NonBlittable(seed % 2 == 0, chr(ord("a") + seed), f"value {seed}", seed)


def nested(seed: int) -> tc.Nested:
    return tc.Nested(blittable(seed), non_blittable(seed))
