#if(PROJECT_SETUP OR PROJECT_RELEASE)
#	
#endif()
#if(EXISTS "${c_RootTempDirPath}")
	file(REMOVE_RECURSE "${c_RootTempDirPath}")
#endif()
unset(downloaded_files CACHE)