set(LibName libclang)
set(ThirdPartyLibDirPath ${RootThirdPartyDirPath}/${LibName})
set(LibRootDirPath ${ThirdPartyLibDirPath}/llvm-project)
set(ZipFileName llvm-project_17_0_6.zip)
set(SrcAddrZipFilePath ${StorageAddrPath}/ThirdParty/llvm-project/${ProjectPlatform}/${ZipFileName})

set(LibPlatformArchDirPath ${LibRootDirPath}/build/${ProjectPlatform}/${ProjectArch})
set(LibDirPathDebug ${LibPlatformArchDirPath}/Debug/${LibDirName})
set(LibDirPathRelease ${LibPlatformArchDirPath}/Release/${LibDirName})

set(LibFilePathPrefixDebug ${LibDirPathDebug}/${LibName})
set(LibFilePathPrefixRelease ${LibDirPathRelease}/${LibName})
if(WIN32)
	set(LinkingFilePathDebug ${LibFilePathPrefixDebug}${ProjectStaticLibPostfix})
	set(LinkingFilePathRelease ${LibFilePathPrefixRelease}${ProjectStaticLibPostfix})
else()
	set(LinkingFilePathDebug ${LibFilePathPrefixDebug}${ProjectSharedLibPostfix})
	set(LinkingFilePathRelease ${LibFilePathPrefixRelease}${ProjectSharedLibPostfix})
endif()
	
set(DstDownloadedFilePath ${ThirdPartyLibDirPath}/${ZipFileName})
if(PROJECT_SETUP OR NOT EXISTS "${LibRootDirPath}")
	download_zip_replace_dir(${SrcAddrZipFilePath} ${DstDownloadedFilePath} ${LibRootDirPath})
	file(COPY "${LibPlatformArchDirPath}/" DESTINATION "${ProjectPlatformArchDirPath}/")
endif()

set(BinDirPathDebug ${LibPlatformArchDirPath}/Debug/${BinDirName})
set(BinDirPathRelease ${LibPlatformArchDirPath}/Release/${BinDirName})

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

target_include_directories(${ModuleName} PRIVATE "${LibRootDirPath}/clang/include")

set(LibTargetName ${LibName}_${ModuleName})
add_library(${LibTargetName} SHARED IMPORTED)

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