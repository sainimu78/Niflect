@echo off

set ToolsetAndArch=%1

cmake --build ./%ToolsetAndArch% --target clean --config Debug
cmake --build ./%ToolsetAndArch% --target clean --config Release