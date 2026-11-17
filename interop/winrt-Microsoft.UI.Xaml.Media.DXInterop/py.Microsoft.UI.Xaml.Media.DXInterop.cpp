#include "interop.h"

#include <dxgi.h>

// The interfaces of WinUI 3's microsoft.ui.xaml.media.dxinterop.h that the
// module calls, declared here so that it compiles against the Windows SDK
// alone. The Windows App SDK's license does not let its headers be
// redistributed in the source distribution.
// https://learn.microsoft.com/en-us/windows/windows-app-sdk/api/win32/microsoft.ui.xaml.media.dxinterop/

MIDL_INTERFACE("63aad0b8-7c24-40ff-85a8-640d944cc325")
ISwapChainPanelNative : ::IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE SetSwapChain(
        _In_ IDXGISwapChain * swapChain) noexcept = 0;
};

MIDL_INTERFACE("88fd8248-10da-4810-bb4c-010dd27faea9")
ISwapChainPanelNative2 : ISwapChainPanelNative
{
    virtual HRESULT STDMETHODCALLTYPE SetSwapChainHandle(
        _In_ HANDLE swapChainHandle) noexcept = 0;
};

#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(
    ISwapChainPanelNative,
    // clang-format off
    0x63aad0b8, 0x7c24, 0x40ff, 0x85, 0xa8, 0x64, 0x0d, 0x94, 0x4c, 0xc3, 0x25
    // clang-format on
);
__CRT_UUID_DECL(
    ISwapChainPanelNative2,
    // clang-format off
    0x88fd8248, 0x10da, 0x4810, 0xbb, 0x4c, 0x01, 0x0d, 0xd2, 0x7f, 0xae, 0xa9
    // clang-format on
);
#endif

namespace
{
    PyObject* set_swap_chain(PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* capsule;
        PyObject* swap_chain_capsule;

        if (!PyArg_ParseTuple(args, "OO", &capsule, &swap_chain_capsule))
        {
            return nullptr;
        }

        interop::com_ptr<IDXGISwapChain> swap_chain;

        // None takes the swap chain the panel shows away from it.
        if (!Py_IsNone(swap_chain_capsule)
            && !interop::query_capsule(swap_chain_capsule, swap_chain))
        {
            return nullptr;
        }

        interop::com_ptr<ISwapChainPanelNative> native;
        if (!interop::query_capsule(capsule, native))
        {
            return nullptr;
        }

        HRESULT hr{};

        {
            // A call from a thread other than the panel's may wait for that
            // thread, which may itself be waiting for the GIL.
            interop::gil_released const guard;
            hr = native->SetSwapChain(swap_chain.get());
        }

        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        Py_RETURN_NONE;
    }

    PyObject* set_swap_chain_handle(PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* capsule;
        PyObject* handle_obj;

        if (!PyArg_ParseTuple(args, "OO", &capsule, &handle_obj))
        {
            return nullptr;
        }

        auto const handle = PyLong_AsVoidPtr(handle_obj);
        if (!handle && PyErr_Occurred())
        {
            return nullptr;
        }

        interop::com_ptr<ISwapChainPanelNative2> native2;
        if (!interop::query_capsule(capsule, native2))
        {
            return nullptr;
        }

        HRESULT hr{};

        {
            // as for set_swap_chain()
            interop::gil_released const guard;
            hr = native2->SetSwapChainHandle(static_cast<HANDLE>(handle));
        }

        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        Py_RETURN_NONE;
    }

    /// The identity of the COM object @p capsule holds, which is the address of
    /// its IUnknown, for as long as something holds a reference to it.
    PyObject* get_identity(PyObject* /*unused*/, PyObject* capsule) noexcept
    {
        interop::com_ptr<IUnknown> identity;
        if (!interop::query_capsule(capsule, identity))
        {
            return nullptr;
        }

        return PyLong_FromVoidPtr(identity.get());
    }

    PyMethodDef module_methods[]{
        {"set_swap_chain", set_swap_chain, METH_VARARGS, nullptr},
        {"set_swap_chain_handle", set_swap_chain_handle, METH_VARARGS, nullptr},
        {"get_identity", get_identity, METH_O, nullptr},
        {}};

    PyDoc_STRVAR(
        module_doc,
        "APIs for DirectX interop with the Microsoft.UI.Xaml.Media namespace.");

    PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_microsoft_ui_xaml_media_dxinterop",
           module_doc,
           0,
           module_methods,
           interop::module_slots,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_microsoft_ui_xaml_media_dxinterop(void) noexcept
{
    return PyModuleDef_Init(&module_def);
}
