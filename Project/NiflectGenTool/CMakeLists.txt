cmake_minimum_required(VERSION 3.10.0)

include(../InitRootPath.cmake)

set(c_ProjectName NiflectGenTool)

include(${c_RootCMakeDirPath}/ProjectBegin.cmake)
project(${c_ProjectName})
include(${c_RootCMakeDirPath}/ProjectEnd.cmake)

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${c_ProjectName})
endif()

add_subdirectory(Niflect)
add_subdirectory(NiflectGen)
add_subdirectory(${c_ProjectName})

include(${c_RootCMakeDirPath}/Setup.cmake)
include(${c_RootCMakeDirPath}/Release.cmake)