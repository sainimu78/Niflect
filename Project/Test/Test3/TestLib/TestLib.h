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

namespace TestLibScope
{
	NIF_T()
	class CTestBase
	{
	};

	NIF_T()
	enum class ETestOption
	{
		Opt0
	};

	class CMyMethodNata : public Niflect::CNata
	{
	public:
		CMyMethodNata()
			: m_value(0.0f)
		{
		}
		CMyMethodNata& SetExampleValue(float a) { m_value = a; return *this; }
		float m_value;
	};

	NIF_T()
	class CTestLib : public CTestBase
	{
		NIFGNSD()
	public:
		//CTestLib()
		//{
		//}
		//CTestLib()
		//	: m_float_0(0.0f)
		//	, m_string_2(NULL)
		//{
		//}
		//CTestLib(Niflect::TArray<float> a, const Niflect::TArray<bool>& b, Niflect::CString, Niflect::TMap<Niflect::CString, float>)
		//	: m_float_0(0.0f)
		//	, m_string_2(NULL)
		//{
		//}
		NIF_M(CMyMethodNata().SetExampleValue(222.0f))
		CTestLib(const float& a)
			: m_float_0(0.0f)
			, m_string_2(NULL)
			, m_value(0.0f)
		{
			static int cnt = 0;
			printf("ctor, %f, %d\n", a, ++cnt);
		}
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

	public:
		NIF_M()
		void Nihao()
		{
			static int cnt = 0;
			printf("nihao, %d\n", ++cnt);
		}
		NIF_M()
		void Nihao1(float a)
		{
			static int cnt = 0;
			printf("nihao1, %f, %d\n", a, ++cnt);
		}
		NIF_M()
		const float& Nihao2(const Niflect::CString& a)
		{
			static int cnt = 0;
			printf("nihao2, %s, %d\n", a.c_str(), ++cnt);
			m_value = cnt;
			return m_value;
		}
		NIF_M()
		static void MethodStatic0(float a)
		{
			static int cnt = 0;
			printf("MethodStatic0, %f, %d\n", a, ++cnt);
		}
		float m_value;

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
		NIF_F()
		Niflect::TArray<float> m_array_float_1;
		NIF_F()
		Niflect::CString* m_string_2;
		NIF_F()
		float m_float_0;
		NIF_F()
		Niflect::TArray<float> m_array_float_3;
	};
}

class CMyGlobalsNata : public Niflect::CNata
{
public:
	CMyGlobalsNata()
		: m_value(0.0f)
	{
	}
	CMyGlobalsNata& SetExampleValue(float a) { m_value = a; return *this; }
	float m_value;
};

namespace MyGlobalScope
{
	NIF_F(CMyGlobalsNata().SetExampleValue(111.0f))
	extern float g_a;

	NIF_M(CMyGlobalsNata().SetExampleValue(444.0f))
	static void MyScopedStaticFunc1(float a)
	{
		static int cnt = 0;
		printf("MyScopedStaticFunc1, %f, %d\n", a, ++cnt);
	}

	NIF_M(CMyGlobalsNata().SetExampleValue(666.0f))
	void MyGlobalFunc3(float a);
}

NIF_M(CMyGlobalsNata().SetExampleValue(333.0f))
static void MyStaticFunc0(float a)
{
	static int cnt = 0;
	printf("MyStaticFunc0, %f, %d\n", a, ++cnt);
}

NIF_M(CMyGlobalsNata().SetExampleValue(555.0f))
void MyGlobalFunc2(float a);

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
TESTLIB_API Niflect::CNiflectType* GetTestLibGlobalsType();
TESTLIB_API void TestLibSetGlobalVar0(float val);