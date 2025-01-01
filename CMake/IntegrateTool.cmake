set(DstDownloadedFilePath ${v_IntegratedToolRootDirPath}/${v_ZipFileName})
if(PROJECT_SETUP OR NOT EXISTS "${v_UnzippedDirPath}")
	download_zip_replace_dir(${v_SrcAddrZipFilePath} ${DstDownloadedFilePath} ${v_UnzippedDirPath})
endif()
