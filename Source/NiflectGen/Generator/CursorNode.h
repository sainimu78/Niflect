#pragma once
#include "Niflect/NiflectBase.h"
#include "clang-c/Index.h"

namespace NiflectGen
{
	class CCursorNode;

	using CSharedCursorNode = Niflect::TSharedPtr<CCursorNode>;

	class CCursorNode
	{
		friend class CCursorTree;
	public:
		CCursorNode();
		void Init(const CXCursor& handle);
		void AddChild(const CSharedCursorNode& child);
		//TArray<CSharedCursorNode> GetChildren() const;

	private:
		CXCursor m_handle;
		Niflect::TArrayNif<CSharedCursorNode> m_vecChild;
	};

	class CCursorTree
	{
	public:
		void BuildFromRoot(const CXCursor& handle);
		static void StaticBuildRecurs(const CXCursor& handle, CCursorNode* node);
		static void DebugStaticPrintNodeRecurs(FILE* fp, CCursorNode* node, int level = 0);
		static void StaticPrepareRecurs(const CXCursor& handle);
		static void StaticPrintCursorRecurs(FILE* fp, const CXCursor& handle, int level = 0);
		CCursorNode m_root;
	};
}