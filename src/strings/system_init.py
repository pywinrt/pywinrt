from importlib.machinery import ExtensionFileLoader
from importlib.util import spec_from_loader, module_from_spec
import sys
from types import ModuleType
import uuid

from .._winrt import __file__ as _winrt_file, Array as Array


def _import_ns_module(ns: str) -> ModuleType:
    module_name = f"_%_{ns.replace('.', '_')}"
    loader = ExtensionFileLoader(module_name, _winrt_file)
    spec = spec_from_loader(module_name, loader)
    assert spec is not None
    module = module_from_spec(spec)
    loader.exec_module(module)
    return module


if sys.version_info >= (3, 9):
    from typing import Annotated

    # type aliases with struct format string annotation
    Boolean = Annotated[bool, "?"]
    Int8 = Annotated[int, "b"]
    UInt8 = Annotated[int, "B"]
    Int16 = Annotated[int, "h"]
    UInt16 = Annotated[int, "H"]
    Int32 = Annotated[int, "i"]
    UInt32 = Annotated[int, "I"]
    Int64 = Annotated[int, "q"]
    UInt64 = Annotated[int, "Q"]
    Single = Annotated[float, "f"]
    Double = Annotated[float, "d"]
    Char16 = Annotated[str, "u"]
    Guid = Annotated[uuid.UUID, "T{I2H8B}"]
else:
    Boolean = bool
    Int8 = int
    UInt8 = int
    Int16 = int
    UInt16 = int
    Int32 = int
    UInt32 = int
    Int64 = int
    UInt64 = int
    Single = float
    Double = float
    Char16 = str
    Guid = uuid.UUID
