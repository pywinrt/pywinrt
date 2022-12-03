from importlib.machinery import ExtensionFileLoader
from importlib.util import spec_from_loader, module_from_spec
from types import ModuleType

from ._winrt import __file__ as _winrt_file, Array as Array


def _import_ns_module(ns: str) -> ModuleType:
    module_name = f"_%_{ns.replace('.', '_')}"
    loader = ExtensionFileLoader(module_name, _winrt_file)
    spec = spec_from_loader(module_name, loader)
    assert spec is not None
    module = module_from_spec(spec)
    loader.exec_module(module)
    return module
