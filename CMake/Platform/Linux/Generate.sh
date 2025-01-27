#!/bin/bash
BuildDirPath=$(pwd)/DefaultBuild
rm -f "$BuildDirPath/Debug/CMakeCache.txt"
rm -f "$BuildDirPath/Release/CMakeCache.txt"
$(pwd)/Update.sh