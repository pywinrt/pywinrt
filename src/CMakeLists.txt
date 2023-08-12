cmake_minimum_required(VERSION 3.15)
project(pywinrt)

add_executable(pywinrt)
target_sources(pywinrt PUBLIC main.cpp)
target_compile_definitions(pywinrt PUBLIC "PYWINRT_VERSION_STRING=\"${PYWINRT_BUILD_VERSION}\"")

find_program(NUGET_EXE NAMES nuget REQUIRED)
execute_process(COMMAND ${NUGET_EXE} install "Microsoft.Windows.WinMD" -Version 1.0.210629.2 -ExcludeVersion -OutputDirectory "${CMAKE_BINARY_DIR}/_packages")
target_include_directories(pywinrt PRIVATE "${CMAKE_BINARY_DIR}/_packages/Microsoft.Windows.WinMD")

target_link_libraries(pywinrt windowsapp ole32 shlwapi)

set_target_properties(pywinrt PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
