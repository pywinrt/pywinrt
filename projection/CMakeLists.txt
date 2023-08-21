cmake_minimum_required(VERSION 3.24)
cmake_policy(SET CMP0091 NEW)
project(_winrt LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PYWINRT_FULL_PROJECTION "Build full projection" ON)
set(PYWINRT_SITE_PACKAGES_DIR "." CACHE STRING "Python site-packages install prefix")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (MSVC)
    add_compile_options(/bigobj /permissive- /EHsc)
else()
    add_compile_options(-Werror)

    if (MINGW)
        add_definitions(-D_WIN32_WINNT=_WIN32_WINNT_WIN10)
        # fixes "file too big"
        add_compile_options(-Wa,-mbig-obj)
    endif()
endif()

# work around https://gitlab.kitware.com/cmake/cmake/-/issues/24587
if(CMAKE_VERSION VERSION_LESS 3.27 AND NOT CMAKE_GENERATOR_PLATFORM AND CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64" OR CMAKE_GENERATOR_PLATFORM MATCHES "ARM64")
    set(Python3_ROOT_DIR
        [HKEY_CURRENT_USER/SOFTWARE/Python/PythonCore/${PYTHON_BASE_VERSION}-arm64/InstallPath]
        [HKEY_LOCAL_MACHINE/SOFTWARE/Python/PythonCore/${PYTHON_BASE_VERSION}-arm64/InstallPath]
    )
endif()

find_package(Python3 REQUIRED COMPONENTS Development.Module)

set(WINRT_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/winrt-sdk/src/winrt_sdk")

# main runtime package
################################################################################

set(WINRT_RUNTIME_PATH "${CMAKE_CURRENT_SOURCE_DIR}/winrt-runtime")
set(WINRT_RUNTIME_SOURCES "${WINRT_RUNTIME_PATH}/_winrt.cpp" "${WINRT_RUNTIME_PATH}/_winrt_array.cpp" "${WINRT_RUNTIME_PATH}/runtime.cpp")

Python3_add_library(_winrt MODULE "${WINRT_RUNTIME_SOURCES}")
set_target_properties(_winrt PROPERTIES LIBRARY_OUTPUT_NAME_DEBUG _winrt_d)
target_include_directories(_winrt PRIVATE "${WINRT_SDK_PATH}/cppwinrt" "${WINRT_SDK_PATH}/pywinrt")

install(TARGETS _winrt DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
install(DIRECTORY "${WINRT_RUNTIME_PATH}/src/winrt/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")

# windows sdk packages
################################################################################

if (PYWINRT_FULL_PROJECTION)
    file(GLOB packages CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.*")
    file(GLOB interop_packages CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/interop/winrt-Windows.*")
else()
    # minimum set of packages required for unit tests
    set(packages
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Data.Json"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Devices"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Devices.Geolocation"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Foundation"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Foundation.Collections"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Foundation.Numerics"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Graphics"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Graphics.Capture"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Graphics.DirectX"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Graphics.DirectX.Direct3D11"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Storage"
        "${CMAKE_CURRENT_SOURCE_DIR}/winrt-Windows.Storage.Streams"
    )
    set(interop_packages
        "${CMAKE_CURRENT_SOURCE_DIR}/interop/winrt-Windows.Graphics.Capture.Interop"
    )
endif()

foreach(package ${packages} ${interop_packages})
    get_filename_component(package_name "${package}" NAME)
    string(REPLACE "winrt-" "" package_namespace "${package_name}")
    string(TOLOWER "${package_namespace}" package_namespace_lower)
    string(REPLACE "." "/" package_path_suffix "${package_namespace_lower}")
    string(REPLACE "." "_" package_target_suffix "${package_namespace_lower}")
    set(package_target "_winrt_${package_target_suffix}")
    set(package_sources "${package}/py.${package_namespace}.cpp")

    Python3_add_library(${package_target} MODULE ${package_sources})
    set_target_properties(${package_target} PROPERTIES
        LIBRARY_OUTPUT_NAME_DEBUG ${package_target}_d
        # don't define ${package_target}_EXPORTS symbol to avoid shared precompiled header warning
        DEFINE_SYMBOL ""
    )
    target_include_directories(${package_target} PRIVATE "${WINRT_SDK_PATH}/cppwinrt" "${WINRT_SDK_PATH}/pywinrt")

    # speed up builds by precompiling frequently used headers
    if (package IN_LIST packages)
        if (NOT first_target)
            set(first_target ${package_target})
            target_precompile_headers(${first_target} PRIVATE
                "${WINRT_SDK_PATH}/pywinrt/py.Windows.Foundation.h"
                "${WINRT_SDK_PATH}/pywinrt/py.Windows.Storage.Streams.h"
            )
        else()
            target_precompile_headers(${package_target} REUSE_FROM ${first_target})
        endif()
    endif()

    install(TARGETS ${package_target} DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt/${package_path_suffix}")
    install(DIRECTORY "${package}/winrt/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
endforeach()
