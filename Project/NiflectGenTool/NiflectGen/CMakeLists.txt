cmake_minimum_required(VERSION 3.20.0)

set(ModuleName NiflectGen)

set(ModuleRootPath ${RootSourceDirPath}/${ModuleName})
set(SourcePath ${ModuleRootPath})

file(GLOB_RECURSE SrcAll ${SourcePath}/*.cpp ${SourcePath}/*.h)

create_source_group(${ModuleRootPath} ${SrcAll})

add_library(${ModuleName} SHARED ${SrcAll})

target_precompile_headers(${ModuleName} PRIVATE ${SourcePath}/NiflectGenDevelopmentMacro.h)

set_target_properties(${ModuleName} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
)

target_include_directories(${ModuleName}
	PUBLIC ${RootSourceDirPath}
	PUBLIC ${SourcePath}
)

target_compile_definitions(${ModuleName}
	PRIVATE -DNIFLECTGEN_EXPORTS
)

target_link_libraries(${ModuleName} PRIVATE Niflect)

include(Licensed_libclang.cmake)

include(${ProjectDirPath}/Install.cmake)