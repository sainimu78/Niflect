#pragma once
#include "TestTypes1_gen.h"

NIF_T()
class CPrivateFields
{
	NIFGNSD()
public:
	CPrivateFields()
		: m_float_0(0.0f)
		, m_bool_1(false)
	{
	}
	void InitForTest()
	{
		m_float_0 = 1.23f;
		m_bool_1 = true;
	}
	bool operator==(const CPrivateFields& rhs) const
	{
		return m_float_0 == rhs.m_float_0
			&& m_bool_1 == rhs.m_bool_1
			;
	}
private:
	NIF_F()
	float m_float_0;
	NIF_F()
	bool m_bool_1;
};