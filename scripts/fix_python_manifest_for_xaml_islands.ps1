<#
XAML Islands require that the application manifest has maxversiontested set or
WindowsXamlManager.initialize_for_current_thread() will fail. Python ships with
an application manifest stored as a resource of the .exe file but doesn't have
maxversiontested set. To fix this, we can use the MT.exe command to update the
manifest.

https://learn.microsoft.com/en-us/windows/win32/sbscs/application-manifests#maxversiontested
#>

param (
    [Parameter(Mandatory=$true)]
    [string]$pythonExe,
    [string]$maxVersionTested = "10.0.18226.0"
)

$ErrorActionPreference = "Stop"

$machine = [System.Environment]::GetEnvironmentVariable("PROCESSOR_ARCHITECTURE")

switch ($machine) {
    "AMD64" { $arch = "x64" }
    "x86" { $arch = "x86" }
    "ARM64" { $arch = "arm64" }
    default { throw "Unsupported architecture: $machine" }
}

$programFilesX86 = [System.Environment]::GetEnvironmentVariable("ProgramFiles(x86)")
$mtExePaths = Get-ChildItem -Path "$programFilesX86\Windows Kits\10\bin\*\$arch\mt.exe" -ErrorAction Stop

# Assuming values are sorted and last is the newest.
$mtExe = $mtExePaths[-1].FullName

$oldManifest = "$pythonExe.old.manifest"
$newManifest = "$pythonExe.new.manifest"

# Don't write over the old manifest if it exists.
if (Test-Path -Path $oldManifest) {
    throw "$oldManifest already exists. Please rename it and try again."
}

# Use MT.exe to extract the manifest from the python.exe
& "$mtExe" "-inputresource:$pythonExe" "-out:$oldManifest" "-nologo"
if ($LASTEXITCODE -ne 0) {
    throw "Failed to extract manifest from $pythonExe"
}

# Add maxversiontested element to the manifest

[xml]$doc = Get-Content -Path "$oldManifest"`

$appNode = $doc.GetElementsByTagName("application", "urn:schemas-microsoft-com:compatibility.v1").Item(0)

if ($appNode.GetElementsByTagName("maxversiontested").Count -gt 0) {
    throw "maxversiontested already set"
}

$maxVersionTestedNode = $doc.CreateElement("maxversiontested", "urn:schemas-microsoft-com:compatibility.v1")
$maxVersionTestedNode.SetAttribute("Id", $maxVersionTested)
$appNode.AppendChild($maxVersionTestedNode) | Out-Null

# Apply the new manifest to the python.exe

$doc.Save($newManifest)

& "$mtExe" "-manifest" "$newManifest" "-outputresource:$pythonExe;1" "-nologo"
if ($LASTEXITCODE -ne 0) {
    throw "Failed to update manifest in $pythonExe"
}

Write-Output "Updated $pythonExe with maxversiontested=$maxVersionTested"
Write-output "You may restore the original manifest with the command:"
Write-output "& `"$mtExe`" `"-manifest`" `"$oldManifest`" `"-outputresource:$pythonExe;1`" `"-nologo`""
