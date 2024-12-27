#pragma once
#include "NiflectGen/TaggedType/TaggedInheritableType.h"

namespace NiflectGen
{
	class CTaggedEnum : public CTaggedType
	{
		typedef CTaggedType inherited;
	public:
		virtual bool CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context) override;

	public:
		virtual CSharedTypeRegCodeWriter CreateCodeWriter(const STypeRegClassWritingSetting& setting) const override;
		virtual CSharedTypeRegCodeWriter2 CreateCodeWriter2() const override;
	};
}