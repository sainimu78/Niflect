#pragma once
#include "Niflect/NiflectBase.h"
#include "clang-c/Index.h"
#include "NiflectGen/CodeWriter/CodeWriter.h"

namespace NiflectGen
{
	class CGenLog;
	class CTaggedNode2;

	using CSharedTaggedNode = Niflect::TSharedPtr<CTaggedNode2>;

	struct CXSourceLocationComp
	{
		bool operator()(const CXSourceLocation& lhs, const CXSourceLocation& rhs) const;
	};

	class CMacroTagCollection2
	{
	public:
		CMacroTagCollection2();
		~CMacroTagCollection2();
#ifdef SIMPLIFIED_MACRO_CURSOR_FINDING
		bool PushMacroExpansion(CXCursorKind kind, const CXCursor& cursor);
		void PopMacroExpansion(CXCursor& macroCursor);
#else
		bool CollectMacroExpansion(const CXCursor& cursor);
		//void TakeByTagLocation(const CXSourceLocation& tagLocation, const CXCursor& typeCursor, CTaggedNode& taggedParent);
		void TakeByTagLocation(const CXSourceLocation& tagLocation, CXCursor& macroCursor);
#endif

	private:
#ifdef SIMPLIFIED_MACRO_CURSOR_FINDING
		Niflect::TArrayNif<CXCursor> m_stkMacroCursor;
#else
		Niflect::TMap<CXSourceLocation, CXCursor, CXSourceLocationComp> m_mapLocationToMacroCursor;
#endif
	};

	struct STaggedNodeCollectingContext
	{
		CMacroTagCollection2& m_tagCollection;
		CGenLog*& m_log;
	};

	struct SResolvingMacroNataContext
	{
		CGenLog* m_log;
	};

#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
	class CBuiltinMetadata
	{
	public:
		Niflect::CString m_getterName;
		Niflect::CString m_setterName;
		Niflect::CString m_defaultValue;
	};
#endif

	class CTaggedNode2
	{
	public:
		CTaggedNode2();
		void Init(const CXCursor& cursor);
		//void SetMacroCursor(const CXCursor& macroCursor)
		//{
		//	m_macroCursor = macroCursor;
		//}
		void InitMacroExpansionCursor(const CXCursor& cursor);
		void ResolveMacroNata(const SResolvingMacroNataContext& context);
		void AddChildAndInitDefault(const CSharedTaggedNode& taggedNode, const CXCursor& cursor, const CXCursor& macroCursor);
		uint32 GetChildrenCount() const
		{
			return static_cast<uint32>(m_vecChild.size());
		}
		CTaggedNode2* GetChild(uint32 idx) const
		{
			return m_vecChild[idx].Get();
		}
		const CXCursor& GetCursor() const
		{
			return m_cursor;
		}

	public:
		virtual bool CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context)
		{
			return false;
		}
		void WriteCopyNataCode(CCodeLines& linesCopy) const;
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
		void WriteCopyNataCodeExtractBuiltinMetadata(CCodeLines& linesCopy, CBuiltinMetadata& builtinMetadata, CGenLog* log) const;
#endif

	public:
		const CXCursor& DebugGetMacroCursor() const
		{
			return m_macroCursor;
		}
		const Niflect::TArrayNif<CSharedTaggedNode>& DebugGetChildren() const
		{
			return m_vecChild;
		}
		void DebugPrint(FILE* fp, uint32 level) const;

	protected:
		virtual void DebugDerivedPrint(FILE* fp) const {}

	protected:
		CXCursor m_macroCursor;
		Niflect::TArrayNif<CSharedTaggedNode> m_vecChild;
		Niflect::TArrayNif<Niflect::CString> m_linesRawNata;

	private:
		CXCursor m_cursor;
	};

	void WriteNataArgNullOrVar(const CCodeLines& linesNata, CCodeLines& linesCopy, Niflect::CString& nataNullOrVar);
	bool ExtractBuiltinMetadata(const Niflect::CString& keyword, const Niflect::CString& firstLine, Niflect::CString& metadata, uint32& movedPos);
}