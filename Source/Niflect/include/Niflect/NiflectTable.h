#pragma once
#include "Niflect/NiflectType.h"

namespace Niflect
{
	//ΪModule������CNiflectType������, ����Ҫ���̳�
	class CNiflectTable
	{
	public:
		void Init(const CString& name)
		{
			m_name = name;
		}
		template <typename TInfo, typename TType>
		void RegisterType(const CString& typeName, const CreateFieldLayoutOfTypeFuncOld& Func)
		{
			ASSERT(false);
			//STypeLifecycleFunctions typeFuncs;
			//typeFuncs.m_InvokeConstructorFunc = &GenericInstanceInvokeConstructor<TType>;
			//typeFuncs.m_InvokeDestructorFunc = &GenericInstanceInvokeDestructor<TType>;
			//typeFuncs.m_CreateTypeAccessorFunc = Func;

			//auto shared = MakeShared<TInfo>();
			//auto type = shared.Get();
			//auto idx = this->AddType(shared);
			//ASSERT(!TRegisteredType<TType>::IsValid());
			//TRegisteredType<TType>::s_type = type;
			////type->InitStaticType<TType>();
			//type->InitTypeMeta(sizeof(TType), CNiflectType::GetTypeHash<TType>(), typeName, idx, typeFuncs);
			//ASSERT(TRegisteredType<TType>::IsValid());
		}
		template <typename TType, typename TInfo = CNiflectType>
		void RegisterType2(const Niflect::CString& id, const CreateTypeAccessorFunc& Func)
		{
			ASSERT(false);
			//CTypeInvokations typeFuncs;
			//typeFuncs.m_InvokeConstructorFunc = &GenericInstanceInvokeConstructor<TType>;
			//typeFuncs.m_InvokeDestructorFunc = &GenericInstanceInvokeDestructor<TType>;
			//typeFuncs.m_CreateTypeAccessorFunc = Func;

			//auto shared = Niflect::MakeShared<TInfo>();
			//CNiflectType* type = shared.Get();
			//auto idx = this->AddType(shared);
			//ASSERT(!TRegisteredType<TType>::IsValid());
			//type->InitTypeMeta2(sizeof(TType), CNiflectType::GetTypeHash<TType>(), idx, typeFuncs, id, &TRegisteredType<TType>::s_type, NULL);
			//ASSERT(TRegisteredType<TType>::IsValid());
		}
		template <typename TType, typename TInfo = CNiflectType>
		void RegisterType3(const Niflect::CString& id, const CreateTypeAccessorFunc& inCreateTypeAccessorFunc, const CSharedNata& nata)
		{
			STypeLifecycleMeta lifecycleMeta;
			lifecycleMeta.m_typeSize = sizeof(TType);
			lifecycleMeta.m_InvokeConstructorFunc = &GenericInstanceInvokeConstructor<TType>;
			lifecycleMeta.m_InvokeDestructorFunc = &GenericInstanceInvokeDestructor<TType>;

			auto shared = Niflect::MakeShared<TInfo>();
			CNiflectType* type = shared.Get();
			auto idx = this->GetTypesCount();
			ASSERT(!TRegisteredType<TType>::IsValid());
			type->InitTypeMeta2(lifecycleMeta, inCreateTypeAccessorFunc, CNiflectType::GetTypeHash<TType>(), idx, id, &TRegisteredType<TType>::s_type, nata);
			ASSERT(TRegisteredType<TType>::IsValid());
			this->InsertType(shared, idx);
		}
		uint32 GetTypesCount() const
		{
			return static_cast<uint32>(m_vecType.size());
		}
		CNiflectType* GetTypeByIndex(uint32 idx) const
		{
			return m_vecType[idx].Get();
		}
		CNiflectType* FindTypeByTypeName(const Niflect::CString& id) const
		{
			auto itFound = m_mapIdToIndex.find(id);
			if (itFound != m_mapIdToIndex.end())
				return m_vecType[itFound->second].Get();
			return NULL;
		}
		void InitTypesLayout() const
		{
			for (auto& it : m_vecType)
				it->InitTypeLayout();
		}
		void InsertType(const CSharedNiflectType& type, uint32 idx)
		{
			auto ret = m_mapIdToIndex.insert({ type->GetTypeName(), idx });
			ASSERT(ret.second);
			m_vecType.insert(m_vecType.begin() + idx, type);
		}

	public:
		void DeleteType(const CNiflectType* type)//����
		{
			auto itFound = m_mapIdToIndex.find(type->GetTypeName());
			ASSERT(itFound != m_mapIdToIndex.end());
			m_vecType.erase(m_vecType.begin() + itFound->second);
			m_mapIdToIndex.erase(itFound);
		}

	private:
		TArrayNif<CSharedNiflectType> m_vecType;
		TUnorderedMap<Niflect::CString, uint32> m_mapIdToIndex;
		CString m_name;
	};
	using CSharedTable = TSharedPtr<CNiflectTable>;
	
	template <typename TInfo, typename TType>
	class TStaticTableTypeReg
	{
	public:
		TStaticTableTypeReg(CNiflectTable* table, const CString& typeName, const CreateFieldLayoutOfTypeFuncOld& Func)
		{
			table->RegisterType<TInfo, TType>(typeName, Func);
		}
		template <typename TNatimeta>
		TStaticTableTypeReg(CNiflectTable* table, const CString& typeName, const CreateFieldLayoutOfTypeFuncOld& Func, const TNatimeta& natimeta)
			: TStaticTableTypeReg(table, typeName, Func)
		{
			auto type = StaticGetType<TType>();
			type->SetNatimeta(MakeShared<TNatimeta>(natimeta));
		}
	};

	template <typename TType, typename TInfo>
	class TStaticTypeRegger
	{
	public:
		//TStaticTypeRegger(CNiflectTable* table, const Niflect::CString& id, const InvokeStaticCreateFieldLayoutFunc& Func)
		//{
		//	table->RegisterType2<TType, TInfo>(id, Func);
		//}
		template <typename TNata>
		TStaticTypeRegger(CNiflectTable* table, const Niflect::CString& id, const CreateTypeAccessorFunc& Func, const TNata& nata)
			//: TStaticTypeRegger(table, id, Func)
		{
			table->RegisterType2<TType, TInfo>(id, Func);
			if (CNiflectType::GetTypeHash<TNata>() != CNiflectType::GetTypeHash<CNata>())
			{
				auto type = StaticGetType<TType>();
				type->InitNata(Niflect::MakeShared<TNata>(nata));
			}
		}
	};

	template <typename TAccessor>
	static CSharedAccessor __InternalCreateFieldLayoutForFunctionPointer()
	{
		return MakeShared<TAccessor>();
	}

#define NIFLECT_MEMBER_FUNCTION_GET_TYPE_VIRTUAL()\
	public:\
		virtual Niflect::CNiflectType* GetType() const { return NULL; }

#define NIFLECT_MEMBER_FUNCTION_GET_TYPE_OVERRIDE(typeName)\
	public:\
		virtual Niflect::CNiflectType* GetType() const override\
		{\
			return Niflect::StaticGetType<typeName>();\
		}
	
//begin, ����������, ����Ҫ���Ǿ�̬��������ͻ, ��Ϊ���Ͳ�����ͬnamespace�г���ͬ������
//#define NIFLECT_STATIC_TYPE_REG_VAR_NAME_COMBINE_INNER(A, B) s_typeReg_##A##_##B
//#define NIFLECT_STATIC_TYPE_REG_VAR_NAME_COMBINE(A, B) NIFLECT_STATIC_TYPE_REG_VAR_NAME_COMBINE_INNER(A, B)
//#define NIFLECT_STATIC_TYPE_REG_VAR_NAME(typeName) NIFLECT_STATIC_TYPE_REG_VAR_NAME_COMBINE(typeName, __LINE__)
//end

#define DEPRECATED_NIFLECT_REGISTER(niflectType, typeName, invokeGetTable, customDataObject)\
	static Niflect::TStaticTableTypeReg<niflectType, typeName> s_typeReg_##typeName(invokeGetTable, #typeName, customDataObject);

#define NIFLECT_TYPE_REGISTER(typeName, niflectType, invokeGetTable, staticCreateFieldLayoutFuncAddr, nataType, nataObj)\
	static Niflect::TStaticTypeRegger<typeName, niflectType> s_typeReg_##typeName(invokeGetTable, #typeName, staticCreateFieldLayoutFuncAddr, nataType(nataObj))

#define NIFLECT_CLASS_REGISTER(typeName, invokeGetTable, staticCreateFieldLayoutFuncAddr, nataType, nataObj)\
	NIFLECT_TYPE_REGISTER(typeName, Niflect::CClass, invokeGetTable, staticCreateFieldLayoutFuncAddr, nataType, nataObj)
}