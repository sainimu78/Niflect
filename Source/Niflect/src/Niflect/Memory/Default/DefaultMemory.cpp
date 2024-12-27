#include "Niflect/Memory/Default/DefaultMemory.h"
#include "Niflect/Memory/Generic/GenericMemory.h"
#include "Niflect/Memory/Generic/GenericSharedPtr.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

#include "Niflect/Memory/Default/DefaultMemoryPoolScope.h"
#include "Niflect/NiflectBase.h"
#include "Niflect/Util/DebugUtil.h"

namespace Niflect
{
	thread_local CMemoryStats* g_memoryStatsThreadLocal = NULL;
	
	void threadFunction()
	{
		CDefaultMemoryStatsScope memStatsScope;
		auto memTest = GetDefaultMemoryStats();
		{
			CDefaultMemoryPoolScope sssss;
			auto mem = CMemory::Alloc(123);
			CMemory::Free(mem);
		}
		std::stringstream ss;
		ss << memTest;
		NiflectUtil::Printf("%s\n", ss.str().c_str());
	}

	std::vector<std::thread> threads;
	void TestMemoryStatsOnThreadsBegin()
	{
		for (int i = 0; i < 5; ++i)
		{
			threads.emplace_back(threadFunction);
		}
	}
	void TestMemoryStatsOnThreadsEnd()
	{
		for (auto& t : threads)
		{
			t.join();
		}
		threads.clear();
	}

	//无可配置方法, 须手工修改
	static const bool g_enabledGlobalMemoryStats_MainThreadOnly_IncludingStaticStage = true;

	static void MakeSureInitMemoryStats()
	{
		if (!g_enabledGlobalMemoryStats_MainThreadOnly_IncludingStaticStage)
			return;
		static CDefaultMemoryStatsScope s_holder;
	}
	
	void* CDefaultMemory::Alloc(size_t size)
	{
		MakeSureInitMemoryStats();
		return CGenericMemory::Alloc(size, g_memoryStatsThreadLocal);
	}
	void* CDefaultMemory::Realloc(void* ptr, size_t size)
	{
		MakeSureInitMemoryStats();
		return CGenericMemory::Realloc(ptr, size, g_memoryStatsThreadLocal);
	}
	void CDefaultMemory::Free(void* ptr)
	{
		CGenericMemory::Free(ptr, g_memoryStatsThreadLocal);
	}
	void CDefaultMemory::PushStats(CMemoryStats* stats, CMemoryStats*& lastStats)
	{
		lastStats = g_memoryStatsThreadLocal;
		g_memoryStatsThreadLocal = stats;
	}
	void CDefaultMemory::PopStats(CMemoryStats* lastStats)
	{
		g_memoryStatsThreadLocal = lastStats;
	}

	CMemoryStats* GetDefaultMemoryStats()
	{
		MakeSureInitMemoryStats();
		return g_memoryStatsThreadLocal;
	}
	
	static void TestReserved()
	{
		auto memoryTest = g_memoryStatsThreadLocal;
		auto p0 = CDefaultMemory::Alloc(4);
		ASSERT((memoryTest = g_memoryStatsThreadLocal)->m_allocatedBytesRuntimeTotal == 4);
		auto p1 = CDefaultMemory::Alloc(6);
		ASSERT((memoryTest = g_memoryStatsThreadLocal)->m_allocatedBytesRuntimeTotal == 10);
		p0 = CDefaultMemory::Realloc(p0, 6);
		ASSERT((memoryTest = g_memoryStatsThreadLocal)->m_allocatedBytesRuntimeTotal == 12);
		p1 = CDefaultMemory::Realloc(p1, 4);
		ASSERT(((memoryTest = g_memoryStatsThreadLocal)->m_allocatedBytesRuntimeTotal == 12) && ((memoryTest = g_memoryStatsThreadLocal)->m_freedBytesRuntimeTotal == 2));
		auto p2 = CDefaultMemory::Alloc(3);
		ASSERT((memoryTest = g_memoryStatsThreadLocal)->m_allocatedBytesRuntimeTotal == 15);
		CDefaultMemory::Free(p0);
		ASSERT((memoryTest = g_memoryStatsThreadLocal)->m_freedBytesRuntimeTotal == 8);
		CDefaultMemory::Free(p1);
		ASSERT((memoryTest = g_memoryStatsThreadLocal)->m_freedBytesRuntimeTotal == 12);
		CDefaultMemory::Free(p2);
		auto expectedBytesCount = 3*sizeof(size_t) + 6 + 4 + 3 + 2;//sizeof(size_t)表示每块分配的内存信息头字节数; 6, 4, 3分别为p0, p1, p2释放之前的字节数; 2为p1从6 Realloc为4后, 逻辑上被释放的字节数
		ASSERT(((memoryTest = g_memoryStatsThreadLocal)->m_freedBytesRuntimeTotal == 15) && ((memoryTest = g_memoryStatsThreadLocal)->m_freeCount == 3) && ((memoryTest = g_memoryStatsThreadLocal)->m_freedBytesTotal == expectedBytesCount));
	}
}