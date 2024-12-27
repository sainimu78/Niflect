#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/NiflectAccessor.h"
#include "Engine/Test/TestMyTransform.h"

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	using CMyTM = TestAccessor2::TMyTransform<float>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;//failed
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, float>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, float>;//failed
	using CMyFloat = float;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CMyFloat>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CMyFloat>;//failed
	template <typename T0, typename T1>
	class TSomeBinaryType
	{
	};
	template <typename T0, typename T1>
	class TSomeBinaryTypeAccessor : public Niflect::CAccessor
	{
	};
	template <typename T0, typename T1>
	NIF_AS_A() TSetting<TSomeBinaryTypeAccessor<T0, T1>, TSomeBinaryType<T0, T1> >;
	template <typename T>
	using TPartiallySpecializedAlias = TSomeBinaryTypeAccessor<float, T>;
	template <typename T>
	NIF_AS_A() TSetting<TPartiallySpecializedAlias<T>, TSomeBinaryType<float, T> >;//failed, 不支持部分特化的模板
}