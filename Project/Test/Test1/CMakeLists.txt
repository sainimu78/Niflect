set(ModuleName Test1)

#file(GLOB_RECURSE SrcAll *.cpp *.h)

add_executable(${ModuleName} SaveLoadByModuleInfo.cpp)

target_link_libraries(${ModuleName} PRIVATE Niflect)
target_link_libraries(${ModuleName} PRIVATE NiflectGen)

include(../Licensed_googletest.cmake)
add_test(NAME ${ModuleName} COMMAND ${ModuleName})

if(MSVC)
	set(WorkingDirPath ${CMAKE_BINARY_DIR}/${ModuleName})
	set_target_properties(${ModuleName} PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$<IF:$<CONFIG:Debug>,${WorkingDirPath},${WorkingDirPath}>"
	)
endif()