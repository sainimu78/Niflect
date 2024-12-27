#include "Niflect/NiflectBase.h"
#include "Niflect/Memory/Default/DefaultMemory.h"

namespace Niflect
{
	thread_local IAllocatorInterface* g_currentAllocator = NULL;

	void* CMemory::Alloc(size_t size)
	{
		if (g_currentAllocator == NULL)
			return CDefaultMemory::Alloc(size);
		return g_currentAllocator->Alloc(size);
	}
	void* CMemory::Realloc(void* ptr, size_t size)
	{
		if (g_currentAllocator == NULL)
			return CDefaultMemory::Realloc(ptr, size);
		return g_currentAllocator->Realloc(ptr, size);
	}
	void CMemory::Free(void* ptr)
	{
		if (g_currentAllocator == NULL)
		{
			CDefaultMemory::Free(ptr);
			return;
		}
		g_currentAllocator->Free(ptr);
	}
	void CMemory::SetCurrentAllocator(IAllocatorInterface* p)
	{
		ASSERT(p == NULL || g_currentAllocator == NULL);
		g_currentAllocator = p;
	}
}