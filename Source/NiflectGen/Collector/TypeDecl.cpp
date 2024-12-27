#include "NiflectGen/Collector/TypeDecl.h"
#include "Niflect/Util/DebugUtil.h"
#include "Niflect/Util/StringUtil.h"

namespace NiflectGen
{
	void BuildSubcursorRecursOld(const CXType& parentType, const Niflect::TArrayNif<CXCursor>& vecAAAAAAA, uint32& frontIndex, CSubcursor& parentSubcursor)
	{
		auto cursorDecl = clang_getTypeDeclaration(parentType);

		//auto a = CXStringToCString(clang_getCursorSpelling(cursorDecl));
		//auto b = CXStringToCString(clang_getTypeSpelling(parentType));

		parentSubcursor.m_cursorDecl = cursorDecl;
		parentSubcursor.m_CXType = parentType;

		if (frontIndex < vecAAAAAAA.size())
		{
			if ((clang_getCursorKind(parentSubcursor.m_cursorDecl) != CXCursor_NoDeclFound) || (parentType.kind == CXType_Unexposed))
			{
				while (clang_getCursorKind(vecAAAAAAA[frontIndex]) == CXCursor_NamespaceRef)
				{
					parentSubcursor.m_vecAaaaaaaaaa.push_back(vecAAAAAAA[frontIndex]);
					frontIndex++;
				}
				parentSubcursor.m_vecAaaaaaaaaa.push_back(vecAAAAAAA[frontIndex]);
				frontIndex++;
			}
			else if (parentType.kind == CXType_Invalid)
			{
				while (clang_getCursorKind(vecAAAAAAA[frontIndex]) == CXCursor_NamespaceRef)
				{
					parentSubcursor.m_vecAaaaaaaaaa.push_back(vecAAAAAAA[frontIndex]);
					frontIndex++;
				}
				parentSubcursor.m_vecAaaaaaaaaa.push_back(vecAAAAAAA[frontIndex]);
				frontIndex++;

				parentSubcursor.m_cursorDecl = parentSubcursor.m_vecAaaaaaaaaa.back();
				parentSubcursor.m_CXType = clang_getCursorType(parentSubcursor.m_cursorDecl);
			}
		}

		//if (parentSubcursor.m_vecAaaaaaaaaa.size() > 0)
		//{
		//	auto a = CXStringToCString(clang_getCursorSpelling(parentSubcursor.m_vecAaaaaaaaaa.back()));
		//	auto b = clang_getCursorReferenced(parentSubcursor.m_vecAaaaaaaaaa.back());
		//	auto c = clang_getCursorType(b);
		//	auto d = clang_getTypeDeclaration(c);
		//	auto e = CXStringToCString(clang_getCursorSpelling(b));
		//	auto f = CXStringToCString(clang_getCursorSpelling(d));
		//	auto g = clang_getSpecializedCursorTemplate(parentSubcursor.m_vecAaaaaaaaaa.back());
		//	auto h = CXStringToCString(clang_getTypeSpelling(c));
		//	printf("");
		//}

		auto templateArgsCount = clang_Type_getNumTemplateArguments(parentType);
		if (templateArgsCount > 0)
			parentSubcursor.m_vecChild.resize(templateArgsCount);
		for (int idx = 0; idx < templateArgsCount; ++idx)
		{
			CXType argType = clang_Type_getTemplateArgumentAsType(parentType, idx);
			BuildSubcursorRecursOld(argType, vecAAAAAAA, frontIndex, parentSubcursor.m_vecChild[idx]);
		}
	}
	void BuildSubcursorRecurs2(const CXType& parentType, uint32 maxDepth, const Niflect::TArrayNif<CXCursor>& vecAAAAAAA, uint32& frontIndex, CSubcursor& parentSubcursor, uint32 depth, bool skipAlias)
	{
		auto cursorDecl = clang_getTypeDeclaration(parentType);

		//auto a = CXStringToCString(clang_getCursorSpelling(cursorDecl));
		//auto b = CXStringToCString(clang_getTypeSpelling(parentType));

		if (maxDepth == INDEX_NONE || depth < maxDepth)
		{
			parentSubcursor.m_cursorDecl = cursorDecl;
			parentSubcursor.m_CXType = parentType;

			if (frontIndex < vecAAAAAAA.size())
			{
				bool takingDetail = false;
				if ((clang_getCursorKind(parentSubcursor.m_cursorDecl) != CXCursor_NoDeclFound) 
					|| (parentType.kind == CXType_Unexposed 
						|| parentType.kind == CXType_Pointer))
				{
					takingDetail = true;
				}
				else if (parentType.kind == CXType_Invalid)
				{
					takingDetail = true;
				}
				if (takingDetail)
				{
					while (clang_getCursorKind(vecAAAAAAA[frontIndex]) == CXCursor_NamespaceRef)
					{
						parentSubcursor.m_vecAaaaaaaaaa.push_back(vecAAAAAAA[frontIndex]);
						frontIndex++;
					}
					parentSubcursor.m_vecAaaaaaaaaa.push_back(vecAAAAAAA[frontIndex]);
					frontIndex++;

					//parentSubcursor.m_cursorDecl = parentSubcursor.m_vecAaaaaaaaaa.back();
					//parentSubcursor.m_CXType = clang_getCursorType(parentSubcursor.m_cursorDecl);
				}
			}

			//if (parentSubcursor.m_vecAaaaaaaaaa.size() > 0)
			//{
			//	auto a = CXStringToCString(clang_getCursorSpelling(parentSubcursor.m_vecAaaaaaaaaa.back()));
			//	auto b = clang_getCursorReferenced(parentSubcursor.m_vecAaaaaaaaaa.back());
			//	auto c = clang_getCursorType(b);
			//	auto d = clang_getTypeDeclaration(c);
			//	auto e = CXStringToCString(clang_getCursorSpelling(b));
			//	auto f = CXStringToCString(clang_getCursorSpelling(d));
			//	auto g = clang_getSpecializedCursorTemplate(parentSubcursor.m_vecAaaaaaaaaa.back());
			//	auto h = CXStringToCString(clang_getTypeSpelling(c));
			//	printf("");
			//}
		}

		depth++;

		auto kind = clang_getCursorKind(parentSubcursor.m_cursorDecl);
		if (!skipAlias || (kind != CXCursor_TypeAliasDecl))
		{
			auto templateArgsCount = clang_Type_getNumTemplateArguments(parentType);
			if (templateArgsCount > 0)
			{
				if (maxDepth == INDEX_NONE || depth < maxDepth)
					parentSubcursor.m_vecChild.resize(templateArgsCount);
				for (int idx = 0; idx < templateArgsCount; ++idx)
				{
					CXType argType = clang_Type_getTemplateArgumentAsType(parentType, idx);
					CSubcursor* p = &parentSubcursor;
					if (idx < parentSubcursor.m_vecChild.size())
						p = &parentSubcursor.m_vecChild[idx];
					BuildSubcursorRecurs2(argType, maxDepth, vecAAAAAAA, frontIndex, *p, depth, skipAlias);
				}
			}
#ifdef CLASS_TYPE_SCOPE
			else if (kind == CXCursor_ClassDecl)
			{
				//为支持类型作为Scope, 如TestGenMyScope::CSub_1::CSubSub_0
				while (frontIndex < vecAAAAAAA.size())
				{
					parentSubcursor.m_vecAaaaaaaaaa.push_back(vecAAAAAAA[frontIndex]);
					frontIndex++;
				}
			}
#else
#endif
		}
	}
	bool BuildSubcursor(const CXType& parentType, const Niflect::TArrayNif<CXCursor>& vecAAAAAAA, CSubcursor& parentSubcursor, bool skipAlias)
	{
		uint32 cursorArrayFrontIndex = 0;
		BuildSubcursorRecurs2(parentType, INDEX_NONE, vecAAAAAAA, cursorArrayFrontIndex, parentSubcursor, 0, skipAlias);
		return cursorArrayFrontIndex == vecAAAAAAA.size();
	}
	void PrintSubcursorRecurs(const CSubcursor& parentSubcursor, uint32 level)
	{
		auto strLevel = NiflectUtil::DebugIndentToString(level);
		auto a = GetNameFromCursorOrTypeDeclaration(parentSubcursor.m_cursorDecl, parentSubcursor.m_CXType);
		Niflect::CString b;
		for (uint32 idx = 0; idx < parentSubcursor.m_vecAaaaaaaaaa.size(); ++idx)
		{
			auto& it = parentSubcursor.m_vecAaaaaaaaaa[idx];
			b += CXStringToCString(clang_getCursorSpelling(it));
			if (idx != parentSubcursor.m_vecAaaaaaaaaa.size() - 1)
				b += "::";
		}
		printf("%s%s, %s\n", strLevel.c_str(), a.c_str(), b.c_str());
		level++;
		for (auto& it : parentSubcursor.m_vecChild)
			PrintSubcursorRecurs(it, level);
	}
	void GenerateTemplateInstanceCodeRecurs(const CSubcursor& parentSubcursor, Niflect::CString& text)
	{
		auto a = GetNameFromCursorOrTypeDeclaration(parentSubcursor.m_cursorDecl, parentSubcursor.m_CXType);
		text += a;
		if (parentSubcursor.m_vecChild.size() > 0)
		{
			text += "<";
			for (uint32 idx = 0; idx < parentSubcursor.m_vecChild.size(); ++idx)
			{
				Niflect::CString childText;
				GenerateTemplateInstanceCodeRecurs(parentSubcursor.m_vecChild[idx], childText);
				text += childText;
				if (idx != parentSubcursor.m_vecChild.size() - 1)
					text += ", ";
			}
			if (parentSubcursor.m_vecChild.back().m_vecChild.size() > 1)
				text += " ";
			text += ">";
		}
	}
	static void GenerateTemplateInstanceCodeRecurs2(const CSubcursor& parentSubcursor, Niflect::TMap<Niflect::CString, uint32>& mapArgNameToCount, Niflect::CString& text, bool& withRightAngleBracket, const CGenerateTemplateInstanceCodeOption& opt)
	{
		Niflect::CString name;
		if (parentSubcursor.m_vecAaaaaaaaaa.size() > 0)
		{
			for (uint32 idx = 0; idx < parentSubcursor.m_vecAaaaaaaaaa.size(); ++idx)
			{
				auto& it = parentSubcursor.m_vecAaaaaaaaaa[idx];
				auto kind = clang_getCursorKind(it);
				if (!opt.m_withFullScope)
				{
					if (clang_isReference(kind))
					{
						auto decl = clang_getCursorReferenced(it);
						name += CXStringToCString(clang_getCursorSpelling(decl));
						if (idx != parentSubcursor.m_vecAaaaaaaaaa.size() - 1)
							name += "::";
					}
					else
					{
						ASSERT(false);
					}
				}
				else
				{
					if (kind != CXCursor_NamespaceRef)
					{
						if (clang_isReference(kind))
						{
							auto decl = clang_getCursorReferenced(it);

							//原不区分是否为模板参数
							//auto strScope = GenerateNamespacesAndScopesCode(decl);
							//name += strScope;
							//name += CXStringToCString(clang_getCursorSpelling(decl));
							//实际使用中, 通过递归总是能够先获取到需要替换的Arg Spelling, 在简化用法的约定下, 无须通过区分cursor是否为模板参数再确定如何替换
							bool isArgName = clang_getCursorKind(decl) == CXCursor_TemplateTypeParameter;
							if (!isArgName)
							{
								auto strScope = GenerateNamespacesAndScopesCode(decl);
								name += strScope;
							}
							auto spelling = CXStringToCString(clang_getCursorSpelling(decl));
							if (isArgName && (!opt.m_templateArgsReplacementPattern.empty()))
							{
								auto& cnt = mapArgNameToCount[spelling];
								spelling = NiflectUtil::FormatString(opt.m_templateArgsReplacementPattern.c_str(), cnt);
								cnt++;
							}
							name += spelling;
							//endif

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
			if (opt.m_withFullScope)
			{
				auto strScope = GenerateNamespacesAndScopesCode(parentSubcursor.m_cursorDecl);
				name += strScope;
			}
			name += GetNameFromCursorOrTypeDeclaration(parentSubcursor.m_cursorDecl, parentSubcursor.m_CXType);
		}

		text += name;
		bool canRecurs = true;
		if (parentSubcursor.m_vecAaaaaaaaaa.size() > 0)
		{
			//m_vecChild为模板参数所引用的decl, 如TypedefAliasDecl, m_vecAaaaaaaaaa.back()中为模板参数的Spelling类型, 非模板为TypeRef, 模板为TemplateRef, 因此TypeRef不应继续递归
			if (clang_getCursorKind(parentSubcursor.m_vecAaaaaaaaaa.back()) == CXCursor_TypeRef)
			{
				canRecurs = false;
				withRightAngleBracket = false;
			}
		}
		else
		{
			canRecurs = clang_getCursorKind(parentSubcursor.m_cursorDecl) != CXCursor_TypeAliasDecl;
		}
		if ((canRecurs) && (parentSubcursor.m_vecChild.size() > 0))
		{
			bool isLastChildWithRightAngleBracket = false;
			text += "<";
			for (uint32 idx = 0; idx < parentSubcursor.m_vecChild.size(); ++idx)
			{
				Niflect::CString childText;
				if (opt.m_vecTemplateArgReplacementString != NULL && (opt.m_vecTemplateArgReplacementString->size() > 0))
				{
					ASSERT(opt.m_vecTemplateArgReplacementString->size() == parentSubcursor.m_vecChild.size());
					childText = opt.m_vecTemplateArgReplacementString->at(idx);
					if (childText.back() == '>')
						isLastChildWithRightAngleBracket = true;
				}
				else
				{
					GenerateTemplateInstanceCodeRecurs2(parentSubcursor.m_vecChild[idx], mapArgNameToCount, childText, isLastChildWithRightAngleBracket, opt);
				}
				text += childText;
				if (idx != parentSubcursor.m_vecChild.size() - 1)
					text += ", ";
			}
			if (isLastChildWithRightAngleBracket)
				text += " ";
			text += ">";
			withRightAngleBracket = true;
		}
	}
	void GenerateTemplateInstanceCode(const CSubcursor& parentSubcursor, Niflect::CString& text, const CGenerateTemplateInstanceCodeOption& opt)
	{
		bool withRightAngleBracket = false;
		Niflect::TMap<Niflect::CString, uint32> mapArgNameToCount;
		GenerateTemplateInstanceCodeRecurs2(parentSubcursor, mapArgNameToCount, text, withRightAngleBracket, opt);
	}
	Niflect::CString GenerateFullScopeTypeName(const CSubcursor& bSubcursor, const CGenerateTemplateInstanceTypeNameOption& opt)
	{
		Niflect::CString resolvedName;
		auto kind = clang_getCursorKind(bSubcursor.m_cursorDecl);
		if (!IsCursorKindTemplateDecl(kind))//非模板或暂不在范围, 如需要应确认模板参数排除生成namespace流程(可能已支持, 未测试)
		{
			if (kind == CXCursor_ClassDecl)
			{
				//特化模板
				if (clang_Type_getNumTemplateArguments(bSubcursor.m_CXType) > 0)
				{
					GenerateTemplateInstanceCode(bSubcursor, resolvedName, CGenerateTemplateInstanceCodeOption()
						.SetTemplateArgReplacementStrings(opt.m_vecTemplateArgReplacementString)
						.SetTemplateArgsReplacementPattern(opt.m_templateArgsReplacementPattern)
						.SetWithFullScope(true));
				}
				else
				{
					//类型Scope中的类型, 如 TestGenMyScope::CSub::CSubSub
					resolvedName += GenerateNamespacesAndScopesCode(bSubcursor.m_cursorDecl);
					resolvedName += CXStringToCString(clang_getCursorSpelling(bSubcursor.m_cursorDecl));
				}
			}
			else
			{
				if ((kind == CXCursor_TypeAliasDecl)//using 别名
					|| (kind == CXCursor_TypedefDecl)//typedef 别名
					)
				{
					resolvedName += GenerateNamespacesAndScopesCode(bSubcursor.m_cursorDecl);
					resolvedName += CXStringToCString(clang_getCursorSpelling(bSubcursor.m_cursorDecl));
				}
				else
				{
					//Builtin类型
					ASSERT(kind == CXCursor_NoDeclFound);
					resolvedName += CXStringToCString(clang_getTypeSpelling(bSubcursor.m_CXType));
				}
			}
		}
		else
		{
			GenerateTemplateInstanceCode(bSubcursor, resolvedName, CGenerateTemplateInstanceCodeOption()
				.SetTemplateArgReplacementStrings(opt.m_vecTemplateArgReplacementString)
				.SetTemplateArgsReplacementPattern(opt.m_templateArgsReplacementPattern)
				.SetWithFullScope(true));
		}
		return resolvedName;
	}
}