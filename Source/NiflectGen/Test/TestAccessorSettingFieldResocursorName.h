#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/Default/CompoundAccessor.h"
#include "Niflect/Default/ValueTypeAccessor.h"
#include "Niflect/Default/ContainerTypeAccessor.h"
#include "Niflect/Default/STLValueTypeAccessor.h"
#include "Niflect/Default/STLContainerTypeAccessor.h"
#include "Engine/Test/TestMyTransform.h"
#include "Engine/Test/TestMyResource.h"
#include "Engine/Test/TestMyAsset.h"

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	NIFAS_A() TSetting<CBoolAccessor, bool>;
	NIFAS_A() TSetting<CFloatAccessor, float>;
	NIFAS_A() TSetting<CStringAccessor, Niflect::CString>;
	NIFAS_A() TSetting<CSTLStringAccessor, std::string>;
	template <typename TInstance, typename T>
	NIFAS_A() TSetting<TArrayAccessor<TInstance>, Niflect::TArray<T> >;
	NIFAS_A() TSetting<CBitsArrayAccessor, Niflect::TArray<bool> >;
	template <typename TInstance, typename T0, typename T1>
	NIFAS_A() TSetting<TMapAccessor<TInstance>, Niflect::TMap<T0, T1>, std::pair<T0, T1> >;
	template <typename T0, typename T1>
	NIFAS_A() TSetting<CCompoundAccessor, std::pair<T0, T1> >;
	NIFAS_A() TSetting<CBitsArrayAccessor, std::vector<bool> >;
	template <typename TInstance, typename T>
	NIFAS_A() TSetting<TSTLArrayAccessor<TInstance>, std::vector<T> >;
	NIFAS_A() TSetting<TBuiltinTypeAccessor<int32>, int32>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;
	NIFAS_A() TSetting<TestMyScope::CMyAssetAccessor, TestMyScope::CMyAsset*>;
	NIFAS_A() TSetting<TestMyScope::CMyResourceAccessor, TestMyScope::CMyResource*>;
	template <typename TInstance, typename T>
	NIFAS_E() TSetting<TestMyScope::TSharedMyResourceAccessor<TInstance>, TestMyScope::TSharedMyResource<T> >;
	NIFAS_A() TSetting<TestMyScope::CMyResourcePtrPtrAccessor, TestMyScope::CMyResource**>;
}