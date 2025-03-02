#pragma once
#include "Niflect/NiflectBase.h"
#include "NiflectGen/Collector/TypeDecl.h"
#include "NiflectGen/Collector/AccessorData.h"
#include "NiflectGen/Util/CursorMap.h"
#include "NiflectGen/Resolver/ResocursorNode.h"
#include "NiflectGen/Collector/UntaggedTemplateMapping.h"
#include "NiflectGen/Log/Log.h"

namespace NiflectGen
{
	class CTaggedType;
	class CTaggedTypesMapping;

	class CAccessorResocursorNodeInfo
	{
	public:
		Niflect::CString m_resocursorName;
		Niflect::CString m_requiredHeaderFilePath;
	};

	class CAccessorSettingResolvedInfo
	{
	public:
		CAccessorSettingResolvedInfo()
			: m_isPointerTemplate(false)
		{
		}
		CAccessorResocursorNodeInfo m_resoInfo;
		bool m_isPointerTemplate;
	};

	class CBindingSettingData
	{
		enum ESetting
		{
			Field,
			BindingTypeChildStartIndex,
			ElementBindingTypeChildStartIndex,
			Count,
		};
	public:
		CBindingSettingData()
			: deprecated_m_accessorBaseCursorDecl(g_invalidCursor)
			, m_actualFieldDeclCursor(g_invalidCursor)
		{
		}
		const CSubcursor& GetAccessorTypeDecl() const//todo: 计划改名为GetAccessorTypeSubcursor
		{
			return m_subcursorRoot.m_vecChild[Field];
		}
		const CSubcursor& GetBindingTypeDecl() const//todo: 计划改名为GetBindingTypeSubcursor
		{
			return m_subcursorRoot.m_vecChild[BindingTypeChildStartIndex];
		}
		uint32 GetBindingTypeDeclsCount() const//todo: 计划改名为GetDimensionalBindingTypeSubcursorsCount
		{
			return static_cast<uint32>(m_subcursorRoot.m_vecChild.size()) - BindingTypeChildStartIndex;
		}
		const CSubcursor& GetBindingTypeDecl(uint32 idx) const//todo: 计划改名为GetDimensionalBindingTypeSubcursor
		{
			return m_subcursorRoot.m_vecChild[BindingTypeChildStartIndex + idx];
		}
		uint32 GetELementBindingTypeDeclsCountOld() const
		{
			return static_cast<uint32>(m_subcursorRoot.m_vecChild.size()) - ElementBindingTypeChildStartIndex;
		}
		const CSubcursor& GetElementBindingTypeDeclOld(uint32 idx) const
		{
			return m_subcursorRoot.m_vecChild[ElementBindingTypeChildStartIndex + idx];
		}
		uint32 GetELementBindingTypeDeclsCount() const
		{
			return static_cast<uint32>(m_subcursorRoot.m_vecChild.size()) - ElementBindingTypeChildStartIndex;
		}
		const CSubcursor& GetELementBindingTypeDecl(uint32 idx) const
		{
			return m_subcursorRoot.m_vecChild[ElementBindingTypeChildStartIndex + idx];
		}
		bool Deprecated_IsValidBindingSetting() const
		{
			bool valid = false;
			if (m_subcursorRoot.m_vecChild.size() >= ElementBindingTypeChildStartIndex && m_subcursorRoot.m_vecChild.size() <= Count)
				valid = true;
			return valid;
		}
		bool IsValid() const
		{
			return m_subcursorRoot.m_vecChild.size() > 0;
		}

	public:
		CXCursor deprecated_m_accessorBaseCursorDecl;
		CXCursor m_actualFieldDeclCursor;//todo: 应废弃, 改为通过aliasChain查找
		CSubcursor m_subcursorRoot;
		CAccessorData m_accessorData;//todo: 应废弃, 改为通过AccessorBindingMapping查找并获取对应信息
		Niflect::CString m_bindingResocursorName;
		CAccessorSettingResolvedInfo m_accessorSettingResolvedInfo;
	};

	class CAccessorBindingFindingContext
	{
	public:
		CAccessorBindingFindingContext(const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CUntaggedTemplatesMapping& untaggedTemplateMapping, const CAliasChain& aliasChain, uint32& detailIteratingIdx)
			: m_fieldOrArgCXType(fieldOrArgCXType)
			, m_vecDetailCursor(vecDetailCursor)
			, m_untaggedTemplateMapping(untaggedTemplateMapping)
			, m_aliasChain(aliasChain)
			, m_outDetailIteratingIdx(detailIteratingIdx)
			, m_continuing(true)
		{
		}

		const CXType& m_fieldOrArgCXType;
		const Niflect::TArrayNif<CXCursor>& m_vecDetailCursor;
		const CUntaggedTemplatesMapping& m_untaggedTemplateMapping;
		const CAliasChain& m_aliasChain;
		uint32& m_outDetailIteratingIdx;
		bool m_continuing;
	};

	class CCollectedAccessorSettings
	{
	public:
		Niflect::TArrayNif<CBindingSettingData> m_vecAccessorBindingSetting;
#ifdef ACCESSOR_SETTING_ABCD
		CBindingSettingData m_settingCompound;
		CBindingSettingData m_settingEnumClass;
		CBindingSettingData m_settingEnumBitMask;
#else
#endif
	};

	struct SResonodesInitContext
	{
		CGenLog* m_log;
	};

	class CPointerCursor
	{
	public:
		CPointerCursor()
			: m_cxType{}
			, m_cursor(g_invalidCursor)
			, m_dim(0)
		{
		}
		CPointerCursor(const CXType& cxType, const CXCursor& cursor, uint32 dim)
			: m_cxType(cxType)
			, m_cursor(cursor)
			, m_dim(dim)
		{
		}
		bool IsValid() const
		{
			return m_dim > 0;
		}

	public:
		CXType m_cxType;
		CXCursor m_cursor;
		uint32 m_dim;
	};

	struct SPointerCursorHash {
		std::size_t operator()(const CPointerCursor& c) const noexcept {
			return c.m_dim + SCursorHash()(c.m_cursor);
		}
	};

	struct SPointerCursorEqual {
		bool operator()(const CPointerCursor& a, const CPointerCursor& b) const noexcept {
			if (a.m_dim == b.m_dim)
				return SCursorEqual()(a.m_cursor, b.m_cursor);
			return false;
		}
	};
	template <typename TValue>
	using TPointerCursorMap = Niflect::TUnorderedMap<CPointerCursor, TValue, SPointerCursorHash, SPointerCursorEqual>;

	class CAccessorBindingMapping2
	{
	public:
		void InitPatterns();
		void InitIndexedNodeForField(const SResonodesInitContext& context, const CResolvedCursorNode& parentResonode, const CXCursor& fieldCursor, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, const CAliasChain& aliasChain, CResolvedCursorNode& resultIndexedParent) const;

	private:
		bool IterateForTemplate(const SResonodesInitContext& context, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, const CAliasChain& aliasChain, CResolvedCursorNode& resultIndexedParent, uint32& detailIteratingIdx) const;
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
		bool InitResocursorNodeIfFoundRecurs(CAccessorBindingFindingContext& ctx, CResolvedCursorNode& resocursorNode) const;
#else
		bool InitResocursorNodeIfFound(CAccessorBindingFindingContext& ctx, CResolvedCursorNode& resocursorNode) const;
#endif
		void FindBindingTypeRecurs(const SResonodesInitContext& context, const CResolvedCursorNode& parentResonode, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, const CAliasChain& aliasChain, CResolvedCursorNode& resultIndexedParent, uint32& detailIteratingIdx) const;

	public:
		CCollectedAccessorSettings m_settings;
		TCursorMap<uint32> m_mapCursorToIndex;
		TCXTypeMap<uint32> m_mapCXTypeToIndex;
		TCursorMap<uint32> m_mapSpecializedCursorToIndex; //包括特化模板与 TaggedType 对应的类型
		TPointerCursorMap<uint32> m_mapPointerCursorToIndex;
	};
	using CSharedAccessorBindingMapping = Niflect::TSharedPtr<CAccessorBindingMapping2>;

	struct SResolvedMappings
	{
		const CAccessorBindingMapping2& m_accessorSetting;
		const CTaggedTypesMapping& m_tagged;
		const CUntaggedTemplatesMapping& m_untaggedTemplate;
		const CAliasChain& m_aliasChain;
	};

	CPointerCursor GetPointerCursorFromPointerType(const CXType& pointerType);
}