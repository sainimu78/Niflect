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
#include "Niflect/NiflectBase.h"
#include "Niflect/NiflectAddr.h"

namespace Niflect
{
	class CNiflectType;

	class CNiflectMethod2
	{
		using InstanceType = CAddrOffset::DummyType;
		using OffsetType = CAddrOffset::OffsetType;
		typedef void (*InvokeMethodFunc2)(InstanceType* obj, InstanceType** inputInstanceArray, InstanceType** ouputInstanceArray, const TArrayNif<CNiflectType*>& vecInputType, const TArrayNif<CNiflectType*>& vecOutputType);
	public:
		CNiflectMethod2()
			: m_InvokeFunc(NULL)
		{
		}
		void Invoke(InstanceType* obj, InstanceType** inputInstanceArray, InstanceType** ouputInstanceArray) const
		{
			m_InvokeFunc(obj, inputInstanceArray, ouputInstanceArray, m_vecInputType, m_vecOutputType);
		}
		TArrayNif<CNiflectType*> m_vecInputType;
		TArrayNif<CNiflectType*> m_vecOutputType;
		InvokeMethodFunc2 m_InvokeFunc;
	};
}