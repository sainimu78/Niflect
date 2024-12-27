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

xcopy /y "%LicenseFilePath%" "%DstBinDirPathDebug%\"
xcopy /y "%LicenseFilePath%" "%DstBinDirPathRelease%\"

xcopy /E /y "%SrcIncludeDirPath%\" "%DstIncludeDirPathDebug%\"
xcopy /E /y "%SrcIncludeDirPath%\" "%DstIncludeDirPathRelease%\"

endlocal