cmake_minimum_required(VERSION 3.20.0)

include(${ProjectDirPath}/Niflect/Shared.cmake)

set(ListModuleInstallingHeaderDirPath "")
list(APPEND ListModuleInstallingHeaderDirPath ${ModuleHeaderDirPath})
include(${ProjectDirPath}/Install.cmake)