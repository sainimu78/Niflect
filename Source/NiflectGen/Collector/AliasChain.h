#pragma once
#include "NiflectGen/Util/CursorMap.h"

namespace NiflectGen
{
	class CAliasInfo
	{
	public:
		CAliasInfo();
		CXCursor m_decl;
		uint32 m_templateArgsCount;
	};
	class CAliasChain
	{
	public:
		void AddLinkDecl(const CXCursor& decl);
		void LinkToReferenced(const CXCursor& decl, const CXCursor& alias, uint32 templateArgsCount);
		//CXCursor FindOriginalDeclOld(CXCursor decl) const;
		bool FindOriginalDecl(CXCursor decl, CXCursor& lastValidDecl, bool stoppedOnClassDecl = false) const;

	private:
		TCursorMap<uint32> m_mapDeclToIndex;
		Niflect::TArrayNif<CAliasInfo> m_vecItem;
	};
	using CSharedAliasChain = Niflect::TSharedPtr<CAliasChain>;
}