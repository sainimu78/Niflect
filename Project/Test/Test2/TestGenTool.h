#pragma once
#include "TestGenTool_gen.h"

NIF_T()
class CTest2Class
{
public:
	CTest2Class()
		: m_float_0(0.0f)
		, m_bool_1(false)
	{
	}
	void InitForTest()
	{
		m_float_0 = 1.23f;
		m_bool_1 = true;
	}
	bool operator==(const CTest2Class& rhs) const
	{
		return m_float_0 == rhs.m_float_0
			&& m_bool_1 == rhs.m_bool_1
			;
	}
public:
	NIF_F()
	float m_float_0;
	NIF_F()
	bool m_bool_1;
};