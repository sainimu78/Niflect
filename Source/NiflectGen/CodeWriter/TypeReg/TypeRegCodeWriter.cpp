#include "NiflectGen/CodeWriter/TypeReg/TypeRegCodeWriter.h"
#include "NiflectGen/Util/CursorUtil.h"
//#include "NiflectGen/CodeWriter/HardCoded/TypeRegGenHeaderDeclTemplate.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/CodeWriter/HardCoded/TypeRegInitialRegTemplate.h"
#include "NiflectGen/Resolver/ResolvedData.h"

namespace NiflectGen
{
	CTypeRegCodeWriter::CTypeRegCodeWriter(const CXCursor& cursor, const STypeRegClassWritingSetting& writingSetting)
		: m_cursor(cursor)
		, m_writingSetting(writingSetting)
	{
		//m_typeName = CXStringToCString(clang_getCursorSpelling(m_cursor));
		//m_typeNameWithScope = CXStringToCString(clang_getTypeSpelling(clang_getCursorType(m_cursor)));
		//Niflect::TArrayNif<Niflect::CString> vecDeclScope;
		//FindNamespaceAndScopeNameRecursOld(m_cursor, m_vecNamespace, vecDeclScope);
	}
	void CTypeRegCodeWriter::WriteGenHeaderDeclaration(const CWritingContext& context, STypeRegGenHeaderDeclarationWritingData& data) const
	{
		//data.m_linesTypeDeclaration.push_back(ReplaceLabelToText1(HardCodedTemplate::TypeRegGenHeader_TypeDecl, LABEL_0, m_typeName));

		//{
		//	CCodeTemplate tpl;
		//	tpl.ReadFromRawData(HardCodedTemplate::TypeRegGenHeader_StaticGetTypeSpecialization);
		//	CLabelToCodeMapping map;
		//	MapLabelToText(map, LABEL_1, m_typeNameWithScope);
		//	tpl.ReplaceLabels(map, data.m_linesStaticGetTypeSpecialization);
		//}

		//{
		//	CCodeTemplate tpl;
		//	tpl.ReadFromRawData(HardCodedTemplate::TypeRegGenHeader_ExposedPrivateMacro);
		//	CLabelToCodeMapping map;
		//	MapLabelToText(map, LABEL_0, "reserved_decl_without_namespace");
		//	MapLabelToText(map, LABEL_2, "reserved_file_id");
		//	MapLabelToText(map, LABEL_3, "reserved_line_number");
		//	tpl.ReplaceLabels(map, data.m_linesExposedPrivateMacro);
		//}
	}
	void CTypeRegCodeWriter::WritePrivateHeaderIncludePath(const CWritingContext& context, CNoDupPathCollector& includePathRequirement) const
	{
		this->CollectIncludePathFromCursor(context, m_cursor, includePathRequirement);
	}
	void CTypeRegCodeWriter::CollectIncludePathFromCursor(const CWritingContext& context, const CXCursor& cursor, CNoDupPathCollector& includePathRequirement) const
	{
		auto filePath = GetCursorFilePath(cursor);

		if (!NiflectUtil::EndsWith(filePath, NiflectGenDefinition::NiflectFramework::FileName::BypassingHeader))
		{
			includePathRequirement.Add(CIncludesHelper::ConvertToIncludePath(filePath, m_writingSetting.m_vecHeaderSearchPath));
		}
		else
		{
			//todo: ר�Ŵ���STL������:
			//1. ����NiflectSTLDeclaration.h��������, ��ɢ��ͬ����·ͷ�ļ���std::string�Ƶ�string�ļ���
			//2. NiflectSTLDeclaration.h��չ�÷�, ���ڱ�ʾ���ܱ�Binding��STL����
			//3. �Ի�ȡ����·������Ƿ�Ϊ��·����·��, ����ת��ΪSTL����·��
			//4. ���·��ΪNiflectSTLDeclaration.h, �򱨴�, ��ʾ����Binding
			auto type = clang_getCursorType(cursor);
			GenLogError(context.m_log, NiflectUtil::FormatString("Does not support binding type %s", CXStringToCString(clang_getTypeSpelling(type)).c_str()));
		}
	}

	CTypeRegCodeWriter2::CTypeRegCodeWriter2()
		: m_resolvedData(NULL)
		, m_bindingTypeIndexedRoot(NULL)
	{

	}
	void CTypeRegCodeWriter2::Init(const CResolvedData* resolvedData, const CResolvedCursorNode* bindingTypeIndexedRoot)
	{
		m_bindingTypeIndexedRoot = bindingTypeIndexedRoot;
		m_resolvedData = resolvedData;
	}
	void CTypeRegCodeWriter2::WriteInvokeRegisterType(const STypeRegRegisterTypeContext& context, STypeRegInvokeRegisterTypeWritingData& data) const
	{
		CCodeLines linesReg;
		CCodeLines linesNata;
		if (m_bindingTypeIndexedRoot->m_taggedTypeIndex != INDEX_NONE)
		{
			ASSERT(data.m_taggedTypeHeaderFilePathAddr == NULL);
			data.m_taggedTypeHeaderFilePathAddr = m_bindingTypeIndexedRoot->GetHeaderFilePathAddrForTaggedType();
			{
				auto& tt = m_resolvedData->m_taggedMapping.m_vecType[m_bindingTypeIndexedRoot->m_taggedTypeIndex];
				tt->WriteTaggedTypeCopyNata(linesNata);
			}
		}
		{
			const char* hct = NULL;
			auto infoTypeName = m_resolvedData->m_taggedMapping.GetInfoTypeName(m_bindingTypeIndexedRoot->m_taggedTypeIndex);
			Niflect::CString funcName;
			if (m_bindingTypeIndexedRoot->IsTaggedType())
			{
				funcName = "RegisterType3<" + m_bindingTypeIndexedRoot->m_resocursorName + ", " + infoTypeName + ">";
				hct = HardCodedTemplate::InvokeRegisterTypeByFrameworkTableMethod;
			}
			else
			{
				funcName = "RegisterType<" + m_bindingTypeIndexedRoot->m_resocursorName + ", " + infoTypeName + ">";
				funcName = context.m_moduleRegInfo.m_moduleScopeSymbolPrefix + funcName;
				hct = HardCodedTemplate::InvokeRegisterTypeByGeneratedStaticFunc;
			}

			CCodeTemplate tpl0;
			ReadTemplateFromRawData(tpl0, hct);
			CLabelToCodeMapping map;
			MapLabelToText(map, LABEL_0, m_bindingTypeIndexedRoot->m_resocursorName);
			MapLabelToText(map, LABEL_2, funcName);
			MapLabelToText(map, LABEL_13, m_bindingTypeIndexedRoot->GetCreateTypeAccessorFuncName(context.m_moduleRegInfo.m_moduleScopeSymbolPrefix));
			Niflect::CString nataNullOrVar;
			WriteNataArgNullOrVar(linesNata, linesReg, nataNullOrVar);
			MapLabelToText(map, LABEL_14, nataNullOrVar);
			Niflect::TSet<Niflect::CString> setReplacedLabel;
			tpl0.ReplaceLabels(map, linesReg, &setReplacedLabel);
			ASSERT(setReplacedLabel.size() == map.size());
		}
		ReplaceLabelToImplScopeLines(linesReg, data.m_linesInvokeRegisterType);
	}
	void CTypeRegCodeWriter2::WriteWriteCreateTypeAccessorFunc(const STypeRegCreateTypeAccessorWritingContext& context, STypeRegCreateTypeAccessorWritingData& data) const
	{
		this->WriteCreateTypeAccessor(context, data.m_linesCreateTypeAccessorDecl, data.m_linesCreateTypeAccessorImpl, data.m_dependencyHeaderFilePathAddrs
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
			, data.m_vecGetSetData
#endif
		);

		this->CollectDependencyHeaderFilePathAddrs(data.m_dependencyHeaderFilePathAddrs);
	}
	void CTypeRegCodeWriter2::WriteCreateTypeAccessor(const STypeRegCreateTypeAccessorWritingContext& context, CCodeLines& dataDecl, CCodeLines& dataImpl, CDependencyHeaderFilePathAddrs& dependencyHeaderFilePathAddrs
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
		, Niflect::TArrayNif<SGetterSetterData>& vecGetSetData
#endif
	) const
	{
		auto createTypeAccessorFuncName = m_bindingTypeIndexedRoot->GetCreateTypeAccessorFuncName(context.m_moduleRegInfo.m_moduleScopeSymbolPrefix);
		{
			//�п���ʵ�ֶ���ָ����� BindingType ֻ����, ���� include ����ͷ�ļ�, �����ܽ�����, �뿼��
			//1. ָ��ģ��, �� std::shared_ptr, ָ��ͷ�ļ�����1ά����ģ��Ļ�ȡ��ʽ��ע�ͱ�� Resolve element resonode decl header file path, ������� CreateTypeAccessor ʱ����ر��� CAccessorSettingResonodeInfo::m_isPointerTemplate
			//2. Raw ָ��, �� CTestBase1*, ָ��ͷ�ļ���ȡλ�ü�ע�ͱ�� It's a non-builtin pointer type
			//����������������������
			//���⻹�뿼��ָ�뱻����Ϊ�����������������������������
			//������������, ������ GenInclude �е�ȫ Scope �������� GenerateFullScopedTypeDeclCodeRecurs
			//���ɷ�������򵥵�, ֻ��Ҫ�ܹ����� TaggedType ��������������, TaggedType �ޱ����ĸ������

			CCodeTemplate tpl0;
			ReadTemplateFromRawData(tpl0, HardCodedTemplate::CreateTypeAccessorDecl);
			CLabelToCodeMapping map;
			MapLabelToText(map, LABEL_2, createTypeAccessorFuncName);
			Niflect::TSet<Niflect::CString> setReplacedLabel;
			tpl0.ReplaceLabels(map, dataDecl, &setReplacedLabel);
			ASSERT(setReplacedLabel.size() == map.size());
		}
		{
			CCodeTemplate tpl0;
			ReadTemplateFromRawData(tpl0, HardCodedTemplate::CreateTypeAccessorImpl);
			CLabelToCodeMapping map;
			MapLabelToText(map, LABEL_2, createTypeAccessorFuncName);

			CCodeLines linesBody;
			Niflect::CString accessorResocursorName;
			{
				if (m_bindingTypeIndexedRoot->m_accessorBindingIndex != INDEX_NONE)
				{
					auto& setting = m_resolvedData->m_accessorBindingMapping->m_settings.m_vecAccessorBindingSetting[m_bindingTypeIndexedRoot->m_accessorBindingIndex];
					accessorResocursorName = setting.m_accessorSettingResolvedInfo.m_resoInfo.m_resocursorName;
					dependencyHeaderFilePathAddrs.m_vecImpl.push_back(&setting.m_accessorSettingResolvedInfo.m_resoInfo.m_requiredHeaderFilePath);

					if (IsCursorTemplateDecl(setting.GetAccessorTypeDecl().m_cursorDecl))//ע, �ػ��� Kind Ϊ ClassDecl
					{
						auto& arg = m_bindingTypeIndexedRoot->m_resocursorName;
						NiflectGenDefinition::CodeStyle::TemplateAngleBracketL(accessorResocursorName);
						accessorResocursorName += arg;
						NiflectGenDefinition::CodeStyle::TemplateAngleBracketR(accessorResocursorName);
					}
				}
				else
				{
					ASSERT(m_bindingTypeIndexedRoot->m_taggedTypeIndex != INDEX_NONE);
					auto& tt = m_resolvedData->m_taggedMapping.m_vecType[m_bindingTypeIndexedRoot->m_taggedTypeIndex];
					auto& cursor = tt->GetCursor();
					auto kind = clang_getCursorKind(cursor);
					CBindingSettingData* p = NULL;
					if (kind == CXCursor_ClassDecl || kind == CXCursor_StructDecl)
					{
						p = &m_resolvedData->m_accessorBindingMapping->m_settings.m_settingCompound;
					}
					else if (kind == CXCursor_EnumDecl)
					{
						if (clang_EnumDecl_isScoped(cursor))
							p = &m_resolvedData->m_accessorBindingMapping->m_settings.m_settingEnumClass;
						else
							p = &m_resolvedData->m_accessorBindingMapping->m_settings.m_settingEnumBitMask;
					}
					else
					{
						ASSERT(false);
					}
					if (p != NULL)
					{
						ASSERT(p->IsValid());//todo: ����
						accessorResocursorName = p->m_accessorSettingResolvedInfo.m_resoInfo.m_resocursorName;
						dependencyHeaderFilePathAddrs.m_vecImpl.push_back(&p->m_accessorSettingResolvedInfo.m_resoInfo.m_requiredHeaderFilePath);
					}
					else
					{
						ASSERT(false);
					}

					tt->WriteUsingNamespaceDirectiveForNata(linesBody);
				}
				NiflectGenDefinition::CodeStyle::TryFormatNestedTemplate(accessorResocursorName);
			}

			{
				CCodeTemplate tpl0;
				ReadTemplateFromRawData(tpl0, HardCodedTemplate::CreateAndInitTypeAccessor);
				CLabelToCodeMapping map;
				MapLabelToText(map, LABEL_4, accessorResocursorName);
				MapLabelToText(map, LABEL_2, m_bindingTypeIndexedRoot->GetStaticGetTypeFuncName(context.m_moduleRegInfo.m_moduleScopeSymbolPrefix));
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
				SGetterSetterWritingData dddddData{ vecGetSetData };
#else
				SGetterSetterWritingData dddddData;
#endif
				SResocursorNodeBodyCodeWritingContext bodyCodeCtx{ context.m_moduleRegInfo, context.m_log };
				this->WriteResocursorNodeBodyCode(bodyCodeCtx, dddddData);
				MapLabelToLines(map, LABEL_5, dddddData.m_linesResoBodyCode);
				Niflect::TSet<Niflect::CString> setReplacedLabel;
				tpl0.ReplaceLabels(map, linesBody, &setReplacedLabel);
				ASSERT(setReplacedLabel.size() == map.size());
			}

			MapLabelToLines(map, LABEL_3, linesBody);

			Niflect::TSet<Niflect::CString> setReplacedLabel;
			tpl0.ReplaceLabels(map, dataImpl, &setReplacedLabel);
			ASSERT(setReplacedLabel.size() == map.size());
		}

#ifdef DEBUG_FOR_TYPE_REG
		DebugPrintCodeLines(dataImpl);
#endif
	}
	bool CTypeRegCodeWriter2::CompareLess(const CTypeRegCodeWriter2& a, const CTypeRegCodeWriter2& b)
	{
		ASSERT(!a.m_bindingTypeIndexedRoot->m_resocursorName.empty());
		ASSERT(!b.m_bindingTypeIndexedRoot->m_resocursorName.empty());
		return NiflectUtil::CompareLessHumanReadable(a.m_bindingTypeIndexedRoot->m_resocursorName, b.m_bindingTypeIndexedRoot->m_resocursorName);
	}
	void CTypeRegCodeWriter2::WriteGeneratedBody(const STypeRegClassGenHWritingContext& context, CTypeRegTaggedTypeGeneratedHeaderData& data) const
	{
		auto& ttIdx = m_bindingTypeIndexedRoot->m_taggedTypeIndex;
		if (ttIdx != INDEX_NONE)
		{
			Niflect::CString staticGetTypeFuncName;
			auto resocursorNameForLastTemplateArg = m_bindingTypeIndexedRoot->GetResocursorNameForLastTemplateArg();
			{
				auto& hct = HardCodedTemplate::StaticGetTypeFuncName;
				staticGetTypeFuncName = ReplaceLabelToText1(hct, LABEL_9, resocursorNameForLastTemplateArg);
			}
			{
				auto& tt = m_resolvedData->m_taggedMapping.m_vecType[ttIdx];
				auto& cursor = tt->GetCursor();
				auto kind = clang_getCursorKind(cursor);
				Niflect::CString cppTypeKeyword;
				Niflect::CString enumUnderlyingType;
				switch (kind)
				{
				case CXCursor_ClassDecl:
					cppTypeKeyword = "class";
					break;
				case CXCursor_StructDecl:
					cppTypeKeyword = "struct";
					break;
				case CXCursor_EnumDecl:
					if (clang_EnumDecl_isScoped(cursor))
						cppTypeKeyword = "enum class";
					else
						cppTypeKeyword = "enum";
					{
						CXType cxUnderlyingType = clang_getEnumDeclIntegerType(cursor);
						if (cxUnderlyingType.kind != CXType_Int)
							enumUnderlyingType = NiflectUtil::FormatString(" : %s", CXStringToCString(clang_getTypeSpelling(cxUnderlyingType)).c_str());
					}
					break;
				default:
					ASSERT(false);
					break;
				}
				auto declCode = NiflectUtil::FormatString("%s %s%s;", cppTypeKeyword.c_str(), CXStringToCString(clang_getCursorSpelling(cursor)).c_str(), enumUnderlyingType.c_str());
				data.m_specTemplateRequiredTypeDecl = declCode;
				data.m_vecScopeNameAddr = &tt->m_vecScopeName;
			}
			{
				{
					SCollectingGeneratedBodyWritingData forGenHData{ data.m_lineNumberMacroData.m_generatedBodyLineNumber };
					this->CollectDataForGenH(forGenHData);
				}
				if (context.m_moduleRegInfo.m_userProvided.m_toGenGeneratedBodyThisType)
				{
					CMacroDefinitionData md;
					md.m_namePostfix = "ThisType";
					ReplaceLabelToLines1(HardCodedTemplate::MacroBodyThisType, LABEL_9, resocursorNameForLastTemplateArg, md.m_linesBody);
					data.m_lineNumberMacroData.m_vecMacroDefinitionData.push_back(md);
				}
				{
					CMacroDefinitionData md;
					md.m_namePostfix = "ExposeToAccessor";
					ReplaceLabelToLines1(HardCodedTemplate::MacroBodyExposeToAccessor, LABEL_9, resocursorNameForLastTemplateArg, md.m_linesBody);
					data.m_lineNumberMacroData.m_vecMacroDefinitionData.push_back(md);
				}
				{
					CMacroDefinitionData md;
					md.m_namePostfix = "AnotherExample0";
					ReplaceLabelToLines1(HardCodedTemplate::MacroBodyAnotherExample0, LABEL_9, resocursorNameForLastTemplateArg, md.m_linesBody);
					data.m_lineNumberMacroData.m_vecMacroDefinitionData.push_back(md);
				}
			}
			{
				CCodeTemplate tpl0;
				ReadTemplateFromRawData(tpl0, HardCodedTemplate::StaticGetTypeSpecDecl);
				CLabelToCodeMapping map;
				MapLabelToText(map, LABEL_2, staticGetTypeFuncName);
				auto apiMacroSpace = context.m_moduleRegInfo.m_userProvided.m_moduleApiMacro;
				if (!apiMacroSpace.empty())
				{
					apiMacroSpace += ' ';
					MapLabelToText(map, LABEL_10, apiMacroSpace);
				}
				Niflect::TSet<Niflect::CString> setReplacedLabel;
				tpl0.ReplaceLabels(map, data.m_linesStaticGetTypeSpecDecl, &setReplacedLabel);
				ASSERT(setReplacedLabel.size() == map.size());
			}
			{
				CCodeTemplate tpl0;
				ReadTemplateFromRawData(tpl0, HardCodedTemplate::StaticGetTypeSpecImpl);
				CLabelToCodeMapping map;
				MapLabelToText(map, LABEL_2, staticGetTypeFuncName);
				MapLabelToText(map, LABEL_9, resocursorNameForLastTemplateArg);
				Niflect::TSet<Niflect::CString> setReplacedLabel;
				tpl0.ReplaceLabels(map, data.m_linesStaticGetTypeSpecImpl, &setReplacedLabel);
				ASSERT(setReplacedLabel.size() == map.size());
			}
		}
	}
}