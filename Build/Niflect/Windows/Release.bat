@echo off
setlocal

set ProjectName=Niflect
set ToolsetAndArch=%1

set DstRootDirPath=F:\sainimu78_Storage\Release\Windows\x64
set SrcBinDirPathDebug=%cd%\%ToolsetAndArch%\Debug\%ProjectName%
set SrcBinDirPathRelease=%cd%\%ToolsetAndArch%\Release\%ProjectName%
set DstProjectDirPathDebug=%DstRootDirPath%\Debug\%ProjectName%
set DstProjectDirPathRelease=%DstRootDirPath%\Release\%ProjectName%
set DstBinDirPathDebug=%DstProjectDirPathDebug%\bin
set DstBinDirPathRelease=%DstProjectDirPathRelease%\bin
set LicenseFilePath=%cd%\..\..\..\LICENSE.md
set SrcIncludeDirPath=%cd%\..\..\..\Source\%ProjectName%\include
set DstIncludeDirPathDebug=%DstProjectDirPathDebug%\include
set DstIncludeDirPathRelease=%DstProjectDirPathRelease%\include
set DstZipFilePathDebug=%DstProjectDirPathDebug%.zip
set DstZipFilePathRelease=%DstProjectDirPathRelease%.zip

rmdir /s /q "%DstProjectDirPathDebug%"
rmdir /s /q "%DstProjectDirPathRelease%"

::begin, 不发布 Debug 版本的 NiflectGenTool
xcopy /y "%SrcBinDirPathRelease%\libclang.dll" "%DstBinDirPathDebug%\"
xcopy /y "%SrcBinDirPathRelease%\NiflectGen.dll" "%DstBinDirPathDebug%\"
xcopy /y "%SrcBinDirPathRelease%\NiflectGenCLI.exe" "%DstBinDirPathDebug%\"
xcopy /y "%SrcBinDirPathDebug%\Niflect.dll" "%DstBinDirPathDebug%\"
xcopy /y "%SrcBinDirPathDebug%\Niflect.pdb" "%DstBinDirPathDebug%\"
::end

xcopy /E /y "%SrcBinDirPathRelease%\" "%DstBinDirPathRelease%\"

xcopy /y "%LicenseFilePath%" "%DstProjectDirPathDebug%\"
xcopy /y "%LicenseFilePath%" "%DstProjectDirPathRelease%\"

xcopy /E /y "%SrcIncludeDirPath%\" "%DstIncludeDirPathDebug%\"
xcopy /E /y "%SrcIncludeDirPath%\" "%DstIncludeDirPathRelease%\"

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

del "%DstZipFilePathDebug%"
del "%DstZipFilePathRelease%"

7za a "%DstZipFilePathDebug%" "%DstProjectDirPathDebug%"
7za a "%DstZipFilePathRelease%" "%DstProjectDirPathRelease%"

rmdir /s /q  "%DstProjectDirPathDebug%"
rmdir /s /q  "%DstProjectDirPathRelease%"
del "%Dst7zExeFilePath%"

endlocal