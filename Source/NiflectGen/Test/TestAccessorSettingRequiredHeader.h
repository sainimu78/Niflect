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
	NIFAS_A() TSetting<Niflect::TArrayAccessor<TInstance>, MyTestType::TMyArray<T> >;
	NIFAS_A() TSetting<Niflect::CFloatAccessor, float>;
	NIFAS_A() TSetting<Niflect::CStringAccessor, CString>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;
	NIFAS_A() TSetting<Niflect::CSTLStringAccessor, std::string>;
}