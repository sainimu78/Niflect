set(ModuleName NiflectGenTool)

set(ModuleRootPath ${c_RootSourceDirPath}/${ModuleName})
set(SourcePath ${ModuleRootPath})

file(GLOB_RECURSE SrcAll ${SourcePath}/*.cpp ${SourcePath}/*.h)

create_source_group(${ModuleRootPath} ${SrcAll})

add_executable(${ModuleName} ${SrcAll})

target_link_libraries(${ModuleName} PRIVATE Niflect)
target_link_libraries(${ModuleName} PRIVATE NiflectGen)

target_include_directories(${ModuleName}
	PRIVATE ${c_RootSourceDirPath}
)

if(MSVC)
	set_target_properties(${ModuleName} PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$<IF:$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG},${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>"
	)
endif()

include(Licensed_CommandLineArgParser.cmake)

include(${c_ProjectDirPath}/Install.cmake)