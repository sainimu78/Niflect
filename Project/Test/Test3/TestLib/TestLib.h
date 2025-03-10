#pragma once
#include "Niflect/NiflectTable.h"
#include "TestLib_gen.h"

NIF_T()
class CTestLib
{
public:
	CTestLib()
		: m_float_0(0.0f)
	{
	}
	void InitForTest()
	{
		m_float_0 = 1.23f;
	}
	bool operator==(const CTestLib& rhs) const
	{
		return m_float_0 == rhs.m_float_0
			;
	}

public:
	NIF_F()
	float m_float_0;
};

#ifdef TESTLIB_API
#else
#ifdef WIN32
#ifdef TESTLIB_EXPORTS
#define TESTLIB_API __declspec(dllexport)
#else
#define TESTLIB_API __declspec(dllimport)
#endif
#else
#ifdef TESTLIB_EXPORTS
#define TESTLIB_API __attribute__((visibility("default")))
#else
#define TESTLIB_API 
#endif
#endif
#endif

TESTLIB_API void InitTestLib(Niflect::CNiflectTable& table);