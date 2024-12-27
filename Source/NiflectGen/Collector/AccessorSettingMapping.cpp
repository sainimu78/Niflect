#include "NiflectGen/Collector/AccessorSettingMapping.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/TaggedType/TaggedType.h"
#include "NiflectGen/Util/CursorUtil.h"

namespace NiflectGen
{
	bool CAccessorBindingMapping2::IterateForTemplate(const SResonodesInitContext& context, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, CResolvedCursorNode& resultIndexedParent, uint32& detailIteratingIdx) const
	{
		//auto argsCount = clang_Type_getNumTemplateArguments(fieldOrArgCXType);
		//for (int32 idx1 = 0; idx1 < argsCount; ++idx1)
		//{
		//	CXType argType = clang_Type_getTemplateArgumentAsType(fieldOrArgCXType, idx1);
		//	CBindingAccessorIndexedNode indexedChild;
		//	this->FindBindingTypeRecurs(argType, vecDetailCursor, resolvedTaggedTypeMapping, indexedChild, detailIteratingIdx);
		//	resultIndexedParent.m_vecChild.push_back(indexedChild);
		//}

		auto argsCount = clang_Type_getNumTemplateArguments(fieldOrArgCXType);
		bool isTemplateFormat = argsCount > 0;
		for (int32 idx1 = 0; idx1 < argsCount; ++idx1)
		{
			CXType argType = clang_Type_getTemplateArgumentAsType(fieldOrArgCXType, idx1);
			CResolvedCursorNode* indexedNext = NULL;
			if (argsCount > 1)
			{
				resultIndexedParent.m_vecChild.push_back(CResolvedCursorNode());
				indexedNext = &resultIndexedParent.m_vecChild.back();
			}
			else
			{
				//����������ʽ�����Ƿ�Ϊ�ṹģ��, ���ṹģ���ģ�����ȷʵΪ1��ʱ, ���̴���
				//����Ϊ�б�Ҫ֧��1��ģ������Ľṹģ��, Ӧʹ��һ�����ַ�ʽ, ��ͨ���� UntaggedType ���в����Ƿ�����Ӧģ����, ��ģ�����������ͬ, ����϶�Ϊ�ṹģ��
				//1ά�ṹģ����
				//AccessorBindingSetting
				//template <typename T0>
				//NIF_BS() TBindingSetting<CCompoundAccessor, TestAccessor2::TMyTransform<T0> >;
				//��Ա
				//NIF_F()
				//TestAccessor2::TMyTransform<bool> m_tm_18;
				//��ͨ�����·�ʽ����
				//auto c = clang_getTypeDeclaration(fieldOrArgCXType);
				//auto d = clang_getSpecializedCursorTemplate(c);
				//auto itFound = untaggedTemplateMapping.m_mapCursorToIndex.find(d);
				//if (itFound != untaggedTemplateMapping.m_mapCursorToIndex.end())
				//{
				//	//��һ�����ģ�������Ӧ�ĳ�Ա�����Ƿ���ģ�������ͬ, ����Ϊ��ȷָ���Ľṹģ��, ����Ϊ���������ģ��
				//	printf("");
				//}
				//����ͨ��ĳЩ�ṹ�ص��϶�, ���޷���֤��ȷ��������ģ���ṹģ��, ��Ϊ�����;���޷�ͨ���ṹ�����ƶϵ�
				//��˽��鿼�����ƿ�ܹ���
				//1. ��BindingType��1άģ��, ��һ�����϶�Ϊ����ģ��
				//2. ��BindingType�Ƕ�άģ��, ��һ�������ά��BindingType�϶�Ϊ�ṹģ��, ����Ϊ����ģ��
				//��� Main.cpp �� 12, 13 �� 14 ˵��

				resultIndexedParent.m_elem = Niflect::MakeShared<CResolvedCursorNode>();
				indexedNext = resultIndexedParent.m_elem.Get();
			}
			this->FindBindingTypeRecurs(context, resultIndexedParent, argType, vecDetailCursor, taggedMapping, untaggedTemplateMapping, *indexedNext, detailIteratingIdx);
		}
		return isTemplateFormat;
	}
	static const CXCursor* FindRefCursorInDetailCursors(const CAccessorBindingFindingContext& ctx)
	{
		const CXCursor* refCursor = NULL;
		while (ctx.m_outDetailIteratingIdx < ctx.m_vecDetailCursor.size())
		{
			auto& it = ctx.m_vecDetailCursor[ctx.m_outDetailIteratingIdx];
			ctx.m_outDetailIteratingIdx++;
			auto kind = clang_getCursorKind(it);
			//�����׸��ɴ�������͵�Ref
			if (kind == CXCursor_TemplateRef || kind == CXCursor_TypeRef)
			{
				refCursor = &it;
				break;
			}
		}
		return refCursor;
	}
	bool CAccessorBindingMapping2::InitResocursorNodeIfFound(CAccessorBindingFindingContext& ctx, CResolvedCursorNode& resocursorNode) const
	{
		uint32 foundAccessorBindingIdx = INDEX_NONE;
		uint32 foundUntaggedTemplateIndex = INDEX_NONE;
		auto& continuing = ctx.m_continuing;

		Niflect::CString header;
		{
			auto itFound = m_mapCXTypeToIndex.find(ctx.m_fieldOrArgCXType);
			if (itFound != m_mapCXTypeToIndex.end())
			{
				foundAccessorBindingIdx = itFound->second;
				if (const CXCursor* refCursor = FindRefCursorInDetailCursors(ctx))
				{
					//It's a non-builtin pointer type
					auto cursor = clang_getCursorReferenced(*refCursor);
					header = GetCursorFilePath(cursor);
					ASSERT(!header.empty());
				}
			}
		}
		if (foundAccessorBindingIdx == INDEX_NONE)
		{
			//�ػ�, <Niflect::TArrayNif<bool>, ��ֱ��ͨ��field����CXType��cursor���ҵ�BindingType��cursor
			auto cursor = clang_getTypeDeclaration(ctx.m_fieldOrArgCXType);
			auto itFound = m_mapSpecializedCursorToIndex.find(cursor);
			if (itFound != m_mapSpecializedCursorToIndex.end())
			{
				foundAccessorBindingIdx = itFound->second;
				header = GetCursorFilePath(cursor);
				continuing = false;
			}
		}
		if (foundAccessorBindingIdx == INDEX_NONE)
		{
			if (const CXCursor* refCursor = FindRefCursorInDetailCursors(ctx))
			{
				auto cursor = clang_getCursorReferenced(*refCursor);
				auto itFound = m_mapCursorToIndex.find(cursor);
				if (itFound != m_mapCursorToIndex.end())
				{
					foundAccessorBindingIdx = itFound->second;
					header = GetCursorFilePath(cursor);
					bool isTemplate = IsCursorTemplateDecl(cursor);
					if (isTemplate)
					{
						auto itFound = ctx.m_untaggedTemplateMapping.m_mapCursorToIndex.find(cursor);
						ASSERT(itFound != ctx.m_untaggedTemplateMapping.m_mapCursorToIndex.end());
						foundUntaggedTemplateIndex = itFound->second;
					}
					continuing = isTemplate;
				}
			}
		}
		if (foundAccessorBindingIdx != INDEX_NONE)
		{
			resocursorNode.InitForAccessorBinding(foundAccessorBindingIdx, foundUntaggedTemplateIndex, header);
			return true;
		}
		return false;
	}
	void CAccessorBindingMapping2::FindBindingTypeRecurs(const SResonodesInitContext& context, const CResolvedCursorNode& parentResonode, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, CResolvedCursorNode& resultIndexedParent, uint32& detailIteratingIdx) const
	{
		CAccessorBindingFindingContext result(fieldOrArgCXType, vecDetailCursor, untaggedTemplateMapping, detailIteratingIdx);
		if (this->InitResocursorNodeIfFound(result, resultIndexedParent))
		{
			auto& bindingSetting = m_settings.m_vecAccessorBindingSetting[resultIndexedParent.m_accessorBindingIndex];
			resultIndexedParent.InitForTemplateBegin(bindingSetting.m_bindingResocursorName);

			if (result.m_continuing)
			{
				auto elemDeclsCount = bindingSetting.GetELementBindingTypeDeclsCount();
				if (elemDeclsCount > 0)
				{
					auto pIndexedParent = &resultIndexedParent;
					//1ά����ģ���׶�����ṹģ��� BindingType, ����͵� TStlMapAccessor ����, 1ά����ģ��Ϊ TMap, ������ṹģ�� sd::pair, ��� ContainerTemplate1D �� StructuralTemplateND �ĸ���˵��
					Niflect::TArrayNif<CResolvedCursorNode*> vecElemResocursorNode;
					for (uint32 idx0 = 0; idx0 < elemDeclsCount; ++idx0)
					{
						auto& elemSubcursor = bindingSetting.GetELementBindingTypeDecl(idx0);
						pIndexedParent->m_elem = Niflect::MakeShared<CResolvedCursorNode>();
						pIndexedParent = pIndexedParent->m_elem.Get();
						uint32 aaaa = 0;
						CAccessorBindingFindingContext result2222(elemSubcursor.m_CXType, elemSubcursor.m_vecAaaaaaaaaa, untaggedTemplateMapping, aaaa);
						if (!this->InitResocursorNodeIfFound(result2222, *pIndexedParent))
						{
							ASSERT(false);//todo: ����, ÿάBindingType����Ҫָ��AccessorType
							break;
						}
						vecElemResocursorNode.push_back(pIndexedParent);
					}
					bool isTemplateFormat = this->IterateForTemplate(context, fieldOrArgCXType, vecDetailCursor, taggedMapping, untaggedTemplateMapping, *pIndexedParent, detailIteratingIdx);
					resultIndexedParent.InitForTemplateArguments(*pIndexedParent, isTemplateFormat);

					for (auto& it : vecElemResocursorNode)
					{
						auto& elemSetting = m_settings.m_vecAccessorBindingSetting[it->m_accessorBindingIndex];
						it->InitForTemplate(elemSetting.m_bindingResocursorName, *pIndexedParent, isTemplateFormat);
					}
				}
				else
				{
					//ģ�����ػ��� BindingType, ��Ա������ Niflect::TArrayNif<Niflect::TArrayNif<bool> >
					bool isTemplateFormat = this->IterateForTemplate(context, fieldOrArgCXType, vecDetailCursor, taggedMapping, untaggedTemplateMapping, resultIndexedParent, detailIteratingIdx);
					resultIndexedParent.InitForTemplateArguments(resultIndexedParent, isTemplateFormat);
				}
			}
			resultIndexedParent.InitForTemplateEnd();
		}
		else
		{
			auto cursor = clang_getTypeDeclaration(fieldOrArgCXType);
			if (!taggedMapping.InitIndexedNodeForClassDecl(cursor, *this, resultIndexedParent))
			{
				if (parentResonode.m_accessorBindingIndex != INDEX_NONE)
				{
					auto& parentAs = m_settings.m_vecAccessorBindingSetting[parentResonode.m_accessorBindingIndex];
					if (parentAs.m_accessorSettingResolvedInfo.m_isPointerTemplate)
					{
						auto cxType = clang_getCursorType(cursor);
						auto resocursorName = CXStringToCString(clang_getTypeSpelling(cxType));//ͨ�� Field �� Cursor �� CXType �ɻ�ȡ������ Scope �� Spelling
						auto headerFilePath = GetCursorFilePath(cursor);
						//auto resocursorName = GenerateNamespacesAndScopesCode(cursor);
						//resocursorName += CXStringToCString(clang_getCursorSpelling(cursor));
						resultIndexedParent.InitForClassDecl(resocursorName, INDEX_NONE, INDEX_NONE, headerFilePath);
					}
					else
					{
						auto& tpl = parentAs.m_bindingResocursorName;
						auto arg = CXStringToCString(clang_getTypeSpelling(clang_getCursorType(cursor)));
						GenLogError(context.m_log, NiflectUtil::FormatString("The template arg %s of the container template %s must be tagged", arg.c_str(), tpl.c_str()));
					}
				}
			}
		}
	}
	void CAccessorBindingMapping2::InitIndexedNodeForField(const SResonodesInitContext& context, const CResolvedCursorNode& parentResonode, const CXCursor& fieldCursor, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, CResolvedCursorNode& resultIndexedParent) const
	{
		auto fieldCXType = clang_getCursorType(fieldCursor);
		uint32 detailIteratingIdx = 0;
		this->FindBindingTypeRecurs(context, parentResonode, fieldCXType, vecDetailCursor, taggedMapping, untaggedTemplateMapping, resultIndexedParent, detailIteratingIdx);
	}
	static Niflect::CString GenerateBindingTypeCursorName(const CXCursor& cursor, const CXType& type)
	{
		Niflect::CString name;
		auto kind = clang_getCursorKind(cursor);
		if ((kind != CXCursor_NoDeclFound) && (kind != CXCursor_ClassDecl))
		{
			name = GenerateNamespacesAndScopesCode(cursor);
			name += CXStringToCString(clang_getCursorSpelling(cursor));
		}
		else
		{
			name = CXStringToCString(clang_getTypeSpelling(type));
		}
		return name;
	}

	enum class EGenerateAccessorBindingCursorNameVisitingState
	{
		None,
		Stop,
	};

	Niflect::CString AttachStarsForPointerType(const CXType& pointerType)
	{
		Niflect::CString stars;
		auto originalType = pointerType;
		while (originalType.kind == CXType_Pointer)
		{
			stars += "*";
			originalType = clang_getPointeeType(originalType);
		}
		return stars;
	}
	static void GenerateResocursorNameRecurs(const CSubcursor& parentSubcursor, EGenerateAccessorBindingCursorNameVisitingState& visitingState, Niflect::CString& text)
	{
		//�ú������ʵ�� GenerateTemplateInstanceCode ��������, ��Ҫ����Ϊ
		//1. ��֧������ FullScope ������
		//2. ���� NiflectGenDefinition::CodeStyle ȷ��Ƕ��ģ���Ҽ������Ƿ�ӿո�
		//3. ����ģ�����Ͷ���, �����ػ���ǲ����ػ��� Subcursor, ���ݹ�����Ŷ�Ӧ�� CXType, ����������, �� CursorName
		//������ FindBindingTypeRecurs ��ԭ���� AccessorBinding �г��ֵ�����, ��һ������ Collect �׶�֧���ռ�������, ����� Accessor ����, ͨ�����ޱ�ǵ�
		Niflect::CString name;
		if (parentSubcursor.m_vecAaaaaaaaaa.size() > 0)
		{
			for (uint32 idx = 0; idx < parentSubcursor.m_vecAaaaaaaaaa.size(); ++idx)
			{
				auto& it = parentSubcursor.m_vecAaaaaaaaaa[idx];
				auto kind = clang_getCursorKind(it);
				{
					//�ֹ���д��Ӧ��Cursor(��Field, AccessorBinding)���ܱ�֤scope(namespace����scope)��������, ��˺���, ͨ��GenerateNamespacesAndScopesCode��������scope
					if (kind != CXCursor_NamespaceRef)
					{
						if (clang_isReference(kind))
						{
							auto decl = clang_getCursorReferenced(it);

							bool isArgName = clang_getCursorKind(decl) == CXCursor_TemplateTypeParameter;
							if (!isArgName)
							{
								auto strScope = GenerateNamespacesAndScopesCode(decl);
								name += strScope;
							}
							else
							{
								visitingState = EGenerateAccessorBindingCursorNameVisitingState::Stop;
								return;
							}
							auto spelling = CXStringToCString(clang_getCursorSpelling(decl));
							name += spelling;

							if (idx != parentSubcursor.m_vecAaaaaaaaaa.size() - 1)
								name += "::";
						}
						else
						{
							ASSERT(false);
						}
					}
				}
			}
		}
		else
		{
			auto strScope = GenerateNamespacesAndScopesCode(parentSubcursor.m_cursorDecl);
			name += strScope;
			name += GetNameFromCursorOrTypeDeclaration(parentSubcursor.m_cursorDecl, parentSubcursor.m_CXType);
		}

		text += name;
		bool canRecurs = true;
		if (parentSubcursor.m_vecAaaaaaaaaa.size() > 0)
		{
			//m_vecChildΪģ����������õ�decl, ��TypedefAliasDecl, m_vecAaaaaaaaaa.back()��Ϊģ�������Spelling����, ��ģ��ΪTypeRef, ģ��ΪTemplateRef, ���TypeRef��Ӧ�����ݹ�
			if (clang_getCursorKind(parentSubcursor.m_vecAaaaaaaaaa.back()) == CXCursor_TypeRef)
			{
				text += AttachStarsForPointerType(parentSubcursor.m_CXType);
				canRecurs = false;
			}
		}
		else
		{
			auto kind = clang_getCursorKind(parentSubcursor.m_cursorDecl);
			canRecurs = kind != CXCursor_TypeAliasDecl && kind != CXCursor_TypedefDecl;
		}
		if ((canRecurs) && (parentSubcursor.m_vecChild.size() > 0))
		{
			Niflect::CString childrenText;
			for (uint32 idx = 0; idx < parentSubcursor.m_vecChild.size(); ++idx)
			{
				Niflect::CString childText;
				GenerateResocursorNameRecurs(parentSubcursor.m_vecChild[idx], visitingState, childText);

				if (visitingState == EGenerateAccessorBindingCursorNameVisitingState::Stop)
					break;

				childrenText += childText;
				if (idx != parentSubcursor.m_vecChild.size() - 1)
					childrenText += ", ";
			}
			if (!childrenText.empty())
			{
				NiflectGenDefinition::CodeStyle::TemplateAngleBracketL(text);
				text += childrenText;
				NiflectGenDefinition::CodeStyle::TemplateAngleBracketR(text);
			}
		}
	}
	static void GenerateResocursorName(const CSubcursor& parentSubcursor, Niflect::CString& text)
	{
		auto visitingState = EGenerateAccessorBindingCursorNameVisitingState::None;
		GenerateResocursorNameRecurs(parentSubcursor, visitingState, text);
	}
	static void GenerateAccessorResocursorNodeInfo(const CSubcursor& parentSubcursor, CAccessorResocursorNodeInfo& info)
	{
		auto visitingState = EGenerateAccessorBindingCursorNameVisitingState::None;
		GenerateResocursorNameRecurs(parentSubcursor, visitingState, info.m_resocursorName);
		info.m_requiredHeaderFilePath = GetCursorFilePath(parentSubcursor.m_cursorDecl);
	}
	void CAccessorBindingMapping2::InitPatterns()
	{
		for (uint32 idx = 0; idx < m_settings.m_vecAccessorBindingSetting.size(); ++idx)
		{
			auto& it = m_settings.m_vecAccessorBindingSetting[idx];
			GenerateAccessorResocursorNodeInfo(it.GetAccessorTypeDecl(), it.m_accessorSettingResolvedInfo.m_resoInfo);
			GenerateResocursorName(it.GetBindingTypeDecl(), it.m_bindingResocursorName);
		}
		if (m_settings.m_settingCompound.IsValid())
			GenerateAccessorResocursorNodeInfo(m_settings.m_settingCompound.GetAccessorTypeDecl(), m_settings.m_settingCompound.m_accessorSettingResolvedInfo.m_resoInfo);
		if (m_settings.m_settingEnumClass.IsValid())
			GenerateAccessorResocursorNodeInfo(m_settings.m_settingEnumClass.GetAccessorTypeDecl(), m_settings.m_settingEnumClass.m_accessorSettingResolvedInfo.m_resoInfo);
		if (m_settings.m_settingEnumBitMask.IsValid())
			GenerateAccessorResocursorNodeInfo(m_settings.m_settingEnumBitMask.GetAccessorTypeDecl(), m_settings.m_settingEnumBitMask.m_accessorSettingResolvedInfo.m_resoInfo);
	}
}