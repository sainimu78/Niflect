#pragma once
#include "Niflect/Default/DefaultAccessorSetting.h"
#include "Engine/Test/TestMyTransform.h"
#include "TestModule1/TestResourceAccessor.h"

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;

	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;
	NIFAS_A() TSetting<TestModule1::CRawTestResourceAccessor, TestModule1::CTestResource*>;
	template <typename TInstance, typename T>
	NIFAS_E() TSetting<TestModule1::TRawTestResourcePointerAccessor<TInstance>, TestModule1::TTestResourcePointer<T> >;
	NIFAS_A() TSetting<TestModule1::CSharedTestResourceAccessor, TestModule1::CSharedTestResource>;
	NIFAS_A() TSetting<TestModule1::CRawTestResourcePtrPtrAccessor, TestModule1::CTestResource**>;
	NIFAS_A() TSetting<TestModule1::TSharedInstanceAccessor<TestModule1::CTestMyInstance>, TestModule1::CSharedTestMyInstance>;
}


//#pragma once
//#include "Niflect/NiflectAccessorSetting.h"
//#include "Niflect/CommonlyUsed/CompoundAccessor.h"
//#include "Niflect/CommonlyUsed/EnumAccessor.h"
//#include "Niflect/CommonlyUsed/ValueTypeAccessor.h"
//#include "Niflect/CommonlyUsed/ContainerTypeAccessor.h"
//#include "Niflect/CommonlyUsed/STLValueTypeAccessor.h"
//#include "Niflect/CommonlyUsed/STLContainerTypeAccessor.h"
//#include "Engine/Test/TestMyTransform.h"
//
//namespace DefaultAccessorSetting
//{
//	using namespace NiflectAccessorSetting;
//	using namespace Niflect;
//
//	NIFAS_A() TSetting<CBoolAccessor, bool>;
//	NIFAS_A() TSetting<CInt32Accessor, int32>;
//	NIFAS_A() TSetting<CFloatAccessor, float>;
//	NIFAS_A() TSetting<CStringAccessor, Niflect::CString>;
//	template <typename TInstance, typename T>
//	NIFAS_A() TSetting<TArrayAccessor<TInstance>, Niflect::TArrayNif<T> >;
//	template <typename TInstance>
//	NIFAS_A() TSetting<TBitsArrayAccessor<TInstance>, Niflect::TArrayNif<bool> >;
//	template <typename TInstance, typename T0, typename T1>
//	NIFAS_A() TSetting<TMapAccessor<TInstance>, Niflect::TMap<T0, T1>, std::pair<T0, T1> >;
//	template <typename T0, typename T1>
//	NIFAS_A() TSetting<Niflect::CCompoundAccessor, std::pair<T0, T1> >;
//	NIFAS_A() TSetting<CSTLStringAccessor, std::string>;
//	template <typename TInstance>
//	NIFAS_A() TSetting<TSTLBitsArrayAccessor<TInstance>, std::vector<bool> >;
//	template <typename TInstance, typename T>
//	NIFAS_A() TSetting<TSTLArrayAccessor<TInstance>, std::vector<T> >;
//	template <typename TInstance, typename T0, typename T1>
//	NIFAS_A() TSetting<TSTLMapAccessor<TInstance>, std::map<T0, T1>, std::pair<T0, T1> >;
//	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;
//	NIFAS_B() TSetting<CCompoundAccessor>;
//	NIFAS_C() TSetting<CEnumClassAccessor>;
//	NIFAS_D() TSetting<CEnumBitMaskAccessor>;
//}