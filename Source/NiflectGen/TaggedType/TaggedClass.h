#pragma once
#include "NiflectGen/TaggedType/TaggedInheritableType.h"

namespace NiflectGen
{
	class CTaggedClass : public CTaggedInheritableType
	{
		typedef CTaggedInheritableType inherited;
	public:
		CTaggedClass();

	public:
		virtual bool CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context) override;
		virtual Niflect::CString GetInvokeCtorAddr(const Niflect::CString& resocursorName) const override;

	private:
		enum class EStage
		{
			None,
			FoundMember,
		};
		EStage m_stage;
#ifdef SIMPLIFIED_MACRO_CURSOR_FINDING
#else
		CXSourceLocation m_tagLocation;
#endif
		CX_CXXAccessSpecifier m_lastAccessSpecifier;
		Niflect::TSharedPtr<CTaggedInheritableTypeMember> m_createdTaggedChild;
		Niflect::TArray<bool> m_vecIsDefaultCtor;
	};
}