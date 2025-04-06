#pragma once
#include "Niflect/Default/DefaultMacroTag.h"
#include "Niflect/NiflectBase.h"
#include "Engine/Test/TestMyTransform.h"

namespace TestMyFinding
{
	using namespace Niflect;

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
			: m_float_0(0.0f)
			, m_bool_1(false)
			, m_int32_3(0)
			, m_ec_opt_6(EMyOption::EDefaultMode)
		{

		}

		NIF_F()
		float m_float_0;
		NIF_F()
		bool m_bool_1;
		NIF_F()
		CString m_string_2;
		NIF_F()
		int32 m_int32_3;
		NIF_F()
		TestAccessor2::TMyTransform<float> m_tm_4;
		NIF_F()
		Niflect::CString m_string_5;
		NIF_F()
		EMyOption m_ec_opt_6;
	};
}