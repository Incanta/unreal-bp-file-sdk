echo off

set LINUX_MULTIARCH_ROOT=C:\UnrealToolchains\v15_clang-8.0.1-centos7\
%LINUX_MULTIARCH_ROOT%x86_64-unknown-linux-gnu\bin\clang++ -v

"C:\Program Files\Epic Games\UE_4.24\Engine\Build\BatchFiles\RunUAT.bat" ^
  BuildPlugin ^
  -Plugin=%CD%\FileSDK.uplugin ^
  -Package=%CD%\Dist ^
  -TargetPlatforms=Win32+Win64+Mac+Linux ^
  -Rocket ^
  -VS2017 ^
  | sed "s|Dist[/\\]HostProject[/\\]Plugins[/\\]FileSDK[/\\]||g"
