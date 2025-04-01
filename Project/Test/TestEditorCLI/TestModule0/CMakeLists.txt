set(ModuleName TestModule0)

set(SourcePath ${c_RootSourceDirPath}/${ModuleName})

file(GLOB_RECURSE SrcAll ${SourcePath}/*.cpp ${SourcePath}/*.h)

create_source_group(${SourcePath} ${SrcAll})

add_library(${ModuleName} SHARED ${SrcAll})
#set_target_properties(${ModuleName} PROPERTIES 
#    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
#    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
#)

target_include_directories(${ModuleName}
	PUBLIC ${c_RootSourceDirPath}
)

target_compile_definitions(${ModuleName}
	PRIVATE -DTESTMODULE0_EXPORTS
	PRIVATE -DTESTMODULE0_C_EXPORTS
)

target_link_libraries(${ModuleName} PRIVATE TestEngine)

include(${c_TestProjectRootDirPath}/TestIntegrateNiflectGenTool.cmake)

#include(${c_ProjectDirPath}/Install.cmake)