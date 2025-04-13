# /// script
# requires-python = ">=3.10"
# dependencies = [
#   "comtypes",
#   "rich",
#   "winrt-runtime",
#   "winrt-Windows.Data.Xml.Dom",
#   "winrt-Windows.Foundation",
#   "winrt-Windows.Foundation.Collections",
#   "winrt-Windows.UI.Notifications",
# ]
# ///

"""
Example of how to use Windows Toast Notifications with Python.

In particular, shows how to use a COM activator to receive toast activation
so that it works even when the script is not running.

Run this example from the web with:
uv run --script https://raw.githubusercontent.com/pywinrt/pywinrt/main/samples/toast_activation.py

When you are done, run again with the ``-Uninstall`` argument to remove any
registry keys created by this script.
"""

import ctypes
import sys
import time
import winreg
from ctypes import wintypes
from typing import Protocol

from rich.console import Console
from rich.prompt import Prompt
from winrt.system import Object, unbox_string
from winrt.windows.data.xml.dom import XmlDocument
from winrt.windows.ui.notifications import (
    ToastActivatedEventArgs,
    ToastNotification,
    ToastNotificationManager,
)

# Have to do this before importing comtypes to ensure it doesn't call
# CoInitializeEx() with STA_THREADING_MODEL. Omit this if your app is a GUI
# with a Windows message loop.
sys.coinit_flags = 0  # type: ignore

from comtypes import (  # noqa: E402
    CLSCTX_LOCAL_SERVER,
    COMMETHOD,
    GUID,
    COMObject,
    IUnknown,
)
from comtypes.server.localserver import (  # noqa: E402
    REGCLS_MULTIPLEUSE,
    ClassFactory,
    run as run_local_server,
)

# Don't reuse this ID. Create your own for your application.
# https://learn.microsoft.com/en-us/windows/win32/shell/appids
APP_USER_MODULE_ID = "PyWinRT.Example.ToastActivation"
APP_NAME = "PyWinRT Example Toast Activation"
# Don't reuse this UUID, generate your own for your application.
# You can do this by running ``py -m uuid`` in a terminal.
ACTIVATOR_UUID = "{848E2D95-75F6-4AFB-9624-C6A924E174D1}"


class _SetCurrentProcessExplicitAppUserModelID(Protocol):
    def __call__(self, appUserModelId: str) -> None: ...


# https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nf-shobjidl_core-setcurrentprocessexplicitappusermodelid
SetCurrentProcessExplicitAppUserModelID: _SetCurrentProcessExplicitAppUserModelID = (
    ctypes.WINFUNCTYPE(ctypes.HRESULT, wintypes.LPCWSTR)(
        ("SetCurrentProcessExplicitAppUserModelID", ctypes.windll.shell32),
        ((1, "appUserModelId"),),
    )
)


def _errcheck(
    result: int, func: _SetCurrentProcessExplicitAppUserModelID, args: tuple[str]
):
    if result:
        raise ctypes.WinError(result)


SetCurrentProcessExplicitAppUserModelID.errcheck = _errcheck  # type: ignore


# https://learn.microsoft.com/en-us/windows/win32/api/notificationactivationcallback/ns-notificationactivationcallback-notification_user_input_data
class NOTIFICATION_USER_INPUT_DATA(ctypes.Structure):
    Key: str
    Value: str

    _fields_ = [
        ("Key", wintypes.LPCWSTR),
        ("Value", wintypes.LPCWSTR),
    ]


# https://learn.microsoft.com/en-us/windows/win32/api/notificationactivationcallback/nn-notificationactivationcallback-inotificationactivationcallback
class INotificationActivationCallback(IUnknown):
    _iid_ = GUID("{53E31837-6600-4A81-9395-75CFFE746F94}")
    _methods_ = [
        COMMETHOD(
            [],
            ctypes.HRESULT,
            "Activate",
            (["in"], wintypes.LPCWSTR, "appUserModelId"),
            (["in"], wintypes.LPCWSTR, "invokedArgs"),
            (["in"], ctypes.POINTER(NOTIFICATION_USER_INPUT_DATA), "data"),
            (["in"], wintypes.ULONG, "count"),
        )
    ]


S_OK = 0
E_FAIL = -2147467259


# This class will be instantiated when the toast is activated when the script
# is not running.
# https://learn.microsoft.com/en-us/windows/apps/design/shell/tiles-and-notifications/send-local-toast-other-apps#step-2-set-up-your-com-activator
class NotificationActivationHandler(COMObject):
    _com_interfaces_ = [INotificationActivationCallback]
    _reg_clsid_ = GUID(ACTIVATOR_UUID)
    _reg_threading_ = "Both"
    _reg_clsctx_ = CLSCTX_LOCAL_SERVER
    _regcls_ = REGCLS_MULTIPLEUSE

    def INotificationActivationCallback_Activate(
        self,
        appUserModelId: str,
        invokedArgs: str,
        data: "ctypes._Pointer[NOTIFICATION_USER_INPUT_DATA]",
        count: int,
    ):
        try:
            print(
                f"Toast activation triggered for '{appUserModelId}' with args '{invokedArgs}' and {count} user input data."
            )

            for i in range(count):
                ii: NOTIFICATION_USER_INPUT_DATA = data[i]
                print(ii.Key, f"'{ii.Value}'", sep=": ")

            return S_OK
        except BaseException as e:
            # We can't have unhandled exceptions in this callback since it returns
            # to C/C++ code and not Python.
            # REVISIT: how to handle unraisable exceptions? e.g. using sys.unraisablehook
            print(
                f"Unhandled exception in INotificationActivationCallback_Activate: {e}",
                file=sys.stderr,
            )

            return E_FAIL


def create_registry_keys():
    # Notifications toasts need some info in the registry to make it fully work.
    # https://learn.microsoft.com/en-us/windows/apps/design/shell/tiles-and-notifications/send-local-toast-other-apps#step-1-register-your-app-in-the-registry
    with winreg.CreateKey(
        winreg.HKEY_CURRENT_USER,
        f"Software\\Classes\\AppUserModelId\\{APP_USER_MODULE_ID}",
    ) as key:
        winreg.SetValueEx(key, "DisplayName", 0, winreg.REG_EXPAND_SZ, APP_NAME)
        # TODO: set IconUri
        winreg.SetValueEx(key, "CustomActivator", 0, winreg.REG_SZ, ACTIVATOR_UUID)

    with winreg.CreateKey(
        winreg.HKEY_CURRENT_USER,
        f"Software\\Classes\\CLSID\\{ACTIVATOR_UUID}\\LocalServer32",
    ) as key:
        # This is what gets run when the toast is activated after the process
        # that created it ends. It can be customized, but Windows will always
        # add "-Embedding" to the command line arguments.
        winreg.SetValueEx(
            key, None, 0, winreg.REG_SZ, f'"{sys.executable}" "{__file__}"'
        )


def remove_registry_keys():
    # Undo create_registry_keys()
    try:
        winreg.DeleteKey(
            winreg.HKEY_CURRENT_USER,
            f"Software\\Classes\\AppUserModelId\\{APP_USER_MODULE_ID}",
        )
    except FileNotFoundError:
        pass

    try:
        winreg.DeleteKey(
            winreg.HKEY_CURRENT_USER,
            f"Software\\Classes\\CLSID\\{ACTIVATOR_UUID}\\LocalServer32",
        )
    except FileNotFoundError:
        pass

    try:
        winreg.DeleteKey(
            winreg.HKEY_CURRENT_USER,
            f"Software\\Classes\\CLSID\\{ACTIVATOR_UUID}",
        )
    except FileNotFoundError:
        pass


def show_toast():
    manager = ToastNotificationManager.get_default()
    toaster = manager.create_toast_notifier_with_id(APP_USER_MODULE_ID)

    # https://learn.microsoft.com/en-us/uwp/schemas/tiles/toastschema/schema-root
    xml = XmlDocument()
    xml.load_xml(
        """
        <toast>
            <visual>
                <binding template="ToastText01">
                    <text id="1">Hello from PyWinRT Toast Activation!</text>
                </binding>
            </visual>
            <actions>
                <input id="textInput" type="text" title="Enter something" placeHolderContent="This is user input data" />
                <input id="selectInput" type="selection" title="Pick one">
                    <selection id="" content="Default" />
                    <selection id="2" content="Option 2" />
                </input>
                <action
                    activationType="foreground"
                    arguments="submit"
                    content="Submit"
                    />
            </actions>
        </toast>
        """
    )

    notification = ToastNotification(xml)

    # This is redundant since the COM activator will also handle activation.
    # But could be useful if you want to perform an action only if the script
    # is still running.
    def on_activated(toast: ToastNotification, obj: Object):
        args = obj.as_(ToastActivatedEventArgs)
        print(
            "Toast activated!",
            args.arguments,
            {k: unbox_string(v) for k, v in args.user_input.items()},
        )

    notification.add_activated(on_activated)

    toaster.show(notification)


def interactive_prompt():
    console = Console()

    while True:
        choice = Prompt.ask(
            "Enter [bold]s[/bold] to show notification or [bold]q[/bold] to quit",
            choices=["s", "q"],
        )

        match choice:
            case "s":
                console.print("Showing notification...")
                show_toast()
            case "q":
                console.print("Exiting...")
                break
            case _:
                pass


def uninstall():
    manager = ToastNotificationManager.get_default()
    notifier = manager.create_toast_notifier_with_id(APP_USER_MODULE_ID)

    # Remove any scheduled notifications that haven't been shown yet. Do
    # this first to avoid race condition with clearing history.
    for n in notifier.get_scheduled_toast_notifications():
        notifier.remove_from_schedule(n)

    # Remove any notifications that might currently exist.
    manager.history.clear_with_id(APP_USER_MODULE_ID)

    remove_registry_keys()


def main():
    # This is just one way to set the AppUserModelID for the current process.
    # https://learn.microsoft.com/en-us/windows/win32/shell/appids
    SetCurrentProcessExplicitAppUserModelID(APP_USER_MODULE_ID)

    # Since there isn't an installer for this script, we need a way to clean up
    # the registry keys if the user wants to uninstall.
    if "-Uninstall" in sys.argv:
        uninstall()
        print("Uninstalled")
        sys.exit(0)

    # Windows will add "-Embedding" to the command line when activating the toast.
    if "-Embedding" in sys.argv:
        # This blocks until INotificationActivationCallback_Activate returns.
        run_local_server([NotificationActivationHandler])

        print("Exiting in", end=" ")

        for i in range(5, 0, -1):
            print(i, end="...", flush=True)
            time.sleep(2)

        print("bye")

        sys.exit(0)

    # If you package an app with an installer, this would be taken care of by
    # the installer. But for ad-hoc scripts, we need to dynamically create some
    # registry keys so Windows knows what to do when the script isn't running.
    create_registry_keys()

    # Register our handler so that activating the notification doesn't start
    # a new process. This can be omitted if you want the activation to always
    # start a new process.
    #
    # NB: Have to keep reference to the class factory to keep it alive due to
    # COM reference counting not being implemented for the ClassFactory type.
    _ = ClassFactory(NotificationActivationHandler)

    interactive_prompt()


if __name__ == "__main__":
    main()
