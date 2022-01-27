rem @echo off

set target_version=%1
if "%target_version%"=="" set target_version=1.2.3.4

cmd /c scripts\vsdevcmd-build.cmd --force-cmake --build-type=Release --build-version=%target_version% pywinrt

nuget pack nuget\PyWinRT.nuspec -Version %target_version% -Properties pywinrt_exe=%cd%\_build\Windows\x86\Release\tool\python\pywinrt.exe
