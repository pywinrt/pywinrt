# fetch the latest pythoncapi_compat.h from the pythoncapi-compat repo and copy it to the winrt-runtime project

$repoRootPath = (Get-Item $PSScriptRoot).Parent.FullName

Invoke-WebRequest "https://raw.githubusercontent.com/python/pythoncapi-compat/main/pythoncapi_compat.h" -OutFile "$repoRootPath/runtime/python/winrt/include/pythoncapi_compat.h"
