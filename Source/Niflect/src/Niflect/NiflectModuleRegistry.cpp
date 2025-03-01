#include "Niflect/NiflectModuleRegistry.h"

namespace Niflect
{
	void CNiflectModuleRegistry::InitRegisteredModules()
	{
		for (auto& it0 : m_vecModule)
			it0.RegisterTypes();
		for (auto& it0 : m_vecModule)
			it0.InitTypes();
		for (auto& it0 : m_vecModule)
			it0.InitTableTypesLayout();
	}
	bool CNiflectModuleRegistry::RegisterModuleStatically(const Niflect::CString& name, const ModuleRegisterTypesFunc& RegisterTypesFunc, const ModuleInitTypesFunc& InitTypesFunc)
	{
		uint32 idx = this->GetModulesCount();
		m_vecModule.push_back(CNiflectModule2());
		auto& module = m_vecModule.back();
		module.m_indexInManager = idx;
		module.InitMeta(name, RegisterTypesFunc, InitTypesFunc);
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
		//�˷�ʽ���ú����ڵľ�̬��������ȫ�־�̬�����ͷ�ȷ������ MemoryStats ����ڴ�ǰ�ͷ�
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