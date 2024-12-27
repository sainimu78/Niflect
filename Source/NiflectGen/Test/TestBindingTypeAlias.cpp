#include "NiflectGen/Test/TestBindingTypeAlias.h"
#include "NiflectGen/Test/TestGen.h"
#include "NiflectGen/Generator/Generator.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "Niflect/Memory/Default/DefaultMemory.h"
#include "NiflectGen/Collector/Collector.h"

namespace TestGen
{
	using namespace NiflectGen;
	static void TestFailure_BindingTypeAlias0()
	{
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
		auto memTest = Niflect::GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			CModuleRegInfo info;
			info.m_vecBindingSettingHeader.push_back(CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/TestAccessorSettingBindingTypeAlias0.h"));
			NiflectGenDefinition::Test::AddBasicHeaderSearchPaths(info.m_vecHeaderSearchPath);
			gen->SetModuleRegInfo(info);
			gen->Generate([&info](void* cursorAddr)
				{
					auto& cursor = *static_cast<CXCursor*>(cursorAddr);
					CTaggedNode2 taggedRoot;
					CGenLog log;
					log.Config(CGenLogOption().SetAssertionOnAddingItem(false).SetCachedItems(true));
					CCollectingContext context(&log);
					CCollectionData collectionData;
					CDataCollector collector;
					collector.Collect(cursor, &taggedRoot, context, collectionData);
					ASSERT(log.m_vecText.size() == 3);
				});
		}
#else
#endif
	}
	void TestFailure_BindingTypeAlias()
	{
		TestFailure_BindingTypeAlias0();
	}
}