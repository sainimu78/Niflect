#pragma once
#include "Niflect/NiflectBase.h"
#include "clang-c/Index.h"

namespace NiflectGen
{
	//Subcursor为不以CXCursor的子节点组织的节点, 但逻辑上仍可构成树型结构, 以递归遍历

	class CTemplateSubcursor
	{
	public:
		CTemplateSubcursor()
			: m_type{}
			, m_cursor{}
		{

		}
		CXType m_type;
		CXCursor m_cursor;
		Niflect::TArrayNif<CTemplateSubcursor> m_vecChild;
	};
}