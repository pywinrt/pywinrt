from enum import IntEnum

from winrt._winrt import init_apartment, uninit_apartment, STA, MTA


__all__ = ["ApartmentType", "init_apartment", "uninit_apartment"]


class ApartmentType(IntEnum):
    """
    Enumeration of COM apartment types.
    """

    SINGLE_THREADED = STA
    """
    Single-threaded apartment.
    """
    MULTI_THREADED = MTA
    """
    Multi-threaded apartment.
    """
