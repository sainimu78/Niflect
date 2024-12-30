cmake_minimum_required(VERSION 3.20.0)

include(${RootProjectDirPath}/Niflect/Niflect/Shared.cmake)

set(ListModuleInstallingHeaderDirPath "")
list(APPEND ListModuleInstallingHeaderDirPath ${ModuleHeaderDirPath})
include(${RootCMakeDirPath}/Install.cmake)