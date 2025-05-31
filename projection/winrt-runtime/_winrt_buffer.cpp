#define PYWINRT_RUNTIME_MODULE
#include <Python.h>
#include <Robuffer.h>
#include "pybase.h"
#include <winrt/base.h>

namespace py::cpp::_winrt
{
    struct PyWinRTBuffer : winrt::implements<
                               PyWinRTBuffer,
                               winrt::Windows::Storage::Streams::IBuffer,
                               Windows::Storage::Streams::IBufferByteAccess>
    {
      private:
        Py_buffer buffer;

      public:
        PyWinRTBuffer(PyObject* obj)
        {
            if (PyObject_GetBuffer(obj, &buffer, PyBUF_SIMPLE) == -1)
            {
                throw python_exception();
            };
        }

        ~PyWinRTBuffer()
        {
            PyBuffer_Release(&buffer);
        }

        uint32_t Capacity() const
        {
            return static_cast<uint32_t>(buffer.len);
        }

        uint32_t Length() const
        {
            return static_cast<uint32_t>(buffer.len);
        }

        void Length(uint32_t /*unused*/)
        {
            // can't resize Python buffers.
            throw winrt::hresult_not_implemented{};
        }

        HRESULT __stdcall Buffer(uint8_t** value)
        {
            *value = reinterpret_cast<uint8_t*>(buffer.buf);
            return S_OK;
        }
    };

} // namespace py::cpp::_winrt

winrt::Windows::Storage::Streams::IBuffer py::convert_to_ibuffer(PyObject* obj)
{
    return winrt::make<py::cpp::_winrt::PyWinRTBuffer>(obj);
}
