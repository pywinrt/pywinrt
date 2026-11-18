"""
Windows.Foundation.DateTime converted over the whole range of a datetime.
"""

import unittest
from datetime import datetime, timedelta, timezone, UTC

import winrt.windows.storage.streams as wss

# A DateTime counts 100 ns ticks from the start of 1601.
EPOCH = datetime(1601, 1, 1, tzinfo=UTC)


def expected_ticks(value: datetime) -> int:
    """
    The tick count of @p value, from Python's own datetime arithmetic.
    """
    return (value - EPOCH) // timedelta(microseconds=1) * 10


def ticks_of(value: datetime) -> int:
    """
    The tick count @p value converts to.
    """
    writer = wss.DataWriter()
    writer.write_date_time(value)
    return wss.DataReader.from_buffer(writer.detach_buffer()).read_int64()


def date_time_of(ticks: int) -> datetime:
    """
    The datetime a DateTime of @p ticks converts to.
    """
    writer = wss.DataWriter()
    writer.write_int64(ticks)
    return wss.DataReader.from_buffer(writer.detach_buffer()).read_date_time()


class TestDateTime(unittest.TestCase):
    def test_every_year_converts_as_datetime_counts(self) -> None:
        # Windows converts from 1601 on and the runtime before that, so the
        # years around 1601 and the century leap-year rules are all here.
        years = [1, 4, 100, 400, 1599, 1600, 1601, 1700, 1900, 2000, 9996, 9999]
        years += range(1, 10000, 97)

        for year in years:
            values = [
                datetime(year, 1, 1, tzinfo=UTC),
                datetime(year, 2, 28, 23, 59, 59, 999999, tzinfo=UTC),
                datetime(year, 3, 1, 12, 34, 56, 789012, tzinfo=UTC),
                datetime(year, 12, 31, 23, 59, 59, 999999, tzinfo=UTC),
            ]

            if year % 4 == 0 and (year % 100 != 0 or year % 400 == 0):
                values.append(datetime(year, 2, 29, 6, tzinfo=UTC))

            for value in values:
                with self.subTest(value=value):
                    self.assertEqual(ticks_of(value), expected_ticks(value))
                    self.assertEqual(date_time_of(expected_ticks(value)), value)

    def test_a_time_zone_is_converted_to_utc_before_1601(self) -> None:
        value = datetime(1, 1, 1, 12, tzinfo=timezone(timedelta(hours=-5)))
        self.assertEqual(date_time_of(ticks_of(value)), value)
        self.assertEqual(ticks_of(value), expected_ticks(value))

    def test_ticks_round_down_to_the_microsecond(self) -> None:
        for ticks, value in (
            (9, EPOCH),
            (-1, EPOCH - timedelta(microseconds=1)),
            (-10, EPOCH - timedelta(microseconds=1)),
            (-11, EPOCH - timedelta(microseconds=2)),
        ):
            with self.subTest(ticks=ticks):
                self.assertEqual(date_time_of(ticks), value)

    def test_a_year_a_datetime_cannot_hold(self) -> None:
        first = expected_ticks(datetime.min.replace(tzinfo=UTC))
        last = expected_ticks(datetime.max.replace(tzinfo=UTC)) + 9

        for ticks, year in (
            (first - 1, "0"),
            (last + 1, "10000"),
            (-(2**63), "-27627"),
            (2**63 - 1, "30828"),
        ):
            with (
                self.subTest(ticks=ticks),
                self.assertRaisesRegex(
                    OverflowError,
                    f"^a DateTime in the year {year} is out of range for a "
                    "datetime, which holds the years 1 to 9999$",
                ),
            ):
                date_time_of(ticks)


if __name__ == "__main__":
    unittest.main()
