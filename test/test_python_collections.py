"""
A Python list, dict or iterable passed where WinRT wants a collection.

test_collections.py is the other direction - a WinRT collection read from
Python - and these are the calls where the Python object is the collection and
WinRT is the one reading it. TestComponent's collectionN() methods take one of
each of the five collection interfaces and hand back a copy of it; its
collectionN_call() methods run the pass the other way round, calling a Python
handler that returns one and then reading, changing and emptying what it gets,
which is what reaches the members that only a mutable collection has.
"""

import sys
import unittest
from typing import Any

import test_winrt.testcomponent as tc
from winrt.system.hresult import E_BOUNDS, PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION

from ._util import catch_unraisable


class LoggingSequence:
    """A sequence that records the Python operations WinRT reads it with."""

    def __init__(self, items: list[str]) -> None:
        self.items = list(items)
        self.log: list[str] = []

    def __len__(self) -> int:
        self.log.append("len")
        return len(self.items)

    def __getitem__(self, index: int) -> str:
        self.log.append("getitem")
        return self.items[index]


class LoggingMapping:
    """A mapping that records the Python operations WinRT reads it with."""

    def __init__(self, items: dict[str, str]) -> None:
        self.items = dict(items)
        self.log: list[str] = []

    def __len__(self) -> int:
        self.log.append("len")
        return len(self.items)

    def __getitem__(self, key: str) -> str:
        self.log.append("getitem")
        return self.items[key]

    def __iter__(self) -> Any:
        self.log.append("iter")
        return iter(self.items)


class LoggingList(list):  # type: ignore[type-arg]
    """A list that records every operation, including the ones that change it."""

    log: list[str]

    def __init__(self, items: Any) -> None:
        super().__init__(items)
        self.log = []

    def __len__(self) -> int:
        self.log.append("len")
        return super().__len__()

    def __getitem__(self, index: Any) -> Any:
        self.log.append("getitem")
        return super().__getitem__(index)

    def __setitem__(self, index: Any, value: Any) -> None:
        self.log.append("setitem")
        super().__setitem__(index, value)

    def __delitem__(self, index: Any) -> None:
        self.log.append("delslice" if isinstance(index, slice) else "delitem")
        super().__delitem__(index)

    def __iter__(self) -> Any:
        self.log.append("iter")
        return super().__iter__()

    def append(self, value: Any) -> None:
        self.log.append("append")
        super().append(value)


class LoggingDict(dict):  # type: ignore[type-arg]
    """A dict that records every operation, including the ones that change it."""

    log: list[str]

    def __init__(self, items: Any) -> None:
        super().__init__(items)
        self.log = []

    def __len__(self) -> int:
        self.log.append("len")
        return super().__len__()

    def __getitem__(self, key: Any) -> Any:
        self.log.append("getitem")
        return super().__getitem__(key)

    def __setitem__(self, key: Any, value: Any) -> None:
        self.log.append("setitem")
        super().__setitem__(key, value)

    def __delitem__(self, key: Any) -> None:
        self.log.append("delitem")
        super().__delitem__(key)

    def __iter__(self) -> Any:
        self.log.append("iter")
        return super().__iter__()

    def clear(self) -> None:
        self.log.append("clear")
        super().clear()


class TestSequenceAsCollection(unittest.TestCase):
    def setUp(self) -> None:
        self.tests = tc.TestRunner.make_tests()

    def test_a_list_is_an_iterable(self) -> None:
        items = ["1", "2", "3"]

        ret, out = self.tests.collection1(items)

        self.assertListEqual(list(ret), items)
        self.assertListEqual(list(out), items)

    def test_a_generator_is_an_iterable(self) -> None:
        items = ["1", "2", "3"]

        ret, _ = self.tests.collection1(x for x in items)

        self.assertListEqual(list(ret), items)

    def test_a_list_is_a_vector(self) -> None:
        items = ["1", "2", "3"]

        ret, out = self.tests.collection5(items)

        self.assertListEqual(list(ret), items)
        self.assertListEqual(list(out), items)

    def test_a_tuple_is_a_vector(self) -> None:
        # Nothing this call asks a vector of changes it, so a sequence that
        # cannot be changed still stands in for one.
        ret, _ = self.tests.collection5(("1", "2"))  # type: ignore

        self.assertListEqual(list(ret), ["1", "2"])

    def test_a_list_is_a_vector_view(self) -> None:
        items = ["1", "2", "3"]

        ret, out = self.tests.collection6(items)

        self.assertListEqual(list(ret), items)
        self.assertListEqual(list(out), items)

    def test_an_empty_list_is_a_collection(self) -> None:
        for method in (
            self.tests.collection1,
            self.tests.collection5,
            self.tests.collection6,
        ):
            with self.subTest(method=method):
                ret, _ = method([])

                self.assertListEqual(list(ret), [])

    def test_a_sequence_is_read_rather_than_copied(self) -> None:
        sequence = LoggingSequence(["1", "2"])

        self.tests.collection5(sequence)  # type: ignore

        # The call reached the Python object itself, which is what makes both
        # sides see the same collection.
        self.assertIn("len", sequence.log)
        self.assertIn("getitem", sequence.log)

    def test_the_sequence_is_let_go_of_after_the_call(self) -> None:
        items = ["1", "2"]
        before = sys.getrefcount(items)

        self.tests.collection5(items)

        self.assertEqual(sys.getrefcount(items), before)

    def test_an_index_the_sequence_does_not_have_is_out_of_bounds(self) -> None:
        class ShortSequence:
            def __len__(self) -> int:
                return 3

            def __getitem__(self, index: int) -> str:
                return ["1", "2"][index]

        # The one failure WinRT has a name for, so the call reports it rather
        # than the unraisable hook.
        with self.assertRaises(OSError) as ctx, catch_unraisable() as exceptions:
            self.tests.collection5(ShortSequence())  # type: ignore

        self.assertEqual(ctx.exception.winerror, E_BOUNDS)
        self.assertEqual(exceptions, [])

    def test_an_element_of_the_wrong_type_is_unraisable(self) -> None:
        # There is no Python caller to raise it to while WinRT is reading the
        # list, so the TypeError goes to the unraisable hook and the call
        # fails with the HRESULT that says one was raised.
        with self.assertRaises(OSError) as ctx, catch_unraisable() as exceptions:
            self.tests.collection5([1])  # type: ignore

        self.assertEqual(ctx.exception.winerror, PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION)
        self.assertEqual(len(exceptions), 1)
        self.assertEqual(exceptions[0].exc_type, TypeError)

    def test_something_that_is_not_a_sequence_is_refused(self) -> None:
        with self.assertRaisesRegex(TypeError, "sequence"):
            self.tests.collection5(42)  # type: ignore

    def test_something_that_cannot_be_iterated_is_refused(self) -> None:
        with self.assertRaisesRegex(TypeError, "iterable"):
            self.tests.collection1(42)  # type: ignore

    def test_a_winrt_collection_is_still_passed_as_itself(self) -> None:
        ret, _ = self.tests.collection5(["1", "2"])

        again, _ = self.tests.collection5(ret)

        self.assertListEqual(list(again), ["1", "2"])


class TestMappingAsCollection(unittest.TestCase):
    def setUp(self) -> None:
        self.tests = tc.TestRunner.make_tests()

    def test_a_dict_is_a_map(self) -> None:
        entries = {"1": "2", "3": "4"}

        ret, out = self.tests.collection3(entries)

        self.assertDictEqual(dict(ret), entries)
        self.assertDictEqual(dict(out), entries)

    def test_a_dict_is_a_map_view(self) -> None:
        entries = {"1": "2", "3": "4"}

        ret, out = self.tests.collection4(entries)

        self.assertDictEqual(dict(ret), entries)
        self.assertDictEqual(dict(out), entries)

    def test_a_dict_is_an_iterable_of_pairs(self) -> None:
        entries = {"1": "2", "3": "4"}

        ret, out = self.tests.collection2(entries)

        # An IIterable<IKeyValuePair<K, V>> yields pairs where Python yields
        # keys, so the runtime makes a pair of each entry as it goes.
        self.assertDictEqual({i.key: i.value for i in ret}, entries)
        self.assertDictEqual({i.key: i.value for i in out}, entries)

    def test_an_empty_dict_is_a_collection(self) -> None:
        for method in (
            self.tests.collection2,
            self.tests.collection3,
            self.tests.collection4,
        ):
            with self.subTest(method=method):
                ret, _ = method({})

                self.assertEqual(len(list(ret)), 0)

    def test_a_mapping_is_read_rather_than_copied(self) -> None:
        mapping = LoggingMapping({"1": "2"})

        self.tests.collection3(mapping)  # type: ignore

        self.assertIn("iter", mapping.log)
        self.assertIn("getitem", mapping.log)

    def test_the_mapping_is_let_go_of_after_the_call(self) -> None:
        entries = {"1": "2"}
        before = sys.getrefcount(entries)

        self.tests.collection3(entries)

        self.assertEqual(sys.getrefcount(entries), before)

    def test_something_that_is_not_a_mapping_is_refused(self) -> None:
        with self.assertRaisesRegex(TypeError, "mapping"):
            self.tests.collection3(42)  # type: ignore


class TestCollectionFromAHandler(unittest.TestCase):
    """
    The same collections, handed to WinRT out of a Python implementation.

    A value that leaves a call the other way round is converted by the
    callback half of the interpreter rather than by the call half, so this is
    the second of the two places a Python object becomes a WinRT collection -
    and the only one that reaches the members WinRT uses to change one, since
    TestComponent checks each step it takes and fails the call if what it
    reads back is not what it asked for.
    """

    def setUp(self) -> None:
        self.tests = tc.TestRunner.make_tests()

    def test_a_handler_hands_back_an_iterable(self) -> None:
        handed: list[LoggingList] = []

        def handler(items):
            handed.append(LoggingList(items))
            handed.append(LoggingList(items))
            return handed[-2], handed[-1]

        self.tests.collection1_call(handler)

        self.assertIn("iter", handed[0].log)

    def test_a_handler_hands_back_a_vector(self) -> None:
        handed: list[LoggingList] = []

        def handler(items):
            handed.append(LoggingList(items))
            handed.append(LoggingList(items))
            return handed[-2], handed[-1]

        self.tests.collection5_call(handler)

        # The call is what says the members did the right thing - it checks
        # every step and fails if they did not - and the log is which of them
        # it took.
        log = set(handed[0].log)

        self.assertIn("len", log)
        self.assertIn("getitem", log)
        self.assertIn("iter", log)
        self.assertIn("append", log)
        self.assertIn("setitem", log)
        self.assertIn("delitem", log)
        self.assertIn("delslice", log)

    def test_a_handler_hands_back_a_vector_view(self) -> None:
        handed: list[LoggingList] = []

        def handler(items):
            handed.append(LoggingList(items))
            handed.append(LoggingList(items))
            return handed[-2], handed[-1]

        self.tests.collection6_call(handler)

        log = set(handed[0].log)

        self.assertIn("len", log)
        self.assertIn("getitem", log)

    def test_a_handler_hands_back_a_map(self) -> None:
        handed: list[LoggingDict] = []

        def handler(entries):
            values = {key: entries.lookup(key) for key in entries}
            handed.append(LoggingDict(values))
            handed.append(LoggingDict(values))
            return handed[-2], handed[-1]

        self.tests.collection3_call(handler)

        log = set(handed[0].log)

        self.assertIn("len", log)
        self.assertIn("getitem", log)
        self.assertIn("iter", log)
        self.assertIn("setitem", log)
        self.assertIn("delitem", log)
        self.assertIn("clear", log)

    def test_a_handler_hands_back_a_map_view(self) -> None:
        handed: list[LoggingDict] = []

        def handler(entries):
            values = {key: entries.lookup(key) for key in entries}
            handed.append(LoggingDict(values))
            handed.append(LoggingDict(values))
            return handed[-2], handed[-1]

        self.tests.collection4_call(handler)

        self.assertIn("getitem", handed[0].log)

    def test_a_handler_hands_back_an_iterable_of_pairs(self) -> None:
        handed: list[LoggingDict] = []

        def handler(pairs):
            values = {pair.key: pair.value for pair in pairs}
            handed.append(LoggingDict(values))
            handed.append(LoggingDict(values))
            return handed[-2], handed[-1]

        self.tests.collection2_call(handler)

        self.assertIn("iter", handed[0].log)


class TestCollectionElements(unittest.TestCase):
    def test_a_list_of_numbers_is_an_iterable(self) -> None:
        # Every element goes through the conversion its own type code names,
        # so a collection of something other than strings needs nothing of its
        # own.
        expected = [1, 2, 3]
        actual: list[int] = []

        op = tc.TestRunner.create_async_action_with_progress(10, expected)
        op.progress = lambda sender, value: actual.append(value)
        op.get()

        self.assertListEqual(actual, expected)


if __name__ == "__main__":
    unittest.main()
