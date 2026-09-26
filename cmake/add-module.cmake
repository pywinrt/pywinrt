set(PYWINRT_CHECK_DLL_IMPORTS "${CMAKE_CURRENT_LIST_DIR}/check-dll-imports.cmake")

function(pywinrt_add_module target sources)
    Python3_add_library(${target} MODULE "${sources}")

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
