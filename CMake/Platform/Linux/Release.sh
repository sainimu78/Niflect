#!/bin/bash
BuildDirPathDebug=$(pwd)/DefaultBuild/Debug
BuildDirPathRelease=$(pwd)/DefaultBuild/Release
InstallPrefix=../Installed
Toolset="Unix Makefiles"
OldDirPath=$(pwd)

cd $BuildDirPathRelease
cmake $ProjectDirPath -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=ON
cd $OldDirPath