#include "interop.h"

#include <windows.ui.composition.interop.h>

// MINGW header is missing some interfaces
#ifdef __MINGW32__
#ifndef ____x_ABI_CWindows_CUI_CComposition_CDesktop_CICompositorDesktopInterop_INTERFACE_DEFINED__
#include <unknwn.h>

DEFINE_GUID(
    IID___x_ABI_CWindows_CUI_CComposition_CDesktop_CICompositorDesktopInterop,
    // clang-format off
    0x29E691FA, 0x4567, 0x4DCA, 0xB3, 0x19, 0xD0, 0xF2, 0x07, 0xEB, 0x68, 0x07
    // clang-format on
);

namespace ABI::Windows::UI::Composition::Desktop
{
    typedef interface IDesktopWindowTarget IDesktopWindowTarget;

    MIDL_INTERFACE("29E691FA-4567-4DCA-B319-D0F207EB6807")
    ICompositorDesktopInterop : ::IUnknown
    {
        virtual HRESULT STDMETHODCALLTYPE CreateDesktopWindowTarget(
            HWND hwndTarget,
            BOOL isTopmost,
            ABI::Windows::UI::Composition::Desktop::IDesktopWindowTarget * *result) = 0;

        virtual HRESULT STDMETHODCALLTYPE EnsureOnThread(DWORD threadId) = 0;
    };
} // namespace ABI::Windows::UI::Composition::Desktop

#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(
    ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop,
    // clang-format off
    0x29E691FA, 0x4567, 0x4DCA, 0xB3, 0x19, 0xD0, 0xF2, 0x07, 0xEB, 0x68, 0x07
    // clang-format on
)
#endif
#endif
#endif

// https://learn.microsoft.com/en-us/windows/win32/api/windows.ui.composition.interop

namespace
{
    // See
    // https://learn.microsoft.com/en-us/windows/apps/desktop/modernize/using-the-visual-layer-with-win32
    PyObject* create_desktop_window_target(
        PyObject* /*unused*/, PyObject* args) noexcept
    {
        namespace abi = ABI::Windows::UI::Composition::Desktop;

        PyObject* compositor;
        Py_ssize_t hwnd_target;
        int is_topmost;

        if (!PyArg_ParseTuple(args, "Onp", &compositor, &hwnd_target, &is_topmost))
        {
            return nullptr;
        }

        abi::ICompositorDesktopInterop* interop{};
        if (!interop::query_capsule(compositor, &interop))
        {
            return nullptr;
        }

        abi::IDesktopWindowTarget* target{};

        auto const hr = interop->CreateDesktopWindowTarget(
            reinterpret_cast<HWND>(hwnd_target), is_topmost, &target);
        interop->Release();
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        return interop::new_interface_capsule(reinterpret_cast<IUnknown*>(target));
    }

    PyMethodDef module_methods[]{
        {"create_desktop_window_target",
         create_desktop_window_target,
         METH_VARARGS,
         nullptr},
        {}};

    PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_ui_composition_interop",
           nullptr,
           0,
           module_methods,
           interop::module_slots,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_windows_ui_composition_interop(void) noexcept
{
    return PyModuleDef_Init(&module_def);
}
