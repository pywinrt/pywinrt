cmake_minimum_required(VERSION 3.31)
cmake_policy(SET CMP0091 NEW)
project(pywinrt-projection LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES 0)

# This can be a lot of files, so this really speeds up the install and prevents
# filling up the console buffer with the output.
set(CMAKE_INSTALL_MESSAGE NEVER)

option(PYWINRT_FULL_PROJECTION "Build full projection" ON)
set(PYWINRT_SITE_PACKAGES_DIR "." CACHE STRING "Python site-packages install prefix")

# .config/_tools.json is the single source of truth for NuGet package versions
file(READ "../.config/_tools.json" TOOLS_JSON)
string(JSON WINDOWS_APP_SDK_VERSION GET ${TOOLS_JSON} "Microsoft.WindowsAppSDK")
string(JSON WEBVIEW2_VERSION GET ${TOOLS_JSON} "Microsoft.Web.WebView2")
string(JSON TEST_WINRT_VERSION GET ${TOOLS_JSON} "PyWinRT.TestWinRT")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
link_libraries("RuntimeObject.lib")

if (MSVC)
    add_compile_options(/bigobj /permissive- /W4 /WX /Gy)
else()
    add_compile_options(-Werror -Wall -Wextra)

    if (MINGW)
        add_definitions(-D_WIN32_WINNT=_WIN32_WINNT_WIN10)
        # fixes "file too big"
        add_compile_options(-Wa,-mbig-obj)
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC 15 complains about type-punned pointers
    add_compile_options(-Wno-strict-aliasing)
    # GCC 15 doesn't like winrt::deferrable_event_args
    add_compile_options(-Wno-template-body)
endif()

find_package(Python3 REQUIRED COMPONENTS Development.Module)

# helper functions
################################################################################

set(WINRT_SDK_PATH "winrt-sdk/src/winrt_sdk")
set(WINRT_MICROSOFT_UI_XAML_PATH "winrt-Microsoft.UI.Xaml/src/winrt_microsoft_ui_xaml")
set(WINRT_WINDOWS_APP_SDK_PATH "winrt-WindowsAppSDK/src/winrt_windows_app_sdk")
set(WINRT_WEBVIEW2_PATH "webview2/webview2-Microsoft.Web.WebView2.Core/webview2")
set(WINDOWS_APP_SDK_PATH "../_tools/Microsoft.WindowsAppSDK.${WINDOWS_APP_SDK_VERSION}")
set(WEBVIEW2_PATH "../_tools/Microsoft.Web.WebView2.${WEBVIEW2_VERSION}")

if (MSVC)
    string(TOLOWER ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID} WINDOWS_APP_SDK_ARCH)
else()
    if (CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES "^i\\d86$")
        set(WINDOWS_APP_SDK_ARCH "x86")
    elseif (CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "x86_64")
        set(WINDOWS_APP_SDK_ARCH "x64")
    elseif (CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "aarch64")
        set(WINDOWS_APP_SDK_ARCH "arm64")
    else()
        message(FATAL_ERROR "Unsupported compiler architecture: ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
    endif()
endif()
message(STATUS "Windows App SDK architecture: ${WINDOWS_APP_SDK_ARCH}")

function(pywinrt_add_library target sources)
    Python3_add_library(${target} MODULE "${sources}")
    set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_NAME_DEBUG ${target}_d)
    target_include_directories(${target} PRIVATE "${WINRT_SDK_PATH}/cppwinrt" "${WINRT_SDK_PATH}/pywinrt")

    if (MSVC)
        target_link_options(${target} PRIVATE $<$<CONFIG:RelWithDebInfo>:/PROFILE /INCREMENTAL:NO>)
    endif()
endfunction()

set(circular_dependency_packages
    "winrt/winrt-Windows.UI.Xaml.Controls"
    "winrt/winrt-Windows.UI.Xaml.Controls.Primitives"
    "winui3/winui3-Microsoft.UI.Xaml.Controls"
    "winui3/winui3-Microsoft.UI.Xaml.Controls.Primitives"
)

function(pywinrt_add_namespace_package package)
    # package_name: e.g. winrt-Windows.Foundation, test-winrt-TestComponent
    get_filename_component(package_name "${package}" NAME)

    # split package name on last dash
    string(FIND "${package_name}" "-" index REVERSE)
    math(EXPR index_plus "${index} + 1")

    # package_root: e.g. winrt, test-winrt
    string(SUBSTRING "${package_name}" 0 ${index} package_root)

    # package_root_dir: e.g. winrt, test_winrt
    string(REPLACE "-" "_" package_root_dir "${package_root}")

    # package_namespace: e.g. Windows.Foundation, TestComponent
    string(SUBSTRING "${package_name}" ${index_plus} -1 package_namespace)

    # package_namespace_lower: e.g. windows.foundation, testcomponent
    string(TOLOWER "${package_namespace}" package_namespace_lower)

    # package_target: e.g. _winrt_windows_foundation, _test_winrt_testcomponent
    string(REPLACE "." "_" package_target_suffix "${package_namespace_lower}")
    set(package_target "_${package_root_dir}_${package_target_suffix}")

    # package_sources: e.g. py.Windows.Foundation.cpp, py.TestComponent.cpp
    set(package_sources "${package}/py.${package_namespace}.cpp")


    pywinrt_add_library(${package_target} "${package_sources}")
    target_include_directories(${package_target} PRIVATE
        "${package}/${package_root_dir}/cppwinrt"
        "${package}/${package_root_dir}/pywinrt"
    )

    install(TARGETS ${package_target} DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/${package_root_dir}")
    install(DIRECTORY "${package}/${package_root_dir}/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/${package_root_dir}")

    if (package IN_LIST circular_dependency_packages)
        set(package_sources_2 "${package}/py.${package_namespace}_2.cpp")

        pywinrt_add_library(${package_target}_2 "${package_sources_2}")
        target_include_directories(${package_target}_2 PRIVATE
            "${package}/${package_root_dir}/cppwinrt"
            "${package}/${package_root_dir}/pywinrt"
        )

        install(TARGETS ${package_target}_2 DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/${package_root_dir}")
    endif()

    return(PROPAGATE package_target)
endfunction()

# main runtime package
################################################################################

set(WINRT_RUNTIME_PATH "winrt-runtime")
set(WINRT_RUNTIME_SOURCES
    "${WINRT_RUNTIME_PATH}/_winrt.cpp"
    "${WINRT_RUNTIME_PATH}/_winrt_array.cpp"
    "${WINRT_RUNTIME_PATH}/_winrt_box.cpp"
    "${WINRT_RUNTIME_PATH}/_winrt_buffer.cpp"
    "${WINRT_RUNTIME_PATH}/runtime.cpp"
)
pywinrt_add_library(_winrt "${WINRT_RUNTIME_SOURCES}")
install(TARGETS _winrt DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
install(DIRECTORY "${WINRT_RUNTIME_PATH}/src/winrt/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")

# windows sdk packages
################################################################################

if (PYWINRT_FULL_PROJECTION)
    file(GLOB packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winrt/winrt-*")
    file(GLOB winui2_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winui2/winui2-*")
    file(GLOB winui3_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winui3/winui3-*")
    file(GLOB webview2_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "webview2/webview2-*")
    file(GLOB interop_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "interop/winrt-*")
    file(GLOB winui3_interop_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "interop/winui3-*")
else()
    # minimum set of packages required for unit tests
    set(packages
        "winrt/winrt-Windows.ApplicationModel.Core"
        "winrt/winrt-Windows.Data.Json"
        "winrt/winrt-Windows.Devices"
        "winrt/winrt-Windows.Devices.Geolocation"
        "winrt/winrt-Windows.Foundation"
        "winrt/winrt-Windows.Foundation.Collections"
        "winrt/winrt-Windows.Foundation.Numerics"
        "winrt/winrt-Windows.Graphics"
        "winrt/winrt-Windows.Graphics.Capture"
        "winrt/winrt-Windows.Graphics.DirectX"
        "winrt/winrt-Windows.Graphics.DirectX.Direct3D11"
        "winrt/winrt-Windows.Graphics.Imaging"
        "winrt/winrt-Windows.Media"
        "winrt/winrt-Windows.Storage"
        "winrt/winrt-Windows.Storage.Streams"
        "winrt/winrt-Windows.System"
        "winrt/winrt-Windows.UI"
        "winrt/winrt-Windows.UI.Composition"
        "winrt/winrt-Windows.UI.Composition.Desktop"
        "winrt/winrt-Windows.UI.WindowManagement"
        "winrt/winrt-Windows.UI.Xaml.Hosting"
    )
    set(winui3_packages
        "winui3/winui3-Microsoft.UI"
        "winui3/winui3-Microsoft.Windows.AppNotifications"
    )
    set(interop_packages
        "interop/winrt-Windows.Graphics.Capture.Interop"
        "interop/winrt-Windows.Graphics.DirectX.Direct3D11.Interop"
        "interop/winrt-Windows.Media.Interop"
        "interop/winrt-Windows.System.Interop"
        "interop/winrt-Windows.UI.Composition.Interop"
        "interop/winrt-Windows.UI.Xaml.Hosting.Interop"
    )
    set(winui3_interop_packages
        "interop/winui3-Microsoft.UI.Interop"
        "interop/winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap"
    )
endif()

foreach(package ${packages} ${winui2_packages} ${winui3_packages} ${webview2_packages} ${interop_packages} ${winui3_interop_packages})
    pywinrt_add_namespace_package(${package})

    # speed up builds by precompiling frequently used headers
    if (package IN_LIST packages OR package IN_LIST winui2_packages OR package IN_LIST winui3_packages)
        if (NOT first_target)
            set(first_target ${package_target})

            if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
                # GCC can't allocate enough memory for more than this
                target_precompile_headers(${first_target} PRIVATE
                    "${WINRT_SDK_PATH}/pywinrt/py.Windows.Foundation.h"
                )
            else()
                target_precompile_headers(${first_target} PRIVATE
                    "${WINRT_SDK_PATH}/pywinrt/py.Windows.Foundation.h"
                    "${WINRT_SDK_PATH}/pywinrt/py.Windows.Storage.Streams.h"
                )
            endif()
        else()
            target_precompile_headers(${package_target} REUSE_FROM ${first_target})
            if (${package} IN_LIST circular_dependency_packages)
                target_precompile_headers(${package_target}_2 REUSE_FROM ${first_target})
            endif()
        endif()
    endif()

    # don't define ${package_target}_EXPORTS symbol to avoid shared precompiled header warning
    set_target_properties(${package_target} PROPERTIES DEFINE_SYMBOL "")
    if (${package} IN_LIST circular_dependency_packages)
        set_target_properties(${package_target}_2 PROPERTIES DEFINE_SYMBOL "")
    endif()

    # microsft ui xaml headers
    if (package IN_LIST winui2_packages)
        target_include_directories(${package_target} PRIVATE
            "${WINRT_MICROSOFT_UI_XAML_PATH}/cppwinrt"
            "${WINRT_MICROSOFT_UI_XAML_PATH}/pywinrt"
            "${WINRT_WEBVIEW2_PATH}/cppwinrt"
            "${WINRT_WEBVIEW2_PATH}/pywinrt"
        )
    endif()

    # windows app sdk headers
    if (package IN_LIST winui3_packages OR package IN_LIST winui3_interop_packages)
        target_include_directories(${package_target} PRIVATE
            "${WINRT_WINDOWS_APP_SDK_PATH}/cppwinrt"
            "${WINRT_WINDOWS_APP_SDK_PATH}/pywinrt"
            "${WINRT_WEBVIEW2_PATH}/cppwinrt"
            "${WINRT_WEBVIEW2_PATH}/pywinrt"
        )
        if (${package} IN_LIST circular_dependency_packages)
            target_include_directories(${package_target}_2 PRIVATE
                "${WINRT_WINDOWS_APP_SDK_PATH}/cppwinrt"
                "${WINRT_WINDOWS_APP_SDK_PATH}/pywinrt"
                "${WINRT_WEBVIEW2_PATH}/cppwinrt"
                "${WINRT_WEBVIEW2_PATH}/pywinrt"
            )
        endif()
    endif()

    if (package IN_LIST winui3_interop_packages)
        target_include_directories(${package_target} PRIVATE
            "${WINDOWS_APP_SDK_PATH}/include"
        )
        target_link_directories(${package_target} PRIVATE
            "${WINDOWS_APP_SDK_PATH}/lib/win10-${WINDOWS_APP_SDK_ARCH}"
        )
    endif()

    if (${package} STREQUAL "interop/winrt-Windows.Graphics.DirectX.Direct3D11.Interop")
        target_link_libraries(${package_target} PRIVATE "D3D11")
    endif()

    if (${package} STREQUAL "interop/winrt-Windows.System.Interop")
        target_link_libraries(${package_target} PRIVATE "CoreMessaging")
    endif()

    if (${package} STREQUAL "interop/winui3-Microsoft.UI.Interop")
        if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            # winui3/Microsoft.UI.Interop.h has stuff that other compilers don't like
            target_compile_options(${package_target} PRIVATE -Wno-unknown-pragmas)
        endif()
    endif()

    if (${package} STREQUAL "interop/winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap")
        target_link_libraries(${package_target} PRIVATE "Microsoft.WindowsAppRuntime.Bootstrap")
        install(FILES "${WINDOWS_APP_SDK_PATH}/runtimes/win-${WINDOWS_APP_SDK_ARCH}/native/Microsoft.WindowsAppRuntime.Bootstrap.dll"
            DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winui3")
    endif()

    if (${package} STREQUAL "webview2/webview2-Microsoft.Web.WebView2.Core")
        install(FILES "${WEBVIEW2_PATH}/runtimes/win-${WINDOWS_APP_SDK_ARCH}/native_uap/Microsoft.Web.WebView2.Core.dll"
            DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/webview2/microsoft/web/webview2/core")
    endif()
endforeach()

# test component package
################################################################################

pywinrt_add_namespace_package("test-winrt/test-winrt-TestComponent")
install(FILES "../_tools/PyWinRT.TestWinRT.${TEST_WINRT_VERSION}/runtimes/win10-${WINDOWS_APP_SDK_ARCH}/native/TestComponent.dll"
    DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/test_winrt/testcomponent/")
