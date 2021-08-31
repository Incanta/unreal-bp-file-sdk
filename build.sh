# only used in windows for cross compiling
export LINUX_MULTIARCH_ROOT=/c/UnrealToolchains/v16_clang-9.0.1-centos7/
if [ "$(uname)" != "Darwin" ]; then
  ${LINUX_MULTIARCH_ROOT}x86_64-unknown-linux-gnu/bin/clang++ -v
fi

export ENGINE_VERSION=4.25

if [ "$(uname)" == "Darwin" ]; then
  UAT_PATH="/Users/Shared/Epic Games/UE_${ENGINE_VERSION}/Engine/Build/BatchFiles/RunUAT.sh"
  TARGET_PLATFORMS=Mac
else
  # windows handles linux cross compile
  UAT_PATH="/c/Program Files/Epic Games/UE_${ENGINE_VERSION}/Engine/Build/BatchFiles/RunUAT.bat"
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
