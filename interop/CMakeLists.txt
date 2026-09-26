# interop modules
################################################################################
#
# The desktop interop APIs are the only compiled part of the projection that is
# left: each of them calls a plain COM interface or a flat C function that is
# not in any winmd, so there is nothing for a table to describe. They are
# hand-written and they name the WinRT types they hand over rather than sharing
# generated code with the packages that project them. They reach the runtime
# only through Python objects, so they are compiled without its headers.

function(pywinrt_add_interop_module target package source root)
    pywinrt_add_module(${target} "${package}/${source}")
    install(TARGETS ${target} DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/${root}")
    install(DIRECTORY "${package}/${root}/"
        DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/${root}")
endfunction()

pywinrt_add_interop_module(_winrt_windows_graphics_capture_interop
    "winrt-Windows.Graphics.Capture.Interop"
    "py.Windows.Graphics.Capture.Interop.cpp" "winrt")

pywinrt_add_interop_module(_winrt_windows_graphics_directx_direct3d11_interop
    "winrt-Windows.Graphics.DirectX.Direct3D11.Interop"
    "py.Windows.Graphics.DirectX.Direct3D11.Interop.cpp" "winrt")
target_link_libraries(_winrt_windows_graphics_directx_direct3d11_interop PRIVATE "D3D11.lib")

pywinrt_add_interop_module(_winrt_windows_media_interop
    "winrt-Windows.Media.Interop" "py.Windows.Media.Interop.cpp" "winrt")

pywinrt_add_interop_module(_winrt_windows_system_interop
    "winrt-Windows.System.Interop" "py.Windows.System.Interop.cpp" "winrt")
target_link_libraries(_winrt_windows_system_interop PRIVATE "CoreMessaging.lib")

pywinrt_add_interop_module(_winrt_windows_ui_composition_interop
    "winrt-Windows.UI.Composition.Interop" "py.Windows.UI.Composition.Interop.cpp" "winrt")

pywinrt_add_interop_module(_winrt_windows_ui_xaml_hosting_interop
    "winrt-Windows.UI.Xaml.Hosting.Interop"
    "py.Windows.UI.Xaml.Hosting.Interop.cpp" "winrt")

# The two Windows App SDK modules take its headers and its import libraries as
# well, since the App SDK is redistributed with the app rather than part of
# Windows. Each takes them from the component that ships what it includes:
# Microsoft.UI.Interop.h from InteractiveExperiences, and the bootstrapper's
# header, import library and DLL from Foundation. The one header the Runtime
# component is read for is fetched by scripts/app_sdk.py rather than installed.
set(WASDK_FOUNDATION_PATH "${PYWINRT_TOOLS_PATH}/Microsoft.WindowsAppSDK.Foundation.${WASDK_FOUNDATION_VERSION}")
set(WASDK_INTERACTIVE_EXPERIENCES_PATH "${PYWINRT_TOOLS_PATH}/Microsoft.WindowsAppSDK.InteractiveExperiences.${WASDK_INTERACTIVE_EXPERIENCES_VERSION}")
set(WASDK_RUNTIME_PATH "${PYWINRT_TOOLS_PATH}/Microsoft.WindowsAppSDK.Runtime.${WASDK_RUNTIME_VERSION}")
set(WASDK_FOUNDATION_LIB_PATH "${WASDK_FOUNDATION_PATH}/lib/native/${PYWINRT_TARGET_ARCH}")

pywinrt_add_interop_module(_winrt_microsoft_ui_interop
    "winrt-Microsoft.UI.Interop" "py.Microsoft.UI.Interop.cpp" "winrt")
# the one interop module written against C++/WinRT, since that is the only
# form in which the App SDK ships the functions it calls
target_include_directories(_winrt_microsoft_ui_interop PRIVATE
    "winrt-Microsoft.UI.Interop/cppwinrt"
    "${WASDK_INTERACTIVE_EXPERIENCES_PATH}/include")
if (NOT MSVC)
    # the App SDK's Microsoft.UI.Interop.h uses MSVC-only pragmas
    target_compile_options(_winrt_microsoft_ui_interop PRIVATE -Wno-unknown-pragmas)
endif()

pywinrt_add_interop_module(_winrt_microsoft_windows_applicationmodel_dynamicdependency_bootstrap
    "winrt-wasdk-bootstrap"
    "py.Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap.cpp" "winrt")
# MddBootstrap.h is in Foundation and WindowsAppSDK-VersionInfo.h, which says
# which Windows App Runtime this release expects, is in Runtime.
target_include_directories(_winrt_microsoft_windows_applicationmodel_dynamicdependency_bootstrap PRIVATE
    "${WASDK_FOUNDATION_PATH}/include"
    "${WASDK_RUNTIME_PATH}/include")
target_link_directories(_winrt_microsoft_windows_applicationmodel_dynamicdependency_bootstrap PRIVATE
    "${WASDK_FOUNDATION_LIB_PATH}")
target_link_libraries(_winrt_microsoft_windows_applicationmodel_dynamicdependency_bootstrap PRIVATE
    "Microsoft.WindowsAppRuntime.Bootstrap.lib")
# The bootstrapper is what finds the Windows App Runtime, so it is the one App
# SDK binary that ships with the package rather than being found through it.
install(FILES "${WASDK_FOUNDATION_PATH}/runtimes/win-${PYWINRT_TARGET_ARCH}/native/Microsoft.WindowsAppRuntime.Bootstrap.dll"
    DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
