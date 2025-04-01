#include "Engine/Engine.h"
#include <stdio.h>

void TestAPI_Cpp()
{
	printf("Invoke C++ API\n");
	fflush(stdout);
}
void TestAPI_C()
{
	printf("Invoke C API\n");
	fflush(stdout);
}

#ifdef ENABLED_TEST_MODULEREG_GEN_BY_NGT
#include "TestEngine_private.h"
#include "Niflect/NiflectTable.h"
#include "Engine/DerivedObject.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "TestModule1/TestModule1.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

Niflect::TSharedPtr<Niflect::CNiflectTable> g_defaultTable;

void TestEngineCreate()
{
	TestModule1Create();

	g_defaultTable = Niflect::MakeShared<Niflect::CNiflectTable>();
	Niflect::GeneratedRegisterTypes(g_defaultTable.Get());
	Niflect::GeneratedInitTypes();
	g_defaultTable->InitTypesLayout();
}
void TestEngineRun()
{
	{
		using CTestType = Engine::CDerivedObject;
		auto type = Niflect::StaticGetType<CTestType>();
		RwTree::CRwNode rw;
		CTestType srcData;
		{
			srcData.InitForTest();
			type->SaveInstanceToRwNode(&srcData, &rw);
		}
		auto instance = Niflect::NiflectTypeMakeShared<CTestType>(type);
		CTestType& dstData = *instance;
		type->LoadInstanceFromRwNode(&dstData, &rw);
		ASSERT(srcData == dstData);
		Niflect::CStringStream ss;
		RwTree::CJsonFormat::Write(&rw, ss);
		printf("%s\n", ss.str().c_str());
	}
	{
		TestModule1::CreateTestResourceFactory();
		using CTestType = Engine::CDerivedFromModule1;
		auto type = Niflect::StaticGetType<CTestType>();
		RwTree::CRwNode rw;
		CTestType srcData;
		{
			srcData.InitForTest();
			type->SaveInstanceToRwNode(&srcData, &rw);
		}
		auto instance = Niflect::NiflectTypeMakeShared<CTestType>(type);
		CTestType& dstData = *instance;
		type->LoadInstanceFromRwNode(&dstData, &rw);
		ASSERT(srcData == dstData);
		Niflect::CStringStream ss;
		RwTree::CJsonFormat::Write(&rw, ss);
		printf("%s\n", ss.str().c_str());
		TestModule1::DestroyTestResourceFactory();
	}
	{
		using CTestType = TestModule1::SMyRecord;
		auto type = Niflect::StaticGetType<CTestType>();
		RwTree::CRwNode rw;
		CTestType srcData;
		{
			TestModule1::InitForTest_SMyRecord(srcData);
			type->SaveInstanceToRwNode(&srcData, &rw);
		}
		auto instance = Niflect::NiflectTypeMakeShared<CTestType>(type);
		CTestType& dstData = *instance;
		type->LoadInstanceFromRwNode(&dstData, &rw);
		ASSERT(srcData == dstData);
		Niflect::CStringStream ss;
		RwTree::CJsonFormat::Write(&rw, ss);
		printf("%s\n", ss.str().c_str());
	}
	{
		auto type = Niflect::CEnum::Cast(Niflect::StaticGetType<TestModule1::ETestEnum0>());
		auto& em = type->GetEnumMeta();
		Niflect::TArray<Niflect::CString> vecExpected;
		vecExpected.push_back("MyDefault");
		vecExpected.push_back("Nihao opt 0");
		vecExpected.push_back("Option1");
		vecExpected.push_back("Option2");
		uint32 idx = 0;
		for (auto& it : em.m_vecEnumConstMeta)
		{
			Niflect::CString name = it.m_name;
			if (auto nata = it.GetNata())
			{
				auto ecn = TestModule1::CMyEnumConstNata::Cast(nata);
				name = ecn->m_friendlyName;
			}
			ASSERT(name == vecExpected[idx++]);
			printf("%s\n", name.c_str());
		}
		printf("");
	}
}
void TestEngineDestroy()
{
	TestModule1Destroy();
	g_defaultTable = NULL;
}
#else
void TestEngineCreate()
{
}
void TestEngineRun()
{
}
void TestEngineDestroy()
{
}
#endif