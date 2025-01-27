@echo off
@set BuildDirPath=%cd%\DefaultBuild
@set OldDirPath=%cd%
@cd %BuildDirPath%
@cmake --build . -j 1024 --config Debug
@if %ERRORLEVEL% neq 0 (
	echo "### cmake generating failed ###"
	pause
) else (
	@cmake --build . -j 1024 --config Release
	@if %ERRORLEVEL% neq 0 (
		echo "### cmake generating failed ###"
		pause
	)
)
@cd %OldDirPath%