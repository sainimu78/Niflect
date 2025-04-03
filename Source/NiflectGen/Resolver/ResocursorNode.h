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
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		Niflect::CString GetBuildTypeMetaFuncName(const Niflect::CString& prefix) const;
#else
		Niflect::CString GetCreateTypeAccessorFuncName(const Niflect::CString& prefix) const;
#endif
		Niflect::CString GetStaticGetTypeFuncName(const Niflect::CString& prefix) const;
		bool IsTaggedType() const;
		Niflect::CString GetResocursorInstanceName() const;

	public:
		const Niflect::TArrayNif<Niflect::CString>& DebugGetHeaderFilePaths() const { return m_vecRequiredHeaderFilePath; }

	public:
		uint32 m_accessorBindingIndex;
		uint32 m_taggedTypeIndex;//该索引对应的类型一定不是模板, 现可为 class, struct, enum, 如指定了对应的 AccessorBinding, 则具有 m_accessorBindingIndex
		uint32 m_untaggedTemplateIndex;//该索引有效时, 表明指定了对应的 AccessorBinding, 即具有 m_accessorBindingIndex
		Niflect::TArrayNif<CResolvedCursorNode> m_vecChild;
		Niflect::CString m_key;
		Niflect::CString m_resocursorName;
		Niflect::CString m_qualifiers;
		Niflect::CString m_referenceModifier;
		//要求与 m_vecChild 互斥, 即无法支持容器模板含成员变量, 而结构模板可含成员变量
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