#include "Niflect/NiflectField.h"
#include "Niflect/NiflectType.h"
#include "Niflect/Util/StringUtil.h"

namespace Niflect
{
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	bool CField::LayoutSaveToRwNode(const InstanceType* base, CRwNode* rw) const
	{
		return m_type->GetTypeLayout().AccessorsSaveToRwNode(base, m_offset, rw);
	}
	bool CField::LayoutLoadFromRwNode(InstanceType* base, const CRwNode* rw) const
	{
		return m_type->GetTypeLayout().AccessorsLoadFromRwNode(base, m_offset, rw);
	}

	HashInt ComputeFieldHash(const Niflect::CString& str)
	{
		return NiflectUtil::StableHash(str);
	}
#endif
}