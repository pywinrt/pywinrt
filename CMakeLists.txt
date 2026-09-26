# 4.4.1 is where FindPython learned to manage the Windows pydebug ABI flag, so
# Python3_add_library() names a Debug module <name>_d.pyd - what the debug
# interpreter looks for - without us doing it by hand. It is also the version
# every toolchain the project builds on already has.
cmake_minimum_required(VERSION 4.4.1)
cmake_policy(SET CMP0091 NEW)
project(pywinrt LANGUAGES CXX)

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
file(READ ".config/_tools.json" TOOLS_JSON)
string(JSON WEBVIEW2_VERSION GET ${TOOLS_JSON} "Microsoft.Web.WebView2")
# Only the two components the interop modules compile against: the App SDK
# is a metapackage and nothing else here is built from it.
string(JSON WASDK_FOUNDATION_VERSION GET ${TOOLS_JSON} "Microsoft.WindowsAppSDK.Foundation")
string(JSON WASDK_INTERACTIVE_EXPERIENCES_VERSION GET ${TOOLS_JSON} "Microsoft.WindowsAppSDK.InteractiveExperiences")
string(JSON WASDK_RUNTIME_VERSION GET ${TOOLS_JSON} "Microsoft.WindowsAppSDK.Runtime")
string(JSON TEST_WINRT_VERSION GET ${TOOLS_JSON} "PyWinRT.TestWinRT")

# where scripts/fetch-tools.ps1 puts those packages
set(PYWINRT_TOOLS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/_tools")

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

# The table compiler is a package of its own, because it is pure Python and a
# projection package should not need a compiler to be built. The runtime's tests
# and the projection's build both use it.
set(WINRT_TABLE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/table")

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

include(cmake/add-module.cmake)

# the hand-written runtime, the hand-written interop modules, and the generated
# projection packages with the test component
add_subdirectory(runtime)
add_subdirectory(interop)
add_subdirectory(projection)
