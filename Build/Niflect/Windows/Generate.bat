@echo off
mkdir DefaultBuild
cd DefaultBuild
cmake ..\..\..\..\Project\Niflect -DCMAKE_INSTALL_PREFIX=Installed -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=OFF
cd ..