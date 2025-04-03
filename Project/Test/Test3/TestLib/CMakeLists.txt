set(ModuleName TestLib)

set(ModuleRootDirPath ${CMAKE_CURRENT_SOURCE_DIR})
set(ModuleHeaders)
#file(GLOB_RECURSE ModuleHeaders *.h)
list(APPEND ModuleHeaders ${ModuleRootDirPath}/TestLib.h)
set(ModuleSources)
#file(GLOB_RECURSE ModuleSources *.cpp)
list(APPEND ModuleSources ${ModuleRootDirPath}/TestLib.cpp)

set(ModuleAccessorSetting ${ModuleRootDirPath}/TestLibAccessorSetting.h)
list(APPEND ModuleHeaders ${ModuleAccessorSetting})
set(SrcAll)
list(APPEND SrcAll ${ModuleHeaders} ${ModuleSources})
add_library(${ModuleName} SHARED ${SrcAll})

target_include_directories(${ModuleName} PUBLIC ${ModuleRootDirPath})
target_compile_definitions(${ModuleName} PRIVATE -DTESTLIB_EXPORTS)

list(APPEND v_ListAccessorSettingHeaderFilePath ${ModuleAccessorSetting})
list(APPEND v_ListModuleHeaderFilePath ${ModuleHeaders})
#set(v_EnabledDebuggerAttaching ON)
include(${c_TestProjectRootDirPath}/TestIntegrateNiflectGenTool.cmake)