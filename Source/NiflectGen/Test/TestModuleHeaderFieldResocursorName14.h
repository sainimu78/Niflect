#pragma once
#include "Niflect/Default/DefaultMacroTag.h"
#include "Niflect/NiflectBase.h"

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
		Niflect::TArray<std::pair<Niflect::CString, float> > m_array_pair_string_float_0;
	};
}