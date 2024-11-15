cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0091 NEW)
project(pywinrt-projection LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PYWINRT_FULL_PROJECTION "Build full projection" ON)
set(PYWINRT_SITE_PACKAGES_DIR "." CACHE STRING "Python site-packages install prefix")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
link_libraries("RuntimeObject.lib")

if (MSVC)
    add_compile_options(/bigobj /permissive- /EHsc /W4 /WX /Gy)
else()
    add_compile_options(-Werror -Wall -Wextra)

    if (MINGW)
        add_definitions(-D_WIN32_WINNT=_WIN32_WINNT_WIN10)
        # fixes "file too big"
        add_compile_options(-Wa,-mbig-obj)
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC 13 complains about error: 'this' pointer is null [-Werror=nonnull]
    # in cppwrint/winrt/base.h
    # https://github.com/microsoft/cppwinrt/issues/1349
    add_compile_options(-Wno-nonnull)
endif()

# work around https://gitlab.kitware.com/cmake/cmake/-/issues/24587
if(CMAKE_VERSION VERSION_LESS 3.27 AND (NOT CMAKE_GENERATOR_PLATFORM AND CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64" OR CMAKE_GENERATOR_PLATFORM MATCHES "ARM64"))
    foreach (version IN_LIST 3.13 3.12 3.11 3.10 3.9)
        list(APPEND Python3_ROOT_DIR [HKEY_CURRENT_USER/SOFTWARE/Python/PythonCore/${version}-arm64/InstallPath])
        list(APPEND Python3_ROOT_DIR [HKEY_LOCAL_MACHINE/SOFTWARE/Python/PythonCore/${version}-arm64/InstallPath])
    endforeach()
endif()

find_package(Python3 REQUIRED COMPONENTS Development.Module)

# helper functions
################################################################################

set(WINRT_SDK_PATH "winrt-sdk/src/winrt_sdk")
set(WINRT_WINDOWS_APP_SDK_PATH "winrt-WindowsAppSDK/src/winrt_windows_app_sdk")
set(WINRT_WEBVIEW2_PATH "winrt-Microsoft.Web.WebView2.Core")
set(WINDOWS_APP_SDK_PATH "../_tools/Microsoft.WindowsAppSDK")
set(WEBVIEW2_PATH "../_tools/Microsoft.Web.WebView2")

if (CMAKE_CXX_COMPILER_ARCHITECTURE_ID)
    string(TOLOWER ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID} WINDOWS_APP_SDK_ARCH)
else()
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine
        RESULT_VARIABLE EXIT_CODE
        OUTPUT_VARIABLE TARGET_TRIPLE
        ERROR_QUIET
    )

    if (EXIT_CODE)
        message(FATAL_ERROR "Failed to get target architecture triple: ${EXIT_CODE}")
    endif()

    if (TARGET_TRIPLE MATCHES "^x86_64-.*")
        set(WINDOWS_APP_SDK_ARCH "x64")
    elseif (TARGET_TRIPLE MATCHES "^i686-.*")
        set(WINDOWS_APP_SDK_ARCH "x86")
    elseif (TARGET_TRIPLE MATCHES "^aarch64-.*")
        set(WINDOWS_APP_SDK_ARCH "arm64")
    else()
        message(FATAL_ERROR "Unsupported target architecture: ${TARGET_TRIPLE}")
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
    "winrt-Microsoft.UI.Xaml.Controls"
    "winrt-Microsoft.UI.Xaml.Controls.Primitives"
    "winrt-Windows.UI.Xaml.Controls"
    "winrt-Windows.UI.Xaml.Controls.Primitives"
)

function(pywinrt_add_namespace_package package)
    get_filename_component(package_name "${package}" NAME)
    string(REPLACE "winrt-" "" package_namespace "${package_name}")
    string(TOLOWER "${package_namespace}" package_namespace_lower)
    string(REPLACE "." "_" package_target_suffix "${package_namespace_lower}")
    set(package_target "_winrt_${package_target_suffix}" )
    set(package_sources "${package}/py.${package_namespace}.cpp")

    pywinrt_add_library(${package_target} "${package_sources}")
    target_include_directories(${package_target} PRIVATE "${package}/cppwinrt" "${package}")

    install(TARGETS ${package_target} DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
    install(DIRECTORY "${package}/winrt/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")

    if (package IN_LIST circular_dependency_packages)
        set(package_sources_2 "${package}/py.${package_namespace}_2.cpp")

        pywinrt_add_library(${package_target}_2 "${package_sources_2}")
        target_include_directories(${package_target}_2 PRIVATE "${package}/cppwinrt" "${package}")

        install(TARGETS ${package_target}_2 DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
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
    "${WINRT_RUNTIME_PATH}/runtime.cpp"
)
pywinrt_add_library(_winrt "${WINRT_RUNTIME_SOURCES}")
install(TARGETS _winrt DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
install(DIRECTORY "${WINRT_RUNTIME_PATH}/src/winrt/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")

# windows sdk packages
################################################################################

if (PYWINRT_FULL_PROJECTION)
    file(GLOB packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winrt-Windows.*")
    file(GLOB app_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winrt-Microsoft.*")
    file(GLOB webview2_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winrt-Microsoft.Web.WebView2.*")
    list(REMOVE_ITEM app_packages ${webview2_packages})
    file(GLOB interop_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "interop/winrt-Windows.*")
    file(GLOB app_interop_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "interop/winrt-Microsoft.*")
else()
    # minimum set of packages required for unit tests
    set(packages
        "winrt-Windows.ApplicationModel.Core"
        "winrt-Windows.Data.Json"
        "winrt-Windows.Devices"
        "winrt-Windows.Devices.Geolocation"
        "winrt-Windows.Foundation"
        "winrt-Windows.Foundation.Collections"
        "winrt-Windows.Foundation.Numerics"
        "winrt-Windows.Graphics"
        "winrt-Windows.Graphics.Capture"
        "winrt-Windows.Graphics.DirectX"
        "winrt-Windows.Graphics.DirectX.Direct3D11"
        "winrt-Windows.Graphics.Imaging"
        "winrt-Windows.Storage"
        "winrt-Windows.Storage.Streams"
        "winrt-Windows.System"
        "winrt-Windows.UI"
        "winrt-Windows.UI.Composition"
        "winrt-Windows.UI.Composition.Desktop"
        "winrt-Windows.UI.WindowManagement"
    )
    set(app_packages
        "winrt-Microsoft.UI"
        "winrt-Microsoft.Windows.AppNotifications"
    )
    set(interop_packages
        "interop/winrt-Windows.Graphics.Capture.Interop"
        "interop/winrt-Windows.Graphics.DirectX.Direct3D11.Interop"
        "interop/winrt-Windows.System.Interop"
        "interop/winrt-Windows.UI.Composition.Interop"
    )
    set(app_interop_packages
        "interop/winrt-Microsoft.UI.Interop"
        "interop/winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap"
    )
endif()

foreach(package ${packages} ${app_packages} ${webview2_packages} ${interop_packages} ${app_interop_packages})
    pywinrt_add_namespace_package(${package})

    # speed up builds by precompiling frequently used headers
    if (package IN_LIST packages OR package IN_LIST app_packages)
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

    # windows app sdk headers
    if (package IN_LIST app_packages OR package IN_LIST app_interop_packages)
        target_include_directories(${package_target} PRIVATE
            "${WINRT_WINDOWS_APP_SDK_PATH}/cppwinrt"
            "${WINRT_WINDOWS_APP_SDK_PATH}/pywinrt"
            "${WINRT_WEBVIEW2_PATH}/cppwinrt"
            "${WINRT_WEBVIEW2_PATH}"
        )
        if (${package} IN_LIST circular_dependency_packages)
            target_include_directories(${package_target}_2 PRIVATE
                "${WINRT_WINDOWS_APP_SDK_PATH}/cppwinrt"
                "${WINRT_WINDOWS_APP_SDK_PATH}/pywinrt"
                "${WINRT_WEBVIEW2_PATH}/cppwinrt"
                "${WINRT_WEBVIEW2_PATH}"
            )
        endif()
    endif()

    if (package IN_LIST app_interop_packages)
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

    if (${package} STREQUAL "interop/winrt-Microsoft.UI.Interop")
        if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            # winrt/Microsoft.UI.Interop.h has stuff that other compilers don't like
            target_compile_options(${package_target} PRIVATE -Wno-unknown-pragmas)
        endif()
    endif()

    if (${package} STREQUAL "interop/winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap")
        target_link_libraries(${package_target} PRIVATE "Microsoft.WindowsAppRuntime.Bootstrap")
        install(FILES "${WINDOWS_APP_SDK_PATH}/runtimes/win-${WINDOWS_APP_SDK_ARCH}/native/Microsoft.WindowsAppRuntime.Bootstrap.dll"
            DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
    endif()

    if (${package} STREQUAL "winrt-Microsoft.Web.WebView2.Core")
        install(FILES "${WEBVIEW2_PATH}/runtimes/win-${WINDOWS_APP_SDK_ARCH}/native_uap/Microsoft.Web.WebView2.Core.dll"
            DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt/microsoft/web/webview2/core")
    endif()
endforeach()

# test component package
################################################################################

pywinrt_add_namespace_package("winrt-TestComponent")
install(FILES "../_tools/PyWinRT.TestWinRT/runtimes/win10-${WINDOWS_APP_SDK_ARCH}/native/TestComponent.dll"
    DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt/testcomponent/")
