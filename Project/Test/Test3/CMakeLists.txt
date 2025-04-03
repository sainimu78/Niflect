set(ModuleName Test3)

set(ModuleRootDirPath ${CMAKE_CURRENT_SOURCE_DIR})
set(ModuleHeaders)
#file(GLOB_RECURSE ModuleHeaders *.h)
list(APPEND ModuleHeaders ${ModuleRootDirPath}/TestApp.h)
set(ModuleSources)
#file(GLOB_RECURSE ModuleSources *.cpp)
list(APPEND ModuleSources ${ModuleRootDirPath}/TestApp.cpp)

#set(ModuleAccessorSetting ${ModuleRootDirPath}/Test3AccessorSetting.h)
#list(APPEND ModuleHeaders ${ModuleAccessorSetting})
set(SrcAll)
list(APPEND SrcAll ${ModuleHeaders} ${ModuleSources})
add_executable(${ModuleName} ${SrcAll})

target_include_directories(${ModuleName} PRIVATE ${ModuleRootDirPath})
target_link_libraries(${ModuleName} PRIVATE TestLib)

#list(APPEND v_ListAccessorSettingHeaderFilePath ${ModuleAccessorSetting})
list(APPEND v_ListModuleHeaderFilePath ${ModuleHeaders})
#set(v_EnabledDebuggerAttaching ON)
include(${c_TestProjectRootDirPath}/TestIntegrateNiflectGenTool.cmake)

include(../Licensed_googletest.cmake)
add_test(NAME ${ModuleName} COMMAND ${ModuleName})

if(MSVC)
	set(WorkingDirPath ${c_ProjectPlatformArchDirPath}/${ModuleName})
	set_target_properties(${ModuleName} PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$<IF:$<CONFIG:Debug>,${WorkingDirPath},${WorkingDirPath}>"
	)
endif()

add_subdirectory(TestLib)