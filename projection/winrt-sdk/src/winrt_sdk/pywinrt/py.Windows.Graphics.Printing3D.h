// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Foundation.Numerics.h")
#include "py.Windows.Foundation.Numerics.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

#if __has_include("py.Windows.UI.h")
#include "py.Windows.UI.h"
#endif

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.h>

#include <winrt/Windows.Graphics.Printing3D.h>

namespace py::proj::Windows::Graphics::Printing3D
{}

namespace py::impl::Windows::Graphics::Printing3D
{
    struct Print3DTaskSourceRequestedHandler
    {
        static winrt::Windows::Graphics::Printing3D::Print3DTaskSourceRequestedHandler get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)](auto param0)
            {
                winrt::handle_type<py::gil_state_traits> gil_state{ PyGILState_Ensure() };

                py::pyobj_handle py_param0{ py::convert(param0) };

                py::pyobj_handle args{ PyTuple_Pack(1, py_param0.get()) };

                if (!args) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }

                py::pyobj_handle return_value{ PyObject_CallObject(delegate.callable(), args.get()) };

                if (!return_value)
                {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }
            };
        };
    };
}

namespace py::wrapper::Windows::Graphics::Printing3D
{
    using Print3DManager = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Print3DManager>;
    using Print3DTask = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Print3DTask>;
    using Print3DTaskCompletedEventArgs = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Print3DTaskCompletedEventArgs>;
    using Print3DTaskRequest = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Print3DTaskRequest>;
    using Print3DTaskRequestedEventArgs = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Print3DTaskRequestedEventArgs>;
    using Print3DTaskSourceChangedEventArgs = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Print3DTaskSourceChangedEventArgs>;
    using Print3DTaskSourceRequestedArgs = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Print3DTaskSourceRequestedArgs>;
    using Printing3D3MFPackage = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3D3MFPackage>;
    using Printing3DBaseMaterial = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DBaseMaterial>;
    using Printing3DBaseMaterialGroup = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DBaseMaterialGroup>;
    using Printing3DColorMaterial = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DColorMaterial>;
    using Printing3DColorMaterialGroup = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DColorMaterialGroup>;
    using Printing3DComponent = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DComponent>;
    using Printing3DComponentWithMatrix = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DComponentWithMatrix>;
    using Printing3DCompositeMaterial = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DCompositeMaterial>;
    using Printing3DCompositeMaterialGroup = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DCompositeMaterialGroup>;
    using Printing3DFaceReductionOptions = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DFaceReductionOptions>;
    using Printing3DMaterial = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DMaterial>;
    using Printing3DMesh = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DMesh>;
    using Printing3DMeshVerificationResult = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DMeshVerificationResult>;
    using Printing3DModel = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DModel>;
    using Printing3DModelTexture = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DModelTexture>;
    using Printing3DMultiplePropertyMaterial = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DMultiplePropertyMaterial>;
    using Printing3DMultiplePropertyMaterialGroup = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DMultiplePropertyMaterialGroup>;
    using Printing3DTexture2CoordMaterial = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DTexture2CoordMaterial>;
    using Printing3DTexture2CoordMaterialGroup = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DTexture2CoordMaterialGroup>;
    using Printing3DTextureResource = py::winrt_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DTextureResource>;
    using Printing3DBufferDescription = py::winrt_struct_wrapper<winrt::Windows::Graphics::Printing3D::Printing3DBufferDescription>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Printing3D::Print3DTaskCompletion> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Printing3D::Print3DTaskDetail> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Printing3D::Printing3DBufferFormat> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Printing3D::Printing3DMeshVerificationMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Printing3D::Printing3DModelUnit> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Printing3D::Printing3DObjectType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Printing3D::Printing3DPackageCompression> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Printing3D::Printing3DTextureEdgeBehavior> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Graphics::Printing3D::Printing3DBufferDescription> = "T{i:format:I:stride:}";


    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Print3DTaskCompletion>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Print3DTaskCompletion";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Print3DTaskDetail>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Print3DTaskDetail";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DBufferFormat>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DBufferFormat";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DMeshVerificationMode>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DMeshVerificationMode";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DModelUnit>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DModelUnit";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DObjectType>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DObjectType";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DPackageCompression>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DPackageCompression";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DTextureEdgeBehavior>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DTextureEdgeBehavior";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Print3DManager>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Print3DManager";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Print3DTask>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Print3DTask";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Print3DTaskCompletedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Print3DTaskCompletedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Print3DTaskRequest>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Print3DTaskRequest";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Print3DTaskRequestedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Print3DTaskRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Print3DTaskSourceChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Print3DTaskSourceChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Print3DTaskSourceRequestedArgs>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Print3DTaskSourceRequestedArgs";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3D3MFPackage>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3D3MFPackage";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DBaseMaterial>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DBaseMaterial";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DBaseMaterialGroup>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DBaseMaterialGroup";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DColorMaterial>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DColorMaterial";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DColorMaterialGroup>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DColorMaterialGroup";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DComponent>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DComponent";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DComponentWithMatrix>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DComponentWithMatrix";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DCompositeMaterial>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DCompositeMaterial";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DCompositeMaterialGroup>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DCompositeMaterialGroup";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DFaceReductionOptions>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DFaceReductionOptions";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DMaterial>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DMaterial";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DMesh>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DMesh";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DMeshVerificationResult>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DMeshVerificationResult";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DModel>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DModel";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DModelTexture>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DModelTexture";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DMultiplePropertyMaterial>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DMultiplePropertyMaterial";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DMultiplePropertyMaterialGroup>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DMultiplePropertyMaterialGroup";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DTexture2CoordMaterial>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DTexture2CoordMaterial";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DTexture2CoordMaterialGroup>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DTexture2CoordMaterialGroup";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DTextureResource>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DTextureResource";
    };

    template<>
    struct py_type<winrt::Windows::Graphics::Printing3D::Printing3DBufferDescription>
    {
        static constexpr const char* module_name = "winrt.windows.graphics.printing3d";
        static constexpr const char* type_name = "Printing3DBufferDescription";
    };
    template <>
    struct delegate_python_type<winrt::Windows::Graphics::Printing3D::Print3DTaskSourceRequestedHandler>
    {
        using type = py::impl::Windows::Graphics::Printing3D::Print3DTaskSourceRequestedHandler;
    };

}