@echo off
mkdir DefaultBuild
cd DefaultBuild
cmake ..\..\..\..\Project\NiflectGenTool -DCMAKE_INSTALL_PREFIX=Installed -DPROJECT_SETUP=ON -DPROJECT_RELEASE=OFF
cd ..