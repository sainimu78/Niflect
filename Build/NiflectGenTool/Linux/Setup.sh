#!/bin/bash
ProjectDirPath=../../../../../Project/NiflectGenTool
BuildDirPathDebug=./DefaultBuild/Debug
BuildDirPathRelease=./DefaultBuild/Release
InstallPrefix=../Installed
Toolset="Unix Makefiles"
OldDirPath=$(pwd)

mkdir -p $BuildDirPathDebug
cd $BuildDirPathDebug
cmake $ProjectDirPath -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DPROJECT_SETUP=ON -DPROJECT_RELEASE=OFF -DCMAKE_BUILD_TYPE=Debug
cd $OldDirPath

mkdir -p $BuildDirPathRelease
cd $BuildDirPathRelease
cmake $ProjectDirPath -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DPROJECT_SETUP=ON -DPROJECT_RELEASE=OFF -DCMAKE_BUILD_TYPE=Release
cd $OldDirPath