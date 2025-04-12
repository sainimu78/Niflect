#include "Niflect/NiflectTable.h"

namespace Niflect
{
	void CNiflectTable::BuildTypesMeta() const
	{
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		for (auto& it : m_vecType)
			it->BuildTypeMeta();
#else
		for (auto& it : m_vecType)
			it->InitTypeLayout();
#endif
	}
}