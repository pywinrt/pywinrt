set(PYWINRT_CHECK_DLL_IMPORTS "${CMAKE_CURRENT_LIST_DIR}/check-dll-imports.cmake")

# pywinrt_add_module(<target> <sources> [USE_SABI <version>])
#
# USE_SABI compiles the module against the stable ABI of that Python version,
# as Python3_add_library() does.
function(pywinrt_add_module target sources)
    cmake_parse_arguments(PARSE_ARGV 2 arg "" "USE_SABI" "")

    if (arg_USE_SABI)
        Python3_add_library(${target} MODULE USE_SABI ${arg_USE_SABI} "${sources}")
    else()
        Python3_add_library(${target} MODULE "${sources}")
    endif()

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
