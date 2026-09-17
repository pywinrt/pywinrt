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
string(JSON WINDOWS_APP_SDK_VERSION GET ${TOOLS_JSON} "Microsoft.WindowsAppSDK")
string(JSON WEBVIEW2_VERSION GET ${TOOLS_JSON} "Microsoft.Web.WebView2")
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

find_package(Python3 REQUIRED COMPONENTS Development.Module)

# helper functions
################################################################################

set(WINRT_SDK_PATH "winrt-sdk/src/winrt_sdk")
# The runtime is hand-written, so it lives outside of projection/. Its public
# C++ headers sit inside its Python package, the way numpy and pybind11 ship
# theirs, so that they are found at the same place in a wheel and here.
set(WINRT_RUNTIME_PATH "../runtime")
set(WINRT_RUNTIME_SRC_PATH "${WINRT_RUNTIME_PATH}/src")
set(WINRT_RUNTIME_PYTHON_PATH "${WINRT_RUNTIME_PATH}/python/winrt")
set(WINRT_RUNTIME_INCLUDE_PATH "${WINRT_RUNTIME_PYTHON_PATH}/include")
set(PYWINRT_CHECK_DLL_IMPORTS "${CMAKE_CURRENT_SOURCE_DIR}/check-dll-imports.cmake")
# The interop modules are hand-written too, so they also live outside of
# projection/. Their packaging is still generated, like every other package's.
set(INTEROP_PATH "../interop")
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
    target_include_directories(${target} PRIVATE
        "${WINRT_RUNTIME_INCLUDE_PATH}"
        "${WINRT_SDK_PATH}/cppwinrt"
        "${WINRT_SDK_PATH}/pywinrt"
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

set(WINRT_RUNTIME_SOURCES
    "${WINRT_RUNTIME_SRC_PATH}/_winrt.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/_winrt_array.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/_winrt_box.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/_winrt_buffer.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/async.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/compose.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/errors.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/runtime.cpp"
)
pywinrt_add_library(_winrt "${WINRT_RUNTIME_SOURCES}")
install(TARGETS _winrt DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
install(DIRECTORY "${WINRT_RUNTIME_PYTHON_PATH}/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")

# windows sdk packages
################################################################################

if (PYWINRT_FULL_PROJECTION)
    file(GLOB packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winrt/winrt-*")
    file(GLOB winui2_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winui2/winui2-*")
    file(GLOB winui3_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winui3/winui3-*")
    file(GLOB webview2_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "webview2/webview2-*")
    file(GLOB interop_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "${INTEROP_PATH}/winrt-*")
    file(GLOB winui3_interop_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "${INTEROP_PATH}/winui3-*")
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
    set(interop_packages
        "${INTEROP_PATH}/winrt-Windows.Graphics.Capture.Interop"
        "${INTEROP_PATH}/winrt-Windows.Graphics.DirectX.Direct3D11.Interop"
        "${INTEROP_PATH}/winrt-Windows.Media.Interop"
        "${INTEROP_PATH}/winrt-Windows.System.Interop"
        "${INTEROP_PATH}/winrt-Windows.UI.Composition.Interop"
        "${INTEROP_PATH}/winrt-Windows.UI.Xaml.Hosting.Interop"
    )
    set(winui3_interop_packages
        "${INTEROP_PATH}/winui3-Microsoft.UI.Interop"
        "${INTEROP_PATH}/winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap"
    )
endif()

# Precompiled header groups
################################################################################
#
# The XAML namespaces are by far the most expensive thing to compile, and the
# reason is shared between them: every winrt/impl/Microsoft.UI.Xaml.*.2.h pulls
# in the whole declaration layer of Microsoft.UI.Xaml and
# Microsoft.UI.Xaml.Controls (and likewise for Windows.UI.Xaml), which is over
# ten seconds of parsing repeated in each of the ~25 translation units of the
# group. They get their own precompiled header so that it is parsed once.
#
# A group only earns a precompiled header of its own if it has enough members
# to pay for building it, so with PYWINRT_FULL_PROJECTION=OFF, where only a
# couple of XAML packages are built, everything shares the common one.

function(pywinrt_pch_group package out_var)
    if (package MATCHES "^winrt/winrt-Windows[.]UI[.]Xaml" AND winrt_xaml_package_count GREATER 3)
        set(${out_var} "winrt_xaml" PARENT_SCOPE)
    elseif (package MATCHES "^winui3/winui3-Microsoft[.]UI[.]Xaml" AND winui3_xaml_package_count GREATER 3)
        set(${out_var} "winui3_xaml" PARENT_SCOPE)
    elseif (package MATCHES "^winui2/" AND winui2_package_count GREATER 3)
        set(${out_var} "winui2" PARENT_SCOPE)
    else()
        set(${out_var} "common" PARENT_SCOPE)
    endif()
endfunction()

set(winrt_xaml_packages ${packages})
list(FILTER winrt_xaml_packages INCLUDE REGEX "^winrt/winrt-Windows[.]UI[.]Xaml")
list(LENGTH winrt_xaml_packages winrt_xaml_package_count)

set(winui3_xaml_packages ${winui3_packages})
list(FILTER winui3_xaml_packages INCLUDE REGEX "^winui3/winui3-Microsoft[.]UI[.]Xaml")
list(LENGTH winui3_xaml_packages winui3_xaml_package_count)

list(LENGTH winui2_packages winui2_package_count)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC can't allocate enough memory for more than this
    set(pch_headers_common "${WINRT_SDK_PATH}/pywinrt/py.Windows.Foundation.h")
else()
    set(pch_headers_common
        "${WINRT_SDK_PATH}/pywinrt/py.Windows.Foundation.h"
        "${WINRT_SDK_PATH}/pywinrt/py.Windows.Storage.Streams.h"
    )
endif()

# The runtime module gets a precompiled header of its own rather than sharing
# the common one. Its five translation units all define PYWINRT_RUNTIME_MODULE,
# which selects the implementation side of pywinrt/base.h's runtime API; the other
# packages get the client side, a set of inline forwarders through the
# winrt._winrt capsule that would collide with the real definitions here. So
# the two cannot share a precompiled header, but the runtime still parses the
# same ~6 s of pywinrt/base.h and cppwinrt headers in every one of its translation
# units, and those are rebuilt on exactly the edits the iteration loop is made
# of.
#
# The sources do the #define themselves because the setuptools build (used for
# the wheels) has no other way to set it, but the precompiled header is its own
# translation unit compiled ahead of them, so the target needs the definition
# too. It has to be defined empty to match the #define in the sources,
# otherwise MSVC warns about a macro redefinition and /WX turns that into an
# error.
target_compile_definitions(_winrt PRIVATE "PYWINRT_RUNTIME_MODULE=")
target_precompile_headers(_winrt PRIVATE ${pch_headers_common})

set(pch_headers_winrt_xaml ${pch_headers_common}
    "${WINRT_SDK_PATH}/pywinrt/py.Windows.UI.Xaml.types.h"
    "${WINRT_SDK_PATH}/pywinrt/py.Windows.UI.Xaml.Controls.types.h"
)
set(pch_headers_winui3_xaml ${pch_headers_common}
    "${WINRT_WINDOWS_APP_SDK_PATH}/pywinrt/py.Microsoft.UI.Xaml.types.h"
    "${WINRT_WINDOWS_APP_SDK_PATH}/pywinrt/py.Microsoft.UI.Xaml.Controls.types.h"
)
# WinUI 2 has no Microsoft.UI.Xaml namespace of its own: its controls derive
# from the Windows SDK's Windows.UI.Xaml, which is where its cost is.
set(pch_headers_winui2 ${pch_headers_common}
    "${WINRT_SDK_PATH}/pywinrt/py.Windows.UI.Xaml.types.h"
    "${WINRT_SDK_PATH}/pywinrt/py.Windows.UI.Xaml.Controls.types.h"
    "${WINRT_MICROSOFT_UI_XAML_PATH}/pywinrt/py.Microsoft.UI.Xaml.Controls.types.h"
)

# Build order
################################################################################
#
# Ninja starts the jobs that are ready in the order their edges appear in
# build.ninja, which is the order in which the targets are created here. Its
# critical path scheduler does not reorder them: given a complete .ninja_log
# from a previous build, both the 1.12.1 that Visual Studio bundles and 1.13.2
# still run the ready edges in declaration order.
#
# That decides the wall time of the full projection, because none of its ~430
# translation units waits for anything but its group's precompiled header, and
# those are all finished within the first 16 seconds. Every remaining job is
# runnable from then on, so this order *is* the schedule, and the build ends
# when the longest of the jobs that were started last ends. Compiling
# py.Microsoft.UI.Xaml.Controls.cpp alone takes 162 s of a 365 s Debug build,
# and it used to be picked almost last, starting 201 s in with the other seven
# cores draining behind it.
#
# So start the most expensive translation units first. Source size stands in
# for compile time, which it predicts well at the end that matters: the two
# XAML Controls namespaces are the two largest generated sources by a wide
# margin, and the two longest compiles by the same margin.

function(pywinrt_sort_packages_by_size out_var)
    set(keyed_packages "")

    foreach(package ${ARGN})
        # one source per package, or two where it is split in half to break a
        # circular dependency
        file(GLOB package_sources "${CMAKE_CURRENT_SOURCE_DIR}/${package}/py.*.cpp")

        set(size 0)
        foreach(source ${package_sources})
            file(SIZE "${source}" source_size)
            math(EXPR size "${size} + ${source_size}")
        endforeach()

        # zero-pad the key so that a plain string sort orders by size, and keep
        # the package name in it so that equal sizes still sort deterministically
        string(LENGTH "${size}" size_length)
        math(EXPR padding_length "12 - ${size_length}")
        string(REPEAT "0" ${padding_length} padding)

        list(APPEND keyed_packages "${padding}${size} ${package}")
    endforeach()

    list(SORT keyed_packages ORDER DESCENDING)
    list(TRANSFORM keyed_packages REPLACE "^[0-9]+ " "")

    set(${out_var} "${keyed_packages}" PARENT_SCOPE)
endfunction()

pywinrt_sort_packages_by_size(sorted_packages
    ${packages} ${winui2_packages} ${winui3_packages} ${webview2_packages}
    ${interop_packages} ${winui3_interop_packages}
)

# namespace packages
################################################################################

foreach(package ${sorted_packages})
    pywinrt_add_namespace_package(${package})

    # speed up builds by precompiling frequently used headers
    if (NOT package IN_LIST winui3_interop_packages)
        pywinrt_pch_group("${package}" pch_group)

        if (NOT DEFINED pch_root_${pch_group})
            set(pch_root_${pch_group} ${package_target})
            target_precompile_headers(${package_target} PRIVATE ${pch_headers_${pch_group}})
        else()
            target_precompile_headers(${package_target} REUSE_FROM ${pch_root_${pch_group}})
        endif()

        if (${package} IN_LIST circular_dependency_packages)
            target_precompile_headers(${package_target}_2 REUSE_FROM ${pch_root_${pch_group}})
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

    if (${package} STREQUAL "${INTEROP_PATH}/winrt-Windows.Graphics.DirectX.Direct3D11.Interop")
        target_link_libraries(${package_target} PRIVATE "D3D11")
    endif()

    if (${package} STREQUAL "${INTEROP_PATH}/winrt-Windows.System.Interop")
        target_link_libraries(${package_target} PRIVATE "CoreMessaging")
    endif()

    if (${package} STREQUAL "${INTEROP_PATH}/winui3-Microsoft.UI.Interop")
        if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            # winui3/Microsoft.UI.Interop.h has stuff that other compilers don't like
            target_compile_options(${package_target} PRIVATE -Wno-unknown-pragmas)
        endif()
    endif()

    if (${package} STREQUAL "${INTEROP_PATH}/winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap")
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
target_precompile_headers(${package_target} REUSE_FROM ${pch_root_common})
set_target_properties(${package_target} PROPERTIES DEFINE_SYMBOL "")
install(FILES "../_tools/PyWinRT.TestWinRT.${TEST_WINRT_VERSION}/runtimes/win10-${WINDOWS_APP_SDK_ARCH}/native/TestComponent.dll"
    DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/test_winrt/testcomponent/")
