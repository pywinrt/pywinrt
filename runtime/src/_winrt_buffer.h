// A Python object that exports a buffer, seen from WinRT as an IBuffer.

#pragma once

#include <Python.h>

#include <pywinrt/base.h>

namespace py
{
    winrt::Windows::Storage::Streams::IBuffer convert_to_ibuffer(PyObject* obj);
} // namespace py
