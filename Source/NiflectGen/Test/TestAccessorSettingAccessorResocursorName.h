#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/Default/CompoundAccessor.h"
#include "Niflect/Default/ContainerTypeAccessor.h"
#include "Engine/Test/TestMyTransform.h"

namespace MyScope2
{
	template <typename T>
	using TMyScope2TM = TestAccessor2::TMyTransform<T>;

	namespace SubScope1
	{
		template <typename T>
		using TMySubScope1TM = TestAccessor2::TMyTransform<T>;
	}

	struct SMyStruct
	{
		int32 m_int_0;
	};
}

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;
	using namespace TestAccessor2;
	using namespace MyScope2;
	using namespace SubScope1;
	using namespace std;

	NIFAS_A() TSetting<TMyTransformAccessor<float>, TMyTransform<float> >;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TMyTransform<double> >;
	NIFAS_A() TSetting<TMyTransformAccessor<float>, TMyScope2TM<int32> >;
	NIFAS_A() TSetting<TMyTransformAccessor<float>, TMySubScope1TM<uint32> >;
	NIFAS_A() TSetting<TMyTransformAccessor<float>, SubScope1::TMySubScope1TM<int64> >;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TArray<SMyStruct> >;
	template <typename TInstance, typename T>
	NIFAS_A() TSetting<TArrayAccessor<TInstance>, TArray<T> >;
	template <typename T0, typename T1>
	NIFAS_A() TSetting<CCompoundAccessor, pair<T0, T1> >;
}