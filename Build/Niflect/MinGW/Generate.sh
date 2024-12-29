#!/bin/bash

#添加 gcc 环境
export PATH="/d/HenDuoDongXi/perl/c/bin:$PATH"

Toolset="Unix Makefiles"
BuildDirPathDebug=./DefaultBuild/Debug
BuildDirPathRelease=./DefaultBuild/Release
InstallPrefix=../Installed

export CFLAGS="-m64"
export CXXFLAGS="-m64"
#export CFLAGS="-m32"
#export CXXFLAGS="-m32"

OldDirPath=$(pwd)
mkdir -p $BuildDirPathDebug
cd $BuildDirPathDebug
rm -f "./CMakeCache.txt"
cmake ../../../../../Project/Niflect -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DCMAKE_BUILD_TYPE=Debug
if [ $? -ne 0 ]; then
    echo "CMake configuration failed for Debug build."
    exit 2
fi
cd $OldDirPath

mkdir -p $BuildDirPathRelease
cd $BuildDirPathRelease
rm -f "./CMakeCache.txt"
cmake ../../../../../Project/Niflect -G "$Toolset" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ]; then
    echo "CMake configuration failed for Debug build."
    exit 2
fi
cd $OldDirPath