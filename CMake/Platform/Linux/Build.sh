#!/bin/bash
BuildDirPath=$(pwd)/DefaultBuild
OldDirPath=$(pwd)
cd $BuildDirPath
cmake --build ./Debug -j 1024 --config Debug
if [ $? -ne 0 ]; then
    echo "CMake configuration failed for Debug build."
else
	cmake --build ./Release -j 1024 --config Release
	if [ $? -ne 0 ]; then
		echo "CMake configuration failed for Release build."
	fi
fi
cd $OldDirPath