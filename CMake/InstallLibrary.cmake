install(TARGETS ${ModuleName}
	RUNTIME DESTINATION "${ProjectInstallingTargetDirPath}/${BinDirName}"
	LIBRARY DESTINATION "${ProjectInstallingTargetDirPath}/${LibDirName}"
	ARCHIVE DESTINATION "${ProjectInstallingTargetDirPath}/${LibDirName}"
)

#foreach(It IN LISTS ListModuleInstallingHeaderDirPath)
#	install(DIRECTORY "${It}" DESTINATION "${ProjectInstallingDirPath}")
#endforeach()

install(DIRECTORY "${ModuleHeaderDirPath}" DESTINATION "${ProjectInstallingDirPath}")

#install(FILES ${ModuleHeaders} DESTINATION include)

if(WIN32)
	install(FILES "$<TARGET_FILE_DIR:${ModuleName}>/${ModuleName}.pdb"
		DESTINATION "${ProjectInstallingTargetDirPath}/${BinDirName}"
		CONFIGURATIONS Debug
	)
endif()