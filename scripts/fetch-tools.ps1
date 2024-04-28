# fetch code generation tools from nuget

param(
    [Parameter(Mandatory=$false)]
    [string]$CppWinRTVersion = "2.0.240111.5",

    [Parameter(Mandatory=$false)]
    [string]$PyWinRTVersion = "2.0.1",

    [Parameter(Mandatory=$false)]
    [string]$WindowsAppSDKVersion = "1.5.240311000",

    [Parameter(Mandatory=$false)]
    [string]$TestWinRTVersion = "1.0.12",

    [switch]$useLocalPyWinRTNuget,
    [switch]$noPyWinRT
)

$repoRootPath = (Get-Item $PSScriptRoot).Parent.FullName

try {
    Get-Command nuget -ErrorAction Stop | Out-Null
} catch [System.Management.Automation.CommandNotFoundException] {
    Write-Host "Nuget command not found. Ensure that nuget.exe is installed and is in your `$env:PATH and try again." -ForegroundColor Red
    exit 1
}

& nuget install Microsoft.Windows.CppWinRT -Version $CppWinRTVersion -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools"

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

If (!$noPyWinRT) {
    $source = If ($useLocalPyWinRTNuget) { "-Source", "$repoRootPath" } Else { "" }
    & nuget install PyWinRT -Version $PyWinRTVersion -Prerelease -DependencyVersion Ignore -ExcludeVersion -OutputDirectory "$repoRootPath/_tools" $source
}

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

& nuget install Microsoft.WindowsAppSDK -Version $WindowsAppSDKVersion -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools"

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

& nuget install KennyKerr.Windows.TestWinRT -Version $TestWinRTVersion -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools"

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
