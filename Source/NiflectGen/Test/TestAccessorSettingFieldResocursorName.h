#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/CommonlyUsed/CompoundAccessor.h"
#include "Niflect/CommonlyUsed/ValueTypeAccessor.h"
#include "Niflect/CommonlyUsed/ContainerTypeAccessor.h"
#include "Niflect/CommonlyUsed/STLValueTypeAccessor.h"
#include "Niflect/CommonlyUsed/STLContainerTypeAccessor.h"
#include "Engine/Test/TestMyTransform.h"
#include "Engine/Test/TestMyResource.h"
#include "Engine/Test/TestMyAsset.h"

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	NIF_AS_A() TSetting<CBoolAccessor, bool>;
	NIF_AS_A() TSetting<CFloatAccessor, float>;
	NIF_AS_A() TSetting<CStringAccessor, Niflect::CString>;
	NIF_AS_A() TSetting<CSTLStringAccessor, std::string>;
	template <typename TInstance, typename T>
	NIF_AS_A() TSetting<TArrayAccessor<TInstance>, Niflect::TArray<T> >;
	NIF_AS_A() TSetting<CBitsArrayAccessor, Niflect::TArray<bool> >;
	template <typename TInstance, typename T0, typename T1>
	NIF_AS_A() TSetting<TMapAccessor<TInstance>, Niflect::TMap<T0, T1>, std::pair<T0, T1> >;
	template <typename T0, typename T1>
	NIF_AS_A() TSetting<CCompoundAccessor, std::pair<T0, T1> >;
	NIF_AS_A() TSetting<CBitsArrayAccessor, std::vector<bool> >;
	template <typename TInstance, typename T>
	NIF_AS_A() TSetting<TSTLArrayAccessor<TInstance>, std::vector<T> >;
	NIF_AS_A() TSetting<TBuiltinTypeAccessor<int32>, int32>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;
	NIF_AS_A() TSetting<TestMyScope::CMyAssetAccessor, TestMyScope::CMyAsset*>;
	NIF_AS_A() TSetting<TestMyScope::CMyResourceAccessor, TestMyScope::CMyResource*>;
	template <typename TInstance, typename T>
	NIF_AS_E() TSetting<TestMyScope::TSharedMyResourceAccessor<TInstance>, TestMyScope::TSharedMyResource<T> >;
}