#pragma once
#include "NiflectGen/NiflectGenCommon.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCode.h"

//��Ϊ��������clangͷ�ļ���cursor���Ͷ���Ϊvoid*
typedef std::function<void(void* cursorAddr)> TestInterfaceFunc;

namespace NiflectGen
{
	class CDataCollector;
	class CCodeGenData;

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
		void SaveCodeToFile(const CCodeLines& linesCode, const Niflect::CString& relativeFilePath) const;
		void SaveFileToGenSource(const CCodeLines& linesCode, const Niflect::CString& relativeFilePath) const;

	private:
		Niflect::TArrayNif<Niflect::CString> m_vecTypeBindingSettingFilePath;
		Niflect::TArrayNif<Niflect::CString> m_vecFileForSearchingCpp;
		Niflect::TArrayNif<Niflect::CString> m_vecFileForSearchingH;
		Niflect::TArrayNif<Niflect::CString> m_vecHeaderSearchPath;
		Niflect::TSharedPtr<CDataCollector> m_collectorHolder;//��Ϊ������ͷ�ļ�, ����ʹ�ø���Ĺ�����Ҫ���clangͷ�ļ�·��
		CDataCollector& m_collector;
		CModuleRegInfoValidated m_moduleRegInfo;
	};

	NIFLECTGEN_API Niflect::TSharedPtr<CGenerator> CreateGenerator();

	//S3ENGINE_API2 Niflect::TSharedPtr<CTypeBindingTable> CreateTypeBindingTable();
	NIFLECTGEN_API void TestMustache();
}