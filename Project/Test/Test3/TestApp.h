#pragma once
#include "TestApp_gen.h"

NIF_T()
class CTestApp
{
public:
	CTestApp()
		: m_float_0(0.0f)
	{
	}
	void InitForTest()
	{
		m_float_0 = 1.23f;
	}
	bool operator==(const CTestApp& rhs) const
	{
		return m_float_0 == rhs.m_float_0
			;
	}

public:
	NIF_F()
	float m_float_0;
};