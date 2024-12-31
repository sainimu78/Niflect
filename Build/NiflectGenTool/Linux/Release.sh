#!/bin/bash
ProjectName=NiflectGenTool
BuildDirPathDebug=./DefaultBuild/Debug
BuildDirPathRelease=./DefaultBuild/Release
InstallPrefix=../Installed
Toolset="Unix Makefiles"
OldDirPath=$(pwd)

#cd $BuildDirPathDebug
#cmake ../../../../../Project/$ProjectName -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=ON -DCMAKE_BUILD_TYPE=Debug
#cd $OldDirPath

cd $BuildDirPathRelease
cmake ../../../../../Project/$ProjectName -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=ON -DCMAKE_BUILD_TYPE=Release
cd $OldDirPath