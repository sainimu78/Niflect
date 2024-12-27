@echo off
setlocal

set ProjectName=Niflect
set ToolsetAndArch=%1

set DstRootDirPath=F:\sainimu78_Storage\Release\Windows\x64
set SrcBinDirPathDebug=%cd%\%ToolsetAndArch%\Debug\%ProjectName%
set SrcBinDirPathRelease=%cd%\%ToolsetAndArch%\Release\%ProjectName%
set DstProjectDirPath=%DstRootDirPath%\%ProjectName%
set DstProjectDirPathDebug=%DstProjectDirPath%\Debug
set DstProjectDirPathRelease=%DstProjectDirPath%\Release
set DstBinDirPathDebug=%DstProjectDirPathDebug%\bin
set DstBinDirPathRelease=%DstProjectDirPathRelease%\bin
set LicenseFilePath=%cd%\..\..\..\LICENSE.md
set SrcIncludeDirPath=%cd%\..\..\..\Source\%ProjectName%\include
set DstIncludeDirPath=%DstProjectDirPath%\include
set DstZipFilePath=%DstProjectDirPath%.zip

rmdir /s /q "%DstProjectDirPath%"

::begin, 不发布 Debug 版本的 NiflectGenTool
xcopy /y "%SrcBinDirPathRelease%\libclang.dll" "%DstBinDirPathDebug%\"
xcopy /y "%SrcBinDirPathRelease%\NiflectGen.dll" "%DstBinDirPathDebug%\"
xcopy /y "%SrcBinDirPathRelease%\NiflectGenCLI.exe" "%DstBinDirPathDebug%\"
xcopy /y "%SrcBinDirPathDebug%\Niflect.dll" "%DstBinDirPathDebug%\"
xcopy /y "%SrcBinDirPathDebug%\Niflect.pdb" "%DstBinDirPathDebug%\"
::end

xcopy /E /y "%SrcBinDirPathRelease%\" "%DstBinDirPathRelease%\"

xcopy /y "%LicenseFilePath%" "%DstProjectDirPath%\"

xcopy /E /y "%SrcIncludeDirPath%\" "%DstIncludeDirPath%\"

::---------------------------------------------------------------

set ToolDirPath=%cd%
set path=%ToolDirPath%;%path%
set UserName=WishingContributor
set Password=1
set StorageDirPath=http://192.168.245.158/sainimu78_Storage/
set Name7zExe=7za.exe
set Dst7zExeFilePath=%ToolDirPath%\%Name7zExe%
set DstZippedDirPath=%DstRootDirPath%\%ZipFileName%

curl -u %UserName%:%Password% -L -o "%Dst7zExeFilePath%" "%StorageDirPath%/Tool/Windows/%Name7zExe%"

del "%DstZipFilePath%"

7za a "%DstZipFilePath%" "%DstProjectDirPath%"

rmdir /s /q  "%DstProjectDirPath%"
del "%Dst7zExeFilePath%"

endlocal