project(pywinrt)

add_executable(pywinrt)
target_sources(pywinrt PUBLIC main.cpp pch.cpp "${PROJECT_BINARY_DIR}/strings.cpp")
target_include_directories(pywinrt PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})
target_compile_definitions(pywinrt PUBLIC "XLANG_VERSION_STRING=\"${XLANG_BUILD_VERSION}\"")

find_program(NUGET_EXE NAMES nuget)
exec_program(${NUGET_EXE} ARGS install "Microsoft.Windows.WinMD" -Version "1.0.210629.2" -ExcludeVersion -OutputDirectory "${CMAKE_BINARY_DIR}/_packages")
target_include_directories(pywinrt PRIVATE "${CMAKE_BINARY_DIR}/_packages/Microsoft.Windows.WinMD")

GENERATE_STRING_LITERAL_FILES("${PROJECT_SOURCE_DIR}/strings/*" "strings" "pywinrt::strings" pywinrt)

TARGET_CONFIG_MSVC_PCH(pywinrt pch.cpp pch.h)
target_compile_options(pywinrt PUBLIC /await)
target_link_libraries(pywinrt windowsapp ole32 shlwapi)

set_target_properties(pywinrt PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
