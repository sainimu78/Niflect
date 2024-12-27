#pragma once
#include "Niflect/NiflectMacro.h"
#include "Niflect/NiflectBase.h"
#include "Engine/Test/TestMyTransform.h"

namespace TestMyFinding
{
	using namespace Niflect;
	
	NIF_T()
	class CMyClass_0
	{
	public:
		CMyClass_0()
			: m_float_2(0.0f)
		{
	
		}
	
		NIF_F()
		Niflect::TMap<Niflect::CString, Niflect::TArray<Niflect::TMap<Niflect::CString, float> > > m_map_string_array_map_string_float_0;
		NIF_F()
		Niflect::TMap<Niflect::CString, float> m_map_string_float_1;
		NIF_F()
		float m_float_2;
		NIF_F()
		Niflect::CString m_string_3;
	};
}

//namespace TestMyFinding
//{
//	using namespace Niflect;
//
//	NIF_T()
//	class CMyClass_0
//	{
//	public:
//		CMyClass_0()
//			: m_float_0(0.0f)
//			, m_bool_1(false)
//			, m_int32_10(0)
//		{
//
//		}
//
//		//NIF_F()
//		float m_float_0;
//		//NIF_F()
//		bool m_bool_1;
//		//NIF_F()
//		TArrayNif<float> m_array_float_2;
//		//NIF_F()
//		Niflect::TArrayNif<bool> m_bits_3;
//		//NIF_F()
//		Niflect::TArrayNif<Niflect::TArrayNif<float> > m_array_array_float_4;
//		//NIF_F()
//		Niflect::TArrayNif<Niflect::TArrayNif<bool> > m_array_bits_5;
//		//NIF_F()
//		std::vector<float> m_std_array_float_6;
//		//NIF_F()
//		std::vector<bool> m_std_bits_7;
//		//NIF_F()
//		Niflect::CString m_string_8;
//		//NIF_F()
//		Niflect::TMap<Niflect::CString, float> m_map_string_float_9;
//		//NIF_F()
//		int32 m_int32_10;
//		//NIF_F()
//		std::pair<Niflect::CString, float> m_pair_string_float_11;
//		//NIF_F()
//		Niflect::TMap<Niflect::CString, Niflect::TArrayNif<float> > m_map_string_array_float_12;
//		NIF_F()
//		Niflect::TMap<Niflect::CString, Niflect::TArrayNif<bool> > m_map_string_bits_13;
//		//NIF_F()
//		Niflect::TMap<Niflect::CString, Niflect::TArrayNif<Niflect::TMap<Niflect::CString, float> > > m_map_string_array_map_string_float_14;
//		//NIF_F()
//		TestAccessor2::TMyTransform<float> m_tm_15;
//		//NIF_F()
//		Niflect::TArrayNif<Niflect::TMap<Niflect::CString, float> > m_array_map_string_float_16;
//		//NIF_F()
//		Niflect::TArrayNif<Niflect::TMap<Niflect::CString, Niflect::TArrayNif<float> > > m_array_map_string_array_float_17;
//		//NIF_F()
//		//TestAccessor2::TMyTransform<bool> m_tm_18;
//	};
//
//	NIF_T()
//	class CMyClass_1
//	{
//	public:
//		CMyClass_1()
//			: m_float_0(0.0f)
//		{
//
//		}
//		NIF_F()
//		float m_float_0;
//		NIF_F()
//		CMyClass_0 m_my_1;
//		NIF_F()
//		std::pair<Niflect::CString, CMyClass_0> m_pair_string_my_2;
//		NIF_F()
//		Niflect::TArrayNif<CMyClass_0> m_array_my_3;
//		NIF_F()
//		Niflect::TMap<Niflect::CString, CMyClass_0> m_map_string_my_4;
//		NIF_F()
//		Niflect::TArrayNif<Niflect::TMap<Niflect::CString, Niflect::TArrayNif<CMyClass_0> > > m_array_map_string_array_my_5;
//		NIF_F()
//		Niflect::TMap<Niflect::CString, Niflect::TArrayNif<Niflect::TMap<Niflect::CString, CMyClass_0> > > m_map_string_array_map_string_my_6;
//	};
//}