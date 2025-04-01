#pragma once
#include "Engine/EngineCommon.h"
#include "Niflect/NiflectAccessor.h"

namespace TestAccessor
{
	template <typename T>
	class TMyVector
	{
	public:
		//TMyVector()
		//	: m_x(0)
		//	, m_y(0)
		//	, m_z(0)
		//{

		//}
		T m_x;
		T m_y;
		T m_z;
	};

	template <typename T>
	class TMyQuaternion
	{
	public:
		//TMyQuaternion()
		//	: m_x(0)
		//	, m_y(0)
		//	, m_z(0)
		//	, m_w(0)
		//{

		//}
		T m_x;
		T m_y;
		T m_z;
		T m_w;
	};

	template <typename T>
	class TMyTransform
	{
	public:
		TMyVector<T> m_translation;
		TMyQuaternion<T> m_rotation;
		TMyVector<T> m_scale;
	};

	template <typename T>
	static bool operator==(const TMyVector<T>& lhs, const TMyVector<T>& rhs)
	{
		return
			lhs.m_x == rhs.m_x &&
			lhs.m_y == rhs.m_y &&
			lhs.m_z == rhs.m_z
			;
	}
	template <typename T>
	static bool operator==(const TMyQuaternion<T>& lhs, const TMyQuaternion<T>& rhs)
	{
		return
			lhs.m_x == rhs.m_x &&
			lhs.m_y == rhs.m_y &&
			lhs.m_z == rhs.m_z &&
			lhs.m_w == rhs.m_w
			;
	}
	template <typename T>
	static bool operator==(const TMyTransform<T>& lhs, const TMyTransform<T>& rhs)
	{
		return
			lhs.m_translation == rhs.m_translation &&
			lhs.m_rotation == rhs.m_rotation &&
			lhs.m_scale == rhs.m_scale
			;
	}

	typedef TMyTransform<float> CMyTransformFloat;

	template <typename T>
	static void TestInit_TMyTransform(TMyTransform<T>& srcData)
	{
		srcData.m_translation.m_x = 1.0f;
		srcData.m_translation.m_y = 2.0f;
		srcData.m_translation.m_z = 3.0f;
		srcData.m_rotation.m_x = 4.0f;
		srcData.m_rotation.m_y = 5.0f;
		srcData.m_rotation.m_z = 6.0f;
		srcData.m_rotation.m_w = 7.0f;
		srcData.m_scale.m_x = 8.0f;
		srcData.m_scale.m_y = 9.0f;
		srcData.m_scale.m_z = 10.0f;
	}

	class CTestClassMy
	{
	public:
		CTestClassMy()
			: m_float_0(0.0f)
			, m_float_2(0.0f)
			, m_bool_3(false)
		{

		}

		void TestInit()
		{
			m_float_0 = 1.0f;
			m_array_1.resize(3);
			m_array_1[0] = 2.1f;
			m_array_1[1] = 2.2f;
			m_array_1[2] = 2.3f;
			m_float_2 = 1.1f;
			m_bool_3 = true;
			m_array_array_4.resize(2);
			m_array_array_4[0].resize(3);
			m_array_array_4[0][0] = 1.2f;
			m_array_array_4[0][1] = 1.3f;
			m_array_array_4[0][2] = 1.4f;
			m_array_array_4[1].resize(2);
			m_array_array_4[1][0] = 1.5f;
			m_array_array_4[1][1] = 1.6f;
			TestInit_TMyTransform(m_tm_5);
		}

		bool operator==(const CTestClassMy& rhs) const
		{
			return
				(m_float_0 == rhs.m_float_0) &&
				(m_array_1 == rhs.m_array_1) &&
				(m_float_2 == rhs.m_float_2) &&
				(m_bool_3 == rhs.m_bool_3) &&
				(m_array_array_4 == rhs.m_array_array_4) &&
				(m_tm_5 == rhs.m_tm_5)
				;
		}

		float m_float_0;
		Niflect::TArrayNif<float> m_array_1;
		float m_float_2;
		bool m_bool_3;
		Niflect::TArrayNif<Niflect::TArrayNif<float> > m_array_array_4;
		TMyTransform<float> m_tm_5;
	};

	class CTestClassMy2 : public CTestClassMy
	{
		typedef CTestClassMy inherited;
	public:
		CTestClassMy2()
			: m_derived_bool_0(false)
			, m_derived_float_2(0.0f)
			, m_derived_bool_4(true)
		{

		}
		void TestInit()
		{
			inherited::TestInit();

			m_derived_bool_0 = true;
			m_derived_array_1.resize(4);
			m_derived_array_1[0] = 20.1f;
			m_derived_array_1[1] = 20.2f;
			m_derived_array_1[2] = 20.3f;
			m_derived_array_1[3] = 20.4f;
			m_derived_float_2 = 10.1f;
			m_derived_array_3.resize(3);
			m_derived_array_3[0] = true;
			m_derived_array_3[1] = false;
			m_derived_array_3[2] = true;
			m_derived_bool_4 = false;
			m_derived_array_5.resize(3);
			for (auto idx = 0; idx < m_derived_array_5.size(); ++idx)
			{
				auto& it = m_derived_array_5[idx];
				TestInit_TMyTransform(it);
				it.m_scale.m_y += 1 + idx;
			}
		}

		bool operator==(const CTestClassMy2& rhs) const
		{
			if (inherited::operator==(rhs))
			{
				return
					(m_derived_bool_0 == rhs.m_derived_bool_0) &&
					(m_derived_array_1 == rhs.m_derived_array_1) &&
					(m_derived_float_2 == rhs.m_derived_float_2) &&
					(m_derived_array_3 == rhs.m_derived_array_3) &&
					(m_derived_bool_4 == rhs.m_derived_bool_4) &&
					(m_derived_array_5 == rhs.m_derived_array_5)
					;
			}
			return false;
		}

		bool m_derived_bool_0;
		Niflect::TArrayNif<float> m_derived_array_1;
		float m_derived_float_2;
		Niflect::TArrayNif<bool> m_derived_array_3;
		bool m_derived_bool_4;
		Niflect::TArrayNif<TMyTransform<float> > m_derived_array_5;
	};

	ENGINE_API void TestFieldLayout();
}