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
		//Linux ͨ�� clang_getSpellingLocation �»�ȡ���� lineNumber ʼ�ղ���, ��Ϊ5, ���ͨ������İ취��ȡ�к�, ʵ���϶��ڴ˱��, WIN32 ��Ҳ��ʹ��
		//2024.11.24, ��ȷ�ϴ�����Ϊ�� Unbutu ������ libclang 20.0.0, Ӧ�� libclang 17.0.6, ���������Ƿͨ����, ��Ŀǰ�޸��ð취, ͬʱ�ܻ�ȡ��ȷ�� Nata
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
			ASSERT(lineNumber > 0);//�б�Ҫ��������������͵� LineNumber ��1 ?
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
		//����
		ASSERT(m_baseTaggedType == NULL);
		auto baseTypeCursorDecl = clang_getTypeDeclaration(clang_getCursorType(m_baseTypeSpecifierCursor));
		auto& taggedMapping = context.m_mappings.m_tagged;
		auto itFound = taggedMapping.m_mapCursorToIndex.find(baseTypeCursorDecl);
		if (itFound != taggedMapping.m_mapCursorToIndex.end())
			m_baseTaggedType = taggedMapping.m_vecType[itFound->second];

		//��Ա����
		ASSERT(m_vecMemberField.size() == 0);
		for (auto& it : m_vecChild)
		{
			//Ƕ������ҲΪtaggedType���ӽڵ�
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
				//���ƻ�֧�� Field ������Ϊ BindingType �ı���, ��Ϊ�����������ñ���, ������ AccessorBinding �Ķ�Ӧ��ϵ��ֱ��, ����Ҫʹ�ñ���, Ӧ�� AccessorBinding ��ָ��, �� Field ��ʹ��ʱ, Ӧֱ��ʹ�ñ���
				//����Ϊȷʵ��Ҫʵ��֧�ִ��÷�, �ɿ����ڲ��ҹ�����, ������ aliasChain ���Ҷ�Ӧ�� BindingType
				GenLogError(context.m_log, NiflectUtil::FormatString("The accessor of the field %s::%s is not specified", m_resocursorName.c_str(), CXStringToCString(clang_getCursorSpelling(fieldCursor)).c_str()));
				break;
			}
			//DebugPrintIndexedNodeRecurs(indexedRoot, indexedRoot, context.m_bindingAccessorMapping, 0);
			ResolveSignature(indexedRoot, context, data.m_signatureMapping);
		}

		//�ں�ִ��, ��Ϊʹ��Ա������������ע��, ʵ����˳�򲢲���Ҫ, ����Ϊ����������ǰ������鿴
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