#pragma once
#include "NiflectGen/CodeWriter/CppWriter.h"

namespace NiflectGen
{
	class CGenLog;

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
			: m_toGenGeneratedDeclsThisType(false)
			, m_toGenStaticModuleReg(false)
			, m_toGenStaticallyRegisterToRegistry(false)
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
			, m_allowedFieldTypeAsForBindingTypeInAS(false)
#else
#endif
			, m_exportedStaticGetTypeFunctions(false)
			, m_genFileMode(EGeneratingHeaderAndSourceFileMode::EHeaderOnly)
		{
		}
		Niflect::CString m_moduleName;
		Niflect::CString m_genOutputDirPath;
		Niflect::CString m_genSourceOutputDirPath;
		Niflect::TArrayNif<Niflect::CString> m_vecModuleHeader2;
		Niflect::TArray<Niflect::CString> m_vecModulePrecompileHeader;
		Niflect::TArrayNif<Niflect::CString> m_vecAccessorSettingHeader;
		Niflect::TArrayNif<Niflect::CString> m_vecModuleHeaderSearchPath2;//header search paths for libclang parsing
		Niflect::TArrayNif<Niflect::CString> m_vecResolverCustomizationHeader;
		Niflect::CString m_specifiedModuleApiMacro;
		Niflect::CString m_specifiedModuleApiMacroHeaderFilePath;
		Niflect::CString m_toolHeaderSearchPath;
		bool m_toGenGeneratedDeclsThisType;
		bool m_toGenStaticModuleReg;
		bool m_toGenStaticallyRegisterToRegistry;
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
		bool m_allowedFieldTypeAsForBindingTypeInAS;
#else
#endif
		bool m_exportedStaticGetTypeFunctions;
		EGeneratingHeaderAndSourceFileMode m_genFileMode;
	};

	class CModuleRegInfoValidated
	{
	public:
		bool Init(const CModuleRegInfo& info, CGenLog* log);
		Niflect::CString GetSourceFileExtForGenFileMode() const;

		Niflect::CString m_moduleGenSourceRoot;
		Niflect::CString m_typeRegBasePath;
		Niflect::CString m_genIncludeBasePath;
		Niflect::CString m_genSrcBasePath;
		Niflect::CString m_moduleRegBasePath;
		Niflect::CString m_genTimeBasePath;
		Niflect::CString m_genSourceRootParentDir;
		Niflect::CString m_moduleGenDirPath;
		Niflect::CString m_moduleRegisteredTypeHeaderFilePath;
		Niflect::CString m_moduleScopeSymbolPrefix;
		Niflect::CString m_moduleApiMacro;
		Niflect::CString m_moduleApiMacroHeaderFilePath;
		Niflect::TArray<Niflect::CString> m_vecParsingHeaderSearchPath;
		CWritingHeaderSearchPaths m_writingHeaderSearchPaths;
		CModuleRegInfo m_userProvided;
	};

	namespace Test
	{
		void InitArgs(CModuleRegInfo& info);
	}
}