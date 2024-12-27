#include "NiflectGen/Collector/UntaggedTemplateMapping.h"
#include "NiflectGen/TaggedType/TaggedType.h"

namespace NiflectGen
{
	void CUntaggedTemplatesMapping::Init(const CAliasChain& aliasChain)
	{
		for (auto& it : m_vecType)
			it->InitForAlias(aliasChain, *this);
	}
}