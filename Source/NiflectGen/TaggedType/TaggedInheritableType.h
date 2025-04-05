#pragma once
#include "NiflectGen/TaggedType/TaggedType.h"

namespace NiflectGen
{
	class CResolvedMethod
	{
	public:
		Niflect::TArray<CResolvedCursorNode> m_vecArgument;
		CResolvedCursorNode m_resultType;
	};

	class CTaggedInheritableType : public CTaggedType
	{
		typedef CTaggedType inherited;
	public:
		CTaggedInheritableType();

	public:
		virtual bool RequiredGenHIncluded() const override;

	protected:
		void InitBaseTypeSpecifierCursor(const CXCursor& cursor);
		bool CollectGeneratedBodyTag(const CXCursor& cursor, const CXCursorKind& kind);
		void ErrorIfNoGeneratedBodyTag(const CXCursor& cursor) const;

	public:
		virtual void Deprecated_ResolveDependcies(const TCursorMap<CTaggedType*>& mapCursorDeclToTaggedType) override;
		virtual void ResolveDependcies(const CResolvingDependenciesContext& context, SResolvingDependenciesData& data) override;
		virtual CSharedTypeRegCodeWriter2 CreateCodeWriter2() const override;
		virtual Niflect::CString GetInvokeCtorAddr(const Niflect::CString& resocursorName) const override;

	protected:
		virtual void DebugDerivedPrint(FILE* fp) const override;

	private:
		CXCursor m_baseTypeSpecifierCursor;
		CTaggedType* m_baseTaggedType;
		Niflect::TArray<CTaggedInheritableTypeField*> m_vecField;
		Niflect::TArray<CTaggedInheritableTypeMethod*> m_vecMethod;
#ifdef PORTING_ACCESS_METHOD
		Niflect::TArray<CTaggedInheritableTypeAccessMethod*> m_vecMemberAccessMethod;
#endif
		Niflect::TArray<CResolvedCursorNode> m_vecMemberIndexedRoot;
		Niflect::TArray<CResolvedMethod> m_vecResomethod;
		uint32 m_generatedBodyLineNumber;
	};
}