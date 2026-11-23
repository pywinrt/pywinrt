"""
Asking git about this repository.

Where git is not on PATH, as in an MSYS2 shell by default, these are skipped.
"""

import contextlib
import shutil
import unittest

import repo
import versions

HAS_GIT = shutil.which("git") is not None


@unittest.skipUnless(HAS_GIT, "git is not on PATH")
class Git(unittest.TestCase):
    def test_runs_at_the_root(self) -> None:
        # from a directory below the root, which git would otherwise run in
        with contextlib.chdir(versions.REPO_PATH / "scripts"):
            self.assertEqual(repo.git("rev-parse", "--show-prefix").strip(), "")


@unittest.skipUnless(HAS_GIT, "git is not on PATH")
class Exists(unittest.TestCase):
    def test_a_revision_that_resolves(self) -> None:
        self.assertTrue(repo.exists("HEAD"))

    def test_a_tag_that_does_not(self) -> None:
        self.assertFalse(repo.exists("refs/tags/no/such/tag"))
