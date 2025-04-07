#include "Niflect/NiflectAccessor.h"
#include "Niflect/NiflectType.h"

namespace Niflect
{
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	bool CTypeLayout::AccessorsSaveToRwNode(const InstanceType* base, const OffsetType& offs, CRwNode* rw) const
	{
		for (auto& it : m_vecSection)
		{
			if (!it->m_accessor->SaveToRwNode(base, offs, rw))
				return false;
		}
		return true;
	}
	bool CTypeLayout::AccessorsLoadFromRwNode(InstanceType* base, const OffsetType& offs, const CRwNode* rw) const
	{
		for (auto& it : m_vecSection)
		{
			if (!it->m_accessor->LoadFromRwNode(base, offs, rw))
				return false;
		}
		return true;
	}

	const Niflect::TArray<CField>& CAccessor::GetFields() const
	{
		return m_type->GetFields();
	}
	const CTypeLayout& CAccessor::GetElementLayout() const
	{
		return m_elemType->GetTypeLayout();
	}
#endif
}