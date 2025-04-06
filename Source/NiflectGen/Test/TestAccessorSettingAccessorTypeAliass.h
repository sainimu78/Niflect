#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/NiflectAccessor.h"
#include "Engine/Test/TestMyTransform.h"

namespace TestAccessor2
{
	using DDDDDDDDDDD = Niflect::CAccessor;
	using SSSSS = DDDDDDDDDDD;

	template <typename T>
	class TMSSSSSSSSSSSSSSSSS : public SSSSS
	{

	};
}

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestAccessor2::TMyTransform<float> >;
	using CNihao = TestAccessor2::TMyTransformAccessor<float>;
	NIFAS_A() TSetting<CNihao, int8>;
	template <typename T>
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<T>, int16>;
	template <typename T>
	using TMyAccessorAlias = TestAccessor2::TMSSSSSSSSSSSSSSSSS<T>;
	template <typename T>
	NIFAS_A() TSetting<TMyAccessorAlias<T>, int32>;

	template <typename T0, typename T1>
	class TSomeBinaryType
	{
	};
	template <typename T0, typename T1>
	class TSomeBinaryTypeAccessor : public Niflect::CAccessor
	{
	};
	template <typename T>
	using TPartiallySpecializedAlias = TSomeBinaryTypeAccessor<float, T>;

	template <typename T>
	NIFAS_A() TSetting<TSomeBinaryTypeAccessor<float, T>, TSomeBinaryType<float, T> >;
	template <typename T>
	NIFAS_A() TSetting<TPartiallySpecializedAlias<T>, TSomeBinaryType<float, bool> >;
}