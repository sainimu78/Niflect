cd DefaultBuild
rmdir /s /q Installed
::cmake --install . --config Debug
cmake --install . --config Release
cd ..