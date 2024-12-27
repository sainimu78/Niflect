#include "NiflectGen/TaggedType/TaggedType.h"
#include "NiflectGen/Util/CursorUtil.h"
#include "Niflect/Util/DebugUtil.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"

namespace NiflectGen
{
	static Niflect::CString ResolveSignatureRecurs(const CResolvedCursorNode& indexedParent, const CResolvedCursorNode& childrenOwnerOld, const CResolvingDependenciesContext& ctx, CResolvedCursorRootsMapping& signatureMapping)
	{
		auto ret = signatureMapping.m_mapKeyToIndex.insert({ indexedParent.m_key, static_cast<uint32>(signatureMapping.m_vecItem.size()) });
		if (ret.second)
		{
			auto& childrenOwner = indexedParent;
			if (indexedParent.m_accessorBindingIndex != INDEX_NONE)
			{
				//for (uint32 idx = 0; idx < childrenOwner.m_vecChild.size(); ++idx)
				//{
				//	auto& it = childrenOwner.m_vecChild[idx];
				//	MakeSignatureForFieldRecurs(it, it, ctx, signatureMapping);
				//}
				if (auto elem = indexedParent.m_elem.Get())
				{
					ASSERT(childrenOwner.m_vecChild.size() == 0);
					ResolveSignatureRecurs(*elem, *elem, ctx, signatureMapping);
				}
				else
				{
					for (auto& it : childrenOwner.m_vecChild)
						ResolveSignatureRecurs(it, it, ctx, signatureMapping);
				}
			}
			else if (indexedParent.m_taggedTypeIndex != INDEX_NONE)
			{
				//类或结构体定义不继续递归, 计划在生成阶段遍历member
				ASSERT(childrenOwner.m_vecChild.size() == 0);
				ASSERT(indexedParent.m_elem == NULL);
			}
			else
			{
				ASSERT(!indexedParent.m_resocursorName.empty());
			}
			signatureMapping.m_vecItem.push_back(CResolvedCursorRoots(Niflect::CString(), indexedParent));
		}
		else
		{
		}

		return Niflect::CString();
	}

	Niflect::CString ResolveSignature(const CResolvedCursorNode& indexedParent, const CResolvingDependenciesContext& ctx, CResolvedCursorRootsMapping& signatureMapping)
	{
		return ResolveSignatureRecurs(indexedParent, indexedParent, ctx, signatureMapping);
	}

	void CTaggedType::ResolveBasic(const CResolvingDependenciesContext& context, SResolvingDependenciesData& data)
	{
		auto& taggedMapping = context.m_mappings.m_tagged;
		auto& cursor = this->GetCursor();
		ASSERT(!m_taggedResoRoot.IsValid());
#ifdef TODO_SIMPLIFY_TAGGED_TYPE_INDEXED_ROOT_INITIALIZATION
		实际上在插入到 taggedMapping 时, 就已经能正确获取 IndexedRoot 需要的信息, 因此可考虑简化流程
#else
#endif
		if (!taggedMapping.InitIndexedNodeForClassDecl(cursor, context.m_mappings.m_accessorSetting, m_taggedResoRoot))
		{
			ASSERT(false);
		}
		ResolveSignature(m_taggedResoRoot, context, data.m_signatureMapping);
	}
	void CTaggedType::WriteUsingNamespaceDirectiveForNata(CCodeLines& lines) const
	{
		//指定在与 Field 所有者同 Scope, 可使 Nata 提供方式可编译, 此 using 后的代码可与所有者共享声明过的 Scope
		if (m_vecScopeName.size() > 0)
			lines.push_back("using namespace " + m_vecScopeName[0] + ";// This is for Nata");
	}
	void CTaggedType::WriteTaggedTypeCopyNata(CCodeLines& lines) const
	{
		if (m_linesRawNata.size() > 0)
		{
			this->WriteUsingNamespaceDirectiveForNata(lines);
			this->WriteCopyNataCode(lines);
		}
	}

	void CTaggedTypesMapping::Resolve()
	{
		for (auto& it : m_vecType)
			it->ResolveForResocursorNode();
	}
	bool CTaggedTypesMapping::InitIndexedNodeForClassDecl(const CXCursor& cursor, const CAccessorBindingMapping2& accessorSettingMapping, CResolvedCursorNode& indexedParent) const
	{
		auto itFound = m_mapCursorToIndex.find(cursor);
		if (itFound != m_mapCursorToIndex.end())
		{
			ASSERT(!indexedParent.IsValid());
			auto headerFilePath = GetCursorFilePath(cursor);
			ASSERT(!headerFilePath.empty());
			uint32 taggedTypeIdx = INDEX_NONE;
			auto itFound2 = accessorSettingMapping.m_mapSpecializedCursorToIndex.find(cursor);
			if (itFound2 != accessorSettingMapping.m_mapSpecializedCursorToIndex.end())
				taggedTypeIdx = itFound2->second;
			indexedParent.InitForClassDecl(m_vecType[itFound->second]->m_resocursorName, itFound->second, taggedTypeIdx, headerFilePath);
			return true;
		}
		return false;
	}
	Niflect::CString CTaggedTypesMapping::GetInfoTypeName(uint32 taggedTypeIdx) const
	{
		Niflect::CString infoTypeName = NiflectGenDefinition::NiflectFramework::InfoTypeName::NiflectType;
		this->GetDerivedInfoTypeName(taggedTypeIdx, infoTypeName);
		return infoTypeName;
	}
	bool CTaggedTypesMapping::GetDerivedInfoTypeName(uint32 taggedTypeIdx, Niflect::CString& infoTypeName) const
	{
		bool isDerivedType = false;
		if (taggedTypeIdx != INDEX_NONE)
		{
			auto& tagged = m_vecType[taggedTypeIdx];
			auto& cursor = tagged->GetCursor();
			auto kind = clang_getCursorKind(cursor);
			switch (kind)
			{
			case CXCursor_ClassDecl: infoTypeName = NiflectGenDefinition::NiflectFramework::InfoTypeName::Class; break;
			case CXCursor_StructDecl: infoTypeName = NiflectGenDefinition::NiflectFramework::InfoTypeName::Struct; break;
			case CXCursor_EnumDecl: infoTypeName = NiflectGenDefinition::NiflectFramework::InfoTypeName::Enum; break;
			default:
				ASSERT(false);
				return false;//预留的检查, 须避免返回true
				break;
			}
			isDerivedType = true;
		}
		return isDerivedType;
	}

	void CTaggedType::ResolveForResocursorNode()
	{
		auto& cursor = this->GetCursor();
		ASSERT(m_vecScopeName.size() == 0);
		FindNamespaceAndScopeNameRecurs2(cursor, m_vecScopeName);
		auto resocursorName = GenerateScopesCodeFromScopeNames(m_vecScopeName);;
		resocursorName += CXStringToCString(clang_getCursorSpelling(cursor));
		m_resocursorName = resocursorName;
	}
	void CTaggedType::InitForImportType()
	{
		ASSERT(!m_isImportType);
		m_isImportType = true;
	}

	CTaggedInheritableTypeMember::CTaggedInheritableTypeMember()
		: m_accessSpecifier(CX_CXXPrivate)
	{

	}
	void CTaggedInheritableTypeMember::InitMember(CX_CXXAccessSpecifier accessSpecifier)
	{
		m_accessSpecifier = accessSpecifier;
	}
	bool CTaggedInheritableTypeMember::CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context)
	{
		m_vecDetailCursor.push_back(cursor);
		return false;
	}
	void CTaggedInheritableTypeMember::DebugDerivedPrint(FILE* fp) const
	{
		const char* memberTypeStr = NULL;
		switch (clang_getCursorKind(this->GetCursor()))
		{
		case CXCursor_FieldDecl: memberTypeStr = "A field"; break;
		case CXCursor_CXXMethod: memberTypeStr = "A method"; break;
		}

		const char* accessStr = NULL;
		switch (m_accessSpecifier) {
		case CX_CXXInvalidAccessSpecifier:
			ASSERT(false);
			accessStr = "invalid"; break;
		case CX_CXXPublic:
			accessStr = "public"; break;
		case CX_CXXProtected:
			accessStr = "protected"; break;
		case CX_CXXPrivate:
			accessStr = "private"; break;
		}
		printf(", %s, %s", memberTypeStr, accessStr);
	}

	CUntaggedTemplate::CUntaggedTemplate()
		: m_originalUntaggedDecl(NULL)
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
		, m_argsCount(0)
#else
#endif
	{
	}
	bool CUntaggedTemplate::CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context)
	{
		bool addedTaggedChild = false;
		auto kind = clang_getCursorKind(cursor);
		bool ok = false;
		if (kind == CXCursor_FieldDecl)
			ok = true;
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
		else if (kind == CXCursor_TemplateTypeParameter)
			m_argsCount++;
#else
#endif
		if (ok)
		{
			auto taggedChild = Niflect::MakeShared<CTaggedInheritableTypeMember>();
			this->AddChildAndInitDefault(taggedChild, cursor, g_invalidCursor);
			addedTaggedChild = true;
		}
		return addedTaggedChild;
	}
	void CUntaggedTemplate::InitForAlias(const CAliasChain& aliasChain, const CUntaggedTemplatesMapping& untaggedMapping)
	{
		CXCursor originalCursor;
		//auto a = CXStringToCString(clang_getCursorSpelling(this->GetCursor()));
		//if (a == "_Apply")
		//	printf("");
		//auto dddddddd = aliasChain.FindOriginalDeclOld(this->GetCursor());
		if (aliasChain.FindOriginalDecl(this->GetCursor(), originalCursor))
		{
			auto itFound = untaggedMapping.m_mapCursorToIndex.find(originalCursor);
			//ASSERT(itFound != untaggedMapping.m_mapCursorToIndex.end());
			if (itFound != untaggedMapping.m_mapCursorToIndex.end())
				m_originalUntaggedDecl = untaggedMapping.m_vecType[itFound->second];
		}
	}

#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
	CUntaggedType::CUntaggedType()
	{

	}
	bool CUntaggedType::CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context)
	{
		//return inherited::CollectSibling(cursor, context);
		auto untaggedChild = Niflect::MakeShared<CUntaggedTemplate>();
		this->AddChildAndInitDefault(untaggedChild, cursor, g_invalidCursor);
		return true;
	}
#else
#endif
}