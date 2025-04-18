#include "NiflectGen/Collector/AccessorSettingMapping.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/TaggedType/TaggedType.h"
#include "NiflectGen/Util/CursorUtil.h"

namespace NiflectGen
{
	bool CCollectedAccessorSettings::AreValid() const
	{
		return m_vecAccessorBindingSetting.size() > 0
			|| m_settingCompound.IsValid()
			|| m_settingEnumClass.IsValid()
			|| m_settingEnumBitMask.IsValid()
			;
	}

	bool CAccessorBindingMapping2::IterateForTemplate(const SResonodesInitContext& context, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, const CAliasChain& aliasChain, CResolvedCursorNode& resultIndexedParent, uint32& detailIteratingIdx) const
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
			if (argType.kind == CXType_LValueReference || argType.kind == CXType_RValueReference)
			{
				GenLogError(context.m_log, NiflectUtil::FormatString("Reference modifier (& or &&) is not supported for %s", CXStringToCString(clang_getTypeSpelling(fieldOrArgCXType)).c_str()));
			}
			CResolvedCursorNode* indexedNext = NULL;
			if (argsCount > 1)
			{
				resultIndexedParent.m_vecChild.push_back(CResolvedCursorNode());
				indexedNext = &resultIndexedParent.m_vecChild.back();
			}
			else
			{
				//现无其它方式区分是否为结构模板, 当结构模板的模板参数确实为1个时, 流程错误
				//如认为有必要支持1个模板参数的结构模板, 应使用一种区分方式, 如通过在 UntaggedType 表中查找是否定义相应模板类, 且模板参数数量相同, 则可认定为结构模板
				//1维结构模板如
				//AccessorBindingSetting
				//template <typename T0>
				//NIF_BS() TBindingSetting<CCompoundAccessor, TestAccessor2::TMyTransform<T0> >;
				//成员
				//NIF_F()
				//TestAccessor2::TMyTransform<bool> m_tm_18;
				//可通过如下方式查找
				//auto c = clang_getTypeDeclaration(fieldOrArgCXType);
				//auto d = clang_getSpecializedCursorTemplate(c);
				//auto itFound = untaggedTemplateMapping.m_mapCursorToIndex.find(d);
				//if (itFound != untaggedTemplateMapping.m_mapCursorToIndex.end())
				//{
				//	//进一步检查模板参数对应的成员数量是否与模板参数相同, 是则为正确指定的结构模板, 否则为错误的容器模板
				//	printf("");
				//}
				//但仅通过某些结构特点认定, 是无法保证正确区分容器模板或结构模板, 因为类的用途是无法通过结构定义推断的
				//因此建议考虑限制框架功能
				//1. 当BindingType是1维模板, 则一定被认定为容器模板
				//2. 当BindingType是多维模板, 则一定将最后维的BindingType认定为结构模板, 其余为容器模板
				//另见 Main.cpp 中 12, 13 与 14 说明

				resultIndexedParent.m_elem = Niflect::MakeShared<CResolvedCursorNode>();
				indexedNext = resultIndexedParent.m_elem.Get();
			}
			this->FindBindingTypeRecurs(context, resultIndexedParent, argType, vecDetailCursor, taggedMapping, untaggedTemplateMapping, aliasChain, *indexedNext, detailIteratingIdx);
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
			//查找首个可处理的类型的Ref
			if (kind == CXCursor_TemplateRef || kind == CXCursor_TypeRef || kind == CXCursor_TypeAliasDecl)
			{
				refCursor = &it;
				break;
			}
		}
		return refCursor;
	}
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
	static bool FindDirectAliasDeclType(const CXCursor& declCursor, const CAliasChain& aliasChain, CXType& foundCxType)
	{
		auto itFound = aliasChain.m_mapDeclToIndex.find(declCursor);
		if (itFound != aliasChain.m_mapDeclToIndex.end())
		{
			auto& aliasInfo = aliasChain.m_vecItem[itFound->second];
			if (aliasInfo.m_underlyingType.kind != CXType_Invalid)
			{
				foundCxType = aliasInfo.m_underlyingType;
				return true;
			}
		}
		return false;
	}
	static bool FindDirectAliasDeclTypeForTypedefOrTypeAlias(const CXCursor& cursor, const CAliasChain& aliasChain, CXType& foundCxType)
	{
		auto kind = clang_getCursorKind(cursor);
		if (kind == CXCursor_TypedefDecl || kind == CXCursor_TypeAliasDecl || kind == CXCursor_TypeAliasTemplateDecl)
			return FindDirectAliasDeclType(cursor, aliasChain, foundCxType);
		return false;
	}
	static void TryGetQualifiers(const CXType& type, Niflect::CString& qualifiers)
	{
		//如 const 有特定用途, 可将 qualifiers 改为 bool
		if (clang_isConstQualifiedType(type))
			qualifiers += "const";
		if (clang_isVolatileQualifiedType(type))
			qualifiers += "volatile";
	}
	static void TryGetReferenceModifierCXType(const CXType& pointerType, CXType& modifierCXType, Niflect::CString& qualifiers, Niflect::CString& modifier)
	{
		//如 & 有特定用途, 可将 modifier 改为 bool
		if (pointerType.kind == CXType_LValueReference)
		{
			modifierCXType = clang_getPointeeType(pointerType);
			modifier = "&";
		}
		else if (pointerType.kind == CXType_RValueReference)
		{
			ASSERT(false);//不应支持罕见写法
			modifierCXType = clang_getPointeeType(pointerType);
			modifier = "&&";
		}
		if (!modifier.empty())
			TryGetQualifiers(modifierCXType, qualifiers);
	}
	bool CAccessorBindingMapping2::InitResocursorNodeIfFoundRecurs(CAccessorBindingFindingContext& ctx, CResolvedCursorNode& resocursorNode) const
	{
		uint32 foundAccessorBindingIdx = INDEX_NONE;
		uint32 foundUntaggedTemplateIndex = INDEX_NONE;
		auto& continuing = ctx.m_continuing;

		Niflect::CString header;
		ctx.m_originalOrDereferencedCXTypeToFind = ctx.m_fieldOrArgCXType;

		//逻辑为先以指针查找 AS, 每维的指针都被当作独立的类型进行查找
		//这可能与标准概念不符, libclang 接口获取指针所指类型是通过 clang_getPointeeType, 这意味着可能在标准概念中指针不是独立的类型
		auto pointerCursor = GetPointerCursor(ctx.m_fieldOrArgCXType, resocursorNode.m_qualifiers);
		if (pointerCursor.IsValid())
		{
			auto itFound = m_mapPointerCursorToIndex.find(pointerCursor);
			if (itFound != m_mapPointerCursorToIndex.end())
			{
				foundAccessorBindingIdx = itFound->second;
				header = GetCursorFilePath(pointerCursor.m_cursor);
				ASSERT(!header.empty());
			}
		}
		else
		{
			//对于引用修饰则用于查找的类型为其所指类型, 这与对指针的处理是不同的
			TryGetReferenceModifierCXType(ctx.m_fieldOrArgCXType, ctx.m_originalOrDereferencedCXTypeToFind, resocursorNode.m_qualifiers, resocursorNode.m_referenceModifier);
			const auto cxTypeToFind = clang_getUnqualifiedType(ctx.m_originalOrDereferencedCXTypeToFind);//避免含 qualifier(const) 影响而查找失败
			auto itFound = m_mapCXTypeToIndex.find(cxTypeToFind);
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
			CXCursor fieldCursor = g_invalidCursor;
			if (foundAccessorBindingIdx == INDEX_NONE)
			{
				//特化, <Niflect::TArrayNif<bool>, 可直接通过field本身CXType的cursor查找到BindingType的cursor
				fieldCursor = clang_getTypeDeclaration(cxTypeToFind);
				auto itFound = m_mapSpecializedCursorToIndex.find(fieldCursor);
				if (itFound != m_mapSpecializedCursorToIndex.end())
				{
					foundAccessorBindingIdx = itFound->second;
					header = GetCursorFilePath(fieldCursor);
					continuing = false;
				}
			}
			if (foundAccessorBindingIdx == INDEX_NONE)
			{
				if (const CXCursor* refCursor = FindRefCursorInDetailCursors(ctx))
				{
					fieldCursor = clang_getCursorReferenced(*refCursor);
					auto itFound = m_mapCursorToIndex.find(fieldCursor);
					if (itFound != m_mapCursorToIndex.end())
					{
						foundAccessorBindingIdx = itFound->second;
						header = GetCursorFilePath(fieldCursor);
						bool isTemplate = IsCursorTemplateDecl(fieldCursor);
						if (isTemplate)
						{
							auto itFound = ctx.m_untaggedTemplateMapping.m_mapCursorToIndex.find(fieldCursor);
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
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
			else
			{
				ASSERT(!clang_Cursor_isNull(fieldCursor));
				CXType fieldOrArgCXType;
				if (FindDirectAliasDeclTypeForTypedefOrTypeAlias(fieldCursor, ctx.m_aliasChain, fieldOrArgCXType))
				{
					Niflect::TArray<CXCursor> vecDetailCursorSub;
					CollectSubCursorChildrenFirstLevel(fieldCursor, vecDetailCursorSub);//据观察, 别名定义仅具一层子节点, 另未实验更多层时如何处理, 因此仅处理一层子节点
					uint32 outDetailIteratingIdxSub = 0;
					CAccessorBindingFindingContext ctxSub(fieldOrArgCXType, vecDetailCursorSub, ctx.m_untaggedTemplateMapping, ctx.m_aliasChain, outDetailIteratingIdxSub);
					return this->InitResocursorNodeIfFoundRecurs(ctxSub, resocursorNode);
				}
			}
#else
#endif
		}
		if (foundAccessorBindingIdx != INDEX_NONE)
		{
			resocursorNode.InitForAccessorBinding(foundAccessorBindingIdx, foundUntaggedTemplateIndex, header);
			return true;
		}
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
		else
		{
			//指针作独立类型, 无别名
		}
#else
#endif
		return false;
	}
#else
	bool CAccessorBindingMapping2::InitResocursorNodeIfFound(CAccessorBindingFindingContext& ctx, CResolvedCursorNode& resocursorNode) const
	{
		uint32 foundAccessorBindingIdx = INDEX_NONE;
		uint32 foundUntaggedTemplateIndex = INDEX_NONE;
		auto& continuing = ctx.m_continuing;

		Niflect::CString header;
		{
			auto pointerCursor = GetPointerCursorFromPointerType(ctx.m_fieldOrArgCXType);
			if (pointerCursor.IsValid())
			{
				auto itFound = m_mapPointerCursorToIndex.find(pointerCursor);
				if (itFound != m_mapPointerCursorToIndex.end())
				{
					foundAccessorBindingIdx = itFound->second;
					header = GetCursorFilePath(pointerCursor.m_cursor);
					ASSERT(!header.empty());
				}
			}
			else
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
		}
		if (foundAccessorBindingIdx == INDEX_NONE)
		{
			//特化, <Niflect::TArrayNif<bool>, 可直接通过field本身CXType的cursor查找到BindingType的cursor
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
#endif
	void CAccessorBindingMapping2::FindBindingTypeRecurs(const SResonodesInitContext& context, const CResolvedCursorNode& parentResonode, const CXType& fieldOrArgCXType2, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, const CAliasChain& aliasChain, CResolvedCursorNode& resultIndexedParent, uint32& detailIteratingIdx) const
	{
		CAccessorBindingFindingContext result(fieldOrArgCXType2, vecDetailCursor, untaggedTemplateMapping, aliasChain, detailIteratingIdx);
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
		auto initRet = this->InitResocursorNodeIfFoundRecurs(result, resultIndexedParent);
#else
		auto initRet = this->InitResocursorNodeIfFound(result, resultIndexedParent);
#endif
		//此为实现 Method 参数类型解析时保持流程不变的写法, 并未理清解析逻辑, 例如未理清为何流程分为多种递归函数, 因此带引用的 CXType 通过 result 的成员变量作输出的之后须使用的 CXType
		const auto& originalOrDereferencedCXTypeToFind = result.m_originalOrDereferencedCXTypeToFind;

		if (initRet)
		{
			auto& bindingSetting = m_settings.m_vecAccessorBindingSetting[resultIndexedParent.m_accessorBindingIndex];
			resultIndexedParent.InitForTemplateBegin(bindingSetting.m_bindingResocursorName);

			if (result.m_continuing)
			{
				auto elemDeclsCount = bindingSetting.GetELementBindingTypeDeclsCount();
				if (elemDeclsCount > 0)
				{
					auto pIndexedParent = &resultIndexedParent;
					//1维容器模板套多参数结构模板的 BindingType, 如典型的 TStlMapAccessor 设置, 1维容器模板为 TMap, 多参数结构模板 sd::pair, 另见 ContainerTemplate1D 与 StructuralTemplateND 的概念说明
					Niflect::TArrayNif<CResolvedCursorNode*> vecElemResocursorNode;
					for (uint32 idx0 = 0; idx0 < elemDeclsCount; ++idx0)
					{
						auto& elemSubcursor = bindingSetting.GetELementBindingTypeDecl(idx0);
						pIndexedParent->m_elem = Niflect::MakeShared<CResolvedCursorNode>();
						pIndexedParent = pIndexedParent->m_elem.Get();
						uint32 aaaa = 0;
						CAccessorBindingFindingContext result2222(elemSubcursor.m_CXType, elemSubcursor.m_vecAaaaaaaaaa, untaggedTemplateMapping, aliasChain, aaaa);
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
						auto initRet = this->InitResocursorNodeIfFoundRecurs(result2222, *pIndexedParent);
#else
						auto initRet = this->InitResocursorNodeIfFound(result2222, *pIndexedParent);
#endif
						if (!initRet)
						{
							ASSERT(false);//todo: 报错, 每维BindingType都需要指定AccessorType
							break;
						}
						vecElemResocursorNode.push_back(pIndexedParent);
					}
					bool isTemplateFormat = this->IterateForTemplate(context, originalOrDereferencedCXTypeToFind, vecDetailCursor, taggedMapping, untaggedTemplateMapping, aliasChain, *pIndexedParent, detailIteratingIdx);
					resultIndexedParent.InitForTemplateArguments(*pIndexedParent, isTemplateFormat);

					for (auto& it : vecElemResocursorNode)
					{
						auto& elemSetting = m_settings.m_vecAccessorBindingSetting[it->m_accessorBindingIndex];
						it->InitForTemplate(elemSetting.m_bindingResocursorName, *pIndexedParent, isTemplateFormat);
					}
				}
				else
				{
					//模板套特化的 BindingType, 成员类型如 Niflect::TArrayNif<Niflect::TArrayNif<bool> >
					bool isTemplateFormat = this->IterateForTemplate(context, originalOrDereferencedCXTypeToFind, vecDetailCursor, taggedMapping, untaggedTemplateMapping, aliasChain, resultIndexedParent, detailIteratingIdx);
					resultIndexedParent.InitForTemplateArguments(resultIndexedParent, isTemplateFormat);
				}
			}
			resultIndexedParent.InitForTemplateEnd();
		}
		else
		{
			auto cursor = clang_getTypeDeclaration(originalOrDereferencedCXTypeToFind);
			if (!taggedMapping.InitIndexedNodeForClassDecl(cursor, *this, resultIndexedParent))
			{
				if (parentResonode.m_accessorBindingIndex != INDEX_NONE)
				{
					auto& parentAs = m_settings.m_vecAccessorBindingSetting[parentResonode.m_accessorBindingIndex];
					if (parentAs.m_accessorSettingResolvedInfo.m_isPointerTemplate)
					{
						auto cxType = clang_getCursorType(cursor);
						auto resocursorName = CXStringToCString(clang_getTypeSpelling(cxType));//通过 Field 的 Cursor 的 CXType 可获取到完整 Scope 的 Spelling
						auto headerFilePath = GetCursorFilePath(cursor);
						//auto resocursorName = GenerateNamespacesAndScopesCode(cursor);
						//resocursorName += CXStringToCString(clang_getCursorSpelling(cursor));
						resultIndexedParent.InitForClassDecl(resocursorName, INDEX_NONE, INDEX_NONE, headerFilePath);
					}
					else
					{
						auto& tpl = parentAs.m_bindingResocursorName;
						auto arg = CXStringToCString(clang_getTypeSpelling(clang_getCursorType(cursor)));
						GenLogError(context.m_log, GetCursorLogLocationInfo(cursor), NiflectUtil::FormatString("The type %s must be tagged (if it can be), as it is used as a template parameter for the container template %s", arg.c_str(), tpl.c_str()));
					}
				}
			}
		}
	}
	void CAccessorBindingMapping2::InitIndexedNodeForField(const SResonodesInitContext& context, const CResolvedCursorNode& parentResonode, const CXType& fieldCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, const CAliasChain& aliasChain, CResolvedCursorNode& resultIndexedParent) const
	{
		uint32 detailIteratingIdx = 0;
		this->FindBindingTypeRecurs(context, parentResonode, fieldCXType, vecDetailCursor, taggedMapping, untaggedTemplateMapping, aliasChain, resultIndexedParent, detailIteratingIdx);
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

	static Niflect::CString AttachStarsForPointerType(const CXType& pointerType)
	{
		Niflect::CString stars;
		auto currentType = pointerType;
		while (currentType.kind == CXType_Pointer)
		{
			stars += "*";
			currentType = clang_getPointeeType(currentType);
			ASSERT(currentType.kind != CXType_LValueReference && currentType.kind != CXType_RValueReference);//AccessorType 与 BindingType 不可以引用修饰
		}
		return stars;
	}
	CPointerCursor CAccessorBindingMapping2::GetPointerCursor(const CXType& pointerType, Niflect::CString& qualifiers)
	{
		auto currentType = pointerType;
		uint32 dim = 0;
		while (currentType.kind == CXType_Pointer)
		{
			currentType = clang_getPointeeType(currentType);
			dim++;
			ASSERT(currentType.kind != CXType_LValueReference && currentType.kind != CXType_RValueReference);//AccessorType 与 BindingType 不可以引用修饰
		}
		if (dim > 0)
			TryGetQualifiers(currentType, qualifiers);
		return CPointerCursor(pointerType, clang_getTypeDeclaration(currentType), dim);
	}
	static void GenerateResocursorNameRecurs(const CSubcursor& parentSubcursor, EGenerateAccessorBindingCursorNameVisitingState& visitingState, Niflect::CString& text)
	{
		//该函数与旧实验 GenerateTemplateInstanceCode 流程类似, 主要区别为
		//1. 仅支持生成 FullScope 的名称
		//2. 根据 NiflectGenDefinition::CodeStyle 确定嵌套模板右尖括号是否加空格
		//3. 对于模板类型定义, 即非特化或非部分特化的 Subcursor, 不递归尖括号对应的 CXType, 仅生成名称, 即 CursorName
		//不复用 FindBindingTypeRecurs 的原因是 AccessorBinding 中出现的类型, 不一定都是 Collect 阶段支持收集的类型, 如各种 Accessor 子类, 通常是无标记的
		Niflect::CString name;
		if (parentSubcursor.m_vecAaaaaaaaaa.size() > 0)
		{
			for (uint32 idx = 0; idx < parentSubcursor.m_vecAaaaaaaaaa.size(); ++idx)
			{
				auto& it = parentSubcursor.m_vecAaaaaaaaaa[idx];
				auto kind = clang_getCursorKind(it);
				{
					//手工编写对应的Cursor(如Field, AccessorBinding)不能保证scope(namespace与类scope)是完整的, 因此忽略, 通过GenerateNamespacesAndScopesCode生成完整scope
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
			//m_vecChild为模板参数所引用的decl, 如TypedefAliasDecl, m_vecAaaaaaaaaa.back()中为模板参数的Spelling类型, 非模板为TypeRef, 模板为TemplateRef, 因此TypeRef不应继续递归
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