#include "TestLib.h"
#include "TestLib_private.h"

namespace MyGlobalScope
{
	float g_a = 0.0f;

	void MyGlobalFunc3(float a)
	{
		static int cnt = 0;
		printf("MyGlobalFunc3, %f, %d\n", a, ++cnt);
	}
}

void MyGlobalFunc2(float a)
{
	static int cnt = 0;
	printf("MyGlobalFunc2, %f, %d\n", a, ++cnt);
}

void InitTestLib(Niflect::CNiflectTable& table)
{
	Niflect::GeneratedRegisterTypes(&table);
	Niflect::GeneratedInitTypes();
	table.InitTypesLayout();
}
Niflect::CNiflectType* GetTestLibGlobalsType()
{
	return Niflect::_TestLib_StaticGetType<Niflect::_TestLib_CGlobals>();
}
void TestLibSetGlobalVar0(float val)
{
	MyGlobalScope::g_a = val;
}