# redistribution packages
################################################################################
#
# A redistribution package is a module that puts a third-party .dll on the DLL
# search path, plus the .dll itself. The wheel build fetches the .dll from the
# NuGet package, so here it is taken from the copy fetch-tools.ps1 unpacked.

set(WEBVIEW2_PATH "${PYWINRT_TOOLS_PATH}/Microsoft.Web.WebView2.${WEBVIEW2_VERSION}")
install(DIRECTORY "winrt-Microsoft.Web.WebView2.Dll/winrt/"
    DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt")
install(FILES "${WEBVIEW2_PATH}/runtimes/win-${PYWINRT_TARGET_ARCH}/native_uap/Microsoft.Web.WebView2.Core.dll"
    DESTINATION "${PYWINRT_SITE_PACKAGES_DIR}/winrt/microsoft/web/webview2/dll")
