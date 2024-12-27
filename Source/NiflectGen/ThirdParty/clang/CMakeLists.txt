set(LibName libclang)
add_library(${LibName} SHARED IMPORTED)

if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
	set(LibPath "${DependenciesPath}/lib/llvm-project/x64")
	set(LibPathDebug "${LibPath}/Debug/lib")
	set(LibPathRelease "${LibPath}/Release/lib")
	target_link_directories(${LinkParentName} PRIVATE 
		$<$<CONFIG:Debug>:${LibPathDebug}>
		$<$<CONFIG:Release>:${LibPathRelease}>
	)
	
	#[[
	message(STATUS "was commentted")
	set_target_properties(${LibName} PROPERTIES
		IMPORTED_LOCATION_DEBUG "${LibPath}/Debug/bin/libclang.dll"
		IMPORTED_IMPLIB_DEBUG "${LibPath}/Debug/lib/libclang.lib"
		IMPORTED_LOCATION_RELEASE "${LibPath}/Release/bin/libclang.dll"
		IMPORTED_IMPLIB_RELEASE "${LibPath}/Release/lib/libclang.lib"
	)
	]]
else()
	message(ERROR "asdf")
endif()
