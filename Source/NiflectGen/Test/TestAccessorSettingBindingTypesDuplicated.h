#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/NiflectAccessor.h"
#include "Engine/Test/TestMyTransform.h"

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	using CMyTM = TestAccessor2::TMyTransform<float>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;//failed
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, float>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, float>;//failed
	using CMyFloat = float;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CMyFloat>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CMyFloat>;//failed
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
	template <typename T>
	using TPartiallySpecializedAlias = TSomeBinaryTypeAccessor<float, T>;
	template <typename T>
	NIFAS_A() TSetting<TPartiallySpecializedAlias<T>, TSomeBinaryType<float, T> >;//failed, 不支持部分特化的模板
}