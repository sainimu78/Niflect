install(TARGETS ${ModuleName}
	RUNTIME DESTINATION "${ProjectInstallingTargetDirPath}/${BinDirName}"
)
if(WIN32)
	install(FILES "$<TARGET_FILE_DIR:${ModuleName}>/${ModuleName}.pdb"
		DESTINATION "${ProjectInstallingTargetDirPath}/${BinDirName}"
		CONFIGURATIONS Debug
	)
else()
	install(TARGETS ${ModuleName}
		LIBRARY DESTINATION "${ProjectInstallingTargetDirPath}/${LibDirName}"
	)
endif()