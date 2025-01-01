
set(LibDirPathDebug ${v_LibPlatformArchDirPath}/Debug/${c_LibDirName})
set(LibDirPathRelease ${v_LibPlatformArchDirPath}/Release/${c_LibDirName})

set(LibFileNameDebug ${v_ImportedLibName})
set(LibFileNameRelease ${v_ImportedLibName})
if(WIN32)
else()
	if(NOT LibFileNameDebug MATCHES "^lib")
		set(LibFileNameDebug lib${LibFileNameDebug})
	endif()
	if(NOT LibFileNameRelease MATCHES "^lib")
		set(LibFileNameRelease lib${LibFileNameRelease})
	endif()
endif()
if(v_DebugLibFileNamePostfix)
	set(LibFileNameDebug ${LibFileNameDebug}${v_DebugLibFileNamePostfix})
endif()

set(LibFilePathPrefixDebug ${LibDirPathDebug}/${LibFileNameDebug})
set(LibFilePathPrefixRelease ${LibDirPathRelease}/${LibFileNameRelease})
if(WIN32)
	set(LinkingFilePathDebug ${LibFilePathPrefixDebug}${c_StaticLibFileExt})
	set(LinkingFilePathRelease ${LibFilePathPrefixRelease}${c_StaticLibFileExt})
else()
	set(LinkingFilePathDebug ${LibFilePathPrefixDebug}${c_SharedLibFileExt})
	set(LinkingFilePathRelease ${LibFilePathPrefixRelease}${c_SharedLibFileExt})
endif()
	
set(DstDownloadedFilePath ${v_ImportedLibRootDirPath}/${v_ZipFileName})
if(PROJECT_SETUP OR NOT EXISTS "${v_UnzippedDirPath}")
	download_zip_replace_dir_if_not_exists(${v_SrcAddrZipFilePath} ${DstDownloadedFilePath} ${v_UnzippedDirPath} IsDownloaded)
	if(IsDownloaded)
		file(COPY "${v_LibPlatformArchDirPath}/" DESTINATION "${c_ProjectPlatformArchDirPath}/")
	endif()
endif()

set(BinDirPathDebug ${v_LibPlatformArchDirPath}/Debug/${c_BinDirName})
set(BinDirPathRelease ${v_LibPlatformArchDirPath}/Release/${c_BinDirName})
# 区分不同 Configuration 是为支持仅以 Release 版本安装与发布
if(WIN32)
	install(DIRECTORY "${BinDirPathDebug}/"
		DESTINATION "${c_ProjectInstallingTargetDirPathDebug}/${c_BinDirName}/"
		CONFIGURATIONS Debug
		#FILES_MATCHING PATTERN "*.so"
		USE_SOURCE_PERMISSIONS
	)
	install(DIRECTORY "${BinDirPathRelease}/"
		DESTINATION "${c_ProjectInstallingTargetDirPathRelease}/${c_BinDirName}/"
		CONFIGURATIONS Release
		#FILES_MATCHING PATTERN "*.so"
		USE_SOURCE_PERMISSIONS
	)
else()
	install(DIRECTORY "${LibDirPathDebug}/"
		DESTINATION "${c_ProjectInstallingTargetDirPathDebug}/${c_LibDirName}/"
		CONFIGURATIONS Debug
		#FILES_MATCHING PATTERN "*.so"
		USE_SOURCE_PERMISSIONS
	)
	install(DIRECTORY "${LibDirPathRelease}/"
		DESTINATION "${c_ProjectInstallingTargetDirPathRelease}/${c_LibDirName}/"
		CONFIGURATIONS Release
		#FILES_MATCHING PATTERN "*.so"
		USE_SOURCE_PERMISSIONS
	)
endif()

if(v_LibIncludeDirPath)
	target_include_directories(${ModuleName} PRIVATE "${v_LibIncludeDirPath}")
endif()

set(LibTargetName ${v_ImportedLibName}_${ModuleName})
add_library(${LibTargetName} SHARED IMPORTED)
		
if(v_LibPlatformArchIncludeDirPath)
	target_include_directories(${ModuleName} PRIVATE "${v_LibPlatformArchDirPath}/include")
endif()

if(x64)
	if(WIN32)
		set_target_properties(${LibTargetName} PROPERTIES
			IMPORTED_IMPLIB_DEBUG "${LinkingFilePathDebug}"
			IMPORTED_IMPLIB_RELEASE "${LinkingFilePathRelease}"
			IMPORTED_IMPLIB_RELWITHDEBINFO "${LinkingFilePathDebug}"
			IMPORTED_IMPLIB_MINSIZEREL "${LinkingFilePathRelease}"
		)
	else()
		set(libclangBinDebug "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/${LibFileNameDebug}${c_SharedLibFileExt}")
		set(libclangBinRelease "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/${LibFileNameRelease}${c_SharedLibFileExt}")
		set_target_properties(${LibTargetName} PROPERTIES
			IMPORTED_LOCATION_DEBUG "${libclangBinDebug}"
			IMPORTED_LOCATION_RELEASE "${libclangBinRelease}"
			IMPORTED_LOCATION_RELWITHDEBINFO "${libclangBinDebug}"
			IMPORTED_LOCATION_MINSIZEREL "${libclangBinRelease}"
		)
	endif()
	
	target_link_libraries(${ModuleName} PRIVATE ${LibTargetName})
else()
	message(ERROR "Doesn't support ${v_ImportedLibName} for x86")
endif()

set(v_LibPlatformArchIncludeDirPath "")
set(v_DebugLibFileNamePostfix "")