//#pragma once
//#include "Niflect/Accessor/NiflectAccessor.h"
//
//namespace Niflect
//{
//	typedef void (*InvokeMethodFunc)(void** paramAddrArray, const TArrayNif<CSharedAccessor>& vecInput, const TArrayNif<CSharedAccessor>& vecOutput);
//
//	class CNiflectMethod
//	{
//	public:
//		CNiflectMethod()
//			: m_InvokeFunc(NULL)
//		{
//		}
//		void Invoke(void** paramAddrArray) const
//		{
//			m_InvokeFunc(paramAddrArray, m_vecInputAccessor, m_vecOutputAccessor);
//		}
//		TArrayNif<CSharedAccessor> m_vecOutputAccessor;
//		TArrayNif<CSharedAccessor> m_vecInputAccessor;
//		InvokeMethodFunc m_InvokeFunc;
//	};
//}

#pragma once
#include "Niflect/Base/Array.h"
#include "Niflect/NiflectAddr.h"

namespace Niflect
{
	class CNiflectType;

	class CNiflectMethod
	{
		typedef void (*InvokeMethodFunc)(InstanceType* obj, InstanceType** inputInstanceArray, InstanceType** ouputInstanceArray, const TArray<CNiflectType*>& vecInputType, const TArray<CNiflectType*>& vecOutputType);
	public:
		CNiflectMethod()
			: m_InvokeFunc(NULL)
		{
		}
		inline void Invoke(InstanceType* obj, InstanceType** inputInstanceArray, InstanceType** ouputInstanceArray) const
		{
			m_InvokeFunc(obj, inputInstanceArray, ouputInstanceArray, m_vecInputType, m_vecOutputType);
		}
		TArray<CNiflectType*> m_vecInputType;
		TArray<CNiflectType*> m_vecOutputType;
		InvokeMethodFunc m_InvokeFunc;
	};
}