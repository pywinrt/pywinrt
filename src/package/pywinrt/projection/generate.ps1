param ([switch]$clean, [switch]$fullProjection, [switch]$useLocalPyWinRTNuget)

$windows_sdk = 'sdk+'
$repoRootPath = (get-item $PSScriptRoot).parent.Parent.parent.Parent.FullName
$projectionPath = "$PSScriptRoot"

if ($clean) {
    write-host "removing generated projection files"
    remove-item "$repoRootPath/_build/py-projection/" -rec -for -ErrorAction SilentlyContinue
    exit
}

if ($useLocalPyWinRTNuget) {
    nuget install PyWinRT -Prerelease -DependencyVersion Ignore -ExcludeVersion -OutputDirectory "$repoRootPath/_build/tools" -Source "$repoRootPath"
    $pywinrt_exe = "$repoRootPath/_build/tools/PyWinRT/bin/pywinrt.exe"
} else {
    $pywinrt_exe = Get-ChildItem $repoRootPath\_build\Windows\x86\Debug\pywinrt\pywinrt.exe | 
    Sort-Object -Descending | Select-Object -first 1
}

if (-not $pywinrt_exe) {
    Write-Error "pywinrt not avaliable"
    exit
}

nuget install Microsoft.Windows.CppWinRT -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_build/tools"

$cppwinrt_exe = "$repoRootPath/_build/tools/Microsoft.Windows.CppWinRT\bin\cppwinrt.exe"

& $cppwinrt_exe -input $windows_sdk -output $projectionPath/cppwinrt -verbose

$pywinrt_path = "$projectionPath/pywinrt"

remove-item $pywinrt_path -Recurse -Force -ErrorAction SilentlyContinue

if ($fullProjection) {
    $pyinclude = @("Windows.")
}
else {
    $pyinclude = "Windows.Data.Json", "Windows.Devices.Geolocation", "Windows.Foundation", "Windows.Graphics.Capture", "Windows.Graphics.DirectX", "Windows.Storage.Streams"
}
$pyexclude = "Windows.UI.Composition", "Windows.UI.Xaml"
$pyin = $pyinclude | ForEach-Object { "-include", "$_" }
$pyout = $pyexclude | ForEach-Object { "-exclude", "$_" }

$pyparams = ("-input", $windows_sdk, "-output", $pywinrt_path, "-verbose") + $pyin + $pyout

& $pywinrt_exe $pyparams
