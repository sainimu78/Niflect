#pragma once
#include "NiflectGen/CodeWriter/CppWriter.h"

namespace NiflectGen
{
	enum class EGeneratingHeaderAndSourceFileMode
	{
		EHeaderOnly,

		//ʹ�ô�ģʽ�뽫���ɵ��ļ����� Build ϵͳ, ��֧�������ɺ�����ִ�� Module �� cmake
		//��δ���� Build ϵͳ, ����ִ�� Module �� cmake, �����ɵ��ļ����� Module ����
		//��ģʽͨ�����ڷ���������ɵĴ���, �����öദ��������, һ�������ʹ��
		ESourceAndHeader,
	};

	class CModuleRegInfo
	{
	public:
		CModuleRegInfo()
			: m_toGenGeneratedBodyThisType(false)
			, m_toGenStaticModuleReg(false)
			, m_genFileMode(EGeneratingHeaderAndSourceFileMode::EHeaderOnly)
		{
		}
		Niflect::CString m_moduleName;
		Niflect::CString m_genOutputDirPath;
		Niflect::CString m_genSourceOutputDirPath;
		Niflect::TArrayNif<Niflect::CString> m_vecModuleHeader2;
		Niflect::TArrayNif<Niflect::CString> m_vecAccessorSettingHeader;
		Niflect::TArrayNif<Niflect::CString> m_vecModuleHeaderSearchPath2;//header search paths for libclang parsing
		Niflect::TArrayNif<Niflect::CString> m_vecResolverCustomizationHeader;
		Niflect::CString m_moduleApiMacro;
		Niflect::CString m_moduleApiMacroHeader;
		Niflect::CString m_toolHeaderSearchPath;
		bool m_toGenGeneratedBodyThisType;
		bool m_toGenStaticModuleReg;
		EGeneratingHeaderAndSourceFileMode m_genFileMode;
	};

	class CModuleRegInfoValidated
	{
	public:
		bool Init(const CModuleRegInfo& info);
		Niflect::CString GetSourceFileExtForGenFileMode() const;

		Niflect::CString m_moduleGenSourceRoot;
		Niflect::CString m_typeRegBasePath;
		Niflect::CString m_genIncludeBasePath;
		Niflect::CString m_genSrcBasePath;
		Niflect::CString m_moduleRegBasePath;
		Niflect::CString m_genTimeBasePath;
		Niflect::CString m_genSourceRootParentDir;
		Niflect::CString m_moduleRegisteredTypeHeaderFilePath;
		Niflect::CString m_moduleScopeSymbolPrefix;
		Niflect::TArrayNif<Niflect::CString> m_vecParsingHeaderSearchPath;
		CWritingHeaderSearchPaths m_writingHeaderSearchPaths;
		CModuleRegInfo m_userProvided;
	};

	namespace Test
	{
		void InitArgs(CModuleRegInfo& info);
	}
}