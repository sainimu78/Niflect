#include "Test1BeingTestedHeader.h"
#include "Test1BeingTestedModule_private.h"
#include "Niflect/NiflectModuleRegistry.h"

int main()
{
	auto reg = Niflect::GetModuleRegistry();
	reg->InitRegisteredModules();
	printf("Modules count: %u", reg->GetModulesCount());
	return 0;
}