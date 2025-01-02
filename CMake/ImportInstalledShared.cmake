target_include_directories(${ModuleName} PRIVATE "${v_ListLibIncludeDirPath}")

if(v_ListImportedLibFilePathDebug)
	target_link_libraries(${ModuleName} PRIVATE "$<$<CONFIG:Debug>:${v_ListImportedLibFilePathDebug}>")
endif()
if(v_ListImportedLibFilePathRelease)
	target_link_libraries(${ModuleName} PRIVATE "$<$<CONFIG:Release>:${v_ListImportedLibFilePathRelease}>")
endif()

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
	list(APPEND LibFilePathsDebug "${LibDirPathDebug}/${LibFileNameDebug}${LibFileExt}")
	list(APPEND LibFilePathsRelease "${LibDirPathRelease}/${LibFileNameRelease}${LibFileExt}")
endforeach()

target_link_libraries(${ModuleName} PRIVATE
	"$<$<CONFIG:Debug>:${LibFilePathsDebug}>"
	"$<$<CONFIG:Release>:${LibFilePathsRelease}>"
)

#begin, Required
set(v_ListLibIncludeDirPath "")
#end

#begin, Optional
set(v_LibPlatformArchDirPath "")
set(v_DebugLibFileNamePostfix "")
set(v_ListImportedLibFilePathDebug "")
set(v_ListImportedLibFilePathRelease "")
set(v_ListImportedLibFileName "")
set(v_InstalledLibDirPathDebug "")
set(v_InstalledLibDirPathRelease "")
#end