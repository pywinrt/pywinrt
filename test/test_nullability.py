"""
Members that nullability/windows-sdk.json marks as returning or accepting null,
checked where a test machine can produce the null case.
"""

import pathlib
import tempfile
import unittest

import winrt.windows.devices.enumeration as wde
import winrt.windows.storage as ws
import winrt.windows.storage.search as wss
import winrt.windows.system.diagnostics as wsd

from ._util import async_test

# An interface class that no device implements.
NO_DEVICES = (
    'System.Devices.InterfaceClassGuid:="{00000000-0000-0000-0000-000000000000}"'
)


def windows_path(path: str) -> str:
    """
    @p path with backslashes, which StorageFolder insists on. MSYS2's Python
    separates paths with forward slashes.
    """
    return path.replace("/", "\\")


class TestReturnsNone(unittest.TestCase):
    @async_test
    async def test_storage_folder_get_parent_async_of_drive_root(self):
        # The second await also checks that an async operation releases its
        # cross-apartment proxy without the GIL: the first operation is let go
        # of after the second has set its completed handler, and the thread
        # that runs that handler is the one the release has to call into.
        root = pathlib.Path(tempfile.gettempdir()).anchor
        folder = await ws.StorageFolder.get_folder_from_path_async(windows_path(root))
        self.assertIsNone(await folder.get_parent_async())

    @async_test
    async def test_storage_folder_try_get_item_async_not_found(self):
        with tempfile.TemporaryDirectory() as path:
            folder = await ws.StorageFolder.get_folder_from_path_async(
                windows_path(path)
            )
            self.assertIsNone(await folder.try_get_item_async("missing"))

    def test_process_diagnostic_info_try_get_for_process_id_not_found(self):
        # Process ids are multiples of 4, so this one cannot exist.
        self.assertIsNone(wsd.ProcessDiagnosticInfo.try_get_for_process_id(0xFFFFFFFD))


class TestAcceptsNone(unittest.TestCase):
    def test_device_information_find_all_async_additional_properties(self):
        devices = wde.DeviceInformation.find_all_async(NO_DEVICES, None).get()
        self.assertEqual(devices.size, 0)

    def test_query_options_file_type_filter(self):
        options = wss.QueryOptions(wss.CommonFileQuery.DEFAULT_QUERY, None)
        self.assertEqual(list(options.file_type_filter), [])

    @async_test
    async def test_retrieve_properties_async_properties_to_retrieve(self):
        with tempfile.TemporaryDirectory() as path:
            folder = await ws.StorageFolder.get_folder_from_path_async(
                windows_path(path)
            )
            properties = await folder.properties.retrieve_properties_async(None)
            self.assertGreater(len(properties), 0)


if __name__ == "__main__":
    unittest.main()
