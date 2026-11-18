"""
Asking git about this repository.
"""

import subprocess

# the root of the repository, which every command runs in
import versions


def git(*args: str) -> str:
    """
    What a git command prints, run at the root of the repository. A command
    that fails raises CalledProcessError.
    """
    return subprocess.run(
        ["git", *args],
        cwd=versions.REPO_PATH,
        capture_output=True,
        text=True,
        encoding="utf-8",
        check=True,
    ).stdout


def exists(rev: str) -> bool:
    """
    Whether git resolves @p rev, such as a tag as refs/tags/<name>, to an
    object name.
    """
    result = subprocess.run(
        ["git", "rev-parse", "--quiet", "--verify", rev],
        cwd=versions.REPO_PATH,
        capture_output=True,
        check=False,
    )

    return result.returncode == 0
