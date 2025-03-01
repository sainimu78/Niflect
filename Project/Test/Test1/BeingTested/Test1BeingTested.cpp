#include "Test1BeingTested.h"
#include "Test1BeingTestedModule_private.h"
#include "Niflect/NiflectModuleRegistry.h"

int main()
{
	auto reg = Niflect::GetModuleRegistry();
	reg->InitRegisteredModules();
	auto type = Niflect::StaticGetType<CTest1_Class_0>();
	ASSERT(type != NULL);
	auto table = type->GetTable();
	auto module = table->GetModule();
	auto moduleName = module->GetName();
	auto typeName = type->GetTypeName();
	ASSERT(moduleName == "Test1BeingTestedModule");
	ASSERT(typeName == "CTest1_Class_0");
	printf("Modules count: %u", reg->GetModulesCount());
	return 0;
}