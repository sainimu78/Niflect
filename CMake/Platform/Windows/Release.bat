@echo off
@set BuildDirPath=%cd%\DefaultBuild
@set InstallPrefix=Installed
@set OldDirPath=%cd%

if defined Toolset (
    set CMakeToolsetArgs=-T %Toolset%
)

cd %BuildDirPath%
cmake %ProjectDirPath% -DCMAKE_INSTALL_PREFIX=%InstallPrefix% -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=ON %CMakeToolsetArgs%
if %ERRORLEVEL% neq 0 (
    echo "### cmake generating failed ###"
    pause
)
cd %OldDirPath%