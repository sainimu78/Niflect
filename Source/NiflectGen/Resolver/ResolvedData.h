#pragma once
#include "NiflectGen/Util/CursorMap.h"
#include "NiflectGen/Collector/CollectionData.h"
#include "NiflectGen/TaggedType/TaggedType.h"

namespace NiflectGen
{
	class CResolvedTaggedType
	{
	public:
		CResolvedTaggedType()
			: m_taggedType(NULL)
			//, m_isBuiltinType(false)
		{

		}
		CResolvedTaggedType(CTaggedType* taggedType)
			: m_taggedType(taggedType)
			//, m_isBuiltinType(false)
		{

		}
		CTaggedType* m_taggedType;
		//bool m_isBuiltinType;
	};

	class CFieldWWWWWWW
	{
	public:
		CFieldWWWWWWW(const CSubcursor& subcursor)
			: m_untaggedType(NULL)
			, m_subcursor(subcursor)
		{

		}
		CTaggedType* m_untaggedType;
		CSubcursor m_subcursor;
	};

	class CAccessorBinding
	{
	public:
		CAccessorBinding(const CSubcursor& accessorSubcursor, const CXCursor& actualFieldDeclCursor, const CAccessorData& accessorData)
			: m_accessorSubcursor(accessorSubcursor)
			, m_actualFieldDeclCursor(actualFieldDeclCursor)
#ifdef TAGGED_REQUIREMENT_OF_NON_TEMPLATE_ACCESSOR_TYPE
			, m_accessorTaggedType(NULL)
#else
#endif
			, m_accessorData(accessorData)
		{
		}
		bool Is1D() const
		{
			return m_vecWWWW.size() == 1;
		}
		bool Is2D() const
		{
			return m_vecWWWW.size() == 2;
		}
		CSubcursor m_accessorSubcursor;
		CXCursor m_actualFieldDeclCursor;
#ifdef TAGGED_REQUIREMENT_OF_NON_TEMPLATE_ACCESSOR_TYPE
		CTaggedType* m_accessorTaggedType;
#else
#endif
		Niflect::TArrayNif<CFieldWWWWWWW> m_vecWWWW;
		CAccessorData m_accessorData;
	};

	struct SAccessorBindingIndexAndBasicBindingCXType
	{
		uint32 m_accessorBindingIndex;
		CXType m_CXType;
	};

	class CAccessorBindingMapping
	{
	public:
		//const CAccessorBinding* FindByTypeElaborated(const CXType& type, bool& isTemplate) const
		//{
		//	auto cursorDecl = clang_getTypeDeclaration(type);
		//	auto templateDecl = clang_getSpecializedCursorTemplate(cursorDecl);//cursor为FieldDecl时, template实例的成员cursorDecl仍为ClassDecl, 因此使用该函数获取BindingSetting中注册的类型为TemplateDecl的cursorDecl
		//	if (!clang_Cursor_isNull(templateDecl))
		//	{
		//		cursorDecl = templateDecl;
		//		isTemplate = true;
		//	}
		//	if (clang_getCursorKind(cursorDecl) != CXCursor_NoDeclFound)
		//		return this->FindByCursorDecl(cursorDecl);
		//	return this->FindByCXType(type);
		//}
		const CAccessorBinding* FindByCXType(const CXType& type) const
		{
			auto itFound = m_mapBasicTypeKindToAccessorBindingIndex.find(type.kind);
			if (itFound != m_mapBasicTypeKindToAccessorBindingIndex.end())
				return &m_vecAccessorBinding2[itFound->second.m_accessorBindingIndex];
			return NULL;
		}
		const CAccessorBinding* FindByCursorDecl(const CXCursor& cursorDecl) const
		{
			auto itFound = m_mapBasicCusorDeclToAccessorBindingIndex.find(cursorDecl);
			if (itFound != m_mapBasicCusorDeclToAccessorBindingIndex.end())
				return &m_vecAccessorBinding2[itFound->second];
			return NULL;
		}
		Niflect::TArrayNif<CAccessorBinding> m_vecAccessorBinding2;
		Niflect::TMap<CXTypeKind, SAccessorBindingIndexAndBasicBindingCXType> m_mapBasicTypeKindToAccessorBindingIndex;
		TCursorMap<uint32> m_mapBasicCusorDeclToAccessorBindingIndex;
	};

	class CResolvedMapping
	{
	public:
		CAccessorBindingMapping m_accessorBindingMapping;
		TCursorMap<CTaggedType*> m_mapCursorDeclToTaggedType;
		TCursorMap<CUntaggedTemplate*> m_mapCursorDeclToUntaggedTemplate;
	};

	struct SModuleRegIndicesAndIncludePath
	{
		Niflect::CString m_includePath_reserved;
		uint32 m_includePathPrivateHIndex;
	};

	struct SModuleRegAndTypeRegMapping
	{
		Niflect::TMap<Niflect::CString, SModuleRegIndicesAndIncludePath> m_mapOriginalFilePathToModuleRegIndicesAndIncPath;
		Niflect::TArrayNif<uint32> m_vecTypeRegIndices;
		Niflect::TArrayNif<const Niflect::CString*> m_vecTypeRegIncludePathPrivateHRef;
	};

	class CTypeRegGenFileInfo
	{
	public:
		CTypeRegGenFileInfo(const Niflect::CString& prevateHIncludePath, const Niflect::CString& genHIncludePath)
			: m_prevateHIncludePath(prevateHIncludePath)
			, m_genHIncludePath(genHIncludePath)
		{

		}
		Niflect::CString m_prevateHIncludePath;
		Niflect::CString m_genHIncludePath;
		Niflect::TArrayNif<uint32> m_vecTypeRegDataIndex;
	};

	class CResolvedData
	{
	public:
		////Niflect::TArrayNif<SResolvedType> m_vecResolvedType;//由于解析时已按照用户的BindingSetting.h中include DFS, 因此builtin类型的accessor一定在其它依赖builtin类型的类型之前
		//Niflect::TArrayNif<CResolvedTypes> m_vecResolvedTypes;//由于解析时已按照用户的BindingSetting.h中include DFS, 因此builtin类型的accessor一定在其它依赖builtin类型的类型之前
		//CAccessorTypeTable m_accessorTypeTable;
		//TCursorMap<CTaggedType*> m_mapDeclarationToTaggedType;

		Niflect::TArrayNif<CResolvedTaggedType> m_vecResolvedTypes2;
		//Niflect::TArrayNif<CAccessorBinding> m_vecAccessorBinding;
		//TCursorMap<CTaggedType*> m_mapCusorDeclToTaggedType;
		CResolvedMapping deprecated_m_mapping;


		SModuleRegAndTypeRegMapping m_regMapping;

		TCursorMap<CUntaggedTemplate*> deprecated_m_mapCursorDeclToUntaggedTemplate;
		Niflect::TArrayNif<CSharedTypeRegCodeWriter> m_vecWriter;
		Niflect::TArrayNif<CTypeRegGenFileInfo> m_vecTypeRegGenFileInfo;
		CResolvedCursorRootsMapping m_signatureMapping;
		CSharedAccessorBindingMapping m_accessorBindingMapping;
		CTaggedTypesMapping m_taggedMapping;
		CUntaggedTemplatesMapping m_untaggedTemplateMapping;
	};
}