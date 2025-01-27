@echo off
@set BuildDirPath=%cd%\DefaultBuild
@del %BuildDirPath%\CMakeCache.txt
@%cd%\Update.bat