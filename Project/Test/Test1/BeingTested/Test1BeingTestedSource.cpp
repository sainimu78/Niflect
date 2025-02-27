#include "Test1BeingTestedHeader.h"
#include "Test1BeingTestedModule_private.h"
#include "Niflect/NiflectModuleManager.h"

int main()
{
	auto mgr = Niflect::GetModuleManager();
	mgr->InitRegisteredModules();
	printf("Modules count: %u", mgr->GetModulesCount());
	Niflect::CleanupModuleManager();
	return 0;
}