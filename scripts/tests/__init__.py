import importlib.util
import sys
from pathlib import Path
from types import ModuleType

SCRIPTS = Path(__file__).parent.parent

# The scripts import one another by their bare names, as they do when they are
# run as files, and a script that load_script() runs imports the modules beside
# it the same way. A test imports them by those names too. All of that needs
# their directory on sys.path.
sys.path.insert(0, str(SCRIPTS))


def load_script(name: str) -> ModuleType:
    """
    The script @p name in scripts/, such as build-bdist.py, loaded as a module.

    A script's file name has hyphens in it, which an import statement cannot
    name, so it is loaded from its path, under its name with underscores.
    """
    path = SCRIPTS / name
    spec = importlib.util.spec_from_file_location(path.stem.replace("-", "_"), path)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)

    return module
