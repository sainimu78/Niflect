cmake_minimum_required(VERSION 3.20.0)

include(../InitRootPath.cmake)

set(ProjectName Niflect)

include(${RootProjectDirPath}/CommonBeforeProject.cmake)
project(${ProjectName})
include(${RootProjectDirPath}/CommonAfterProject.cmake)

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
endif()

add_subdirectory(Niflect)

include(${RootProjectDirPath}/Release.cmake)