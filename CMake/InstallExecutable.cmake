install(TARGETS ${ModuleName}
	RUNTIME DESTINATION "${ProjectInstallingTargetDirPath}/bin"
)
if(WIN32)
	install(FILES "$<TARGET_FILE_DIR:${ModuleName}>/${ModuleName}.pdb"
		DESTINATION "${ProjectInstallingTargetDirPath}/bin"
		CONFIGURATIONS Debug
	)
endif()