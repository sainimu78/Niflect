#include "TestModule1/TestResourceFactory.h"

namespace TestModule1
{
	Niflect::TSharedPtr<CTestResourceFactory> g_factory;
	void CreateTestResourceFactory()
	{
		ASSERT(g_factory == NULL);
		g_factory = Niflect::MakeShared<CTestResourceFactory>();
	}
	void DestroyTestResourceFactory()
	{
		g_factory = NULL;
	}
	CTestResourceFactory* GetTestResourceFactory()
	{
		return g_factory.Get();
	}
}