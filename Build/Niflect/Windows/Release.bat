@echo off
set ProjectDirPath=..\..\..\..\Project\Niflect
set BuildDirPath=DefaultBuild
set InstallPrefix=Installed
set OldDirPath=%cd%

cd %BuildDirPath%
cmake %ProjectDirPath% -DCMAKE_INSTALL_PREFIX=%InstallPrefix% -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=ON
if %ERRORLEVEL% neq 0 (
    echo "### cmake generating failed ###"
    pause
)
cd %OldDirPath%