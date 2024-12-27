#pragma once
#include "Niflect/NiflectBase.h"
#include "Niflect/NiflectAddr.h"
#include "Niflect/Serialization/RwTree.h"
#include "Niflect/NiflectNata.h"

namespace Niflect
{
	using namespace RwTree;

	class CNiflectType;

	class CAccessor;
	using CSharedAccessor = TSharedPtr<CAccessor>;

	class CTypeLayout
	{
	public:
		bool AccessorsSaveToRwNode(const AddrType base, CRwNode* rw) const;
		bool AccessorsLoadFromRwNode(AddrType base, const CRwNode* rw) const;
		Niflect::TArrayNif<CSharedAccessor> m_vecAccessor;
	};

	class CField
	{
		friend class CNiflectType;
	public:
		void Init(const Niflect::CString& name, const CSharedNata& nata)
		{
			m_name = name;
			m_nata = nata;
		}
		void InitAddAccessor(const CSharedAccessor& accessor)
		{
			m_layout.m_vecAccessor.push_back(accessor);
		}
		const Niflect::CString& GetName() const
		{
			return m_name;
		}
		const Niflect::TArrayNif<CSharedAccessor>& GetAccessors() const
		{
			return m_layout.m_vecAccessor;
		}
		bool LayoutSaveToRwNode(const AddrType base, CRwNode* rw) const
		{
			return m_layout.AccessorsSaveToRwNode(base, rw);
		}
		bool LayoutLoadFromRwNode(AddrType base, const CRwNode* rw) const
		{
			return m_layout.AccessorsLoadFromRwNode(base, rw);
		}

	private:
		Niflect::CString m_name;
		CTypeLayout m_layout;
		CSharedNata m_nata;
	};

	class CAccessor
	{
	public:
		CAccessor()
			: m_type(NULL)
		{
		}

	public:
		bool SaveToRwNode(const AddrType base, CRwNode* rw) const
		{
			return this->SaveInstanceImpl(this->GetAddrFromBase(base), rw);
		}
		bool LoadFromRwNode(AddrType base, const CRwNode* rw) const
		{
			return this->LoadInstanceImpl(this->GetAddrFromBase(base), rw);
		}

	public:
		void InitMemberMeta(const CString& name, const AddrOffsetType& offset)
		{
			ASSERT(false);
			//m_name = name;
			m_addrOffset.SetOffset(offset);
		}

	public:
		void InitType(CNiflectType* type)
		{
			ASSERT(false);
			m_type = type;
			//ASSERT(m_name.empty());
			ASSERT(m_addrOffset.GetOffset() == CAddrOffset::None);
		}
		void InitForField(const CString& name, const AddrOffsetType& offset)
		{
			ASSERT(false);
			//ASSERT(m_name.empty());
			//m_name = name;
			ASSERT(m_addrOffset.GetOffset() == CAddrOffset::None);
			m_addrOffset.SetOffset(offset);
		}
		void InitForElement()
		{
			ASSERT(false);//
			//ASSERT(m_name.empty());
			//m_name = "reserved_dim";
			ASSERT(m_addrOffset.GetOffset() == CAddrOffset::None);
		}
		void InitElementAccessor(const CSharedAccessor& accessor)
		{
			ASSERT(false);
			//m_elemAccessor = accessor;
		}
		void AddChild(const CSharedAccessor& accessor)
		{
			this->InsertChild(accessor, this->GetChildrenCount());
		}
		void InsertChild(const CSharedAccessor& accessor, uint32 idx)
		{
			ASSERT(false);
			//m_vecChild.insert(m_vecChild.begin() + idx, accessor);
		}

	public:
		void InitType2(CNiflectType* type)
		{
			m_type = type;
			ASSERT(m_addrOffset.GetOffset() == CAddrOffset::None);
		}
		void InitOffset(const AddrOffsetType& offset)
		{
			ASSERT(m_addrOffset.GetOffset() == CAddrOffset::None);
			m_addrOffset.SetOffset(offset);
		}
		void InitAddField(const CField& field)
		{
			this->InsertField(field, this->GetFieldsCount());
		}
		void InitElementLayout(const CTypeLayout& layout)
		{
			m_elemLayout = layout;
		}

	private:
		void InsertField(const CField& field, uint32 idx)
		{
			m_vecField.insert(m_vecField.begin() + idx, field);
		}

	public:
		CNiflectType* GetType() const
		{
			return m_type;
		}
		const CString& GetName() const
		{
			//return m_name;
			ASSERT(false);
			static Niflect::CString s_a;
			return s_a;
		}
		uint32 GetChildrenCount() const
		{
			//return static_cast<uint32>(m_vecChild.size());
			ASSERT(false);
			return 0;
		}
		uint32 GetFieldsCount() const
		{
			return static_cast<uint32>(m_vecField.size());
		}
		const Niflect::TArrayNif<CField>& GetFields() const
		{
			return m_vecField;
		}
		CAccessor* GetChild(uint32 idx) const
		{
			//return m_vecChild[idx].Get();
			ASSERT(false);
			return NULL;
		}
		CAccessor* GetElementAccessor() const
		{
			ASSERT(false);
			//return m_elemAccessor.Get();
			return NULL;
		}
		const CTypeLayout& GetElementLayout() const
		{
			return m_elemLayout;
		}

	protected:
		virtual bool SaveInstanceImpl(const AddrType base, CRwNode* rw) const = 0;
		virtual bool LoadInstanceImpl(AddrType base, const CRwNode* rw) const = 0;

	private:
		inline const AddrType GetAddrFromBase(const AddrType& base) const
		{
			return static_cast<const char*>(base) + m_addrOffset.GetOffset();
		}
		inline AddrType GetAddrFromBase(AddrType& base) const
		{
			return static_cast<char*>(base) + m_addrOffset.GetOffset();
		}

	private:
		CAddrOffset m_addrOffset;
		//CString m_name;
		TArrayNif<CField> m_vecField;
		//CSharedAccessor m_elemAccessor;
		CTypeLayout m_elemLayout;
		CNiflectType* m_type;
	};

	inline bool CTypeLayout::AccessorsSaveToRwNode(const AddrType base, CRwNode* rw) const
	{
		for (auto& it : m_vecAccessor)
		{
			if (!it->SaveToRwNode(base, rw))
				return false;
		}
		return true;
	}
	inline bool CTypeLayout::AccessorsLoadFromRwNode(AddrType base, const CRwNode* rw) const
	{
		for (auto& it : m_vecAccessor)
		{
			if (!it->LoadFromRwNode(base, rw))
				return false;
		}
		return true;
	}

	template <typename TType>
	CSharedAccessor CreateTypeAccessor();
}