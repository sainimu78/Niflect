#pragma once
#include "NiflectGen/Util/CursorMap.h"
#include "NiflectGen/Collector/AccessorSettingMapping.h"
#include "NiflectGen/Collector/AliasChain.h"

namespace NiflectGen
{
	class CCollectionData
	{
	public:
		Niflect::TArrayNif<CBindingSettingData> deprecated_m_vecBindingSetting;
		TCursorMap<CXCursor> deprecated_m_mapAliasTemplateDeclToClassTemplateCursor;
		CSharedAliasChain m_aliasChain;
		CSharedAccessorBindingMapping m_accessorBindingMapping;
		Niflect::TSharedPtr<CTaggedType> m_globalsTypeNode;
	};
}