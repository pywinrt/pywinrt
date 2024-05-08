cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0091 NEW)
project(pywinrt-projection LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PYWINRT_FULL_PROJECTION "Build full projection" ON)
set(PYWINRT_SITE_PACKAGES_DIR "." CACHE STRING "Python site-packages install prefix")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
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
set(WINDOWS_APP_SDK_PATH "../_tools/Microsoft.WindowsAppSDK")

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
endfunction()

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

    return(PROPAGATE package_target)
endfunction()

# main runtime package
################################################################################

set(WINRT_RUNTIME_PATH "winrt-runtime")
set(WINRT_RUNTIME_SOURCES "${WINRT_RUNTIME_PATH}/_winrt.cpp" "${WINRT_RUNTIME_PATH}/_winrt_array.cpp" "${WINRT_RUNTIME_PATH}/runtime.cpp")
pywinrt_add_library(_winrt "${WINRT_RUNTIME_SOURCES}")
install(TARGETS _winrt DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
install(DIRECTORY "${WINRT_RUNTIME_PATH}/src/winrt/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")

# windows sdk packages
################################################################################

if (PYWINRT_FULL_PROJECTION)
    file(GLOB packages CONFIGURE_DEPENDS "winrt-Windows.*")
    file(GLOB app_packages CONFIGURE_DEPENDS "winrt-Microsoft.*")
    file(GLOB interop_packages CONFIGURE_DEPENDS "interop/winrt-Windows.*")
    file(GLOB app_interop_packages CONFIGURE_DEPENDS "interop/winrt-Microsoft.*")
else()
    # minimum set of packages required for unit tests
    set(packages
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
        "winrt-Windows.Storage"
        "winrt-Windows.Storage.Streams"
    )
    set(app_packages
        "winrt-Microsoft.Windows.AppNotifications"
    )
    set(interop_packages
        "interop/winrt-Windows.Foundation.Interop"
        "interop/winrt-Windows.Graphics.Capture.Interop"
    )
    set(app_interop_packages
        "interop/winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap"
    )
endif()

foreach(package ${packages} ${app_packages} ${interop_packages} ${app_interop_packages})
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
        endif()
    endif()

    # don't define ${package_target}_EXPORTS symbol to avoid shared precompiled header warning
    set_target_properties(${package_target} PROPERTIES DEFINE_SYMBOL "")

    # windows app sdk headers
    if (package IN_LIST app_packages)
        target_include_directories(${package_target} PRIVATE
            "${WINRT_WINDOWS_APP_SDK_PATH}/cppwinrt"
            "${WINRT_WINDOWS_APP_SDK_PATH}/pywinrt"
        )
    endif()

    if (package IN_LIST app_interop_packages)
        target_include_directories(${package_target} PRIVATE
            "${WINDOWS_APP_SDK_PATH}/include"
        )
        target_link_directories(${package_target} PRIVATE
            "${WINDOWS_APP_SDK_PATH}/lib/win10-${WINDOWS_APP_SDK_ARCH}"
        )
    endif()

    if (${package} STREQUAL "interop/winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap")
        target_link_libraries(${package_target} PRIVATE "Microsoft.WindowsAppRuntime.Bootstrap")
        install(FILES "${WINDOWS_APP_SDK_PATH}/runtimes/win10-${WINDOWS_APP_SDK_ARCH}/native/Microsoft.WindowsAppRuntime.Bootstrap.dll"
            DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
    endif()
endforeach()

# test component package
################################################################################

pywinrt_add_namespace_package("winrt-TestComponent")
