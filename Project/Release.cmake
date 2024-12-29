if(DEPLOY)
	set(ModuleTempDirPath ${RootTempDirPath}/${ModuleName})
	if(WIN32)
		set(ExeName7z 7za.exe)
		set(ExeFilePath7z ${ModuleTempDirPath}/${ExeName7z})
		list(APPEND ListSrcToDownload ${StorageAddrPath}/Tool/${ProjectPlatform}/7z/${ExeName7z})
		list(APPEND ListDstDownloadTo ${ExeFilePath7z})
	else()
		set(ExeFilePath7z 7z)
	endif()
	
	list(LENGTH ListSrcToDownload ListCount0)
	list(LENGTH ListDstDownloadTo ListCount1)
	#math(EXPR MinCount "ListCount0 < ListCount1 ? ListCount0 : ListCount1")
	foreach(Idx RANGE 0 ${MinCount})
		list(GET ListSrcToDownload ${Idx} SrcItem)
		list(GET ListDstDownloadTo ${Idx} Dstitem)
		message(STATUS "Downloading ${Dstitem} from ${SrcItem}")
		file(DOWNLOAD ${SrcItem} ${Dstitem}
			 STATUS status
			 #SHOW_PROGRESS
		)
		if(status EQUAL 0)
			message(STATUS "File downloaded successfully.")
		else()
			message(FATAL_ERROR "File download failed.")
		endif()
	endforeach()
	
	set(ZipFileName ${ModuleName}.zip)
	set(ZipFilePath ${ModuleTempDirPath}/${ZipFileName})
	if(EXISTS "${ModuleInstalledDirPath}")
		execute_process(
			COMMAND ${CMAKE_COMMAND} -E echo "Packaging ${ModuleName} ..."
			COMMAND ${ExeFilePath7z} a "${ZipFilePath}" "${ModuleInstalledDirPath}"
		)
		file(MAKE_DIRECTORY "${PlatformDeployDirPath}")
		file(RENAME "${ZipFilePath}" "${PlatformDeployDirPath}/${ZipFileName}")
	else()
		message(FATAL_ERROR "${ModuleInstalledDirPath} does not exist")
	endif()
endif()