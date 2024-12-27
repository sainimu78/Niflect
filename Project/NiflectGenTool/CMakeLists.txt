cmake_minimum_required(VERSION 3.10.0)

include(../RootPathSetup.cmake)

set(ProjectName NiflectGenTool)
include(${RootProjectPath}/CommonBeforeProject.cmake)
project(${ProjectName})
include(${RootProjectPath}/CommonAfterProject.cmake)

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})
endif()

add_subdirectory(Niflect)
add_subdirectory(NiflectGen)
add_subdirectory(${ProjectName})
