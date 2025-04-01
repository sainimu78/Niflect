#pragma once
#include "Engine/EngineCommon.h"
#include "Niflect/NiflectRegisteredType.h"

namespace TestRegistration
{
	class CMyRegClass;
}

namespace Niflect
{
	//template <typename T>
	//extern CNiflectType* StaticGetType();

	//template <typename T>
	//CNiflectType* StaticGetType()
	//{
	//	return TInternalRegisteredType<T>::s_type;
	//}

	template <>
	ENGINE_API CNiflectType* StaticGetTypeOld<TestRegistration::CMyRegClass>();
}

namespace TestRegistration
{
	class CMyRegClass
	{
	public:
		CMyRegClass()
			: m_float_0(0.0f)
			, m_bool_1(false)
		{

		}

		void TestInit()
		{
			m_float_0 = 1.23f;
			m_bool_1 = true;
		}

		bool operator==(const CMyRegClass& rhs) const
		{
			return
				m_float_0 == rhs.m_float_0 &&
				m_bool_1 == rhs.m_bool_1
				;
		}

	public:
		float m_float_0;
		bool m_bool_1;
	};

	ENGINE_API void TestTypeNatimeta();
}