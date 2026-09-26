# main runtime package
################################################################################

# The C++ headers are beside the sources rather than in the Python package,
# since nothing outside the runtime compiles against them.
set(WINRT_RUNTIME_SRC_PATH "src")
set(WINRT_RUNTIME_PYTHON_PATH "python/winrt")
set(WINRT_RUNTIME_INCLUDE_PATH "${WINRT_RUNTIME_SRC_PATH}/include")
# The C++/WinRT headers scripts/generate-cppwinrt.py writes. The runtime
# carries what pywinrt/base.h includes; an interop module carries the whole
# closure of the namespaces it includes, in a cppwinrt directory of its own.
set(CPPWINRT_PATH "${WINRT_RUNTIME_INCLUDE_PATH}/cppwinrt")

set(WINRT_RUNTIME_SOURCES
    "${WINRT_RUNTIME_SRC_PATH}/_winrt.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/_winrt_array.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/_winrt_box.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/_winrt_buffer.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/arrays.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/async.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/callbacks.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/collections.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/compose.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/delegates.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/enums.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/errors.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/generics.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/implements.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/interp.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/members.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/numerics-statics.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/numerics.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/objects.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/protocols.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/pycollections.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/pymapping.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/pysequence.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/runtime.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/shapes.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/structs.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/table.cpp"
    "${WINRT_RUNTIME_SRC_PATH}/types.cpp"
)
pywinrt_add_module(_winrt "${WINRT_RUNTIME_SOURCES}")
target_include_directories(_winrt PRIVATE
    "${WINRT_RUNTIME_INCLUDE_PATH}"
    "${CPPWINRT_PATH}"
)

# Every translation unit of the runtime parses the same several seconds of
# pywinrt/base.h and the C++/WinRT headers under it, and those are rebuilt on
# exactly the edits the iteration loop is made of.
target_precompile_headers(_winrt PRIVATE "${WINRT_RUNTIME_INCLUDE_PATH}/pywinrt/base.h")

install(TARGETS _winrt DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
install(DIRECTORY "${WINRT_RUNTIME_PYTHON_PATH}/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
# test_table.py and test_table_text.py read the compiler the same way the
# build does, so it is installed as well
install(DIRECTORY "${WINRT_TABLE_PATH}/winrt/" DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
