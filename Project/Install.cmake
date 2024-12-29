set(ModuleInstallingDirPath ${ModuleName})
set(ModuleInstallingTargetDirPath ${ModuleInstallingDirPath}/${InstalledPlatformArchDirPath})
set(ModuleInstalledDirPath ${CMAKE_INSTALL_PREFIX}/${ModuleName})

if(NOT SETUP AND NOT DEPLOY AND EXISTS "${ModuleInstalledDirPath}")
    message(STATUS "Clearing install directory: ${ModuleInstalledDirPath}")
    file(REMOVE_RECURSE "${ModuleInstalledDirPath}")
endif()
install(TARGETS ${ModuleName}
	RUNTIME DESTINATION ${ModuleInstallingTargetDirPath}/bin
	LIBRARY DESTINATION ${ModuleInstallingTargetDirPath}/lib
	ARCHIVE DESTINATION ${ModuleInstallingTargetDirPath}/lib
)
install(DIRECTORY "${ModuleHeaderDirPath}" DESTINATION ${ModuleInstallingDirPath})
#install(FILES ${ModuleHeaders} DESTINATION include)
if(WIN32)
	install(FILES "$<TARGET_FILE_DIR:${ModuleName}>/${ModuleName}.pdb"
		DESTINATION ${ModuleInstallingTargetDirPath}/bin
		CONFIGURATIONS Debug
	)
endif()