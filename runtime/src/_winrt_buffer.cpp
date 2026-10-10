#include <Python.h>
#include <Robuffer.h>
#include <pywinrt/base.h>
#include "_winrt_buffer.h"
#include <winrt/base.h>

#include <atomic>

namespace py::cpp::_winrt
{
    struct PyWinRTBuffer : winrt::implements<
                               PyWinRTBuffer,
                               winrt::Windows::Storage::Streams::IBuffer,
                               Windows::Storage::Streams::IBufferByteAccess>
    {
      private:
        py::buffer_view buffer;
        /// How many of the bytes hold data: all of them at first, for a
        /// reader, and what a writer sets after it has written, which may be
        /// on another thread.
        std::atomic<uint32_t> length;

      public:
        PyWinRTBuffer(PyObject* obj) : buffer{obj, PyBUF_SIMPLE}
        {
            if (!buffer)
            {
                throw python_exception();
            }

            length = Capacity();
        }

        static void final_release(std::unique_ptr<PyWinRTBuffer> self) noexcept
        {
            // WinRT lets go of the buffer on whatever thread it happens to be
            // on, and letting go of the Python buffer can run a finalizer.
            auto gil = ensure_gil();
            if (!gil)
            {
                // Releasing the view would call into an interpreter that is
                // gone, so it is left as it is.
                self.release();
                return;
            }

            self.reset();
        }

        uint32_t Capacity() const
        {
            return static_cast<uint32_t>(buffer.size());
        }

        uint32_t Length() const
        {
            return length;
        }

        void Length(uint32_t value)
        {
            if (value > Capacity())
            {
                throw winrt::hresult_invalid_argument{};
            }

            length = value;
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
