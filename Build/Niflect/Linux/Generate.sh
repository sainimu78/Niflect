#!/bin/bash

AppName="Niflect"
AppOutputPath=$AppName
option_output_dir=$1
option_generator_toolset=""
option_generator_toolset_Predefines=

if [ -z "$1" ]; then
    option_visual_studio="Unix Makefiles"
    option_output_dir="Make"
	option_generator_toolset="DefaultGCC"
	#指定工具链如下, 现用默认即可
    #option_generator_toolset_Predefines="-DCMAKE_C_COMPILER=D:/HenDuoDongXi/perl/c/bin/gcc.exe -DCMAKE_CXX_COMPILER=D:/HenDuoDongXi/perl/c/bin/g++.exe"
elif [ "$1" == "vs2015" ]; then
    option_visual_studio="Visual Studio 14 2015"
elif [ "$1" == "vs2017" ]; then
    option_visual_studio="Visual Studio 15 2017"
    option_generator_toolset="v140"
else
    option_visual_studio="Visual Studio 17 2022"
    option_output_dir="vs2022"
    option_generator_toolset="v143"
fi

if [ -z "$2" ]; then
	export CFLAGS="-m64"
	export CXXFLAGS="-m64"
    option_platform="x64"
elif [ "$2" == "x64" ]; then
    option_platform="x64"
else
	export CFLAGS="-m32"
	export CXXFLAGS="-m32"
    option_platform="Win32"
fi

if [ -z "$3" ]; then
    option_md=""
elif [ "$3" == "MD" ]; then
    option_md="_md"
fi

echo "option_visual_studio: $option_visual_studio"
echo "option_md: $option_md"
echo "option_platform: $option_platform"

OutputDirName="${option_output_dir}_${option_platform}${option_md}"

aaaaaaaaa=$(pwd)
OutputDirNameDebug=$OutputDirName/Debug
OutputDirNameRelease=$OutputDirName/Release
mkdir -p $OutputDirNameDebug
mkdir -p $OutputDirNameRelease
cd $OutputDirNameDebug

# xcopy commands transformed to cp commands (adjust paths and patterns as necessary)
# cp -r "../../${option_platform}/*.*" "lib/Release/" --exclude="../excludedfileslist.txt"
# cp "../../Dependencies/lib/PhysX/vc14${option_platform}${option_md}/Release/*.dll" "lib/Release/"

# cp -r "../../${option_platform}/*.*" "lib/Debug/"
# cp "../../Dependencies/lib/PhysX/vc14${option_platform}${option_md}/Debug/*.dll" "lib/Debug/"

if true; then
    echo "==================== Start Copy DLL ==========================="
    cp "../../Dependencies/lib/llvm-project/${option_platform}${option_md}/Debug/bin/libclang.dll" "${AppOutputPath}/Debug/" -y
    cp "../../Dependencies/lib/llvm-project/${option_platform}${option_md}/Debug/bin/libclang.pdb" "${AppOutputPath}/Debug/" -y
    cp "../../Dependencies/lib/llvm-project/${option_platform}${option_md}/Release/bin/libclang.dll" "${AppOutputPath}/Release/" -y
fi

if [ "$option_md" != "" ]; then
    DEFINES="-DIGNORE_MT=ON"
    echo "==================== cmake generate md project ===================="
else
    echo "==================== cmake generate project ===================="
fi

rm -f "./CMakeCache.txt"
#cmake ../../Project/Interedit -G $option_visual_studio -A $option_platform -T $option_generator_toolset
#eval $option_platform
#cmake ../../../Project/Interedit -G "Unix Makefiles" $option_generator_toolset_Predefines
cmake ../../../../../Project/$AppName -G "$option_visual_studio" $option_generator_toolset_Predefines -DCMAKE_BUILD_TYPE=Debug

cd $aaaaaaaaa
cd $OutputDirNameRelease
cmake ../../../../../Project/$AppName -G "$option_visual_studio" $option_generator_toolset_Predefines -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "运行cmake出错"
    read -p "Press Enter to continue..."
fi

cd ..
#read -p "Press Enter to continue..."
