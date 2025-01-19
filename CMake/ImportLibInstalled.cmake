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
	set(LibDirPathDebug ${v_InstalledLibDirPathDebug})
	set(LibDirPathRelease ${v_InstalledLibDirPathRelease})
	if(NOT LibDirPathDebug)
		set(LibDirPathDebug ${v_LibPlatformArchDirPath}/Debug/${c_LibDirName})
	endif()
	if(NOT LibDirPathRelease)
		set(LibDirPathRelease ${v_LibPlatformArchDirPath}/Release/${c_LibDirName})
	endif()
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

#begin, Required
unset(v_ListLibIncludeDirPathPrivate)
#end

#begin, Optional
unset(v_LibPlatformArchDirPath)
unset(v_DebugLibFileNamePostfix)
unset(v_ListImportedLibFilePathDebugPrivate)
unset(v_ListImportedLibFilePathReleasePrivate)
unset(v_ListImportedLibFileName)
unset(v_InstalledLibDirPathDebug)
unset(v_InstalledLibDirPathRelease)
#end