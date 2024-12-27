#pragma once
#include "Niflect/NiflectBase.h"
#include <functional>
#include "clang-c/Index.h"

namespace NiflectGen
{
	extern const CXCursor g_invalidCursor;

	typedef std::function<void(const CXCursor& handle)> VisitEnterFunc;
	typedef std::function<void(const CXCursor& handle)> VisitLeaveFunc;

	class CVisitCallbacks
	{
	public:
		VisitEnterFunc m_EnterFunc;
		VisitLeaveFunc m_LeaveFunc;
	};

	void VisitCursorRecurs(const CXCursor& handle, const CVisitCallbacks& callbacks);

	void DebugPrintCursor(FILE* fp, const CXCursor& handle, int level);

	Niflect::CString CXStringToCString(const CXString& cxStr);
	bool FindTagByDisplayName(const CXCursor& cursor, const Niflect::CString& tagName);
	bool FindTagByKindAndDisplayName(const CXCursor& cursor, CXCursorKind kindToFind, const Niflect::CString& tagName);
	CXType GetPointerOriginalType(const CXType& pointerType);
	Niflect::CString GetCursorInfoTypeName(const CXCursor& cursor);
	Niflect::CString GetNameFromTypeDeclaration(const CXType& type);
	Niflect::CString GetNameFromCursorOrTypeDeclaration(const CXCursor& cursor, const CXType& type);
	static CXType getUnderlyingType(CXType type) {
		while (type.kind == CXType_Typedef || type.kind == CXType_Elaborated) {
			if (type.kind == CXType_Typedef) {
				type = clang_getTypedefDeclUnderlyingType(clang_getTypeDeclaration(type));
			}
			else if (type.kind == CXType_Elaborated) {
				type = clang_Type_getNamedType(type);
			}
		}
		return type;
	}

	void FindNamespaceAndScopeNameRecursOld(const CXCursor& cursor, Niflect::TArrayNif<Niflect::CString>& vecNamespace, Niflect::TArrayNif<Niflect::CString>& vecScope);
	void FindNamespaceAndScopeNameRecurs2(const CXCursor& cursor, Niflect::TArrayNif<Niflect::CString>& vecScope);
	Niflect::CString GenerateNamespacesAndScopesCode(const CXCursor& cursor);
	Niflect::CString GenerateScopesCodeFromScopeNames(const Niflect::TArrayNif<Niflect::CString>& vecScopeName);
	void RemoveUnnessesaryNamespacees(const Niflect::TArrayNif<Niflect::CString>& vecNamespaceReference, Niflect::TArrayNif<Niflect::CString>& vecNamespace);
	bool IsCursorKindTemplateDecl(const CXCursorKind& kind);
	bool IsCursorTemplateDecl(const CXCursor& cursor);
	bool IsCursorKindAliasDecl(const CXCursorKind& kind);
	bool IsCursorAliasDecl(const CXCursor& cursor);
	bool IsCAccessorClassDecl(const CXCursor& cursor);
	Niflect::CString GetCursorFilePath(const CXCursor& cursor);
	Niflect::CString GetMacroExpansionTokensInString(const CXCursor& cursor);
	Niflect::CString GetCursorFormattedLocationInfo(const CXCursor& cursor);;
}