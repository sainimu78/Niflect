@echo off
setlocal

set ProjectName=Niflect

set SrcDirPath=%cd%\..\..\Source\%ProjectName%\include\%ProjectName%
set DstDirPath=F:\sainimu78_Storage\Release\include\%ProjectName%

rmdir /s /q "%DstDirPath%"
xcopy /E /y "%SrcDirPath%\" "%DstDirPath%\"

endlocal