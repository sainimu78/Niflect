@echo off
cd DefaultBuild
cmake ..\..\..\..\Project\NiflectGenTool -DCMAKE_INSTALL_PREFIX=Installed -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=ON
cd ..