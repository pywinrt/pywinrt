# fetch code generation tools from nuget

param(
    [Parameter(Mandatory=$false)]
    [string]$CppWinRTVersion = "2.0.230706.1",

    [Parameter(Mandatory=$false)]
    [string]$PyWinRTVersion = "2.0.0-beta.2",

    [Parameter(Mandatory=$false)]
    [string]$WindowsAppSDKVersion = "1.4.231115000",

    [Parameter(Mandatory=$false)]
    [string]$TestWinRTVersion = "1.0.12",

    [switch]$useLocalPyWinRT
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

$source = If ($useLocalPyWinRT) { "-Source", "$repoRootPath" } Else { "" }
& nuget install PyWinRT -Version $PyWinRTVersion -Prerelease -DependencyVersion Ignore -ExcludeVersion -OutputDirectory "$repoRootPath/_tools" $source

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
