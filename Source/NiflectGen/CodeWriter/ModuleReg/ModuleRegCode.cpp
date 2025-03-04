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
				ASSERT(false);//todo: 提示指定前者必须同时指定后者
				return false;
			}
		}

		m_userProvided = info;

		//m_genSourceRootParentDir 现为空, 预留附加一层输出目录

		m_moduleGenDirPath = NiflectUtil::ConcatPath(m_userProvided.m_genOutputDirPath, m_genSourceRootParentDir);
		if (!m_userProvided.m_genSourceOutputDirPath.empty())
		{
			ASSERT(false);//经检查, 此选项可能废弃
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
		//须添加在最后, NiflectMacro.h 应使用 _GenTime 中生成的, 而 NiflectMacro.h 仍须使用 m_toolHeaderSearchPath 中的其它头文件, 如 ConcatSymbols.h
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
				ASSERT(m_userProvided.m_specifiedModuleApiMacro.empty());//todo: 提示只能选择一种
				GetModuleApiMacroHeaderInfo(NiflectUtil::ConcatPath(m_userProvided.m_genOutputDirPath, m_moduleGenSourceRoot), m_userProvided.m_moduleName, m_moduleApiMacro, m_moduleApiMacroHeaderFilePath);
			}
			else
			{
				m_moduleApiMacro = m_userProvided.m_specifiedModuleApiMacro;
				m_moduleApiMacroHeaderFilePath = m_userProvided.m_specifiedModuleApiMacroHeaderFilePath;//如未提供则可能由 PrecompileHeader 或其它头文件中提供, 因此允许为空
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