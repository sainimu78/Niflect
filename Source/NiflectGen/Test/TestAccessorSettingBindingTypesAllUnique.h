#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/NiflectAccessor.h"
#include "Engine/Test/TestMyTransform.h"

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	using CMyTM = TestAccessor2::TMyTransform<float>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CMyTM>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<double> >;
	using CMyFloat = float;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, float>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CMyFloat>;
	using CMyTM2 = TestAccessor2::TMyTransform<CMyFloat>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CMyTM2>;
	template <typename T>
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<T>, TestAccessor2::TMyTransform<T> >;
	template <typename T>
	using TMyTM = TestAccessor2::TMyTransform<T>;
	template <typename T>
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<T>, TMyTM<T> >;
	template <typename T0, typename T1>
	class TSomeBinaryType
	{
	};
	template <typename T0, typename T1>
	class TSomeBinaryTypeAccessor : public Niflect::CAccessor
	{
	};
	template <typename T0, typename T1>
	NIFAS_A() TSetting<TSomeBinaryTypeAccessor<T0, T1>, TSomeBinaryType<T0, T1> >;
}