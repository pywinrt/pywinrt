# PyInstaller spec for hello_app.py: pyinstaller hello_app.spec
#
# Two things make the executable able to host a XAML Island where python.exe
# cannot: the manifest embedded in it, and the resources.pri beside it.

import ctypes
import os
import shutil
import sys
from ctypes import wintypes

sys.path.insert(0, SPECPATH)

from hello_app import WINUI2_PACKAGE_FAMILY, add_package_dependency


def get_package_path(package_full_name: str) -> str:
    """
    Where an installed package is.
    """
    kernel32 = ctypes.WinDLL("kernel32")
    length = wintypes.UINT(0)
    kernel32.GetPackagePathByFullName(package_full_name, ctypes.byref(length), None)
    path = ctypes.create_unicode_buffer(length.value)
    error = kernel32.GetPackagePathByFullName(
        package_full_name, ctypes.byref(length), path
    )

    if error:
        raise ctypes.WinError(error)

    return path.value


a = Analysis(["hello_app.py"])
pyz = PYZ(a.pure)
exe = EXE(
    pyz,
    a.scripts,
    [],
    exclude_binaries=True,
    name="hello_app",
    manifest=os.path.join(SPECPATH, "hello_app.manifest"),
)
coll = COLLECT(exe, a.binaries, a.datas, name="hello_app")

# An unpackaged process reads its resources from the resources.pri beside its
# executable, and XamlControlsResources loads WinUI 2's styles through
# ms-appx://Microsoft.UI.Xaml.2.8/, so that file has to be the framework
# package's own. It is taken from the version the application will resolve to
# when it runs, which is the newest one installed.
shutil.copyfile(
    os.path.join(
        get_package_path(add_package_dependency(WINUI2_PACKAGE_FAMILY)),
        "resources.pri",
    ),
    os.path.join(DISTPATH, "hello_app", "resources.pri"),
)
