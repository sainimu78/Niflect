#pragma once
#include "NiflectGen/TaggedType/TaggedType.h"

namespace NiflectGen
{
	class CTaggedInheritableType : public CTaggedType
	{
		typedef CTaggedType inherited;
	public:
		CTaggedInheritableType();

	public:
		virtual bool RequiredGenHIncluded() const { return m_generatedBodyLineNumber != INDEX_NONE; }

	protected:
		void InitBaseTypeSpecifierCursor(const CXCursor& cursor);
		bool CollectGeneratedBodyTag(const CXCursor& cursor, const CXCursorKind& kind);
		void ErrorIfNoGeneratedBodyTag(const CXCursor& cursor) const;

	public:
		virtual void Deprecated_ResolveDependcies(const TCursorMap<CTaggedType*>& mapCursorDeclToTaggedType) override;
		virtual void ResolveDependcies(const CResolvingDependenciesContext& context, SResolvingDependenciesData& data) override;
		virtual CSharedTypeRegCodeWriter2 CreateCodeWriter2() const override;

	protected:
		virtual void DebugDerivedPrint(FILE* fp) const override;

	private:
		CXCursor m_baseTypeSpecifierCursor;
		CTaggedType* m_baseTaggedType;
		Niflect::TArrayNif<CTaggedInheritableTypeField*> m_vecMemberField;
		Niflect::TArrayNif<CTaggedInheritableTypeMethod*> m_vecMemberMethod;
#ifdef PORTING_ACCESS_METHOD
		Niflect::TArrayNif<CTaggedInheritableTypeAccessMethod*> m_vecMemberAccessMethod;
#endif
		Niflect::TArrayNif<CResolvedCursorNode> m_vecMemberIndexedRoot;
		uint32 m_generatedBodyLineNumber;
	};
}