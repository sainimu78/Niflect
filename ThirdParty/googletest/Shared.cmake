set(v_FindPackageBasedIntegration FALSE)

set(ImportedLibName googletest)
if(v_FindPackageBasedIntegration)
    message(FATAL_ERROR)
    set(ImportedLibNameVersioned ${ImportedLibName}_1_16_Official)
else()
    set(ImportedLibNameVersioned ${ImportedLibName}_1_16_Custom_Partial)
endif()
set(v_ImportedLibRootDirPath ${c_RootThirdPartyDirPath}/${ImportedLibName})
set(v_UnzippedDirPath ${v_ImportedLibRootDirPath}/${ImportedLibNameVersioned})
set(v_ZipFileName ${ImportedLibNameVersioned}.zip)
set(v_SrcAddrZipFilePath ${c_StorageAddrPath}/ThirdParty/${ImportedLibName}/${c_ProjectPlatform}/${v_ZipFileName})

if(v_FindPackageBasedIntegration)
    message(FATAL_ERROR)
    set(v_LibNameFindPackgeBased GTest)
    list(APPEND v_ListComponentFindPackgeBased 
        GTest
        )
    set(GTest_ROOT "${v_UnzippedDirPath}")
else()
    #set(v_DebugLibFileNamePostfix d)
    set(v_LibPlatformArchDirPath ${v_UnzippedDirPath}/build/${c_ProjectPlatform}/${c_ProjectArch})
    list(APPEND v_ListLibIncludeDirPathPrivate ${v_UnzippedDirPath}/include)
    list(APPEND v_ListImportedLibFileName gtest)
endif()

include(${c_RootCMakeProjectFrameworkDirPath}/ImportLibDownloaded.cmake)