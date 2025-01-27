@echo off
@set ProjectDirPath=..\..\..\..\Project\NiflectGenTool
@set BuildDirPath=DefaultBuild
@set InstallPrefix=Installed
@set OldDirPath=%cd%

@mkdir %BuildDirPath%
@cd %BuildDirPath%
@cmake %ProjectDirPath% -DCMAKE_INSTALL_PREFIX=%InstallPrefix% -DPROJECT_SETUP=OFF -DPROJECT_RELEASE=OFF
@if %ERRORLEVEL% neq 0 (
    echo "### cmake generating failed ###"
    pause
)
@cd %OldDirPath%