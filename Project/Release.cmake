if(DEPLOY)
	if(WIN32)
		set(ExeName7z 7za.exe)
		list(APPEND ListSrcToDownload ${StorageDirPath}/7z/${ExeName7z})
		set(ExeFilePath7z ${RootDirPath}/${ExeName7z})
		list(APPEND ListDstDownloadTo ${ExeFilePath7z})
	else
		set(ExeFilePath7z 7z)
	endif()
	
	list(LENGTH ListSrcToDownload ListCount0)
	list(LENGTH ListDstDownloadTo ListCount1)
	math(EXPR MinCount "ListCount0 < ListCount1 ? ListCount0 : ListCount1")
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
endif()