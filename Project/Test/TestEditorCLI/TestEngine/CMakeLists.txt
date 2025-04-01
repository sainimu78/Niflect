set(ModuleName TestEngine)

set(ModuleRootPath ${c_RootSourceDirPath}/${ModuleName})
set(ModuleSourcePath ${ModuleRootPath}/src)
set(ModuleIncludePath ${ModuleRootPath}/include)

set(IncludePathsPrivate "")
set(IncludePathsPublic "")
list(APPEND IncludePathsPrivate ${ModuleSourcePath})
list(APPEND IncludePathsPublic ${ModuleIncludePath})

file(GLOB_RECURSE ModuleSources ${ModuleSourcePath}/*.cpp ${ModuleSourcePath}/*.h)
create_source_group(${ModuleSourcePath} ${ModuleSources})
file(GLOB_RECURSE ModuleHeaders ${ModuleIncludePath}/*.h)
create_source_group(${ModuleIncludePath} ${ModuleHeaders})
set(SrcAll "")
list(APPEND SrcAll ${ModuleSources})
list(APPEND SrcAll ${ModuleHeaders})

add_library(${ModuleName} SHARED ${SrcAll})

target_precompile_headers(${ModuleName} PRIVATE ${ModuleSourcePath}/EngineDevelopmentMacro.h)

#set_target_properties(${ModuleName} PROPERTIES 
#    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
#    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
#)

target_include_directories(${ModuleName}
	PRIVATE ${IncludePathsPrivate}
	PUBLIC ${IncludePathsPublic}
)

target_compile_definitions(${ModuleName}
	PRIVATE -DENGINE_EXPORTS
	PRIVATE -DENGINE_C_EXPORTS
)

cpf_include(${c_Test4RootDirPath}/TestModule1/Shared.cmake)#cpf_include_install(${c_Test4RootDirPath}/TestModule1/Shared.cmake)

#target_link_libraries(${ModuleName} PRIVATE TestModule1)
ngt_target_link_libraries(TestEngine PRIVATE TestModule1)

list(APPEND v_ListAccessorSettingHeaderFilePath ${ModuleIncludePath}/EngineAccessorSetting.h)
#list(APPEND v_ListModuleIncludeDirPath ${IncludePathsPrivate})
#list(APPEND v_ListModuleIncludeDirPath ${IncludePathsPublic})
list(APPEND v_ListModuleHeaderFilePath ${ModuleHeaders})
#set(v_ModuleAPIMacro ENGINE_API)
#set(v_ModuleAPIMacroHeaderFilePath ${ModuleIncludePath}/Engine/EngineCommon.h)
#set(v_EnabledDebuggerAttaching ON)
include(${c_TestProjectRootDirPath}/TestIntegrateNiflectGenTool.cmake)

#include(${c_ProjectDirPath}/Install.cmake)