@echo off
cd DefaultBuild
cmake ..\..\..\..\Project\Niflect -DCMAKE_INSTALL_PREFIX=Installed -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=ON
cd ..