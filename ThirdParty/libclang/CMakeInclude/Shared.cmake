
add_library(libclang2 SHARED IMPORTED)

target_include_directories(${ModuleName} PRIVATE "${RootThirdPartyPath}/libclang/llvm-project/clang/include")

if(UNIX)
	message(STATUS "Target Is on UNIX")
	set(OsType Linux)
	set(DlPost .so)
	set(SlPost .a)
elseif(APPLE)
	message(STATUS "Target Is on APPLE")
	message(FATAL_ERROR "OsType & DynamicLibraryPrefix & DynamicLibraryPostfix are not specified")
elseif(WIN32)
	message(STATUS "Target Is on WIN32")
	set(OsType Windows)
	set(DlPost .dll)
	set(SlPost .lib)
endif()

if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
	if(WIN32)
		set(LibPath "${RootThirdPartyPath}/libclang/llvm-project/build/${OsType}/x64")
		set(libclangLibDebug "${LibPath}/Debug/bin/libclang${SlPost}")
		set(libclangLibRelease "${LibPath}/Release/bin/libclang${SlPost}")
		set_target_properties(libclang2 PROPERTIES
			IMPORTED_IMPLIB_DEBUG "${libclangLibDebug}"
			IMPORTED_IMPLIB_RELEASE "${libclangLibRelease}"
			IMPORTED_IMPLIB_RELWITHDEBINFO "${libclangLibDebug}"
			IMPORTED_IMPLIB_MINSIZEREL "${libclangLibRelease}"
		)
		target_include_directories(${ModuleName} PRIVATE "${LibPath}/include")
	else()
		set(libclangBinDebug "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/libclang${DlPost}")
		set(libclangBinRelease "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/libclang${DlPost}")
		set_target_properties(libclang2 PROPERTIES
			IMPORTED_LOCATION_DEBUG "${libclangBinDebug}"
			IMPORTED_LOCATION_RELEASE "${libclangBinRelease}"
			IMPORTED_LOCATION_RELWITHDEBINFO "${libclangBinDebug}"
			IMPORTED_LOCATION_MINSIZEREL "${libclangBinRelease}"
		)
	endif()
	
	target_link_libraries(${ModuleName} PRIVATE libclang2)
else()
	message(ERROR "asdf")
endif()
