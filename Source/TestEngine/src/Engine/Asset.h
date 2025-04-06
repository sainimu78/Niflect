#pragma once
#include "Niflect/NiflectMacroTag.h"

namespace Engine
{
	NIF_T()
	class CAsset
	{
	public:
		CAsset();

	public:
		void InitForTest()
		{
			m_float_0 = 1.23f;
			m_float_1 = 4.56f;
		}
		bool operator==(const CAsset& rhs) const
		{
			return m_float_0 == rhs.m_float_0
				&& m_float_1 == rhs.m_float_1
				;
		}

	public:
		NIF_F()
		float m_float_0;
		NIF_F()
		float m_float_1;
	};
}