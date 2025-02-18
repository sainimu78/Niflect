#include "NiflectGen/Test/TestFieldResocursorName.h"
#include "NiflectGen/Generator/Generator.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "Niflect/Memory/Default/DefaultMemory.h"
#include "NiflectGen/Collector/Collector.h"
#include "NiflectGen/Resolver/Resolver.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCodeWriter.h"

namespace TestGen
{
	using namespace NiflectGen;

	static void InitGenFieldResocursorName(CGenerator& gen, uint32 idx)
	{
		CModuleRegInfo info;
		info.m_vecAccessorSettingHeader.push_back(CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestAccessorSettingFieldResocursorName.h"));
		auto moduleHeader = NiflectUtil::FormatString("%s%u.h", CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PATH, "/TestModuleHeaderFieldResocursorName"), idx);
		info.m_vecModuleHeader2.push_back(moduleHeader);
		Test::InitArgs(info);
		CGenLog log;
		gen.Init(info, &log);
	}

	static void TestSuccess_FieldResocursorName0()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 0);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//单一类型
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::EMyOption");
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("float");
					vecExpected.push_back("bool");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("int32");
					vecExpected.push_back("TestAccessor2::TMyTransform<float>");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName1()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 1);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//特化
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("TestAccessor2::TMyTransform<float>");
					vecExpected.push_back("Niflect::TArray<bool>");
					vecExpected.push_back("std::vector<bool>");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName2()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 2);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//1维容器模板
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("float");
					vecExpected.push_back("Niflect::TArray<float>");
					vecExpected.push_back("+float");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName3()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 3);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//嵌套1维容器模板
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("float");
					vecExpected.push_back("Niflect::TArray<float>");
					vecExpected.push_back("+float");
					vecExpected.push_back("Niflect::TArray<Niflect::TArray<float> >");
					vecExpected.push_back("+Niflect::TArray<float>");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName4()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 4);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//结构模板
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("float");
					vecExpected.push_back("std::pair<Niflect::CString, float>");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-float");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName5()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 5);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//1维容器模板与结构模板
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("float");
					vecExpected.push_back("std::pair<Niflect::CString, float>");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-float");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, float>");
					vecExpected.push_back("+std::pair<Niflect::CString, float>");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName6()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 6);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//不同的1容器模板嵌套
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("float");
					vecExpected.push_back("Niflect::TArray<float>");
					vecExpected.push_back("+float");
					vecExpected.push_back("std::pair<Niflect::CString, Niflect::TArray<float> >");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-Niflect::TArray<float>");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, Niflect::TArray<float> >");
					vecExpected.push_back("+std::pair<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName7()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 7);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//1维容器模板套特化
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("Niflect::TArray<bool>");
					vecExpected.push_back("std::pair<Niflect::CString, Niflect::TArray<bool> >");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-Niflect::TArray<bool>");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, Niflect::TArray<bool> >");
					vecExpected.push_back("+std::pair<Niflect::CString, Niflect::TArray<bool> >");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName8()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 8);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//不同的1维容器模板嵌套
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("float");
					vecExpected.push_back("std::pair<Niflect::CString, float>");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-float");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, float>");
					vecExpected.push_back("+std::pair<Niflect::CString, float>");
					vecExpected.push_back("Niflect::TArray<Niflect::TMap<Niflect::CString, float> >");
					vecExpected.push_back("+Niflect::TMap<Niflect::CString, float>");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName9()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 9);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//不同的1维容器模板嵌套
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("float");
					vecExpected.push_back("Niflect::TArray<float>");
					vecExpected.push_back("+float");
					vecExpected.push_back("std::pair<Niflect::CString, Niflect::TArray<float> >");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-Niflect::TArray<float>");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, Niflect::TArray<float> >");
					vecExpected.push_back("+std::pair<Niflect::CString, Niflect::TArray<float> >");
					vecExpected.push_back("Niflect::TArray<Niflect::TMap<Niflect::CString, Niflect::TArray<float> > >");
					vecExpected.push_back("+Niflect::TMap<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName10()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 10);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//位置在后的成员不产生重复Signature
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("float");
					vecExpected.push_back("std::pair<Niflect::CString, float>");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-float");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, float>");
					vecExpected.push_back("+std::pair<Niflect::CString, float>");
					vecExpected.push_back("Niflect::TArray<Niflect::TMap<Niflect::CString, float> >");
					vecExpected.push_back("+Niflect::TMap<Niflect::CString, float>");
					vecExpected.push_back("std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-Niflect::TArray<Niflect::TMap<Niflect::CString, float> >");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					vecExpected.push_back("+std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName11()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 11);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//综合测试
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("float");
					vecExpected.push_back("bool");
					vecExpected.push_back("Niflect::TArray<float>");
					vecExpected.push_back("+float");
					vecExpected.push_back("Niflect::TArray<bool>");
					vecExpected.push_back("Niflect::TArray<Niflect::TArray<float> >");
					vecExpected.push_back("+Niflect::TArray<float>");
					vecExpected.push_back("Niflect::TArray<Niflect::TArray<bool> >");
					vecExpected.push_back("+Niflect::TArray<bool>");
					vecExpected.push_back("std::vector<float>");
					vecExpected.push_back("+float");
					vecExpected.push_back("std::vector<bool>");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("std::pair<Niflect::CString, float>");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-float");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, float>");
					vecExpected.push_back("+std::pair<Niflect::CString, float>");
					vecExpected.push_back("int32");
					vecExpected.push_back("std::pair<Niflect::CString, Niflect::TArray<float> >");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-Niflect::TArray<float>");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, Niflect::TArray<float> >");
					vecExpected.push_back("+std::pair<Niflect::CString, Niflect::TArray<float> >");
					vecExpected.push_back("std::pair<Niflect::CString, Niflect::TArray<bool> >");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-Niflect::TArray<bool>");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, Niflect::TArray<bool> >");
					vecExpected.push_back("+std::pair<Niflect::CString, Niflect::TArray<bool> >");
					vecExpected.push_back("Niflect::TArray<Niflect::TMap<Niflect::CString, float> >");
					vecExpected.push_back("+Niflect::TMap<Niflect::CString, float>");
					vecExpected.push_back("std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-Niflect::TArray<Niflect::TMap<Niflect::CString, float> >");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					vecExpected.push_back("+std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					vecExpected.push_back("TestAccessor2::TMyTransform<float>");
					vecExpected.push_back("Niflect::TArray<Niflect::TMap<Niflect::CString, Niflect::TArray<float> > >");
					vecExpected.push_back("+Niflect::TMap<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName12()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 12);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//成员为TaggedType
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("TestMyFinding::CMyClass_1");
					vecExpected.push_back("bool");
					vecExpected.push_back("float");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName13()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 13);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//成员含TaggedType的综合测试
					Niflect::TArray<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("TestMyFinding::CMyClass_1");
					vecExpected.push_back("bool");
					vecExpected.push_back("float");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("std::pair<Niflect::CString, TestMyFinding::CMyClass_0>");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::TArray<TestMyFinding::CMyClass_0>");
					vecExpected.push_back("+TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0>");
					vecExpected.push_back("+std::pair<Niflect::CString, TestMyFinding::CMyClass_0>");
					vecExpected.push_back("std::pair<Niflect::CString, Niflect::TArray<TestMyFinding::CMyClass_0> >");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-Niflect::TArray<TestMyFinding::CMyClass_0>");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, Niflect::TArray<TestMyFinding::CMyClass_0> >");
					vecExpected.push_back("+std::pair<Niflect::CString, Niflect::TArray<TestMyFinding::CMyClass_0> >");
					vecExpected.push_back("Niflect::TArray<Niflect::TMap<Niflect::CString, Niflect::TArray<TestMyFinding::CMyClass_0> > >");
					vecExpected.push_back("+Niflect::TMap<Niflect::CString, Niflect::TArray<TestMyFinding::CMyClass_0> >");
					vecExpected.push_back("Niflect::TArray<Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0> >");
					vecExpected.push_back("+Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0>");
					vecExpected.push_back("std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0> > >");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-Niflect::TArray<Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0> >");
					vecExpected.push_back("Niflect::TMap<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0> > >");
					vecExpected.push_back("+std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0> > >");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName14()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 14);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//1维容器模板套结构模板
					Niflect::TArrayNif<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("float");
					vecExpected.push_back("std::pair<Niflect::CString, float>");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-float");
					vecExpected.push_back("Niflect::TArray<std::pair<Niflect::CString, float> >");
					vecExpected.push_back("+std::pair<Niflect::CString, float>");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName15()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 15);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//Raw 指针
					Niflect::TArrayNif<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("TestMyScope::CMyResource*");
					vecExpected.push_back("TestMyScope::CMyAsset*");
					vecExpected.push_back("Niflect::TArray<TestMyScope::CMyAsset*>");
					vecExpected.push_back("+TestMyScope::CMyAsset*");
					vecExpected.push_back("TestMyScope::CMyResource**");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	static void TestSuccess_FieldResocursorName16()
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			InitGenFieldResocursorName(*gen, 16);
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
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					//Raw 指针
					Niflect::TArrayNif<Niflect::CString> vecExpected;
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("TestMyScope::CMyResource");
					vecExpected.push_back("TestMyScope::TSharedMyResource<TestMyScope::CMyResource>");
					vecExpected.push_back("+TestMyScope::CMyResource");
					ASSERT(vecSignature.size() == vecExpected.size());
					for (uint32 idx = 0; idx < vecExpected.size(); ++idx)
						ASSERT(vecSignature[idx] == vecExpected[idx]);
				});
		}
	}
	void TestSuccess_FieldResocursorName()
	{
		TestSuccess_FieldResocursorName0();
		TestSuccess_FieldResocursorName1();
		TestSuccess_FieldResocursorName2();
		TestSuccess_FieldResocursorName3();
		TestSuccess_FieldResocursorName4();
		TestSuccess_FieldResocursorName5();
		TestSuccess_FieldResocursorName6();
		TestSuccess_FieldResocursorName7();
		TestSuccess_FieldResocursorName8();
		TestSuccess_FieldResocursorName9();
		TestSuccess_FieldResocursorName10();
		TestSuccess_FieldResocursorName11();
		TestSuccess_FieldResocursorName12();
		TestSuccess_FieldResocursorName13();
		TestSuccess_FieldResocursorName14();
		TestSuccess_FieldResocursorName15();
		TestSuccess_FieldResocursorName16();
	}
}