#pragma once
#include "TestModule1/TestCommonTypes_gen.h"

namespace TestModule1
{
	NIF_T()
	class CMyClassInPrecompileHeader
	{
		NIFGNSD()
	public:
		CMyClassInPrecompileHeader()
			: m_value_0(0.0f)
		{
		}
		void InitForTest()
		{
			m_value_0 = 1.25f;
		}
		bool operator==(const CMyClassInPrecompileHeader& rhs) const
		{
			return m_value_0 == rhs.m_value_0
				;
		}

	private:
		NIF_F()
		float m_value_0;
	};
}