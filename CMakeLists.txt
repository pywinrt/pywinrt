cmake_minimum_required(VERSION 3.15)
project(pywinrt)

set(PYWINRT_SCRIPTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/scripts")

if(NOT DEFINED PYWINRT_BUILD_VERSION)
set(PYWINRT_BUILD_VERSION "0.0.0")
endif()

function(ADD_OBJECT_DEPENDS file dependency)
    get_source_file_property(cur_obj_depends ${file} OBJECT_DEPENDS)
    if (${cur_obj_depends} STREQUAL "NOTFOUND")
        set(new_obj_depends "${pch_output_file_name}")
    else()
        set(new_obj_depends "${cur_obj_depends};${dependency}")
    endif()
    set_source_files_properties(${file}  PROPERTIES OBJECT_DEPENDS ${new_obj_depends})
endfunction(ADD_OBJECT_DEPENDS)

function(TARGET_CONFIG_MSVC_PCH target pch_cpp pch_header)

    get_target_property(target_name ${target} NAME)
    set(pch_output_file_name "${target_name}.pch")
    set(pch_output_file "\"${CMAKE_CURRENT_BINARY_DIR}/${pch_output_file_name}\"")

    get_target_property(sources ${target} SOURCES)
    foreach(file ${sources})
        if (${file} STREQUAL ${pch_cpp})
            set_source_files_properties(${file}
                PROPERTIES
                COMPILE_FLAGS " /Yc${pch_header} /Fp${pch_output_file}"
                OBJECT_OUTPUTS ${pch_output_file_name})
        else()
            set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS " /Yu${pch_header} /Fp${pch_output_file}")
            ADD_OBJECT_DEPENDS(${file} ${pch_output_file_name})
        endif()
    endforeach()

    set_target_properties(${target} PROPERTIES PCH_OUTPUT_FILE ${pch_output_file})
    set_property(GLOBAL APPEND PROPERTY PCH_OUTPUT_FILES ${pch_output_file})

endfunction(TARGET_CONFIG_MSVC_PCH)

function(GENERATE_STRING_LITERAL_FILES glob_expression output_filename namespace target)

    file(GLOB input_files CONFIGURE_DEPENDS "${glob_expression}")

    set(out_h   "${PROJECT_BINARY_DIR}/${output_filename}.h")
    set(out_cpp "${PROJECT_BINARY_DIR}/${output_filename}.cpp")
    set(output_files ${out_h} ${out_cpp})

    add_custom_command(
        OUTPUT  ${output_files}
        DEPENDS ${input_files}
        COMMAND ${CMAKE_COMMAND} \"-Dglob_expression=${glob_expression}\" \"-Dout_h=${out_h}\" \"-Dout_cpp=${out_cpp}\" \"-Dnamespace=${namespace}\" -P \"${PYWINRT_SCRIPTS_PATH}/genStringLiteralFiles.cmake\"
    )

    set(gen_target_name "generated_${target}_${output_filename}")
    add_custom_target(${gen_target_name} DEPENDS ${output_files})
    add_dependencies(${target} ${gen_target_name})

endfunction()

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

add_subdirectory(src)
