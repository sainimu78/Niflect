@echo off

set ToolsetAndArch=%1

cmake --build ./%ToolsetAndArch% --config Debug -j 1024
cmake --build ./%ToolsetAndArch% --config Release -j 1024