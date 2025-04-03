#pragma once
#include "Niflect/NiflectTable.h"
#include "Niflect/Base/Map.h"
#include "TestLib_gen.h"

//NIF_T()
//struct STestBase
//{
//
//};
//
//NIF_T()
//struct STestLib : STestBase
//{
//
//};

NIF_T()
class CTestBase
{
	GENERATED_BODY()
};

NIF_T()
class CTestLib : public CTestBase
{
	GENERATED_BODY()
public:
	CTestLib()
		: m_float_0(0.0f)
		, m_string_2(NULL)
	{
	}
	//CTestLib(Niflect::TArray<float> a, const Niflect::TArray<bool>& b, Niflect::CString, Niflect::TMap<Niflect::CString, float>)
	//	: m_float_0(0.0f)
	//	, m_string_2(NULL)
	//{
	//}
	//NIF_M()
	//CTestLib(const float& a)
	//	: m_float_0(0.0f)
	//	, m_string_2(NULL)
	//{
	//}
	//NIF_M()
	//Niflect::CString FuncB(
	//	//Niflect::TMap<Niflect::CString, float> a,
	//	//Niflect::TArray<Niflect::CString> b,
	//	//Niflect::CString& c,
	//	//const Niflect::CString& d,
	//	//Niflect::CString* e,
	//	const Niflect::CString* f//,
	//	//Niflect::TArray<float>& g,
	//	//Niflect::TArray<float>* h,
	//	//const Niflect::TArray<float>* i,
	//	//const Niflect::TArray<Niflect::TMap<int32, Niflect::CString> >* j
	//)
	//{
	//	return "";
	//}
	
private:
	void Nihao()
	{
	}

public:
	//NIF_M()
	//void FuncA(Niflect::TArray<float> a, const Niflect::TArray<bool>& b, Niflect::CString, Niflect::TMap<Niflect::CString, float>)
	//{
	//}
	//NIF_M()
	//Niflect::CString FuncB(Niflect::TArray<float> a, const Niflect::TArray<bool>& b, Niflect::CString, Niflect::TMap<Niflect::CString, float>)
	//{
	//	return "";
	//}
	//float FuncC(Niflect::TArray<float> a, const Niflect::TArray<bool>& b, Niflect::CString, Niflect::TMap<Niflect::CString, float>)
	//{
	//}
	//const Niflect::CString& FuncD(Niflect::TArray<float> a, const Niflect::TArray<bool>& b, Niflect::CString, Niflect::TMap<Niflect::CString, float>)
	//{
	//	static Niflect::CString s;
	//	return s;
	//}
	//void FuncD() const
	//{

	//}
	//void FuncE(float*, Niflect::CString*, Niflect::TArray<float>**)
	//{

	//}
	//float* FuncF() const
	//{
	//	return NULL;
	//}
	//Niflect::CString* FuncG()
	//{
	//	return NULL;
	//}
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
	//NIF_F()
	//Niflect::TArray<float> m_array_float_1;
	NIF_F()
	Niflect::CString* m_string_2;
	NIF_F()
	float m_float_0;
	NIF_F()
	Niflect::TArray<float> m_array_float_3;
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