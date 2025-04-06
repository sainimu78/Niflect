#pragma once
#include "Niflect/NiflectMacroTag.h"

namespace TestMyFinding
{
	NIF_T()
	enum class EMyOption
	{
		EDefaultMode NIF_C(),
		EMode0 NIF_C(),
		EState0 NIF_C(),
	};

	NIF_T()
	class CMyClass_0
	{
	public:
		CMyClass_0()
			: m_ec_opt_6(EMyOption::EDefaultMode)
		{

		}

		NIF_F()
		EMyOption m_ec_opt_6;
	};

	NIF_T()
	class CMyClass_1
	{
	public:
		CMyClass_1()
			: m_float_0(0.0f)
		{

		}

		NIF_F()
		float m_float_0;
	};
}
