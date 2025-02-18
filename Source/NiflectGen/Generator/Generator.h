#pragma once
#include "NiflectGen/NiflectGenCommon.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCode.h"
#include "NiflectGen/Log/Log.h"

//仅为避免引用clang头文件将cursor类型定义为void*
typedef std::function<void(void* cursorAddr)> TestInterfaceFunc;

namespace NiflectGen
{
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
		NIFLECTGEN_API bool Init(const CModuleRegInfo& userProvied, CGenLog* log);
		NIFLECTGEN_API void AddTypeBindingSettingHeader(const Niflect::CString& filePath);
		NIFLECTGEN_API void AddFileForSearching(const Niflect::CString& filePath);
		NIFLECTGEN_API void AddIncludePath(const Niflect::CString& dirPath);
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
		CModuleRegInfoValidated m_moduleRegInfo;
		CGenLog* m_log;
	};

	NIFLECTGEN_API Niflect::TSharedPtr<CGenerator> CreateGenerator();
}