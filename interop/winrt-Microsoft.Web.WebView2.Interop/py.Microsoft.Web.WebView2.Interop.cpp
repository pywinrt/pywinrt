#include "interop.h"

#include <ole2.h>

#include <WebView2Interop.h>

#include <atomic>
#include <utility>

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
        drop_target(
            interop::com_ptr<ICoreWebView2CompositionControllerInterop2> controller,
            HWND hwnd) noexcept
            : controller{std::move(controller)}, hwnd{hwnd}
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
        /// OLE gives the point in screen coordinates.
        POINT to_client(POINTL point) const noexcept
        {
            POINT client{point.x, point.y};
            ScreenToClient(hwnd, &client);

            return client;
        }

        std::atomic<ULONG> references{1};
        interop::com_ptr<ICoreWebView2CompositionControllerInterop2> const controller;
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

    /// The window property that marks a window whose drop target this module
    /// registered, and so whose OLE initialization it owns. It goes away with
    /// the window, so a window that reuses the handle does not have it.
    constexpr wchar_t registered_property[]
        = L"winrt.microsoft.web.webview2.interop.drop_target";

    /// Owns one initialization of OLE on the calling thread, or none, and
    /// ends it when it goes out of scope. It is destroyed on the thread that
    /// made it, with the GIL held. OLE is initialized and uninitialized with
    /// the GIL released: the thread's first initialization sets OLE up, and
    /// its last one can flush the OLE clipboard, which waits for other
    /// processes.
    class ole_initialization
    {
      public:
        ole_initialization(ole_initialization const&) = delete;
        ole_initialization& operator=(ole_initialization const&) = delete;

        ~ole_initialization() noexcept
        {
            if (owned)
            {
                interop::gil_released const guard;
                OleUninitialize();
            }
        }

        /// Initializes OLE on the calling thread.
        /// @returns one that owns nothing when that fails, with the error in
        /// @p hr.
        static ole_initialization initialize(HRESULT& hr) noexcept
        {
            {
                interop::gil_released const guard;
                hr = OleInitialize(nullptr);
            }

            return ole_initialization{SUCCEEDED(hr)};
        }

        /// Removes the mark of @p hwnd and takes the initialization that it
        /// held.
        /// @returns one that owns nothing when @p hwnd has no mark.
        static ole_initialization take(HWND hwnd) noexcept
        {
            return ole_initialization{
                RemovePropW(hwnd, registered_property) != nullptr};
        }

        explicit operator bool() const noexcept
        {
            return owned;
        }

        /// Gives up the initialization, which something else then owns.
        void detach() noexcept
        {
            owned = false;
        }

      private:
        explicit ole_initialization(bool owned) noexcept : owned{owned}
        {
        }

        bool owned;
    };

    /// Owns the registration of a window's drop target, or none, and revokes
    /// it when it goes out of scope. It is destroyed on the thread that owns
    /// the window, with the GIL held, which it releases around OLE.
    class drag_drop_registration
    {
      public:
        drag_drop_registration(drag_drop_registration const&) = delete;
        drag_drop_registration& operator=(drag_drop_registration const&) = delete;

        ~drag_drop_registration() noexcept
        {
            if (hwnd)
            {
                // OLE's reference may be the last one to the drop target,
                // which holds the controller.
                interop::gil_released const guard;
                RevokeDragDrop(hwnd);
            }
        }

        /// Registers @p target as the drop target of @p hwnd. OLE holds a
        /// reference of its own to it until it is revoked.
        /// @returns one that owns nothing when that fails, with the error in
        /// @p hr.
        static drag_drop_registration make(
            HWND hwnd, IDropTarget* target, HRESULT& hr) noexcept
        {
            {
                interop::gil_released const guard;
                hr = RegisterDragDrop(hwnd, target);
            }

            return drag_drop_registration{SUCCEEDED(hr) ? hwnd : nullptr};
        }

        explicit operator bool() const noexcept
        {
            return hwnd != nullptr;
        }

        /// Gives up the registration, which something else then owns.
        void detach() noexcept
        {
            hwnd = nullptr;
        }

      private:
        explicit drag_drop_registration(HWND hwnd) noexcept : hwnd{hwnd}
        {
        }

        HWND hwnd;
    };

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

        interop::com_ptr<ICoreWebView2CompositionControllerInterop2> controller;
        if (!interop::query_capsule(controller_capsule, controller))
        {
            return nullptr;
        }

        // Until it is registered, the drop target is a plain COM object that
        // needs no OLE.
        auto const target
            = interop::make_com_ptr<drop_target>(std::move(controller), hwnd);
        if (!target)
        {
            return PyErr_NoMemory();
        }

        // RegisterDragDrop needs OLE, not only COM.
        HRESULT hr;
        auto ole = ole_initialization::initialize(hr);
        if (!ole)
        {
            return interop::set_hresult_error(hr);
        }

        auto registration = drag_drop_registration::make(hwnd, target.get(), hr);
        if (!registration)
        {
            return interop::set_hresult_error(hr);
        }

        // Any handle but null marks the window, and its own is one.
        if (!SetPropW(hwnd, registered_property, hwnd))
        {
            return interop::set_hresult_error(
                HRESULT_FROM_WIN32(GetLastError()), nullptr);
        }

        // The window keeps its registration, and the mark keeps the OLE
        // initialization until revoke_drop_target() takes it.
        registration.detach();
        ole.detach();

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

        // A drop target this module did not register, and the OLE
        // initialization that goes with it, belong to someone else.
        if (!GetPropW(hwnd, registered_property))
        {
            return interop::set_hresult_error(DRAGDROP_E_NOTREGISTERED, nullptr);
        }

        HRESULT hr;
        {
            // OLE's reference may be the last one to the drop target, which
            // holds the controller.
            interop::gil_released const guard;
            hr = RevokeDragDrop(hwnd);
        }

        if (hr == DRAGDROP_E_NOTREGISTERED)
        {
            // Other code revoked the drop target, so nothing of this module
            // uses the OLE initialization any more. It ends once the error is
            // raised.
            auto const error = interop::set_hresult_error(hr);
            auto const ole = ole_initialization::take(hwnd);
            return error;
        }

        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        // The OLE initialization ends when this returns.
        auto const ole = ole_initialization::take(hwnd);

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
