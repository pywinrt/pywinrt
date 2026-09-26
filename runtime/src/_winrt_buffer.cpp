#include <Python.h>
#include <Robuffer.h>
#include <pywinrt/base.h>
#include "_winrt_buffer.h"
#include <winrt/base.h>

namespace py::cpp::_winrt
{
    struct PyWinRTBuffer : winrt::implements<
                               PyWinRTBuffer,
                               winrt::Windows::Storage::Streams::IBuffer,
                               Windows::Storage::Streams::IBufferByteAccess>
    {
      private:
        py::buffer_view buffer;

      public:
        PyWinRTBuffer(PyObject* obj) : buffer{obj, PyBUF_SIMPLE}
        {
            if (!buffer)
            {
                throw python_exception();
            }
        }

        uint32_t Capacity() const
        {
            return static_cast<uint32_t>(buffer.size());
        }

        uint32_t Length() const
        {
            return static_cast<uint32_t>(buffer.size());
        }

        void Length(uint32_t /*unused*/)
        {
            // can't resize Python buffers.
            throw winrt::hresult_not_implemented{};
        }

        HRESULT __stdcall Buffer(uint8_t** value)
        {
            *value = reinterpret_cast<uint8_t*>(buffer.data());
            return S_OK;
        }
    };

} // namespace py::cpp::_winrt

winrt::Windows::Storage::Streams::IBuffer py::convert_to_ibuffer(PyObject* obj)
{
    return winrt::make<py::cpp::_winrt::PyWinRTBuffer>(obj);
}
