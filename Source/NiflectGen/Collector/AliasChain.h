#pragma once
#include "NiflectGen/Util/CursorMap.h"

namespace NiflectGen
{
	class CAliasInfo
	{
	public:
		CAliasInfo();
		CXCursor m_decl;
		CXType m_underlyingType;
		uint32 m_templateArgsCount;
	};
	class CAliasChain
	{
	public:
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
		void AddLinkDecl2(const CXCursor& decl);
		void AddLinkAliasDecl(const CXCursor& decl);
		void AddLinkTemplateAliasDecl(const CXCursor& decl);
#else
		void AddLinkDecl(const CXCursor& decl);
#endif
		void LinkToReferenced(const CXCursor& decl, const CXCursor& alias, uint32 templateArgsCount);
		//CXCursor FindOriginalDeclOld(CXCursor decl) const;
		bool FindOriginalDecl(CXCursor decl, CXCursor& lastValidDecl, bool stoppedOnClassDecl = false) const;

	private:
		CAliasInfo& AddLinkDeclInternal(const CXCursor& decl);

	public:
		TCursorMap<uint32> m_mapDeclToIndex;
		Niflect::TArrayNif<CAliasInfo> m_vecItem;
	};
	using CSharedAliasChain = Niflect::TSharedPtr<CAliasChain>;
}