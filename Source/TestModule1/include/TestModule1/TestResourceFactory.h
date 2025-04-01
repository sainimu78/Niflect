#pragma once
#include "TestModule1Common.h"
#include "TestModule1/TestResource.h"
#include "Niflect/Base/Map.h"
#include "Niflect/Base/String.h"
#include "Niflect/Base/Array.h"

namespace TestModule1
{
	class CTestResourceFactory
	{
	public:
		~CTestResourceFactory()
		{
			for (auto& it : m_vecRawResource)
				Niflect::CInstance::Delete(it);
		}
		CSharedTestResource FindOrAddShared(const Niflect::CString& path)
		{
			CSharedTestResource resource;
			auto ret = m_mapPathToSharedResource.insert({ path, NULL });
			if (ret.second)
			{
				resource = Niflect::MakeShared<CTestResource>();
				ret.first->second = resource;
				resource->InitForTest(path);
			}
			else
			{
				resource = ret.first->second;
			}
			return resource;
		}
		CTestResource* FindOrAddRaw(const Niflect::CString& path)
		{
			auto shared = this->FindOrAddShared(path);
			return shared.Get();
		}
		CTestResource** FindOrAddRawPtrPtr(const Niflect::CString& path)
		{
			CTestResource** resourcePtrPtr = NULL;
			auto ret = m_mapPathToRawResourceIndex.insert({ path, static_cast<uint32>(m_vecRawResource.size())});
			if (ret.second)
			{
				m_vecRawResource.push_back(Niflect::CInstance::New<CTestResource>());
				auto& resource = m_vecRawResource.back();
				resource->InitForTest(path);
				resourcePtrPtr = &resource;
			}
			else
			{
				resourcePtrPtr = &m_vecRawResource[ret.first->second];
			}
			return resourcePtrPtr;
		}

	private:
		Niflect::TMap<Niflect::CString, CSharedTestResource> m_mapPathToSharedResource;
		Niflect::TMap<Niflect::CString, uint32> m_mapPathToRawResourceIndex;
		Niflect::TArray<CTestResource*> m_vecRawResource;
	};

	TESTMODULE1_API void CreateTestResourceFactory();
	TESTMODULE1_API void DestroyTestResourceFactory();
	TESTMODULE1_API CTestResourceFactory* GetTestResourceFactory();
}