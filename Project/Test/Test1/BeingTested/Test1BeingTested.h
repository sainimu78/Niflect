#pragma once
#include "Test1BeingTested_gen.h"

NIF_T()
class CTest1_Class_0
{
	GENERATED_BODY()
public:
	CTest1_Class_0()
		: m_float_0(0.0f)
	{
	}
	void InitForTest()
	{
		m_float_0 = 1.23f;
	}
	bool operator==(const CTest1_Class_0& rhs) const
	{
		return m_float_0 == rhs.m_float_0
			;
	}

private:
	NIF_F()
	float m_float_0;
};