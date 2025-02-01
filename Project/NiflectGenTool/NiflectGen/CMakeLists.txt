set(ModuleName NiflectGen)

set(ModuleRootPath ${c_RootSourceDirPath}/${ModuleName})
set(ModuleSourcePath ${ModuleRootPath})
set(ModuleIncludePath ${ModuleRootPath})

set(IncludePathsPrivate "")
set(IncludePathsPublic "")
list(APPEND IncludePathsPrivate ${c_RootSourceDirPath})
list(APPEND IncludePathsPrivate ${ModuleSourcePath}/Test/TestEngine/include)#仅为查看测试代码方便
list(APPEND IncludePathsPublic ${ModuleIncludePath})

file(GLOB_RECURSE ModuleSources ${ModuleSourcePath}/*.cpp ${ModuleSourcePath}/*.h)
create_source_group(${ModuleSourcePath} ${ModuleSources})
file(GLOB_RECURSE ModuleHeaders ${ModuleIncludePath}/*.h)
create_source_group(${ModuleIncludePath} ${ModuleHeaders})
set(SrcAll "")
list(APPEND SrcAll ${ModuleSources})
list(APPEND SrcAll ${ModuleHeaders})

add_library(${ModuleName} SHARED ${SrcAll})

target_precompile_headers(${ModuleName} PRIVATE ${ModuleSourcePath}/NiflectGenDevelopmentMacro.h)

target_include_directories(${ModuleName}
	PRIVATE ${IncludePathsPrivate}
	PUBLIC ${IncludePathsPublic}
)

target_compile_definitions(${ModuleName}
	PRIVATE -DNIFLECTGEN_EXPORTS
)

target_link_libraries(${ModuleName} PRIVATE Niflect)

include(Licensed_libclang.cmake)

include(${c_ProjectDirPath}/Install.cmake)