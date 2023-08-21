cmake_minimum_required(VERSION 3.15)
project(pywinrt)

if(NOT DEFINED PYWINRT_BUILD_VERSION)
set(PYWINRT_BUILD_VERSION "0.0.0")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# change the warning level to 4
string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Always generate symbols for release builds
string(APPEND CMAKE_CXX_FLAGS_RELEASE " /Zi")
string(APPEND CMAKE_SHARED_LINKER_FLAGS_RELEASE " /DEBUG /OPT:REF /OPT:ICF /MAP")
string(APPEND CMAKE_EXE_LINKER_FLAGS_RELEASE " /DEBUG /OPT:REF /OPT:ICF /MAP")

add_compile_options(/permissive-)

# Explicitly configure _DEBUG preprocessor macro
string(APPEND CMAKE_CXX_FLAGS_DEBUG " /D_DEBUG")

add_definitions(-DNOMINMAX)

add_executable(pywinrt)
target_sources(pywinrt PUBLIC main.cpp)
target_compile_definitions(pywinrt PUBLIC "PYWINRT_VERSION_STRING=\"${PYWINRT_BUILD_VERSION}\"")

find_program(NUGET_EXE NAMES nuget REQUIRED)
execute_process(COMMAND ${NUGET_EXE} install "Microsoft.Windows.WinMD" -Version 1.0.210629.2 -ExcludeVersion -OutputDirectory "${CMAKE_BINARY_DIR}/_packages")
target_include_directories(pywinrt PRIVATE "${CMAKE_BINARY_DIR}/_packages/Microsoft.Windows.WinMD")

target_link_libraries(pywinrt windowsapp ole32 shlwapi)

set_target_properties(pywinrt PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
