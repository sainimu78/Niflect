#pragma once
#include "Niflect/NiflectAccessor.h"

namespace TestMyScope
{
	class CMyAsset
	{

	};

	using namespace RwTree;

	class CMyAssetAccessor : public Niflect::CAccessor
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