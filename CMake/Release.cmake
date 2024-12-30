if(PROJECT_RELEASE)	
	include(${RootCMakeDirPath}/Get7z.cmake)
	
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
	file(REMOVE_RECURSE "${RootTempDirPath}")
endif()