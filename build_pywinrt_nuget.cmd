rem @echo off

set target_version=%1
if "%target_version%"=="" set target_version=1.2.3.4

pushd src\scripts\windows
cmd /c vsdevcmd-build.cmd --force-cmake --build-type=Release --build-version=%target_version% pywinrt
popd

nuget pack src\package\pywinrt\tool\Microsoft.Windows.PyWinRT.nuspec -Version %target_version% -Properties pywinrt_exe=%cd%\_build\Windows\x86\Release\tool\python\pywinrt.exe
