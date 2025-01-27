#!/bin/bash
BuildDirPath=$(pwd)/DefaultBuild
OldDirPath=$(pwd)
cd $BuildDirPath
cmake --build ./Debug --target clean --config Debug
if [ $? -ne 0 ]; then
    echo "CMake configuration failed for Debug build."
else
	cmake --build ./Release --target clean --config Release
	if [ $? -ne 0 ]; then
		echo "CMake configuration failed for Release build."
	fi
fi
cd $OldDirPath