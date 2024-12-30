#!/bin/bash
BuildDirPathDebug=./DefaultBuild/Debug
BuildDirPathRelease=./DefaultBuild/Release
InstallPrefix=../Installed
Toolset="Unix Makefiles"
OldDirPath=$(pwd)

mkdir -p $BuildDirPathDebug
cd $BuildDirPathDebug
cmake ../../../../../Project/Niflect -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=OFF -DCMAKE_BUILD_TYPE=Debug
if [ $? -ne 0 ]; then
    echo "CMake configuration failed for Debug build."
fi
cd $OldDirPath

mkdir -p $BuildDirPathRelease
cd $BuildDirPathRelease
cmake ../../../../../Project/Niflect -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=OFF -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ]; then
    echo "CMake configuration failed for Debug build."
fi
cd $OldDirPath