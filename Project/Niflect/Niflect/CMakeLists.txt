include(${c_ProjectDirPath}/Niflect/Shared.cmake)

set(v_ListModuleInstallingHeaderDirPath "")
list(APPEND v_ListModuleInstallingHeaderDirPath ${ModuleHeaderDirPath})
include(${c_ProjectDirPath}/Install.cmake)