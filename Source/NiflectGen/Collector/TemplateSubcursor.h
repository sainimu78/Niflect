#pragma once
#include "Niflect/NiflectBase.h"
#include "clang-c/Index.h"

namespace NiflectGen
{
	//SubcursorΪ����CXCursor���ӽڵ���֯�Ľڵ�, ���߼����Կɹ������ͽṹ, �Եݹ����

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