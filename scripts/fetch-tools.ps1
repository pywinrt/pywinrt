# fetch code generation tools from nuget

param(
    # CppWinRT
    [Parameter(Mandatory=$false)]
    [string]$CppWinRTVersion = "2.0.250303.1",
    [switch]$noCppWinRT,

    # Windows SDK
    [Parameter(Mandatory=$false)]
    [string]$WindowsSdkVersion = "10.0.26100.4188",
    [switch]$noWindowsSdk,

    # WinUI 2
    [Parameter(Mandatory=$false)]
    [string]$MicrosoftUiXaml = "2.8.7",
    [switch]$noMicrosoftUiXaml,

    # WinUI 3
    [Parameter(Mandatory=$false)]
    [string]$WindowsAppSDKVersion = "1.7.250513003",
    [switch]$noWindowsAppSDK,

    # WebView2
    [Parameter(Mandatory=$false)]
    [string]$WebView2Version = "1.0.3240.44",
    [switch]$noWebView2,

    # TestWinRT
    [Parameter(Mandatory=$false)]
    [string]$TestWinRTVersion = "1.3.0",
    [switch]$noTestWinRT
)

$repoRootPath = (Get-Item $PSScriptRoot).Parent.FullName
$pywinrtVersionFileName = "pywinrt_version_52c8"

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
    Write-Output "$CppWinRTVersion" > "$repoRootPath/_tools/Microsoft.Windows.CppWinRT/$pywinrtVersionFileName"
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noWindowsSdk) {
    & nuget install Microsoft.Windows.SDK.CPP -Version $WindowsSdkVersion -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools" -NoHttpCache

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
    Write-Output "$WindowsSdkVersion" > "$repoRootPath/_tools/Microsoft.Windows.SDK.CPP/$pywinrtVersionFileName"
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noMicrosoftUiXaml) {
    & nuget install Microsoft.UI.Xaml -Version $MicrosoftUiXaml -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools" -NoHttpCache

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
    Write-Output "$MicrosoftUiXaml" > "$repoRootPath/_tools/Microsoft.UI.Xaml/$pywinrtVersionFileName"
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noWindowsAppSDK) {
    & nuget install Microsoft.WindowsAppSDK -Version $WindowsAppSDKVersion -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools" -NoHttpCache

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
    Write-Output "$WindowsAppSDKVersion" > "$repoRootPath/_tools/Microsoft.WindowsAppSDK/$pywinrtVersionFileName"
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noWebView2) {
    & nuget install Microsoft.Web.WebView2 -Version $WebView2Version -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools" -NoHttpCache

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
    Write-Output "$WebView2Version" > "$repoRootPath/_tools/Microsoft.Web.WebView2/$pywinrtVersionFileName"
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (!$noTestWinRT) {
    & nuget install PyWinRT.TestWinRT -Version $TestWinRTVersion -ExcludeVersion -DependencyVersion Ignore -OutputDirectory "$repoRootPath/_tools" -NoHttpCache

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
    Write-Output "$TestWinRTVersion" > "$repoRootPath/_tools/PyWinRT.TestWinRT/$pywinrtVersionFileName"
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}
