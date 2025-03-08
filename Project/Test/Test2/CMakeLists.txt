set(ModuleName Test2)

set(ModuleRootDirPath ${CMAKE_CURRENT_SOURCE_DIR})
set(ModuleHeaders)
file(GLOB_RECURSE ModuleHeaders *.h)
set(ModuleSources)
file(GLOB_RECURSE ModuleSources *.cpp)

set(Test2AccessorSetting ${ModuleRootDirPath}/Test2AccessorSetting.h)
list(APPEND ModuleHeaders ${Test2AccessorSetting})
set(SrcAll)
list(APPEND SrcAll ${ModuleHeaders} ${ModuleSources})
add_executable(${ModuleName} ${SrcAll})

target_include_directories(${ModuleName} PRIVATE ${ModuleRootDirPath})
target_link_libraries(${ModuleName} PRIVATE Niflect)

list(APPEND v_ListAccessorSettingHeaderFilePath ${Test2AccessorSetting})
list(APPEND v_ListModuleHeaderFilePath ${ModuleHeaders})
#set(v_EnabledDebuggerAttaching ON)

set(IntegratedToolName NiflectGenTool)
list(APPEND v_ListIntegratedToolDependency ${IntegratedToolName})
set(v_IntegratedToolName ${IntegratedToolName})
#set(v_IntegratedToolRootDirPath ${c_RootThirdPartyDirPath}/${v_IntegratedToolName})
#set(v_UnzippedDirPath ${v_IntegratedToolRootDirPath}/${v_IntegratedToolName})
#set(v_ZipFileName ${v_IntegratedToolName}.zip)
#set(v_SrcAddrZipFilePath ${c_StorageAddrPath}/Release/${c_ProjectPlatform}/${v_ZipFileName})
#set(v_NiflectGenToolBinDirPath ${c_ProjectBinDirPathDebug})
set(v_NiflectGenToolBinDirPath "$<IF:$<CONFIG:Debug>,${c_ProjectBinDirPathDebug},${c_ProjectBinDirPathRelease}>")
set(v_NiflectRootPath ${c_RootSourceDirPath}/Niflect)
include(${c_RootCMakeProjectFrameworkDirPath}/IntegrateNiflectGenTool.cmake)

include(../Licensed_googletest.cmake)
add_test(NAME ${ModuleName} COMMAND ${ModuleName})

if(MSVC)
	set(WorkingDirPath ${c_ProjectPlatformArchDirPath}/${ModuleName})
	set_target_properties(${ModuleName} PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$<IF:$<CONFIG:Debug>,${WorkingDirPath},${WorkingDirPath}>"
	)
endif()