
target_link_libraries(${ModuleName} PRIVATE Niflect)

set(IntegratedToolName NiflectGenTool)
list(APPEND v_ListIntegratedToolDependency ${IntegratedToolName})
set(v_IntegratedToolName ${IntegratedToolName})
set(v_NiflectGenToolBinDirPath "$<IF:$<CONFIG:Debug>,${c_ProjectBinDirPathDebug},${c_ProjectBinDirPathRelease}>")
set(v_NiflectRootPath ${c_RootSourceDirPath}/Niflect)
include(${c_RootCMakeProjectFrameworkDirPath}/IntegrateNiflectGenTool.cmake)