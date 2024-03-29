from datetime import datetime, timedelta
from enum import IntEnum, IntFlag
from typing import Optional
from uuid import UUID

from winrt.system import Object, Array
from winrt.windows.foundation import Point, PropertyValue, PropertyType, IPropertyValue, Rect, Size

__all__ = ["box", "unbox"]

_PROPERTY_TYPE_BOX_MAP = {
    PropertyType.BOOLEAN_ARRAY: PropertyValue.create_boolean_array,
    PropertyType.BOOLEAN: PropertyValue.create_boolean,
    PropertyType.CHAR16_ARRAY: PropertyValue.create_char16_array,
    PropertyType.CHAR16: PropertyValue.create_char16,
    PropertyType.DATE_TIME_ARRAY: PropertyValue.create_date_time_array,
    PropertyType.DATE_TIME: PropertyValue.create_date_time,
    PropertyType.DOUBLE_ARRAY: PropertyValue.create_double_array,
    PropertyType.DOUBLE: PropertyValue.create_double,
    PropertyType.EMPTY: PropertyValue.create_empty,
    PropertyType.GUID_ARRAY: PropertyValue.create_guid_array,
    PropertyType.GUID: PropertyValue.create_guid,
    PropertyType.INSPECTABLE_ARRAY: PropertyValue.create_inspectable_array,
    PropertyType.INSPECTABLE: PropertyValue.create_inspectable,
    PropertyType.INT16_ARRAY: PropertyValue.create_int16_array,
    PropertyType.INT16: PropertyValue.create_int16,
    PropertyType.INT32_ARRAY: PropertyValue.create_int32_array,
    PropertyType.INT32: PropertyValue.create_int32,
    PropertyType.INT64_ARRAY: PropertyValue.create_int64_array,
    PropertyType.INT64: PropertyValue.create_int64,
    PropertyType.POINT_ARRAY: PropertyValue.create_point_array,
    PropertyType.POINT: PropertyValue.create_point,
    PropertyType.RECT_ARRAY: PropertyValue.create_rect_array,
    PropertyType.RECT: PropertyValue.create_rect,
    PropertyType.SINGLE_ARRAY: PropertyValue.create_single_array,
    PropertyType.SINGLE: PropertyValue.create_single,
    PropertyType.SIZE_ARRAY: PropertyValue.create_size_array,
    PropertyType.SIZE: PropertyValue.create_size,
    PropertyType.STRING_ARRAY: PropertyValue.create_string_array,
    PropertyType.STRING: PropertyValue.create_string,
    PropertyType.TIME_SPAN_ARRAY: PropertyValue.create_time_span_array,
    PropertyType.TIME_SPAN: PropertyValue.create_time_span,
    PropertyType.UINT16_ARRAY: PropertyValue.create_uint16_array,
    PropertyType.UINT16: PropertyValue.create_uint16,
    PropertyType.UINT32_ARRAY: PropertyValue.create_uint32_array,
    PropertyType.UINT32: PropertyValue.create_uint32,
    PropertyType.UINT64_ARRAY: PropertyValue.create_uint64_array,
    PropertyType.UINT64: PropertyValue.create_uint64,
    PropertyType.UINT8_ARRAY: PropertyValue.create_uint8_array,
    PropertyType.UINT8: PropertyValue.create_uint8,
}

_PROPERTY_TYPE_UNBOX_MAP = {
    PropertyType.BOOLEAN_ARRAY: IPropertyValue.get_boolean_array,
    PropertyType.BOOLEAN: IPropertyValue.get_boolean,
    PropertyType.CHAR16_ARRAY: IPropertyValue.get_char16_array,
    PropertyType.CHAR16: IPropertyValue.get_char16,
    PropertyType.DATE_TIME_ARRAY: IPropertyValue.get_date_time_array,
    PropertyType.DATE_TIME: IPropertyValue.get_date_time,
    PropertyType.DOUBLE_ARRAY: IPropertyValue.get_double_array,
    PropertyType.DOUBLE: IPropertyValue.get_double,
    PropertyType.GUID_ARRAY: IPropertyValue.get_guid_array,
    PropertyType.GUID: IPropertyValue.get_guid,
    PropertyType.INSPECTABLE_ARRAY: IPropertyValue.get_inspectable_array,
    PropertyType.INT16_ARRAY: IPropertyValue.get_int16_array,
    PropertyType.INT16: IPropertyValue.get_int16,
    PropertyType.INT32_ARRAY: IPropertyValue.get_int32_array,
    PropertyType.INT32: IPropertyValue.get_int32,
    PropertyType.INT64_ARRAY: IPropertyValue.get_int64_array,
    PropertyType.INT64: IPropertyValue.get_int64,
    PropertyType.POINT_ARRAY: IPropertyValue.get_point_array,
    PropertyType.POINT: IPropertyValue.get_point,
    PropertyType.RECT_ARRAY: IPropertyValue.get_rect_array,
    PropertyType.RECT: IPropertyValue.get_rect,
    PropertyType.SINGLE_ARRAY: IPropertyValue.get_single_array,
    PropertyType.SINGLE: IPropertyValue.get_single,
    PropertyType.SIZE_ARRAY: IPropertyValue.get_size_array,
    PropertyType.SIZE: IPropertyValue.get_size,
    PropertyType.STRING_ARRAY: IPropertyValue.get_string_array,
    PropertyType.STRING: IPropertyValue.get_string,
    PropertyType.TIME_SPAN_ARRAY: IPropertyValue.get_time_span_array,
    PropertyType.TIME_SPAN: IPropertyValue.get_time_span,
    PropertyType.UINT16_ARRAY: IPropertyValue.get_uint16_array,
    PropertyType.UINT16: IPropertyValue.get_uint16,
    PropertyType.UINT32_ARRAY: IPropertyValue.get_uint32_array,
    PropertyType.UINT32: IPropertyValue.get_uint32,
    PropertyType.UINT64_ARRAY: IPropertyValue.get_uint64_array,
    PropertyType.UINT64: IPropertyValue.get_uint64,
    PropertyType.UINT8_ARRAY: IPropertyValue.get_uint8_array,
    PropertyType.UINT8: IPropertyValue.get_uint8,
}

def box(value, ptype: Optional[PropertyType] = None) -> Optional[Object]:
    """
    Boxes a Python value into a System.Object.

    This function is analogous to ``winrt::box_value`` in C++/WinRT.

    Parameters:
        value: The Python value to box.
        ptype: The property type to box the value as. If not provided, the type
            will be inferred from the value. If the value is an int, float, or
            System.Array the type must be provided.

    Returns:
        A System.Object representing the boxed value.
    """
    if value is None:
        return None

    if ptype is not None:
        try:
            func = _PROPERTY_TYPE_BOX_MAP[ptype]
        except KeyError:
            raise ValueError(f"Unsupported property type: {ptype.name}")

        return func(value)  # type: ignore[operator]

    if isinstance(value, str):
        return PropertyValue.create_string(value)

    if isinstance(value, bool):
        return PropertyValue.create_boolean(value)

    if isinstance(value, IntFlag):
        return PropertyValue.create_uint32(value)

    if isinstance(value, IntEnum):
        return PropertyValue.create_int32(value)

    if isinstance(value, UUID):
        return PropertyValue.create_guid(value)

    if isinstance(value, datetime):
        return PropertyValue.create_date_time(value)

    if isinstance(value, timedelta):
        return PropertyValue.create_time_span(value)

    if isinstance(value, Point):
        return PropertyValue.create_point(value)

    if isinstance(value, Size):
        return PropertyValue.create_size(value)

    if isinstance(value, Rect):
        return PropertyValue.create_rect(value)

    if isinstance(value, int):
        raise TypeError("Must provide a property type for int values")

    if isinstance(value, float):
        raise TypeError("Must provide a property type for float values")

    if isinstance(value, Object):
        return value

    if isinstance(value, Array):
        raise TypeError("Must provide a property type for System.Array values")

    raise TypeError(f"Unsupported type: {type(value)}")


def unbox(obj: Optional[Object]):
    """
    Unboxes a System.Object into a Python value.

    This function is analogous to ``winrt::unbox_value`` in C++/WinRT.

    Parameters:
        obj: The System.Object to unbox.

    Returns:
        The unboxed Python value.
    """
    if obj is None:
        return None

    if not isinstance(obj, Object):
        raise TypeError("Must provide a boxed value")

    try:
        pvalue = IPropertyValue._from(obj)
    except OSError:
        return obj

    try:
        func = _PROPERTY_TYPE_UNBOX_MAP[pvalue.type]
    except KeyError:
        raise ValueError(f"Unsupported property type: {pvalue.type.name}")

    return func(pvalue)
