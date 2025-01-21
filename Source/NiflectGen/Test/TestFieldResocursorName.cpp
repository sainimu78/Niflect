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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 7);
					auto it = vecSignature.begin();
					//单一类型
					ASSERT(*(it++) == "TestMyFinding::EMyOption");
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "bool");
					ASSERT(*(it++) == "Niflect::CString");
					ASSERT(*(it++) == "int32");
					ASSERT(*(it++) == "TestAccessor2::TMyTransform<float>");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 4);
					auto it = vecSignature.begin();
					//特化
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "TestAccessor2::TMyTransform<float>");
					ASSERT(*(it++) == "Niflect::TArray<bool>");
					ASSERT(*(it++) == "std::vector<bool>");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 4);
					auto it = vecSignature.begin();
					//1维容器模板
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "Niflect::TArray<float>");
					ASSERT(*(it++) == "+float");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 6);
					auto it = vecSignature.begin();
					//嵌套1维容器模板
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "Niflect::TArray<float>");
					ASSERT(*(it++) == "+float");
					ASSERT(*(it++) == "Niflect::TArray<Niflect::TArray<float> >");
					ASSERT(*(it++) == "+Niflect::TArray<float>");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 6);
					auto it = vecSignature.begin();
					//结构模板
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "Niflect::CString");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "std::pair<Niflect::CString, float>");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-float");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 8);
					auto it = vecSignature.begin();
					//1维容器模板与结构模板
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "Niflect::CString");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "std::pair<Niflect::CString, float>");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-float");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, float>");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, float>");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 10);
					auto it = vecSignature.begin();
					//不同的1容器模板嵌套
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "Niflect::CString");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "Niflect::TArray<float>");
					ASSERT(*(it++) == "+float");
					ASSERT(*(it++) == "std::pair<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-Niflect::TArray<float>");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 8);
					auto it = vecSignature.begin();
					//1维容器模板套特化
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "Niflect::CString");
					ASSERT(*(it++) == "Niflect::TArray<bool>");
					ASSERT(*(it++) == "std::pair<Niflect::CString, Niflect::TArray<bool> >");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-Niflect::TArray<bool>");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, Niflect::TArray<bool> >");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, Niflect::TArray<bool> >");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 10);
					auto it = vecSignature.begin();
					//不同的1维容器模板嵌套
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "Niflect::CString");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "std::pair<Niflect::CString, float>");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-float");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, float>");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, float>");
					ASSERT(*(it++) == "Niflect::TArray<Niflect::TMap<Niflect::CString, float> >");
					ASSERT(*(it++) == "+Niflect::TMap<Niflect::CString, float>");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 12);
					auto it = vecSignature.begin();
					//不同的1维容器模板嵌套
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "Niflect::CString");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "Niflect::TArray<float>");
					ASSERT(*(it++) == "+float");
					ASSERT(*(it++) == "std::pair<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-Niflect::TArray<float>");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(*(it++) == "Niflect::TArray<Niflect::TMap<Niflect::CString, Niflect::TArray<float> > >");
					ASSERT(*(it++) == "+Niflect::TMap<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 15);
					auto it = vecSignature.begin();
					//位置在后的成员不产生重复Signature
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "Niflect::CString");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "std::pair<Niflect::CString, float>");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-float");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, float>");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, float>");
					ASSERT(*(it++) == "Niflect::TArray<Niflect::TMap<Niflect::CString, float> >");
					ASSERT(*(it++) == "+Niflect::TMap<Niflect::CString, float>");
					ASSERT(*(it++) == "std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-Niflect::TArray<Niflect::TMap<Niflect::CString, float> >");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 40);
					auto it = vecSignature.begin();
					//综合测试
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "bool");
					ASSERT(*(it++) == "Niflect::TArray<float>");
					ASSERT(*(it++) == "+float");
					ASSERT(*(it++) == "Niflect::TArray<bool>");
					ASSERT(*(it++) == "Niflect::TArray<Niflect::TArray<float> >");
					ASSERT(*(it++) == "+Niflect::TArray<float>");
					ASSERT(*(it++) == "Niflect::TArray<Niflect::TArray<bool> >");
					ASSERT(*(it++) == "+Niflect::TArray<bool>");
					ASSERT(*(it++) == "std::vector<float>");
					ASSERT(*(it++) == "+float");
					ASSERT(*(it++) == "std::vector<bool>");
					ASSERT(*(it++) == "Niflect::CString");
					ASSERT(*(it++) == "std::pair<Niflect::CString, float>");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-float");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, float>");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, float>");
					ASSERT(*(it++) == "int32");
					ASSERT(*(it++) == "std::pair<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-Niflect::TArray<float>");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(*(it++) == "std::pair<Niflect::CString, Niflect::TArray<bool> >");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-Niflect::TArray<bool>");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, Niflect::TArray<bool> >");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, Niflect::TArray<bool> >");
					ASSERT(*(it++) == "Niflect::TArray<Niflect::TMap<Niflect::CString, float> >");
					ASSERT(*(it++) == "+Niflect::TMap<Niflect::CString, float>");
					ASSERT(*(it++) == "std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-Niflect::TArray<Niflect::TMap<Niflect::CString, float> >");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > >");
					ASSERT(*(it++) == "TestAccessor2::TMyTransform<float>");
					ASSERT(*(it++) == "Niflect::TArray<Niflect::TMap<Niflect::CString, Niflect::TArray<float> > >");
					ASSERT(*(it++) == "+Niflect::TMap<Niflect::CString, Niflect::TArray<float> >");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 4);
					auto it = vecSignature.begin();
					//成员为TaggedType
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "TestMyFinding::CMyClass_1");
					ASSERT(*(it++) == "bool");
					ASSERT(*(it++) == "float");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					ASSERT(vecSignature.size() == 26);
					auto it = vecSignature.begin();
					//成员含TaggedType的综合测试
					ASSERT(*(it++) == "TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "TestMyFinding::CMyClass_1");
					ASSERT(*(it++) == "bool");
					ASSERT(*(it++) == "float");
					ASSERT(*(it++) == "Niflect::CString");
					ASSERT(*(it++) == "std::pair<Niflect::CString, TestMyFinding::CMyClass_0>");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "Niflect::TArray<TestMyFinding::CMyClass_0>");
					ASSERT(*(it++) == "+TestMyFinding::CMyClass_0");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0>");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, TestMyFinding::CMyClass_0>");
					ASSERT(*(it++) == "std::pair<Niflect::CString, Niflect::TArray<TestMyFinding::CMyClass_0> >");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-Niflect::TArray<TestMyFinding::CMyClass_0>");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, Niflect::TArray<TestMyFinding::CMyClass_0> >");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, Niflect::TArray<TestMyFinding::CMyClass_0> >");
					ASSERT(*(it++) == "Niflect::TArray<Niflect::TMap<Niflect::CString, Niflect::TArray<TestMyFinding::CMyClass_0> > >");
					ASSERT(*(it++) == "+Niflect::TMap<Niflect::CString, Niflect::TArray<TestMyFinding::CMyClass_0> >");
					ASSERT(*(it++) == "Niflect::TArray<Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0> >");
					ASSERT(*(it++) == "+Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0>");
					ASSERT(*(it++) == "std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0> > >");
					ASSERT(*(it++) == "-Niflect::CString");
					ASSERT(*(it++) == "-Niflect::TArray<Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0> >");
					ASSERT(*(it++) == "Niflect::TMap<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0> > >");
					ASSERT(*(it++) == "+std::pair<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, TestMyFinding::CMyClass_0> > >");
					ASSERT(it == vecSignature.end());
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
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 0);
					CResolvingContext resolvingContext(&log);
					CResolver resolver(collectionData, gen->GetModuleRegInfo());
					CResolvedData resolvedData;
					resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);
					ASSERT(log.m_vecText.size() == 0);
					Niflect::TArrayNif<Niflect::CString> vecSignature;
					resolvedData.m_signatureMapping.DebugGenSignatures(vecSignature);
					Niflect::TArrayNif<Niflect::CString> vecExpected;
					//1维容器模板套结构模板
					vecExpected.push_back("TestMyFinding::CMyClass_0");
					vecExpected.push_back("Niflect::CString");
					vecExpected.push_back("float");
					vecExpected.push_back("std::pair<Niflect::CString, float>");
					vecExpected.push_back("-Niflect::CString");
					vecExpected.push_back("-float");
					vecExpected.push_back("Niflect::TArray<std::pair<Niflect::CString, float> >");
					vecExpected.push_back("+std::pair<Niflect::CString, float>");
					uint32 idx = 0;
					for (auto& it : vecSignature)
						ASSERT(vecExpected[idx++] == it);
					ASSERT(vecSignature.size() == idx);
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
	}
}