// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Windows.ApplicationModel.Preview.Notes.h>

namespace py::proj::Windows::ApplicationModel::Preview::Notes
{
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Preview::Notes::NotePlacementChangedPreviewEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.applicationmodel.preview.notes.NotePlacementChangedPreviewEventArgs";
        static constexpr const char* module_name = "winrt.windows.applicationmodel.preview.notes";
        static constexpr const char* type_name = "NotePlacementChangedPreviewEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Preview::Notes::NoteVisibilityChangedPreviewEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.applicationmodel.preview.notes.NoteVisibilityChangedPreviewEventArgs";
        static constexpr const char* module_name = "winrt.windows.applicationmodel.preview.notes";
        static constexpr const char* type_name = "NoteVisibilityChangedPreviewEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Preview::Notes::NotesWindowManagerPreview>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.applicationmodel.preview.notes.NotesWindowManagerPreview";
        static constexpr const char* module_name = "winrt.windows.applicationmodel.preview.notes";
        static constexpr const char* type_name = "NotesWindowManagerPreview";
    };

    template<>
    struct py_type<winrt::Windows::ApplicationModel::Preview::Notes::NotesWindowManagerPreviewShowNoteOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.applicationmodel.preview.notes.NotesWindowManagerPreviewShowNoteOptions";
        static constexpr const char* module_name = "winrt.windows.applicationmodel.preview.notes";
        static constexpr const char* type_name = "NotesWindowManagerPreviewShowNoteOptions";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Graphics.Imaging.h")
#include "py.Windows.Graphics.Imaging.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

namespace py::impl::Windows::ApplicationModel::Preview::Notes
{
}

namespace py::wrapper::Windows::ApplicationModel::Preview::Notes
{
    using NotePlacementChangedPreviewEventArgs = py::winrt_wrapper<winrt::Windows::ApplicationModel::Preview::Notes::NotePlacementChangedPreviewEventArgs>;
    using NoteVisibilityChangedPreviewEventArgs = py::winrt_wrapper<winrt::Windows::ApplicationModel::Preview::Notes::NoteVisibilityChangedPreviewEventArgs>;
    using NotesWindowManagerPreview = py::winrt_wrapper<winrt::Windows::ApplicationModel::Preview::Notes::NotesWindowManagerPreview>;
    using NotesWindowManagerPreviewShowNoteOptions = py::winrt_wrapper<winrt::Windows::ApplicationModel::Preview::Notes::NotesWindowManagerPreviewShowNoteOptions>;
}

namespace py
{
}
