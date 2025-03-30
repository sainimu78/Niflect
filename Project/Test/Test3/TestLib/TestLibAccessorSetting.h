#pragma once
#include "Niflect/Default/DefaultAccessorSetting.h"

using namespace NiflectAccessorSetting;
using namespace RwTree;

class CMyStringPointerAccessor : public Niflect::CAccessor
{
protected:
	virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
	{
		return false;
	}
	virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
	{
		return false;
	}
};

NIF_AS_A() TSetting<CMyStringPointerAccessor, Niflect::CString*>;