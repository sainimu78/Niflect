@echo off
setlocal

set ProjectName=NiflectGenTool
set Arch=%2
set ToolsetAndArch=%1_%Arch%
set Platform=Windows

set DstRootDirPath=F:\sainimu78_Storage\Release\%Platform%
set SrcBinDirPathDebug=%cd%\%ToolsetAndArch%\Debug\bin
set SrcBinDirPathRelease=%cd%\%ToolsetAndArch%\Release\bin
set DstProjectDirPath=%DstRootDirPath%\%ProjectName%
set DetBuildArchDirPath=%DstProjectDirPath%\build\%Platform%\%Arch%
set DstBuildDirPathDebug=%DetBuildArchDirPath%\Debug
set DstBuildDirPathRelease=%DetBuildArchDirPath%\Release
set DstBinDirPathDebug=%DstBuildDirPathDebug%\bin
set DstBinDirPathRelease=%DstBuildDirPathRelease%\bin
set LicenseFilePath=%cd%\..\..\..\LICENSE.md
set SrcIncludeDirPath=%cd%\..\..\..\Source\%ProjectName%\include
set DstIncludeDirPath=%DstProjectDirPath%\include
set DstZipFilePath=%DstProjectDirPath%.zip

rmdir /s /q "%DstProjectDirPath%"

::xcopy /E /y "%SrcBinDirPathDebug%\*.dll" "%DstBinDirPathDebug%\"
::xcopy /E /y "%SrcBinDirPathDebug%\*.exe" "%DstBinDirPathDebug%\"
xcopy /y "%SrcBinDirPathRelease%\*.dll" "%DstBinDirPathRelease%\"
xcopy /y "%SrcBinDirPathRelease%\*.exe" "%DstBinDirPathRelease%\"

xcopy /y "%LicenseFilePath%" "%DstProjectDirPath%\"

xcopy /E /y "%SrcIncludeDirPath%\" "%DstIncludeDirPath%\"

::---------------------------------------------------------------

set ToolDirPath=%cd%
set path=%ToolDirPath%;%path%
set UserName=WishingContributor
set Password=1
set StorageDirPath=http://192.168.245.158/sainimu78_Storage/
set Name7zExe=7za.exe
set Storage7zDirPath=%StorageDirPath%/Tool/%Platform%/7z
set Dst7zExeFilePath=%ToolDirPath%\%Name7zExe%
set DstZippedDirPath=%DstRootDirPath%\%ZipFileName%

curl -u %UserName%:%Password% -L -o "%Dst7zExeFilePath%" "%Storage7zDirPath%/%Name7zExe%"

del "%DstZipFilePath%"

%Name7zExe% a "%DstZipFilePath%" "%DstProjectDirPath%"

rmdir /s /q "%DstProjectDirPath%"
del "%Dst7zExeFilePath%"

endlocal