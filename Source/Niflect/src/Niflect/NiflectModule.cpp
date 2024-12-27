#include "Niflect/NiflectModule.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace Niflect
{
	static Niflect::CString ConvertToLibName(const Niflect::CString& moduleName)
	{
		Niflect::CString result;
#ifdef WIN32
		result = moduleName + ".dll";
#else
		result = "lib" + moduleName + ".so";
#endif
		return result;
	}

	bool CNiflectModuleManager::RegisterStaticlyLoadedModule(const Niflect::CString& moduleName)
	{
		bool ok = false;
		auto ret = m_mapNameToIdx.insert({ moduleName, this->GetModulesCount()});
		ASSERT(ret.second);
		auto libName = ConvertToLibName(moduleName);
		void* nativeHandle = NULL;
		auto getInfoFuncName = Niflect::GetGeneratedGetModuleInfoFuncName(moduleName);
		Niflect::GeneratedGetModuleInfoFunc Func = NULL;
#ifdef WIN32
		auto moduleHandle = GetModuleHandle(libName.c_str());
		if (moduleHandle != NULL)
		{
			nativeHandle = moduleHandle;
			Func = reinterpret_cast<Niflect::GeneratedGetModuleInfoFunc>(GetProcAddress(moduleHandle, getInfoFuncName.c_str()));
		}
#else
		//测试时须确保在相应 cmake 中指定 target_link_libraries(${ ModuleName } PRIVATE dl)
		auto moduleHandle = dlopen(libName.c_str(), RTLD_LAZY);
		if (moduleHandle != NULL)
		{
			nativeHandle = moduleHandle;
			Func = reinterpret_cast<Niflect::GeneratedGetModuleInfoFunc>(dlsym(moduleHandle, getInfoFuncName.c_str()));
		}
#endif
		if (Func != NULL)
		{
			CNiflectModule md;
			md.Init(nativeHandle, Func());
			m_vecModule.push_back(md);
			ok = true;
		}
		else
		{
			ASSERT(false);
		}
		return ok;
	}
}