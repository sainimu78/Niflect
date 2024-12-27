@echo off
setlocal

set ProjectName=NiflectGenTool
set ToolsetAndArch=%1

set DstRootDirPath=F:\sainimu78_Storage\Release\Windows\x64
set SrcDirPathDebug=%cd%\%ToolsetAndArch%\Debug\%ProjectName%
set SrcDirPathRelease=%cd%\%ToolsetAndArch%\Release\%ProjectName%
set DstDirPathDebug=%DstRootDirPath%\Debug\%ProjectName%
set DstDirPathRelease=%DstRootDirPath%\Release\%ProjectName%

::begin, 不发布 Debug 版本的 NiflectGenTool
xcopy /E /y "%SrcDirPathRelease%\libclang.dll" "%DstDirPathDebug%\"
xcopy /E /y "%SrcDirPathRelease%\NiflectGen.dll" "%DstDirPathDebug%\"
xcopy /E /y "%SrcDirPathRelease%\NiflectGenTool.exe" "%DstDirPathDebug%\"
xcopy /E /y "%SrcDirPathDebug%\Niflect.dll" "%DstDirPathDebug%\"
::end

xcopy /E /y "%SrcDirPathRelease%\" "%DstDirPathRelease%\"

endlocal