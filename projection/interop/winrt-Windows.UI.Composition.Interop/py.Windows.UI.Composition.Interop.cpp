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
            ABI::Windows::UI::Composition::Desktop::IDesktopWindowTarget * *result)
            = 0;

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

#include <winrt/base.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>

#include <Python.h>

#include "pybase.h"
#include <py.Windows.UI.Composition.h>
#include <py.Windows.UI.Composition.Desktop.h>

// https://learn.microsoft.com/en-us/windows/win32/api/windows.ui.composition.interop

namespace py::cpp::Windows::UI::Composition::Interop
{
    // See
    // https://learn.microsoft.com/en-us/windows/apps/desktop/modernize/using-the-visual-layer-with-win32
    static PyObject* create_desktop_window_target(
        PyObject* /*unused*/, PyObject* args, PyObject* kw) noexcept
    {
        namespace abi = ABI::Windows::UI::Composition::Desktop;

        try
        {
            PyObject* arg0;
            Py_ssize_t arg1;
            int arg2{};

            static const char* const keywords[]
                = {"compositor", "hwnd_target", "is_topmost", nullptr};

            if (!PyArg_ParseTupleAndKeywords(
                    args,
                    kw,
                    "On|$p",
                    const_cast<char**>(keywords),
                    &arg0,
                    &arg1,
                    &arg2))
            {
                return nullptr;
            }

            auto compositor
                = convert_to<winrt::Windows::UI::Composition::Compositor>(arg0);
            auto interop = compositor.as<abi::ICompositorDesktopInterop>();
            auto hwnd_target = reinterpret_cast<HWND>(arg1);
            bool is_topmost = arg2;

            winrt::Windows::UI::Composition::Desktop::DesktopWindowTarget target{
                nullptr};
            winrt::check_hresult(interop->CreateDesktopWindowTarget(
                hwnd_target,
                is_topmost,
                reinterpret_cast<abi::IDesktopWindowTarget**>(winrt::put_abi(target))));

            return convert(target);
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }
    }

    PyMethodDef module_methods[]
        = {{"create_desktop_window_target",
            reinterpret_cast<PyCFunction>(
                reinterpret_cast<void*>(create_desktop_window_target)),
            METH_VARARGS | METH_KEYWORDS,
            nullptr},
           {}};

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_ui_composition_interop",
           nullptr,
           0,
           module_methods,
           nullptr,
           nullptr,
           nullptr,
           nullptr};
} // namespace py::cpp::Windows::UI::Composition::Interop

PyMODINIT_FUNC PyInit__winrt_windows_ui_composition_interop(void) noexcept
{
    using namespace py::cpp::Windows::UI::Composition::Interop;

    if (py::import_winrt_runtime() == -1)
    {
        return nullptr;
    }

    return PyModule_Create(&module_def);
}
