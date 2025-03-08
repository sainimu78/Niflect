cmake_minimum_required(VERSION 3.20)

include(../InitRootPath.cmake)

set(c_ProjectName MyTestExample)

include(${c_RootCMakeProjectFrameworkDirPath}/ProjectBegin.cmake)
project(${c_ProjectName})
include(${c_RootCMakeProjectFrameworkDirPath}/ProjectEnd.cmake)

enable_testing() 

set(ModuleName MyTestExample_0)
add_executable(${ModuleName} TestExample_0.cpp)
include(Licensed_googletest.cmake)
add_test(NAME ${ModuleName} COMMAND ${ModuleName}) # 支持 ctest

set(ModuleName MyTestExample_1)
add_executable(${ModuleName} TestExample_1.cpp)
include(Licensed_googletest.cmake)
add_test(NAME ${ModuleName} COMMAND ${ModuleName}) # 支持 ctest

cpf_include(${c_RootProjectDirPath}/Niflect/Niflect/Shared.cmake)
cpf_include(${c_RootProjectDirPath}/NiflectGenTool/NiflectGen/Shared.cmake)
cpf_include(${c_RootProjectDirPath}/NiflectGenTool/NiflectGenTool/Exe.cmake)

add_subdirectory(Test0)
add_subdirectory(Test1)
add_subdirectory(Test2)
set(DefaultModuleName Test2)

include(${c_RootCMakeProjectFrameworkDirPath}/Cleanup.cmake)

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${DefaultModuleName})
endif()