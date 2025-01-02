include(${c_RootCMakeDirPath}/IntegrateTool.cmake)

#begin, 区分 Debug 与 Release 两种版本的 GenTool, add_custom_command 中变量不用引号
#set(GenToolRootPath ${c_RootThirdPartyDirPath}/${v_IntegratedToolName}/${v_IntegratedToolName})
#set(ToolExeName ${v_IntegratedToolName}${c_ExecutableFileExt})
#set(GenToolBinDirPathDebug ${GenToolRootPath}/build/${c_ProjectPlatform}/${c_ProjectArch}/Debug/bin)
#set(GenToolBinDirPathRelease ${GenToolRootPath}/build/${c_ProjectPlatform}/${c_ProjectArch}/Release/bin)
#set(GenToolFilePathDebug ${GenToolBinDirPathDebug}/${ToolExeName})
#set(GenToolFilePathRelease ${GenToolBinDirPathRelease}/${ToolExeName})
#set(GenToolExeFilePath "$<$<CONFIG:Debug>:${GenToolFilePathDebug}>$<$<CONFIG:Release>:${GenToolFilePathRelease}>")
#end

set(ToolExeName ${v_IntegratedToolName}${c_ExecutableFileExt})
set(GenToolExeFilePath ${v_NiflectGenToolBinDirPath}/${ToolExeName})

set(NiflectRootPath ${c_RootThirdPartyDirPath}/Niflect/Niflect)
if(NOT EXISTS "${NiflectRootPath}")
	message(FATAL_ERROR "Not found: ${NiflectRootPath}")
endif()

set(GenOutputDirPath ${c_RootGeneratedDirPath}/${ModuleName})
set(GenSourcePrivate ${GenOutputDirPath}/_GenSource)
set(GenSourcePublic ${GenSourcePrivate}/include)
#beign, 用于模式 EGeneratingHeaderAndSourceFileMode::ESourceAndHeader, 将生成文件加到模块
#file(GLOB_RECURSE GeneratedSrc ${GenSourcePrivate}/*.cpp ${GenSourcePrivate}/*.h)
#create_source_group(${NiflectGeneratedRootPath} ${GeneratedSrc})
#list(APPEND SrcAll ${GeneratedSrc})
#end

target_include_directories(${ModuleName}
	PRIVATE ${GenSourcePrivate}
	PUBLIC ${GenSourcePublic}
)

set(ListOptModuleHeaders "")
foreach(It IN LISTS v_ListModuleHeaderFilePath)
    list(APPEND ListOptModuleHeaders "-h" "${It}")
endforeach()
set(ListOptModuleIncludeDirPath "")
foreach(It IN LISTS v_ListModuleIncludeDirPath)
	list(APPEND ListOptModuleIncludeDirPath "-I" "${It}")
endforeach()

set(ListOptAccessorSettingHeaders "")
foreach(It IN LISTS v_ListAccessorSettingHeaderFilePath)
	list(APPEND ListOptAccessorSettingHeaders "-a" "${It}")
endforeach()
set(ListOptModuleAPIMacro "")
if(v_ModuleAPIMacro)
    list(APPEND ListOptModuleAPIMacro "-am" "${v_ModuleAPIMacro}")
endif()
set(ListOptModuleAPIMacroHeader "")
if(v_ModuleAPIMacroHeaderFilePath)
    list(APPEND ListOptModuleAPIMacroHeader "-amh" "${v_ModuleAPIMacroHeaderFilePath}")
endif()

set(GeneratedModulePrivateH ${GenSourcePrivate}/${ModuleName}_private.h)

set(DebugIntegration OFF)
if(DebugIntegration)
	message(${ModuleName})
	foreach(It IN LISTS ListOptModuleHeaders)
		message(${It})
	endforeach()
	foreach(It IN LISTS ListOptModuleAPIMacro)
		message(${It})
	endforeach()
	foreach(It IN LISTS ListOptModuleAPIMacroHeader)
		message(${It})
	endforeach()
	foreach(It IN LISTS ListOptAccessorSettingHeaders)
		message(${It})
	endforeach()
	message("${NiflectRootPath}/include")
	foreach(It IN LISTS ListOptModuleIncludeDirPath)
		message(${It})
	endforeach()
endif()

#在模块 cmake 中, 如 Wishing\Project\TestEditorCLI\TestEngine\CMakeLists.txt
#定义变量 set(v_EnabledDebuggerAttaching ON) 即可启用可附加进程的调试模式
set(ListOptGenToolDebuggerAttaching "")
set(ListOptCmdCallingGenTool "")
if(v_EnabledDebuggerAttaching)
	list(APPEND ListOptCmdCallingGenTool cmd.exe /C start)
	list(APPEND ListOptGenToolDebuggerAttaching "-debuggerattaching")
endif()
if(WIN32)
else()
	set(GenToolLibDirPath "${v_NiflectGenToolBinDirPath}/../${c_LibDirName}")
	list(APPEND ListOptCmdCallingGenTool ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${GenToolLibDirPath}:${CMAKE_LIBRARY_PATH})
endif()

add_custom_command(
    OUTPUT "${GeneratedModulePrivateH}"
    COMMAND ${ListOptCmdCallingGenTool} "${GenToolExeFilePath}" 
            -n ${ModuleName} 
            ${ListOptModuleHeaders}
            ${ListOptModuleAPIMacro} 
            ${ListOptModuleAPIMacroHeader}
            ${ListOptAccessorSettingHeaders} 
            -t "${NiflectRootPath}/include" 
            ${ListOptModuleIncludeDirPath} 
            -g "${GenOutputDirPath}"
			-gbt 
			${ListOptGenToolDebuggerAttaching} 
    DEPENDS ${v_ListModuleHeaderFilePath}
    COMMENT "${v_IntegratedToolName}: ${ModuleName}"
)

set(GenToolTargetName ${v_IntegratedToolName}_${ModuleName})
add_custom_target(${GenToolTargetName} DEPENDS "${GeneratedModulePrivateH}")
set_target_properties(${GenToolTargetName} PROPERTIES FOLDER "AutoGen")
add_dependencies(${ModuleName} ${GenToolTargetName})

#begin, Required
set(v_IntegratedToolName "")
set(v_NiflectGenToolBinDirPath "")
set(v_ListAccessorSettingHeaderFilePath "")
set(v_ListModuleIncludeDirPath "")
#end

#begin, Optional
set(v_ModuleAPIMacro "")
set(v_ModuleAPIMacroHeaderFilePath "")
set(v_ListModuleHeaderFilePath "")
set(v_EnabledDebuggerAttaching "")#清理标志, 避免影响其它模块
#endif