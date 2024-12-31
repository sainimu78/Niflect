cmake_minimum_required(VERSION 3.20.0)

include(${c_ProjectDirPath}/Niflect/Shared.cmake)

set(v_ListModuleInstallingHeaderDirPath "")
list(APPEND v_ListModuleInstallingHeaderDirPath ${ModuleHeaderDirPath})
include(${c_ProjectDirPath}/Install.cmake)