#include "NiflectGen/Generator/CursorNode.h"

namespace NiflectGen
{
	using namespace Niflect;

	CCursorNode::CCursorNode()
		: m_handle{}
	{
		//clang_Cursor_isNull()
	}
	void CCursorNode::Init(const CXCursor& handle)
	{
		m_handle = handle;
	}
	void CCursorNode::AddChild(const CSharedCursorNode& child)
	{
		m_vecChild.push_back(child);
	}
	//TArray<CSharedCursorNode> CCursorNode::GetChildren() const
	//{

	//}

	void CCursorTree::BuildFromRoot(const CXCursor& handle)
	{
		m_root.Init(handle);
		StaticBuildRecurs(handle, &m_root);
	}
	void CCursorTree::StaticBuildRecurs(const CXCursor& handle, CCursorNode* node)
	{
		auto visitor = [](CXCursor childHandle, CXCursor parentHandle, CXClientData data)
		{
			auto parentNode = static_cast<CCursorNode*>(data);

			auto childNode = MakeShared<CCursorNode>();
			childNode->Init(childHandle);
			parentNode->AddChild(childNode);

			//if (childHandle.kind == CXCursor_LastPreprocessing)
			//	return CXChildVisit_Break;

			return CXChildVisit_Continue;

			//return CXChildVisit_Recurse;
		};

		clang_visitChildren(handle, visitor, node);

		for (auto& it : node->m_vecChild)
		{
			StaticBuildRecurs(it->m_handle, it.Get());
		}
	}
	static void MytoString(const CXString& str, CString& output)
	{
		output = clang_getCString(str);
		clang_disposeString(str);
	}
	static CString DebugIndentToString(uint32 level, const CString& singleIndentInString = "-")
	{
		auto curLevel = level;
		CString strLevel;
		while (curLevel--)
			strLevel += singleIndentInString;
		return strLevel;
	}
	void CCursorTree::DebugStaticPrintNodeRecurs(FILE* fp, CCursorNode* parentNode, int level)
	{
		auto strLevel = DebugIndentToString(level);

		CString displayName;
		MytoString(clang_getCursorDisplayName(parentNode->m_handle), displayName);
		
		CString strKind;
		{
			CXCursorKind cursorKind = clang_getCursorKind(parentNode->m_handle);
			CXString cursorKindSpelling = clang_getCursorKindSpelling(cursorKind);
			strKind = clang_getCString(cursorKindSpelling);
			clang_disposeString(cursorKindSpelling);
		}


		fprintf(fp, "%s%s (%s)\n", strLevel.c_str(), displayName.c_str(), strKind.c_str());

		//auto& kind = parentNode->m_handle.kind;
		//if (kind == CXCursor_MacroDefinition ||
		//	kind == CXCursor_MacroExpansion ||
		//	kind == CXCursor_MacroInstantiation
		//	)
		//{
		//	CString displayName;
		//	MytoString(clang_getCursorDisplayName(parentNode->m_handle), displayName);
		//	if (!displayName.empty())
		//	{
		//		if (displayName[0] != '_')
		//			printf("");
		//	}
		//}
		//if (clang_isCursorDefinition(parentNode->m_handle))
		//{
		//	if (parentNode->m_handle.kind == CXCursor_MacroDefinition)
		//	{
		//		printf("");
		//	}
		//}

		level++;
		for (auto& it : parentNode->m_vecChild)
		{
			DebugStaticPrintNodeRecurs(fp, it.Get(), level);
		}
	}
	void CCursorTree::StaticPrepareRecurs(const CXCursor& handle)
	{

	}
	void CCursorTree::StaticPrintCursorRecurs(FILE* fp, const CXCursor& handle, int level)
	{
		auto strLevel = DebugIndentToString(level);

		CString displayName;
		MytoString(clang_getCursorDisplayName(handle), displayName);

		CString strKind;
		{
			CXCursorKind cursorKind = clang_getCursorKind(handle);
			CXString cursorKindSpelling = clang_getCursorKindSpelling(cursorKind);
			strKind = clang_getCString(cursorKindSpelling);
			clang_disposeString(cursorKindSpelling);
		}


		fprintf(fp, "%s%s (%s)\n", strLevel.c_str(), displayName.c_str(), strKind.c_str());

		using CCursorArray = Niflect::TArrayNif<CXCursor>;
		CCursorArray vecChild;

		auto visitor = [](CXCursor childHandle, CXCursor parentHandle, CXClientData data)
		{
			auto& vecChild = *static_cast<CCursorArray*>(data);
			vecChild.push_back(childHandle);

			//if (childHandle.kind == CXCursor_LastPreprocessing)
			//	return CXChildVisit_Break;

			return CXChildVisit_Continue;
		};

		clang_visitChildren(handle, visitor, &vecChild);

		level++;
		for (auto& it : vecChild)
		{
			StaticPrintCursorRecurs(fp, it, level);
		}
	}
}