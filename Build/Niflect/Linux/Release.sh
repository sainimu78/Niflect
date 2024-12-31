#!/bin/bash
BuildDirPathDebug=./DefaultBuild/Debug
BuildDirPathRelease=./DefaultBuild/Release
InstallPrefix=../Installed
Toolset="Unix Makefiles"
OldDirPath=$(pwd)

cd $BuildDirPathRelease
cmake ../../../../../Project/Niflect -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=ON
cd $OldDirPath