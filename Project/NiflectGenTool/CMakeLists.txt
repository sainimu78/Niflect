cmake_minimum_required(VERSION 3.10.0)

include(../InitRootPath.cmake)

set(ProjectName NiflectGenTool)

include(${RootProjectDirPath}/CommonBeforeProject.cmake)
project(${ProjectName})
include(${RootProjectDirPath}/CommonAfterProject.cmake)

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
endif()

add_subdirectory(Niflect)
add_subdirectory(NiflectGen)
add_subdirectory(${ProjectName})

include(${RootProjectDirPath}/Setup.cmake)
include(${RootProjectDirPath}/Release.cmake)