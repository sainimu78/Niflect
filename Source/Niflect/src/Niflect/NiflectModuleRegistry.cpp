#include "Niflect/NiflectModuleRegistry.h"

namespace Niflect
{
	void CNiflectModuleRegistry::InitRegisteredModules()
	{
		for (auto& it0 : m_vecModule)
			it0->RegisterTypes();
		for (auto& it0 : m_vecModule)
			it0->InitTypes();
		for (auto& it0 : m_vecModule)
			it0->BuildTableTypesMeta();
	}
	bool CNiflectModuleRegistry::RegisterModuleStatically(const Niflect::CString& name, const ModuleRegisterTypesFunc& RegisterTypesFunc, const ModuleInitTypesFunc& InitTypesFunc)
	{
		uint32 idx = this->GetModulesCount();
		auto shared = Niflect::MakeShared<CNiflectModule2>();
		auto& module = *shared;
		module.InitMeta(name, idx, RegisterTypesFunc, InitTypesFunc);
		m_vecModule.push_back(shared);
		return true;
	}
	uint32 CNiflectModuleRegistry::GetModulesCount() const
	{
		return static_cast<uint32>(m_vecModule.size());
	}

	using CSharedModuleRegistry = Niflect::TSharedPtr<CNiflectModuleRegistry>;
	static CSharedModuleRegistry* g_mgrPtr = NULL;
	CNiflectModuleRegistry* CNiflectModuleRegistry::InitializeSingleton()
	{
		//此方式借用函数内的静态变量先于全局静态变量释放确保可在 MemoryStats 检查内存前释放
		static CSharedModuleRegistry g_reg = Niflect::MakeShared<CNiflectModuleRegistry>();
		ASSERT(g_mgrPtr == NULL);
		g_mgrPtr = &g_reg;
		return g_reg.Get();
	}
	void CleanupModuleRegistry_reserved()
	{
		*g_mgrPtr = NULL;
		g_mgrPtr = NULL;
	}
	CNiflectModuleRegistry* GetModuleRegistry()
	{
		if (g_mgrPtr != NULL)
			return g_mgrPtr->Get();
		return NULL;
	}
}