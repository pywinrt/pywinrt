#include "interop.h"

#include <ole2.h>

#include <WebView2Interop.h>

#include <atomic>
#include <new>

// The WebView2 headers are made by MIDL, whose MIDL_INTERFACE names the IID
// with __declspec(uuid), which GCC does not have.
#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(
    ICoreWebView2CompositionControllerInterop2,
    // clang-format off
    0x6b47bbe1, 0x2480, 0x4ff8, 0xa5, 0xba, 0x69, 0xc2, 0xf0, 0xb8, 0x68, 0xb3
    // clang-format on
)
#endif

// https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/winrt/interop/

namespace
{
    /// An OLE drop target for a window that shows a visually hosted WebView2.
    /// It hands each call to the WebView's composition controller, with the
    /// point in the window's client coordinates, which is where the WebView
    /// is drawn from.
    ///
    /// OLE calls it on the thread that registered it, from that thread's
    /// message loop, and it calls nothing but the controller, so it needs no
    /// GIL.
    class drop_target final : public IDropTarget
    {
      public:
        /// The reference to @p controller moves in.
        drop_target(
            ICoreWebView2CompositionControllerInterop2* controller, HWND hwnd) noexcept
            : controller{controller}, hwnd{hwnd}
        {
        }

        drop_target(drop_target const&) = delete;
        drop_target& operator=(drop_target const&) = delete;

        HRESULT STDMETHODCALLTYPE
        QueryInterface(REFIID iid, void** result) noexcept override
        {
            if (!result)
            {
                return E_POINTER;
            }

            if (iid != __uuidof(IUnknown) && iid != __uuidof(IDropTarget))
            {
                *result = nullptr;
                return E_NOINTERFACE;
            }

            AddRef();
            *result = static_cast<IDropTarget*>(this);

            return S_OK;
        }

        ULONG STDMETHODCALLTYPE AddRef() noexcept override
        {
            return ++references;
        }

        ULONG STDMETHODCALLTYPE Release() noexcept override
        {
            auto const remaining = --references;
            if (remaining == 0)
            {
                delete this;
            }

            return remaining;
        }

        HRESULT STDMETHODCALLTYPE DragEnter(
            IDataObject* data_object,
            DWORD key_state,
            POINTL point,
            DWORD* effect) noexcept override
        {
            return controller->DragEnter(
                data_object, key_state, to_client(point), effect);
        }

        HRESULT STDMETHODCALLTYPE
        DragOver(DWORD key_state, POINTL point, DWORD* effect) noexcept override
        {
            return controller->DragOver(key_state, to_client(point), effect);
        }

        HRESULT STDMETHODCALLTYPE DragLeave() noexcept override
        {
            return controller->DragLeave();
        }

        HRESULT STDMETHODCALLTYPE Drop(
            IDataObject* data_object,
            DWORD key_state,
            POINTL point,
            DWORD* effect) noexcept override
        {
            return controller->Drop(data_object, key_state, to_client(point), effect);
        }

      private:
        ~drop_target()
        {
            controller->Release();
        }

        /// OLE gives the point in screen coordinates.
        POINT to_client(POINTL point) const noexcept
        {
            POINT client{point.x, point.y};
            ScreenToClient(hwnd, &client);

            return client;
        }

        std::atomic<ULONG> references{1};
        ICoreWebView2CompositionControllerInterop2* const controller;
        HWND const hwnd;
    };

    /// Parses a window handle.
    /// @returns false with a Python exception set when @p obj is not one.
    bool parse_hwnd(PyObject* obj, HWND* hwnd) noexcept
    {
        auto const handle = PyLong_AsVoidPtr(obj);
        if (!handle && PyErr_Occurred())
        {
            return false;
        }

        *hwnd = static_cast<HWND>(handle);

        return true;
    }

    /// OLE registers and revokes a window's drop target on the thread that
    /// owns the window, and calls it there.
    /// @returns false with a Python exception set when @p hwnd is not a
    /// window of the calling thread.
    bool check_window_thread(HWND hwnd) noexcept
    {
        if (GetWindowThreadProcessId(hwnd, nullptr) != GetCurrentThreadId())
        {
            PyErr_SetString(PyExc_ValueError, "hwnd is not a window of this thread");
            return false;
        }

        return true;
    }

    PyObject* register_drop_target(PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* controller_capsule;
        PyObject* hwnd_obj;

        if (!PyArg_ParseTuple(args, "OO", &controller_capsule, &hwnd_obj))
        {
            return nullptr;
        }

        HWND hwnd;
        if (!parse_hwnd(hwnd_obj, &hwnd))
        {
            return nullptr;
        }

        if (!check_window_thread(hwnd))
        {
            return nullptr;
        }

        ICoreWebView2CompositionControllerInterop2* controller{};
        if (!interop::query_capsule(controller_capsule, &controller))
        {
            return nullptr;
        }

        // RegisterDragDrop needs OLE, not only COM, and revoke_drop_target()
        // balances this.
        auto hr = OleInitialize(nullptr);
        if (FAILED(hr))
        {
            controller->Release();
            return interop::set_hresult_error(hr);
        }

        auto const target = new (std::nothrow) drop_target{controller, hwnd};
        if (!target)
        {
            controller->Release();
            OleUninitialize();
            return PyErr_NoMemory();
        }

        // On success, OLE holds a reference of its own until the window's
        // drop target is revoked.
        hr = RegisterDragDrop(hwnd, target);
        target->Release();
        if (FAILED(hr))
        {
            auto const error = interop::set_hresult_error(hr);
            OleUninitialize();
            return error;
        }

        Py_RETURN_NONE;
    }

    PyObject* revoke_drop_target(PyObject* /*unused*/, PyObject* hwnd_obj) noexcept
    {
        HWND hwnd;
        if (!parse_hwnd(hwnd_obj, &hwnd))
        {
            return nullptr;
        }

        if (!check_window_thread(hwnd))
        {
            return nullptr;
        }

        HRESULT hr;
        {
            // OLE's reference may be the last one to the drop target, which
            // holds the controller.
            interop::gil_released const guard;
            hr = RevokeDragDrop(hwnd);
        }

        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        OleUninitialize();

        Py_RETURN_NONE;
    }

    PyMethodDef module_methods[]{
        {"register_drop_target", register_drop_target, METH_VARARGS, nullptr},
        {"revoke_drop_target", revoke_drop_target, METH_O, nullptr},
        {}};

    PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_microsoft_web_webview2_interop",
           nullptr,
           0,
           module_methods,
           interop::module_slots,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_microsoft_web_webview2_interop(void) noexcept
{
    return PyModuleDef_Init(&module_def);
}
