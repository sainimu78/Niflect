#!/bin/bash
cd DefaultBuild
cmake --build ./Debug --target clean --config Debug
cmake --build ./Release --target clean --config Release
cd ..