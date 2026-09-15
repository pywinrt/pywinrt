# Fails the build if a module imports the C++ runtime DLL.
#
# Python ships the C runtime next to python3xx.dll and the UCRT is an operating
# system component, but the C++ standard library is neither, so a module that
# imports MSVCP140.dll fails to load on any machine where the Visual C++
# redistributable was never installed. No wheel carries a copy of it any more -
# pywinrt/throw_helpers.h is what keeps the import out.
#
# This is worth checking mechanically because the dependency appears in no build
# file. It arrives through an #include and a use of a standard container that
# grows, so nothing in the CMakeLists or in the sources looks any different, and
# the first sign of it would be a user's ImportError. A newly *linked* library
# is not like that: it is right there in target_link_libraries, so it needs no
# guard and there is no allowlist here to keep up to date.
#
# Run as:
#   cmake -D LINKER=<link.exe> -D CONFIG=<config> -D MODULE=<module.pyd>
#        -P check-dll-imports.cmake

# A debug build links the debug CRT, so it imports MSVCP140D.dll however this
# turns out - along with ucrtbased.dll and VCRUNTIME140D.dll, which are not
# redistributable either. Only what ships has to satisfy this, and debug builds
# do not ship, so checking one would only break the build CONTRIBUTING.md tells
# people to use.
if (CONFIG STREQUAL "Debug")
    return()
endif()

execute_process(
    COMMAND "${LINKER}" -dump -nologo -dependents "${MODULE}"
    OUTPUT_VARIABLE dependents
    ERROR_VARIABLE error
    RESULT_VARIABLE result
)

if (NOT result EQUAL 0)
    message(FATAL_ERROR "could not read the imports of ${MODULE}: ${error}")
endif()

string(REGEX MATCHALL "[A-Za-z0-9_.+-]+\\.[Dd][Ll][Ll]" imports "${dependents}")

foreach (import IN LISTS imports)
    string(TOLOWER "${import}" lower_import)

    if (lower_import MATCHES "^msvcp")
        message(FATAL_ERROR
            "${MODULE} imports ${import}.\n"
            "Python does not ship the C++ runtime, so this module would fail to "
            "load wherever the Visual C++ redistributable is not installed. "
            "Something here now uses a part of the standard library that is not "
            "header-only - see pywinrt/throw_helpers.h, which covers the parts "
            "the projection used to need."
        )
    endif()
endforeach()
