#include "NiflectGen/CodeWriter/TypeReg/TypeAccessorCodeWriter.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/HardCoded/TypeRegInitialRegTemplate.h"
#include "NiflectGen/Collector/TaggedNode.h"//WriteNataArgNullOrVar

namespace NiflectGen
{
	static void WriteNextInitNextAccessor2(const Niflect::CString& funcName, CCodeLines& linesCode)
	{
		CCodeTemplate tpl0;
		ReadTemplateFromRawData(tpl0, HardCodedTemplate::CreateAndInitNextsAccessor);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_2, funcName);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl0.ReplaceLabels(map, linesCode, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}
	void WriteNextInitChildAccessor2(const Niflect::CString& fieldOwnerResocursorName, const Niflect::CString& funcName, const Niflect::CString& fieldName, const CCodeLines& linesNata, CCodeLines& linesResoBodyCode)
	{
		CCodeLines linesCode;
		WriteNextInitNextAccessor2(funcName, linesCode);
		Niflect::CString nataNullOrVar;
		WriteNataArgNullOrVar(linesNata, linesCode, nataNullOrVar);
		{
			CCodeTemplate tpl0;
			ReadTemplateFromRawData(tpl0, HardCodedTemplate::InitChildAccessor);
			CLabelToCodeMapping map;
			MapLabelToText(map, LABEL_0, fieldName);
			auto invokeGetFieldOffset = ReplaceLabelToText2(HardCodedTemplate::GetFieldOffset, LABEL_0, LABEL_8, fieldOwnerResocursorName, fieldName);
			MapLabelToText(map, LABEL_7, invokeGetFieldOffset);
			MapLabelToText(map, LABEL_14, nataNullOrVar);
			Niflect::TSet<Niflect::CString> setReplacedLabel;
			tpl0.ReplaceLabels(map, linesCode, &setReplacedLabel);
			ASSERT(setReplacedLabel.size() == map.size());
		}
		ReplaceLabelToImplScopeLines(linesCode, linesResoBodyCode);
	}
	void WriteNextInitElementAccessor2(const Niflect::CString& funcName, CCodeLines& linesResoBodyCode)
	{
		CCodeLines linesCode;
		WriteNextInitNextAccessor2(funcName, linesCode);
		{
			CCodeTemplate tpl0;
			ReadTemplateFromRawData(tpl0, HardCodedTemplate::InitElementAccessor);
			CLabelToCodeMapping map;
			Niflect::TSet<Niflect::CString> setReplacedLabel;
			tpl0.ReplaceLabels(map, linesCode, &setReplacedLabel);
			ASSERT(setReplacedLabel.size() == map.size());
		}
		ReplaceLabelToImplScopeLines(linesCode, linesResoBodyCode);
	}
}

//#include "NiflectGen/Util/CursorUtil.h"
////#include "NiflectGen/CodeWriter/HardCoded/TypeRegGenHeaderDeclTemplate.h"
//#include "NiflectGen/CodeWriter/CodeTemplate.h"
//#include "NiflectGen/CodeWriter/CppWriter.h"
//#include "NiflectGen/Base/NiflectGenDefinition.h"
//#include "NiflectGen/CodeWriter/HardCoded/TypeRegInitialRegTemplate.h"
//#include "NiflectGen/Resolver/ResolvedData.h"
//
//namespace NiflectGen
//{
//	CFieldLayoutBasedCodeWriter::CFieldLayoutBasedCodeWriter()
//	{
//
//	}
//	void CFieldLayoutBasedCodeWriter::WriteForFieldLayout(const STypeRegCreateFieldLayoutOfTypeWritingContext& context, STypeRegCreateFieldLayoutOfTypeWritingData& data) const
//	{
//		this->WriteCreateFieldLayoutOfType(context, data.m_linesCreateFieldLayoutOfTypeDecl, data.m_linesCreateFieldLayoutOfTypeImpl);
//
//		this->CollectDependencyHeaderFilePaths(data.m_dependencyHeaderFilePathRefs);
//	}
//	void CFieldLayoutBasedCodeWriter::WriteCreateFieldLayoutOfType(const STypeRegCreateFieldLayoutOfTypeWritingContext& context, CCodeLines& dataDecl, CCodeLines& dataImpl) const
//	{
//		{
//			CCodeTemplate tpl0;
//			tpl0.ReadFromRawData(HardCodedTemplate::CreateFieldLayoutOfTypeDecl);
//			CLabelToCodeMapping map;
//			MapLabelToText(map, LABEL_2, context.m_createFieldLayoutOfTypeFuncName);
//			Niflect::TSet<Niflect::CString> setReplacedLabel;
//			tpl0.ReplaceLabels(map, dataDecl, &setReplacedLabel);
//			ASSERT(setReplacedLabel.size() == map.size());
//		}
//		{
//			CCodeTemplate tpl0;
//			tpl0.ReadFromRawData(HardCodedTemplate::CreateFieldLayoutOfTypeImpl);
//			CLabelToCodeMapping map;
//			MapLabelToText(map, LABEL_2, context.m_createFieldLayoutOfTypeFuncName);
//
//			CCodeLines linesBody;
//			{
//				Niflect::CString accessorResoCursorName;
//				if (m_bindingTypeIndexedRoot->m_accessorBindingIndex != INDEX_NONE)
//				{
//					auto& setting = m_resolvedData->m_accessorBindingMapping->m_settings.m_vecAccessorBindingSetting[m_bindingTypeIndexedRoot->m_accessorBindingIndex];
//					accessorResoCursorName = setting.m_accessorTypeCursorName;
//
//					if (IsCursorTemplateDecl(setting.GetAccessorTypeDecl().m_cursorDecl))//注, 特化的 Kind 为 ClassDecl
//					{
//						auto& arg = m_bindingTypeIndexedRoot->m_resocursorName;
//						NiflectGenDefinition::CodeStyle::TemplateAngleBracketL(accessorResoCursorName);
//						accessorResoCursorName += arg;
//						NiflectGenDefinition::CodeStyle::TemplateAngleBracketR(accessorResoCursorName);
//					}
//				}
//				else
//				{
//					//auto& taggedType = m_resolvedData->m_taggedMapping.m_vecType[m_bindingTypeIndexedRoot->m_taggedTypeIndex];
//					//accessorResoCursorName = CXStringToCString(clang_getCursorSpelling(taggedType->GetCursor()));
//
//					accessorResoCursorName = m_resolvedData->m_accessorBindingMapping->m_settings.m_settingCompound.m_accessorTypeCursorName;
//				}
//
//				linesBody.push_back(accessorResoCursorName);
//				MapLabelToLines(map, LABEL_3, linesBody);
//			}
//
//			this->WriteResocursorNodeBodyCode(linesBody);
//
//			Niflect::TSet<Niflect::CString> setReplacedLabel;
//			tpl0.ReplaceLabels(map, dataImpl, &setReplacedLabel);
//			ASSERT(setReplacedLabel.size() == map.size());
//		}
//
//#ifdef DEBUG_FOR_TYPE_REG
//		DebugPrintCodeLines(dataImpl);
//#endif
//	}
//}