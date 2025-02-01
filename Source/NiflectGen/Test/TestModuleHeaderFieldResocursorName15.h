#pragma once
#include "Niflect/NiflectMacro.h"
#include "Engine/Test/TestMyResource.h"
#include "Engine/Test/TestMyAsset.h"

namespace TestMyFinding
{
	NIF_T()
	class CMyClass_0
	{
	public:
		CMyClass_0()
			: m_resource(NULL)
		{
		}

		NIF_F()
		TestMyScope::CMyResource* m_resource;
		NIF_F()
		Niflect::TArray<TestMyScope::CMyAsset*> m_vecAsset;
	};
}