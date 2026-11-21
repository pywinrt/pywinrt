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
    /**
     * How a Python buffer given to WinRT as an IBuffer is told apart from any
     * other IBuffer when it comes back, which nothing outside the runtime
     * implements.
     */
    struct
#if WINRT_IMPL_HAS_DECLSPEC_UUID
        __declspec(uuid("2c818abf-c31d-43a5-bfde-27da32bd393f"))
#endif
        IPywinrtBuffer : ::IUnknown
    {
        virtual int32_t __stdcall IsReadOnly(bool&) = 0;
    };
} // namespace py::cpp::_winrt

#if !WINRT_IMPL_HAS_DECLSPEC_UUID
__CRT_UUID_DECL(
    py::cpp::_winrt::IPywinrtBuffer,
    // clang-format off
    0x2c818abf, 0xc31d, 0x43a5, 0xbf, 0xde, 0x27, 0xda, 0x32, 0xbd, 0x39, 0x3f
);
#endif

namespace py::cpp::_winrt
{
    /**
     * A Python buffer as an IBuffer.
     *
     * It marshals by value, as Windows.Storage.Streams.Buffer does, rather than
     * with the free-threaded marshaler that C++/WinRT gives an object by
     * default. A buffer passed to a service in another process, as a GATT write
     * passes its value to the Bluetooth service, then arrives there as a copy
     * of its bytes. Marshaled by reference, it arrives as a proxy, the service
     * never reads through it, and the write fails with ACCESS_DENIED.
     *
     * COM calls the IMarshal methods on whatever thread marshals the buffer,
     * and the buffer marshaler reads Length, Capacity and Buffer() there, in
     * MarshalInterface, to copy the bytes. That thread need not hold the GIL,
     * and the thread that does may be waiting for the call to return, so
     * nothing here but final_release() takes the GIL or touches a Python
     * object: the view is taken once, with the GIL, when the buffer is made,
     * and holds the memory still until it is released, so its pointer and size
     * are plain values that any thread may read, and the length is atomic. The
     * view is released only when the last reference goes, on whatever thread
     * that is, and final_release() takes the GIL for it as the runtime's other
     * objects do.
     */
    struct PyWinRTBuffer : winrt::implements<
                               PyWinRTBuffer,
                               winrt::Windows::Storage::Streams::IBuffer,
                               Windows::Storage::Streams::IBufferByteAccess,
                               IPywinrtBuffer,
                               ::IMarshal>
    {
      private:
        /**
         * Calls @p f with the marshaler that Windows provides for IBuffer
         * implementations (RoGetBufferMarshaler), the one that
         * Windows.Storage.Streams.Buffer and .NET's WindowsRuntimeBuffer use.
         *
         * It is asked for on every call rather than kept, so there is nothing
         * for two threads to fill in at once, and it takes no GIL.
         */
        template <typename F>
        static HRESULT with_buffer_marshaler(F&& f) noexcept
        {
            winrt::com_ptr<::IMarshal> marshaler;
            if (HRESULT const hr = ::RoGetBufferMarshaler(marshaler.put()); FAILED(hr))
            {
                return hr;
            }

            return f(marshaler.get());
        }

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
                    L"be set, since WinRT writes only into a writable one, such as a "
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

        int32_t __stdcall IsReadOnly(bool& value) noexcept final
        {
            value = read_only;
            return S_OK;
        }

        HRESULT __stdcall GetUnmarshalClass(
            REFIID riid,
            void* pv,
            DWORD dest_context,
            void* reserved,
            DWORD flags,
            CLSID* clsid) noexcept final
        {
            return with_buffer_marshaler([&](::IMarshal* marshaler) {
                return marshaler->GetUnmarshalClass(
                    riid, pv, dest_context, reserved, flags, clsid);
            });
        }

        HRESULT __stdcall GetMarshalSizeMax(
            REFIID riid,
            void* pv,
            DWORD dest_context,
            void* reserved,
            DWORD flags,
            DWORD* size) noexcept final
        {
            return with_buffer_marshaler([&](::IMarshal* marshaler) {
                return marshaler->GetMarshalSizeMax(
                    riid, pv, dest_context, reserved, flags, size);
            });
        }

        HRESULT __stdcall MarshalInterface(
            IStream* stream,
            REFIID riid,
            void* pv,
            DWORD dest_context,
            void* reserved,
            DWORD flags) noexcept final
        {
            return with_buffer_marshaler([&](::IMarshal* marshaler) {
                return marshaler->MarshalInterface(
                    stream, riid, pv, dest_context, reserved, flags);
            });
        }

        HRESULT __stdcall UnmarshalInterface(
            IStream* stream, REFIID riid, void** object) noexcept final
        {
            return with_buffer_marshaler([&](::IMarshal* marshaler) {
                return marshaler->UnmarshalInterface(stream, riid, object);
            });
        }

        HRESULT __stdcall ReleaseMarshalData(IStream* stream) noexcept final
        {
            return with_buffer_marshaler([&](::IMarshal* marshaler) {
                return marshaler->ReleaseMarshalData(stream);
            });
        }

        HRESULT __stdcall DisconnectObject(DWORD reserved) noexcept final
        {
            return with_buffer_marshaler([&](::IMarshal* marshaler) {
                return marshaler->DisconnectObject(reserved);
            });
        }
    };

} // namespace py::cpp::_winrt

winrt::Windows::Storage::Streams::IBuffer py::convert_to_ibuffer(
    PyObject* obj, bool fill)
{
    return winrt::make<py::cpp::_winrt::PyWinRTBuffer>(obj, fill);
}

/**
 * Whether @p object is a Python buffer that promises never to change, such as
 * bytes, so that a view of it is read-only too and WinRT may not write into it.
 *
 * It may be a proxy, so this is a call into its apartment.
 */
bool py::is_read_only_python_buffer(
    winrt::Windows::Foundation::IUnknown const& object)
{
    auto const python_buffer = object.try_as<py::cpp::_winrt::IPywinrtBuffer>();
    if (!python_buffer)
    {
        return false;
    }

    bool read_only{};
    winrt::check_hresult(python_buffer->IsReadOnly(read_only));
    return read_only;
}
