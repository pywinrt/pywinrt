from enum import IntEnum

# winrt._winrt imports uuid when it loads, from compiled code, where a freezer
# that follows a program's imports cannot see it. Everything that loads the
# runtime imports this package, so saying it here puts uuid in the bundle.
import uuid

del uuid

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
