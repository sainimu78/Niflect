set(ImportedLibName CommandLineArgParser)
set(v_ImportedLibRootDirPath ${c_RootThirdPartyDirPath}/${ImportedLibName})
set(v_UnzippedDirPath ${v_ImportedLibRootDirPath}/${ImportedLibName})
set(v_ZipFileName ${ImportedLibName}.zip)
set(v_SrcAddrZipFilePath ${c_StorageAddrPath}/Release/${c_ProjectPlatform}/${v_ZipFileName})
list(APPEND v_ListLibIncludeDirPathPrivate ${v_UnzippedDirPath}/include)

include(${c_RootCMakeProjectFrameworkDirPath}/ImportLibDownloaded.cmake)