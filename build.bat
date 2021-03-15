echo off

set LINUX_MULTIARCH_ROOT=C:\UnrealToolchains\v11_clang-5.0.0-centos7\
%LINUX_MULTIARCH_ROOT%x86_64-unknown-linux-gnu\bin\clang++ -v

"C:\Program Files\Epic Games\UE_4.20\Engine\Build\BatchFiles\RunUAT.bat" ^
  BuildPlugin ^
  -Plugin=%CD%\FileSDK.uplugin ^
  -Package=%CD%\Dist ^
  -TargetPlatforms=Win64 ^
  -Rocket ^
  -VS2017 ^
  | sed "s|Dist[/\\]HostProject[/\\]Plugins[/\\]FileSDK[/\\]||g"
