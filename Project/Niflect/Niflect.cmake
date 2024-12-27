set(ModuleName Niflect)

set(ModuleRootPath ${RootSourcePath}/Niflect)
set(SourcePath ${ModuleRootPath}/src)
file(GLOB_RECURSE Sources ${SourcePath}/*.cpp)
create_source_group(${SourcePath} ${Sources})
set(IncludePath ${RootSourcePath}/Niflect/include)
file(GLOB_RECURSE Headers ${IncludePath}/*.h)
create_source_group(${IncludePath} ${Headers})
set(SrcAll "")
list(APPEND SrcAll ${Sources})
list(APPEND SrcAll ${Headers})

add_library(${ModuleName} SHARED ${SrcAll})
set_target_properties(${ModuleName} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
)

target_include_directories(${ModuleName}
	PUBLIC ${IncludePath}
)

if(CMAKE_CXX_COMPILER MATCHES "c\\+\\+$")
	message(STATUS "Using GCC")
	#find_library(PTHREAD_LIB pthread)
	#if (PTHREAD_LIB)
	#	target_link_libraries(${ModuleName} PRIVATE ${PTHREAD_LIB})
	#endif()
	target_link_libraries(${ModuleName} PRIVATE pthread)
	target_link_libraries(${ModuleName} PRIVATE dl) # For loading module info using dlopen
elseif(CMAKE_CXX_COMPILER MATCHES "clang\\+\\+$")
	message(STATUS "Using Clang")
	
elseif(MSVC)
	message(STATUS "Using MSVC")
	
endif()

target_compile_definitions(${ModuleName} 
	PRIVATE -DNIFLECT_EXPORTS
)

set(LinkParentName ${ModuleName})