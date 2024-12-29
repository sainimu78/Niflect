@echo off
mkdir DefaultBuild
cd DefaultBuild
cmake ..\..\..\..\Project\Niflect -DCMAKE_INSTALL_PREFIX=Installed -DSETUP=OFF -DDEPLOY=ON
cd ..