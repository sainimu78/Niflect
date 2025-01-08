#pragma once
#include "NiflectGen/NiflectGenCommon.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCode.h"

//仅为避免引用clang头文件将cursor类型定义为void*
typedef std::function<void(void* cursorAddr)> TestInterfaceFunc;

namespace NiflectGen
{
	class CDataCollector;
	class CCodeGenData;

	class CFilePathAndContent
	{
	public:
		Niflect::CString m_filePath;
		CCodeWriter m_writer;
	};

	class CSavingData
	{
	public:
		std::vector<CFilePathAndContent> m_vecFileInfo;
	};

	class CGenerator
	{
	public:
		CGenerator();

	public:
		NIFLECTGEN_API void AddTypeBindingSettingHeader(const Niflect::CString& filePath);
		NIFLECTGEN_API void AddFileForSearching(const Niflect::CString& filePath);
		NIFLECTGEN_API void AddIncludePath(const Niflect::CString& dirPath);
		NIFLECTGEN_API bool InitModuleRegInfo(const CModuleRegInfo& userProvied);
		NIFLECTGEN_API void Generate(CCodeGenData& genData, TestInterfaceFunc TestFunc = NULL);
		//NIFLECTGEN_API void Save() const;
		NIFLECTGEN_API void Save2(const CCodeGenData& genData) const;
		NIFLECTGEN_API void Cleanup() const;

	public:
		const CModuleRegInfoValidated& GetModuleRegInfo() const { return m_moduleRegInfo; }

	private:
		void SaveCodeToFile(const CCodeLines& linesCode, const Niflect::CString& relativeFilePath, CSavingData& saving) const;
		void SaveFileToGenSource(const CCodeLines& linesCode, const Niflect::CString& relativeFilePath, CSavingData& saving) const;

	private:
		Niflect::TArrayNif<Niflect::CString> m_vecTypeBindingSettingFilePath;
		Niflect::TArrayNif<Niflect::CString> m_vecFileForSearchingCpp;
		Niflect::TArrayNif<Niflect::CString> m_vecFileForSearchingH;
		Niflect::TArrayNif<Niflect::CString> m_vecHeaderSearchPath;
		Niflect::TSharedPtr<CDataCollector> m_collectorHolder;//仅为不包含头文件, 避免使用该类的工程需要添加clang头文件路径
		CDataCollector& m_collector;
		CModuleRegInfoValidated m_moduleRegInfo;
	};

	NIFLECTGEN_API Niflect::TSharedPtr<CGenerator> CreateGenerator();

	//S3ENGINE_API2 Niflect::TSharedPtr<CTypeBindingTable> CreateTypeBindingTable();
	NIFLECTGEN_API void TestMustache();
}