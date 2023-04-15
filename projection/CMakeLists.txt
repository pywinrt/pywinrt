cmake_minimum_required(VERSION 3.24)
cmake_policy(SET CMP0091 NEW)
project(_winrt LANGUAGES CXX)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

file(GLOB sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pywinrt/winrt/src/*.cpp")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    # TODO: re-enable /GL and /LTCG when we get a bigger build machine
    add_compile_options(/bigobj /GR- /permissive- /Gy)
else()
    add_compile_options(-Werror -D_WIN32_WINNT=_WIN32_WINNT_WIN10)
endif()

# strip suffix to allow support for prerelease version
string(REGEX MATCH "^[0-9]\\.[0-9]+(\\.[0-9]+)?" PYTHON_BASE_VERSION ${PYTHON_VERSION})

# work around https://gitlab.kitware.com/cmake/cmake/-/issues/24587
if(NOT CMAKE_GENERATOR_PLATFORM AND CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64" OR CMAKE_GENERATOR_PLATFORM MATCHES "ARM64")
    set(Python3_ROOT_DIR
        [HKEY_CURRENT_USER/SOFTWARE/Python/PythonCore/${PYTHON_BASE_VERSION}-arm64/InstallPath]
        [HKEY_LOCAL_MACHINE/SOFTWARE/Python/PythonCore/${PYTHON_BASE_VERSION}-arm64/InstallPath]
    )
endif()

find_package(Python3 ${PYTHON_BASE_VERSION} EXACT REQUIRED COMPONENTS Development.Module)

Python3_add_library (_winrt MODULE ${sources})
set_target_properties(_winrt PROPERTIES LIBRARY_OUTPUT_NAME_DEBUG _winrt_d)
target_include_directories(_winrt PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/cppwinrt" "${CMAKE_CURRENT_SOURCE_DIR}/pywinrt/winrt/src")

# speed up builds by precompiling commonly used headers
target_precompile_headers(_winrt PRIVATE
    "pywinrt/winrt/src/py.Windows.Foundation.h"
    "pywinrt/winrt/src/py.Windows.Storage.Streams.h"
)
# needed because including windows.h affects winrt
set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/pywinrt/winrt/src/runtime.cpp" PROPERTY SKIP_PRECOMPILE_HEADERS ON)
# needed because including Shobjidl.h affects winrt
set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/pywinrt/winrt/src/_winrt.cpp" PROPERTY SKIP_PRECOMPILE_HEADERS ON)
# needed because including unknwn.h affects winrt
set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/pywinrt/winrt/src/py.Windows.Graphics.Capture.Interop.cpp" PROPERTY SKIP_PRECOMPILE_HEADERS ON)

install(TARGETS _winrt DESTINATION ".")
