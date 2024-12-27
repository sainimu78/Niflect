::参数[vs2015/vs2017/vs2022] [x64/Win32] [MD/MT]
@echo off

set AppName=NiflectGenTool
set AppOutputPath=%AppName%
set option_output_dir=%1
set option_generator_toolset=
if [%1]==[] (
    set option_visual_studio=Visual Studio 17 2022
	set option_output_dir=vs2022
	set option_generator_toolset=v143
) else if "%1"=="vs2015" (
    set option_visual_studio=Visual Studio 14 2015
) else if "%1"=="vs2017" (
    set option_visual_studio=Visual Studio 15 2017
	set option_output_dir=vs2017
	set option_generator_toolset=v140
) else (
    set option_visual_studio=Visual Studio 17 2022
	set option_output_dir=vs2022
	set option_generator_toolset=v143
)

if [%2]==[] (
	set option_platform=x64
) else if "%2"=="x64" (
	set option_platform=x64
) else (
	set option_platform=Win32
)

set option_visual_studio="%option_visual_studio%"

if [%3]==[] (
	set option_md=""
) else if "%3"=="MD" (
	set option_md="_md"
)

echo option_visual_studio:%option_visual_studio%
echo option_md:%option_md%
echo option_platform:%option_platform%

set OutputDirName=%option_output_dir%_%option_platform%%option_md%
mkdir %OutputDirName%
cd %OutputDirName%

::xcopy "..\..\%option_platform%\*.*" "lib\Release\" /exclude:..\excludedfileslist.txt /y
::xcopy "..\..\Dependencies\lib\PhysX\vc14%option_platform%%option_md%\Release\*.dll" "lib\Release\" /y

::xcopy "..\..\%option_platform%\*.*" "lib\Debug\" /y
::xcopy "..\..\Dependencies\lib\PhysX\vc14%option_platform%%option_md%\Debug\*.dll" "lib\Debug\" /y

::if 1==1 (
if 1 NEQ 1 (
	echo "====================Start Copy DLL ==========================="

	xcopy "..\..\..\..\ThirdParty\libclang\llvm-project\build\Windows\%option_platform%%option_md%\Debug\bin\libclang.dll" "Debug\%AppName%\" /y
	xcopy "..\..\..\..\ThirdParty\libclang\llvm-project\build\Windows\%option_platform%%option_md%\Debug\bin\libclang.pdb" "Debug\%AppName%\" /y

	xcopy "..\..\..\..\ThirdParty\libclang\llvm-project\build\Windows\%option_platform%%option_md%\Release\bin\libclang.dll" "Release\%AppName%\" /y
)

if "%option_md%" NEQ "" (
	SET DEFINES=-DIGNORE_MT=ON
	echo "====================cmake generate md project===================="
) else (
	echo "====================cmake generate project===================="
)

del /s /q "./CMakeCache.txt"
cmake ../../../../Project/%AppName% -G %option_visual_studio% -A %option_platform% -T %option_generator_toolset%
if %ERRORLEVEL% neq 0 (
    echo 运行cmake出错
    pause
)

cd ..
pause