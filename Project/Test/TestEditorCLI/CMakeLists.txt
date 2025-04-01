set(ModuleName TestEditorCLI)

set(c_Test4RootDirPath ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${ModuleName})

include(../Licensed_googletest.cmake)
add_test(NAME ${ModuleName} COMMAND ${ModuleName})

if(MSVC)
	set(WorkingDirPath ${c_ProjectPlatformArchDirPath}/${ModuleName})
	set_target_properties(${ModuleName} PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$<IF:$<CONFIG:Debug>,${WorkingDirPath},${WorkingDirPath}>"
	)
endif()

add_subdirectory(TestEngine)
add_subdirectory(TestModule0)