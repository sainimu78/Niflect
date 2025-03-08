#pragma once
#include "Niflect/NiflectAccessor.h"

namespace TestMyScope
{
	class CMyResource
	{

	};

	using namespace RwTree;

	class CMyResourceAccessor : public Niflect::CAccessor
	{
		//protected:
		//	virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		//	{
		//		return false;
		//	}
		//	virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		//	{
		//		return false;
		//	}
	};

	template <typename T>
	class TSharedMyResource
	{
	public:
		TSharedMyResource()
			: m_ptr(NULL)
		{

		}
		T* m_ptr;
	};

	template <typename TResource>
	class TSharedMyResourceAccessor : public Niflect::CAccessor
	{
		//protected:
		//	virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		//	{
		//		return false;
		//	}
		//	virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		//	{
		//		return false;
		//	}
	};

	class CMyResourcePtrPtrAccessor : public Niflect::CAccessor
	{
		//protected:
		//	virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		//	{
		//		return false;
		//	}
		//	virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		//	{
		//		return false;
		//	}
	};
}