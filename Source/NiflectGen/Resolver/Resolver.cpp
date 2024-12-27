#include "NiflectGen/Resolver/Resolver.h"
#include "Niflect/Util/DebugUtil.h"
#include <algorithm>
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCodeWriter.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include <string.h>//memcpy

namespace NiflectGen
{
	using namespace Niflect;

	CResolver::CResolver(const CCollectionData& collectionData, const CModuleRegInfoValidated& moduleRegInfo)
		: m_collectionData(collectionData)
		//, m_vecHeaderSearchPath(vecHeaderSearchPath)
		, m_foundCursorsCount(0)
		, m_moduleRegInfo(moduleRegInfo)
	{
	}
	//void CResolver::Resolve(CTaggedNode2* taggedRoot, CResolvingContext& context, CResolvedData& data)
	//{
	//	this->ResolveRecurs(taggedRoot, context, data);

	//	//for (auto& it : m_collectionData.m_vecTemplateSubcursorRoot)
	//	//{
	//	//	auto accessorCursor = clang_getTypeDeclaration(it.m_vecChild[0].m_type);
	//	//	auto accessorTypeName = CXStringToCString(clang_getCursorSpelling(accessorCursor));
	//	//	CXType bindingType = it.m_vecChild[1].m_vecChild[0].m_type;
	//	//	auto bindingTypeCursor = clang_getTypeDeclaration(bindingType);
	//	//	auto bindingTypeName = CXStringToCString(clang_getCursorSpelling(bindingTypeCursor));
	//	//	//auto bindingTypeName = CXStringToCString(clang_getTypeSpelling(bindingType));
	//	//	printf("%s <-> %s\n", accessorTypeName.c_str(), bindingTypeName.c_str());
	//	//}

	//	for (auto& it0 : m_collectionData.m_vecBindingSetting)
	//	{
	//		if (!it0.IsValidBindingSetting())
	//		{
	//			GenLogError(context.m_log, "Unable to obtain type bindings from the TBindingSetting template instance");
	//			break;
	//		}
	//		auto accessorCursor = clang_getTypeDeclaration(it0.GetAccessorType());
	//		auto itFoundTaggedType = data.m_mapDeclarationToTaggedType.find(accessorCursor);
	//		if (itFoundTaggedType == data.m_mapDeclarationToTaggedType.end())
	//		{
	//			if (false)
	//			{
	//				//�ֲ�֧��ָ�������ʹ���ģ������Scope, ���¶���, m_mapDeclarationToTaggedType�е�keyΪģ�嶨��, ��accessorCursor��Ϊģ��ʵ��, �����accessorCursor����ʧ��

	//				//template <typename T>
	//				//class CFieldTestMyTemplate : public Niflect::CFieldAccessor
	//				//{
	//				//public:
	//				//	NIF_T()
	//				//		class CSubMy2 : public Niflect::CFieldAccessor
	//				//	{
	//				//	};
	//				//};
	//				// 
	//				//NIF_BS() TBindingSetting2<MySub::CFieldTestMyTemplate<float>::CSubMy2, TTypeTuple<char> >;

	//				for (auto& it000 : data.m_mapDeclarationToTaggedType)
	//				{
	//					auto a = CXStringToCString(clang_getCursorSpelling(it000.first));
	//					if (a == "CSubMy2")
	//					{
	//						auto b = clang_equalCursors(it000.first, accessorCursor);//���Ϊ, false
	//						auto c = CXStringToCString(clang_getCursorDisplayName(accessorCursor));
	//						auto d = CXStringToCString(clang_getCursorDisplayName(it000.first));
	//						auto e = clang_getCursorType(accessorCursor);
	//						auto f = clang_getCursorType(it000.first);
	//						auto g = CXStringToCString(clang_getTypeSpelling(e));//���Ϊ, MyTestClassScope::MySub::CFieldTestMyTemplate<float>::CSubMy2
	//						auto h = CXStringToCString(clang_getTypeSpelling(f));//���Ϊ, MyTestClassScope::MySub::CFieldTestMyTemplate::CSubMy2
	//						printf("");
	//					}
	//					printf("%s\n", a.c_str());
	//				}
	//			}

	//			//todo: ��Ҫ��Accessor��������, ����ʧ����3�����
	//			//1. Accessorδ�ӱ��
	//			//2. û�ж���Accessor, �����뱾���޷�����
	//			//3. ʹ��ģ��ʵ��Scope
	//			//�ƻ���ΪҪ��Accessor�����3������Accessor�̳�, �������ɸ��ݼ������ɴ��뼴��, �����Ƿ�Ϊ�����������������

	//			GenLogError(context.m_log, "The bound accessor type is not tagged");//, or not defined
	//			break;
	//		}
	//		auto& taggedAccessorType = itFoundTaggedType->second;
	//		for (auto& it1 : it0.GetBindingTypesData())
	//		{
	//			const auto& bindingType = it1.m_type;
	//			auto bindingTypeCursor = clang_getTypeDeclaration(bindingType);
	//			bool ok = false;
	//			if (clang_getCursorKind(bindingTypeCursor) == CXCursor_NoDeclFound)
	//			{
	//				if (bindingType.kind == CXType_Pointer)
	//				{
	//					GenLogError(context.m_log, "Pointer is not supported");//todo: ֧������ָ��������ʵ����;, Ӧ֧���ض����͵�ָ��, ��Ҫ��ȡ����Ϣ�缸άָ����ԭʼ����, �ƻ��ӵ���m_mapUserTypePointer1D��, ����ָ�����Ϊר�ŵ�����, ����ר�ŵ�ָ����ҪRuntime�ڴ����
	//					break;//todo: ����2��for, ��Ҫ��������߼�����д����򲻱���sibling
	//				}
	//				else
	//				{
	//					ok = data.m_accessorTypeTable.m_mapBuiltinTypeKind.insert({ bindingType.kind, taggedAccessorType }).second;
	//					if (ok)
	//					{
	//						for (uint32 idx2 = 0; idx2 < data.m_vecResolvedTypes.size(); ++idx2)
	//						{
	//							bool found = false;
	//							auto& vec = data.m_vecResolvedTypes[idx2].m_vecResolvedTaggedType;
	//							for (uint32 idx3 = 0; idx3 < vec.size(); ++idx3)
	//							{
	//								auto& resolvedTaggedType = vec[idx3];
	//								ASSERT(resolvedTaggedType.m_taggedType != NULL);
	//								if (resolvedTaggedType.m_taggedType == taggedAccessorType)
	//								{
	//									ASSERT(false);//m_isBuiltinType���Ƴ�
	//									//ASSERT(!resolvedTaggedType.m_isBuiltinType);
	//									//resolvedTaggedType.m_isBuiltinType = true;//todo: �����ޱ�Ҫ, ���ڽ���ʱ�Ѱ����û���BindingSetting.h��include DFS, ���builtin���͵�accessorһ������������builtin���͵�����֮ǰ
	//									found = true;
	//									break;
	//								}
	//							}
	//							if (found)
	//								break;
	//						}
	//					}
	//				}
	//			}
	//			else
	//			{
	//				ok = data.m_accessorTypeTable.m_mapUserType.insert({ bindingTypeCursor, taggedAccessorType }).second;
	//			}
	//			if (!ok)
	//			{
	//				GenLogError(context.m_log, "Binding types are conflicting, an identical binding type is expected for an accessor");
	//				break;//todo: ����2��for, ��Ҫ��������߼�����д����򲻱���sibling
	//			}
	//		}
	//	}
	//}
	//void CResolver::ResolveRecurs(CTaggedNode2* taggedParent, CResolvingContext& context, CResolvedData& data)
	//{
	//	if (auto taggedType = CTaggedType::CastChecked(taggedParent))
	//	{
	//		auto& cursor = taggedType->GetCursor();
	//		{
	//			auto loc = clang_getCursorLocation(cursor);
	//			CXFile file;
	//			clang_getSpellingLocation(loc, &file, NULL, NULL, NULL);
	//			auto filePathAbs = CXStringToCString(clang_getFileName(file));
	//			std::replace(filePathAbs.begin(), filePathAbs.end(), '\\', '/');
	//			auto ret = m_mapFilePathToResolvedTypeIndex.insert({ filePathAbs, static_cast<uint32>(data.m_vecResolvedTypes.size()) });
	//			if (ret.second)
	//			{
	//				CResolvedTypes item;
	//				item.m_filePathAbs = filePathAbs;
	//				bool foundPrefix = false;
	//				for (auto& it : m_vecHeaderSearchPath)
	//				{
	//					ASSERT(!it.empty());
	//					auto pos = filePathAbs.find(it);
	//					if (pos != std::string::npos)
	//					{
	//						auto prefixLength = pos + it.length();
	//						item.m_filePathRelativeToHeaderSearchPath = filePathAbs.substr(prefixLength + 1, filePathAbs.length() - prefixLength);
	//						foundPrefix = true;
	//						break;
	//					}
	//				}
	//				if (!foundPrefix)
	//				{
	//					GenLogError(context.m_log, "Unable to resolve the header relative path");
	//					return;//todo: ���ڵݹ�, ��Ҫ��������߼�����д����򲻱���sibling
	//				}
	//				data.m_vecResolvedTypes.emplace_back(item);
	//			}
	//			auto& item = data.m_vecResolvedTypes[ret.first->second];
	//			item.m_vecResolvedTaggedType.push_back(taggedType);
	//		}
	//		ASSERT(clang_isDeclaration(clang_getCursorKind(cursor)));
	//		auto ret = data.m_mapDeclarationToTaggedType.insert({ cursor, taggedType });
	//		if (!ret.second)
	//		{
	//			GenLogError(context.m_log, "Tagged type collection goes wrong");
	//			return;//todo: ���ڵݹ�, ��Ҫ��������߼�����д����򲻱���sibling
	//		}
	//	}

	//	for (auto& it0 : taggedParent->DebugGetChildren())
	//	{
	//		this->ResolveRecurs(it0.Get(), context, data);
	//	}
	//}
	//static void FindBindingTypeRecurs(const CAccessorBindingMapping2* mapping, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, Niflect::TArrayNif<uint32>& vecFoundIdx, uint32& detailIteratingIdx);
	//static void IterateForTemplate(const CAccessorBindingMapping2* mapping, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, Niflect::TArrayNif<uint32>& vecFoundIdx, uint32& detailIteratingIdx)
	//{
	//	auto argsCount = clang_Type_getNumTemplateArguments(fieldOrArgCXType);
	//	for (int32 idx1 = 0; idx1 < argsCount; ++idx1)
	//	{
	//		CXType argType = clang_Type_getTemplateArgumentAsType(fieldOrArgCXType, idx1);
	//		FindBindingTypeRecurs(mapping, argType, vecDetailCursor, vecFoundIdx, detailIteratingIdx);
	//	}
	//}
	//class CFoundResult
	//{
	//public:
	//	CFoundResult(Niflect::TArrayNif<uint32>& vecFoundIdx)
	//		: m_vecFoundIdx(vecFoundIdx)
	//		, m_foundIdx(INDEX_NONE)
	//		, m_continuing(true)
	//	{
	//	}
	//	Niflect::TArrayNif<uint32>& m_vecFoundIdx;
	//	uint32 m_foundIdx;
	//	bool m_continuing;
	//};
	//static bool FindBindingTypesSSSSSSSSSSS(const CAccessorBindingMapping2* mapping, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, uint32& detailIteratingIdx, CFoundResult& result)
	//{
	//	auto& foundIdx = result.m_foundIdx;
	//	auto& continuing = result.m_continuing;
	//	auto& vecFoundIdx = result.m_vecFoundIdx;
	//	{
	//		auto itFound = mapping->m_mapCXTypeToIndex.find(fieldOrArgCXType);
	//		if (itFound != mapping->m_mapCXTypeToIndex.end())
	//			foundIdx = itFound->second;
	//	}
	//	if (foundIdx == INDEX_NONE)
	//	{
	//		//�ػ�, <Niflect::TArrayNif<bool>, ��ֱ��ͨ��field����CXType��cursor���ҵ�BindingType��cursor
	//		auto cursor = clang_getTypeDeclaration(fieldOrArgCXType);
	//		auto itFound = mapping->m_mapSpecializedCursorToIndex.find(cursor);
	//		if (itFound != mapping->m_mapSpecializedCursorToIndex.end())
	//		{
	//			foundIdx = itFound->second;
	//			continuing = false;
	//		}
	//	}
	//	if (foundIdx == INDEX_NONE)
	//	{
	//		const CXCursor* refCursor = NULL;
	//		while ((detailIteratingIdx++) < vecDetailCursor.size())
	//		{
	//			auto& it = vecDetailCursor[detailIteratingIdx];
	//			auto kind = clang_getCursorKind(it);
	//			//�����׸��ɴ�������͵�Ref
	//			if (kind == CXCursor_TemplateRef || kind == CXCursor_TypeRef)
	//			{
	//				refCursor = &it;
	//				break;
	//			}
	//		}
	//		if (refCursor != NULL)
	//		{
	//			auto cursor = clang_getCursorReferenced(*refCursor);
	//			auto itFound = mapping->m_mapCursorToIndex.find(cursor);
	//			if (itFound != mapping->m_mapCursorToIndex.end())
	//			{
	//				foundIdx = itFound->second;
	//				continuing = IsCursorTemplateDecl(cursor);
	//			}
	//		}
	//	}
	//	if (foundIdx != INDEX_NONE)
	//	{
	//		vecFoundIdx.push_back(foundIdx);
	//		return true;
	//	}
	//	return false;
	//}
	//static void FindBindingTypeRecurs(const CAccessorBindingMapping2* mapping, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, Niflect::TArrayNif<uint32>& vecFoundIdx, uint32& detailIteratingIdx)
	//{
	//	CFoundResult result(vecFoundIdx);
	//	if (FindBindingTypesSSSSSSSSSSS(mapping, fieldOrArgCXType, vecDetailCursor, detailIteratingIdx, result))
	//	{
	//		if (result.m_continuing)
	//		{
	//			auto& bindingSetting = mapping->m_vecAccessorBindingSetting[result.m_foundIdx];
	//			auto elemDeclsCount = bindingSetting.GetELementBindingTypeDeclsCount();
	//			if (elemDeclsCount == 0)
	//			{
	//				//ģ�����ػ�, Niflect::TArrayNif<Niflect::TArrayNif<bool> >
	//				IterateForTemplate(mapping, fieldOrArgCXType, vecDetailCursor, vecFoundIdx, detailIteratingIdx);
	//			}
	//			else
	//			{
	//				//��άBindingType
	//				for (uint32 idx0 = 0; idx0 < elemDeclsCount; ++idx0)
	//				{
	//					auto& elemSubcursor = bindingSetting.GetELementBindingTypeDecl(idx0);
	//					CFoundResult result2222(vecFoundIdx);
	//					uint32 aaaa = 0;
	//					if (!FindBindingTypesSSSSSSSSSSS(mapping, elemSubcursor.m_CXType, elemSubcursor.m_vecAaaaaaaaaa, aaaa, result2222))
	//					{
	//						ASSERT(false);//todo: ����
	//						break;
	//					}
	//					auto p = &detailIteratingIdx;
	//					auto idxForRepeatedDetail = detailIteratingIdx;
	//					if (idx0 > 0)
	//						p = &idxForRepeatedDetail;
	//					IterateForTemplate(mapping, fieldOrArgCXType, vecDetailCursor, vecFoundIdx, *p);
	//				}
	//			}
	//		}
	//	}
	//}
	//static void FindBindingTypeForField(const CAccessorBindingMapping2* mapping, const CXCursor& fieldCursor, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, Niflect::TArrayNif<uint32>& vecFoundIdx)
	//{
	//	auto fieldCXType = clang_getCursorType(fieldCursor);
	//	uint32 detailIteratingIdx = 0;
	//	FindBindingTypeRecurs(mapping, fieldCXType, vecDetailCursor, vecFoundIdx, detailIteratingIdx);
	//}
	void CResolver::ResolveRecurs2(CTaggedNode2* taggedParent, CResolvingContext& context, CResolvedData& data)
	{
		if (auto taggedType = CTaggedType::CastChecked(taggedParent))
		{
			auto& cursor = taggedType->GetCursor();
			ASSERT(clang_isDeclaration(clang_getCursorKind(cursor)));
			//auto ret = m_mapCusorDeclToTaggedType.insert({ cursor, taggedType });
			//if (!ret.second)
			//{
			//	GenLogError(context.m_log, "Tagged type collection goes wrong");
			//	return;//todo: ���ڵݹ�, ��Ҫ��������߼�����д����򲻱���sibling
			//}
			//bool isAccessorCursorDecl = false;
			//if (m_foundCursorsCount < data.m_vecAccessorBinding2.size())
			//{
			//	auto itFound = m_mapAccessorCursorDeclToIndex.find(cursor);
			//	if (itFound != m_mapAccessorCursorDeclToIndex.end())
			//	{
			//		auto& item = data.m_vecAccessorBinding2[itFound->second];
			//		ASSERT(item.m_accesorTaggedType == NULL);
			//		item.m_accesorTaggedType = taggedType;
			//		m_foundCursorsCount++;
			//		isAccessorCursorDecl = true;
			//	}
			//}
			//if (!isAccessorCursorDecl)
			//{
			//	data.m_vecResolvedTypes2.push_back(taggedType);
			//}

			bool isAccessorCursorDecl = false;
#ifdef TAGGED_REQUIREMENT_OF_NON_TEMPLATE_ACCESSOR_TYPE
			auto& accessorBindingMapping = data.m_mapping.m_accessorBindingMapping;
			if (m_foundCursorsCount < accessorBindingMapping.m_vecAccessorBinding2.size())
			{
				auto itFound = m_mapAccessorCursorDeclToAccessorBindingIndex.find(cursor);
				if (itFound != m_mapAccessorCursorDeclToAccessorBindingIndex.end())
				{
					ASSERT(*itFound->second == NULL);
					*itFound->second = taggedType;
					m_foundCursorsCount++;
					isAccessorCursorDecl = true;
				}
			}
#else
#endif
			if (!isAccessorCursorDecl)
			{
				data.m_vecResolvedTypes2.push_back(taggedType);
			}
			//auto a = CXStringToCString(clang_getCursorSpelling(cursor));
			//printf("%s\n", a.c_str());
			data.deprecated_m_mapping.m_mapCursorDeclToTaggedType.insert({ cursor, taggedType });
		}
		else if (auto untaggedType = CUntaggedTemplate::CastChecked(taggedParent))
		{
			auto& cursor = untaggedType->GetCursor();
			data.deprecated_m_mapping.m_mapCursorDeclToUntaggedTemplate.insert({ cursor, untaggedType });
		}

		for (auto& it0 : taggedParent->DebugGetChildren())
		{
			this->ResolveRecurs2(it0.Get(), context, data);
		}
	}
	struct STaggedTypeIndexAndBelongingFilePath
	{
		Niflect::CString m_filePath;
		uint32 m_taggedTypeIdx;
	};
	void CResolver::Resolve4(CTaggedNode2* taggedRoot, const CResolvingContext& context, CResolvedData& data)
	{
		data.m_accessorBindingMapping = m_collectionData.m_accessorBindingMapping;

		this->ResolveRecurs4(context, taggedRoot, data, data.m_taggedMapping, data.m_untaggedTemplateMapping);

		data.m_taggedMapping.Resolve();
		data.m_untaggedTemplateMapping.Init(*m_collectionData.m_aliasChain);

		SResolvedMappings mappings{ *data.m_accessorBindingMapping, data.m_taggedMapping, data.m_untaggedTemplateMapping };
		CResolvingDependenciesContext resolvingDepCtx(mappings, context.m_log);
		SResolvingDependenciesData resolvingDepData{ data.m_signatureMapping };
		Niflect::TArrayNif<STaggedTypeIndexAndBelongingFilePath> vecFilePathAndTaggedTypeIdx;
		Niflect::TSet<Niflect::CString> setNoDup;
		//δʵ�ְ�CursorDeclaration����˳�����, ��������ResolveDependcies
		std::vector<CTaggedType*> vecValidToResolveDependencies;
		for (uint32 idx0 = 0; idx0 < data.m_taggedMapping.m_vecType.size(); ++idx0)
		{
			auto& it0 = data.m_taggedMapping.m_vecType[idx0];
			bool found = false;
			for (auto& it1 : m_moduleRegInfo.m_userProvided.m_vecModuleHeader2)
			{
				auto filePath = GetCursorFilePath(it0->GetCursor());
				if (it1 == filePath)
				{
					found = true;

					if (setNoDup.insert(filePath).second)
						vecFilePathAndTaggedTypeIdx.push_back({ filePath, idx0 });

					break;
				}
			}
			if (found)
			{
				it0->ResolveBasic(resolvingDepCtx, resolvingDepData);
				vecValidToResolveDependencies.push_back(it0);
			}
			else
			{
				it0->InitForImportType();
			}
		}
		for (auto& it0 : vecValidToResolveDependencies)
		{
			it0->ResolveDependcies(resolvingDepCtx, resolvingDepData);
		}

		for (auto& it0 : vecFilePathAndTaggedTypeIdx)
		{
			auto& tt = data.m_taggedMapping.m_vecType[it0.m_taggedTypeIdx];
			if (!tt->RequiredGenHIncluded())
				continue;
			auto& cursor = tt->GetCursor();
			CXSourceRange range = clang_getCursorExtent(cursor);
			CXTranslationUnit translationUnit = clang_Cursor_getTranslationUnit(cursor);
			{
				CXFile begin_file;
				unsigned begin_line, begin_column, begin_offset;
				clang_getSpellingLocation(clang_getRangeStart(range),
					&begin_file, &begin_line, &begin_column, &begin_offset);
				if (begin_file != NULL)
				{
					size_t size = 0;
					auto contents = clang_getFileContents(translationUnit, begin_file, &size);

					Niflect::CStringStream ss;
					Niflect::CString str;
					str.resize(begin_offset);
					memcpy(&str[0], contents, begin_offset);
					ss << str;
					Niflect::CString line;
					Niflect::TArrayNif<Niflect::CString> vecIncludeDirective;
					const Niflect::CString keywordInclude = "include";
					while (std::getline(ss, line))
					{
						bool found = false;
						auto posSingleLineComment = line.find("//");
						if (posSingleLineComment == std::string::npos)
						{
							auto pos0 = line.find('#');
							if (pos0 != std::string::npos)
							{
								auto pos1 = line.find(keywordInclude, pos0 + 1);
								if (pos1 != std::string::npos)
								{
									auto pos2 = line.find_last_of('"');
									if (pos2 == std::string::npos)
										pos2 = line.find_last_of('>');
									if (pos2 != std::string::npos)
									{
										auto posQuotL = pos1 + keywordInclude.length() + 2;
										vecIncludeDirective.push_back(line.substr(posQuotL, pos2 - posQuotL));
										found = true;
									}
								}
							}
						}
						if (!found)
						{
							if (vecIncludeDirective.size() > 0)
								break;
						}
					}
					auto includeFilePath = CIncludesHelper::ConvertToIncludePath(it0.m_filePath, m_moduleRegInfo.m_writingHeaderSearchPaths.m_vecForRegularConversion);
					if (includeFilePath.empty())
					{
						Niflect::CString str;
						for (uint32 idx1 = 0; idx1 < m_moduleRegInfo.m_writingHeaderSearchPaths.m_vecForRegularConversion.size(); ++idx1)
						{
							auto& it = m_moduleRegInfo.m_writingHeaderSearchPaths.m_vecForRegularConversion[idx1];
							str += NiflectUtil::FormatString("[%u] %s\n", idx1, it.c_str());
						}
						GenLogError(context.m_log, NiflectUtil::FormatString(
R"(The conversion of %s to an include directive failed because the file could not be found in the user provided include search paths, which are as follows:
%s)", it0.m_filePath.c_str(), str.c_str()));
					}
					auto filePathNoExt = NiflectUtil::RemoveFileExt(includeFilePath);
					auto expectedIncludeFilePath = filePathNoExt + NiflectGenDefinition::FileExt::GenH;
					bool includedGenH = vecIncludeDirective.size() > 0;
					if (includedGenH)
					{
						auto& lastInclude = vecIncludeDirective.back();
						includedGenH = lastInclude == expectedIncludeFilePath;
					}
					if (!includedGenH)
						GenLogError(context.m_log, NiflectUtil::FormatString("Expected `#include \"%s\"` at the top of the header and follows all other includes", expectedIncludeFilePath.c_str()));
				}
			}
		}
	}
	void CResolver::ResolveRecurs4(const CResolvingContext& context, CTaggedNode2* taggedParent, CResolvedData& data, CTaggedTypesMapping& taggedTypesMapping, CUntaggedTemplatesMapping& untaggedTemplatesMapping)
	{
		SResolvingMacroNataContext macroNataCtx{ context.m_log };
		taggedParent->ResolveMacroNata(macroNataCtx);

		if (auto taggedType = CTaggedType::CastChecked(taggedParent))
		{
			auto& cursor = taggedType->GetCursor();
			ASSERT(clang_isDeclaration(clang_getCursorKind(cursor)));
			auto ret = taggedTypesMapping.m_mapCursorToIndex.insert({ cursor, static_cast<uint32>(taggedTypesMapping.m_vecType.size())});
			ASSERT(ret.second);
			taggedTypesMapping.m_vecType.push_back(taggedType);
		}
		else if (auto untaggedType = CUntaggedTemplate::CastChecked(taggedParent))
		{
			auto& cursor = untaggedType->GetCursor();
			auto ret = untaggedTemplatesMapping.m_mapCursorToIndex.insert({ cursor, static_cast<uint32>(untaggedTemplatesMapping.m_vecType.size()) });
			ASSERT(ret.second);
			untaggedTemplatesMapping.m_vecType.push_back(untaggedType);
		}

		for (auto& it0 : taggedParent->DebugGetChildren())
		{
			this->ResolveRecurs4(context, it0.Get(), data, taggedTypesMapping, untaggedTemplatesMapping);
		}
	}

	// Ԥ���Resolve�����ǽ����������, ���������ɴ����׼������, �罫TaggedNode������˳������
	// C++�﷨�н�ö��֧��ֻ��������ʹ��, �������÷���ȫ�ޱ�Ҫ֧��
	// ����ݲ�������˳�����ɴ���, ʹ�õ�˳����ȫ�������Դ��
	//����Ϊ���ô���, ����������˳���TaggedNode����
	
	TSharedPtr<TCursorMap<CTaggedType*> > g_my_unordered_map;//�����ָ���Ϊ����δʹ��ʱռ�õ��ڴ�(��̬ռ��)
	void DDDDDDDDDDDDDDDDDRecurs(CTaggedNode2* taggedParent, int level)
	{
		if (g_my_unordered_map == NULL)
			g_my_unordered_map = MakeShared<TCursorMap<CTaggedType*> >();

		auto& my_unordered_map = *g_my_unordered_map;
		if (auto taggedType = CTaggedType::CastChecked(taggedParent))
			my_unordered_map[taggedType->GetCursor()] = taggedType;


		//auto strLevel = NiflectUtil::DebugIndentToString(level);

		//auto& cursor = taggedParent->DebugGetCursor();
		//auto strName = CXStringToCString(clang_getCursorSpelling(cursor));
		//auto type = clang_getCursorType(cursor);
		//auto strType = CXStringToCString(clang_getTypeSpelling(type));
		//printf("%s%s, %s\n", strLevel.c_str(), strName.c_str(), strType.c_str());


		level++;

		for (auto& it0 : taggedParent->DebugGetChildren())
		{
			DDDDDDDDDDDDDDDDDRecurs(it0.Get(), level);
		}
	}
	void DDDDDDDDDDDDDDDDDRecurs2(CTaggedNode2* taggedParent, TArrayNif<CTaggedType*>& vecOrderedType, TSet<CTaggedType*>& setAdded, int level)
	{
		auto& my_unordered_map = *g_my_unordered_map;
		auto taggedType = CTaggedType::CastChecked(taggedParent);
		if (taggedType != NULL)
		{
			if (!setAdded.insert(taggedType).second)
				return;
		}
		else
		{
		}

		{
			auto& cursor = taggedParent->GetCursor();
			auto type = clang_getCursorType(cursor);
			type = GetPointerOriginalType(type);
			auto typeCursor = clang_getTypeDeclaration(type);
			auto itFound = my_unordered_map.find(typeCursor);
			if (itFound != my_unordered_map.end())
			{
				DDDDDDDDDDDDDDDDDRecurs2(itFound->second, vecOrderedType, setAdded, level);
			}
			else
			{
				//root��builtin
			}
		}

		for (auto& it0 : taggedParent->DebugGetChildren())
		{
			DDDDDDDDDDDDDDDDDRecurs2(it0.Get(), vecOrderedType, setAdded, level);
		}

		if (taggedType != NULL)
		{
			//vecOrderedType.insert(vecOrderedType.begin(), taggedType);
			vecOrderedType.push_back(taggedType);
		}
	}
	void SortTaggedNodesInDependencyOrder(CTaggedNode2* taggedParent, Niflect::TArrayNif<CTaggedType*>& vecOrderedType)
	{
		DDDDDDDDDDDDDDDDDRecurs(taggedParent, 0);
		TSet<CTaggedType*> setAdded;
		DDDDDDDDDDDDDDDDDRecurs2(taggedParent, vecOrderedType, setAdded, 0);
	}
}