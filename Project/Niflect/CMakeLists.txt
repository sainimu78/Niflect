cmake_minimum_required(VERSION 3.10.0)

include(../InitRootPath.cmake)

set(c_ProjectName Niflect)

include(${c_RootCMakeProjectFrameworkDirPath}/ProjectBegin.cmake)
project(${c_ProjectName})
include(${c_RootCMakeProjectFrameworkDirPath}/ProjectEnd.cmake)

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${c_ProjectName})
endif()

add_subdirectory(Niflect)

include(${c_RootCMakeProjectFrameworkDirPath}/Release.cmake)
include(${c_RootCMakeProjectFrameworkDirPath}/Cleanup.cmake)
include(${c_RootCMakeProjectFrameworkDirPath}/GenerateReleaseNotes.cmake)