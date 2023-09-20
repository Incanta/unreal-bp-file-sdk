# only used in windows for cross compiling
export LINUX_MULTIARCH_ROOT=/c/UnrealToolchains/v22_clang-16.0.6-centos7/
if [ "$(uname)" != "Darwin" ]; then
  ${LINUX_MULTIARCH_ROOT}x86_64-unknown-linux-gnu/bin/clang++ -v
fi

export ENGINE_VERSION=5.3
export PLUGIN_NAME=$(basename $(pwd))

if [ "$(uname)" == "Darwin" ]; then
  UAT_PATH="/Users/Shared/Epic Games/UE_${ENGINE_VERSION}/Engine/Build/BatchFiles/RunUAT.sh"
  TARGET_PLATFORMS=Mac
else
  # windows handles linux cross compile
  UAT_PATH="/c/Program Files/Epic Games/UE_${ENGINE_VERSION}/Engine/Build/BatchFiles/RunUAT.bat"
  TARGET_PLATFORMS=Win64+Linux+LinuxArm64
fi

"${UAT_PATH}" \
  BuildPlugin \
  -Plugin=$(pwd)/${PLUGIN_NAME}.uplugin \
  -Package=$(pwd)/Dist \
  -TargetPlatforms=${TARGET_PLATFORMS} \
  -Rocket \
  -VS2022 \
  | ../../compiler-beautify.pl --short
