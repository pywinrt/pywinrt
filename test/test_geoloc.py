from datetime import datetime
from concurrent.futures import Future, wait
import os
import unittest

import winrt.windows.devices.geolocation as wdg
import winrt.windows.foundation as wf

from ._util import async_test

ON_CI = os.environ.get("CI")


class TestGeolocation(unittest.TestCase):
    def test_pinterface_qi(self):
        locator = wdg.Geolocator()
        op = locator.get_geoposition_async()
        self.assertEqual(type(op), wf.IAsyncOperation)
        op.cancel()

    def test_struct_ctor(self):
        basic_pos = wdg.BasicGeoposition(latitude=47.1, longitude=-122.1, altitude=0.0)
        self.assertEqual(basic_pos.latitude, 47.1)
        self.assertEqual(basic_pos.longitude, -122.1)
        self.assertEqual(basic_pos.altitude, 0.0)

        geocircle = wdg.Geocircle(basic_pos, 10)
        center = geocircle.center

        self.assertEqual(10, geocircle.radius)
        for x in ["latitude", "longitude", "altitude"]:
            self.assertEqual(getattr(basic_pos, x), getattr(center, x))

    def test_iiterable_wrapping(self):
        basic_pos1 = wdg.BasicGeoposition(47.1, -122.1, 0.0)
        basic_pos2 = wdg.BasicGeoposition(47.2, -122.2, 0.0)

        box = wdg.GeoboundingBox.try_compute([basic_pos1, basic_pos2])
        nw = box.northwest_corner
        se = box.southeast_corner

        self.assertAlmostEqual(nw.latitude, basic_pos2.latitude)
        self.assertAlmostEqual(nw.longitude, basic_pos2.longitude)
        self.assertAlmostEqual(se.latitude, basic_pos1.latitude)
        self.assertAlmostEqual(se.longitude, basic_pos1.longitude)

    @unittest.skipIf(ON_CI, "Geolocation service not available on CI")
    def test_GetGeopositionAsync(self):
        """test async method using IAsyncOperation Completed callback"""
        op_future = Future()

        def callback(operation: wf.IAsyncOperation, status: wf.AsyncStatus):
            if status == wf.AsyncStatus.COMPLETED:
                op_future.set_result(operation.get_results())
            elif status == wf.AsyncStatus.CANCELED:
                op_future.cancel()
            elif status == wf.AsyncStatus.ERROR:
                op_future.set_exception(OSError(operation.error_code.value))

        locator = wdg.Geolocator()

        assert op_future.set_running_or_notify_cancel()
        op = locator.get_geoposition_async()
        op.completed = callback

        done, not_done = wait([op_future], 10)

        assert len(done) == 1
        assert not any(not_done)

        pos: wdg.Geoposition = op_future.result()

        self.assertEqual(type(pos), wdg.Geoposition)

        coord = pos.coordinate
        self.assertIsInstance(coord.timestamp, datetime)

        basic_pos = coord.point.position
        lat = basic_pos.latitude
        self.assertEqual(type(lat), float)

    @unittest.skipIf(ON_CI, "Geolocation service not available on CI")
    @async_test
    async def test_GetGeopositionAsync_await(self):
        """test async method by directly awaiting IAsyncOperation"""

        locator = wdg.Geolocator()
        pos = await locator.get_geoposition_async()
        self.assertEqual(type(pos), wdg.Geoposition)

        coord = pos.coordinate
        self.assertIsInstance(coord.timestamp, datetime)

        basic_pos = coord.point.position
        lat = basic_pos.latitude
        self.assertEqual(type(lat), float)


if __name__ == "__main__":
    unittest.main()
