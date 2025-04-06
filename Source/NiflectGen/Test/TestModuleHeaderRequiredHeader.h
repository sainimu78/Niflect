#pragma once
#include "Niflect/Default/DefaultMacroTag.h"
#include "Niflect/NiflectBase.h"
#include "Engine/Test/TestMyTransform.h"
#include "NiflectGen/Test/TestSomeTypes.h"

namespace MyTestType
{
	template <typename T>
	class TMyArray
	{
	public:
		TMyArray()
			: m_item{}
			, m_count(0)
		{

		}
		void push_back(T item)
		{
			m_item[m_count++] = item;
		}
		T& operator[](uint32 idx)
		{
			return m_item[idx];
		}
		T m_item[16];
		uint32 m_count;
	};
}

namespace TestMyFinding
{
	using namespace Niflect;
	using namespace MyTestType;

	NIF_T()
	class CMyClass_0
	{
	public:
		CMyClass_0()
			: m_float_0(0.0f)
			, m_tm_2{}
		{

		}

		NIF_F()
		float m_float_0;
		NIF_F()
		Niflect::CString m_string_1;
		NIF_F()
		TestAccessor2::TMyTransform<float> m_tm_2;
		NIF_F()
		std::string m_std_string_3;
		NIF_F()
		TMyArray<float> m_my_array_4;
		NIF_F()
		TestSomeScope::EMyEnumClassOption m_ec_opt_5;
	};
}