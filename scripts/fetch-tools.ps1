# fetch code generation tools from nuget

param(
    [Parameter(Mandatory=$false)]
    [string]$CppWinRTVersion = "2.0.240111.5",
    [switch]$noCppWinRT,

    [Parameter(Mandatory=$false)]
    [string]$WindowsAppSDKVersion = "1.6.240829007",
    [switch]$noWindowsAppSDK,

    [Parameter(Mandatory=$false)]
    [string]$WebView2Version = "1.0.2739.15",
    [switch]$noWebView2,

    [Parameter(Mandatory=$false)]
    [string]$TestWinRTVersion = "1.0.0",
    [switch]$noTestWinRT
)

$repoRootPath = (Get-Item $PSScriptRoot).Parent.FullName

try {
    Get-Command nuget -ErrorAction Stop | Out-Null
} catch [System.Management.Automation.CommandNotFoundException] {
    Write-Host "Nuget command not found. Ensure that nuget.exe is installed and is in your `$env:PATH and try again." -ForegroundColor Red
    exit 1
}

if (!$noCppWinRT) {
    & nuget install Microsoft.Windows.CppWinRT -Version $CppWinRTVersion -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools"

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noWindowsAppSDK) {
    & nuget install Microsoft.WindowsAppSDK -Version $WindowsAppSDKVersion -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools"

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noWebView2) {
    & nuget install Microsoft.Web.WebView2 -Version $WebView2Version -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools"

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noTestWinRT) {
    & nuget install PyWinRT.TestWinRT -Version $TestWinRTVersion -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools"

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}
