@echo off
@set BuildDirPath=%cd%\DefaultBuild
@set OldDirPath=%cd%
@cd %BuildDirPath%
@rmdir /s /q Installed
@cmake --install . --config Debug
@if %ERRORLEVEL% neq 0 (
    echo "### cmake generating failed ###"
    pause
) else (
	@cmake --install . --config Release
	@if %ERRORLEVEL% neq 0 (
		echo "### cmake generating failed ###"
		pause
	)
)
@cd %OldDirPath%