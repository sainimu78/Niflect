#pragma once
#include "Niflect/Default/DefaultMacroTag.h"
#include "Engine/Test/TestMyResource.h"

namespace TestMyFinding
{
	NIF_T()
	class CMyClass_0
	{
	public:
		CMyClass_0()
		{
		}

		NIF_F()
		TestMyScope::TSharedMyResource<TestMyScope::CMyResource> m_sharedResource;
	};
}