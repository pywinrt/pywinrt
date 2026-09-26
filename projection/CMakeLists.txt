# projection packages
################################################################################
#
# A projection package is data now: a table per namespace plus the __init__.py
# and the type stubs beside it. Nothing in one is compiled, so all there is to
# do is compile the table and put both where the tests will import them from.
#
# The table is carried as text and the runtime reads a binary, so compiling one
# into the other is a build step: winrt/table.py, which winrt-table-compiler
# ships for whatever builds a projection. Nothing is written back into the
# source tree, which is what keeps a generated binary out of the repository.

set(PYWINRT_TABLE_DIR "${CMAKE_CURRENT_BINARY_DIR}/tables")
set(PYWINRT_TABLE_COMPILER "${WINRT_TABLE_PATH}/winrt/table/__init__.py")

function(pywinrt_install_namespace_package package)
    # package_name: e.g. winrt-Windows.Foundation, test-winrt-TestComponent
    get_filename_component(package_name "${package}" NAME)

    # split package name on last dash
    string(FIND "${package_name}" "-" index REVERSE)

    # package_root: e.g. winrt, test-winrt
    string(SUBSTRING "${package_name}" 0 ${index} package_root)

    # package_root_dir: e.g. winrt, test_winrt
    string(REPLACE "-" "_" package_root_dir "${package_root}")

    install(DIRECTORY "${package}/${package_root_dir}/"
        DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/${package_root_dir}"
        PATTERN "_table.pywinrt.txt" EXCLUDE
    )

    set(package_dir "${CMAKE_CURRENT_SOURCE_DIR}/${package}/${package_root_dir}")

    file(GLOB_RECURSE tables RELATIVE "${package_dir}" CONFIGURE_DEPENDS
        "${package_dir}/*/_table.pywinrt.txt")

    foreach(table ${tables})
        # ns_dir: the namespace's own folder, e.g. windows/foundation
        get_filename_component(ns_dir "${table}" DIRECTORY)

        set(binary "${PYWINRT_TABLE_DIR}/${package_root_dir}/${ns_dir}/_table.pywinrt")

        add_custom_command(
            OUTPUT "${binary}"
            COMMAND "${CMAKE_COMMAND}" -E env
                "PYTHONPATH=${WINRT_TABLE_PATH}"
                "${Python3_EXECUTABLE}" -m winrt.table "${package_dir}/${table}" "${binary}"
            DEPENDS "${package_dir}/${table}" "${PYWINRT_TABLE_COMPILER}"
            COMMENT "Compiling ${package_root_dir}/${ns_dir}/_table.pywinrt"
            VERBATIM
        )

        install(FILES "${binary}"
            DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/${package_root_dir}/${ns_dir}")

        list(APPEND PYWINRT_TABLES "${binary}")
    endforeach()

    set(PYWINRT_TABLES "${PYWINRT_TABLES}" PARENT_SCOPE)
endfunction()

if (PYWINRT_FULL_PROJECTION)
    file(GLOB packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winrt/winrt-*")
    file(GLOB winui2_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "winui2/winui2-*")
    file(GLOB wasdk_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "wasdk/winrt-*")
    file(GLOB webview2_packages RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "webview2/winrt-*")
else()
    # minimum set of packages required for unit tests
    set(packages
        "winrt/winrt-Windows.ApplicationModel.Core"
        "winrt/winrt-Windows.Data.Json"
        "winrt/winrt-Windows.Devices"
        "winrt/winrt-Windows.Devices.Geolocation"
        "winrt/winrt-Windows.Foundation"
        "winrt/winrt-Windows.Foundation.Collections"
        "winrt/winrt-Windows.Foundation.Metadata"
        "winrt/winrt-Windows.Foundation.Numerics"
        "winrt/winrt-Windows.Globalization.NumberFormatting"
        "winrt/winrt-Windows.Graphics"
        "winrt/winrt-Windows.Graphics.Capture"
        "winrt/winrt-Windows.Graphics.DirectX"
        "winrt/winrt-Windows.Graphics.DirectX.Direct3D11"
        "winrt/winrt-Windows.Graphics.Imaging"
        "winrt/winrt-Windows.Media"
        "winrt/winrt-Windows.Storage"
        "winrt/winrt-Windows.Storage.Streams"
        "winrt/winrt-Windows.System"
        "winrt/winrt-Windows.UI"
        "winrt/winrt-Windows.UI.Composition"
        "winrt/winrt-Windows.UI.Composition.Desktop"
        "winrt/winrt-Windows.UI.WindowManagement"
        "winrt/winrt-Windows.UI.Xaml.Hosting"
    )
    # The App SDK is published one distribution per NuGet component, so the
    # two namespaces the tests use bring their components with them.
    set(wasdk_packages
        "wasdk/winrt-Microsoft.WindowsAppSDK.InteractiveExperiences"
        "wasdk/winrt-Microsoft.WindowsAppSDK.Foundation"
    )
endif()

foreach(package ${packages} ${winui2_packages} ${wasdk_packages} ${webview2_packages})
    pywinrt_install_namespace_package(${package})
endforeach()

if (webview2_packages)
    set(WEBVIEW2_PATH "${PYWINRT_TOOLS_PATH}/Microsoft.Web.WebView2.${WEBVIEW2_VERSION}")
    install(FILES "${WEBVIEW2_PATH}/runtimes/win-${PYWINRT_TARGET_ARCH}/native_uap/Microsoft.Web.WebView2.Core.dll"
        DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt/microsoft/web/webview2/core")
endif()

# test component package
################################################################################

pywinrt_install_namespace_package("test-winrt/test-winrt-TestComponent")
install(FILES "${PYWINRT_TOOLS_PATH}/PyWinRT.TestWinRT.${TEST_WINRT_VERSION}/runtimes/win10-${PYWINRT_TARGET_ARCH}/native/TestComponent.dll"
    DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/test_winrt/testcomponent/")

# the tables of every package that is being installed, which nothing links
# against and so nothing else would ask to be built
add_custom_target(pywinrt-tables ALL DEPENDS ${PYWINRT_TABLES})
