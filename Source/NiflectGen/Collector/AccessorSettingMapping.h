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
		const CSubcursor& GetAccessorTypeDecl() const//todo: �ƻ�����ΪGetAccessorTypeSubcursor
		{
			return m_subcursorRoot.m_vecChild[Field];
		}
		const CSubcursor& GetBindingTypeDecl() const//todo: �ƻ�����ΪGetBindingTypeSubcursor
		{
			return m_subcursorRoot.m_vecChild[BindingTypeChildStartIndex];
		}
		uint32 GetBindingTypeDeclsCount() const//todo: �ƻ�����ΪGetDimensionalBindingTypeSubcursorsCount
		{
			return static_cast<uint32>(m_subcursorRoot.m_vecChild.size()) - BindingTypeChildStartIndex;
		}
		const CSubcursor& GetBindingTypeDecl(uint32 idx) const//todo: �ƻ�����ΪGetDimensionalBindingTypeSubcursor
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
		CXCursor m_actualFieldDeclCursor;//todo: Ӧ����, ��Ϊͨ��aliasChain����
		CSubcursor m_subcursorRoot;
		CAccessorData m_accessorData;//todo: Ӧ����, ��Ϊͨ��AccessorBindingMapping���Ҳ���ȡ��Ӧ��Ϣ
		Niflect::CString m_bindingResocursorName;
		CAccessorSettingResolvedInfo m_accessorSettingResolvedInfo;
	};

	class CAccessorBindingFindingContext
	{
	public:
		CAccessorBindingFindingContext(const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CUntaggedTemplatesMapping& untaggedTemplateMapping, uint32& detailIteratingIdx)
			: m_fieldOrArgCXType(fieldOrArgCXType)
			, m_vecDetailCursor(vecDetailCursor)
			, m_untaggedTemplateMapping(untaggedTemplateMapping)
			, m_outDetailIteratingIdx(detailIteratingIdx)
			, m_continuing(true)
		{
		}

		const CXType& m_fieldOrArgCXType;
		const Niflect::TArrayNif<CXCursor>& m_vecDetailCursor;
		const CUntaggedTemplatesMapping& m_untaggedTemplateMapping;
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

	class CAccessorBindingMapping2
	{
	public:
		void InitPatterns();
		void InitIndexedNodeForField(const SResonodesInitContext& context, const CResolvedCursorNode& parentResonode, const CXCursor& fieldCursor, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, CResolvedCursorNode& resultIndexedParent) const;

	private:
		bool IterateForTemplate(const SResonodesInitContext& context, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, CResolvedCursorNode& resultIndexedParent, uint32& detailIteratingIdx) const;
		bool InitResocursorNodeIfFound(CAccessorBindingFindingContext& ctx, CResolvedCursorNode& resocursorNode) const;
		void FindBindingTypeRecurs(const SResonodesInitContext& context, const CResolvedCursorNode& parentResonode, const CXType& fieldOrArgCXType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CTaggedTypesMapping& taggedMapping, const CUntaggedTemplatesMapping& untaggedTemplateMapping, CResolvedCursorNode& resultIndexedParent, uint32& detailIteratingIdx) const;

	public:
		CCollectedAccessorSettings m_settings;
		TCursorMap<uint32> m_mapCursorToIndex;
		TCXTypeMap<uint32> m_mapCXTypeToIndex;
		TCursorMap<uint32> m_mapSpecializedCursorToIndex; //�����ػ�ģ���� TaggedType ��Ӧ������
	};
	using CSharedAccessorBindingMapping = Niflect::TSharedPtr<CAccessorBindingMapping2>;

	struct SResolvedMappings
	{
		const CAccessorBindingMapping2& m_accessorSetting;
		const CTaggedTypesMapping& m_tagged;
		const CUntaggedTemplatesMapping& m_untaggedTemplate;
	};
}