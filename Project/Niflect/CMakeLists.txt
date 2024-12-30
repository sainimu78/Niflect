cmake_minimum_required(VERSION 3.20.0)

include(../InitRootPath.cmake)

set(ProjectName Niflect)

include(${RootCMakeDirPath}/CommonBeforeProject.cmake)
project(${ProjectName})
include(${RootCMakeDirPath}/CommonAfterProject.cmake)

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
endif()

add_subdirectory(Niflect)

include(${RootCMakeDirPath}/Release.cmake)