#!/bin/bash
BuildDirPath=$(pwd)/DefaultBuild
OldDirPath=$(pwd)
cd $BuildDirPath
rm -rf Installed
#cmake --install ./Debug --config Debug
#if [ $? -ne 0 ]; then
#    echo "CMake configuration failed for Debug build."
#else
	cmake --install ./Release --config Release
	if [ $? -ne 0 ]; then
		echo "CMake configuration failed for Release build."
	fi
#fi
cd $OldDirPath