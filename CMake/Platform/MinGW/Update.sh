#!/bin/bash

#添加 gcc 环境
export PATH="/d/HenDuoDongXi/perl/c/bin:$PATH"

BuildDirPathDebug=$(pwd)/DefaultBuild/Debug
BuildDirPathRelease=$(pwd)/DefaultBuild/Release
InstallPrefix=../Installed
Toolset="Unix Makefiles"
OldDirPath=$(pwd)

export CFLAGS="-m64"
export CXXFLAGS="-m64"
#export CFLAGS="-m32"
#export CXXFLAGS="-m32"
#export CFLAGS="-m64 -g -O0"

mkdir -p $BuildDirPathDebug
cd $BuildDirPathDebug
cmake $ProjectDirPath -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DCMAKE_BUILD_TYPE=Debug
if [ $? -ne 0 ]; then
    echo "CMake configuration failed for Debug build."
else
	mkdir -p $BuildDirPathRelease
	cd $BuildDirPathRelease
	cmake $ProjectDirPath -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DCMAKE_BUILD_TYPE=Release
	if [ $? -ne 0 ]; then
		echo "CMake configuration failed for Debug build."
	fi
fi
cd $OldDirPath