#pragma once
#include "NiflectGen/Collector/TypeDecl.h"

namespace NiflectGen
{
	enum class EContainerAccessorType
	{
		None,
		Array,
	};

	class CAccessorData
	{
	public:
		CAccessorData()
			: m_isNotATemplate(false)
			, m_containerAccessorType(EContainerAccessorType::None)
		{

		}
		bool m_isNotATemplate;
		EContainerAccessorType m_containerAccessorType;
	};
}