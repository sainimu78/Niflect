#include "NiflectGen/Collector/Collector.h"
#include "Niflect/Util/StringUtil.h"
#include "NiflectGen/Log/Log.h"
#include "Niflect/Util/DebugUtil.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/Util/CursorUtil.h"
#include "NiflectGen/TaggedType/TaggedStruct.h"
#include "NiflectGen/TaggedType/TaggedClass.h"
#include "NiflectGen/TaggedType/TaggedEnum.h"
#include "NiflectGen/Collector/AccessorSetting.h"

#include "NiflectGen/Collector/TemplateSubcursor.h"//����, ��������ʵ��, ��CSubcursor

//����˵��
//CodeTag: ָNiflectGenר�õı��, ��NiflectGenDefinition::CodeTag�еĶ���, ��ӦNiflectMacro.h�е���typedef void* __NiflectGen_Type��ʶ
//Macro��MacroExpansion: ָNiflectMacro.h����NIF_T��, չ�����ݼ�ΪTag, ������Ƶĺô������û����Զ���NIF_T����������
//TaggedNode: ָ��CodeTag���Ϊ����������Ͷ���, ��֧��3��, class, struct, enum
//Subcursor: ������Cursor����, ����Cursor����֧�ֵ�һЩ�����Ե����νṹ��ʾ, ������BindingSettingģ����ʵ�������α�ʾ

namespace NiflectGen
{
	using namespace Niflect;

	using CCursorArray = Niflect::TArrayNif<CXCursor>;

	static bool CXType_IsBuiltinType(CXTypeKind typeKind)
	{
		return (typeKind > CXType_FirstBuiltin && typeKind < CXType_LastBuiltin);
	}
	static void CollectTemplateSubcursorRecurs(const CXType& parentType, const CCursorArray& vecCursor, CTemplateSubcursor& parentSubcursor, int& cursorArrayFrontIndex)
	{
		parentSubcursor.m_type = parentType;
		auto templateArgsCount = clang_Type_getNumTemplateArguments(parentType);
		auto originalType = GetPointerOriginalType(parentType);
		if (!CXType_IsBuiltinType(originalType.kind))
		{
			ASSERT(cursorArrayFrontIndex < vecCursor.size());
			parentSubcursor.m_cursor = vecCursor[cursorArrayFrontIndex];
			cursorArrayFrontIndex++;
		}
		for (int idx = 0; idx < templateArgsCount; ++idx)
		{
			CXType argType = clang_Type_getTemplateArgumentAsType(parentType, idx);
			parentSubcursor.m_vecChild.push_back(CTemplateSubcursor());
			auto& childSubcursor = parentSubcursor.m_vecChild.back();
			CollectTemplateSubcursorRecurs(argType, vecCursor, childSubcursor, cursorArrayFrontIndex);
		}
	}
	//static void CollectTemplateSubcursorRecurs2(const CXType& parentType, const CCursorArray& vecCursor, CTemplateSubcursor& parentSubcursor, int& cursorArrayFrontIndex)
	//{
	//	auto templateArgsCount = clang_Type_getNumTemplateArguments(parentType);
	//	auto& cursor = vecCursor[cursorArrayFrontIndex];

	//	auto sssss = CXStringToCString(clang_getCursorSpelling(cursor));
	//	auto ddddd = CXStringToCString(clang_getTypeSpelling(parentType));

	//	auto cursorKind = clang_getCursorKind(cursor);
	//	if (cursorKind == CXCursor_TemplateRef)
	//	{
	//		parentSubcursor.m_cursor = cursor;
	//		parentSubcursor.m_type = parentType;
	//		if (templateArgsCount > 0)
	//		{
	//			parentSubcursor.m_vecChild.resize(templateArgsCount);
	//			cursorArrayFrontIndex++;
	//		}
	//	}
	//	else
	//	{
	//		if (cursorKind == CXCursor_NamespaceRef)
	//		{
	//			parentSubcursor.m_vecScopeCursor.push_back(cursor);
	//			parentSubcursor.m_vecScopeType.push_back(CXType{ CXType_Invalid });
	//			cursorArrayFrontIndex++;
	//			CollectTemplateSubcursorRecurs2(parentType, vecCursor, parentSubcursor, cursorArrayFrontIndex);
	//		}
	//		else if (cursorKind == CXCursor_TypeRef)
	//		{
	//			parentSubcursor.m_cursor = cursor;
	//			parentSubcursor.m_type = parentType;
	//			cursorArrayFrontIndex++;
	//		}
	//		else
	//		{
	//			parentSubcursor.m_type = parentType;
	//		}
	//		//auto originalType = GetPointerOriginalType(parentType);
	//		//if (!CXType_IsBuiltinType(originalType.kind))
	//		//{

	//		//}
	//	}

	//	for (int idx = 0; idx < templateArgsCount; ++idx)
	//	{
	//		auto argType = clang_Type_getTemplateArgumentAsType(parentType, idx);
	//		CollectTemplateSubcursorRecurs2(argType, vecCursor, parentSubcursor.m_vecChild[idx], cursorArrayFrontIndex);
	//	}
	//}
	 


	//static void CollectTemplateSubcursorRecurs2(const CXType& parentType, const CCursorArray& vecCursor, CTemplateSubcursor& parentSubcursor, int& cursorArrayFrontIndex)
	//{
	//	//auto templateArgsCount = clang_Type_getNumTemplateArguments(parentType);
	//	//auto& cursor = vecCursor[cursorArrayFrontIndex];
	//	////auto sssss = CXStringToCString(clang_getCursorSpelling(cursor));
	//	//auto ddddd = CXStringToCString(clang_getTypeSpelling(parentType));
	//	//printf("%s, %d\n", ddddd.c_str(), templateArgsCount);
	//	//printf("");

	//	//for (int idx = 0; idx < templateArgsCount; ++idx)
	//	//{
	//	//	auto argType = clang_Type_getTemplateArgumentAsType(parentType, idx);
	//	//	CollectTemplateSubcursorRecurs2(argType, vecCursor, parentSubcursor, cursorArrayFrontIndex);
	//	//}

	//	for (int idx = 0; idx < vecCursor.size(); ++idx)
	//	{
	//		auto& cursor = vecCursor[idx];
	//		auto type = clang_getCursorType(cursor);
	//		auto templateArgsCount = clang_Type_getNumTemplateArguments(type);
	//		auto sssss = CXStringToCString(clang_getCursorSpelling(cursor));
	//		//auto ddddd = CXStringToCString(clang_getTypeSpelling(parentType));
	//		printf("%s, %d\n", sssss.c_str(), templateArgsCount);
	//		printf("");
	//	}
	//}

	static void CollectTemplateSubcursorRecurs_______(const CCursorArray& vecCursor, int& cursorArrayFrontIndex)
	{
		while (true)
		{
			auto& cursor = vecCursor[cursorArrayFrontIndex];

			CXType typeInTemplateScope = clang_getCursorType(cursor);
			int numTemplateArgs = clang_Type_getNumTemplateArguments(typeInTemplateScope);
			//auto kind = clang_getCursorKind(cursor);
			//if (kind == CXCursor_TypeRef)
			//{
			//	auto typeRef = clang_getCursorType(cursor);
			//	auto decl = clang_getTypeDeclaration(typeRef);
			//	auto a = CXStringToCString(clang_getCursorSpelling(decl));
			//	auto b = CXStringToCString(clang_getCursorSpelling(endCursor));
			//	if (clang_equalCursors(decl, endCursor))
			//		break;
			//}
			cursorArrayFrontIndex++;
		}
	}

	static void CollectTemplateSubcursorRecurs2(const CXType& parentType, const CCursorArray& vecCursor, CTemplateSubcursor& parentSubcursor, int& cursorArrayFrontIndex)
	{
		auto templateArgsCount = clang_Type_getNumTemplateArguments(parentType);
		auto& cursor = vecCursor[cursorArrayFrontIndex];

		parentSubcursor.m_type = parentType;
		parentSubcursor.m_cursor = clang_getTypeDeclaration(parentType);
		if (templateArgsCount > 0)
			parentSubcursor.m_vecChild.resize(templateArgsCount);

		////if (parentType.kind == CXType_Typedef)
		//if (clang_getCursorKind(parentSubcursor.m_cursor) == CXCursor_TemplateRef)
		//	CollectTemplateSubcursorRecurs_______(vecCursor, parentSubcursor.m_cursor, cursorArrayFrontIndex);
		
		//parentSubcursor.m_ccccccRoot.m_vecChild.push_back(CCCCCCCCCCC());
		//parentSubcursor.m_ccccccRoot.m_vecChild.back().m_cursor = vecCursor[cursorArrayFrontIndex];
		//cursorArrayFrontIndex++;
		
		//CollectTemplateSubcursorRecurs_______(vecCursor, cursorArrayFrontIndex);

		for (int idx = 0; idx < templateArgsCount; ++idx)
		{
			auto argType = clang_Type_getTemplateArgumentAsType(parentType, idx);
			auto& child = parentSubcursor.m_vecChild[idx];
			CollectTemplateSubcursorRecurs2(argType, vecCursor, child, cursorArrayFrontIndex);
			//CollectTemplateSubcursorRecurs_______(vecCursor, parentSubcursor.m_ccccccRoot.m_vecChild.back(), parentSubcursor.m_cursor, cursorArrayFrontIndex);
		}
	}


	static void FindNamespaceCursorNameRecurs(const CXCursor& cursor, CString& strNamespace)
	{
		CXCursor parentCursor;
		if (clang_getCursorKind(cursor) == CXCursor_TemplateRef)
		{
			parentCursor = clang_getCursorReferenced(cursor);
			FindNamespaceCursorNameRecurs(parentCursor, strNamespace);
		}
		else
		{
			parentCursor = clang_getCursorSemanticParent(cursor);
		}

		if (clang_getCursorKind(parentCursor) == CXCursor_Namespace)
		{
			FindNamespaceCursorNameRecurs(parentCursor, strNamespace);

			CXString namespaceName = clang_getCursorSpelling(parentCursor);
			strNamespace = CString(clang_getCString(namespaceName));
			clang_disposeString(namespaceName);
		}
		else if (clang_getCursorKind(parentCursor) == CXCursor_ClassDecl)
		{
			FindNamespaceCursorNameRecurs(parentCursor, strNamespace);
		}
		else if (clang_getCursorKind(parentCursor) == CXCursor_TranslationUnit)
		{
			strNamespace = "CustomInGlobal";
			return;
		}
	}
	static void PrintTemplateSubcursor(const CTemplateSubcursor& parentSubcursor, int level)
	{
		CString displayName;
		auto originalType = parentSubcursor.m_type;
		while (originalType.kind == CXType_Pointer)
			originalType = clang_getPointeeType(originalType);
		CXCursor cursor = parentSubcursor.m_cursor;
		if (parentSubcursor.m_type.kind != CXType_Invalid)
			cursor = clang_getTypeDeclaration(originalType);
		if (parentSubcursor.m_type.kind == CXType_Pointer)
		{
			displayName = CXStringToCString(clang_getTypeSpelling(parentSubcursor.m_type));
		}
		else
		{
			if (clang_getCursorKind(cursor) != CXCursor_NoDeclFound)
				displayName = CXStringToCString(clang_getCursorSpelling(parentSubcursor.m_cursor));//�����cursor��ȡ��ɿ���using��������ͱ���
			else
				displayName = CXStringToCString(clang_getTypeSpelling(originalType));
		}

		CString strNamespace;
		{
			if (clang_getCursorKind(cursor) != CXCursor_NoDeclFound)
				FindNamespaceCursorNameRecurs(cursor, strNamespace);
			else
				strNamespace = "BuiltinInGlobal";
		}

		auto strLevel = NiflectUtil::DebugIndentToString(level);
		printf("%s%s, %s\n", strLevel.c_str(), displayName.c_str(), strNamespace.c_str());

		level++;
		for (auto& it : parentSubcursor.m_vecChild)
			PrintTemplateSubcursor(it, level);
	}

	void PrintTemplateSubcursor333333333(const CTemplateSubcursor& parentSubcursor, int level)
	{
		CString displayName;
		auto originalType = parentSubcursor.m_type;
		while (originalType.kind == CXType_Pointer)
			originalType = clang_getPointeeType(originalType);
		CXCursor cursor = parentSubcursor.m_cursor;
		if (parentSubcursor.m_type.kind != CXType_Invalid)
			cursor = clang_getTypeDeclaration(originalType);
		if (parentSubcursor.m_type.kind == CXType_Pointer)
		{
			displayName = CXStringToCString(clang_getTypeSpelling(parentSubcursor.m_type));
		}
		else
		{
			if (clang_getCursorKind(cursor) != CXCursor_NoDeclFound)
				displayName = CXStringToCString(clang_getCursorSpelling(parentSubcursor.m_cursor));//�����cursor��ȡ��ɿ���using��������ͱ���
			else
				displayName = CXStringToCString(clang_getTypeSpelling(originalType));
		}

		CString strNamespace;
		{
			if (clang_getCursorKind(cursor) != CXCursor_NoDeclFound)
				FindNamespaceCursorNameRecurs(cursor, strNamespace);
			else
				strNamespace = "BuiltinInGlobal";
		}

		auto strLevel = NiflectUtil::DebugIndentToString(level);
		printf("%s%s, %s\n", strLevel.c_str(), displayName.c_str(), strNamespace.c_str());

		level++;
		for (auto& it : parentSubcursor.m_vecChild)
			PrintTemplateSubcursor333333333(it, level);
	}

	class CScopeNamespace
	{
	public:
		CScopeNamespace(CDataCollector& collection, const CXCursor& cursor)
			: m_collection(collection)
			, m_entered(false)
		{
			if (clang_getCursorKind(cursor) == CXCursor_Namespace)
			{
				m_collection.m_stkNamespaceCursor.push_back(cursor);
				m_entered = true;
			}
		}
		~CScopeNamespace()
		{
			if (m_entered)
				m_collection.m_stkNamespaceCursor.pop_back();
		}

	private:
		CDataCollector& m_collection;
		bool m_entered;
	};
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
	class CSSSSDFASFD;
	using CSharedSSSSDFASFD = Niflect::TSharedPtr<CSSSSDFASFD>;
	class CSSSSDFASFD
	{
	public:
		CTypeDecl m_typeDecl;
		Niflect::TArrayNif<CXCursor> m_vecDetail;
		Niflect::TArrayNif<CSharedSSSSDFASFD> m_vecChild;
	};
	static void TakeNamespaceRefs(const CXType& cxType, const CXCursor& cursor, bool checking, const Niflect::TArrayNif<CXCursor>& vecDetail, int32& idxFront, CSSSSDFASFD* parentSetting)
	{
		if (vecDetail.size() == idxFront)
			return;
		//if (checking && ((cxType.kind != CXType_Unexposed) && (clang_getCursorKind(cursor) == CXCursor_NoDeclFound)))
		//	return;
		if (checking && ((clang_getCursorKind(cursor) == CXCursor_NoDeclFound)))
			return;
		auto kind = clang_getCursorKind(vecDetail[idxFront]);
		while (kind == CXCursor_NamespaceRef)
		{
			parentSetting->m_vecDetail.push_back(vecDetail[idxFront]);
			idxFront++;
			kind = clang_getCursorKind(vecDetail[idxFront]);
		}
		parentSetting->m_vecDetail.push_back(vecDetail[idxFront++]);
	}
	static void wwwwwwwwwwwwwRecurs(const CUntaggedTypesMapping& untaggedTypesMapping, const CXType& cxType, const Niflect::TArrayNif<CXCursor>& vecDetail, int32& idxFront, bool ssssssssssss, CSSSSDFASFD* parentSetting)
	{
		parentSetting->m_typeDecl.m_CXType = cxType;
		parentSetting->m_typeDecl.m_cursorDecl = clang_getTypeDeclaration(cxType);

		auto& cursor = parentSetting->m_typeDecl.m_cursorDecl;

		if (true)
		{
			//һ�������scope, ��ΪǶ�׵�����scope, ����Ҫ��Ϊ�ݹ��߼�, �ɼ��ǳ�����, ���ƻ�֧��, ��Ӧʵ����Ӧ������ʾ, �����������޷���ȷ����, ��������ʾ
			auto parentCursor = clang_getCursorSemanticParent(cursor);
			auto parentKind = clang_getCursorKind(parentCursor);
			if (parentKind == CXCursor_ClassDecl)
			{
				TakeNamespaceRefs(clang_getCursorType(parentCursor), parentCursor, false, vecDetail, idxFront, parentSetting);
				auto t = clang_getCursorType(parentCursor);
				auto argsCount = clang_Type_getNumTemplateArguments(t);
				for (int idx = 0; idx < argsCount; ++idx)
				{
					auto scopeCXType = clang_Type_getTemplateArgumentAsType(t, idx);
					auto scopeCursor = clang_getTypeDeclaration(scopeCXType);
					TakeNamespaceRefs(scopeCXType, scopeCursor, true, vecDetail, idxFront, parentSetting);
				}
			}
		}

		TakeNamespaceRefs(cxType, cursor, false, vecDetail, idxFront, parentSetting);

		auto argsCount = clang_Type_getNumTemplateArguments(cxType);
		for (int idx = 0; idx < argsCount; ++idx)
		{
			auto childSetting = Niflect::MakeShared<CSSSSDFASFD>();
			auto childCXType = clang_Type_getTemplateArgumentAsType(cxType, idx);
			wwwwwwwwwwwwwRecurs(untaggedTypesMapping, childCXType, vecDetail, idxFront, ssssssssssss, childSetting.Get());
			parentSetting->m_vecChild.push_back(childSetting);
		}
	}
	//static void wwwwwwwwwwwwwRecurs22222(const CUntaggedTypesMapping& untaggedTypesMapping, const CXType& cxType, const Niflect::TArrayNif<CXCursor>& vecDetail, int32& idxFront, bool ssssssssssss, CSSSSDFASFD* parentSetting)
	//{
	//	parentSetting->m_typeDecl.m_CXType = cxType;
	//	parentSetting->m_typeDecl.m_cursorDecl = clang_getTypeDeclaration(cxType);

	//	auto argsCount = clang_Type_getNumTemplateArguments(cxType);
	//	for (int idx = 0; idx < argsCount; ++idx)
	//	{
	//		auto childSetting = Niflect::MakeShared<CSSSSDFASFD>();
	//		auto childCXType = clang_Type_getTemplateArgumentAsType(cxType, idx);
	//		wwwwwwwwwwwwwRecurs22222(untaggedTypesMapping, childCXType, vecDetail, idxFront, ssssssssssss, childSetting.Get());
	//		parentSetting->m_vecChild.push_back(childSetting);
	//	}
	//}
	static void PrintMySettingCursorSpellingRecurs(CSSSSDFASFD* parentSetting, uint32 lv = 0)
	{
		auto strLv = NiflectUtil::DebugIndentToString(lv);
		auto kind = clang_getCursorKind(parentSetting->m_typeDecl.m_cursorDecl);
		Niflect::CString a;
		if (kind != CXCursor_NoDeclFound)
			a = CXStringToCString(clang_getCursorSpelling(parentSetting->m_typeDecl.m_cursorDecl));
		else
			a = CXStringToCString(clang_getTypeSpelling(parentSetting->m_typeDecl.m_CXType));
		Niflect::CString detail;
		for (uint32 idx = 0; idx < parentSetting->m_vecDetail.size(); ++idx)
		{
			auto asdf = NiflectUtil::FormatString("[%s, %s]", CXStringToCString(clang_getCursorSpelling(parentSetting->m_vecDetail[idx])).c_str(), CXStringToCString(clang_getCursorKindSpelling(clang_getCursorKind(parentSetting->m_vecDetail[idx]))).c_str());
			detail += asdf;
			if (idx != parentSetting->m_vecDetail.size() - 1)
				detail += ", ";
		}
		auto strKind = CXStringToCString(clang_getCursorKindSpelling(kind));
		printf("%s%s, %s (%s)\n", strLv.c_str(), a.c_str(), strKind.c_str(), detail.c_str());
		lv++;
		for (auto& it : parentSetting->m_vecChild)
		{
			PrintMySettingCursorSpellingRecurs(it.Get(), lv);
		}
	}
	static void PrintMySettingTypeSpellingRecurs(CSSSSDFASFD* parentSetting, uint32 lv = 0)
	{
		auto strLv = NiflectUtil::DebugIndentToString(lv);
		auto a = CXStringToCString(clang_getTypeSpelling(parentSetting->m_typeDecl.m_CXType));
		auto kind = clang_getCursorKind(parentSetting->m_typeDecl.m_cursorDecl);
		auto strKind = CXStringToCString(clang_getCursorKindSpelling(kind));
		printf("%s%s, %s\n", strLv.c_str(), a.c_str(), strKind.c_str());
		lv++;
		for (auto& it : parentSetting->m_vecChild)
		{
			PrintMySettingTypeSpellingRecurs(it.Get(), lv);
		}
	}
	static void PrintSubcursorTypeNameRecurs(const CSubcursor& parentSubcursor, uint32 lv = 0)
	{
		auto strLv = NiflectUtil::DebugIndentToString(lv);
		auto a = CXStringToCString(clang_getTypeSpelling(parentSubcursor.m_CXType));
		auto kind = clang_getCursorKind(parentSubcursor.m_cursorDecl);
		auto strKind = CXStringToCString(clang_getCursorKindSpelling(kind));
		printf("%s%s\n", strLv.c_str(), a.c_str());
		lv++;
		for (auto& it : parentSubcursor.m_vecChild)
		{
			PrintSubcursorTypeNameRecurs(it, lv);
		}
	}
	static void BuildSubcursorRecurs2(const CXType& cxType, const Niflect::TArrayNif<CXCursor>& vecDetail, int32& idxFront, CSubcursor& parentSubcursor)
	{
		parentSubcursor.m_CXType = cxType;
		parentSubcursor.m_cursorDecl = clang_getTypeDeclaration(cxType);

		auto argsCount = clang_Type_getNumTemplateArguments(cxType);
		if (argsCount > 0)
		{
			parentSubcursor.m_vecChild.resize(argsCount);
			for (int idx = 0; idx < argsCount; ++idx)
			{
				auto childCXType = clang_Type_getTemplateArgumentAsType(cxType, idx);
				auto& childSubcursor = parentSubcursor.m_vecChild[idx];
				BuildSubcursorRecurs2(childCXType, vecDetail, idxFront, childSubcursor);
			}
		}
	}
	static void BuildSubcursor2(const CXCursor& cursorSettingRoot, const Niflect::TArrayNif<CXCursor>& vecDetail, CSubcursor& parentSubcursor)
	{
		auto cxType = clang_getCursorType(cursorSettingRoot);
		int32 idxFront = 0;
		BuildSubcursorRecurs2(cxType, vecDetail, idxFront, parentSubcursor);
	}
#else
#endif

	class CScopeBindingSetting
	{
	public:
		CScopeBindingSetting(CDataCollector& collection, const CXCursor& cursor, const CCursorArray& vecChild, const CCollectingContext& context, SRecursCollectingData& recursCollectiingData)
			: m_collection(collection)
			, m_cursor(cursor)
			, m_vecChild(vecChild)
			, m_context(context)
			, m_recursCollectiingData(recursCollectiingData)
			, m_entered(false)
			, m_tagType(EAccessorSettingTagType::Count)
		{
#ifdef ACCESSOR_SETTING_ABCD
			for (uint32 idx = 0; idx < GetAccessorSettingTypesCount(); ++idx)
			{
				if (FindTagByKindAndDisplayName(m_cursor, CXCursor_TypeAliasDecl, AccessorSetingTagNames[idx]))
				{
					m_tagType = static_cast<EAccessorSettingTagType>(idx);
					m_entered = true;
					break;
				}
			}
#else
			if (m_collection.GetNamespaceDepth() <= 1)//TypeBinding��֧�ֶ�����ȫ�ֻ�1�����namespace��
			{
				m_entered = FindTagByKindAndDisplayName(m_cursor, CXCursor_TypeAliasDecl, NiflectGenDefinition::CodeTag::BindingSetting);
				
				//2024.09.23, TAccessorBinding���������ܴ�, ��Ϊ��˳��֧����������﷨�ı���, ��˲����Ʊ������׸�AccessorBinding֮ǰ������������ͻ����
				//if (m_entered)
				//	m_collection.m_collectingClassBaseCursorDecl = false;//�߼�Ϊ������BindingSetting����ͷ�ļ��е�cursor֮ǰһ���Ѿ�����������Accessor����
			}
#endif
		}
		~CScopeBindingSetting()
		{
			if (m_entered)
			{
				MyLogErrorChecked(m_vecChild.size() > 0, NiflectUtil::FormatString("Template must have child nodes"));
				MyLogErrorChecked(clang_getCursorKind(m_vecChild[0]) == CXCursor_TemplateRef, NiflectUtil::FormatString("The first node must be TemplateRef"));


#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
				m_recursCollectiingData.m_vecAccessorBindingSetting.push_back(CBindingSettingData());
				auto& data = m_recursCollectiingData.m_vecAccessorBindingSetting.back();
				BuildSubcursor2(m_cursor, m_vecChild, data.m_subcursorRoot);
#else
				CXType templateType = clang_getCursorType(m_cursor);
				CBindingSettingData data;
				if (!BuildSubcursor(templateType, m_vecChild, data.m_subcursorRoot))
				{
					Niflect::CString str0;
					GenerateTemplateInstanceCode(data.m_subcursorRoot, str0);
					GenLogError(m_context.m_log, NiflectUtil::FormatString("Fail to apply the BindingSetting, %s", str0.c_str()));
					return;
				}
				//auto a = CXStringToCString(clang_getTypeSpelling(templateType));
				////CollectTemplateSubcursorRecurs2(templateType, m_vecChild, data.m_subCursorRoot, cursorArrayFrontIndex);
				//auto& b = m_vecChild[5];
				//auto c = clang_isReference(b.kind);
				//auto d = clang_getCursorReferenced(b);

				//PrintSubcursorRecurs(data.m_subcursorRoot);
				//Niflect::CString str0;
				//GenerateTemplateInstanceCode(data.m_subcursorRoot.m_vecChild[0], str0);
				//Niflect::CString str1;
				//GenerateTemplateInstanceCode(data.m_subcursorRoot.m_vecChild[1], str1);

	#ifdef ACCESSOR_SETTING_ABCD
				switch (m_tagType)
				{
				case EAccessorSettingTagType::A:
					m_recursCollectiingData.m_settings.m_vecAccessorBindingSetting.push_back(data);
					break;
				case EAccessorSettingTagType::B:
				{
					auto& s = m_recursCollectiingData.m_settings.m_settingCompound;
					ASSERT(!s.IsValid());//todo: ��⵽��ͻ����, ����
					s = data;
					break;
				}
				case EAccessorSettingTagType::C:
				{
					auto& s = m_recursCollectiingData.m_settings.m_settingEnumClass;
					ASSERT(!s.IsValid());//todo: ��⵽��ͻ����, ����
					s = data;
					break;
				}
				case EAccessorSettingTagType::D:
				{
					auto& s = m_recursCollectiingData.m_settings.m_settingEnumBitMask;
					ASSERT(!s.IsValid());//todo: ��⵽��ͻ����, ����
					s = data;
					break;
				}
				case EAccessorSettingTagType::E:
					data.m_accessorSettingResolvedInfo.m_isPointerTemplate = true;
					m_recursCollectiingData.m_settings.m_vecAccessorBindingSetting.push_back(data);
					break;
				default:
					ASSERT(false);
					break;
				}
	#else
			m_recursCollectiingData.m_settings.m_vecAccessorBindingSetting.push_back(data);
	#endif
#endif
			}
		}

	private:
		CDataCollector& m_collection;
		const CXCursor& m_cursor;
		const CCursorArray& m_vecChild;
		const CCollectingContext& m_context;
		SRecursCollectingData& m_recursCollectiingData;
		bool m_entered;
		EAccessorSettingTagType m_tagType;
	};

	static bool IsTypeForBaseClassVerifying(CXCursorKind kind)
	{
		return kind == CXCursor_ClassDecl || kind == CXCursor_ClassTemplate;
	}

	class CScopeAccessorBaseCursorDecl
	{
	public:
		CScopeAccessorBaseCursorDecl(CDataCollector& collection, const CXCursor& cursor, const CCursorArray& vecChild)
			: m_collection(collection)
			, m_cursor(cursor)
			, m_vecChild(vecChild)
			, m_entered(false)
		{
			m_entered = IsTypeForBaseClassVerifying(clang_getCursorKind(cursor));
		}
		~CScopeAccessorBaseCursorDecl()
		{
			if (m_entered)
			{
				CXCursor foundCursor = g_invalidCursor;
				for (uint32 idx = 0; idx < m_vecChild.size(); ++idx)
				{
					//�ƺ��ɱ�����ȡ��̳ж���, ����̳е�֧�ֲ��ڹ滮��
					if (clang_getCursorKind(m_vecChild[idx]) == CXCursor_CXXBaseSpecifier)
					{
						foundCursor = m_vecChild[idx];
						break;
					}
				}
				bool canAdd = !clang_Cursor_isNull(foundCursor);
				if (!canAdd)
					canAdd = IsCAccessorClassDecl(m_cursor);//��second���ڱ�ʾ���ӵĻ���, Ϊ�����¼̫���޹�cursor, ��˼��ֻ�����������cursor
				if (canAdd)
				{
					auto ret = m_collection.m_mapCursorDeclToBaseCursorDecl.insert({ m_cursor, foundCursor });
					ASSERT(ret.second);
				}
			}
		}

	private:
		CDataCollector& m_collection;
		const CXCursor& m_cursor;
		const CCursorArray& m_vecChild;
		bool m_entered;
	};

	class CScopeTemplateArgsCount
	{
	public:
		CScopeTemplateArgsCount(const CXCursor& cursor, const CCursorArray& vecChild)
			: m_cursor(cursor)
			, m_vecChild(vecChild)
			, m_argsCount(0)
			, m_entered(false)
		{
			auto kind = clang_getCursorKind(cursor);
			m_entered = kind == CXCursor_TypeAliasTemplateDecl || kind == CXCursor_ClassTemplate;
		}
		~CScopeTemplateArgsCount()
		{
			if (m_entered)
			{
				for (uint32 idx = 0; idx < m_vecChild.size(); ++idx)
				{
					auto& cursor = m_vecChild[idx];
					auto childKind = clang_getCursorKind(cursor);
					if (childKind == CXCursor_TemplateTypeParameter)
						m_argsCount++;
					else
						break;
				}

				//auto a = CXStringToCString(clang_getCursorSpelling(m_cursor));
				//if (a.find("vector") != std::string::npos)
				//	printf("%s, %u\n", a.c_str(), m_argsCount);
			}
		}

	private:
		const CXCursor& m_cursor;
		const CCursorArray& m_vecChild;
		uint32 m_argsCount;
		bool m_entered;
	};

	//todo: Ӧ��Ϊ ScopeAliasChainLinking ?
	class CScopeTemplateDecl
	{
	public:
		CScopeTemplateDecl(const CXCursor& cursor, const CCursorArray& vecChild, const CXCursor& parentCursor, CAliasChain* aliasChain)
			: m_cursor(cursor)
			, m_vecChild(vecChild)
			, m_parentCursor(parentCursor)
			, m_aliasChain(aliasChain)
			, m_entered(false)
		{
			m_entered = clang_getCursorKind(cursor) == CXCursor_TypeAliasDecl;
		}
		~CScopeTemplateDecl()
		{
			if (m_entered)
			{
				for (uint32 idx = 0; idx < m_vecChild.size(); ++idx)
				{
					auto& cursor = m_vecChild[idx];

					//auto a = CXStringToCString(clang_getCursorSpelling(m_parentCursor));
					//auto b = CXStringToCString(clang_getCursorSpelling(m_cursor));
					//printf("%s, %s\n", a.c_str(), b.c_str());

					//if (b == "MyString")
					//	printf("");

					auto childKind = clang_getCursorKind(cursor);
					if (childKind == CXCursor_TypeRef || childKind == CXCursor_TemplateRef)
					{
						auto dsssssssss = m_cursor;
						bool ok = false;
						if (clang_getCursorKind(m_parentCursor) == CXCursor_TypeAliasTemplateDecl)
						{
							ok = true;
							dsssssssss = m_parentCursor;
						}
						else if (clang_getCursorKind(m_cursor) == CXCursor_TypeAliasDecl)
						{
							ok = true;
						}
						if (ok)
						{
							m_aliasChain->LinkToReferenced(dsssssssss, cursor, 0);
						}
						break;
					}
				}
			}
		}

	private:
		const CXCursor& m_cursor;
		const CXCursor& m_parentCursor;
		const CCursorArray& m_vecChild;
		CAliasChain* m_aliasChain;
		bool m_entered;
	};

	CVisitingDebugData::CVisitingDebugData()
		: m_allCursorsCountForChecking(0)
		, m_fp(NULL)
		, m_level(0)
		, m_allCursorsCount(0)
	{

	}
	void CVisitingDebugData::Init(const CXCursor& cursor, FILE* fp)
	{
		m_allCursorsCountForChecking = 0;
		m_fp = fp;
		m_level = 0;
		m_allCursorsCount = 0;

		CVisitCallbacks callbacks;
		callbacks.m_EnterFunc = [this](const CXCursor& cursor)
			{
				m_allCursorsCountForChecking++;
			};
		VisitCursorRecurs(cursor, callbacks);
	}
	bool CVisitingDebugData::Check() const
	{
		if (m_allCursorsCount != m_allCursorsCountForChecking)
		{
			ASSERT(false);
			return false;
		}
		return true;
	}

	CTaggedTypeCollector::CTaggedTypeCollector(CMacroTagCollection2& tagCollection)
		: m_tagCollection(tagCollection)
		, m_stage(EStage::None)
		, m_tagLocation{}
	{

	}
	bool CTaggedTypeCollector::Collect(const CXCursor& cursor, CTaggedNode2* taggedParent, CGenLog* log)
	{
		bool addedTaggedChidl = false;
		if (m_stage == EStage::None)
		{
			if (FindTagByKindAndDisplayName(cursor, CXCursor_TypedefDecl, NiflectGenDefinition::CodeTag::Type))
			{
				m_tagLocation = clang_getCursorLocation(cursor);
				m_stage = EStage::Found;
			}
		}
		else if (m_stage == EStage::Found)
		{
			TSharedPtr<CTaggedType> taggedChild;
			auto kind = clang_getCursorKind(cursor);
			if (kind == CXCursor_ClassDecl)
				taggedChild = MakeShared<CTaggedClass>();
			else if (kind == CXCursor_EnumDecl)
				taggedChild = MakeShared<CTaggedEnum>();
			else if (kind == CXCursor_StructDecl)
				taggedChild = MakeShared<CTaggedStruct>();
			if (taggedChild != NULL)
			{
				CXCursor macroCursor;
#ifdef SIMPLIFIED_MACRO_CURSOR_FINDING
				m_tagCollection.PopMacroExpansion(macroCursor);
#else
				m_tagCollection.TakeByTagLocation(m_tagLocation, macroCursor);
#endif
				taggedParent->AddChildAndInitDefault(taggedChild, cursor, macroCursor);
				m_stage = EStage::None;
				addedTaggedChidl = true;
			}
			else
			{
				auto spelling = CXStringToCString(clang_getCursorSpelling(cursor));
				GenLogError(log, NiflectUtil::FormatString("The type of %s must be either a class, a struct, or an enum", spelling.c_str()));
			}
		}
		return addedTaggedChidl;
	}

	CTemplateCollector::CTemplateCollector()
	{

	}
	bool CTemplateCollector::Collect(const CXCursor& cursor, CTaggedNode2* taggedParent, CGenLog* log)
	{
		//����clang_visitChildren��������ȱ���, ����AccessorBinding����ͷ�ļ�ʱ, �ȼ��������õ�ͷ�ļ��ݹ�, ��˲�����ǰȷ����Ҫ��¼ʵ���õ���UntaggedTemplate, ���ܿ��Ʊ�����ʽΪ�������, �����ǰȷ��ʵ��ʹ�õķ�1άBindingType��UntaggedTemplate
		taggedParent->AddChildAndInitDefault(MakeShared<CUntaggedTemplate>(), cursor, g_invalidCursor);
		return true;
	}

#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
	CUntaggedTypeCollector::CUntaggedTypeCollector()
	{

	}
	bool CUntaggedTypeCollector::Collect(const CXCursor& cursor, CTaggedNode2* taggedParent, CGenLog* log)
	{
		taggedParent->AddChildAndInitDefault(MakeShared<CUntaggedType>(), cursor, g_invalidCursor);
		return true;
	}
#else
#endif

	CDataCollector::CDataCollector()
		: m_taggedTypeCollector(m_macroTagCollection)
		, m_collectingClassBaseCursorDecl(true)
	{
	}
	uint32 CDataCollector::GetNamespaceDepth() const
	{
		return static_cast<uint32>(m_stkNamespaceCursor.size());
	}
	const CXCursor& CDataCollector::GetVisitingNamespaceReserved() const
	{
		static CXCursor s_invalidCursor{};
		if (m_stkNamespaceCursor.size() > 0)
			return m_stkNamespaceCursor.back();
		return s_invalidCursor;
	}
	struct SVisitorCallbackData
	{
		CDataCollector* m_this;
		CTaggedNode2* m_taggedParent;
		CCollectingContext& m_context;
		CAliasChain* m_aliasChain;
		CCursorArray m_vecCursorChild;
		SVisitingData m_visitingData;
	};
	void CDataCollector::Visit(const CXCursor& cursor, CTaggedNode2* taggedParent, CCollectingContext& context, CAliasChain* aliasChain, SVisitingData& data)
	{
		uint32 taggedChildIndex = taggedParent->GetChildrenCount();
		bool addedTaggedChild = false;
#ifdef SIMPLIFIED_MACRO_CURSOR_FINDING
		auto kind = clang_getCursorKind(cursor);
		if (!m_macroTagCollection.PushMacroExpansion(kind, cursor))
		{
			if (m_collectingClassBaseCursorDecl)
			{

				//if (IsTypeForBaseClassVerifying(kind))
				//{
				//	auto ret = m_mapCursorDeclToBaseCursorDecl.insert({ cursor, g_invalidCursor });
				//	ASSERT(ret.second);
				//}

	#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
				bool isUntagged = false;
				switch (kind)
				{
				case CXCursor_TypeAliasTemplateDecl:
				case CXCursor_TypeAliasDecl:
				case CXCursor_ClassTemplate:

					isUntagged = true;
					break;
				default: break;
				};
				if (isUntagged)
				{
					aliasChain->AddLinkDecl(cursor);
					addedTaggedChild = m_untaggedTypeCollector.Collect(cursor, taggedParent, context.m_log);
				}
	#else
				if (kind == CXCursor_TypeAliasTemplateDecl)
				{
					aliasChain->AddLinkDecl(cursor);
					addedTaggedChild = m_templateCollector.Collect(cursor, taggedParent, context.m_log);
				}
				else if (kind == CXCursor_TypeAliasDecl)
				{
					aliasChain->AddLinkDecl(cursor);
				}
				else if (kind == CXCursor_ClassTemplate)
				{
					aliasChain->AddLinkDecl(cursor);
					addedTaggedChild = m_templateCollector.Collect(cursor, taggedParent, context.m_log);
				}
	#endif
			}
			if (!addedTaggedChild)
			{
				addedTaggedChild = m_taggedTypeCollector.Collect(cursor, taggedParent, context.m_log);
				if (!addedTaggedChild)
				{
					STaggedNodeCollectingContext taggedNodeContext{ m_macroTagCollection, context.m_log };
					addedTaggedChild = taggedParent->CollectSibling(cursor, taggedNodeContext);
				}
			}
		}
#else
		if (!m_macroTagCollection.CollectMacroExpansion(cursor))
		{
			if (m_collectingClassBaseCursorDecl)
			{
				auto kind = clang_getCursorKind(cursor);

				//if (IsTypeForBaseClassVerifying(kind))
				//{
				//	auto ret = m_mapCursorDeclToBaseCursorDecl.insert({ cursor, g_invalidCursor });
				//	ASSERT(ret.second);
				//}

	#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
				bool isUntagged = false;
				switch (kind)
				{
				case CXCursor_TypeAliasTemplateDecl: 
				case CXCursor_TypeAliasDecl:
				case CXCursor_ClassTemplate:

					isUntagged = true;
					break;
				default: break;
				};
				if (isUntagged)
				{
					aliasChain->AddLinkDecl(cursor);
					addedTaggedChild = m_untaggedTypeCollector.Collect(cursor, taggedParent, context.m_log);
				}
	#else
				if (kind == CXCursor_TypeAliasTemplateDecl)
				{
					aliasChain->AddLinkDecl(cursor);
					addedTaggedChild = m_templateCollector.Collect(cursor, taggedParent, context.m_log);
				}
				else if (kind == CXCursor_TypeAliasDecl)
				{
					aliasChain->AddLinkDecl(cursor);
				}
				else if (kind == CXCursor_ClassTemplate)
				{
					aliasChain->AddLinkDecl(cursor);
					addedTaggedChild = m_templateCollector.Collect(cursor, taggedParent, context.m_log);
				}
	#endif
			}
			if (!addedTaggedChild)
			{
				addedTaggedChild = m_taggedTypeCollector.Collect(cursor, taggedParent, context.m_log);
				if (!addedTaggedChild)
				{
					STaggedNodeCollectingContext taggedNodeContext{ m_macroTagCollection, context.m_log };
					addedTaggedChild = taggedParent->CollectSibling(cursor, taggedNodeContext);
				}
			}
		}
#endif
		if (addedTaggedChild)
			data.m_vecTaggedChildIndex.push_back(taggedChildIndex);
		else
			data.m_vecTaggedChildIndex.push_back(INDEX_NONE);
	}
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
	void CDataCollector::CollectUntaggedTypesRecurs(CTaggedNode2* taggedParent, CUntaggedTypesMapping& mapping) const
	{
		if (auto untaggedType = CUntaggedType::CastChecked(taggedParent))
		{
			auto& cursor = untaggedType->GetCursor();
			auto ret = mapping.m_mapCursorToIndex.insert({ cursor, static_cast<uint32>(mapping.m_vecType.size()) });
			ASSERT(ret.second);
			mapping.m_vecType.push_back(untaggedType);
		}

		for (auto& it0 : taggedParent->DebugGetChildren())
		{
			this->CollectUntaggedTypesRecurs(it0.Get(), mapping);
		}
	}
#else
#endif
	CXChildVisitResult VisitorCallback(CXCursor cursor, CXCursor parent, CXClientData data)
	{
		auto& d = *static_cast<SVisitorCallbackData*>(data);
		d.m_this->Visit(cursor, d.m_taggedParent, d.m_context, d.m_aliasChain, d.m_visitingData);
		d.m_vecCursorChild.push_back(cursor);

		return CXChildVisit_Continue;
	}
	bool CDataCollector::VerifyDerivedFromCAccessor(CXCursor p, CAliasChain* aliasChain) const
	{
		//todo: �ɿ����ռ������ͽṹ��, ����ʹ�ýӿ���ʱ��ȡ
		bool ok = false;
		while (true)
		{
			if (IsCAccessorClassDecl(p))
			{
				ok = true;
				break;
			}
			aliasChain->FindOriginalDecl(p, p);
			auto itFound = m_mapCursorDeclToBaseCursorDecl.find(p);
			if (itFound != m_mapCursorDeclToBaseCursorDecl.end())
			{
				if (!clang_Cursor_isNull(itFound->second))
				{
					p = clang_getTypeDeclaration(clang_getCursorType(itFound->second));
				}
				else
				{
					if (IsCAccessorClassDecl(p))
						ok = true;
					break;//secondΪ����cursor, Ϊ�����������Ҫ�ټ���, ���break
				}
			}
			else
			{
				break;
			}
		}
		return ok;
	}
	CXCursor CDataCollector::FindAliasDeclOld(CXCursor decl) const
	{
		ASSERT(false);
		return g_invalidCursor;
		//return m_aliasChain->FindOriginalDecl(decl);
	}
	static bool ContainsTemplateTypeParameterRecurs(const CSubcursor& par)
	{
		bool yes = false;
		//if (par.m_vecAaaaaaaaaa.size() == 0)
		//{
		//	yes = (clang_getCursorKind(par.m_cursorDecl) == CXCursor_NoDeclFound) &&
		//		(par.m_CXType.kind == CXType_Unexposed);
		//}
		//else
		{
			for (auto& it : par.m_vecAaaaaaaaaa)
			{
				auto kind = clang_getCursorKind(it);
				if (kind == CXCursor_TypeRef)
				{
					auto cursor = clang_getCursorReferenced(it);
					if (clang_getCursorKind(cursor) == CXCursor_TemplateTypeParameter)
					{
						yes = true;
						break;
					}
				}
			}
		}
		if (!yes)
		{
			for (auto& it : par.m_vecChild)
			{
				bool yesForChild = ContainsTemplateTypeParameterRecurs(it);
				if (yesForChild)
				{
					yes = true;
					break;
				}
			}
		}
		return yes;
	}
	void DebugPrintTaggedNodeRecurs3(CTaggedNode2* taggedParent, int level)
	{
		taggedParent->DebugPrint(NULL, level);

		level++;
		for (auto& it0 : taggedParent->DebugGetChildren())
		{
			DebugPrintTaggedNodeRecurs3(it0.Get(), level);
		}
	}
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
	static Niflect::CString BuildBindingTypeName(const CTypeDecl& typeDecl)
	{
		auto kind = clang_getCursorKind(typeDecl.m_cursorDecl);
		Niflect::CString a;
		if (kind != CXCursor_NoDeclFound)
			a = CXStringToCString(clang_getCursorSpelling(typeDecl.m_cursorDecl));
		else
			a = CXStringToCString(clang_getTypeSpelling(typeDecl.m_CXType));
		return a;
	}
	static void BuildSubcursorTypeNameRecurs(const CSubcursor& parentSubcursor, Niflect::CString& str)
	{
		str += CXStringToCString(clang_getTypeSpelling(parentSubcursor.m_CXType));
		if (parentSubcursor.m_vecChild.size() > 0)
		{
			if (IsCursorTemplateDecl(parentSubcursor.m_cursorDecl))
			{
				NiflectGenDefinition::CodeStyle::TemplateAngleBracketL(str);
				Niflect::CString childStr;
				for (uint32 idx = 0; idx < parentSubcursor.m_vecChild.size(); ++idx)
				{
					auto& it = parentSubcursor.m_vecChild[idx];
					BuildSubcursorTypeNameRecurs(it, childStr);
					if (idx != parentSubcursor.m_vecChild.size() - 1)
						childStr += ", ";
				}
				str += childStr;
				NiflectGenDefinition::CodeStyle::TemplateAngleBracketR(str);
			}
		}
	}
	static Niflect::CString BuildSubcursorTypeName(const CSubcursor& parentSubcursor)
	{
		Niflect::CString str;
		BuildSubcursorTypeNameRecurs(parentSubcursor, str);
		return str;
	}
#else
#endif
	void CDataCollector::Collect(const CXCursor& cursor, CTaggedNode2* taggedParent, CCollectingContext& context, CCollectionData& collectionData)
	{
		auto aliasChain = Niflect::MakeShared<CAliasChain>();
		auto accessorBindingMapping = Niflect::MakeShared<CAccessorBindingMapping2>();
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
		auto untaggedTypesMapping = Niflect::MakeShared<CUntaggedTypesMapping>();
#else
#endif
		auto& accessorSettings= accessorBindingMapping->m_settings;
		SRecursCollectingData recursCollectiingData{ aliasChain.Get(), accessorSettings };
		this->CollectDataRecurs2(cursor, g_invalidCursor, taggedParent, context, recursCollectiingData);

		if (accessorSettings.m_vecAccessorBindingSetting.size() == 0)
		{
			GenLogError(context.m_log, "There is no valid accessor setting");
			return;
		}

#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
		this->CollectUntaggedTypesRecurs(taggedParent, *untaggedTypesMapping);

		//{
		//	TCXTypeMap<uint32> mapCXTypeToIndex;
		//	TCursorMap<uint32> mapCursorToIndex;
		//	TCursorMap<uint32> mapCanoCursorToIndex;
		//	for (uint32 idx0 = 0; idx0 < vecSetting.size(); ++idx0)
		//	{
		//		auto& it0 = vecSetting[idx0];
		//		auto& bSubcursor = it0.GetBindingTypeDecl();
		//		
		//		auto ret0 = mapCXTypeToIndex.insert({ bSubcursor.m_CXType, idx0 });
		//		if (!ret0.second)
		//		{
		//			printf("");
		//		}
		//		auto ret1 = mapCursorToIndex.insert({ bSubcursor.m_cursorDecl, idx0 });
		//		if (!ret1.second)
		//		{
		//			printf("");
		//		}
		//		auto canoCursor = clang_getCanonicalCursor(bSubcursor.m_cursorDecl);
		//		auto ret2 = mapCanoCursorToIndex.insert({ canoCursor, idx0 });
		//		if (!ret2.second)
		//		{
		//			printf("");
		//		}
		//	}
		//	printf("");
		//}
#endif
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
		auto ReportForDup = [](CCollectingContext& context, const CAccessorBindingMapping2& mapping, uint32 idxErrSrc, uint32 idxDupWith)
			{
				auto& settingErrSrc = mapping.m_vecAccessorBindingSetting[idxErrSrc];
				auto& settingDup = mapping.m_vecAccessorBindingSetting[idxDupWith];
				Niflect::CString str0 = BuildSubcursorTypeName(settingErrSrc.GetBindingTypeDecl());
				Niflect::CString str1 = BuildSubcursorTypeName(settingDup.GetBindingTypeDecl());
				GenLogError(context.m_log, NiflectUtil::FormatString("Duplicated accessor binding of %s with %s. Additionally, partial template specialization is not supported for binding types.", str0.c_str(), str1.c_str()));
			};

		{
			bool ok = true;
			TCursorMap<uint32> mapCursorToIndex;
			for (uint32 idx0 = 0; idx0 < vecSetting.size(); ++idx0)
			{
				auto& it0 = vecSetting[idx0];
				auto& bSubcursor = it0.GetBindingTypeDecl();

				auto a = CXStringToCString(clang_getCursorSpelling(bSubcursor.m_cursorDecl));

				auto cursor = bSubcursor.m_cursorDecl;
				bool isDup = false;
				auto itInitialInsertionPos = mapCursorToIndex.end();
				uint32 idxDupWith = INDEX_NONE;
				bool initialFound = false;
				while (true)
				{
					auto itFound = mapCursorToIndex.find(cursor);
					if (itFound != mapCursorToIndex.end())
					{
						isDup = true;
						idxDupWith = itFound->second;
						break;
					}
					if (!initialFound)
					{
						itInitialInsertionPos = itFound;
						initialFound = true;
					}

					auto kind = clang_getCursorKind(cursor);
					if (kind == CXCursor_TypeAliasDecl)//��ģ��ı���
					{
						auto itFound = untaggedTypesMapping->m_mapCursorToIndex.find(cursor);
						if (itFound != untaggedTypesMapping->m_mapCursorToIndex.end())
						{
							auto& ut = untaggedTypesMapping->m_vecType[itFound->second];
							auto t = clang_getTypedefDeclUnderlyingType(ut->GetCursor());
							ASSERT(t.kind != CXType_Invalid);
							cursor = clang_getTypeDeclaration(t);
						}
						else
						{
							break;
						}
					}
					else if (kind == CXCursor_TypeAliasTemplateDecl)
					{
						//����֤��ģ������߿�����, ������ģ�����, �޷�ʵ�����ֲ����ػ������
						auto itFound = untaggedTypesMapping->m_mapCursorToIndex.find(cursor);
						if (itFound != untaggedTypesMapping->m_mapCursorToIndex.end())
						{
							auto& ut = untaggedTypesMapping->m_vecType[itFound->second];
							for (auto& it1 : ut->DebugGetChildren())
							{
								auto k = clang_getCursorKind(it1->GetCursor());
								if (k == CXCursor_TypeAliasDecl)
								{
									cursor = it1->GetCursor();
									break;
								}
							}
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
				if (!isDup)
				{
					mapCursorToIndex.insert(itInitialInsertionPos, { bSubcursor.m_cursorDecl, idx0 });
				}
				else
				{
					ok = false;
					ReportForDup(context, *accessorBindingMapping, idx0, idxDupWith);
					if (!context.m_log->m_opt.m_cachedItems)
						break;
				}
			}
			if (!ok)
				return;
		}




		{
			//bool ok = true;
			//TCursorMap<uint32> mapCursorToIndex;
			//for (uint32 idx0 = 0; idx0 < vecSetting.size(); ++idx0)
			//{
			//	auto& it0 = vecSetting[idx0];
			//	auto& bSubcursor = it0.GetBindingTypeDecl();

			//	auto ret = mapCursorToIndex.insert({ bSubcursor.m_cursorDecl, idx0 });
			//	if (!ret.second)
			//	{
			//		ok = false;
			//		auto& idxDupWith = ret.first->second;
			//		ReportForDup(context, *accessorBindingMapping, idx0, idxDupWith);
			//		if (!context.m_log->m_opt.m_cachedItems)
			//			break;
			//	}
			//}
			//if (!ok)
			//	return;




			//for (uint32 idx0 = 0; idx0 < vecSetting.size(); ++idx0)
			//{
			//	auto& it0 = vecSetting[idx0];
			//	auto& bSubcursor = it0.GetBindingTypeDecl();

			//	auto a = CXStringToCString(clang_getTypeSpelling(bSubcursor.m_CXType));
			//	printf("%s\n", a.c_str());
			//}
			//return;



			//for (uint32 idx0 = 0; idx0 < vecSetting.size(); ++idx0)
			//{
			//	auto& it0 = vecSetting[idx0];
			//	auto& bSubcursor = it0.GetBindingTypeDecl();

			//	auto cxType = bSubcursor.m_CXType;
			//	auto kind = clang_getCursorKind(bSubcursor.m_cursorDecl);
			//	if (kind == CXCursor_TypeAliasDecl)
			//	{
			//		auto itFound = untaggedTypesMapping->m_mapCursorToIndex.find(bSubcursor.m_cursorDecl);
			//		auto& ut = untaggedTypesMapping->m_vecType[itFound->second];
			//		//cxType = clang_getCursorType(ut->GetCursor());

			//		cxType = clang_getTypedefDeclUnderlyingType(ut->GetCursor());
			//		auto a = clang_getTypeDeclaration(cxType);
			//		printf("");

			//		//for (auto& it1 : ut->DebugGetChildren())
			//		//{
			//		//	auto k = clang_getCursorKind(it1->GetCursor());
			//		//	if (k == CXCursor_TemplateRef || k == CXCursor_TypeRef)
			//		//	{
			//		//		auto c = clang_getCursorReferenced(it1->GetCursor());
			//		//		cxType = clang_getCursorType(c);
			//		//		break;
			//		//	}
			//		//}
			//	}

			//	auto a = CXStringToCString(clang_getTypeSpelling(cxType));
			//	printf("%s\n", a.c_str());
			//}
			//return;



			//bool ok = true;
			//TCXTypeMap<uint32> mapCXTypeToIndex;
			//for (uint32 idx0 = 0; idx0 < vecSetting.size(); ++idx0)
			//{
			//	auto& it0 = vecSetting[idx0];
			//	auto& bSubcursor = it0.GetBindingTypeDecl();

			//	auto ret = mapCXTypeToIndex.insert({ bSubcursor.m_CXType, idx0 });
			//	if (!ret.second)
			//	{
			//		ok = false;
			//		auto& idxDupWith = ret.first->second;
			//		ReportForDup(context, *accessorBindingMapping, idx0, idxDupWith);
			//		if (!context.m_log->m_opt.m_cachedItems)
			//			break;
			//	}
			//}
			//if (!ok)
			//	return;
		}
#else
#endif

		//#1, ���AccessorType�����Ƿ�̳���CAccessor
		for (uint32 idx0 = 0; idx0 < accessorSettings.m_vecAccessorBindingSetting.size(); ++idx0)
		{
			auto& it0 = accessorSettings.m_vecAccessorBindingSetting[idx0];
			auto& aSubcursor = it0.GetAccessorTypeDecl();
			bool ok = this->VerifyDerivedFromCAccessor(aSubcursor.m_cursorDecl, aliasChain.Get());
			//if (!found)
			//{
			//	auto templateDecl = clang_getSpecializedCursorTemplate(foundDecl);//Ԥ��֧���ػ�ģ����Accessor, ����Ҫʵ���ػ�ģ������ָ�����
			//	found = this->CheckDeclDerivedFromFieldBaseClass(templateDecl);
			//}
			if (!ok)
			{
				Niflect::CString str;
				GenerateTemplateInstanceCode(it0.m_subcursorRoot, str);
				GenLogError(context.m_log, NiflectUtil::FormatString("Accessor type must be derived from %s, %s", NiflectGenDefinition::NiflectFramework::AccessorTypeName::Field, str.c_str()));
				break;
			}
		}

#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
#else
		auto ReportForDup = [](CCollectingContext& context, const CAccessorBindingMapping2& mapping, uint32 idxErrSrc, uint32 idxDupWith)
			{
				auto& settingErrSrc = mapping.m_settings.m_vecAccessorBindingSetting[idxErrSrc];
				auto& settingDup = mapping.m_settings.m_vecAccessorBindingSetting[idxDupWith];
				Niflect::CString str0;
				GenerateTemplateInstanceCode(settingErrSrc.m_subcursorRoot, str0);
				Niflect::CString str1;
				GenerateTemplateInstanceCode(settingDup.m_subcursorRoot, str1);
				GenLogError(context.m_log, NiflectUtil::FormatString("Duplicated accessor binding of %s with %s. Additionally, partial template specialization is not supported for binding types.", str0.c_str(), str1.c_str()));
			};

#endif

//#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
//		{
//			bool ok = true;
//			TCursorMap<uint32> mapBindingTypeDeclToIndex;
//			TCXTypeMap<uint32> mapBindingTypeCXTypeToIndex;
//			for (uint32 idx0 = 0; idx0 < vecSetting.size(); ++idx0)
//			{
//				auto& it0 = vecSetting[idx0];
//				auto& bSubcursor = it0.GetBindingTypeDecl();
//				auto originalKind = clang_getCursorKind(bSubcursor.m_cursorDecl);
//				uint32 idxDupWith = INDEX_NONE;
//				bool ok = true;
//				bool useCXType = false;
//				CXType cxType;
//				if (originalKind != CXCursor_NoDeclFound)
//				{
//					auto decl = bSubcursor.m_cursorDecl;
//					aliasChain->FindOriginalDecl(bSubcursor.m_cursorDecl, decl, true);
//					cxType = clang_getTypedefDeclUnderlyingType(decl);
//					if (cxType.kind != CXType_Invalid)
//					{
//						useCXType = true;
//					}
//					else
//					{
//						auto ret = mapBindingTypeDeclToIndex.insert({ decl, idx0 });
//						if (!ret.second)
//						{
//							idxDupWith = ret.first->second;
//							if (IsCursorTemplateDecl(bSubcursor.m_cursorDecl))
//							{
//								auto itFound = untaggedTemplatesMapping->m_mapCursorToIndex.find(decl);
//								ASSERT(itFound != untaggedTemplatesMapping->m_mapCursorToIndex.end());
//
//								//������ TArrayNif �� std::vector, TStdPairAlias �� std::pair �ķ���Ϊ���ԭʼģ�嶨��Ĳ��������Ƿ���ͬ
//								//����ζ���޷����ֲַ��ػ��ı���ģ��, �� TArrayNif �� TArrayNifAlias
//								//����Ҫʵ�ִ�����, �ɿ��Ǳ��� aliasChain ÿ��ʱ���ػ���ʽ, ���漰�ǳ������ĵݹ����
//								auto bindingTypeArgsCount = clang_Type_getNumTemplateArguments(bSubcursor.m_CXType);
//								auto& untaggedItem = untaggedTemplatesMapping->m_vecType[itFound->second];
//								if (bindingTypeArgsCount == untaggedItem->m_argsCount)
//								{
//									ok = false;
//								}
//							}
//							else
//							{
//								ok = false;
//							}
//						}
//					}
//				}
//				else
//				{
//					cxType = bSubcursor.m_CXType;
//					useCXType = true;
//				}
//				if (useCXType)
//				{
//					auto ret = mapBindingTypeCXTypeToIndex.insert({ cxType, idx0 });
//					if (!ret.second)
//					{
//						idxDupWith = ret.first->second;
//						ok = false;
//					}
//				}
//				if (!ok)
//				{
//					ReportForDup(context, *accessorBindingMapping, idx0, idxDupWith);
//					if (!context.m_log->m_opt.m_cachedItems)
//						break;
//				}
//			}
//		}
//#else
//#endif

		//#2, ���BindingType�Ƿ��ض���, ����BindingType�Ĳ��ұ�
		auto& mapCursorToIndex = accessorBindingMapping->m_mapCursorToIndex;
		auto& mapSpecializedCursorToIndex = accessorBindingMapping->m_mapSpecializedCursorToIndex;//�ػ�������������Ϊ����FieldFindingʱ�Ĳ��ҹ�ģ
		auto& mapCXTypeToIndex = accessorBindingMapping->m_mapCXTypeToIndex;
		for (uint32 idx0 = 0; idx0 < accessorSettings.m_vecAccessorBindingSetting.size(); ++idx0)
		{
			auto& it0 = accessorSettings.m_vecAccessorBindingSetting[idx0];
			bool ok = true;
			auto& bSubcursor = it0.GetBindingTypeDecl();//�˴�b����boolǰ׺
			uint32 idxDupWith = INDEX_NONE;

			if (clang_getCursorKind(bSubcursor.m_cursorDecl) == CXCursor_NoDeclFound)
			{
				auto ret = mapCXTypeToIndex.insert({ bSubcursor.m_CXType, idx0 });
				ok = ret.second;
				idxDupWith = ret.first->second;
			}
			else
			{
				//��֧�ֶ�άBindingType�Է�1άcursor����, ԭ��
				//1. ��άcursor���ɵĸ���key�Ը��ӻ�
				//2. ��ν��ά, ��һ�ֱ������⴦���ά�ṹ�Ķ��巽ʽ, �����û�ָ����2ά������Ϊstd::pair��Accessor, �ɱ������std::map�Ķ���, ��std::pair������allocator��ģ�������ָ��
				//	���1ά���BindingType, �ڸ���������1ά��BindingTypeһ���
				auto p = &mapCursorToIndex;
				//if (clang_Type_getNumTemplateArguments(bSubcursor.m_CXType) > 0)
				//{
				//	if (!ContainsTemplateTypeParameterRecurs(bSubcursor))
				//		p = &mapSpecializedCursorToIndex;
				//}
				auto kind = clang_getCursorKind(bSubcursor.m_cursorDecl);
				if ((kind != CXCursor_TypeAliasDecl) && (kind != CXCursor_TypedefDecl))
				{
					if (!ContainsTemplateTypeParameterRecurs(bSubcursor))
						p = &mapSpecializedCursorToIndex;
				}
				auto& map = *p;
				auto ret = map.insert({ bSubcursor.m_cursorDecl, idx0 });
				ok = ret.second;
				idxDupWith = ret.first->second;
			}
			if (!ok)
			{
				ReportForDup(context, *accessorBindingMapping, idx0, idxDupWith);
				if (!context.m_log->m_opt.m_cachedItems)
					break;
			}
		}

		if (false)
		{
			for (uint32 idx0 = 0; idx0 < accessorSettings.m_vecAccessorBindingSetting.size(); ++idx0)
			{
				auto& it0 = accessorSettings.m_vecAccessorBindingSetting[idx0];
				for (uint32 idx1 = 0; idx1 < it0.GetBindingTypeDeclsCount(); ++idx1)
				{
					auto& bindingTypeDecl = it0.GetBindingTypeDecl(idx1);
					if (bindingTypeDecl.m_CXType.kind == CXType_Pointer)
					{
						GenLogError(context.m_log, "Pointer is not supported");//todo: ֧������ָ��������ʵ����;, Ӧ֧���ض����͵�ָ��, ��Ҫ��ȡ����Ϣ�缸άָ����ԭʼ����, �ƻ��ӵ���m_mapUserTypePointer1D��, ����ָ�����Ϊר�ŵ�����, ����ר�ŵ�ָ����ҪRuntime�ڴ����
						break;
					}
				}
			}
		}

		accessorBindingMapping->InitPatterns();

		collectionData.m_aliasChain = aliasChain;
		collectionData.m_accessorBindingMapping = accessorBindingMapping;

		//for (auto& it : collectionData.m_vecBindingSetting)
		//{
		//	auto& typeDecl = it.GetBindingTypeDecl();
		//	if (clang_getCursorKind(typeDecl.m_cursorDecl) != CXCursor_NoDeclFound)
		//	{
		//		auto decl = this->FindAliasDecl(typeDecl.m_cursorDecl);
		//		auto ret = collectionData.m_mapAliasTemplateDeclToClassTemplateCursor.insert({ typeDecl.m_cursorDecl, decl });
		//		if (!ret.second)
		//		{
		//			GenLogError(context.m_log, NiflectUtil::FormatString("Duplicated binding type specified for %s. Additionally, partial template specialization is not supported for binding types.", CXStringToCString(clang_getCursorSpelling(typeDecl.m_cursorDecl)).c_str()));
		//			break;//todo: ����2��for, ��Ҫ��������߼�����д����򲻱���sibling
		//		}
		//	}
		//}

		//for (auto& it : collectionData.m_vecBindingSetting)
		//{
		//	//auto a = it.GetBindingTypeDecl().GetTypeName();
		//	//printf("%s\n", a.c_str());

		//	auto& accessorCursorDecl = it.GetAccessorTypeDecl().m_cursorDecl;
		//	//auto itFound = m_mapCursorDeclToBaseCursorDecl.find(accessorCursorDecl);
		//	//ASSERT(itFound != m_mapCursorDeclToBaseCursorDecl.end());
		//	//it.m_accessorBaseCursorDecl = clang_getTypeDeclaration(clang_getCursorType(itFound->second));

		//	//auto p = it.m_accessorBaseCursorDecl;
		//	//bool isDerivedFromField = false;
		//	//while (true)
		//	//{
		//	//	auto name = CXStringToCString(clang_getCursorSpelling(p));
		//	//	if (name == NiflectGenDefinition::NiflectFramework::AccessorTypeName::Field)
		//	//	{
		//	//		isDerivedFromField = true;
		//	//		break;
		//	//	}
		//	//	auto itFound = m_mapCursorDeclToBaseCursorDecl.find(p);
		//	//	if (itFound != m_mapCursorDeclToBaseCursorDecl.end())
		//	//		p = clang_getTypeDeclaration(clang_getCursorType(itFound->second));
		//	//	else
		//	//		break;
		//	//}
		//	//if (!isDerivedFromField)
		//	//{
		//	//	GenLogError(context.m_log, NiflectUtil::FormatString("Accessor type must be derived from %s", NiflectGenDefinition::NiflectFramework::AccessorTypeName::Field));
		//	//	break;
		//	//}

		//	auto decl = this->FindAliasDecl(accessorCursorDecl);
		//	bool found = this->CheckDeclDerivedFromFieldBaseClass(decl);
		//	if (!found)
		//	{
		//		auto templateDecl = clang_getSpecializedCursorTemplate(decl);//Ԥ��֧���ػ�ģ����Accessor, ����Ҫʵ���ػ�ģ������ָ�����
		//		found = this->CheckDeclDerivedFromFieldBaseClass(templateDecl);
		//	}
		//	if (!found)
		//	{
		//		GenLogError(context.m_log, NiflectUtil::FormatString("Field type must be derived from %s", NiflectGenDefinition::NiflectFramework::AccessorTypeName::Field));
		//		break;
		//	}

		//	it.m_actualFieldDeclCursor = decl;

		//	ASSERT(!it.m_accessorData.m_isNotATemplate);
		//	it.m_accessorData.m_isNotATemplate = !IsCursorTemplateDecl(it.GetBindingTypeDecl().m_cursorDecl);

		//	for (uint32 idx1 = 0; idx1 < it.GetDimensionalBindingTypeDeclsCount(); ++idx1)
		//	{
		//		auto& bindingTypeDecl = it.GetDimensionalBindingTypeDecl(idx1);
		//		if (bindingTypeDecl.m_CXType.kind == CXType_Pointer)
		//		{
		//			GenLogError(context.m_log, "Pointer is not supported");//todo: ֧������ָ��������ʵ����;, Ӧ֧���ض����͵�ָ��, ��Ҫ��ȡ����Ϣ�缸άָ����ԭʼ����, �ƻ��ӵ���m_mapUserTypePointer1D��, ����ָ�����Ϊר�ŵ�����, ����ר�ŵ�ָ����ҪRuntime�ڴ����
		//			break;
		//		}
		//	}
		//}
	}
	void CDataCollector::CollectDataRecurs2(const CXCursor& cursor, const CXCursor& parentCursor, CTaggedNode2* taggedParent, CCollectingContext& context, SRecursCollectingData& recursCollectiingData)
	{
		if (auto debugData = context.m_debugData)
		{
			debugData->m_allCursorsCount++;
			DebugPrintCursor(debugData->m_fp, cursor, debugData->m_level);
		}

		SVisitorCallbackData visitorCbData{this, taggedParent, context, recursCollectiingData.m_aliasChain };
		clang_visitChildren(cursor, &VisitorCallback, &visitorCbData);

		CScopeBindingSetting scopeBindingSetting(*this, cursor, visitorCbData.m_vecCursorChild, context, recursCollectiingData);

		TSharedPtr<CScopeAccessorBaseCursorDecl> scopeCollectingClassBaseCursorDecl;
		TSharedPtr<CScopeTemplateDecl> scopeCollectingAliasTemplateAndClassTemplateDecl;
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
		TSharedPtr<CScopeTemplateArgsCount> scopeTemplateArgsCount;
#else
#endif
		if (m_collectingClassBaseCursorDecl)
		{
			scopeCollectingClassBaseCursorDecl = MakeShared<CScopeAccessorBaseCursorDecl>(*this, cursor, visitorCbData.m_vecCursorChild);
			scopeCollectingAliasTemplateAndClassTemplateDecl = MakeShared<CScopeTemplateDecl>(cursor, visitorCbData.m_vecCursorChild, parentCursor, recursCollectiingData.m_aliasChain);
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
			scopeTemplateArgsCount = MakeShared<CScopeTemplateArgsCount>(cursor, visitorCbData.m_vecCursorChild);
#else
#endif
		}

		if (auto debugData = context.m_debugData)
			debugData->m_level++;

		for (uint32 idx = 0; idx < visitorCbData.m_vecCursorChild.size(); ++idx)
		{
			auto taggedChild = taggedParent;
			auto& taggedChildIndex = visitorCbData.m_visitingData.m_vecTaggedChildIndex[idx];
			if (taggedChildIndex != INDEX_NONE)
				taggedChild = taggedParent->GetChild(taggedChildIndex);
			this->CollectDataRecurs2(visitorCbData.m_vecCursorChild[idx], cursor, taggedChild, context, recursCollectiingData);
		}

		if (auto debugData = context.m_debugData)
			debugData->m_level--;
	}
	void CDataCollector::DebugFinish2(CTaggedNode2* taggedParent, const CCollectionData& collectionData) const
	{
		//if (true)
		//{
		//	for (auto& it : collectionData.m_vecBindingSetting)
		//	{
		//		PrintSubcursorRecurs(it.m_subcursorRoot);
		//		//ASSERT(it.m_subCursorRoot.m_vecChild.size() == 2);
		//		printf("---------\n");
		//	}
		//}
		//DebugPrintTaggedNodeRecurs2(taggedParent, 0);
		DebugPrintTaggedNodeRecurs3(taggedParent, 0);
	}
}