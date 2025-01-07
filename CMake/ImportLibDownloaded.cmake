include(${c_RootCMakeDirPath}/InlineDownloadAndUnzip.cmake)

target_include_directories(${ModuleName} PRIVATE "${v_ListLibIncludeDirPathPrivate}")

if(v_ListImportedLibFilePathDebugPrivate)
	target_link_libraries(${ModuleName} PRIVATE "$<$<CONFIG:Debug>:${v_ListImportedLibFilePathDebugPrivate}>")
	list(APPEND LibFilePathsDebugPrivate ${v_ListImportedLibFilePathDebugPrivate})
endif()
if(v_ListImportedLibFilePathReleasePrivate)
	target_link_libraries(${ModuleName} PRIVATE "$<$<CONFIG:Release>:${v_ListImportedLibFilePathReleasePrivate}>")
	list(APPEND LibFilePathsReleasePrivate ${v_ListImportedLibFilePathReleasePrivate})
endif()

if(v_ListImportedLibFileName)
	set(LibDirPathDebug ${v_LibPlatformArchDirPath}/Debug/${c_LibDirName})
	set(LibDirPathRelease ${v_LibPlatformArchDirPath}/Release/${c_LibDirName})
	foreach(It ${v_ListImportedLibFileName})
		set(LibFileNameDebug ${It})
		set(LibFileNameRelease ${It})
		if(v_DebugLibFileNamePostfix)
			set(LibFileNameDebug ${LibFileNameDebug}${v_DebugLibFileNamePostfix})
		endif()
		set(LibFileExt )
		if(WIN32)
			set(LibFileExt ${c_StaticLibFileExt})
		else()
			if(NOT LibFileNameDebug MATCHES "^${c_SharedLibFileNamePrefix}")
				set(LibFileNameDebug ${c_SharedLibFileNamePrefix}${LibFileNameDebug})
			endif()
			if(NOT LibFileNameRelease MATCHES "^${c_SharedLibFileNamePrefix}")
				set(LibFileNameRelease ${c_SharedLibFileNamePrefix}${LibFileNameRelease})
			endif()
			set(LibFileExt ${c_SharedLibFileExt})
		endif()
		list(APPEND LibFilePathsDebugPrivate "${LibDirPathDebug}/${LibFileNameDebug}${LibFileExt}")
		list(APPEND LibFilePathsReleasePrivate "${LibDirPathRelease}/${LibFileNameRelease}${LibFileExt}")
	endforeach()

	target_link_libraries(${ModuleName} PRIVATE
		"$<$<CONFIG:Debug>:${LibFilePathsDebugPrivate}>"
		"$<$<CONFIG:Release>:${LibFilePathsReleasePrivate}>"
	)
endif()

if(WIN32)
	if(v_LibPlatformArchDirPath)
		set(BinDirPathDebug ${v_LibPlatformArchDirPath}/Debug/${c_BinDirName})
		set(BinDirPathRelease ${v_LibPlatformArchDirPath}/Release/${c_BinDirName})
		
		file(GLOB ListDeployingFilePathDebug "${BinDirPathDebug}/*")
		if(LibFilePathsDebugPrivate)
			list(GET ListDeployingFilePathDebug 0 Item0)
			get_filename_component(FileName "${Item0}" NAME)
			set(FilePathDebug ${ProjectBinDirPathDebug}/${FileName})
		endif()
		if(PROJECT_SETUP OR NOT EXISTS "${FilePathDebug}")
			message("Deploying: ${BinDirPathDebug} ${ProjectBinDirPathDebug}")
			file(COPY "${BinDirPathDebug}/" DESTINATION "${ProjectBinDirPathDebug}/")
		endif()
		
		file(GLOB ListDeployingFilePathRelease "${BinDirPathRelease}/*")
		if(LibFilePathsReleasePrivate)
			list(GET ListDeployingFilePathRelease 0 Item0)
			get_filename_component(FileName "${Item0}" NAME)
			set(FilePathRelease ${ProjectBinDirPathRelease}/${FileName})
		endif()
		if(PROJECT_SETUP OR NOT EXISTS "${FilePathRelease}")
			message("Deploying: ${BinDirPathRelease} ${ProjectBinDirPathRelease}")
			file(COPY "${BinDirPathRelease}/" DESTINATION "${ProjectBinDirPathRelease}/")
		endif()
		
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
	endif()
else()
	if(LibDirPathDebug)
		install(DIRECTORY "${LibDirPathDebug}/"
			DESTINATION "${c_ProjectInstallingTargetDirPathDebug}/${c_LibDirName}/"
			CONFIGURATIONS Debug
			#FILES_MATCHING PATTERN "*.so"
			USE_SOURCE_PERMISSIONS
		)
	endif()
	if(LibDirPathRelease)
		install(DIRECTORY "${LibDirPathRelease}/"
			DESTINATION "${c_ProjectInstallingTargetDirPathRelease}/${c_LibDirName}/"
			CONFIGURATIONS Release
			#FILES_MATCHING PATTERN "*.so"
			USE_SOURCE_PERMISSIONS
		)
	endif()
endif()

#begin, Required
set(v_ImportedLibRootDirPath "")
set(v_UnzippedDirPath "")
set(v_ZipFileName "")
set(v_SrcAddrZipFilePath "")
set(v_LibPlatformArchDirPath "")
set(v_ListLibIncludeDirPathPrivate "")
set(v_LibPlatformArchIncludeDirPath "")
#end

#begin, Optional
set(v_DebugLibFileNamePostfix "")
set(v_ListImportedLibFilePathDebugPrivate "")
set(v_ListImportedLibFilePathReleasePrivate "")
set(v_ListImportedLibFileName "")
#end