#pragma once
#include "Niflect/NiflectBase.h"

namespace NiflectGen
{
	class CResolvedCursorNode;
	using CSharedResolvedCursorNode = Niflect::TSharedPtr<CResolvedCursorNode>;

	class CResolvedCursorNode
	{
	public:
		CResolvedCursorNode()
			: m_accessorBindingIndex(INDEX_NONE)
			, m_taggedTypeIndex(INDEX_NONE)
			, m_untaggedTemplateIndex(INDEX_NONE)
		{
		}
		void InitForAccessorBinding(uint32 accessorBindingIdx, uint32 untaggedTemplateIndex, const Niflect::CString& headerFilePath);
		void InitForTemplateBegin(const Niflect::CString& signature);
		void InitForTemplateArguments(const CResolvedCursorNode& childrenOwner, bool isTemplateFormat);
		void InitForTemplateEnd();
		void InitForTemplate(const Niflect::CString& signature, const CResolvedCursorNode& childrenOwner, bool isTemplateFormat);
		void InitForClassDecl(const Niflect::CString& resocursorName, uint32 taggedTypeIdx, uint32 accessorBindingIdx, const Niflect::CString& headerFilePath);
		bool IsValid() const
		{
			return m_accessorBindingIndex != INDEX_NONE || m_taggedTypeIndex != INDEX_NONE;
		}

	public:
		const Niflect::CString* GetHeaderFilePathAddrForTaggedType() const;
		void GetHeaderFilePathAddrs(Niflect::TArrayNif<const Niflect::CString*>& vecAddr) const;
		Niflect::CString GetResocursorNameForLastTemplateArg() const;
		Niflect::CString GetCreateTypeAccessorFuncName(const Niflect::CString& prefix) const;
		Niflect::CString GetStaticGetTypeFuncName(const Niflect::CString& prefix) const;
		bool IsTaggedType() const;

	public:
		const Niflect::TArrayNif<Niflect::CString>& DebugGetHeaderFilePaths() const { return m_vecRequiredHeaderFilePath; }

	public:
		uint32 m_accessorBindingIndex;
		uint32 m_taggedTypeIndex;//��������Ӧ������һ������ģ��, �ֿ�Ϊ class, struct, enum, ��ָ���˶�Ӧ�� AccessorBinding, ����� m_accessorBindingIndex
		uint32 m_untaggedTemplateIndex;//��������Чʱ, ����ָ���˶�Ӧ�� AccessorBinding, ������ m_accessorBindingIndex
		Niflect::TArrayNif<CResolvedCursorNode> m_vecChild;
		Niflect::CString m_key;
		Niflect::CString m_resocursorName;
		//Ҫ���� m_vecChild ����, ���޷�֧������ģ�庬��Ա����, ���ṹģ��ɺ���Ա����
		CSharedResolvedCursorNode m_elem;

	private:
		Niflect::TArrayNif<Niflect::CString> m_vecRequiredHeaderFilePath;
	};

	class CResolvedCursorRoots
	{
	public:
		CResolvedCursorRoots()
		{
		}
		CResolvedCursorRoots(const Niflect::CString& signatureStr, const CResolvedCursorNode& resoRoot)
			: m_resoRoot(resoRoot)
		{
		}
		Niflect::CString deprecated_m_signatureStr;
		CResolvedCursorNode m_resoRoot;
	};

	class CResolvedCursorRootsMapping
	{
	public:
		void SSSSSSSS();
		void DebugGenSignatures(Niflect::TArrayNif<Niflect::CString>& vecSignature);
		Niflect::TArrayNif<CResolvedCursorRoots> m_vecItem;
		Niflect::TMap<Niflect::CString, uint32> m_mapKeyToIndex;
	};
}