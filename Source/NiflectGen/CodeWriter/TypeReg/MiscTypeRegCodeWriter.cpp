#include "NiflectGen/CodeWriter/TypeReg/MiscTypeRegCodeWriter.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/CodeWriter/CodeTemplate.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/Resolver/ResolvedData.h"
#include "NiflectGen/CodeWriter/TypeReg/TypeAccessorCodeWriter.h"

namespace NiflectGen
{
	static void WriteDDDDDDDD(const CResolvedCursorNode& indexedParent, uint32 lv, const char* pszLv, CCodeLines& lines)
	{
		lines.push_back(indexedParent.m_resocursorName);
	}
	static void WriteNexts(const CResolvedCursorNode& indexedParent, CCodeLines& lines)
	{
		uint32 lv = 0;
		const char* pszLv = "-";
		//WriteDDDDDDDD(indexedParent, lv, pszLv, lines);

		lv++;
		auto& childrenOwner = indexedParent;
		if (auto elem = indexedParent.m_elem.Get())
		{
			ASSERT(childrenOwner.m_vecChild.size() == 0);
			WriteDDDDDDDD(*elem, lv, "+", lines);
		}
		else
		{
			for (auto& it : childrenOwner.m_vecChild)
				WriteDDDDDDDD(it, lv, pszLv, lines);
		}
	}
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	void CMiscTypeRegCodeWriter::WriteResocursorNodeBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const
	{
		//linesResoBodyCode.push_back(NiflectUtil::FormatString("My Name: %s", m_bindingTypeIndexedRoot->m_resocursorName.c_str()));
		ASSERT(m_bindingTypeIndexedRoot->m_accessorBindingIndex != INDEX_NONE);
		if (m_bindingTypeIndexedRoot->m_untaggedTemplateIndex != INDEX_NONE)
		{
			//linesResoBodyCode.push_back("---------------");
			if (auto elemResocursorNode = m_bindingTypeIndexedRoot->m_elem.Get())
			{
				//auto d = NiflectUtil::FormatString("Element -> %s", elemResocursorNode->m_resocursorName.c_str());
				//linesResoBodyCode.push_back(d);

				auto& as = m_resolvedData->m_accessorBindingMapping->m_settings.m_vecAccessorBindingSetting[m_bindingTypeIndexedRoot->m_accessorBindingIndex];
				if (!as.m_accessorSettingResolvedInfo.m_isPointerTemplate)
				{
					auto elemStaticGetTypeFuncName = elemResocursorNode->GetStaticGetTypeFuncName(context.m_moduleRegInfo.m_moduleScopeSymbolPrefix);
					WriteNextInitElementAccessor2(elemStaticGetTypeFuncName, data.m_linesResoElemCode);
				}
			}
			else
			{
				auto ut = m_resolvedData->m_untaggedTemplateMapping.m_vecType[m_bindingTypeIndexedRoot->m_untaggedTemplateIndex];
				if (ut->m_originalUntaggedDecl != NULL)
					ut = ut->m_originalUntaggedDecl;
				ASSERT(ut->DebugGetChildren().size() > 0);//需要通过aliasChain查找原始定义, 以获取结构
				ASSERT(m_bindingTypeIndexedRoot->m_vecChild.size() == ut->GetChildrenCount());

				//for (uint32 idx = 0; idx < ut->DebugGetChildren().size(); ++idx)
				//{
				//	auto& it = ut->DebugGetChildren()[idx];
				//	auto& a = m_bindingTypeIndexedRoot->m_vecChild[idx].m_resocursorName;
				//	auto b = CXStringToCString(clang_getCursorSpelling(it->GetCursor()));
				//	auto d = NiflectUtil::FormatString("%s %s;", a.c_str(), b.c_str());
				//	linesResoBodyCode.push_back(d);
				//}


				for (uint32 idx = 0; idx < ut->DebugGetChildren().size(); ++idx)
				{
					auto it0 = ut->GetChild(idx);
					auto fieldStaticGetTypeFuncName = m_bindingTypeIndexedRoot->m_vecChild[idx].GetStaticGetTypeFuncName(context.m_moduleRegInfo.m_moduleScopeSymbolPrefix);
					auto fieldName = CXStringToCString(clang_getCursorSpelling(it0->GetCursor()));
					WriteNextInitChildAccessor2(m_bindingTypeIndexedRoot->m_resocursorName, false, m_bindingTypeIndexedRoot->m_resocursorName, fieldStaticGetTypeFuncName, fieldName, CCodeLines(), data.m_linesResoBodyCode);
				}
			}
		}
	}
#else
	void CMiscTypeRegCodeWriter::WriteResocursorNodeBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const
	{
		//linesResoBodyCode.push_back(NiflectUtil::FormatString("My Name: %s", m_bindingTypeIndexedRoot->m_resocursorName.c_str()));
		ASSERT(m_bindingTypeIndexedRoot->m_accessorBindingIndex != INDEX_NONE);
		if (m_bindingTypeIndexedRoot->m_untaggedTemplateIndex != INDEX_NONE)
		{
			//linesResoBodyCode.push_back("---------------");
			if (auto elemResocursorNode = m_bindingTypeIndexedRoot->m_elem.Get())
			{
				//auto d = NiflectUtil::FormatString("Element -> %s", elemResocursorNode->m_resocursorName.c_str());
				//linesResoBodyCode.push_back(d);

				auto& as = m_resolvedData->m_accessorBindingMapping->m_settings.m_vecAccessorBindingSetting[m_bindingTypeIndexedRoot->m_accessorBindingIndex];
				if (!as.m_accessorSettingResolvedInfo.m_isPointerTemplate)
				{
					auto elemStaticGetTypeFuncName = elemResocursorNode->GetStaticGetTypeFuncName(context.m_moduleRegInfo.m_moduleScopeSymbolPrefix);
					WriteNextInitElementAccessor2(elemStaticGetTypeFuncName, data.m_linesResoBodyCode);
				}
			}
			else
			{
				auto ut = m_resolvedData->m_untaggedTemplateMapping.m_vecType[m_bindingTypeIndexedRoot->m_untaggedTemplateIndex];
				if (ut->m_originalUntaggedDecl != NULL)
					ut = ut->m_originalUntaggedDecl;
				ASSERT(ut->DebugGetChildren().size() > 0);//需要通过aliasChain查找原始定义, 以获取结构
				ASSERT(m_bindingTypeIndexedRoot->m_vecChild.size() == ut->GetChildrenCount());

				//for (uint32 idx = 0; idx < ut->DebugGetChildren().size(); ++idx)
				//{
				//	auto& it = ut->DebugGetChildren()[idx];
				//	auto& a = m_bindingTypeIndexedRoot->m_vecChild[idx].m_resocursorName;
				//	auto b = CXStringToCString(clang_getCursorSpelling(it->GetCursor()));
				//	auto d = NiflectUtil::FormatString("%s %s;", a.c_str(), b.c_str());
				//	linesResoBodyCode.push_back(d);
				//}


				for (uint32 idx = 0; idx < ut->DebugGetChildren().size(); ++idx)
				{
					auto it0 = ut->GetChild(idx);
					auto fieldStaticGetTypeFuncName = m_bindingTypeIndexedRoot->m_vecChild[idx].GetStaticGetTypeFuncName(context.m_moduleRegInfo.m_moduleScopeSymbolPrefix);
					auto fieldName = CXStringToCString(clang_getCursorSpelling(it0->GetCursor()));
					WriteNextInitChildAccessor2(m_bindingTypeIndexedRoot->m_resocursorName, fieldStaticGetTypeFuncName, fieldName, CCodeLines(), data.m_linesResoBodyCode);
				}
			}
		}
	}
#endif
	void CMiscTypeRegCodeWriter::CollectDependencyHeaderFilePathAddrs(CDependencyHeaderFilePathAddrs& dependencyHeaderFilePathAddrs) const
	{
		m_bindingTypeIndexedRoot->GetHeaderFilePathAddrs(dependencyHeaderFilePathAddrs.m_vecDecl);
	}
}