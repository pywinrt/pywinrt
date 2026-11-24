"""
Which of a tree's tests scripts/abi-compat.py runs against TestComponent.

The trees are made up here, holding only the test files that the answer
depends on.
"""

import tempfile
import unittest
from pathlib import Path
from typing import Any

from scripts.tests import load_script

abi_compat = load_script("abi-compat.py")


class TestComponentTests(unittest.TestCase):
    def setUp(self) -> None:
        self.root = Path(self.enterContext(tempfile.TemporaryDirectory()))
        (self.root / "test").mkdir()

    def tree(self, *files: str) -> Any:
        for file in files:
            (self.root / file).parent.mkdir(parents=True, exist_ok=True)
            (self.root / file).touch()

        return abi_compat.Tree(self.root, "baseline v4.0.0")

    def test_old_layout(self) -> None:
        tree = self.tree("test/test_test_component.py")

        self.assertEqual(tree.test_component_tests, ["test.test_test_component"])

    def test_grouped_layout(self) -> None:
        # in TEST_COMPONENT_TESTS order, with a module the tree lacks left out
        tree = self.tree(
            "test/projection/test_async.py",
            "test/projection/test_test_component.py",
            "test/projection/test_composable.py",
            "test/projection/test_other.py",
        )

        self.assertEqual(
            tree.test_component_tests,
            [
                "test.projection.test_test_component",
                "test.projection.test_composable",
                "test.projection.test_async",
            ],
        )

    def test_no_tests_in_the_old_layout(self) -> None:
        tree = self.tree()

        with self.assertRaisesRegex(
            SystemExit,
            r"baseline v4\.0\.0 at .* has none of TestComponent's tests: no"
            r" test/projection and no test/test_test_component\.py$",
        ):
            tree.test_component_tests

    def test_no_tests_in_the_grouped_layout(self) -> None:
        tree = self.tree("test/projection/test_other.py")

        with self.assertRaisesRegex(
            SystemExit,
            r"baseline v4\.0\.0 at .* has none of TestComponent's tests: no"
            r" test_test_component\.py, .* in test/projection$",
        ):
            tree.test_component_tests


if __name__ == "__main__":
    unittest.main()
