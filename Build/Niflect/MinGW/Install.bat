cd DefaultBuild
rmdir /s /q Installed
cmake --install ./Debug --config Debug
cmake --install ./Release --config Release
cd ..