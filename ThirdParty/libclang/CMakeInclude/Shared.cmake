set(LibName libclang)
set(ThirdPartyLibDirPath ${RootThirdPartyDirPath}/${LibName})
set(LibRootDirPath ${ThirdPartyLibDirPath}/llvm-project)
set(LibTargetName ${LibName}_${ModuleName})

set(LibPlatformArchDirPath ${LibRootDirPath}/build/${ProjectPlatform}/${ProjectArch})
set(BinDirPathDebug ${LibPlatformArchDirPath}/Debug/${BinDirName})
set(BinDirPathRelease ${LibPlatformArchDirPath}/Release/${BinDirName})
set(LibDirPathDebug ${LibPlatformArchDirPath}/Debug/${LibDirName})
set(LibDirPathRelease ${LibPlatformArchDirPath}/Release/${LibDirName})
#set(BinFilePathPrefixDebug ${LibPlatformArchDirPath}/Debug/${BinDirName}/${LibName})
#set(BinFilePathPrefixRelease ${LibPlatformArchDirPath}/Release/${BinDirName}/${LibName})
set(LibFilePathPrefixDebug ${LibPlatformArchDirPath}/Debug/${LibDirName}/${LibName})
set(LibFilePathPrefixRelease ${LibPlatformArchDirPath}/Release/${LibDirName}/${LibName})
#set(BinFilePathDebug ${BinFilePathPrefixDebug}${ProjectSharedLibPostfix})
#set(BinFilePathRelease ${BinFilePathPrefixRelease}${ProjectSharedLibPostfix})
if(WIN32)
	set(LinkingFilePathDebug ${LibFilePathPrefixDebug}${ProjectStaticLibPostfix})
	set(LinkingFilePathRelease ${LibFilePathPrefixRelease}${ProjectStaticLibPostfix})
	#set(PdbFilePath ${BinFilePathPrefixDebug}.pdb)
else()
	set(LinkingFilePathDebug ${LibFilePathPrefixDebug}${ProjectSharedLibPostfix})
	set(LinkingFilePathRelease ${LibFilePathPrefixRelease}${ProjectSharedLibPostfix})
endif()
	
set(ZipFileName llvm-project_17_0_6.zip)
set(SrcAddrZipFilePath ${StorageAddrPath}/ThirdParty/llvm-project/${ProjectPlatform}/${ZipFileName})
set(DstDownloadedFilePath ${ThirdPartyLibDirPath}/${ZipFileName})
if(PROJECT_SETUP OR NOT EXISTS "${LibRootDirPath}")
	download_zip_replace_dir(${SrcAddrZipFilePath} ${DstDownloadedFilePath} ${LibRootDirPath})
	#file(COPY "${LinkingFilePathDebug}" DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}")
	#file(COPY "${LinkingFilePathRelease}" DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}")
	#if(WIN32)
	#	file(COPY "${BinFilePathDebug}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
	#	file(COPY "${BinFilePathRelease}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")
	#	file(COPY "${PdbFilePath}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
	#endif()
	
	#file(COPY "${BinDirPathDebug}/" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/")
	#file(COPY "${BinDirPathRelease}/" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/")
	#file(COPY "${LibDirPathDebug}/" DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/")
	#file(COPY "${LibDirPathRelease}/" DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/")
	
	file(COPY "${LibPlatformArchDirPath}/" DESTINATION "${ProjectPlatformArchDirPath}/")
	
endif()

#if(WIN32)
#	install(FILES "${BinFilePathDebug}" DESTINATION "${ProjectInstallingTargetDirPathDebug}/${BinDirName}" CONFIGURATIONS Debug)
#	install(FILES "${BinFilePathRelease}" DESTINATION "${ProjectInstallingTargetDirPathRelease}/${BinDirName}" CONFIGURATIONS Release)
#	install(FILES "${PdbFilePath}"
#		DESTINATION "${ProjectInstallingTargetDirPath}/${BinDirName}"
#		CONFIGURATIONS Debug
#	)
#else()
#	install(FILES "${LinkingFilePathDebug}" DESTINATION "${ProjectInstallingTargetDirPathDebug}/${LibDirName}" CONFIGURATIONS Debug)
#	install(FILES "${LinkingFilePathRelease}" DESTINATION "${ProjectInstallingTargetDirPathRelease}/${LibDirName}" CONFIGURATIONS Release)
#endif()

# 区分不同 Configuration 是为支持仅以 Release 版本安装与发布
if(WIN32)
	install(DIRECTORY "${BinDirPathDebug}/"
		DESTINATION "${ProjectInstallingTargetDirPathDebug}/${BinDirName}/"
		CONFIGURATIONS Debug
		#FILES_MATCHING PATTERN "*.so"
		USE_SOURCE_PERMISSIONS
	)
	install(DIRECTORY "${BinDirPathRelease}/"
		DESTINATION "${ProjectInstallingTargetDirPathRelease}/${BinDirName}/"
		CONFIGURATIONS Release
		#FILES_MATCHING PATTERN "*.so"
		USE_SOURCE_PERMISSIONS
	)
else()
	install(DIRECTORY "${LibDirPathDebug}/"
		DESTINATION "${ProjectInstallingTargetDirPathDebug}/${LibDirName}/"
		CONFIGURATIONS Debug
		#FILES_MATCHING PATTERN "*.so"
		USE_SOURCE_PERMISSIONS
	)
	install(DIRECTORY "${LibDirPathRelease}/"
		DESTINATION "${ProjectInstallingTargetDirPathRelease}/${LibDirName}/"
		CONFIGURATIONS Release
		#FILES_MATCHING PATTERN "*.so"
		USE_SOURCE_PERMISSIONS
	)
endif()

add_library(${LibTargetName} SHARED IMPORTED)

target_include_directories(${ModuleName} PRIVATE "${LibRootDirPath}/clang/include")

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
		set(libclangBinDebug "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/${LibName}${ProjectSharedLibPostfix}")
		set(libclangBinRelease "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/${LibName}${ProjectSharedLibPostfix}")
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