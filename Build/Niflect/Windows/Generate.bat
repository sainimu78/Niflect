@echo off
mkdir DefaultBuild
cd DefaultBuild
del CMakeCache.txt
cmake ..\..\..\..\Project\Niflect -DCMAKE_INSTALL_PREFIX=Installed
::cmake ..\Project\Niflect -DCMAKE_INSTALL_PREFIX=Installed/Release -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 (
    echo "### cmake generating failed ###"
    pause
)
cd ..