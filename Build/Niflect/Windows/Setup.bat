@echo off
setlocal

set ProjectName=Niflect
set ToolsetAndArch=%1
set ToolDirPath=%cd%
set path=%ToolDirPath%;%path%
set UserName=WishingContributor
set Password=1

::begin, FTP
::set SrcRootDirPath=ftp://192.168.245.158/Storage/ThirdParty/llvm-project
::end

set StorageDirPath=http://192.168.245.158/sainimu78_Storage/
set SrcRootDirPath=%StorageDirPath%/ThirdParty/llvm-project/Windows
set DstRootDirPath=%cd%\..\..\..\ThirdParty\libclang
set ZipFileName=llvm-project_17_0_6.zip
set DstZipFilePath=%DstRootDirPath%\%ZipFileName%
set DstExtractedDirPath=%DstRootDirPath%\llvm-project
set Name7zExe=7za.exe
set Dst7zExeFilePath=%ToolDirPath%\%Name7zExe%
set SrcBinPathDebug=%DstExtractedDirPath%\build\Windows\x64\Debug\bin
set DstBinPathDebug=%cd%\%ToolsetAndArch%\Debug\bin
set SrcBinPathRelease=%DstExtractedDirPath%\build\Windows\x64\Release\bin
set DstBinPathRelease=%cd%\%ToolsetAndArch%\Release\bin

curl -u %UserName%:%Password% -L -o "%Dst7zExeFilePath%" "%StorageDirPath%/Tool/Windows/%Name7zExe%"

::if not exist "%DstZipFilePath%" (
::wget -np -nH --cut-dirs=3 -O "%DstZipFilePath%" "%SrcRootDirPath%/%ZipFileName%"
curl -u %UserName%:%Password% -L -o "%DstZipFilePath%" "%SrcRootDirPath%/%ZipFileName%"
::)

rmdir /s /q "%DstExtractedDirPath%"
7za x "%DstZipFilePath%" -o"%DstRootDirPath%"
del "%DstZipFilePath%"
del "%Dst7zExeFilePath%"

xcopy /E /y "%SrcBinPathDebug%\libclang.dll" "%DstBinPathDebug%\"
xcopy /E /y "%SrcBinPathDebug%\libclang.pdb" "%DstBinPathDebug%\"

xcopy /E /y "%SrcBinPathRelease%\libclang.dll" "%DstBinPathRelease%\"

endlocal