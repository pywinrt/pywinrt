from importlib.machinery import ExtensionFileLoader
from importlib.util import spec_from_loader, module_from_spec
from types import ModuleType

from . import _winrt

def _import_ns_module(ns: str) -> ModuleType:
    module_name = f"_%_{ns.replace('.', '_')}"
    loader = ExtensionFileLoader(module_name, _winrt.__file__)
    spec = spec_from_loader(module_name, loader)
    module = module_from_spec(spec)
    loader.exec_module(module)
    return module
