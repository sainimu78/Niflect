set(ModuleName Niflect)

set(ModuleRootDirPath ${c_RootSourceDirPath}/${ModuleName})
set(ModuleSourceDirPath ${ModuleRootDirPath}/src)
file(GLOB_RECURSE ModuleSources ${ModuleSourceDirPath}/*.cpp)
create_source_group(${ModuleSourceDirPath} ${ModuleSources})
set(ModuleHeaderDirPath ${ModuleRootDirPath}/include)
file(GLOB_RECURSE ModuleHeaders ${ModuleHeaderDirPath}/*.h)
create_source_group(${ModuleHeaderDirPath} ${ModuleHeaders})
set(SrcAll "")
list(APPEND SrcAll ${ModuleSources})
list(APPEND SrcAll ${ModuleHeaders})

add_library(${ModuleName} SHARED ${SrcAll})
#set_target_properties(${ModuleName} PROPERTIES 
#    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
#    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
#)

target_include_directories(${ModuleName}
	PUBLIC ${ModuleHeaderDirPath}
)

if(MSVC)
	message(STATUS "Using MSVC")
elseif(GCC)
	message(STATUS "Using GCC")
	#find_library(PTHREAD_LIB pthread)
	#if (PTHREAD_LIB)
	#	target_link_libraries(${ModuleName} PRIVATE ${PTHREAD_LIB})
	#endif()
	target_link_libraries(${ModuleName} PRIVATE pthread)
	target_link_libraries(${ModuleName} PRIVATE dl) # For loading module info using dlopen
elseif(CLANG)
	message(STATUS "Using Clang")
endif()

target_compile_definitions(${ModuleName} 
	PRIVATE -DNIFLECT_EXPORTS
)