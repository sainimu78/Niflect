#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/Default/ContainerTypeAccessor.h"
#include "Niflect/Default/ValueTypeAccessor.h"
#include "Niflect/Default/STLValueTypeAccessor.h"
#include "Engine/Test/TestMyTransform.h"
#include "NiflectGen/Test/TestModuleHeaderRequiredHeader.h"

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	template <typename TInstance, typename T>
	NIF_AS_A() TSetting<Niflect::TArrayAccessor<TInstance>, MyTestType::TMyArray<T> >;
	NIF_AS_A() TSetting<Niflect::CFloatAccessor, float>;
	NIF_AS_A() TSetting<Niflect::CStringAccessor, CString>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;
	NIF_AS_A() TSetting<Niflect::CSTLStringAccessor, std::string>;
}