#pragma once
#include "NiflectGen/Collector/TaggedNode.h"
#include "NiflectGen/CodeWriter/TypeReg/TypeRegCodeWriter.h"
#include "NiflectGen/Util/CursorMap.h"
#include "NiflectGen/Collector/AccessorSettingMapping.h"

namespace NiflectGen
{
	class CTaggedTypesMapping
	{
	public:
		void Resolve();
		bool InitIndexedNodeForClassDecl(const CXCursor& cursor, const CAccessorBindingMapping2& accessorBindingMapping, CResolvedCursorNode& indexedParent) const;
		Niflect::CString GetInfoTypeName(uint32 taggedTypeIdx) const;
		bool GetDerivedInfoTypeName(uint32 taggedTypeIdx, Niflect::CString& infoTypeName) const;
		TCursorMap<uint32> m_mapCursorToIndex;
		Niflect::TArrayNif<CTaggedType*> m_vecType;
	};

	class CResolvingDependenciesContext
	{
	public:
		CResolvingDependenciesContext(const SResolvedMappings& mappings, CGenLog* log)
			: m_mappings(mappings)
			, m_log(log)
		{

		}
		const SResolvedMappings& m_mappings;
		CGenLog* m_log;
	};

	struct SResolvingDependenciesData
	{
		CResolvedCursorRootsMapping& m_signatureMapping;
	};

	class CTaggedType : public CTaggedNode2
	{
	public:
		CTaggedType()
			: m_isImportType(false)
		{
		}

	public:
		void ResolveForResocursorNode();
		void InitForImportType();

	public:
		virtual void Deprecated_ResolveDependcies(const TCursorMap<CTaggedType*>& mapCursorDeclToTaggedType)
		{

		}
		virtual bool RequiredGenHIncluded() const { return false; }
		virtual void ResolveDependcies(const CResolvingDependenciesContext& context, SResolvingDependenciesData& data) {}
		void ResolveBasic(const CResolvingDependenciesContext& context, SResolvingDependenciesData& data);
		virtual CSharedTypeRegCodeWriter Deprecated_CreateCodeWriter(const STypeRegClassWritingSetting& setting) const
		{
			ASSERT(false);
			return NULL;
		}
		virtual CSharedTypeRegCodeWriter CreateCodeWriter(const STypeRegClassWritingSetting& setting) const
		{
			ASSERT(false);
			return NULL;
		}
		virtual CSharedTypeRegCodeWriter2 CreateCodeWriter2() const
		{
			ASSERT(false);
			return NULL;
		}
		void WriteUsingNamespaceDirectiveForNata(CCodeLines& lines) const;
		void WriteTaggedTypeCopyNata(CCodeLines& lines) const;

	public:
		static CTaggedType* CastChecked(CTaggedNode2* base)
		{
			return dynamic_cast<CTaggedType*>(base);
		}

	public:
		Niflect::CString m_resocursorName;
		Niflect::TArrayNif<Niflect::CString> m_vecScopeName;
		CResolvedCursorNode m_taggedResoRoot;
		bool m_isImportType;
	};

	class CTaggedInheritableTypeMember : public CTaggedNode2
	{
		typedef CTaggedNode2 inherited;
	public:
		CTaggedInheritableTypeMember();
		void InitMember(CX_CXXAccessSpecifier accessSpecifier);

	public:
		virtual bool CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context) override;

	protected:
		virtual void DebugDerivedPrint(FILE* fp) const override;

	public:
		const Niflect::TArrayNif<CXCursor>& GetDetailCursors() const
		{
			return m_vecDetailCursor;
		}

	public:
		CX_CXXAccessSpecifier m_accessSpecifier;
		Niflect::TArrayNif<CXCursor> m_vecDetailCursor;
	};

	class CTaggedInheritableTypeField : public CTaggedInheritableTypeMember
	{
		typedef CTaggedInheritableTypeMember inherited;
	public:
		static CTaggedInheritableTypeField* CastChecked(CTaggedNode2* base)
		{
			return dynamic_cast<CTaggedInheritableTypeField*>(base);
		}
	};

	class CTaggedInheritableTypeMethod : public CTaggedInheritableTypeMember
	{
		typedef CTaggedInheritableTypeMember inherited;
	public:
		static CTaggedInheritableTypeMethod* CastChecked(CTaggedNode2* base)
		{
			return dynamic_cast<CTaggedInheritableTypeMethod*>(base);
		}
	};

#ifdef PORTING_ACCESS_METHOD
	class CTaggedInheritableTypeAccessMethod : public CTaggedInheritableTypeMember
	{
		typedef CTaggedInheritableTypeMember inherited;
	public:
		static CTaggedInheritableTypeAccessMethod* CastChecked(CTaggedNode2* base)
		{
			return dynamic_cast<CTaggedInheritableTypeAccessMethod*>(base);
		}
	};
#endif

	class CUntaggedTemplate : public CTaggedNode2
	{
		typedef CTaggedNode2 inherited;
	public:
		CUntaggedTemplate();

	public:
		virtual bool CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context) override;

	public:
		void InitForAlias(const CAliasChain& aliasChain, const CUntaggedTemplatesMapping& untaggedMapping);

	public:
		static CUntaggedTemplate* CastChecked(inherited* base)
		{
			return dynamic_cast<CUntaggedTemplate*>(base);
		}

	public:
		CUntaggedTemplate* m_originalUntaggedDecl;
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
		uint32 m_argsCount;
#else
#endif
	};

#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
	class CUntaggedType : public CTaggedNode2
	{
		typedef CTaggedNode2 inherited;
	public:
		CUntaggedType();

	public:
		virtual bool CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context) override;

	public:
		static CUntaggedType* CastChecked(inherited* base)
		{
			return dynamic_cast<CUntaggedType*>(base);
		}
	};
#else
#endif

	Niflect::CString ResolveSignature(const CResolvedCursorNode& indexedParent, const CResolvingDependenciesContext& ctx, CResolvedCursorRootsMapping& signatureMapping);
}