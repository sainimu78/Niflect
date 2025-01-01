set(DstDownloadedFilePath ${v_ImportedLibRootDirPath}/${v_ZipFileName})
if(PROJECT_SETUP OR NOT EXISTS "${v_UnzippedDirPath}")
	download_zip_replace_dir_if_not_exists(${v_SrcAddrZipFilePath} ${DstDownloadedFilePath} ${v_UnzippedDirPath} IsDownloaded)
	if(IsDownloaded)
		file(COPY "${v_LibPlatformArchDirPath}/" DESTINATION "${c_ProjectPlatformArchDirPath}/")
	endif()
endif()

target_include_directories(${ModuleName} PRIVATE "${v_LibIncludeDirPaths}")

if(v_ImportedLibFilePathsDebug)
	target_link_libraries(${ModuleName} PRIVATE "$<$<CONFIG:Debug>:${v_ImportedLibFilePathsDebug}>")
endif()
if(v_ImportedLibFilePathsRelease)
	target_link_libraries(${ModuleName} PRIVATE "$<$<CONFIG:Release>:${v_ImportedLibFilePathsRelease}>")
endif()

set(LibDirPathDebug ${v_LibPlatformArchDirPath}/Debug/${c_LibDirName})
set(LibDirPathRelease ${v_LibPlatformArchDirPath}/Release/${c_LibDirName})
foreach(It ${v_ImportedLibFileNames})
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
	list(APPEND LibFilePathsDebug "${LibDirPathDebug}/${LibFileNameDebug}${LibFileExt}")
	list(APPEND LibFilePathsRelease "${LibDirPathRelease}/${LibFileNameRelease}${LibFileExt}")
endforeach()

target_link_libraries(${ModuleName} PRIVATE
	"$<$<CONFIG:Debug>:${LibFilePathsDebug}>"
	"$<$<CONFIG:Release>:${LibFilePathsRelease}>"
)

if(WIN32)
	set(BinDirPathDebug ${v_LibPlatformArchDirPath}/Debug/${c_BinDirName})
	set(BinDirPathRelease ${v_LibPlatformArchDirPath}/Release/${c_BinDirName})
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
	set(LibDirPathDebug ${v_LibPlatformArchDirPath}/Debug/${c_LibDirName})
	set(LibDirPathRelease ${v_LibPlatformArchDirPath}/Release/${c_LibDirName})
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

#begin, Required
set(v_ImportedLibName "")
set(v_ImportedLibRootDirPath "")
set(v_UnzippedDirPath "")
set(v_ZipFileName "")
set(v_SrcAddrZipFilePath "")
set(v_LibPlatformArchDirPath "")
set(v_LibIncludeDirPaths "")
set(v_LibPlatformArchIncludeDirPath "")
#end

#begin, Optional
set(v_DebugLibFileNamePostfix "")
set(v_ImportedLibFilePathsDebug "")
set(v_ImportedLibFilePathsRelease "")
set(v_ImportedLibFileNames "")
#end