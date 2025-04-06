#pragma once
#include "Niflect/NiflectMacroTag.h"

namespace TestSomeScope
{
	NIF_T()
	enum class EMyEnumClassOption
	{
		EDefaultMode NIF_C(),
		EMode0 NIF_C(),
		EState0 NIF_C(),
	};

	template <typename T0, typename T1>
	struct SMyPair
	{
		T0 m_nihao;
		T1 m_bucuo;
	};

	template <typename T0, typename T1>
	using SMyPairAlias = SMyPair<T0, T1>;

	struct SMyStruct
	{
		bool m_bool_0;
		int32 m_int32_1;
	};

	NIF_T()
	class CSomeIrrelavantClass
	{
	};
}