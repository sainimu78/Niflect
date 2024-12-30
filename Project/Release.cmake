#if(PROJECT_RELEASE)	
#	set(ZipFileName ${ProjectName}.zip)
#	set(ZipFilePath ${CMAKE_INSTALL_PREFIX}/${ZipFileName})
#	if(EXISTS "${ProjectInstalledDirPath}")
#		message("SSSSSSSS ${ZipFilePath}, ${ProjectInstalledDirPath}")
#		
#		set(SrcFilePath ${ZipFilePath})
#		set(DstDirPath ${ProjectInstalledDirPath})
#		if(WIN32)
#			string(REPLACE "/" "\\" SrcFilePath "${ZipFilePath}")
#			string(REPLACE "/" "\\" DstDirPath "${ProjectInstalledDirPath}")
#		endif()
#		
#		execute_process(
#			COMMAND ${CMAKE_COMMAND} -E echo "Packaging ${ProjectName} ..."
#			COMMAND ${ExeFilePath7z} a "${SrcFilePath}" "${DstDirPath}"
#		)
#		file(MAKE_DIRECTORY "${DstFilePath}")
#		
#		set(ReleasingFilePath ${PlatformReleaseDirPath}/${ZipFileName})
#		set(DstFilePath ${ReleasingFilePath})
#		if(WIN32)
#			string(REPLACE "/" "\\" DstFilePath "${ReleasingFilePath}")
#		endif()
#		file(RENAME "${SrcFilePath}" "${DstFilePath}")# Move file
#		#end
#	else()
#		message(FATAL_ERROR "${ProjectInstalledDirPath} does not exist")
#	endif()
#endif()
#if(EXISTS "${RootTempDirPath}")
#	file(REMOVE_RECURSE "${RootTempDirPath}")
#endif()

if(PROJECT_RELEASE)	
	include(${RootProjectDirPath}/Get7z.cmake)
	
	set(ZipFileName ${ProjectName}.zip)
	set(ZipFilePath ${CMAKE_INSTALL_PREFIX}/${ZipFileName})
	if(EXISTS "${ProjectInstalledDirPath}")
		execute_process(
			COMMAND ${CMAKE_COMMAND} -E echo "Packaging ${ProjectName} ..."
			COMMAND ${ExeFilePath7z} a "${ZipFilePath}" "${ProjectInstalledDirPath}"
			OUTPUT_VARIABLE zip_output
		)
		set(ReleasingFilePath ${PlatformReleaseDirPath}/${ZipFileName})
		file(COPY "${ZipFilePath}" DESTINATION "${PlatformReleaseDirPath}")
		file(REMOVE "${ZipFilePath}")
	else()
		message(FATAL_ERROR "${ProjectInstalledDirPath} does not exist")
	endif()
endif()
if(EXISTS "${RootTempDirPath}")
	#file(REMOVE_RECURSE "${RootTempDirPath}")
endif()