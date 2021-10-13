# only used in windows for cross compiling
export LINUX_MULTIARCH_ROOT=/c/UnrealToolchains/v19_clang-11.0.1-centos7/
if [ "$(uname)" != "Darwin" ]; then
  ${LINUX_MULTIARCH_ROOT}x86_64-unknown-linux-gnu/bin/clang++ -v
fi

export ENGINE_VERSION=4.27
export PLUGIN_NAME=$(basename $(pwd))

if [ "$(uname)" == "Darwin" ]; then
  UAT_PATH="/Users/Shared/Epic Games/${ENGINE_VERSION}/Engine/Build/BatchFiles/RunUAT.sh"
  TARGET_PLATFORMS=Mac
else
  # windows handles linux cross compile
  UAT_PATH="/c/Program Files/Epic Games/${ENGINE_VERSION}/Engine/Build/BatchFiles/RunUAT.bat"
  TARGET_PLATFORMS=Win64+Linux+LinuxAArch64
fi

"${UAT_PATH}" \
  BuildPlugin \
  -Plugin=$(pwd)/${PLUGIN_NAME}.uplugin \
  -Package=$(pwd)/Dist \
  -TargetPlatforms=${TARGET_PLATFORMS} \
  -Rocket \
  -VS2017 \
  | ../../compiler-beautify.pl --short
