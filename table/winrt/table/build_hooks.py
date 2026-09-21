"""
The hatchling build hook that compiles a projection package's tables.

A projection package carries the text form of each of its namespace tables and
the wheel carries the binary the runtime reads, so building one has exactly one
step. The compiler is :mod:`winrt.table`, which this is part of, so a
projection package reaches the hook by build-depending on
``winrt-table-compiler`` and naming it:

.. code-block:: toml

    [build-system]
    requires = ["hatchling", "winrt-table-compiler"]
    build-backend = "hatchling.build"

    [tool.hatch.build.targets.wheel.hooks.winrt-table-compiler]

Nothing is written back into the package being built: the binaries go to a
temporary directory and into the wheel from there, which is what keeps a
generated binary out of a source tree.
"""

import shutil
import tempfile
from pathlib import Path
from typing import Any

from hatchling.builders.hooks.plugin.interface import BuildHookInterface
from hatchling.plugin import hookimpl

from winrt.table import TABLE_NAME, TABLE_TEXT_NAME, compile_table


class TableBuildHook(BuildHookInterface):
    PLUGIN_NAME = "winrt-table-compiler"

    def initialize(self, version: str, build_data: dict[str, Any]) -> None:
        self.__output = Path(tempfile.mkdtemp(prefix="pywinrt-table-"))

        root = Path(self.root)

        for text in sorted(root.glob(f"**/{TABLE_TEXT_NAME}")):
            table = text.relative_to(root).with_name(TABLE_NAME)
            binary = self.__output / table

            binary.parent.mkdir(parents=True, exist_ok=True)
            compile_table(text, binary)

            # the layout of the package is the layout of the wheel, so the
            # binary lands beside the __init__.py that loads it
            build_data["force_include"][str(binary)] = str(table)

    def finalize(
        self, version: str, build_data: dict[str, Any], artifact_path: str
    ) -> None:
        shutil.rmtree(self.__output, ignore_errors=True)


@hookimpl
def hatch_register_build_hook() -> type[TableBuildHook]:
    return TableBuildHook
