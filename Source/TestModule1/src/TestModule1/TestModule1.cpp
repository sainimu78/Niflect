#include "TestModule1/TestModule1.h"
#include "TestModule1_private.h"
#include "Niflect/NiflectTable.h"

using namespace Niflect;

static Niflect::TSharedPtr<CNiflectTable> g_defaultTable;

void TestModule1Create()
{
	g_defaultTable = Niflect::MakeShared<CNiflectTable>();
	GeneratedRegisterTypes(g_defaultTable.Get());
	GeneratedInitTypes();
	g_defaultTable->BuildTypesMeta();
}
void TestModule1Destroy()
{
	g_defaultTable = NULL;
}