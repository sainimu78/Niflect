#pragma once
#include "Niflect/NiflectModuleInfo.h"

namespace Niflect
{
	class CNiflectModule
	{
	public:
		CNiflectModule()
			: m_nativeHandle(NULL)
			, m_info(NULL)
		{
		}
		void Init(void* nativeHandle, CNiflectModuleInfo* info)
		{
			m_nativeHandle = nativeHandle;
			m_info = info;
		}

	public:
		CNiflectModuleInfo* m_info;

	private:
		void* m_nativeHandle;
	};

	class CNiflectModuleManager
	{
	public:
		NIFLECT_API bool RegisterStaticlyLoadedModule(const Niflect::CString& moduleName);
		uint32 GetModulesCount() const
		{
			return static_cast<uint32>(m_vecModule.size());
		}
		const CNiflectModule& GetModule(uint32 idx) const
		{
			return m_vecModule[idx];
		}

	private:
		Niflect::TArrayNif<CNiflectModule> m_vecModule;
		Niflect::TMap<Niflect::CString, uint32> m_mapNameToIdx;
	};

	class CNiflectModule2
	{
		friend class CNiflectModuleManager2;
	public:
		CNiflectModule2()
			: m_indexInManager(INDEX_NONE)
		{
		}

	private:
		CNiflectModuleInfo m_info;
		uint32 m_indexInManager;
	};

	class CNiflectModuleManager2
	{
		friend class CNiflectModuleRegger;
	public:
		NIFLECT_API void InitRegisteredModules();
		NIFLECT_API uint32 GetModulesCount() const;

	private:
		NIFLECT_API bool RegisterModuleStatically(const Niflect::CString& name, const ModuleRegisterTypesFunc& RegisterTypesFunc, const ModuleInitTypesFunc& InitTypesFunc);
		NIFLECT_API static CNiflectModuleManager2* InitializeSingleton();

	private:
		Niflect::TArrayNif<CNiflectModule2> m_vecModule;
	};

	NIFLECT_API CNiflectModuleManager2* GetModuleManager();
	NIFLECT_API void CleanupModuleManager();

	class CNiflectModuleRegger
	{
	public:
		CNiflectModuleRegger(const Niflect::CString& name, const ModuleRegisterTypesFunc& RegisterTypesFunc, const ModuleInitTypesFunc& InitTypesFunc)
		{
			auto mgr = GetModuleManager();
			if (mgr == NULL)
				mgr = CNiflectModuleManager2::InitializeSingleton();
			mgr->RegisterModuleStatically(name, RegisterTypesFunc, InitTypesFunc);
		}
	};
}