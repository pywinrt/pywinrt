# 4.4.1 is where FindPython learned to manage the Windows pydebug ABI flag, so
# Python3_add_library() names a Debug module <name>_d.pyd - what the debug
# interpreter looks for - without us doing it by hand. It is also the version
# every toolchain the project builds on already has.
cmake_minimum_required(VERSION 4.4.1)
cmake_policy(SET CMP0091 NEW)
project(pywinrt-projection LANGUAGES CXX)

# Embedded (/Z7) debug info instead of /Zi: the compiler no longer has to
# serialize through mspdbsrv, the precompiled header is not tied to a .pdb (so
# CMake does not have to copy it for every target that reuses the PCH), and it
# roughly halves the time to build the PCH. Also required for compiler caches.
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>")

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
string(JSON WEBVIEW2_VERSION GET ${TOOLS_JSON} "Microsoft.Web.WebView2")
string(JSON WINDOWS_APP_SDK_VERSION GET ${TOOLS_JSON} "Microsoft.WindowsAppSDK")
string(JSON TEST_WINRT_VERSION GET ${TOOLS_JSON} "PyWinRT.TestWinRT")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
link_libraries("RuntimeObject.lib")

# /GR- (-fno-rtti): nothing in the projection uses dynamic_cast or typeid, but
# the RTTI data MSVC emits anyway is about 15% of the size of a module, most of
# it type descriptors in writable .data.
if (MSVC)
    add_compile_options(/bigobj /permissive- /W4 /WX /Gy /GR-)
else()
    add_compile_options(-Werror -Wall -Wextra -fno-rtti)

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

# Link time optimization, in the configuration that is timed and shipped. The
# wheels already have it - setuptools compiles every extension with /O2 /GL and
# links with /LTCG - so a Release build without it is not what users run, and it
# is what lets the runtime be laid out one subject per file: the optimizer sees
# across translation units at link time, so a helper does not have to live in a
# header to be inlined into its caller.
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

# The interpreter compiles the projection tables from their text form; the
# module component builds the runtime against the Python it belongs to.
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)

# helper functions
################################################################################

# The runtime is hand-written, so it lives outside of projection/. Its public
# C++ headers sit inside its Python package, the way numpy and pybind11 ship
# theirs, so that they are found at the same place in a wheel and here.
set(WINRT_RUNTIME_PATH "../runtime")
# The table compiler is a package of its own, because it is pure Python and a
# projection package should not need a compiler to be built.
set(WINRT_TABLE_PATH "../table")
set(WINRT_RUNTIME_SRC_PATH "${WINRT_RUNTIME_PATH}/src")
set(WINRT_RUNTIME_PYTHON_PATH "${WINRT_RUNTIME_PATH}/python/winrt")
set(WINRT_RUNTIME_INCLUDE_PATH "${WINRT_RUNTIME_PYTHON_PATH}/include")
# The C++/WinRT headers scripts/generate-cppwinrt.py writes. The runtime
# carries what pywinrt/base.h includes, which is what everything compiled
# against the PyWinRT headers gets; an interop module carries the namespaces it
# includes beyond those, in a cppwinrt directory of its own.
set(CPPWINRT_PATH "${WINRT_RUNTIME_INCLUDE_PATH}/cppwinrt")
set(PYWINRT_CHECK_DLL_IMPORTS "${CMAKE_CURRENT_SOURCE_DIR}/check-dll-imports.cmake")
set(WEBVIEW2_PATH "../_tools/Microsoft.Web.WebView2.${WEBVIEW2_VERSION}")
set(WINDOWS_APP_SDK_PATH "../_tools/Microsoft.WindowsAppSDK.${WINDOWS_APP_SDK_VERSION}")
set(PYWINRT_INTEROP_PATH "../interop")

if (MSVC)
    string(TOLOWER ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID} PYWINRT_TARGET_ARCH)
else()
    if (CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES "^i\\d86$")
        set(PYWINRT_TARGET_ARCH "x86")
    elseif (CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "x86_64")
        set(PYWINRT_TARGET_ARCH "x64")
    elseif (CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "aarch64")
        set(PYWINRT_TARGET_ARCH "arm64")
    else()
        message(FATAL_ERROR "Unsupported compiler architecture: ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
    endif()
endif()
message(STATUS "Target architecture: ${PYWINRT_TARGET_ARCH}")

function(pywinrt_add_library target sources)
    Python3_add_library(${target} MODULE "${sources}")
    target_include_directories(${target} PRIVATE
        "${WINRT_RUNTIME_INCLUDE_PATH}"
        "${CPPWINRT_PATH}"
    )

    if (MSVC)
        target_link_options(${target} PRIVATE $<$<CONFIG:RelWithDebInfo>:/PROFILE /INCREMENTAL:NO>)

        # A module must not end up needing the C++ runtime DLL, which nothing
        # ships any more. Checking after each link puts that in front of
        # whoever wrote the line rather than in front of a user.
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}"
                -D "LINKER=${CMAKE_LINKER}"
                -D "CONFIG=$<CONFIG>"
                -D "MODULE=$<TARGET_FILE:${target}>"
                -P "${PYWINRT_CHECK_DLL_IMPORTS}"
            VERBATIM
        )
    endif()
endfunction()

# main runtime package
################################################################################

set(WINRT_RUNTIME_SOURCES
    "${WINRT_RUNTIME_SRC_PATH}/_winrt.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/_winrt_array.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/_winrt_box.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/_winrt_buffer.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/arrays.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/async.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/callbacks.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/collections.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/compose.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/delegates.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/enums.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/errors.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/generics.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/implements.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/interp.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/members.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/numerics-statics.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/numerics.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/objects.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/protocols.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/pycollections.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/pymapping.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/pysequence.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/runtime.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/shapes.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/structs.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/table.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/types.cpp"
)
pywinrt_add_library(_winrt "${WINRT_RUNTIME_SOURCES}")

# The sources do the #define themselves because the setuptools build (used for
# the wheels) has no other way to set it, but the precompiled header is its own
# translation unit compiled ahead of them, so the target needs the definition
# too. It has to be defined empty to match the #define in the sources,
# otherwise MSVC warns about a macro redefinition and /WX turns that into an
# error.
target_compile_definitions(_winrt PRIVATE "PYWINRT_RUNTIME_MODULE=")
# Every translation unit of the runtime parses the same several seconds of
# pywinrt/base.h and the C++/WinRT headers under it, and those are rebuilt on
# exactly the edits the iteration loop is made of.
target_precompile_headers(_winrt PRIVATE "${WINRT_RUNTIME_INCLUDE_PATH}/pywinrt/base.h")

install(TARGETS _winrt DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
install(DIRECTORY "${WINRT_RUNTIME_PYTHON_PATH}/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
# test_table.py and test_table_text.py read the compiler the same way the
# build does, so it is installed as well
install(DIRECTORY "${WINRT_TABLE_PATH}/winrt/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")

# interop modules
################################################################################
#
# The desktop interop APIs are the only compiled part of the projection that is
# left: each of them calls a plain COM interface or a flat C function that is
# not in any winmd, so there is nothing for a table to describe. They are
# hand-written and they name the WinRT types they hand over rather than sharing
# generated code with the packages that project them.

function(pywinrt_add_interop_module target package source root)
    set(package_path "${PYWINRT_INTEROP_PATH}/${package}")

    pywinrt_add_library(${target} "${package_path}/${source}")
    target_include_directories(${target} PRIVATE "${package_path}/cppwinrt")
    install(TARGETS ${target} DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/${root}")
    install(DIRECTORY "${package_path}/${root}/"
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
# Windows.
set(WINDOWS_APP_SDK_LIB_PATH "${WINDOWS_APP_SDK_PATH}/lib/win10-${PYWINRT_TARGET_ARCH}")

pywinrt_add_interop_module(_winui3_microsoft_ui_interop
    "winui3-Microsoft.UI.Interop" "py.Microsoft.UI.Interop.cpp" "winui3")
target_include_directories(_winui3_microsoft_ui_interop PRIVATE
    "${WINDOWS_APP_SDK_PATH}/include")
if (NOT MSVC)
    # the App SDK's Microsoft.UI.Interop.h uses MSVC-only pragmas
    target_compile_options(_winui3_microsoft_ui_interop PRIVATE -Wno-unknown-pragmas)
endif()

pywinrt_add_interop_module(_winui3_microsoft_windows_applicationmodel_dynamicdependency_bootstrap
    "winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap"
    "py.Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap.cpp" "winui3")
target_include_directories(_winui3_microsoft_windows_applicationmodel_dynamicdependency_bootstrap PRIVATE
    "${WINDOWS_APP_SDK_PATH}/include")
target_link_directories(_winui3_microsoft_windows_applicationmodel_dynamicdependency_bootstrap PRIVATE
    "${WINDOWS_APP_SDK_LIB_PATH}")
target_link_libraries(_winui3_microsoft_windows_applicationmodel_dynamicdependency_bootstrap PRIVATE
    "Microsoft.WindowsAppRuntime.Bootstrap.lib")
# The bootstrapper is what finds the Windows App Runtime, so it is the one App
# SDK binary that ships with the package rather than being found through it.
install(FILES "${WINDOWS_APP_SDK_PATH}/runtimes/win-${PYWINRT_TARGET_ARCH}/native/Microsoft.WindowsAppRuntime.Bootstrap.dll"
    DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winui3")

# projection packages
################################################################################
#
# A projection package is data now: a table per namespace plus the __init__.py
# and the type stubs beside it. Nothing in one is compiled, so all there is to
# do is compile the table and put both where the tests will import them from.
#
# The table is carried as text and the runtime reads a binary, so compiling one
# into the other is a build step: winrt/table.py, which winrt-table-compiler
# ships for whatever builds a projection. Nothing is written back into the
# source tree, which is what keeps a generated binary out of the repository.

set(PYWINRT_TABLE_DIR "${CMAKE_CURRENT_BINARY_DIR}/tables")
set(PYWINRT_TABLE_COMPILER "${WINRT_TABLE_PATH}/winrt/table/__init__.py")

function(pywinrt_install_namespace_package package)
    # package_name: e.g. winrt-Windows.Foundation, test-winrt-TestComponent
    get_filename_component(package_name "${package}" NAME)

    # split package name on last dash
    string(FIND "${package_name}" "-" index REVERSE)

    # package_root: e.g. winrt, test-winrt
    string(SUBSTRING "${package_name}" 0 ${index} package_root)

    # package_root_dir: e.g. winrt, test_winrt
    string(REPLACE "-" "_" package_root_dir "${package_root}")

    install(DIRECTORY "${package}/${package_root_dir}/"
        DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/${package_root_dir}"
        PATTERN "_table.pywinrt.txt" EXCLUDE
    )

    set(package_dir "${CMAKE_CURRENT_SOURCE_DIR}/${package}/${package_root_dir}")

    file(GLOB_RECURSE tables RELATIVE "${package_dir}" CONFIGURE_DEPENDS
        "${package_dir}/*/_table.pywinrt.txt")

    foreach(table ${tables})
        # ns_dir: the namespace's own folder, e.g. windows/foundation
        get_filename_component(ns_dir "${table}" DIRECTORY)

        set(binary "${PYWINRT_TABLE_DIR}/${package_root_dir}/${ns_dir}/_table.pywinrt")

        add_custom_command(
            OUTPUT "${binary}"
            COMMAND "${CMAKE_COMMAND}" -E env
                "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/${WINRT_TABLE_PATH}"
                "${Python3_EXECUTABLE}" -m winrt.table "${package_dir}/${table}" "${binary}"
            DEPENDS "${package_dir}/${table}" "${PYWINRT_TABLE_COMPILER}"
            COMMENT "Compiling ${package_root_dir}/${ns_dir}/_table.pywinrt"
            VERBATIM
        )

        install(FILES "${binary}"
            DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/${package_root_dir}/${ns_dir}")

        list(APPEND PYWINRT_TABLES "${binary}")
    endforeach()

    set(PYWINRT_TABLES "${PYWINRT_TABLES}" PARENT_SCOPE)
endfunction()

if (PYWINRT_FULL_PROJECTION)
    file(GLOB packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winrt/winrt-*")
    file(GLOB winui2_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winui2/winui2-*")
    file(GLOB winui3_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winui3/winui3-*")
    file(GLOB webview2_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "webview2/webview2-*")
else()
    # minimum set of packages required for unit tests
    set(packages
        "winrt/winrt-Windows.ApplicationModel.Core"
        "winrt/winrt-Windows.Data.Json"
        "winrt/winrt-Windows.Devices"
        "winrt/winrt-Windows.Devices.Geolocation"
        "winrt/winrt-Windows.Foundation"
        "winrt/winrt-Windows.Foundation.Collections"
        "winrt/winrt-Windows.Foundation.Metadata"
        "winrt/winrt-Windows.Foundation.Numerics"
        "winrt/winrt-Windows.Globalization.NumberFormatting"
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
endif()

foreach(package ${packages} ${winui2_packages} ${winui3_packages} ${webview2_packages})
    pywinrt_install_namespace_package(${package})
endforeach()

if (webview2_packages)
    install(FILES "${WEBVIEW2_PATH}/runtimes/win-${PYWINRT_TARGET_ARCH}/native_uap/Microsoft.Web.WebView2.Core.dll"
        DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/webview2/microsoft/web/webview2/core")
endif()

# test component package
################################################################################

pywinrt_install_namespace_package("test-winrt/test-winrt-TestComponent")
install(FILES "../_tools/PyWinRT.TestWinRT.${TEST_WINRT_VERSION}/runtimes/win10-${PYWINRT_TARGET_ARCH}/native/TestComponent.dll"
    DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/test_winrt/testcomponent/")

# the tables of every package that is being installed, which nothing links
# against and so nothing else would ask to be built
add_custom_target(pywinrt-tables ALL DEPENDS ${PYWINRT_TABLES})
