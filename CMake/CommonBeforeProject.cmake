FUNCTION(create_source_group relativeSourcePath)
  FOREACH(currentSourceFile ${ARGN})
    FILE(RELATIVE_PATH folder ${relativeSourcePath} ${currentSourceFile})
    GET_FILENAME_COMPONENT(filename ${folder} NAME)
    STRING(REPLACE ${filename} "" folder ${folder})
    IF(NOT folder STREQUAL "")
      STRING(REGEX REPLACE "/+$" "" folderlast ${folder})
      STRING(REPLACE "/" "\\" folderlast ${folderlast})
      SOURCE_GROUP("${folderlast}" FILES ${currentSourceFile})
    ENDIF(NOT folder STREQUAL "")
  ENDFOREACH(currentSourceFile ${ARGN})
ENDFUNCTION()

function(download_zip_replace_dir ExeFilePath7z SrcAddrZipFilePath DstDownloadedFilePath DstUnzippedDirPath)
	message(STATUS "Downloading ${DstDownloadedFilePath} from ${SrcAddrZipFilePath}")
	file(DOWNLOAD ${SrcAddrZipFilePath} ${DstDownloadedFilePath}
		STATUS status
		#SHOW_PROGRESS
	)
	if(status EQUAL 0)
		message(STATUS "File downloaded successfully.")
		
		file(REMOVE_RECURSE "${DstUnzippedDirPath}")
		
		set(DstDirPathUnzipTo ${DstUnzippedDirPath}/..)
		set(DirPathToCreate "${DstDirPathUnzipTo}")
		if(WIN32)
			string(REPLACE "/" "\\" DirPathToCreate "${DstDirPathUnzipTo}")
		endif()
		
		execute_process(
			COMMAND ${CMAKE_COMMAND} -E echo "Unzipping ${DstDownloadedFilePath} ..."
			COMMAND ${ExeFilePath7z} x "${DstDownloadedFilePath}" -o${DirPathToCreate}
		)
		
		file(REMOVE "${DstDownloadedFilePath}")
	else()
		message(FATAL_ERROR "File download failed.")
	endif()
endfunction()

function(download_files_reserved ListSrcToDownload ListDstDownloaded)
	list(LENGTH ListSrcToDownload ListCount0)
	list(LENGTH ListDstDownloaded ListCount1)
	#math(EXPR MinCount "ListCount0 < ListCount1 ? ListCount0 : ListCount1")
	if(NOT ListCount0 EQUAL ListCount1)
		message(FATAL_ERROR "The counts for both ListSrcToDownload and ListDstDownloaded must be identical")
	endif()
	foreach(Idx RANGE 0 ${MinCount})
		list(GET ListSrcToDownload ${Idx} SrcItem)
		list(GET ListDstDownloaded ${Idx} Dstitem)
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
endfunction()

if (WIN32)
	#避免如 freopen 的 Warning C4996
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)# 可将工程添加到目录中, 如 set_target_properties(NiflectGenTool_${ModuleName} PROPERTIES FOLDER "AutoGen")