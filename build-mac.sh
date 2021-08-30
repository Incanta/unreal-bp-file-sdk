/Users/Shared/Epic\ Games/UE_4.26/Engine/Build/BatchFiles/RunUAT.sh \
  BuildPlugin \
  -Plugin=$(pwd)/FileSDK.uplugin \
  -Package=$(pwd)/Dist \
  -TargetPlatforms=Mac \
  -Rocket \
  -VS2017 \
  | ../../compiler-beautify.pl --short
