@echo off
mkdir DefaultBuild
cd DefaultBuild
cmake ..\..\..\..\Project\Niflect -DCMAKE_INSTALL_PREFIX=Installed -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=OFF
if %ERRORLEVEL% neq 0 (
    echo "### cmake generating failed ###"
    pause
)
cd ..