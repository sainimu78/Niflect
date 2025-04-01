#include <stdio.h>
#include "Engine/Engine.h"

//#include "Windows.h"

#include "Engine/Test/TestRegistration.h"
#include "Engine/Test/TestAccessor.h"
#include "Editor/Test/TestSerializationRwTree.h"
#include "TestModule0/TestModule0.h"
#include "Niflect/NiflectRegistrationOld.h"
#include "Niflect/Memory/Default/DefaultMemory.h"
#include "Engine/Test/TestAccessor2.h"
#include "Engine/Engine.h"
#include "Editor/Test/TestEdAsset.h"
//#include "Engine/DerivedObject.h"
//#include "Niflect/Serialization/JsonFormat.h"

class CMyClassForSimplifiedMakeShared
{
public:
	CMyClassForSimplifiedMakeShared()
		: m_float(1.23f)
		, m_bool(true)
		, m_double(43.21)
		, m_arr{}
	{

	}
	float m_float;
	bool m_bool;
	double m_double;
	int m_arr[128];
};

typedef void (*FuncTestAPI_C)();

int main(int argc, char** argv)
{
	TestAPI_Cpp();


	if (false)
	{
		TestEdAsset();
	}


	//TestSerializationRwTree::TestMemory();


	//TestAccessor::TestFieldLayout();
	

	//TestRegistration::TestTypeNatimeta();


	//TestModule0::LoadTypes();
	//TestModule0::UnloadTypes();


	if (false)
	{
		TestSerializationRwTree::TestSerialization();
	}

	if (true)
	{
		auto memTest = Niflect::GetDefaultMemoryStats();
		TestEngineCreate();
		TestEngineRun();
		TestEngineDestroy();
	}


	//TestAccessor2::TestFieldLayout();


	//{
	//	auto hLib = LoadLibrary("Engine");
	//	if (hLib != NULL)
	//	{
	//		if (auto Func = reinterpret_cast<FuncGeneratedNiflectRegistrationGet>(GetProcAddress(hLib, "GeneratedNiflectRegistrationGet")))
	//			Func();
	//	}
	//}
	//{
	//	auto hLib = LoadLibrary("TestModule0");
	//	if (hLib != NULL)
	//	{
	//		if (auto Func = reinterpret_cast<FuncGeneratedNiflectRegistrationGet>(GetProcAddress(hLib, "GeneratedNiflectRegistrationGet")))
	//			Func();
	//		if (auto Func = reinterpret_cast<FuncGeneratedNiflectRegistrationRelease>(GetProcAddress(hLib, "GeneratedNiflectRegistrationRelease")))
	//			Func();
	//		FreeLibrary(hLib);
	//	}
	//}

	printf("Finished\n");
	fflush(stdout);
	return 0;
}