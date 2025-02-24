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

include(${c_RootCMakeProjectFrameworkDirPath}/Cleanup.cmake)

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ModuleName})
	set_target_properties(${ModuleName} PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$<IF:$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG},${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>"
	)
endif()