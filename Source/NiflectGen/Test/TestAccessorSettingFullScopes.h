#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/NiflectAccessor.h"
#include "Engine/Test/TestMyTransform.h"
#include "NiflectGen/NiflectGenDevelopmentMacro.h"

namespace TestGenMyScope
{
	using MyAliasInt8_0 = int8;
	using MyAliasInt8_1 = int8;
	using MyAliasInt8_2 = int8;
	typedef int8 MyAliasInt8_3;
	typedef int8 MyAliasInt8_4;

	namespace SubScope
	{
		using SubMyAliasInt8_0 = int8;
	}

	class CSub_0
	{
	public:
		class CSubSub_0
		{
		};
	};

	class CSub_1
	{
	public:
		class CSubSub_0
		{
		};
	};

	template <typename T>
	class TSub_2
	{
	public:
		class CSubSub_0
		{

		};
	};

	using MyAliasSub_0 = CSub_0;
	using MyAliasSub_1 = Niflect::TArray<float>;
	typedef Niflect::TArray<float> MyAliasSub_2;
}

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;
	using namespace TestAccessor2;

	NIFAS_A() TSetting<TMyTransformAccessor<float>, TMyTransform<float> >;

	using CMyTM = TestAccessor2::TMyTransform<float>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CMyTM>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TArray<float> >;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TArray<EngineTypeBindingSettingScope::CMyTM> >;

	using namespace TestGenMyScope;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestGenMyScope::MyAliasInt8_0>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, MyAliasInt8_1>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, Niflect::TMap<Niflect::CString, CMyTM> >;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TMap<float, CMyTM> >;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TMap<bool, EngineTypeBindingSettingScope::CMyTM> >;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TMap<float, bool> >;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TMap<std::string, bool> >;

	using namespace std;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, map<string, int8> >;

	using namespace TestAccessor2;
	NIFAS_A() TSetting<TMyTransformAccessor<float>, MyAliasInt8_2>;

	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestGenMyScope::SubScope::SubMyAliasInt8_0>;

#ifdef CLASS_TYPE_SCOPE
	//流程缺陷无法区分类Scope与模板定义
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestGenMyScope::CSub_0::CSubSub_0>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CSub_1::CSubSub_0>;
#else
#endif
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, MyAliasInt8_3>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestGenMyScope::MyAliasInt8_4>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, MyAliasSub_0>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, MyAliasSub_1>;
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, MyAliasSub_2>;
#ifdef TEMPLATE_INSTANCE_SCOPE
	NIFAS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TSub_2<float>::CSubSub_0>;//不支持模板实例的Scope
#else
#endif
}