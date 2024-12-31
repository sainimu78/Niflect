if(WIN32)
	set(FileName 7z.exe)
	set(FilePathDownloaded ${ProjectTempDirPath}/${FileName})
	set(ExeFilePath7z ${FilePathDownloaded})
	if(NOT EXISTS "${FilePathDownloaded}")
		set(SrcFilePath ${StorageAddrPath}/Tool/${ProjectPlatform}/7z/${FileName})
		set(DstFilePath ${FilePathDownloaded})
		message(STATUS "Downloading ${DstFilePath} from ${SrcFilePath}")
		file(DOWNLOAD "${SrcFilePath}" "${DstFilePath}"
			 STATUS status
			 #SHOW_PROGRESS
		)
		if(status EQUAL 0)
			message(STATUS "File downloaded successfully.")
		else()
			message(FATAL_ERROR "File download failed.")
		endif()
		set(FileName 7z.dll)
		set(FilePathDownloaded ${ProjectTempDirPath}/${FileName})
		set(SrcFilePath ${StorageAddrPath}/Tool/${ProjectPlatform}/7z/${FileName})
		set(DstFilePath ${FilePathDownloaded})
		message(STATUS "Downloading ${DstFilePath} from ${SrcFilePath}")
		file(DOWNLOAD "${SrcFilePath}" "${DstFilePath}"
			 STATUS status
			 #SHOW_PROGRESS
		)
		if(status EQUAL 0)
			message(STATUS "File downloaded successfully.")
		else()
			message(FATAL_ERROR "File download failed.")
		endif()
	endif()
endif()