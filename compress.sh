#!/bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PLUGIN_NAME=$(basename ${SCRIPT_DIR})

PLUGIN_VERSION=$(grep -Po '"VersionName": "\K[^"]*' ${PLUGIN_NAME}.uplugin)

FILES=$(cat .distribution-list)
FILES_WITH_FOLDER=$(echo ${FILES} | sed "s/[^ ]* */${PLUGIN_NAME}\/&/g")

cd ..

baseZip=${PLUGIN_NAME}-${PLUGIN_VERSION}.zip
/c/Program\ Files/7-Zip/7z a -tzip ${baseZip} ${FILES_WITH_FOLDER}

mv ${baseZip} ${PLUGIN_NAME}

cd ${PLUGIN_NAME}

versions="5.2.0 5.3.0 5.4.0"
for version in ${versions}
do
  newContent=$(sed -r "s/\"EngineVersion\": \"[0-9]+\.[0-9]+\.[0-9]+\"/\"EngineVersion\": \"${version}\"/" ${PLUGIN_NAME}.uplugin)
  newZip=${PLUGIN_NAME}-${PLUGIN_VERSION}-${version}.zip
  cp $baseZip $newZip
  echo "${newContent}" | /c/Program\ Files/7-Zip/7z a -tzip ${newZip} -si${PLUGIN_NAME}/${PLUGIN_NAME}.uplugin
done

rm ${baseZip}
