if(WIN32)
	install(TARGETS ${ModuleName}
		RUNTIME DESTINATION "${c_ProjectInstallingTargetDirPath}/${c_BinDirName}"
	)
	install(FILES "$<TARGET_FILE_DIR:${ModuleName}>/${ModuleName}.pdb"
		DESTINATION "${c_ProjectInstallingTargetDirPath}/${c_BinDirName}"
		CONFIGURATIONS Debug
	)
else()
	install(TARGETS ${ModuleName}
		RUNTIME DESTINATION "${c_ProjectInstallingTargetDirPath}/${c_BinDirName}"
		LIBRARY DESTINATION "${c_ProjectInstallingTargetDirPath}/${c_LibDirName}"
	)
endif()