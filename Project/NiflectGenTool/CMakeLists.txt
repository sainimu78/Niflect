cmake_minimum_required(VERSION 3.10.0)

include(../InitRootPath.cmake)

set(ProjectName NiflectGenTool)

include(${RootCMakeDirPath}/ProjectBegin.cmake)
project(${ProjectName})
include(${RootCMakeDirPath}/ProjectEnd.cmake)

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
endif()

add_subdirectory(Niflect)
add_subdirectory(NiflectGen)
add_subdirectory(${ProjectName})

include(${RootCMakeDirPath}/Setup.cmake)
include(${RootCMakeDirPath}/Release.cmake)