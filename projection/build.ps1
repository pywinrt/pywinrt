param (
    [Parameter(Mandatory=$false)]
    [string]$buildType = "Debug",

    [Parameter(Mandatory=$false)]
    [string]$pythonVersion = "3.7",

    [Parameter(Mandatory=$false)]
    [string]$compiler = "cl.exe"
)

$repoRootPath = (get-item $PSScriptRoot).Parent.FullName.Replace('\', '/')
$sourcePath = $PSScriptRoot.Replace('\', '/')
$buildPath = "$repoRootPath/_build/py-projection/$env:VSCMD_ARG_TGT_ARCH-$buildType"


cmake -S $sourcePath "-B$buildPath" -GNinja "-DCMAKE_BUILD_TYPE=$buildType" "-DCMAKE_C_COMPILER=$compiler" "-DCMAKE_CXX_COMPILER=$compiler" "-DPYTHON_VERSION=$pythonVersion"
cmake --build $buildPath -- -v -j 4

copy-item $buildPath/*.pyd "$sourcePath/pywinrt/winrt"
