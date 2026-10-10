// keeps <windows.h> from defining min and max as macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

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
        /// How many of the bytes hold data: none at first for WinRT to fill,
        /// all of them for it to read, and what a writer sets after it has
        /// written, which may be on another thread.
        std::atomic<uint32_t> length;
        /// The object promises never to change, as bytes does.
        bool read_only;

      public:
        /**
         * @param fill WinRT writes into the buffer rather than only reading
         * it, so @p obj has to be writable, as a new Buffer of that capacity
         * would be.
         */
        PyWinRTBuffer(PyObject* obj, bool fill)
            : buffer{obj, fill ? PyBUF_WRITABLE : PyBUF_SIMPLE}
        {
            if (!buffer)
            {
                throw python_exception();
            }

            read_only = buffer.view().readonly != 0;
            length = fill ? 0 : Capacity();
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
            // A length is set by what writes into the buffer, and a parameter
            // WinRT fills takes only a writable one, so here a writer that no
            // rule names has already written into an object that promises
            // never to change, which is said rather than let pass.
            if (read_only)
            {
                throw winrt::hresult_access_denied{
                    L"the length of a read-only Python buffer, such as bytes, cannot "
                    L"be set; WinRT writes only into a writable one, such as a "
                    L"bytearray"};
            }

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

winrt::Windows::Storage::Streams::IBuffer py::convert_to_ibuffer(
    PyObject* obj, bool fill)
{
    return winrt::make<py::cpp::_winrt::PyWinRTBuffer>(obj, fill);
}
