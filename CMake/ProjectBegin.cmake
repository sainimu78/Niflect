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

function(zip_directory DirPathToZip ZipFilePath)
	if(WIN32)
		include(${c_RootCMakeDirPath}/GetZip.cmake)
		execute_process(
			#COMMAND ${CMAKE_COMMAND} -E echo "Packaging ${c_ProjectName} ..."
			COMMAND ${ExeFilePath7z} a "${ZipFilePath}" "${DirPathToZip}"
			RESULT_VARIABLE ZIP_RESULT
		)
	else()
		get_filename_component(DirName "${DirPathToZip}" NAME)
		get_filename_component(ParentDir "${DirPathToZip}" DIRECTORY)
		execute_process(
			COMMAND zip -r -v "${ZipFilePath}" "${DirName}"
			WORKING_DIRECTORY "${ParentDir}"
			RESULT_VARIABLE ZIP_RESULT
		)
	endif()
endfunction()

function(unzip_file ZipFilePath DirPathUnzipTo)
	if(WIN32)
		include(${c_RootCMakeDirPath}/GetZip.cmake)
		string(REPLACE "/" "\\" DirPathToCreate "${DirPathUnzipTo}")
		execute_process(
			COMMAND ${ExeFilePath7z} x "${ZipFilePath}" -o${DirPathToCreate}
			RESULT_VARIABLE ZIP_RESULT
		)
	else()
		execute_process(
			COMMAND unzip -o "${ZipFilePath}" -d "${DirPathUnzipTo}"
			RESULT_VARIABLE ZIP_RESULT
		)
	endif()
endfunction()

function(download_zip_replace_dir SrcAddrZipFilePath DstDownloadedFilePath DstUnzippedDirPath)
	message(STATUS "Downloading ${DstDownloadedFilePath} from ${SrcAddrZipFilePath}")
	file(DOWNLOAD ${SrcAddrZipFilePath} ${DstDownloadedFilePath}
		STATUS status
		#SHOW_PROGRESS
	)
	if(status EQUAL 0)
		message(STATUS "File downloaded successfully.")
		file(REMOVE_RECURSE "${DstUnzippedDirPath}")
		get_filename_component(ParentDir "${DstUnzippedDirPath}" DIRECTORY)
		unzip_file(${DstDownloadedFilePath} ${ParentDir})		
		file(REMOVE "${DstDownloadedFilePath}")
	else()
		message(FATAL_ERROR "File download failed.")
	endif()
endfunction()

if (WIN32)
	#避免如 freopen 的 Warning C4996
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)# 可将工程添加到目录中, 如 set_target_properties(NiflectGenTool_${ModuleName} PROPERTIES FOLDER "AutoGen")