#pragma once
#include "NiflectGen/Resolver/ResolvedData.h"
#include "NiflectGen/Log/Log.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCode.h"

namespace NiflectGen
{
	//struct SResolvedType
	//{
	//	CTaggedType* m_taggedType;
	//	Niflect::CString m_filePathAbs;
	//};

	//class CAccessorTypeTable
	//{
	//public:
	//	TCursorMap<CTaggedType*> m_mapUserType;
	//	TCursorMap<CTaggedType*> m_mapUserTypePointer1D_reserved;
	//	Niflect::TMap<CXTypeKind, CTaggedType*> m_mapBuiltinTypeKind;
	//};

	//class CResolvedTypes
	//{
	//public:
	//	Niflect::CString m_filePathAbs;
	//	Niflect::CString m_filePathRelativeToHeaderSearchPath;
	//	Niflect::TArrayNif<CResolvedTaggedType> m_vecResolvedTaggedType;
	//};

	class CResolvingContext
	{
	public:
		CResolvingContext(CGenLog* log)
			: m_log(log)
		{
		}

	public:
		CGenLog* m_log;
	};

	class CAccessorBindingItemMapping
	{
	public:
		CAccessorBindingItemMapping()
			: m_taggedType(NULL)
		{
		}
		CTaggedType* m_taggedType;
		Niflect::TArrayNif<uint32> m_vecAccessorBindingIndices;
	};

	class CResolver
	{
	public:
		CResolver(const CCollectionData& collectionData, const CModuleRegInfoValidated& moduleRegInfo);
		//void Resolve(CTaggedNode2* taggedRoot, CResolvingContext& context, CResolvedData& data);
		void Resolve4(CTaggedNode2* taggedRoot, const CResolvingContext& context, CResolvedData& data);

	private:
		//void ResolveRecurs(CTaggedNode2* taggedParent, CResolvingContext& context, CResolvedData& data);
		void ResolveRecurs2(CTaggedNode2* taggedParent, CResolvingContext& context, CResolvedData& data);
		void ResolveRecurs4(const CResolvingContext& context, CTaggedNode2* taggedParent, bool isGlobalsType, CResolvedData& data, CTaggedTypesMapping& resolvedMapping, CUntaggedTemplatesMapping& untaggedTemplatesMapping);

	//public:
	//	void DebugFinish(const CResolvedData& data) const;

	//private:
	//	class CAccessorBindingArrayInfo
	//	{
	//	public:
	//		CAccessorBindingArrayInfo()
	//			: m_foundCursorsCount(0)
	//		{

	//		}
	//		TCursorMap<uint32> m_mapDeclToIndex;
	//		Niflect::TArrayNif<CAccessorBindingItemMapping> m_vecItemMapping;
	//		uint32 m_foundCursorsCount;
	//	};

	private:
		const CCollectionData& m_collectionData;
		//const Niflect::TArrayNif<Niflect::CString>& m_vecHeaderSearchPath;
		//Niflect::TMap<Niflect::CString, uint32> m_mapFilePathToResolvedTypeIndex;
		//TCursorMap<CTaggedType*> m_mapDeclarationToTaggedType;

		//Niflect::TSet<CXTypeKind> m_setBindingTypeKind;
		//CCursorSet m_setBindingCurosrDecl;
		//CAccessorBindingArrayInfo m_accessorBindingArrayInfo;
		TCursorMap<CTaggedType**> m_mapAccessorCursorDeclToAccessorBindingIndex;
		uint32 m_foundCursorsCount;
		const CModuleRegInfoValidated& m_moduleRegInfo;
	};

	void SortTaggedNodesInDependencyOrder(CTaggedNode2* taggedParent, Niflect::TArrayNif<CTaggedType*>& vecOrderedType);
}