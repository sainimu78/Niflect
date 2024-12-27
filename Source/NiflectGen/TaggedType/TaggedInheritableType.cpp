#include "NiflectGen/TaggedType/TaggedInheritableType.h"
#include "NiflectGen/Util/CursorUtil.h"
#include "NiflectGen/CodeWriter/TypeReg/InheritableTypeRegCodeWriter.h"
#include "Niflect/Util/DebugUtil.h"
#include "NiflectGen/CodeWriter/TypeReg/InheritableTypeRegCodeWriter2.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"

namespace NiflectGen
{
	CTaggedInheritableType::CTaggedInheritableType()
		: m_baseTypeSpecifierCursor(g_invalidCursor)
		, m_baseTaggedType(NULL)
		, m_generatedBodyLineNumber(INDEX_NONE)
	{
	}
	void CTaggedInheritableType::InitBaseTypeSpecifierCursor(const CXCursor& cursor)
	{
		ASSERT(clang_Cursor_isNull(m_baseTypeSpecifierCursor));
		m_baseTypeSpecifierCursor = cursor;
	}
	bool CTaggedInheritableType::CollectGeneratedBodyTag(const CXCursor& cursor, const CXCursorKind& kind)
	{
		//Linux 通过 clang_getSpellingLocation 下获取到的 lineNumber 始终不变, 如为5, 因此通过另外的办法获取行号, 实际上对于此标记, WIN32 下也可使用
		//2024.11.24, 已确认此现象为在 Unbutu 下误用 libclang 20.0.0, 应用 libclang 17.0.6, 这表明方法欠通用性, 但目前无更好办法, 同时能获取正确的 Nata
		if (false)
		{
			if (FindTagByDisplayName(cursor, NiflectGenDefinition::CodeTag::GeneratedBody))
			{
				ASSERT(m_generatedBodyLineNumber == INDEX_NONE);
				CXSourceLocation location = clang_getCursorLocation(cursor);
				uint32 lineNumber;
				clang_getSpellingLocation(location, NULL, &lineNumber, NULL, NULL);
				m_generatedBodyLineNumber = lineNumber;
				return true;
			}
		}

		const auto dispName = CXStringToCString(clang_getCursorDisplayName(cursor));
		const Niflect::CString tagName = NiflectGenDefinition::CodeTag::GeneratedBody;
		auto pos = dispName.find(tagName);
		if (pos != std::string::npos)
		{
			ASSERT(m_generatedBodyLineNumber == INDEX_NONE);
			uint32 lineNumber;
			auto tagLen = tagName.length();
			auto numInStr = dispName.substr(tagLen, dispName.length() - tagLen);
			ASSERT(numInStr.size() > 0);
			lineNumber = std::atoi(numInStr.c_str());
			ASSERT(lineNumber > 0);//有必要检测必须比所属类型的 LineNumber 大1 ?
			m_generatedBodyLineNumber = lineNumber;
			return true;
		}
		return false;
	}
	void CTaggedInheritableType::ErrorIfNoGeneratedBodyTag(const CXCursor& cursor) const
	{
		//auto a = CXStringToCString(clang_getCursorSpelling(cursor));
		//ASSERT(m_generatedBodyLineNumber != INDEX_NONE);
	}
	void CTaggedInheritableType::Deprecated_ResolveDependcies(const TCursorMap<CTaggedType*>& mapCursorDeclToTaggedType)
	{
		ASSERT(m_baseTaggedType == NULL);
		auto baseTypeCursorDecl = clang_getTypeDeclaration(clang_getCursorType(m_baseTypeSpecifierCursor));
		auto itFound = mapCursorDeclToTaggedType.find(baseTypeCursorDecl);
		if (itFound != mapCursorDeclToTaggedType.end())
			m_baseTaggedType = itFound->second;
	}
	static void DebugPrintIndexedNodeRecurs(const CResolvedCursorNode& indexedParent, const CResolvedCursorNode& childrenOwner, const CAccessorBindingMapping2& mapping, uint32 lv)
	{
		auto strLv = NiflectUtil::DebugIndentToString(lv);
		auto& cxType = mapping.m_settings.m_vecAccessorBindingSetting[indexedParent.m_accessorBindingIndex].GetAccessorTypeDecl().m_CXType;
		auto a = CXStringToCString(clang_getTypeSpelling(cxType));
		printf("%s%s\n", strLv.c_str(), a.c_str());

		lv++;
		for (auto& it : childrenOwner.m_vecChild)
			DebugPrintIndexedNodeRecurs(it, it, mapping, lv);

		if (auto elem = indexedParent.m_elem.Get())
		{
			lv--;
			printf("---------------\n");
			DebugPrintIndexedNodeRecurs(*elem, indexedParent, mapping, lv);
		}
	}
	void CTaggedInheritableType::ResolveDependcies(const CResolvingDependenciesContext& context, SResolvingDependenciesData& data)
	{
		//基类
		ASSERT(m_baseTaggedType == NULL);
		auto baseTypeCursorDecl = clang_getTypeDeclaration(clang_getCursorType(m_baseTypeSpecifierCursor));
		auto& taggedMapping = context.m_mappings.m_tagged;
		auto itFound = taggedMapping.m_mapCursorToIndex.find(baseTypeCursorDecl);
		if (itFound != taggedMapping.m_mapCursorToIndex.end())
			m_baseTaggedType = taggedMapping.m_vecType[itFound->second];

		//成员变量
		ASSERT(m_vecMemberField.size() == 0);
		for (auto& it : m_vecChild)
		{
			//嵌套类型也为taggedType的子节点
			if (auto member = CTaggedInheritableTypeField::CastChecked(it.Get()))
				m_vecMemberField.push_back(member);
			else if (auto member = CTaggedInheritableTypeMethod::CastChecked(it.Get()))
				m_vecMemberMethod.push_back(member);
#ifdef PORTING_ACCESS_METHOD
			else if (auto member = CTaggedInheritableTypeAccessMethod::CastChecked(it.Get()))
				m_vecMemberAccessMethod.push_back(member);
#endif
		}

		m_vecMemberIndexedRoot.resize(m_vecMemberField.size());
		for (uint32 idx0 = 0; idx0 < m_vecMemberField.size(); ++idx0)
		{
			auto& it0 = m_vecMemberField[idx0];
			auto& indexedRoot = m_vecMemberIndexedRoot[idx0];
			auto& fieldCursor = it0->GetCursor();
			SResonodesInitContext asMapCtx{ context.m_log };
			context.m_mappings.m_accessorSetting.InitIndexedNodeForField(asMapCtx, m_taggedResoRoot, fieldCursor, it0->m_vecDetailCursor, taggedMapping, context.m_mappings.m_untaggedTemplate, indexedRoot);
			if (!indexedRoot.IsValid())
			{
				//不计划支持 Field 的类型为 BindingType 的别名, 因为这样可能滥用别名, 导致与 AccessorBinding 的对应关系不直观, 如需要使用别名, 应在 AccessorBinding 中指定, 在 Field 中使用时, 应直接使用别名
				//如认为确实需要实现支持此用法, 可考虑在查找过程中, 引入逐级 aliasChain 查找对应的 BindingType
				GenLogError(context.m_log, NiflectUtil::FormatString("The accessor of the field %s::%s is not specified", m_resocursorName.c_str(), CXStringToCString(clang_getCursorSpelling(fieldCursor)).c_str()));
				break;
			}
			//DebugPrintIndexedNodeRecurs(indexedRoot, indexedRoot, context.m_bindingAccessorMapping, 0);
			ResolveSignature(indexedRoot, context, data.m_signatureMapping);
		}

		//在后执行, 仅为使成员依赖的类型先注册, 实际上顺序并不重要, 但认为依赖出现在前更方便查看
		inherited::ResolveDependcies(context, data);
	}
	CSharedTypeRegCodeWriter2 CTaggedInheritableType::CreateCodeWriter2() const
	{
		//CXCursor baseTypeCursorDecl = g_invalidCursor;
		//if (m_baseTaggedType != NULL)
		//	baseTypeCursorDecl = m_baseTaggedType->GetCursor();
		//return Niflect::MakeShared<CInheritableTypeRegCodeWriter_ObjectAccessor>(this->GetCursor(), setting, baseTypeCursorDecl, m_vecMember);

		return Niflect::MakeShared<CInheritableTypeRegCodeWriter2>(m_vecMemberIndexedRoot, m_baseTaggedType, m_generatedBodyLineNumber
			, m_vecMemberField
#ifdef PORTING_ACCESS_METHOD
			, m_vecMemberAccessMethod
#endif
		);
	}
	void CTaggedInheritableType::DebugDerivedPrint(FILE* fp) const
	{
		Niflect::CString baseClassStr;
		const auto& baseTypeCursor = m_baseTypeSpecifierCursor;
		if (clang_Cursor_isNull(baseTypeCursor))
			baseClassStr = "No base class";
		else
			baseClassStr = NiflectUtil::FormatString("Base class name: %s", CXStringToCString(clang_getCursorSpelling(baseTypeCursor)).c_str());
		printf(", %s", baseClassStr.c_str());
	}
}