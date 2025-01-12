set(ModuleName NiflectGen)

set(ModuleRootPath ${c_RootSourceDirPath}/${ModuleName})
set(SourcePath ${ModuleRootPath})

file(GLOB_RECURSE SrcAll ${SourcePath}/*.cpp ${SourcePath}/*.h)

create_source_group(${ModuleRootPath} ${SrcAll})

add_library(${ModuleName} SHARED ${SrcAll})

target_precompile_headers(${ModuleName} PRIVATE ${SourcePath}/NiflectGenDevelopmentMacro.h)

#set_target_properties(${ModuleName} PROPERTIES 
#    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
#    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
#)

target_include_directories(${ModuleName}
	PUBLIC ${c_RootSourceDirPath}
	PUBLIC ${SourcePath}
)

target_compile_definitions(${ModuleName}
	PRIVATE -DNIFLECTGEN_EXPORTS
)

target_link_libraries(${ModuleName} PRIVATE Niflect)

include(Licensed_libclang.cmake)

include(${c_ProjectDirPath}/Install.cmake)