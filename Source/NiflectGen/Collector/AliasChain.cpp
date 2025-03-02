#include "NiflectGen/Collector/AliasChain.h"
#include "NiflectGen/Util/CursorUtil.h"

namespace NiflectGen
{
	CAliasInfo::CAliasInfo()
		: m_decl(g_invalidCursor)
		, m_underlyingType({})
		, m_templateArgsCount(0)
	{

	}
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
	CAliasInfo& CAliasChain::AddLinkDeclInternal(const CXCursor& decl)
	{
		auto ret = m_mapDeclToIndex.insert({ decl, static_cast<uint32>(m_vecItem.size()) });
		ASSERT(ret.second);
		m_vecItem.push_back(CAliasInfo());
		return m_vecItem.back();
	}
	void CAliasChain::AddLinkDecl2(const CXCursor& decl)
	{
		this->AddLinkDeclInternal(decl);
	}
	static CXType GetUnderlyingTypeOfTypeAliasTemplate(CXCursor typeAliasTemplateCursor) {
		CXType resultType{};
		clang_visitChildren(
			typeAliasTemplateCursor,
			[](CXCursor c, CXCursor parent, CXClientData client_data) {
				if (clang_getCursorKind(c) == CXCursor_TypeAliasDecl) {
					CXType* typePtr = static_cast<CXType*>(client_data);
					*typePtr = clang_getCursorType(c);
					return CXChildVisit_Break; // ÕÒµ½ºóÍ£Ö¹±éÀú
				}
				return CXChildVisit_Continue;
			},
			&resultType
		);
		return resultType;
	}
	void CAliasChain::AddLinkAliasDecl(const CXCursor& decl)
	{
		auto& item = this->AddLinkDeclInternal(decl);
		item.m_underlyingType = clang_getTypedefDeclUnderlyingType(decl);
		ASSERT(item.m_underlyingType.kind != CXType_Invalid);
	}
	void CAliasChain::AddLinkTemplateAliasDecl(const CXCursor& decl)
	{
		auto& item = this->AddLinkDeclInternal(decl);
		auto a = GetUnderlyingTypeOfTypeAliasTemplate(decl);
		auto b = clang_getTypeDeclaration(a);
		item.m_underlyingType = clang_getTypedefDeclUnderlyingType(b);
		ASSERT(item.m_underlyingType.kind != CXType_Invalid);
	}
#else
	void CAliasChain::AddLinkDecl(const CXCursor& decl)
	{
		auto ret = m_mapDeclToIndex.insert({ decl, static_cast<uint32>(m_vecItem.size()) });
		ASSERT(ret.second);
		m_vecItem.push_back(CAliasInfo());
	}
#endif
	void CAliasChain::LinkToReferenced(const CXCursor& decl, const CXCursor& alias, uint32 templateArgsCount)
	{
		auto itFound = m_mapDeclToIndex.find(decl);
		if (itFound != m_mapDeclToIndex.end())
		{
			auto& info = m_vecItem[itFound->second];
			info.m_decl = clang_getCursorReferenced(alias);
			info.m_templateArgsCount = templateArgsCount;
		}
	}
	//CXCursor CAliasChain::FindOriginalDeclOld(CXCursor decl) const
	//{
	//	CXCursor lastValidDecl = decl;
	//	while (true)
	//	{
	//		auto a = CXStringToCString(clang_getCursorSpelling(decl));
	//		auto templateDecl = clang_getSpecializedCursorTemplate(decl);
	//		if (!clang_Cursor_isNull(templateDecl))
	//			decl = templateDecl;
	//		if (!IsCursorAliasDecl(decl))
	//			return decl;
	//		auto itFound = m_mapDeclToReferenced.find(decl);
	//		if (itFound != m_mapDeclToReferenced.end())
	//			decl = itFound->second;
	//		else
	//			break;
	//		if (clang_Cursor_isNull(decl))
	//			break;
	//		lastValidDecl = decl;
	//	}
	//	return lastValidDecl;
	//}

	//bool CAliasChain::FindOriginalDecl(CXCursor decl, CXCursor& lastValidDecl) const
	//{
	//	CXCursor lastValidDecl = decl;
	//	bool same = true;
	//	while (true)
	//	{
	//		auto a = CXStringToCString(clang_getCursorSpelling(decl));
	//		auto templateDecl = clang_getSpecializedCursorTemplate(decl);
	//		if (!clang_Cursor_isNull(templateDecl))
	//		{
	//			decl = templateDecl;
	//			same = false;
	//		}
	//		if (!IsCursorAliasDecl(decl))
	//		{
	//			lastValidDecl = decl;
	//			break;
	//		}
	//		auto itFound = m_mapDeclToReferenced.find(decl);
	//		if (itFound != m_mapDeclToReferenced.end())
	//		{
	//			decl = itFound->second;
	//			same = false;
	//		}
	//		else
	//		{
	//			break;
	//		}
	//		if (clang_Cursor_isNull(decl))
	//			break;
	//		lastValidDecl = decl;
	//		same = false;
	//	}
	//	return same;
	//}

	bool CAliasChain::FindOriginalDecl(CXCursor decl, CXCursor& lastValidDecl, bool stoppedOnClassDecl) const
	{
		lastValidDecl = decl;
		bool diff = false;
		uint32 originalTemplateArgsCount = 0;
		while (true)
		{
			auto a = CXStringToCString(clang_getCursorSpelling(decl));
			if (stoppedOnClassDecl)
			{
				if (clang_getCursorKind(decl) == CXCursor_ClassDecl)
				{
					lastValidDecl = decl;
					break;
				}
			}
			auto templateDecl = clang_getSpecializedCursorTemplate(decl);
			if (!clang_Cursor_isNull(templateDecl))
			{
				decl = templateDecl;
				if (!IsCursorAliasDecl(decl))
				{
					lastValidDecl = decl;
					diff = true;
					break;
				}
			}
			else
			{
				if (!IsCursorAliasDecl(decl))
				{
					lastValidDecl = decl;
					break;
				}
			}
			auto itFound = m_mapDeclToIndex.find(decl);
			if (itFound != m_mapDeclToIndex.end())
				decl = m_vecItem[itFound->second].m_decl;
			else
				break;
			if (clang_Cursor_isNull(decl))
				break;
			lastValidDecl = decl;
			diff = true;
		}
		return diff;
	}
}