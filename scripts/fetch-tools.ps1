# fetch code generation tools from nuget

param(
    [switch]$noCppWinRT,
    [switch]$noWindowsSdk,
    [switch]$noMicrosoftUiXaml,
    [switch]$noWindowsAppSDK,
    [switch]$noWebView2,
    [switch]$noTestWinRT
)

$repoRootPath = (Get-Item $PSScriptRoot).Parent.FullName

$toolsJsonPath = "$repoRootPath/.config/_tools.json"
$toolsJson = Get-Content $toolsJsonPath | ConvertFrom-Json

$CppWinRTVersion = $toolsJson."Microsoft.Windows.CppWinRT"
$WindowsSdkVersion = $toolsJson."Microsoft.Windows.SDK.CPP"
$MicrosoftUiXaml = $toolsJson."Microsoft.UI.Xaml"
$WindowsAppSDKVersion = $toolsJson."Microsoft.WindowsAppSDK"
$WebView2Version = $toolsJson."Microsoft.Web.WebView2"
$TestWinRTVersion = $toolsJson."PyWinRT.TestWinRT"

try {
    Get-Command nuget -ErrorAction Stop | Out-Null
} catch [System.Management.Automation.CommandNotFoundException] {
    Write-Host "Nuget command not found. Ensure that nuget.exe is installed and is in your `$env:PATH and try again." -ForegroundColor Red
    exit 1
}

if (!$noCppWinRT) {
    & nuget install Microsoft.Windows.CppWinRT -Version $CppWinRTVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools"
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noWindowsSdk) {
    & nuget install Microsoft.Windows.SDK.CPP -Version $WindowsSdkVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools" -NoHttpCache
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noMicrosoftUiXaml) {
    & nuget install Microsoft.UI.Xaml -Version $MicrosoftUiXaml -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools" -NoHttpCache
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noWindowsAppSDK) {
    & nuget install Microsoft.WindowsAppSDK -Version $WindowsAppSDKVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools" -NoHttpCache
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noWebView2) {
    & nuget install Microsoft.Web.WebView2 -Version $WebView2Version -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools" -NoHttpCache
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noTestWinRT) {
    & nuget install PyWinRT.TestWinRT -Version $TestWinRTVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools" -NoHttpCache
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}
