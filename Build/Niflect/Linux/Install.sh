#!/bin/bash
cd DefaultBuild
rm -rf Installed
cmake --install ./Debug --config Debug
cmake --install ./Release --config Release
cd ..