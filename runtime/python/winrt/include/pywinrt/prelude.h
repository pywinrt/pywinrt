// The headers that everything else in pywinrt/ is written against: the Python C
// API, C++/WinRT, and the C++/WinRT namespace headers for the types the runtime
// API itself mentions.
//
// Including them from one place is not only convenience. Several of them are
// order-sensitive - the macros below have to be set before <unknwn.h>, and the
// precomputed GUIDs have to be seen before the namespace headers that would
// otherwise compute them - so a header that included them on its own would be
// one more place to get that order wrong.

#pragma once

// These two headers are part of the Python C Extension API
#include <Python.h>
#include <datetime.h>
#include <structmember.h>
#include <pythoncapi_compat.h>

// Keeps MSVCP140.dll out of every module that includes this header. See the
// file itself - it is deliberately included before anything that instantiates a
// standard container, though either order works.
#include <pywinrt/throw_helpers.h>

#define COM_NO_WINDOWS_H
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <unknwn.h>
#undef GetCurrentTime
#include <winrt/base.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.Storage.Streams.h>

static_assert(PY_VERSION_HEX >= 0x030B0000, "Python 3.11 or later is required");
