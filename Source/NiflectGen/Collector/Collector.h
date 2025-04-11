#pragma once
#include "Niflect/NiflectBase.h"
#include "NiflectGen/Collector/CollectionData.h"
#include "clang-c/CXSourceLocation.h"
#include "NiflectGen/Log/Log.h"
#include "NiflectGen/Collector/TaggedNode.h"
#include "NiflectGen/Util/CursorMap.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCode.h"
#include "NiflectGen/Collector/GlobalsCollection.h"

namespace NiflectGen
{
	class CVisitingDebugData
	{
	public:
		CVisitingDebugData();
		void Init(const CXCursor& cursor, FILE* fp);
		bool Check() const;

	public:
		uint32 m_allCursorsCountForChecking;
		FILE* m_fp;
		int m_level;
		uint32 m_allCursorsCount;
	};

	class CCollectingContext
	{
	public:
		CCollectingContext(CGenLog* log)
			: m_log(log)
			, m_debugData(NULL)
		{

		}

	public:
		CGenLog* m_log;
		CVisitingDebugData* m_debugData;
	};

	class CTaggedTypeCollector
	{
	public:
		CTaggedTypeCollector(CMacroTagCollection2& tagCollection, CGlobalsCollection& globalsCollection);
		bool Collect(const CXCursor& cursor, CTaggedNode2* taggedParent, const CTaggedNode2* rootNode, CGenLog* log);

	private:
		enum class EStage
		{
			None,
			Found,
		};
		EStage m_stage;
		CXSourceLocation m_tagLocation;
		CMacroTagCollection2& m_tagCollection;
		CGlobalsCollection& m_globalsCollection;
	};

	class CTemplateCollector
	{
	public:
		CTemplateCollector();
		bool Collect(const CXCursor& cursor, CTaggedNode2* taggedParent, CGenLog* log);
	};

#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
	class CUntaggedTypeCollector
	{
	public:
		CUntaggedTypeCollector();
		bool Collect(const CXCursor& cursor, CTaggedNode2* taggedParent, CGenLog* log);
	};
#else
#endif

	class CTaggedTypeCollector;

	struct SVisitingData
	{
		Niflect::TArrayNif<uint32> m_vecTaggedChildIndex;
	};

	struct SRecursCollectingData
	{
		CAliasChain* m_aliasChain;
		CCollectedAccessorSettings& m_settings;
	};

	class CDataCollector
	{
		friend class CScopeNamespace;
		friend class CScopeBindingSetting;
		friend class CScopeAccessorBaseCursorDecl;
		friend class CScopeTemplateDecl;
		friend CXChildVisitResult VisitorCallback(CXCursor cursor, CXCursor parent, CXClientData data);
	public:
		CDataCollector(const CModuleRegInfoValidated& moduleRegInfo);

	public:
		void Collect(const CXCursor& cursor, CTaggedNode2* taggedParent, CCollectingContext& context, CCollectionData& collectionData);
		void DebugFinish2(CTaggedNode2* taggedParent, const CCollectionData& collectionData) const;

	private:
		void CollectDataRecurs2(const CXCursor& cursor, const CXCursor& parentCursor, CTaggedNode2* taggedParent, const CTaggedNode2* rootNode, CCollectingContext& context, SRecursCollectingData& recursCollectiingData);
		void Visit(const CXCursor& cursor, CTaggedNode2* taggedParent, const CTaggedNode2* rootNode, CCollectingContext& context, CAliasChain* aliasChain, SVisitingData& data);
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
		void CollectUntaggedTypesRecurs(CTaggedNode2* taggedParent, CUntaggedTypesMapping& mapping) const;
#else
#endif

	private:
		bool VerifyDerivedFromCAccessor(CXCursor p, CAliasChain* aliasChain) const;
		CXCursor FindAliasDeclOld(CXCursor decl) const;

	private:
		uint32 GetNamespaceDepth() const;
		const CXCursor& GetVisitingNamespaceReserved() const;//备用

	private:
		const CModuleRegInfoValidated& m_moduleRegInfo;
		Niflect::TArrayNif<CXCursor> m_stkNamespaceCursor;//备用
		CMacroTagCollection2 m_macroTagCollection;
		CTaggedTypeCollector m_taggedTypeCollector;
		CGlobalsCollection m_globalsCollection;
		CTemplateCollector m_templateCollector;
#ifdef BINDING_TYPE_DUPLICATION_VERIFICATION
		CUntaggedTypeCollector m_untaggedTypeCollector;
#else
#endif
		TCursorMap<CXCursor> m_mapCursorDeclToBaseCursorDecl;
		//TCursorMap<CXCursor> m_mapCursorDeclToAliasDecl;
		//CSharedAliasChain m_aliasChain;
		//CCursorSet m_setAliasTemplateOrClassTemplateDecl;
		bool m_collectingClassBaseCursorDecl;
	};

	//void PrintTemplateSubcursor333333333(const CTemplateSubcursor& parentSubcursor, int level);
}