# only used in windows for cross compiling
export LINUX_MULTIARCH_ROOT=/c/UnrealToolchains/v17_clang-10.0.1-centos7/
${LINUX_MULTIARCH_ROOT}x86_64-unknown-linux-gnu/bin/clang++ -v

if [ "$(uname)" = "Darwin" ]; then
  UAT_PATH="/Users/Shared/Epic Games/UE_4.26/Engine/Build/BatchFiles/RunUAT.sh"
  TARGET_PLATFORMS=Mac
else
  # windows handles linux cross compile
  UAT_PATH="/c/Program Files/Epic Games/UE_4.26/Engine/Build/BatchFiles/RunUAT.bat"
  TARGET_PLATFORMS=Win64+Linux
fi

"${UAT_PATH}" \
  BuildPlugin \
  -Plugin=$(pwd)/FileSDK.uplugin \
  -Package=$(pwd)/Dist \
  -TargetPlatforms=${TARGET_PLATFORMS} \
  -Rocket \
  -VS2017 \
  | ../../compiler-beautify.pl --short
