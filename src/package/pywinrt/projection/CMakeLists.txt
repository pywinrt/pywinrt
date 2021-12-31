cmake_minimum_required(VERSION 3.16)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/pywinrt/winrt/src/*.cpp")
foreach(file ${sources})
    message(${file})
endforeach()

file(GLOB headers RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "cppwinrt/*.h"
    "cppwinrt/impl/*.h"
    "pywinrt/winrt/src/*.h"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO: re-enable /GL and /LTCG when we get a bigger build machine
add_compile_options(/await /bigobj /GR- /permissive-)

project(_winrt)

# strip suffix to allow support for prerelease version
string(REGEX MATCH "^[0-9]\\.[0-9]+(\\.[0-9]+)?" PYTHON_BASE_VERSION ${PYTHON_VERSION})

find_package (Python3 ${PYTHON_BASE_VERSION} EXACT COMPONENTS Interpreter Development)

Python3_add_library (_winrt MODULE ${sources})
set_target_properties(_winrt PROPERTIES LIBRARY_OUTPUT_NAME_DEBUG _winrt_d)
target_precompile_headers(_winrt PRIVATE ${headers})
target_include_directories(_winrt PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/cppwinrt" "${CMAKE_CURRENT_SOURCE_DIR}/pywinrt/winrt/src")
target_link_libraries(_winrt PRIVATE onecore)

if($ENV{CI})
    # CI has limited resources (runs out of heap space), so we limit the number
    # of concurrent processes to combat this
    set_property(GLOBAL PROPERTY JOB_POOLS compile_job=2)
    set_property(TARGET _winrt PROPERTY JOB_POOL_COMPILE compile_job)
endif()

string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
foreach(build_type RELEASE MINSIZEREL RELWITHDEBINFO)
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_${build_type} "${CMAKE_CXX_FLAGS_${build_type}}")
endforeach()

string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# TODO: re-enable /GL and /LTCG when we get a bigger build machine
# string(APPEND CMAKE_MODULE_LINKER_FLAGS " /LTCG:STATUS")

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.20.27404.0")
    string(APPEND CMAKE_CXX_FLAGS " /d2FH4")
endif()

install(TARGETS _winrt DESTINATION ".")
