if(WIN32)
	install(TARGETS ${ModuleName}
		RUNTIME DESTINATION "${ProjectInstallingTargetDirPath}/${BinDirName}"
	)
	install(FILES "$<TARGET_FILE_DIR:${ModuleName}>/${ModuleName}.pdb"
		DESTINATION "${ProjectInstallingTargetDirPath}/${BinDirName}"
		CONFIGURATIONS Debug
	)
else()
	install(TARGETS ${ModuleName}
		RUNTIME DESTINATION "${ProjectInstallingTargetDirPath}/${BinDirName}"
		LIBRARY DESTINATION "${ProjectInstallingTargetDirPath}/${LibDirName}"
	)
endif()