cmake_minimum_required(VERSION 3.20)

set(ModuleRootPath ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(c_RootDirPath "${ModuleRootPath}/../../../.." ABSOLUTE)
set(c_RootCMakeProjectFrameworkDirPath ${c_RootDirPath}/ThirdParty/CMakeProjectFramework/CMakeInclude)
include(${c_RootCMakeProjectFrameworkDirPath}/InitDefaultRootPaths.cmake)

set(c_ProjectName BeingTestedExe)

include(${c_RootCMakeProjectFrameworkDirPath}/ProjectBegin.cmake)
project(${c_ProjectName})
include(${c_RootCMakeProjectFrameworkDirPath}/ProjectEnd.cmake)

set(ModuleName BeingTestedExe)

file(GLOB_RECURSE SrcAll ${ModuleRootPath}/*.cpp ${ModuleRootPath}/*.h)
create_source_group(${ModuleRootPath} ${SrcAll})
add_executable(${ModuleName} ${SrcAll})

get_filename_component(RootGenSourceDirPath "${c_ProjectPlatformArchDirPath}/../NiflectGenerated/_GenSource" ABSOLUTE)

target_include_directories(${ModuleName} PRIVATE
	${RootGenSourceDirPath}
	${RootGenSourceDirPath}/include
	)

target_link_libraries(${ModuleName} PRIVATE Niflect)

add_subdirectory(Niflect)

include(${c_RootCMakeProjectFrameworkDirPath}/Cleanup.cmake)

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ModuleName})
	set_target_properties(${ModuleName} PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$<IF:$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG},${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>"
	)
endif()