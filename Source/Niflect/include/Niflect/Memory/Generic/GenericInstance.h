#pragma once
#include <new>//Placement new

namespace Niflect
{
	typedef void (*InvokeConstructorFunc)(void* obj);
	typedef void (*InvokeDestructorFunc)(void* obj);
	
	//template <typename TClass>
	//static void GenericInstanceInvokeConstructor(void* obj)
	//{
	//	new (obj) TClass();//Ϊ����built in����, ʹ��placement new���ù��캯��
	//}
	
	//��ģ�庯��ȡ��ַ, �ɼ����в������޲����ĺ���ָ������
	template <typename TClass, typename ...TArgs>
	inline static void GenericInstanceInvokeConstructor(void* obj, TArgs&& ...args)
	{
		new (obj) TClass(args...);
	}

	template <typename TClass>
	inline static void GenericInstanceInvokeDestructor(void* obj)
	{
		static_cast<TClass*>(obj)->~TClass();
	}
	
	class CGenericInstance
	{
	public:
		template <typename TMemory, typename TClass, typename ...TArgs>
		inline static TClass* New(TArgs&& ...args)
		{
			auto mem = TMemory::Alloc(sizeof(TClass));
			GenericInstanceInvokeConstructor<TClass>(mem, std::forward<TArgs>(args)...);
			return static_cast<TClass*>(mem);
		}
		template <typename TMemory, typename TClass>
		inline static void Delete(TClass* obj)
		{
			GenericInstanceInvokeDestructor<TClass>(obj);
			TMemory::Free(obj);
		}
		template <typename TMemory>
		inline static void DestructAndFree(void* obj, const InvokeDestructorFunc& Func)
		{
			Func(obj);
			TMemory::Free(obj);
		}
	};
}

//namespace Reflector
//{
//	//��ʵ�ֻ�����virtual����Ҳ�ܰ�ȫDelete����ָ��, ��ʵ���ϴ�����ԭ��Ϊ���벻�淶����, ������һ��ʵ������, �������Delete�����е�˵��
//	template <typename TMemory>
//	class CClassMemorySafe
//	{
//	private:
//		typedef void(*PlacementDeleteFunc)(void* obj);
//		struct SPlacementObjectInfo
//		{
//			PlacementDeleteFunc m_DeleteFunc;
//		};
//		template <typename TClass>
//		inline static void PlacementDeleteGeneric(void* obj)
//		{
//			auto casted = static_cast<TClass*>(obj);
//			casted->~TClass();
//		}
//
//	public:
//		template <typename TClass, typename ...TArgs>
//		static TClass* New(TArgs&& ...args)
//		{
//			const auto placementInfoSize = sizeof(SPlacementObjectInfo);
//			const auto objectSize = sizeof(TClass);
//			auto mem = static_cast<char*>(TMemory::Alloc(placementInfoSize + objectSize));
//			auto placementInfo = reinterpret_cast<SPlacementObjectInfo*>(mem);
//			placementInfo->m_DeleteFunc = &PlacementDeleteGeneric<TClass>;
//			TClass* obj = new (mem + placementInfoSize) TClass(args...);
//			return obj;
//		}
//		template <typename TClass>
//		static void Delete(TClass* obj)
//		{
//			if (obj == NULL)
//				return;
//			const auto placementInfoSize = sizeof(SPlacementObjectInfo);
//			auto mem = reinterpret_cast<char*>(obj) - placementInfoSize;
//			auto placementInfo = reinterpret_cast<SPlacementObjectInfo*>(mem);
//			//ģ�����TClass����Ϊ����, ����δ����override����ʱ, ��ʽ���û���������������಻����ȫ����, ��������������ʱ�ɹ۲쵽�����಻�ᱻ����
//			//���ͨ������ָ����New�׶μ�¼��ȷ������������delete�׶ε��ÿ�ʵ�ְ�ȫdelete placement new�Ķ���, ���ض������ඨ��override����
//			//ȱ�����ڶ����Ӻ���ָ��ռ�ռ��
//			placementInfo->m_DeleteFunc(obj);
//			TMemory::Free(mem);
//		}
//	};
//}