set(LibName libclang)
set(ThirdPartyLibDirPath ${RootThirdPartyDirPath}/${LibName})
set(LibRootDirPath ${ThirdPartyLibDirPath}/llvm-project)
set(LibTargetName ${LibName}_${ModuleName})

set(LibPlatformArchDirPath ${LibRootDirPath}/build/${ProjectPlatform}/${ProjectArch})
set(BinPrefixDirPathDebug ${LibPlatformArchDirPath}/Debug/bin)
set(BinPrefixDirPathRelease ${LibPlatformArchDirPath}/Release/bin)
set(BinFilePathDebug ${BinPrefixDirPathDebug}/${LibName}${ProjectSharedLibPostfix})
set(BinFilePathRelease ${BinPrefixDirPathRelease}/${LibName}${ProjectSharedLibPostfix})
if(WIN32)
	set(LinkingFilePathDebug ${LibPlatformArchDirPath}/Debug/lib/${LibName}${ProjectStaticLibPostfix})
	set(LinkingFilePathRelease ${LibPlatformArchDirPath}/Release/lib/${LibName}${ProjectStaticLibPostfix})
	set(PdbFilePath ${BinPrefixDirPathDebug}/${LibName}.pdb)
else()
	set(LinkingFilePathDebug ${BinFilePathDebug})
	set(LinkingFilePathRelease ${BinFilePathRelease})
endif()
	
set(ZipFileName llvm-project_17_0_6.zip)
set(SrcAddrZipFilePath ${StorageAddrPath}/ThirdParty/llvm-project/${ProjectPlatform}/${ZipFileName})
set(DstDownloadedFilePath ${ThirdPartyLibDirPath}/${ZipFileName})
if(PROJECT_SETUP OR NOT EXISTS "${LibRootDirPath}")
	include(${RootCMakeDirPath}/GetZip.cmake)
	download_zip_replace_dir(${ExeFilePathZip} ${SrcAddrZipFilePath} ${DstDownloadedFilePath} ${LibRootDirPath})
	
	file(COPY "${BinFilePathDebug}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
	file(COPY "${BinFilePathRelease}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")
	if(WIN32)
		file(COPY "${LinkingFilePathDebug}" DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}")
		file(COPY "${LinkingFilePathRelease}" DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}")
		file(COPY "${PdbFilePath}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
	endif()
endif()

install(FILES "${BinFilePathDebug}" DESTINATION "${ProjectInstallingTargetDirPathDebug}/bin" CONFIGURATIONS Debug)
install(FILES "${BinFilePathRelease}" DESTINATION "${ProjectInstallingTargetDirPathRelease}/bin" CONFIGURATIONS Release)
if(WIN32)
	install(FILES "${PdbFilePath}"
		DESTINATION "${ProjectInstallingTargetDirPath}/bin"
		CONFIGURATIONS Debug
	)
endif()
	
add_library(${LibTargetName} SHARED IMPORTED)

target_include_directories(${ModuleName} PRIVATE "${LibRootDirPath}/clang/include")

#if(UNIX)
#	message(STATUS "Target Is on UNIX")
#	set(OsType Linux)
#	set(DlPost .so)
#	set(SlPost .a)
#elseif(APPLE)
#	message(STATUS "Target Is on APPLE")
#	message(FATAL_ERROR "OsType & DynamicLibraryPrefix & DynamicLibraryPostfix are not specified")
#elseif(WIN32)
#	message(STATUS "Target Is on WIN32")
#	set(OsType Windows)
#	set(DlPost .dll)
#	set(SlPost .lib)
#endif()

if(x64)
	if(WIN32)
		set_target_properties(${LibTargetName} PROPERTIES
			IMPORTED_IMPLIB_DEBUG "${LinkingFilePathDebug}"
			IMPORTED_IMPLIB_RELEASE "${LinkingFilePathRelease}"
			IMPORTED_IMPLIB_RELWITHDEBINFO "${LinkingFilePathDebug}"
			IMPORTED_IMPLIB_MINSIZEREL "${LinkingFilePathRelease}"
		)
		target_include_directories(${ModuleName} PRIVATE "${LibPlatformArchDirPath}/include")
	else()
		set(libclangBinDebug "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${${LibName}}${ProjectSharedLibPostfix}")
		set(libclangBinRelease "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${${LibName}}${ProjectSharedLibPostfix}")
		set_target_properties(${LibTargetName} PROPERTIES
			IMPORTED_LOCATION_DEBUG "${libclangBinDebug}"
			IMPORTED_LOCATION_RELEASE "${libclangBinRelease}"
			IMPORTED_LOCATION_RELWITHDEBINFO "${libclangBinDebug}"
			IMPORTED_LOCATION_MINSIZEREL "${libclangBinRelease}"
		)
	endif()
	
	target_link_libraries(${ModuleName} PRIVATE ${LibTargetName})
else()
	message(ERROR "Doesn't support libclang for x86")
endif()