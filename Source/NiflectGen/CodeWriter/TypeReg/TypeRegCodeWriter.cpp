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
			//todo: 专门处理STL的类型:
			//1. 不在NiflectSTLDeclaration.h集中声明, 分散到同名旁路头文件如std::string移到string文件中
			//2. NiflectSTLDeclaration.h扩展用法, 用于表示不能被Binding的STL类型
			//3. 对获取到的路径检查是否为旁路代码路径, 是则转换为STL风格的路径
			//4. 如果路径为NiflectSTLDeclaration.h, 则报错, 提示不能Binding
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
			auto infoTypeName = m_resolvedData->m_taggedMapping.GetInfoTypeName(m_bindingTypeIndexedRoot->m_taggedTypeIndex);
			if (m_bindingTypeIndexedRoot->IsTaggedType())
			{
				Niflect::CString funcName = "RegisterTypeChecked<" + m_bindingTypeIndexedRoot->m_resocursorName + ", " + infoTypeName + ">";
				CCodeTemplate tpl0;
				ReadTemplateFromRawData(tpl0, HardCodedTemplate::InvokeRegisterTypeByFrameworkTableMethod);
				CLabelToCodeMapping map;
				MapLabelToText(map, LABEL_0, m_bindingTypeIndexedRoot->m_resocursorName);
				MapLabelToText(map, LABEL_2, funcName);
				MapLabelToText(map, LABEL_13, m_bindingTypeIndexedRoot->GetCreateTypeAccessorFuncName(context.m_moduleRegInfo.m_moduleScopeSymbolPrefix));
				Niflect::CString nataNullOrVar;
				WriteNataArgNullOrVar(linesNata, linesReg, nataNullOrVar);
				MapLabelToText(map, LABEL_14, nataNullOrVar);
				auto& tt = m_resolvedData->m_taggedMapping.m_vecType[m_bindingTypeIndexedRoot->m_taggedTypeIndex];
				auto invokeCtorAddr = tt->GetInvokeCtorAddr(m_bindingTypeIndexedRoot->m_resocursorName);
				MapLabelToText(map, LABEL_15, invokeCtorAddr);
				Niflect::TSet<Niflect::CString> setReplacedLabel;
				tpl0.ReplaceLabels(map, linesReg, &setReplacedLabel);
				ASSERT(setReplacedLabel.size() == map.size());
			}
			else
			{
				Niflect::CString funcName = "RegisterType<" + m_bindingTypeIndexedRoot->m_resocursorName + ", " + infoTypeName + ">";
				funcName = context.m_moduleRegInfo.m_moduleScopeSymbolPrefix + funcName;
				CCodeTemplate tpl0;
				ReadTemplateFromRawData(tpl0, HardCodedTemplate::InvokeRegisterTypeByGeneratedStaticFunc);
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
			//有可能实现对于指针类的 BindingType 只声明, 而不 include 所在头文件, 但可能较困难, 须考虑
			//1. 指针模板, 如 std::shared_ptr, 指针头文件复用1维容器模板的获取方式见注释标记 Resolve element resonode decl header file path, 另见生成 CreateTypeAccessor 时的相关变量 CAccessorSettingResonodeInfo::m_isPointerTemplate
			//2. Raw 指针, 如 CTestBase1*, 指针头文件获取位置见注释标记 It's a non-builtin pointer type
			//以上情况如何生成声明代码
			//此外还须考虑指针被定义为别名或多层别名后的声明代码如何生成
			//关于声明代码, 在生成 GenInclude 中的全 Scope 类型声明 GenerateFullScopedTypeDeclCodeRecurs
			//生成方法是最简单的, 只需要能够生成 TaggedType 的类型声明即可, TaggedType 无别名的复杂情况

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

					if (IsCursorTemplateDecl(setting.GetAccessorTypeDecl().m_cursorDecl))//注, 特化的 Kind 为 ClassDecl
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
						ASSERT(p->IsValid());//todo: 报错
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
				//{
				//	CMacroDefinitionData md;
				//	md.m_namePostfix = "ExposeToAccessor";
				//	ReplaceLabelToLines1(HardCodedTemplate::MacroBodyExposeToAccessor, LABEL_9, resocursorNameForLastTemplateArg, md.m_linesBody);
				//	data.m_lineNumberMacroData.m_vecMacroDefinitionData.push_back(md);
				//}
				{
					CMacroDefinitionData md;
					md.m_namePostfix = "ExposeToCTypeBody";
					md.m_linesBody.push_back(HardCodedTemplate::MacroBodyExposeToCTypeBody);
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
				auto apiMacroSpace = context.m_moduleRegInfo.m_moduleApiMacro;
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