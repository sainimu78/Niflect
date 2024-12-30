@echo off
mkdir DefaultBuild
cd DefaultBuild
del CMakeCache.txt
cmake ../../../../../Project/Niflect -DCMAKE_INSTALL_PREFIX=../Installed -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=OFF -DCMAKE_BUILD_TYPE=Debug
cmake ../../../../../Project/Niflect -DCMAKE_INSTALL_PREFIX=../Installed -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=OFF -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 (
    echo "### cmake generating failed ###"
    pause
)
cd ..