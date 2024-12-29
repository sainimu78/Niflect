@echo off
mkdir DefaultBuild
cd DefaultBuild
del CMakeCache.txt
cmake ..\..\..\..\Project\Niflect -DCMAKE_INSTALL_PREFIX=Installed -DSETUP=OFF -DDEPLOY=OFF
::cmake ..\Project\Niflect -DCMAKE_INSTALL_PREFIX=Installed/Release -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 (
    echo "### cmake generating failed ###"
    pause
)
cd ..