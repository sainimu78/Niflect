#include "NiflectGen/Test/TestGen.h"
#include "NiflectGen/Generator/Generator.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "Niflect/Memory/Default/DefaultMemory.h"
#include "NiflectGen/Collector/Collector.h"
#include "NiflectGen/Resolver/Resolver.h"
#include "NiflectGen/Test/TestFieldResocursorName.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCodeWriter.h"
#include "NiflectGen/Test/TestBindingTypeAlias.h"

namespace TestGen
{
	using namespace NiflectGen;

	static void TestSuccess_AccessorTypeAlias()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			CModuleRegInfo info;
			info.m_vecAccessorSettingHeader.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAccessorSettingAccessorTypeAliass.h"));
			Test::InitArgs(info);
			gen->Init(info, NULL);
			CCodeGenData genData;
			gen->Generate(genData, [&gen](void* cursorAddr)
				{
					auto& cursor = *static_cast<CXCursor*>(cursorAddr);
					CTaggedNode2 taggedRoot;
					CGenLog log;
					//log.Config(CGenLogOption().SetAssertionOnAddingItem(false).SetCachedItems(true));
					CCollectingContext context(&log);
					CCollectionData collectionData;
					CDataCollector collector(gen->GetModuleRegInfo());
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					auto& vec = collectionData.m_accessorBindingMapping->m_settings.m_vecAccessorBindingSetting;
					ASSERT(log.m_vecText.size() == 0);
					//ASSERT(vec.size() == 1);
					//auto accessorTypeName = CXStringToCString(clang_getTypeSpelling(vec.back().m_subcursorRoot.m_vecChild[0].m_CXType));
					//ASSERT(accessorTypeName == "TMyAccessorAlias<T>");
					//auto bindingTypeName = CXStringToCString(clang_getTypeSpelling(vec.back().m_subcursorRoot.m_vecChild[1].m_CXType));
					//ASSERT(bindingTypeName == "TestAccessor2::TMyTransform<float>");
				});
		}
	}
	static void TestSuccess_BindingTypesAllUnique()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			CModuleRegInfo info;
			info.m_vecAccessorSettingHeader.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAccessorSettingBindingTypesAllUnique.h"));
			Test::InitArgs(info);
			gen->Init(info, NULL);
			CCodeGenData genData;
			gen->Generate(genData, [&gen](void* cursorAddr)
				{
					auto& cursor = *static_cast<CXCursor*>(cursorAddr);
					CTaggedNode2 taggedRoot;
					CGenLog log;
					CCollectingContext context(&log);
					CCollectionData collectionData;
					CDataCollector collector(gen->GetModuleRegInfo());
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					auto& vec = collectionData.m_accessorBindingMapping->m_settings.m_vecAccessorBindingSetting;
					ASSERT(log.m_vecText.size() == 0);
				});
		}
	}
	static void TestFailure_BindingTypesDuplicated()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			CModuleRegInfo info;
			info.m_vecAccessorSettingHeader.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAccessorSettingBindingTypesDuplicated.h"));
			Test::InitArgs(info);
			gen->Init(info, NULL);
			CCodeGenData genData;
			gen->Generate(genData, [&gen](void* cursorAddr)
				{
					auto& cursor = *static_cast<CXCursor*>(cursorAddr);
					CTaggedNode2 taggedRoot;
					CGenLog log;
					log.Config(CGenLogOption().SetAssertionOnAddingItem(false).SetCachedItems(true));
					CCollectingContext context(&log);
					CCollectionData collectionData;
					CDataCollector collector(gen->GetModuleRegInfo());
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					auto& vec = collectionData.m_accessorBindingMapping->m_settings.m_vecAccessorBindingSetting;
					ASSERT(log.m_vecText.size() == 4);
				});
		}
	}
	static void TestSuccess_FullScopes()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			CModuleRegInfo info;
			info.m_vecAccessorSettingHeader.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAccessorSettingFullScopes.h"));
			Test::InitArgs(info);
			gen->Init(info, NULL);
			CCodeGenData genData;
			gen->Generate(genData, [&gen](void* cursorAddr)
				{
					auto& cursor = *static_cast<CXCursor*>(cursorAddr);
					CTaggedNode2 taggedRoot;
					CGenLog log;
					CCollectingContext context(&log);
					CCollectionData collectionData;
					CDataCollector collector(gen->GetModuleRegInfo());
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					auto& vec0 = collectionData.m_accessorBindingMapping->m_settings.m_vecAccessorBindingSetting;
					ASSERT(vec0.size() > 0);

					Niflect::TArrayNif<Niflect::CString> vecExpected;
					vecExpected.push_back("TestAccessor2::TMyTransform<float>");
					vecExpected.push_back("EngineTypeBindingSettingScope::CMyTM");
					vecExpected.push_back("Niflect::TArray<float>");
					vecExpected.push_back("Niflect::TArray<EngineTypeBindingSettingScope::CMyTM>");
					vecExpected.push_back("TestGenMyScope::MyAliasInt8_0");
					vecExpected.push_back("TestGenMyScope::MyAliasInt8_1");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, EngineTypeBindingSettingScope::CMyTM>");
					vecExpected.push_back("Niflect::TMap<float, EngineTypeBindingSettingScope::CMyTM>");
					vecExpected.push_back("Niflect::TMap<bool, EngineTypeBindingSettingScope::CMyTM>");
					vecExpected.push_back("Niflect::TMap<float, bool>");
					vecExpected.push_back("Niflect::TMap<std::string, bool>");
					vecExpected.push_back("std::map<std::string, int8>");
					vecExpected.push_back("TestGenMyScope::MyAliasInt8_2");
					vecExpected.push_back("TestGenMyScope::SubScope::SubMyAliasInt8_0");
#ifdef CLASS_TYPE_SCOPE
					vecExpected.push_back("TestGenMyScope::CSub_0::CSubSub_0");
					vecExpected.push_back("TestGenMyScope::CSub_1::CSubSub_0");
#else
#endif
					vecExpected.push_back("TestGenMyScope::MyAliasInt8_3");
					vecExpected.push_back("TestGenMyScope::MyAliasInt8_4");
					vecExpected.push_back("TestGenMyScope::MyAliasSub_0");
					vecExpected.push_back("TestGenMyScope::MyAliasSub_1");
					vecExpected.push_back("TestGenMyScope::MyAliasSub_2");
#ifdef TEMPLATE_INSTANCE_SCOPE
					vecExpected.push_back("TestGenMyScope::TSub_2<float>::CSubSub_0");
#endif

					uint32 testedIdx = 0;
					for (uint32 idx0 = 0; idx0 < vec0.size(); ++idx0)
					{
						auto& bSubcursor = vec0[idx0].GetBindingTypeDecl();

						//auto kind = clang_getCursorKind(bSubcursor.m_cursorDecl);
						//if (!IsCursorKindTemplateDecl(kind))//非模板或暂不在范围, 如需要应确认模板参数排除生成namespace流程(可能已支持, 未测试)
						//{
						//	Niflect::CString resolvedName;
						//	if (kind == CXCursor_ClassDecl)
						//	{
						//		//特化模板
						//		if (clang_Type_getNumTemplateArguments(bSubcursor.m_CXType) > 0)
						//		{
						//			GenerateTemplateInstanceCode(bSubcursor, resolvedName, CGenerateTemplateInstanceCodeOption().SetWithFullScope(true));
						//		}
						//		else
						//		{
						//			//类型Scope中的类型, 如 TestGenMyScope::CSub::CSubSub
						//			resolvedName += GenerateNamespacesAndScopesCode(bSubcursor.m_cursorDecl);
						//			resolvedName += CXStringToCString(clang_getCursorSpelling(bSubcursor.m_cursorDecl));
						//		}
						//	}
						//	else
						//	{
						//		if ((kind == CXCursor_TypeAliasDecl)//using 别名
						//			|| (kind == CXCursor_TypedefDecl)//typedef 别名
						//			)
						//		{
						//			resolvedName += GenerateNamespacesAndScopesCode(bSubcursor.m_cursorDecl);
						//			resolvedName += CXStringToCString(clang_getCursorSpelling(bSubcursor.m_cursorDecl));
						//		}
						//		else
						//		{
						//			//Builtin类型
						//			ASSERT(kind == CXCursor_NoDeclFound);
						//			resolvedName += CXStringToCString(clang_getTypeSpelling(bSubcursor.m_CXType));
						//		}
						//	}
						//	ASSERT(vecExpected[testedIdx] == resolvedName);
						//	testedIdx++;
						//}

						auto instanceTypeName = GenerateFullScopeTypeName(bSubcursor);
						ASSERT(vecExpected[testedIdx] == instanceTypeName);
						testedIdx++;
					}
					ASSERT(testedIdx == vecExpected.size());
				});
		}
	}
	static void TestSuccess_AccessorFinding()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			CModuleRegInfo info;
			info.m_vecModuleHeader2.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestModuleHeaderAccessorFinding.h"));
			info.m_vecAccessorSettingHeader.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAccessorSettingAccessorFinding.h"));
			Test::InitArgs(info);
			gen->Init(info, NULL);
			CCodeGenData genData;
			gen->Generate(genData, [&gen](void* cursorAddr)
				{
					auto& cursor = *static_cast<CXCursor*>(cursorAddr);
					CTaggedNode2 taggedRoot;
					CGenLog log;
					CCollectingContext context(&log);
					CCollectionData collectionData;
					CDataCollector collector(gen->GetModuleRegInfo());
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecExpected;
					vecExpected.push_back("CCompoundAccessor");//TestMyFinding::EMyOption, 第0个本应为枚举的Accessor, 现未实现对应的AccessorBinding, 实现后再增加相应的测试
					vecExpected.push_back("MyAccessor::CMyClassAccessor");//TestMyFinding::CMyClass_0
					vecExpected.push_back("CCompoundAccessor");//TestMyFinding::CMyClass_1
					uint32 idx0 = 0;
					//仅对 TaggedType 进行测试
					for (auto& it0 : resolvedData.m_signatureMapping.m_vecItem)
					{
						if (it0.m_resoRoot.m_taggedTypeIndex != INDEX_NONE)
						{
							Niflect::CString accessorResocursorName;
							if (it0.m_resoRoot.m_accessorBindingIndex != INDEX_NONE)
							{
								auto& setting = resolvedData.m_accessorBindingMapping->m_settings.m_vecAccessorBindingSetting[it0.m_resoRoot.m_accessorBindingIndex];
								accessorResocursorName = setting.m_accessorSettingResolvedInfo.m_resoInfo.m_resocursorName;
							}
							else
							{
								accessorResocursorName = NiflectGenDefinition::NiflectFramework::AccessorTypeName::CompoundField;
							}
							ASSERT(vecExpected[idx0] == accessorResocursorName);
							idx0++;
						}
					}
					ASSERT(vecExpected.size() == idx0);
				});
		}
	}
	static void TestSuccess_AccessorResocursorName()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			CModuleRegInfo info;
			info.m_vecAccessorSettingHeader.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAccessorSettingAccessorResocursorName.h"));
			Test::InitArgs(info);
			gen->Init(info, NULL);
			CCodeGenData genData;
			gen->Generate(genData, [&gen](void* cursorAddr)
				{
					auto& cursor = *static_cast<CXCursor*>(cursorAddr);
					CTaggedNode2 taggedRoot;
					CGenLog log;
					CCollectingContext context(&log);
					CCollectionData collectionData;
					CDataCollector collector(gen->GetModuleRegInfo());
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecExpectedA;
					Niflect::TArrayNif<Niflect::CString> vecExpectedB;
					vecExpectedA.push_back("TestAccessor2::TMyTransformAccessor<float>");
					vecExpectedB.push_back("TestAccessor2::TMyTransform<float>");
					vecExpectedA.push_back("TestAccessor2::TMyTransformAccessor<float>");
					vecExpectedB.push_back("TestAccessor2::TMyTransform<double>");
					vecExpectedA.push_back("TestAccessor2::TMyTransformAccessor<float>");
					vecExpectedB.push_back("MyScope2::TMyScope2TM<int32>");
					vecExpectedA.push_back("TestAccessor2::TMyTransformAccessor<float>");
					vecExpectedB.push_back("MyScope2::SubScope1::TMySubScope1TM<uint32>");
					vecExpectedA.push_back("TestAccessor2::TMyTransformAccessor<float>");
					vecExpectedB.push_back("MyScope2::SubScope1::TMySubScope1TM<int64>");
					vecExpectedA.push_back("TestAccessor2::TMyTransformAccessor<float>");
					vecExpectedB.push_back("Niflect::TArray<MyScope2::SMyStruct>");
					vecExpectedA.push_back("Niflect::TArrayAccessor");
					vecExpectedB.push_back("Niflect::TArray");
					vecExpectedA.push_back("Niflect::CCompoundAccessor");
					vecExpectedB.push_back("std::pair");
					uint32 idxA = 0;
					uint32 idxB = 0;
					auto& vecCollected = collectionData.m_accessorBindingMapping->m_settings.m_vecAccessorBindingSetting;
					for (auto& it : vecCollected)
					{
						ASSERT(it.m_accessorSettingResolvedInfo.m_resoInfo.m_resocursorName == vecExpectedA[idxA]);
						ASSERT(it.m_bindingResocursorName == vecExpectedB[idxB]);
						idxA++;
						idxB++;
					}
					ASSERT(idxA == vecCollected.size());
					ASSERT(idxB == vecCollected.size());
				});
		}
	}
	static void TestSuccess_RequiredHeader()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			CModuleRegInfo info;
			auto moduleHeader = CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestModuleHeaderRequiredHeader.h");
			info.m_vecModuleHeader2.push_back(moduleHeader);
			info.m_vecAccessorSettingHeader.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAccessorSettingRequiredHeader.h"));
			Test::InitArgs(info);
			gen->Init(info, NULL);
			CCodeGenData genData;
			gen->Generate(genData, [&gen, &moduleHeader](void* cursorAddr)
				{
					auto& cursor = *static_cast<CXCursor*>(cursorAddr);
					CTaggedNode2 taggedRoot;
					CGenLog log;
					CCollectingContext context(&log);
					CCollectionData collectionData;
					CDataCollector collector(gen->GetModuleRegInfo());
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecExpected;
					vecExpected.push_back(moduleHeader);//类本身ResocursorNode所在头文件
					vecExpected.push_back("Niflect/Base/String.h");
					vecExpected.push_back("Engine/Test/TestMyTransform.h");
					vecExpected.push_back("_GenTime/string");
					vecExpected.push_back(moduleHeader);//自定义TMyArray所在头文件
					vecExpected.push_back("NiflectGen/Test/TestSomeTypes.h");
					uint32 idxExpected = 0;
					for (auto& it0 : resolvedData.m_signatureMapping.m_vecItem)
					{
						for (auto& it1 : it0.m_resoRoot.DebugGetHeaderFilePaths())
						{
							auto pos = it1.find(vecExpected[idxExpected]);
							ASSERT(pos != std::string::npos);
							idxExpected++;
							//printf("%s\n", it1.c_str());
						}
						//printf("################\n");
					}
					ASSERT(idxExpected == vecExpected.size());
				});
		}
	}
	static void TestSuccess_AllowedAliasFieldType()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			CModuleRegInfo info;
			info.m_vecAccessorSettingHeader.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAccessorSettingAllowedAliasFieldType.h"));
			info.m_vecModuleHeader2.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAllowedAliasFieldType.h"));
			info.m_allowedFieldTypeAsForBindingTypeInAS = true;
			Test::InitArgs(info);
			gen->Init(info, NULL);
			CCodeGenData genData;
			gen->Generate(genData, [&gen](void* cursorAddr)
				{
					auto& cursor = *static_cast<CXCursor*>(cursorAddr);
					CTaggedNode2 taggedRoot;
					CGenLog log;
					log.Config(CGenLogOption().SetAssertionOnAddingItem(false).SetCachedItems(true));
					CCollectingContext context(&log);
					CCollectionData collectionData;
					CDataCollector collector(gen->GetModuleRegInfo());
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
				});
		}
	}
	static void TestSuccess_TypeRegCodeGen0()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			CModuleRegInfo info;
			info.m_vecModuleHeader2.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestModuleHeaderCodeGen0.h"));
			info.m_vecAccessorSettingHeader.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAccessorSettingCodeGen0.h"));
			info.m_exportedStaticGetTypeFunctions = true;
			Test::InitArgs(info);
			CGenLog log;
			gen->Init(info, &log);
			CCodeGenData genData;
			gen->Generate(genData, [&gen, &genData, &log](void* cursorAddr)
				{
					auto& cursor = *static_cast<CXCursor*>(cursorAddr);
					CTaggedNode2 taggedRoot;
					CCollectingContext context(&log);
					CCollectionData collectionData;
					CDataCollector collector(gen->GetModuleRegInfo());
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					CTemplateBasedCppWriter writer(resolvedData, gen->GetModuleRegInfo());
					CWritingContext writingContext(&log);
					writer.Write3(writingContext, genData);
				});
			gen->Save2(genData);
			gen->Cleanup();
		}
	}
	void TestCollector()
	{
		TestSuccess_AccessorTypeAlias();
		TestFailure_BindingTypeAlias();
		TestSuccess_BindingTypesAllUnique();
		TestFailure_BindingTypesDuplicated();
		TestSuccess_FullScopes();
		TestSuccess_FieldResocursorName();
		TestSuccess_AccessorFinding();
		TestSuccess_AccessorResocursorName();
		TestSuccess_RequiredHeader();
		TestSuccess_AllowedAliasFieldType();
		//TestSuccess_TypeRegCodeGen0();
	}
}