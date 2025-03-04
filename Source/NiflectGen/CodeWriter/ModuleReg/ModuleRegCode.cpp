#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCode.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "Niflect/Util/SystemUtil.h"
#include "NiflectGen/Generator/BypassSTLHeaders.h"
#include "NiflectGen/CodeWriter/GenTimeNiflectMacro.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/CodeWriter/ModuleApiMacroHeaderWriter.h"

namespace NiflectGen
{
	bool CModuleRegInfoValidated::Init(const CModuleRegInfo& info)
	{
		if (info.m_moduleName.empty())
			return false;
		if (!info.m_specifiedModuleApiMacroHeaderFilePath.empty())
		{
			if (info.m_specifiedModuleApiMacro.empty())
			{
				ASSERT(false);//todo: ��ʾָ��ǰ�߱���ͬʱָ������
				return false;
			}
		}

		m_userProvided = info;

		//m_genSourceRootParentDir ��Ϊ��, Ԥ������һ�����Ŀ¼

		m_moduleGenDirPath = NiflectUtil::ConcatPath(m_userProvided.m_genOutputDirPath, m_genSourceRootParentDir);
		if (!m_userProvided.m_genSourceOutputDirPath.empty())
		{
			ASSERT(false);//�����, ��ѡ����ܷ���
			auto genSourceOutputDirPath = NiflectUtil::ConcatPath(m_userProvided.m_genSourceOutputDirPath, m_genSourceRootParentDir);
			NiflectUtil::DeleteDirectory(genSourceOutputDirPath);
		}

		m_moduleGenSourceRoot = NiflectGenDefinition::DirName::GenSourceRoot;
		m_moduleRegBasePath = NiflectGenDefinition::DirName::ModuleReg;
		m_typeRegBasePath = NiflectGenDefinition::DirName::TypeReg;
		m_genIncludeBasePath = NiflectGenDefinition::DirName::GenInclude;
		m_genSrcBasePath = NiflectGenDefinition::DirName::GenSrc;

		m_moduleRegisteredTypeHeaderFilePath = NiflectUtil::ConcatPath(m_typeRegBasePath, NiflectGenDefinition::NiflectFramework::FileName::ModuleRegisteredTypeHeader);
		m_moduleScopeSymbolPrefix = "_" + m_userProvided.m_moduleName + "_";

		for (auto& it : m_userProvided.m_vecModuleHeaderSearchPath2)
			m_writingHeaderSearchPaths.m_vecForRegularConversion.push_back(it);
		//����������, NiflectMacro.h Ӧʹ�� _GenTime �����ɵ�, �� NiflectMacro.h ����ʹ�� m_toolHeaderSearchPath �е�����ͷ�ļ�, �� ConcatSymbols.h
		m_writingHeaderSearchPaths.m_vecForRegularConversion.push_back(m_userProvided.m_toolHeaderSearchPath);
		
		m_genTimeBasePath = NiflectUtil::ConcatPath(m_moduleGenDirPath, NiflectGenDefinition::DirName::GenTime);
		WriteBypassSTLHeaders(m_genTimeBasePath);
		{
			CGenLog log;
			Niflect::TArrayNif<Niflect::CString> vecToolHeaderSearchPath;
			vecToolHeaderSearchPath.push_back(m_userProvided.m_toolHeaderSearchPath);
			SGenTimeNiflectMacroHeaderWritingContext ctx{ vecToolHeaderSearchPath, m_genTimeBasePath, &log };
			WriteGenTimeNiflectMacroHeader(ctx);

			if (m_userProvided.m_exportedStaticGetTypeFunctions)
			{
				ASSERT(m_userProvided.m_specifiedModuleApiMacro.empty());//todo: ��ʾֻ��ѡ��һ��
				GetModuleApiMacroHeaderInfo(NiflectUtil::ConcatPath(m_userProvided.m_genOutputDirPath, m_moduleGenSourceRoot), m_userProvided.m_moduleName, m_moduleApiMacro, m_moduleApiMacroHeaderFilePath);
			}
			else
			{
				m_moduleApiMacro = m_userProvided.m_specifiedModuleApiMacro;
				m_moduleApiMacroHeaderFilePath = m_userProvided.m_specifiedModuleApiMacroHeaderFilePath;//��δ�ṩ������� PrecompileHeader ������ͷ�ļ����ṩ, �������Ϊ��
			}
		}

		m_writingHeaderSearchPaths.m_vecForGenTimeConversion.push_back(NiflectUtil::ConvertToSearchPath(m_genTimeBasePath));

		for (auto& it : m_writingHeaderSearchPaths.m_vecForGenTimeConversion)
			m_vecParsingHeaderSearchPath.push_back(it);
		for (auto& it : m_writingHeaderSearchPaths.m_vecForRegularConversion)
			m_vecParsingHeaderSearchPath.push_back(it);

		//if (m_userProvided.m_specifiedModuleApiMacro.empty())
		//	m_moduleApiMacro = NiflectUtil::FormatString("%s_API", NiflectUtil::ConvertToUpperCase(info.m_moduleName).c_str());

		return true;
	}
	Niflect::CString CModuleRegInfoValidated::GetSourceFileExtForGenFileMode() const
	{
		if (m_userProvided.m_genFileMode == EGeneratingHeaderAndSourceFileMode::EHeaderOnly)
			return NiflectGenDefinition::FileExt::PrivateH;
		return NiflectGenDefinition::FileExt::Cpp;
	}
	namespace Test
	{
		void InitArgs(CModuleRegInfo& info)
		{
			info.m_moduleName = "Engine";
			info.m_genOutputDirPath = CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PROJECT_PATH, "/Generated/NiflectGenerated");
			info.m_toolHeaderSearchPath = CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_INCLUDE_PATH, "/Niflect/include/");
			info.m_vecModuleHeaderSearchPath2.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_INCLUDE_PATH, "/NiflectGen/Test/TestEngine/include/"));
			info.m_vecModuleHeaderSearchPath2.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_INCLUDE_PATH, "/"));
		}
	}
}