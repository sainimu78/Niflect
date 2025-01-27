@echo off
@cd DefaultBuild
@cmake --build . --target clean --config Debug
@if %ERRORLEVEL% neq 0 (
	echo "### cmake generating failed ###"
	pause
) else (
	@cmake --build . --target clean --config Release
	@if %ERRORLEVEL% neq 0 (
		echo "### cmake generating failed ###"
		pause
	)
)
@cd ..