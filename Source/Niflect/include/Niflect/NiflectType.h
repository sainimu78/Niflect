#pragma once
#include "Niflect/NiflectCommon.h"
#include "Niflect/NiflectRegisteredType.h"
#include "Niflect/NiflectMethod.h"
#include "Niflect/NiflectAccessor.h"

namespace Niflect
{

	typedef CSharedAccessor (*CreateTypeAccessorFunc)();
	using CreateFieldLayoutOfTypeFuncOld = CreateTypeAccessorFunc;

	struct STypeLifecycleMeta
	{
		uint32 m_typeSize;
		InvokeConstructorFunc m_InvokeConstructorFunc;//һЩ��������г��� Ctor �� Dtor
		InvokeDestructorFunc m_InvokeDestructorFunc;
	};

	class CNiflectType;
	using CStaticNiflectTypeAddr = CNiflectType*;

	class CNiflectType
	{
	public:
		CNiflectType()
			: m_tableIdx(INDEX_NONE)
			, m_lifecycleMeta{}
			, m_CreateTypeAccessorFunc(NULL)
			, m_staticTypePtrAddr(NULL)
			, m_typeHash(0)
		{
		}
		~CNiflectType()
		{
			if (m_staticTypePtrAddr != NULL)
				*m_staticTypePtrAddr = NULL;
		}

	public:
		void InitTypeMeta(uint32 niflectTypeSize, size_t typeHash, const CString& name, uint32 index, const STypeLifecycleMeta& cb)
		{
			ASSERT(false);
			//m_name = name;
			//m_index = index;
			//m_niflectTypeSize = niflectTypeSize;
			//m_cb = cb;
			//m_typeHash = typeHash;
		}
		void InitTypeMeta2(const STypeLifecycleMeta& lifecycleMeta, const CreateTypeAccessorFunc& inCreateTypeAccessorFunc, size_t typeHash, uint32 tableIdx, const CString& id, CStaticNiflectTypeAddr* staticTypePtrAddr, const CSharedNata& nata)
		{
			m_name = id;
			m_tableIdx = tableIdx;
			m_nata = nata;
			m_lifecycleMeta = lifecycleMeta;
			m_CreateTypeAccessorFunc = inCreateTypeAccessorFunc;
			m_staticTypePtrAddr = staticTypePtrAddr;
			*m_staticTypePtrAddr = this;
			m_typeHash = typeHash;
		}
		
	public:
		//CField* GetRootField() const
		//{
		//	return m_fieldRoot.Get();
		//}
		//const CSharedAccessor& GetSharedAccessorRoot() const
		//{
		//	return m_accessorRoot;
		//}
		const uint32& GetTableIndex() const//todo: �ƻ�����Ϊ GetTableIndex
		{
			return m_tableIdx;
		}
		const CString& GetTypeName() const//todo: �ƻ�����Ϊ GetNativeTypeName
		{
			return m_name;
		}
		const uint32& GetTypeSize() const//todo: �ƻ�����Ϊ GetNativeTypeSize
		{
			return m_lifecycleMeta.m_typeSize;//����C++ Built in����, ��������Ϊconst ref��Ϊ�˷��㸳ֵ������auto
		}
		const CreateTypeAccessorFunc& GetCreateTypeAccessorFunc() const
		{
			return m_CreateTypeAccessorFunc;
		}
		const STypeLifecycleMeta& GetLifecycleMeta() const
		{
			return m_lifecycleMeta;
		}

	public:
		bool SaveInstanceToRwNode(const AddrType base, CRwNode* rw) const
		{
			for (auto& it : m_layout.m_vecAccessor)
			{
				if (!it->SaveToRwNode(base, rw))
					return false;
			}
			return true;
		}
		bool LoadInstanceFromRwNode(AddrType base, const CRwNode* rw) const
		{
			for (auto& it : m_layout.m_vecAccessor)
			{
				if (!it->LoadFromRwNode(base, rw))
					return false;
			}
			return true;
		}

	public:
		//void Construct(void* instanceBase) const
		//{
		//	if (m_cb.m_InvokeConstructorFunc != NULL)
		//		m_cb.m_InvokeConstructorFunc(instanceBase);
		//}
		//void Destruct(void* instanceBase) const
		//{
		//	if (m_cb.m_InvokeDestructorFunc != NULL)
		//		m_cb.m_InvokeDestructorFunc(instanceBase);
		//}
		CSharedAccessor CreateFieldLayout() const//todo:�ƻ�ɾ��
		{
			ASSERT(false);
			if (m_CreateTypeAccessorFunc != NULL)
				return m_CreateTypeAccessorFunc();
			return NULL;
		}
		CSharedAccessor CreateAccessor() const
		{
			ASSERT(false);
			if (m_CreateTypeAccessorFunc != NULL)
				return m_CreateTypeAccessorFunc();
			return NULL;
		}
		void InitFieldLayout()
		{
			ASSERT(false);//todo: �ƻ�����, ����Ӧ�������м̳����ϵ�layout, ����layoutҲ�����������layout������Ӧ�ĳ�ʼ��
			//todo: δȷ������, �õ��ٴ���������Module��ʼ��ʱͳһ��������, ���ú���ʵ��
			//ASSERT(m_fieldRoot == NULL);
			//m_fieldRoot = this->CreateFieldLayout();
		}
		void InitTypeLayout()
		{
			ASSERT(m_layout.m_vecAccessor.size() == 0);
			this->CreateTypeLayout(m_layout);
		}
		void InitAddFieldToAccessor(CAccessor* owner, const Niflect::CString& name, const AddrOffsetType& offset, const CSharedNata& nata) const
		{
			CField field;
			field.Init(name, nata);
			this->CreateTypeLayout(field.m_layout);
			for (auto& it1 : field.m_layout.m_vecAccessor)
				it1->InitOffset(offset);
			owner->InitAddField(field);
		}
		void InitAccessorElementLayout(CAccessor* owner) const
		{
			CTypeLayout layout;
			this->CreateTypeLayout(layout);
			owner->InitElementLayout(layout);
		}

	protected:
		virtual void CreateTypeLayout(CTypeLayout& layout) const
		{
			if (m_CreateTypeAccessorFunc != NULL)
				layout.m_vecAccessor.push_back(m_CreateTypeAccessorFunc());
		}

	public:
		template <typename TBase>
		TSharedPtr<TBase> MakeSharedInstance() const
		{
			return GenericPlacementMakeShared<TBase, CMemory>(m_lifecycleMeta.m_typeSize, m_lifecycleMeta.m_InvokeDestructorFunc, m_lifecycleMeta.m_InvokeConstructorFunc);
		}
		//template <typename TBase>
		//inline TSharedPtr<TBase> MakeSharableInstance(TBase* obj) const
		//{
		//	return GenericMakeSharable<TBase, CMemory>(obj, m_cb.m_InvokeDestructorFunc);
		//}

	//public:
	//	template <typename T>
	//	inline T& GetInstanceRef(void* parentBase) const
	//	{
	//		return this->GetAccessorRoot()->GetInstanceRef<T>(parentBase);
	//	}
	public:
		//todo: �˺����ɷ���, ��Ϊ��̬������ȡm_typeHash
		template <typename T>
		inline T& GetInstanceRef(AddrType base) const
		{
			auto Check = [this]()
			{
				ASSERT(GetTypeHash<T>() == m_typeHash);
				return true;
			};
			ASSERT(Check());
			return *static_cast<T*>(base);
		}

	public:
		void InitNata(const CSharedNata& nata)
		{
			m_nata = nata;
		}
		CNata* GetNata() const
		{
			return m_nata.Get();
		}

	public:
		template <typename T>
		static size_t GetTypeHash()
		{
			return typeid(T).hash_code();
		}

	//private:
	//	virtual void DebugFuncForDynamicCast() {}//��Ϊ��̬������ͱ������, ���Ѷ���ǵ����õ�virtual��������Ƴ�, ��ע: error C2683: 'dynamic_cast': 'XXX' is not a polymorphic type 

	private:
		//todo: �ƻ�����Ϊ m_id, ͨ��Ϊ natiev type name
		//NiflectGen �����ɵ� id Ϊ�� namespace �� type name, ������� NIFLECT_TYPE_REGISTER ��ע��� id �� namespace, ������Ҫ��������װ.
		//����Ҫ��ȷһ��, id ���������ڱ����ظ������л�ʹ��, ʵ���Ƿ�� namespace ͨ���޹ؽ�Ҫ, һ�������ͨ��������������ּ���
		CString m_name;
		CSharedNata m_nata;
		uint32 m_tableIdx;//todo: �ƻ�����Ϊ m_tableIdx;
		CTypeLayout m_layout;
		STypeLifecycleMeta m_lifecycleMeta;//todo: �ƻ�����Ϊ m_typeFuncs
		CreateTypeAccessorFunc m_CreateTypeAccessorFunc;
		CStaticNiflectTypeAddr* m_staticTypePtrAddr;
		size_t m_typeHash;
	};
	using CSharedNiflectType = TSharedPtr<CNiflectType>;
	
	class CEnumConstMeta
	{
	public:
		CEnumConstMeta()
		{

		}
		CEnumConstMeta(const Niflect::CString& name, const CSharedNata& nata)
			: m_name(name)
			, m_nata(nata)
		{
		}
		CNata* GetNata() const
		{
			return m_nata.Get();
		}
		Niflect::CString m_name;

	private:
		CSharedNata m_nata;
	};

	class CEnumMeta
	{
	public:
		void InitAddConst(const Niflect::CString& name, const CSharedNata& nata)
		{
			m_vecEnumConstMeta.push_back(CEnumConstMeta(name, nata));
		}
		Niflect::TArrayNif<CEnumConstMeta> m_vecEnumConstMeta;
	};

	class CEnum : public CNiflectType
	{
		typedef CNiflectType inherited;
	public:
		void InitEnumMeta(const CEnumMeta& data)
		{
			m_enumMeta = data;
		}
		const CEnumMeta& GetEnumMeta() const
		{
			return m_enumMeta;
		}
		const CString& GetEnumConstNameByIndex(uint32 idx) const
		{
			return m_enumMeta.m_vecEnumConstMeta[idx].m_name;
		}
		uint32 FindEnumConstMetaIndex(const CString& name) const
		{
			for (uint32 idx = 0; idx < m_enumMeta.m_vecEnumConstMeta.size(); ++idx)
			{
				if (m_enumMeta.m_vecEnumConstMeta[idx].m_name == name)
					return idx;
			}
			return INDEX_NONE;
		}
		template <typename TEnumType>
		const CString& GetEnumConstName(const TEnumType& e) const
		{
			auto idx = static_cast<uint32>(e);
			return this->GetEnumConstNameByIndex(idx);
		}

	public:
		static CEnum* Cast(CNiflectType* base)
		{
			ASSERT(dynamic_cast<CEnum*>(base) != NULL);
			return static_cast<CEnum*>(base);
		}

	private:
		CEnumMeta m_enumMeta;
	};

	class CFunction : public CNiflectType
	{
	};

	class CInheritableType : public CNiflectType
	{
		typedef CNiflectType inherited;
	public:
		CInheritableType()
			: m_parent(NULL)
		{
		}
		void InitInheritableTypeMeta(CInheritableType* parent)
		{
			m_parent = parent;
		}
		CInheritableType* GetParent() const
		{
			return m_parent;
		}

	protected:
		virtual void CreateTypeLayout(CTypeLayout& layout) const
		{
			inherited::CreateTypeLayout(layout);
			if (m_parent != NULL)
			{
				ASSERT(layout.m_vecAccessor.size() == 1);
				auto par = m_parent;
				while (par != NULL)
				{
					auto& Func = par->GetCreateTypeAccessorFunc();
					ASSERT(Func != NULL);
					layout.m_vecAccessor.insert(layout.m_vecAccessor.begin(), Func());
					par = par->m_parent;
				}
			}
		}

	//private:
	//	static void GetStackedTypes(CNiflectType* derivedType, bool reversed, TArrayNif<CInheritableType*>& vecStackedType)
	//	{
	//		auto baseType = CInheritableType::CastChecked(derivedType);
	//		while (baseType != NULL)
	//		{
	//			if (!reversed)
	//				vecStackedType.insert(vecStackedType.begin(), baseType);
	//			else
	//				vecStackedType.push_back(baseType);
	//			baseType = baseType->GetParent();
	//		}
	//	}

	//public:
	//	inline static void GetStackedTypesConstructionOrder(CNiflectType* derivedType, TArrayNif<CInheritableType*>& vecStackedType)
	//	{
	//		GetStackedTypes(derivedType, false, vecStackedType);
	//	}
	//	inline static void GetStackedTypesDestructionOrder(CNiflectType* derivedType, TArrayNif<CInheritableType*>& vecStackedType)
	//	{
	//		GetStackedTypes(derivedType, true, vecStackedType);
	//	}

	public:
		static CInheritableType* Cast(CNiflectType* base)
		{
			ASSERT(dynamic_cast<CInheritableType*>(base) != NULL);
			return static_cast<CInheritableType*>(base);
		}
		static CInheritableType* CastChecked(CNiflectType* base)
		{
			return dynamic_cast<CInheritableType*>(base);
		}

	private:
		CInheritableType* m_parent;//todo: Ӧ����AddChild�����㼶��ϵ; ��һ����, ���ܻ���Ҫ����������������ù�ϵ, Ŀǰ��ȷ�����ù�ϵ�Ǿ�̬��̬����
	};

	class CStruct : public CInheritableType
	{
		typedef CInheritableType inherited;
	public:
		static CStruct* Cast(CNiflectType* base)
		{
			ASSERT(dynamic_cast<CStruct*>(base) != NULL);
			return static_cast<CStruct*>(base);
		}
	};
	
	//todo: �����Ƿ��CStruct�̳��Ը������͹�ϵ, ��m_parent�ǲ�ͬ��, ���������CClass��m_parentʹ��CStruct��m_parent����ӦCast, �Ƿ�ɽ���
	class CClass : public CInheritableType
	{
		typedef CInheritableType inherited;
	public:
		static CClass* Cast(CNiflectType* base)
		{
			ASSERT(dynamic_cast<CClass*>(base) != NULL);
			return static_cast<CClass*>(base);
		}

	public:
		TArrayNif<CNiflectMethod2> m_vecMethod;
	};
}