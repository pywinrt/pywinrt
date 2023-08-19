# fetch the latest pythoncapi_compat.h from the pythoncapi-compat repo and copy it to the winrt-sdk project

$repoRootPath = (Get-Item $PSScriptRoot).Parent.FullName

Invoke-WebRequest "https://raw.githubusercontent.com/python/pythoncapi-compat/main/pythoncapi_compat.h" -OutFile "$repoRootPath/projection/winrt-sdk/src/winrt_sdk/pywinrt/pythoncapi_compat.h"
